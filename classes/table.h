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

#ifndef HAVE_PROONGA_CLASS_TABLE_H
#define HAVE_PROONGA_CLASS_TABLE_H

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/**
 * Groongaクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    grn_ctx *ctx;
    prn_cmd command;
    char *name;
} groonga_table_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(GTable, __construct);
PHP_METHOD(GTable, __destruct);
PHP_METHOD(GTable, __set);
PHP_METHOD(GTable, __get);
PHP_METHOD(GTable, name);
PHP_METHOD(GTable, flags);
PHP_METHOD(GTable, keyType);
PHP_METHOD(GTable, valueType);
PHP_METHOD(GTable, defaultTokenizer);
PHP_METHOD(GTable, normalizer);
PHP_METHOD(GTable, tokenFilters);
PHP_METHOD(GTable, load);
PHP_METHOD(GTable, delete);
PHP_METHOD(GTable, column);
PHP_METHOD(GTable, select);
PHP_METHOD(GTable, create);
PHP_METHOD(GTable, remove);
PHP_METHOD(GTable, rename);
PHP_METHOD(GTable, dump);
PHP_METHOD(GTable, truncate);
PHP_METHOD(GTable, columnList);


ZEND_BEGIN_ARG_INFO_EX(GTable___construct, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, gobject)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable___destruct, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable___set, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable___get, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable_load, 0, ZEND_RETURN_VALUE, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable_delete, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

extern zend_function_entry groonga_table_class_methods[];

#else
#   ifndef HAVE_PROONGA_CLASS_TABLE
#   define HAVE_PROONGA_CLASS_TABLE

/**
 * クラスの実装部分
 */
 
zend_function_entry groonga_table_class_methods[] = {
    PHP_ME(GTable, __construct,         GTable___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(GTable, __destruct,          GTable___destruct,  ZEND_ACC_PUBLIC)
    PHP_ME(GTable, __set,               GTable___set,       ZEND_ACC_PUBLIC)
    PHP_ME(GTable, __get,               GTable___get,       ZEND_ACC_PUBLIC)
    PHP_ME(GTable, name,                GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, flags,               GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, keyType,             GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, valueType,           GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, defaultTokenizer,    GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, normalizer,          GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, tokenFilters,        GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, load,                GTable_load,        ZEND_ACC_PUBLIC)
    PHP_ME(GTable, delete,              GTable_delete,      ZEND_ACC_PUBLIC)
    PHP_ME(GTable, column,              GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, select,              GTable_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, create,              GTable_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, remove,              GTable_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, rename,              GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, dump,                GTable_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, truncate,            GTable_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, columnList,          GTable_0_param,     ZEND_ACC_PUBLIC)

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
PHP_METHOD(GTable, __construct)
{
    groonga_table_t *self;
    zval *zgroonga, *zinfo, *zvalue;
    char *name;
    uint name_len;
    groonga_t *grn_p;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "os", &zgroonga, &name, &name_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* Groongaコンテキストを自身の変数へ保存 */
    grn_p = (groonga_t *) zend_object_store_get_object(zgroonga TSRMLS_CC);
    self->ctx = grn_p->ctx;

    /* テーブル名をセット */
    self->name = (char *)emalloc(sizeof(name));
    strcpy(self->name, name);

    /* Groonga組み込みコマンドの取得 */
    if (!prn_command(self->ctx, &self->command, "table_create")) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of table.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    /* 変数へ文字列を設定 */
    if (!prn_command_set(self->ctx, &self->command, "name", self->name)) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of table.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
}

/**
 * デストラクタ
 *
 * @access public
 * @return void
 */
PHP_METHOD(GTable, __destruct)
{
    groonga_table_t *self;

    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    prn_command_unlink(self->ctx, &self->command);
    efree(self->name);
}

/**
 * 変数設定
 *
 * @access public
 * @param string   $key
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GTable, __set)
{
    groonga_table_t *self;
    char *key, *value;
    uint key_len, value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
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
 * @param string   $value
 * @return boolean
 */
PHP_METHOD(GTable, __get)
{
    groonga_table_t *self;
    zval *retval;
    char *key;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_get(self->ctx, &self->command, key, (zval **)&retval)) {
        RETURN_FALSE;
    }

    RETURN_ZVAL(retval, 1, 1);
}

/**
 * nameの設定
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, name)
{
    groonga_table_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "name", value)) {
        RETURN_FALSE;
    }
    
    RETURN_CHAIN();
}

/**
 * flagsの設定
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, flags)
{
    groonga_table_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "flags", value)) {
        RETURN_FALSE;
    }
    
    RETURN_CHAIN();
}

/**
 * keyTypeの設定
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, keyType)
{
    groonga_table_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "key_type", value)) {
        RETURN_FALSE;
    }
    
    RETURN_CHAIN();
}

/**
 * valueTypeの設定
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, valueType)
{
    groonga_table_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "value_type", value)) {
        RETURN_FALSE;
    }
    
    RETURN_CHAIN();
}

/**
 * defaultTokenizerの設定
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, defaultTokenizer)
{
    groonga_table_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "default_tokenizer", value)) {
        RETURN_FALSE;
    }
    
    RETURN_CHAIN();
}

/**
 * normalizerの設定
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, normalizer)
{
    groonga_table_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "normalizer", value)) {
        RETURN_FALSE;
    }
    
    RETURN_CHAIN();
}

/**
 * tokenFiltersの設定
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, tokenFilters)
{
    groonga_table_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "token_filters", value)) {
        RETURN_FALSE;
    }
    
    RETURN_CHAIN();
}

/**
 * 変数の設定
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, load)
{
    zval *zload, *zvalues = NULL, retval;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &zvalues) == FAILURE) {
        RETURN_FALSE;
    }

    /* オブジェクトを生成して初期化 */
    ALLOC_INIT_ZVAL(zload);
    object_init_ex(zload, groonga_load_ce);

    /* $load->__construct($this) */
    CALL_METHOD1(GLoad, __construct, NULL, zload, getThis());

    if (NULL != zvalues) {
        /* $load->values($values) */
        CALL_METHOD1(GLoad, values, &retval, zload, zvalues);
        if (!zend_is_true(&retval)) {
            RETURN_FALSE;
        }

        /* $load->exec() */
        CALL_METHOD(GLoad, exec, &retval, zload);
        if (!zend_is_true(&retval)) {
            RETURN_FALSE;
        }
        RETURN_ZVAL(&retval, 1, 0);
    } // if (NULL != zvalues)

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zload, 1, 1);
}

/**
 * レコードを削除
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, delete)
{
    zval *zdelete;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    /* オブジェクトを生成して初期化 */
    ALLOC_INIT_ZVAL(zdelete);
    object_init_ex(zdelete, groonga_delete_ce);

    /* $delete->__construct($this) */
    CALL_METHOD1(GDelete, __construct, NULL, zdelete, getThis());

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zdelete, 1, 1);
}

/**
 * カラム操作
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, column)
{
    zval *zcolumn, zname;
    char *name = NULL;
    unsigned int name_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
        RETURN_FALSE;
    }

    /* オブジェクトを生成して初期化 */
    ALLOC_INIT_ZVAL(zcolumn);
    object_init_ex(zcolumn, groonga_column_ce);

    /* $column->__construct($this, $name) */
    ZVAL_STRINGL(&zname, name, name_len, 0);    
    CALL_METHOD2(GColumn, __construct, NULL, zcolumn, getThis(), &zname);

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zcolumn, 1, 1);
}

/**
 * select操作
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, select)
{
    zval *zselect;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    /* オブジェクトを生成して初期化 */
    ALLOC_INIT_ZVAL(zselect);
    object_init_ex(zselect, groonga_select_ce);

    /* $select->__construct($this) */
    CALL_METHOD1(GSelect, __construct, NULL, zselect, getThis());

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zselect, 1, 1);
}

/**
 * テーブル作成
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, create)
{
    groonga_table_t *self;
    zval *retval;
    int result;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &self->command, 0, (zval **)&retval)) {
        RETURN_FALSE;
    }

    /* 結果の判定 */
    result = (strncasecmp(Z_STRVAL_P(retval), "true", 4) == 0);
    zval_ptr_dtor(&retval);
    if (!result) {
        RETURN_FALSE;    
    }

    RETURN_CHAIN();
}

/**
 * テーブルの削除
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, remove)
{
    groonga_table_t *self;
    prn_cmd command;
    zval *retval;
    int result;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!prn_command(self->ctx, &command, "table_remove")) {
        RETURN_FALSE;
    }

    /* テーブル名を設定 */
    if (!prn_command_set(self->ctx, &command, "name", self->name)) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &command, 0, (zval **)&retval)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    prn_command_unlink(self->ctx, &command);

    /* 結果の判定 */
    result = (strncasecmp(Z_STRVAL_P(retval), "true", 4) == 0);
    zval_ptr_dtor(&retval);
    if (!result) {
        RETURN_FALSE;    
    }

    RETURN_CHAIN();
}

/**
 * テーブル名の変更
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, rename)
{
    groonga_table_t *self;
    prn_cmd command;
    zval *retval;
    int result;
    char *name;
    uint name_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!prn_command(self->ctx, &command, "table_rename")) {
        RETURN_FALSE;
    }

    /* テーブル名を設定 */
    if (!prn_command_set(self->ctx, &command, "name", self->name)) {
        RETURN_FALSE;
    }

    /* 新しいテーブル名を設定 */
    if (!prn_command_set(self->ctx, &command, "new_name", name)) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &command, 0, (zval **)&retval)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    prn_command_unlink(self->ctx, &command);

    /* 結果の判定 */
    result = (strncasecmp(Z_STRVAL_P(retval), "true", 4) == 0);
    zval_ptr_dtor(&retval);
    if (!result) {
        RETURN_FALSE;    
    }

    /* テーブル名の変更 */
    self->name = (char *) erealloc(self->name, sizeof(name));
    strcpy(self->name, name);

    RETURN_CHAIN();
}

/**
 * スキーマとデータを出力
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, dump)
{
    groonga_table_t *self;
    prn_cmd command;
    zval *retval;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!prn_command(self->ctx, &command, "dump")) {
        RETURN_FALSE;
    }

    /* 変数へ文字列を設定 */
    if (!prn_command_set(self->ctx, &command, "tables", self->name)) {
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
 * レコードをすべて削除
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, truncate)
{
    groonga_table_t *self;
    prn_cmd command;
    zval *retval;
    int result;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!prn_command(self->ctx, &command, "truncate")) {
        RETURN_FALSE;
    }

    /* 変数へ文字列を設定 */
    if (!prn_command_set(self->ctx, &command, "table", self->name)) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &command, 0, (zval **)&retval)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    prn_command_unlink(self->ctx, &command);

    /* 結果の判定 */
    result = (strncasecmp(Z_STRVAL_P(retval), "true", 4) == 0);
    zval_ptr_dtor(&retval);
    if (!result) {
        RETURN_FALSE;    
    }

    RETURN_CHAIN();
}

/**
 * カラム一覧を出力
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, columnList)
{
    groonga_table_t *self;
    prn_cmd command;
    zval *retval;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!prn_command(self->ctx, &command, "column_list")) {
        RETURN_FALSE;
    }

    /* 変数へ文字列を設定 */
    if (!prn_command_set(self->ctx, &command, "table", self->name)) {
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

#   endif       /* #ifndef HAVE_PROONGA_CLASS_TABLE */
#endif      /* #ifndef HAVE_PROONGA_CLASS_TABLE_H */
