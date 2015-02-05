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

#ifndef HAVE_PROONGA_CLASS_LOAD_H
#define HAVE_PROONGA_CLASS_LOAD_H

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/**
 * GLoadクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    grn_ctx *ctx;
    prn_cmd command;
} groonga_load_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(GLoad, __construct);
PHP_METHOD(GLoad, __destruct);
PHP_METHOD(GLoad, __set);
PHP_METHOD(GLoad, __get);
PHP_METHOD(GLoad, values);
PHP_METHOD(GLoad, table);
PHP_METHOD(GLoad, columns);
PHP_METHOD(GLoad, ifexists);
PHP_METHOD(GLoad, inputType);
PHP_METHOD(GLoad, each);
PHP_METHOD(GLoad, exec);


ZEND_BEGIN_ARG_INFO_EX(GLoad___construct, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, gobject)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GLoad___destruct, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GLoad___set, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GLoad___get, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GLoad_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GLoad_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

extern zend_function_entry groonga_load_class_methods[];

#else
#   ifndef HAVE_PROONGA_CLASS_LOAD
#   define HAVE_PROONGA_CLASS_LOAD

/**
 * クラスの実装部分
 */
zend_function_entry groonga_load_class_methods[] = {
    PHP_ME(GLoad, __construct, GLoad___construct, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, __destruct,  GLoad___destruct,  ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, __set,       GLoad___set,       ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, __get,       GLoad___get,       ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, values,      GLoad_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, table,       GLoad_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, columns,     GLoad_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, ifexists,    GLoad_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, inputType,   GLoad_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, each,        GLoad_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GLoad, exec,        GLoad_0_param,     ZEND_ACC_PUBLIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * コンストラクタ
 *
 * @access public
 * @param  object $groonga
 * @return void
 */
PHP_METHOD(GLoad, __construct)
{
    groonga_load_t *self;
    groonga_table_t *grn_p;
    zval *ztable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &ztable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* Groongaコンテキストを自身の変数へ保存 */
    grn_p = (groonga_table_t *) zend_object_store_get_object(ztable TSRMLS_CC);
    self->ctx = grn_p->ctx;

    /* Groonga組み込みコマンドの取得 */
    if (!prn_command(self->ctx, &self->command, "load")) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of load.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    if (!PRN_COMMAND_SET(self->ctx, &self->command, "table", Z_STRVAL(grn_p->name))) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of load.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
}

/**
 * デストラクタ
 *
 * @access public
 * @return void
 */
PHP_METHOD(GLoad, __destruct)
{
    groonga_load_t *self;

    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

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
PHP_METHOD(GLoad, __set)
{
    groonga_load_t *self;
    char *key, *value;
    uint key_len, value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

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
PHP_METHOD(GLoad, __get)
{
    groonga_load_t *self;
    char *key;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_GET(self->ctx, &self->command, key, return_value)) {
        RETURN_FALSE;
    }
    
    return;
}

/**
 * valuesの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GLoad, values)
{
    groonga_load_t *self;
    zval *zvalues;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &zvalues) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if (IS_ARRAY == Z_TYPE_P(zvalues)) {
        smart_str buf = {0};
        php_json_encode(&buf, zvalues, 0 TSRMLS_CC);

        /* 変数の設定 */
        if (!PRN_COMMAND_SET(self->ctx, &self->command, "values", buf.c)) {
            RETURN_FALSE;
        }
        if (buf.c) {
            efree(buf.c);
        }
    } else if (IS_STRING == Z_TYPE_P(zvalues)) {
        /* 変数の設定 */
        if (!PRN_COMMAND_SET(self->ctx, &self->command, "values", Z_STRVAL_P(zvalues))) {
            RETURN_FALSE;
        }
    }

    RETURN_CHAIN();
}

/**
 * tableの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GLoad, table)
{
    groonga_load_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_SET(self->ctx, &self->command, "table", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * columnsの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GLoad, columns)
{
    groonga_load_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_SET(self->ctx, &self->command, "columns", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * ifexistsの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GLoad, ifexists)
{
    groonga_load_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_SET(self->ctx, &self->command, "ifexists", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * input_typeの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GLoad, inputType)
{
    groonga_load_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_SET(self->ctx, &self->command, "input_type", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * eachの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GLoad, each)
{
    groonga_load_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!PRN_COMMAND_SET(self->ctx, &self->command, "each", value)) {
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
PHP_METHOD(GLoad, exec)
{
    groonga_load_t *self;
    grn_ctx_info info;
    zval retval;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_load_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

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

#   endif       /* #ifndef HAVE_PROONGA_CLASS_LOAD */
#endif      /* #ifndef HAVE_PROONGA_CLASS_LOAD_H */
