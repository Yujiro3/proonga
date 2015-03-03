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

#ifndef HAVE_PROONGA_CLASS_COMMAND_H
#define HAVE_PROONGA_CLASS_COMMAND_H

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/**
 * GCommandクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    grn_ctx *ctx;
    prn_cmd command;
} groonga_command_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(GCommand, __construct);
PHP_METHOD(GCommand, __destruct);
PHP_METHOD(GCommand, __set);
PHP_METHOD(GCommand, __get);
PHP_METHOD(GCommand, exec);


ZEND_BEGIN_ARG_INFO_EX(GCommand___construct, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, gobject)
    ZEND_ARG_INFO(0, command)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GCommand___destruct, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GCommand___set, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GCommand___get, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GCommand_exec, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

extern zend_function_entry groonga_command_class_methods[];

#else
#   ifndef HAVE_PROONGA_CLASS_COMMAND
#   define HAVE_PROONGA_CLASS_COMMAND

/**
 * クラスの実装部分
 */
zend_function_entry groonga_command_class_methods[] = {
    PHP_ME(GCommand, __construct, GCommand___construct, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GCommand, __destruct,  GCommand___destruct,  ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GCommand, __set,       GCommand___set,       ZEND_ACC_PUBLIC)
    PHP_ME(GCommand, __get,       GCommand___get,       ZEND_ACC_PUBLIC)
    PHP_ME(GCommand, exec,        GCommand_exec,        ZEND_ACC_PUBLIC)

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
PHP_METHOD(GCommand, __construct)
{
    groonga_command_t *self;
    zval *zgroonga;
    char *command;
    uint command_len;
    groonga_t *grn_p;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "os", &zgroonga, &command, &command_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_command_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* Groongaコンテキストを自身の変数へ保存 */
    grn_p = (groonga_t *) zend_object_store_get_object(zgroonga TSRMLS_CC);
    self->ctx = grn_p->ctx;

    /* Groonga組み込みコマンドの取得 */
    if (!prn_command(self->ctx, &self->command, command)) {
        char errmsg[64];
        sprintf(errmsg, "Command not found. [%s]", command);
        zend_throw_exception(groonga_exception_ce, errmsg, 0 TSRMLS_CC);
        RETURN_FALSE;
    }
}

/**
 * デストラクタ
 *
 * @access public
 * @return void
 */
PHP_METHOD(GCommand, __destruct)
{
    groonga_command_t *self;

    self = (groonga_command_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    prn_command_unlink(self->ctx, &self->command);
}

/**
 * 変数設定
 *
 * @access public
 * @param string   $key
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GCommand, __set)
{
    groonga_command_t *self;
    char *key, *value;
    uint key_len, value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_command_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if (!prn_command_set(self->ctx, &self->command, key, value)) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

/**
 * 変数取得
 *
 * @access public
 * @param string   $key
 * @return string
 */
PHP_METHOD(GCommand, __get)
{
    groonga_command_t *self;
    zval *retval;
    char *key;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_command_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if (!prn_command_get(self->ctx, &self->command, key, (zval **)&retval)) {
        RETURN_FALSE;
    }

    RETURN_ZVAL(retval, 1, 1);
}

/**
 * 実行
 *
 * @access public
 * @param boolean  $assoc
 * @return string/array
 */
PHP_METHOD(GCommand, exec)
{
    groonga_command_t *self;
    grn_ctx_info info;
    zend_bool assoc = 0;
    zval *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &assoc) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_command_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &self->command, 1, (zval **)&retval)) {
        RETURN_FALSE;
    }

    RETURN_ZVAL(retval, 1, 1);
}

#   endif       /* #ifndef HAVE_PROONGA_CLASS_COMMAND */
#endif      /* #ifndef HAVE_PROONGA_CLASS_COMMAND_H */
