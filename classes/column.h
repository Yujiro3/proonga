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

#ifndef HAVE_PROONGA_CLASS_COLUMN_H
#define HAVE_PROONGA_CLASS_COLUMN_H

/**
 * GColumnクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    grn_ctx *ctx;
    grn_obj *command;
    char *table;
    char *name;
} groonga_column_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(GColumn, __construct);
PHP_METHOD(GColumn, __destruct);
PHP_METHOD(GColumn, __set);
PHP_METHOD(GColumn, __get);
PHP_METHOD(GColumn, flags);
PHP_METHOD(GColumn, type);
PHP_METHOD(GColumn, source);
PHP_METHOD(GColumn, create);
PHP_METHOD(GColumn, remove);
PHP_METHOD(GColumn, rename);

ZEND_BEGIN_ARG_INFO_EX(GColumn___construct, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, gtalbe)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GColumn___destruct, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GColumn___set, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GColumn___get, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GColumn_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GColumn_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

extern zend_function_entry groonga_column_class_methods[];

#else
#   ifndef HAVE_PROONGA_CLASS_COLUMN
#   define HAVE_PROONGA_CLASS_COLUMN

/**
 * クラスの実装部分
 */
zend_function_entry groonga_column_class_methods[] = {
    PHP_ME(GColumn, __construct, GColumn___construct, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GColumn, __destruct,  GColumn___destruct,  ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GColumn, __set,       GColumn___set,       ZEND_ACC_PUBLIC)
    PHP_ME(GColumn, __get,       GColumn___get,       ZEND_ACC_PUBLIC)
    PHP_ME(GColumn, flags,       GColumn_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GColumn, type,        GColumn_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GColumn, source,      GColumn_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GColumn, create,      GColumn_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GColumn, remove,      GColumn_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GColumn, rename,      GColumn_1_param,     ZEND_ACC_PUBLIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * コンストラクタ
 *
 * @access public
 * @param  object $table
 * @param  string $name
 * @return void
 */
PHP_METHOD(GColumn, __construct)
{
    groonga_column_t *self;
    groonga_table_t *grn_p;
    zval *ztable;
    char *name;
    uint name_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "os", &ztable, &name, &name_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* Groongaコンテキストを自身の変数へ保存 */
    grn_p = (groonga_table_t *) zend_object_store_get_object(ztable TSRMLS_CC);
    self->ctx = grn_p->ctx;

    /* テーブル名をセット */
    self->table = emalloc(strlen(grn_p->name));
    strcpy(self->table, grn_p->name);

    /* カラム名をセット */
    self->name = emalloc(name_len);
    strcpy(self->name, name);

    /* Groonga組み込みコマンドの取得 */
    if (!proonga_command(self->ctx, &self->command, "column_create" TSRMLS_CC)) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of column.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    if (!proonga_command_set(self->ctx, self->command, "table", self->table TSRMLS_CC)) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of column.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    if (!proonga_command_set(self->ctx, self->command, "name", self->name TSRMLS_CC)) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of column.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
}

/**
 * デストラクタ
 *
 * @access public
 * @return void
 */
PHP_METHOD(GColumn, __destruct)
{
    groonga_column_t *self;

    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    grn_obj_unlink(self->ctx, self->command);

    efree(self->name);
    efree(self->table);
}

/**
 * 変数設定
 *
 * @access public
 * @param string   $key
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GColumn, __set)
{
    groonga_column_t *self;
    char *key, *value;
    uint key_len, value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->command, key, value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    RETURN_TRUE;
}

/**
 * 変数設定
 *
 * @access public
 * @param string   $key
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GColumn, __get)
{
    groonga_column_t *self;
    char *key;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_get(self->ctx, self->command, key, return_value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    return;
}

/**
 * flagsの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GColumn, flags)
{
    groonga_column_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->command, "flags", value TSRMLS_CC)) {
        RETURN_FALSE;
    }

    RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * typeの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GColumn, type)
{
    groonga_column_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->command, "type", value TSRMLS_CC)) {
        RETURN_FALSE;
    }

    RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * sourceの設定
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GColumn, source)
{
    groonga_column_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->command, "source", value TSRMLS_CC)) {
        RETURN_FALSE;
    }

    RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * カラムの作成
 *
 * @access public
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GColumn, create)
{
    groonga_column_t *self;
    zval retval;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンドの実行 */
    if (!proonga_command_exec(self->ctx, self->command, &retval, 0 TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* 結果の判定 */
    if (!strcmp("true", Z_STRVAL(retval))) {
        RETURN_FALSE;    
    }

    RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * カラムの削除
 *
 * @access public
 * @return boolean
 */
PHP_METHOD(GColumn, remove)
{
    groonga_column_t *self;
    grn_obj *command;
    zval retval;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!proonga_command(self->ctx, &command, "column_remove" TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* テーブル名を設定 */
    if (!proonga_command_set(self->ctx, command, "table", self->table TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* カラム名を設定 */
    if (!proonga_command_set(self->ctx, command, "name", self->name TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!proonga_command_exec(self->ctx, command, &retval, 0 TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    grn_obj_unlink(self->ctx, command);

    /* 結果の判定 */
    if (!strcmp("true", Z_STRVAL(retval))) {
        RETURN_FALSE;    
    }

    RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * カラム名の変更
 *
 * @access public
 * @return boolean
 */
PHP_METHOD(GColumn, rename)
{
    groonga_column_t *self;
    grn_obj *command;
    zval retval;
    char *name;
    uint name_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_column_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!proonga_command(self->ctx, &command, "column_rename" TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* テーブル名を設定 */
    if (!proonga_command_set(self->ctx, command, "table", self->table TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* カラム名を設定 */
    if (!proonga_command_set(self->ctx, command, "name", self->name TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* 新しいテーブル名を設定 */
    if (!proonga_command_set(self->ctx, command, "new_name", name TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!proonga_command_exec(self->ctx, command, &retval, 0 TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    grn_obj_unlink(self->ctx, command);

    /* 結果の判定 */
    if (!strcmp("true", Z_STRVAL(retval))) {
        RETURN_FALSE;    
    }

    /* テーブル名の変更 */
    efree(self->name);
    self->name = emalloc(name_len);
    strcpy(self->name, name);

    RETURN_ZVAL(getThis(), 1, 0);
}


#   endif       /* #ifndef HAVE_PROONGA_CLASS_COLUMN */
#endif      /* #ifndef HAVE_PROONGA_CLASS_COLUMN_H */
