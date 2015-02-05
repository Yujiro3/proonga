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

#ifndef HAVE_PROONGA_CLASS_DELETE_H
#define HAVE_PROONGA_CLASS_DELETE_H

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/**
 * GDeleteクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    grn_ctx *ctx;
    prn_cmd command;
} groonga_delete_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(GDelete, __construct);
PHP_METHOD(GDelete, __destruct);
PHP_METHOD(GDelete, __set);
PHP_METHOD(GDelete, __get);
PHP_METHOD(GDelete, key);
PHP_METHOD(GDelete, id);
PHP_METHOD(GDelete, filter);
PHP_METHOD(GDelete, exec);


ZEND_BEGIN_ARG_INFO_EX(GDelete___construct, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, gtalbe)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GDelete___destruct, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GDelete___set, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GDelete___get, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GDelete_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GDelete_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

extern zend_function_entry groonga_delete_class_methods[];

#else
#   ifndef HAVE_PROONGA_CLASS_DELETE
#   define HAVE_PROONGA_CLASS_DELETE

/**
 * クラスの実装部分
 */
zend_function_entry groonga_delete_class_methods[] = {
    PHP_ME(GDelete, __construct, GDelete___construct, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GDelete, __destruct,  GDelete___destruct,  ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GDelete, __set,       GDelete___set,       ZEND_ACC_PUBLIC)
    PHP_ME(GDelete, __get,       GDelete___get,       ZEND_ACC_PUBLIC)
    PHP_ME(GDelete, key,         GDelete_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GDelete, id,          GDelete_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GDelete, filter,      GDelete_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GDelete, exec,        GDelete_0_param,     ZEND_ACC_PUBLIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * コンストラクタ
 *
 * @access public
 * @param  object $groonga
 * @param  string $command
 * @return void
 */
PHP_METHOD(GDelete, __construct)
{
    groonga_delete_t *self;
    groonga_table_t *grn_p;
    zval *ztable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &ztable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_delete_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* Groongaコンテキストを自身の変数へ保存 */
    grn_p = (groonga_table_t *) zend_object_store_get_object(ztable TSRMLS_CC);
    self->ctx = grn_p->ctx;

    /* Groonga組み込みコマンドの取得 */
    if (!prn_command(self->ctx, &self->command, "delete")) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of delete.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    if (!PRN_COMMAND_SET(self->ctx, &self->command, "table", Z_STRVAL(grn_p->name))) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of delete.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
}

/**
 * デストラクタ
 *
 * @access public
 * @return void
 */
PHP_METHOD(GDelete, __destruct)
{
    groonga_delete_t *self;

    self = (groonga_delete_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    PRN_COMMAND_UNLINK(self->ctx, &self->command);
}

/**
 * 変数設定
 *
 * @access public
 * @param string   $key
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GDelete, __set)
{
    groonga_delete_t *self;
    char *key, *value;
    uint key_len, value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_delete_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_SET(self->ctx, &self->command, key, value)) {
        RETURN_FALSE;
    }
    
    RETURN_TRUE;
}

/**
 * 変数取得
 *
 * @access public
 * @param string   $key
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GDelete, __get)
{
    groonga_delete_t *self;
    char *key;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_delete_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_GET(self->ctx, &self->command, key, return_value)) {
        RETURN_FALSE;
    }
    
    return;
}

/**
 * keyの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GDelete, key)
{
    groonga_delete_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_delete_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_SET(self->ctx, &self->command, "key", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * idの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GDelete, id)
{
    groonga_delete_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_delete_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_SET(self->ctx, &self->command, "id", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * filterの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GDelete, filter)
{
    groonga_delete_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_delete_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_SET(self->ctx, &self->command, "filter", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * 実行
 *
 * @access public
 * @param boolean  $assoc
 * @return string/array
 */
PHP_METHOD(GDelete, exec)
{
    groonga_delete_t *self;
    grn_ctx_info info;
    zend_bool assoc = 0;
    zval retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &assoc) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_delete_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンドの実行 */
    if (!PRN_COMMAND_EXEC(self->ctx, &self->command, &retval, 0)) {
        RETURN_FALSE;
    }

    /* 結果の判定 */
    if (!strcmp("true", Z_STRVAL(retval))) {
        RETURN_FALSE;    
    }

    RETURN_CHAIN();
}

#   endif       /* #ifndef HAVE_PROONGA_CLASS_DELETE */
#endif      /* #ifndef HAVE_PROONGA_CLASS_DELETE_H */
