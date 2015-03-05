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

#ifndef HAVE_PROONGA_CLASS_GROONGA_H
#define HAVE_PROONGA_CLASS_GROONGA_H

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/**
 * Groongaクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    grn_ctx *ctx;
    grn_obj *db;
} groonga_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(Groonga, __construct);
PHP_METHOD(Groonga, __destruct);
PHP_METHOD(Groonga, query);
PHP_METHOD(Groonga, command);
PHP_METHOD(Groonga, status);
PHP_METHOD(Groonga, tableList);
PHP_METHOD(Groonga, cacheLimit);
PHP_METHOD(Groonga, dump);
PHP_METHOD(Groonga, table);

ZEND_BEGIN_ARG_INFO_EX(Groonga___construct, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, gobject)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga___destruct, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga_query, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, query)
    ZEND_ARG_INFO(0, assoc)
    ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga_command, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, command)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga_status, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga_tableList, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga_cacheLimit, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga_dump, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga_table, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, table)
ZEND_END_ARG_INFO()


extern zend_function_entry groonga_class_methods[];

#else
#   ifndef HAVE_PROONGA_CLASS_GROONGA
#   define HAVE_PROONGA_CLASS_GROONGA

/**
 * クラスの実装部分
 */
 
zend_function_entry groonga_class_methods[] = {
    PHP_ME(Groonga, __construct, Groonga___construct, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(Groonga, __destruct,  Groonga___destruct,  ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
    PHP_ME(Groonga, query,       Groonga_query,       ZEND_ACC_PUBLIC)
    PHP_ME(Groonga, command,     Groonga_command,     ZEND_ACC_PUBLIC)
    PHP_ME(Groonga, status,      Groonga_status,      ZEND_ACC_PUBLIC)
    PHP_ME(Groonga, tableList,   Groonga_tableList,   ZEND_ACC_PUBLIC)
    PHP_ME(Groonga, cacheLimit,  Groonga_cacheLimit,  ZEND_ACC_PUBLIC)
    PHP_ME(Groonga, dump,        Groonga_dump,        ZEND_ACC_PUBLIC)
    PHP_ME(Groonga, table,       Groonga_table,       ZEND_ACC_PUBLIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * コンストラクタ
 *
 * @access public
 * @param string str
 * @param integer port 
 * @param integer flags
 * @return void
 */
PHP_METHOD(Groonga, __construct)
{
    groonga_t *self;
    zval *ztables;
    char errmsg[64];
    char *str = "localhost";
    unsigned int length = 0;
    long port = 0, flags = 0;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &str, &length, &port, &flags) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
    
    /* grn_ctx構造体を初期化 */
    self->ctx = grn_ctx_open(0);
    if (NULL == self->ctx) {
        zend_throw_exception(groonga_exception_ce, "Unable to  finish context.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    if (port != 0) {
        /* GQTPによる接続 */
        if (grn_ctx_connect((grn_ctx *)self->ctx, str, port, flags) != GRN_SUCCESS) {
            sprintf(errmsg, "Unable to connect to [%s:%d] groonga server.", str, port);
            zend_throw_exception(groonga_exception_ce, errmsg, 0 TSRMLS_CC);
            RETURN_FALSE;
        }
        GROONGA_G(gqtpConnected) = 1;
        self->db        = NULL;
    } else {
        /* DBファイルを開く or 作成 */
        GRN_DB_OPEN_OR_CREATE(self->ctx, str, NULL, self->db);
 
        if (!self->db) {
            sprintf(errmsg, "Unable to open the db file [%s].", str);
            zend_throw_exception(groonga_exception_ce, errmsg, 0 TSRMLS_CC);
            RETURN_FALSE;
        }
        GROONGA_G(gqtpConnected) = 0;
    }

    /* 出力タイプの設定 */
    if (GRN_SUCCESS != grn_ctx_set_output_type(self->ctx, GRN_CONTENT_JSON)) {
        zend_throw_exception(groonga_exception_ce, "Unable to set the output type.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
}

/**
 * デストラクタ
 *
 * @access public
 * @return void
 */
PHP_METHOD(Groonga, __destruct)
{
    groonga_t *self;

    self = (groonga_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* Groonga関連メモリ解放 */
    if (NULL != self->ctx) {
        if (NULL != self->db) {
            grn_obj_close(self->ctx, self->db);
            self->db = NULL;
        }

        /* コンテキストを開放 */
        if (GRN_SUCCESS != grn_ctx_close(self->ctx)) {
            zend_throw_exception(groonga_exception_ce, "Unable to close context.", 0 TSRMLS_CC);
            RETURN_FALSE;
        }
    }
}

/**
 * Groongaクラス関数:クエリの実行
 *
 * @param string query
 * @param boolean 結果の形式  1:array 0:json
 * @param integer flags
 * @return mixed 結果情報
 */
PHP_METHOD(Groonga, query)
{
    zend_bool zbool = 0;
    groonga_t *self;
    char *query = NULL, *res;
    uint length;
    long flags = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|bl", &query, &length, &zbool, &flags) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    grn_ctx_send(self->ctx, query, length, flags);
    if (GRN_SUCCESS == self->ctx->rc) {
        int recv_flags;
        grn_ctx_recv(self->ctx, &res, &length, &recv_flags);
    }

    if (GRN_SUCCESS != self->ctx->rc) {
        RETURN_FALSE;
    }

    if (NULL == res) {
        RETURN_FALSE;
    }

    if (zbool) {
        php_json_decode_ex(return_value, res, length, PHP_JSON_OBJECT_AS_ARRAY, JSON_PARSER_GROONGA_DEPTH TSRMLS_CC);
    } else {
        RETURN_STRINGL(res, length, 1);
    }
}

/**
 * コマンド取得
 *
 * @access public
 * @param string  command
 * @return object command
 */
PHP_METHOD(Groonga, command)
{
    zval *zcommand, zcmdname;
    char *command = NULL;
    unsigned int command_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &command, &command_len) == FAILURE) {
        RETURN_FALSE;
    }

    /* オブジェクトを生成して初期化 */
    ALLOC_INIT_ZVAL(zcommand);
    object_init_ex(zcommand, groonga_command_ce);

    /* $command->__construct($this, $cmd_name) */
    ZVAL_STRINGL(&zcmdname, command, command_len, 0);
    CALL_METHOD2(GCommand, __construct, NULL, zcommand, getThis(), &zcmdname);

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zcommand, 1, 1);
}

/**
 * ステータスの取得
 *
 * @access public
 * @return array
 */
PHP_METHOD(Groonga, status)
{
    groonga_t *self;
    prn_cmd command;
    zval *retval;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!prn_command(self->ctx, &command, "status")) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &command, 1, (zval **)&retval)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    prn_command_unlink(self->ctx, &command);

    RETURN_ZVAL(retval, 1, 1);
}

/**
 * テーブルリストの取得
 *
 * @access public
 * @return array
 */
PHP_METHOD(Groonga, tableList)
{
    groonga_t *self;
    prn_cmd command;
    zval *retval;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!prn_command(self->ctx, &command, "table_list")) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &command, 1, (zval **)&retval)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    prn_command_unlink(self->ctx, &command);

    RETURN_ZVAL(retval, 1, 1);
}

/**
 * cache値の設定と取得
 *
 * @access public
 * @param integer $limit
 * @return integer
 */
PHP_METHOD(Groonga, cacheLimit)
{
    groonga_t *self;
    prn_cmd command;
    zval *retval;
    char *limit = NULL;
    unsigned int limit_len = 0;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &limit, &limit_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if (!prn_command(self->ctx, &command, "cache_limit")) {
        RETURN_FALSE;
    }

    if (0 < limit_len) {
        /* 変数へ文字列を設定 */
        if (!prn_command_set(self->ctx, &command, "max", limit)) {
            RETURN_FALSE;
        }

        /* コマンドの実行 */
        if (!prn_command_exec(self->ctx, &command, 0, NULL)) {
            RETURN_FALSE;
        }
    }

    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &command, 0, (zval **)&retval)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    prn_command_unlink(self->ctx, &command);

    RETURN_ZVAL(retval, 1, 1);
}

/**
 * ダンプ
 *
 * @access public
 * @return array
 */
PHP_METHOD(Groonga, dump)
{
    groonga_t *self;
    prn_cmd command;
    zval *retval;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!prn_command(self->ctx, &command, "dump")) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &command, 0, (zval **)&retval)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    prn_command_unlink(self->ctx, &command);

    RETURN_ZVAL(retval, 1, 1);
}

/**
 * テーブルの取得
 *
 * @access public
 * @param string table
 * @return table object
 */
PHP_METHOD(Groonga, table)
{
    zval *ztable, zname;
    char *table = NULL;
    uint table_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &table, &table_len) == FAILURE) {
        RETURN_FALSE;
    }

    /* オブジェクトを生成して初期化 */
    ALLOC_INIT_ZVAL(ztable);
    object_init_ex(ztable, groonga_table_ce);

    /* $table->__construct($this, $name) */
    ZVAL_STRINGL(&zname, table, table_len, 0);
    CALL_METHOD2(GTable, __construct, NULL, ztable, getThis(), &zname);

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(ztable, 1, 1);
}

#   endif       /* #ifndef HAVE_PROONGA_CLASS_GROONGA */
#endif      /* #ifndef HAVE_PROONGA_CLASS_GROONGA_H */
