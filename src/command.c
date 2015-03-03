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
#include "zend_interfaces.h"

#ifndef HAVE_PHP_GROONGA_H
#   include "php_groonga.h"
#endif

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/**
 * GQTPコマンドの生成
 *
 * @param prn_cmd    *cmd     文字列オブジェクト
 * @param const char *name    コマンド名
 * @return int       0:偽 1:真
 */
int prngqtp_command(prn_cmd *cmd, const char *name TSRMLS_DC)
{
    cmd->cmdname = (char *)emalloc(strlen(name) + 1);
    strcpy(cmd->cmdname, name);

    ALLOC_INIT_ZVAL(cmd->zparam);
    array_init(cmd->zparam);

    return 1;
}


/**
 * GQTPコマンドへ変数設定
 *
 * @param prn_cmd    *cmd     文字列オブジェクト
 * @param const char *key     引数名
 * @param zval       *retval  値
 * @return int       0:偽 1:真
 */
int prngqtp_command_get(prn_cmd *cmd, const char *key, zval **retval_pp TSRMLS_DC)
{
    if (SUCCESS != zend_hash_find(Z_ARRVAL_P(cmd->zparam), key, strlen(key) + 1, (void **) retval_pp)) {
        return 0;
    }
    return 1;
}

/**
 * GQTPコマンドの実行
 *
 * @param grn_ctx *ctx        コンテキスト
 * @param prn_cmd *cmd        文字列オブジェクト
 * @param int     assoc       結果フォーマット
 * @param zval    **retval_pp 値
 * @return int    0:偽 1:真
 */
int prngqtp_command_exec(grn_ctx *ctx, prn_cmd *cmd, int assoc, zval **retval_pp TSRMLS_DC)
{
    smart_str query = {0}, param = {0};
    char *arg_sep = "&", *response = NULL;
    uint length;
    long enc_type = PHP_QUERY_RFC1738;
    long flags = 0;

    smart_str_appendl(&query, "/d/", strlen("/d/"));
    smart_str_appendl(&query, cmd->cmdname, strlen(cmd->cmdname));
    smart_str_appendl(&query, ".json", strlen(".json"));

    if (php_url_encode_hash_ex(HASH_OF(cmd->zparam), &param, NULL, 0, NULL, 0, NULL, 0, (Z_TYPE_P(cmd->zparam) == IS_OBJECT ? cmd->zparam : NULL), arg_sep, enc_type TSRMLS_CC) == SUCCESS) {
        smart_str_appendc(&query, '?');
        smart_str_appendl(&query, param.c, param.len);
    }

    grn_ctx_send(ctx, query.c, query.len, flags);
    if (GRN_SUCCESS == ctx->rc) {
        int recv_flags;
        grn_ctx_recv(ctx, &response, &length, &recv_flags);
    }
    smart_str_free(&param);
    smart_str_free(&query);

    if (GRN_SUCCESS != ctx->rc) {
        return 0;
    }

    if (NULL == response) {
        return 0;
    }

    if (NULL != retval_pp) {
        zval *retval = NULL;

        ALLOC_INIT_ZVAL(retval);
        *retval_pp = retval;

        if (assoc) {
            php_json_decode_ex(
                retval, 
                response, 
                length, 
                PHP_JSON_OBJECT_AS_ARRAY, 
                JSON_PARSER_GROONGA_DEPTH TSRMLS_CC
            );
            if (!zend_is_true(retval)) {
                ZVAL_STRINGL(retval, response, length, 1);
            }
        } else {
            ZVAL_STRINGL(retval, response, length, 1);
        }
    } // if (NULL != retval_pp)

    return 1;
}

/**
 * Groongaコマンドの生成
 *
 * @param grn_ctx    *ctx      コンテキスト
 * @param prn_cmd    *cmd      コマンドオブジェクト
 * @param const char *name     コマンド名
 * @return int       0:偽 1:真
 */
int proonga_command(grn_ctx *ctx, prn_cmd *cmd, const char *name TSRMLS_DC)
{
    cmd->command = grn_ctx_get(ctx, name, strlen(name));
    if (NULL == cmd->command) {
        return 0;
    }
    return 1;
}

/**
 * Groongaコマンドへ変数設定
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param prn_cmd    *cmd     コマンドオブジェクト
 * @param const char *key     引数名
 * @param const char *value   値
 * @return int       0:偽 1:真
 */
int proonga_command_set(grn_ctx *ctx, prn_cmd *cmd, const char *key, const char *value TSRMLS_DC)
{
    /* 変数を取得 */
    grn_obj *expr_var = grn_expr_get_var(ctx, cmd->command, key, strlen(key));
    if (NULL == expr_var) {
        grn_obj_unlink(ctx, cmd->command);
        return 0;
    }

    /* expr_varの再初期化 */
    if (GRN_SUCCESS != grn_obj_reinit(ctx, expr_var, GRN_DB_TEXT, 0)) {
        grn_obj_unlink(ctx, cmd->command);
        return 0;
    }

    /* 変数へ文字列を設定 */
    if (GRN_SUCCESS != GRN_TEXT_PUTS(ctx, expr_var, value)) {
        grn_obj_unlink(ctx, cmd->command);
        return 0;
    }

    return 1;
}

/**
 * Groongaコマンドへ変数設定
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param prn_cmd    *cmd      文字列オブジェクト
 * @param const char *key     引数名
 * @param zval       *retval  値
 * @return int       0:偽 1:真
 */
int proonga_command_get(grn_ctx *ctx, prn_cmd *cmd, const char *key, zval **retval_pp TSRMLS_DC)
{
    zval *retval = NULL;

    /* 変数を取得 */
    grn_obj *expr_var = grn_expr_get_var(ctx, cmd->command, key, strlen(key));
    if (NULL == expr_var) {
        grn_obj_unlink(ctx, cmd->command);
        cmd->command = NULL;
        return 0;
    }

    ALLOC_INIT_ZVAL(retval);
    *retval_pp = retval;
    ZVAL_STRINGL(retval, GRN_TEXT_VALUE(expr_var), (int)GRN_TEXT_LEN(expr_var), 1);

    return 1;
}

/**
 * Groongaコマンドの実行
 *
 * @param grn_ctx *ctx        コンテキスト
 * @param prn_cmd *cmd        コマンドオブジェクト
 * @param int     assoc       結果フォーマット
 * @param zval    **retval_pp 結果情報
 * @return int    0:偽 1:真
 */
int proonga_command_exec(grn_ctx *ctx, prn_cmd *cmd, int assoc, zval **retval_pp TSRMLS_DC)
{
    grn_ctx_info info;

    /* コマンドの実行 */
    if (GRN_SUCCESS != grn_expr_exec(ctx, cmd->command, 0)) {
        grn_obj_unlink(ctx, cmd->command);
        return 0;
    }

    /* 実行結果の取得 */
    if (GRN_SUCCESS != grn_ctx_info_get(ctx, &info)) {
        grn_obj_unlink(ctx, cmd->command);
        return 0;
    }

    if (NULL != retval_pp) {
        zval *retval = NULL;

        ALLOC_INIT_ZVAL(retval);
        *retval_pp = retval;

        /* 出力タイプ分岐 */
        if (assoc) {
            php_json_decode_ex(
                retval, 
                GRN_TEXT_VALUE(info.outbuf), 
                (int)GRN_TEXT_LEN(info.outbuf), 
                PHP_JSON_OBJECT_AS_ARRAY, 
                JSON_PARSER_GROONGA_DEPTH TSRMLS_CC
            );

            if (!zend_is_true(retval)) {
                ZVAL_STRINGL(retval, GRN_TEXT_VALUE(info.outbuf), (int)GRN_TEXT_LEN(info.outbuf), 0);
            }
        } else {
            ZVAL_STRINGL(retval, GRN_TEXT_VALUE(info.outbuf), (int)GRN_TEXT_LEN(info.outbuf), 1);
        }
    } // if (NULL != retval_pp)

    /* コマンド結果のクリア */
    GRN_BULK_REWIND(info.outbuf);

    /* コマンド引数のリセット */
    grn_expr_clear_vars(ctx, cmd->command);

    return 1;
}

/**
 * コマンドの生成
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param prn_cmd    *cmd     コマンドオブジェクト
 * @param const char *name    コマンド名
 * @return int      0:偽 1:真
 */
int prn_command(grn_ctx *ctx, prn_cmd *cmd, const char *name)
{
    if (GROONGA_G(gqtpConnected) == 1) {
        return prngqtp_command(cmd, name TSRMLS_CC);
    } else {
        return proonga_command(ctx, cmd, name TSRMLS_CC);
    }
}

/**
 * コマンドの生成
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param prn_cmd    *cmd     コマンドオブジェクト
 * @param const char *key     引数名
 * @param const char *value   値
 * @return int      0:偽 1:真
 */
int prn_command_set(grn_ctx *ctx, prn_cmd *cmd, const char *key, char *value)
{
    if (GROONGA_G(gqtpConnected) == 1) {
        return add_assoc_string(cmd->zparam, key, value, 1);
    } else {
        return proonga_command_set(ctx, cmd, key, value TSRMLS_CC);
    }
}

/**
 * コマンドへ変数設定
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param prn_cmd    *cmd     コマンドオブジェクト
 * @param const char *name    コマンド名
 * @return int      0:偽 1:真
 */
int prn_command_get(grn_ctx *ctx, prn_cmd *cmd, const char *key, zval **retval_pp)
{
    if (GROONGA_G(gqtpConnected) == 1) {
        return prngqtp_command_get(cmd, key, retval_pp TSRMLS_CC);
    } else {
        return proonga_command_get(ctx, cmd, key, retval_pp TSRMLS_CC);
    }
}

/**
 * コマンドの実行
 *
 * @param grn_ctx  *ctx     コンテキスト
 * @param prn_cmd  *cmd     コマンドオブジェクト
 * @param int      *assoc   コマンド名
 * @return int      0:偽 1:真
 */
int prn_command_exec(grn_ctx *ctx, prn_cmd *cmd, int assoc, zval **retval_pp)
{
    int result = 0;

    if (GROONGA_G(gqtpConnected) == 1) {
        result = prngqtp_command_exec(ctx, cmd, assoc, retval_pp TSRMLS_CC);
    } else {
        result = proonga_command_exec(ctx, cmd, assoc, retval_pp TSRMLS_CC);
    }
    return result;
}

/**
 * メモリの開放
 *
 * @param grn_ctx    *ctx     コンテキスト
 * @param prn_cmd    *cmd     コマンドオブジェクト
 * @return int      0:偽 1:真
 */
int prn_command_unlink(grn_ctx *ctx, prn_cmd *cmd)
{
    if (GROONGA_G(gqtpConnected) == 1) {
        if (NULL != cmd->zparam) {
            zval_ptr_dtor(&cmd->zparam);
            cmd->zparam = NULL;
        }
        if (NULL != cmd->cmdname) {
            efree(cmd->cmdname);
        }
    } else {
        grn_obj_unlink(ctx, cmd->command);
        cmd->command = NULL;
    }
    return 1;
}
#endif      /* #ifndef HAVE_PROONGA_COMMAND */
