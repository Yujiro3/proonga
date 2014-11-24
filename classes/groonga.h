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

#ifndef HAVE_GROONGA_CLASS_GROONGA_H
#define HAVE_GROONGA_CLASS_GROONGA_H

/**
 * Groongaクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    grn_ctx *ctx;
    grn_obj *db;
    int connected;
} groonga_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(Groonga, __construct);
PHP_METHOD(Groonga, __destruct);
PHP_METHOD(Groonga, command);


ZEND_BEGIN_ARG_INFO_EX(Groonga___construct, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, gobject)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga___destruct, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Groonga_command, 0, ZEND_RETURN_VALUE, 0)
    ZEND_ARG_INFO(0, command)
ZEND_END_ARG_INFO()

extern zend_function_entry groonga_class_methods[];
#else
#   ifndef HAVE_GROONGA_CLASS_GROONGA
#   define HAVE_GROONGA_CLASS_GROONGA

/**
 * クラスの実装部分
 */
 
zend_function_entry groonga_class_methods[] = {
    PHP_ME(Groonga, __construct, Groonga___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Groonga, __destruct,  Groonga___destruct,  ZEND_ACC_PUBLIC)
    PHP_ME(Groonga, command,     Groonga_command,     ZEND_ACC_PUBLIC)

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
    if (GRN_SUCCESS != grn_ctx_init(self->ctx, 0)) {
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
        self->connected = 1;
        self->db        = NULL;
    } else {
        /* DBファイルを開く or 作成 */
        GRN_DB_OPEN_OR_CREATE(self->ctx, str, NULL, self->db);
 
        if (!self->db) {
            sprintf(errmsg, "Unable to open the db file [%s].", str);
            zend_throw_exception(groonga_exception_ce, errmsg, 0 TSRMLS_CC);
            RETURN_FALSE;
        }
        self->connected = 0;
    }

    /* 出力タイプの設定 */
    if (GRN_SUCCESS != grn_ctx_set_output_type(self->ctx, GRN_CONTENT_JSON)) {
        zend_throw_exception(groonga_exception_ce, "Unable to set the output type.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
}

/**
 * コンストラクタ
 *
 * @access public
 * @return void
 */
PHP_METHOD(Groonga, __destruct)
{
    groonga_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* Groonga関連メモリ解放 */
    if (NULL != self->ctx) {
        if (NULL != self->db) {
            grn_obj_close(self->ctx, self->db);
            self->db = NULL;
        }

        /* GQTPとの接続を切断 */
        if (1 == self->connected) {
            grn_ctx_close(self->ctx);
            self->connected = 0;
        }

        grn_ctx_fin(self->ctx);
    }
}

/**
 * コマンド取得
 *
 * @access public
 * @param string command
 * @return object command
 */
PHP_METHOD(Groonga, command)
{
    zval *zcommand, zcmdname;
    char *command = NULL;
    unsigned int command_len;
    groonga_command_t *gcommand;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &command, &command_len) == FAILURE) {
        RETURN_FALSE;
    }

    /* オブジェクトを生成して初期化 */
    MAKE_STD_ZVAL(zcommand);
    object_init_ex(zcommand, groonga_command_ce);

    ZVAL_STRINGL(&zcmdname, command, command_len, 0);
    CALL_METHOD2(GCommand, __construct, NULL, zcommand, getThis(), &zcmdname);

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zcommand, 1, 0);
}

#   endif
#endif
