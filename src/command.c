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
#include "ext/standard/url.h"
#include "ext/standard/php_http.h"
#include "ext/standard/php_smart_str.h"

#ifndef HAVE_PHP_GROONGA_H
#   include "php_groonga.h"
#endif

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/**
 * GQTP接続フラグ
 *
 * @var int 0:偽 1:真
 */
int gqtpConnected = 0;

/**
 * GQTPコマンドの生成
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param zval       *command 文字列オブジェクト
 * @param const char *name    コマンド名
 * @return int       0:偽 1:真
 */
int prngqtp_command(grn_ctx *ctx, zval *command, const char *name TSRMLS_DC)
{
    ZVAL_STRING(command, name, 1);
    return 1;
}

/**
 * GQTPコマンドへ変数設定
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param zval       *param   配列オブジェクト
 * @param const char *key     引数名
 * @param const char *value   値
 * @return int       0:偽 1:真
 */
int prngqtp_command_set(grn_ctx *ctx, zval *param, const char *key, const char *value TSRMLS_DC)
{
    zval *zvalue = NULL;

    MAKE_STD_ZVAL(zvalue);
    ZVAL_STRING(zvalue, value, 0);

    zend_hash_update(Z_ARRVAL_P(param), key, strlen(key), (void **)&zvalue, sizeof(zval*), NULL);

    return 1;
}

/**
 * GQTPコマンドへ変数設定
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param zval       *param   配列オブジェクト
 * @param const char *key     引数名
 * @param zval       *retval  値
 * @return int       0:偽 1:真
 */
int prngqtp_command_get(grn_ctx *ctx, zval *param, const char *key, zval *retval TSRMLS_DC)
{
    zval *zvalue = NULL;
     
    zend_hash_find(Z_ARRVAL_P(param), key, strlen(key), (void **)&zvalue);

    ZVAL_STRINGL(retval, Z_STRVAL_P(zvalue), Z_STRLEN_P(zvalue), 0);

    return 1;
}

/**
 * GQTPコマンドの実行
 *
 * @param grn_ctx   *ctx       コンテキスト
 * @param zval      *command   文字列オブジェクト
 * @param zval      *param     配列オブジェクト
 * @param zval      *retval    結果情報
 * @param int       assoc      結果フォーマット
 * @return int      0:偽 1:真
 */
int prngqtp_command_exec(grn_ctx *ctx, zval *zcommand, zval *zparam, zval *retval, int assoc TSRMLS_DC)
{
    smart_str query = {0}, param = {0};
    char *arg_sep = "&", *res = NULL;
    uint length;
    long enc_type = PHP_QUERY_RFC1738;
    long flags = 0, recv_flags;

    smart_str_appendl(&query, "/d/", strlen("/d/"));
    smart_str_appendl(&query, Z_STRVAL_P(zcommand), Z_STRLEN_P(zcommand));
    smart_str_appendl(&query, ".json", strlen(".json"));

    if (php_url_encode_hash_ex(HASH_OF(zparam), &param, NULL, 0, NULL, 0, NULL, 0, (Z_TYPE_P(zparam) == IS_OBJECT ? zparam : NULL), arg_sep, enc_type TSRMLS_CC) == FAILURE) {
        if (param.c) {
            efree(param.c);
        }
    } else {
        smart_str_appendc(&query, "?");
        smart_str_appendl(&query, param.c, param.len);
    }

    grn_ctx_send(ctx, query.c, query.len, flags);
    if (GRN_SUCCESS == ctx->rc) {
        grn_ctx_recv(ctx, &res, &length, &recv_flags);
    }

    if (param.c) {
        efree(param.c);
    }
    if (query.c) {
        efree(query.c);
    }

    if (GRN_SUCCESS != ctx->rc) {
        return 0;
    }

    if (NULL == res) {
        return 0;
    }

    if (assoc) {
        php_json_decode_ex(retval, res, length, PHP_JSON_OBJECT_AS_ARRAY, JSON_PARSER_GROONGA_DEPTH TSRMLS_CC);
    } else {
        ZVAL_STRINGL(retval, res, length, 1);
    }

    return 1;
}

/**
 * メモリの開放
 *
 * @param grn_ctx   *ctx       コンテキスト
 * @param zval      *command   文字列オブジェクト
 * @param zval      *param     配列オブジェクト
 * @return int      0:偽 1:真
 */
int prngqtp_command_unlink(zval *zcommand, zval *zparam TSRMLS_DC)
{
    return 1;
}


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
 * @param zval      *retval  結果情報
 * @param int       assoc    結果フォーマット
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
