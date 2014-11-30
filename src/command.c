/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2011-2014 sheeps.me
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @package         proonga
 * @copyright       Copyright (c) 2011-2014 sheeps.me
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 * @filesource
 */

#ifndef HAVE_PROONGA_COMMAND
#define HAVE_PROONGA_COMMAND

#include <groonga.h>
#include "php.h"
#include "ext/json/php_json.h"

#ifndef HAVE_PHP_GROONGA_H
#   include "php_groonga.h"
#endif

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/**
 * Groongaコマンドの生成
 *
 * @param grn_ctx    *ctx      コンテキスト
 * @param grn_obj    **command コマンドオブジェクト
 * @param const char *name     コマンド名
 * @return int       0:偽 1:真
 */
int proonga_command(grn_ctx *ctx, grn_obj **command, const char *name TSRMLS_DC)
{
    *command = grn_ctx_get(ctx, name, strlen(name));
    if (NULL == *command) {
        return 0;
    }
    return 1;
}

/**
 * Groongaコマンドへ変数設定
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param grn_obj    *command コマンドオブジェクト
 * @param const char *key     引数名
 * @param const char *value   値
 * @return int       0:偽 1:真
 */
int proonga_command_set(grn_ctx *ctx, grn_obj *command, const char *key, const char *value TSRMLS_DC)
{
    /* 変数を取得 */
    grn_obj *expr_var = grn_expr_get_var(ctx, command, key, strlen(key));
    if (NULL == expr_var) {
        grn_obj_unlink(ctx, command);
        return 0;
    }

    /* expr_varの再初期化 */
    if (GRN_SUCCESS != grn_obj_reinit(ctx, expr_var, GRN_DB_TEXT, 0)) {
        grn_obj_unlink(ctx, command);
        return 0;
    }

    /* 変数へ文字列を設定 */
    if (GRN_SUCCESS != GRN_TEXT_PUTS(ctx, expr_var, value)) {
        grn_obj_unlink(ctx, command);
        return 0;
    }

    return 1;
}

/**
 * Groongaコマンドへ変数設定
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param grn_obj    *command コマンドオブジェクト
 * @param const char *key     引数名
 * @param zval       *retval  値
 * @return int       0:偽 1:真
 */
int proonga_command_get(grn_ctx *ctx, grn_obj *command, const char *key, zval *retval TSRMLS_DC)
{
    /* 変数を取得 */
    grn_obj *expr_var = grn_expr_get_var(ctx, command, key, strlen(key));
    if (NULL == expr_var) {
        grn_obj_unlink(ctx, command);
        return 0;
    }

    ZVAL_STRINGL(retval, GRN_TEXT_VALUE(expr_var), (int)GRN_TEXT_LEN(expr_var), 1);

    return 1;
}

/**
 * Groongaコマンドの実行
 *
 * @param grn_ctx   *ctx     コンテキスト
 * @param grn_obj   *command コマンドオブジェクト
 * @param zval      *retval    結果情報
 * @param zval      *retval    結果情報
 * @return int      0:偽 1:真
 */
int proonga_command_exec(grn_ctx *ctx, grn_obj *command, zval *retval, int assoc TSRMLS_DC)
{
    grn_ctx_info info;

    /* コマンドの実行 */
    if (GRN_SUCCESS != grn_expr_exec(ctx, command, 0)) {
        grn_obj_unlink(ctx, command);
        return 0;
    }

    /* 実行結果の取得 */
    if (GRN_SUCCESS != grn_ctx_info_get(ctx, &info)) {
        grn_obj_unlink(ctx, command);
        return 0;
    }

    if (NULL != retval) {
        /* 出力タイプ分岐 */
        if (assoc) {
            php_json_decode_ex(
                retval, 
                GRN_TEXT_VALUE(info.outbuf), 
                (int)GRN_TEXT_LEN(info.outbuf), 
                PHP_JSON_OBJECT_AS_ARRAY, 
                JSON_PARSER_GROONGA_DEPTH TSRMLS_CC
            );
        } else {
            ZVAL_STRINGL(retval, GRN_TEXT_VALUE(info.outbuf), (int)GRN_TEXT_LEN(info.outbuf), 1);
        }
    } // if (NULL != retval)

    /* コマンド結果のクリア */
    GRN_BULK_REWIND(info.outbuf);

    /* コマンド引数のリセット */
    grn_expr_clear_vars(ctx, command);

    return 1;
}

#endif      /* #ifndef HAVE_PROONGA_COMMAND */
