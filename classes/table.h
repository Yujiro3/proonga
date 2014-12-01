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

/**
 * Groongaクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    grn_ctx *ctx;
    grn_obj *command;
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
    self->name = emalloc(name_len);
    memcpy(self->name, name, name_len);

    /* Groonga組み込みコマンドの取得 */
    if (!proonga_command(self->ctx, &self->command, "table_create" TSRMLS_CC)) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of table.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    /* 変数へ文字列を設定 */
    if (!proonga_command_set(self->ctx, self->command, "name", self->name TSRMLS_CC)) {
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

    grn_obj_unlink(self->ctx, self->command);
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
    if (!proonga_command_set(self->ctx, self->command, key, value TSRMLS_CC)) {
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
    char *key;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_get(self->ctx, self->command, key, return_value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    return;
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
    if (!proonga_command_set(self->ctx, self->command, "name", value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    RETURN_ZVAL(getThis(), 1, 0);
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
    if (!proonga_command_set(self->ctx, self->command, "flags", value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    RETURN_ZVAL(getThis(), 1, 0);
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
    if (!proonga_command_set(self->ctx, self->command, "key_type", value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    RETURN_ZVAL(getThis(), 1, 0);
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
    if (!proonga_command_set(self->ctx, self->command, "value_type", value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    RETURN_ZVAL(getThis(), 1, 0);
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
    if (!proonga_command_set(self->ctx, self->command, "default_tokenizer", value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    RETURN_ZVAL(getThis(), 1, 0);
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
    if (!proonga_command_set(self->ctx, self->command, "normalizer", value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    RETURN_ZVAL(getThis(), 1, 0);
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
    if (!proonga_command_set(self->ctx, self->command, "token_filters", value TSRMLS_CC)) {
        RETURN_FALSE;
    }
    
    RETURN_ZVAL(getThis(), 1, 0);
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
    zval *zload, *zvalues = NULL;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &zvalues) == FAILURE) {
        RETURN_FALSE;
    }

    /* オブジェクトを生成して初期化 */
    MAKE_STD_ZVAL(zload);
    object_init_ex(zload, groonga_load_ce);

    /* $load->__construct($this) */
    zend_call_method_with_1_params(
        (zval **)&zload, Z_OBJCE_P(zload), 
        NULL, "__construct", 
        NULL, getThis()
    );

    if (NULL != zvalues) {
        /* $load->values($values) */
        zend_call_method_with_1_params(
            (zval **)&zload, Z_OBJCE_P(zload), 
            NULL, "values", 
            NULL, zvalues
        );

        /* $load->exec() */
        zend_call_method_with_0_params(
            (zval **)&zload, Z_OBJCE_P(zload), 
            NULL, "exec", 
            NULL
        );
    }

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zload, 1, 0);
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
    MAKE_STD_ZVAL(zdelete);
    object_init_ex(zdelete, groonga_delete_ce);

    /* $delete->__construct($this) */
    zend_call_method_with_1_params(
        (zval **)&zdelete, Z_OBJCE_P(zdelete), 
        NULL, "__construct", 
        NULL, getThis()
    );

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zdelete, 1, 0);
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
    MAKE_STD_ZVAL(zcolumn);
    object_init_ex(zcolumn, groonga_column_ce);

    /* $column->__construct($this, $name) */
    ZVAL_STRINGL(&zname, name, name_len, 0);    

    zend_call_method_with_2_params(
        (zval **)&zcolumn, Z_OBJCE_P(zcolumn), 
        NULL, "__construct", 
        NULL, getThis(), &zname
    );

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zcolumn, 1, 0);
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
    MAKE_STD_ZVAL(zselect);
    object_init_ex(zselect, groonga_select_ce);

    /* $select->__construct($this) */
    zend_call_method_with_1_params(
        (zval **)&zselect, Z_OBJCE_P(zselect), 
        NULL, "__construct", 
        NULL, getThis()
    );

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(zselect, 1, 0);
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
    zval retval;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
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
 * テーブルの削除
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, remove)
{
    groonga_table_t *self;
    grn_obj *command;
    zval retval;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!proonga_command(self->ctx, &command, "table_remove" TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* テーブル名を設定 */
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
 * テーブル名の変更
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, rename)
{
    groonga_table_t *self;
    grn_obj *command;
    zval retval;
    char *name;
    uint name_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!proonga_command(self->ctx, &command, "table_rename" TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* テーブル名を設定 */
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
    memcpy(self->name, name, name_len);

    RETURN_ZVAL(getThis(), 1, 0);
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
    grn_obj *command;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!proonga_command(self->ctx, &command, "dump" TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* 変数へ文字列を設定 */
    if (!proonga_command_set(self->ctx, command, "tables", self->name TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!proonga_command_exec(self->ctx, command, return_value, 0 TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    grn_obj_unlink(self->ctx, command);

    return ;
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
    grn_obj *command;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!proonga_command(self->ctx, &command, "truncate" TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* 変数へ文字列を設定 */
    if (!proonga_command_set(self->ctx, command, "table", self->name TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!proonga_command_exec(self->ctx, command, return_value, 0 TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    grn_obj_unlink(self->ctx, command);

    RETURN_ZVAL(getThis(), 1, 0);
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
    grn_obj *command;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンド取得 */
    if (!proonga_command(self->ctx, &command, "column_list" TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* 変数へ文字列を設定 */
    if (!proonga_command_set(self->ctx, command, "table", self->name TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* コマンドの実行 */
    if (!proonga_command_exec(self->ctx, command, return_value, 0 TSRMLS_CC)) {
        RETURN_FALSE;
    }

    /* メモリ解放 */
    grn_obj_unlink(self->ctx, command);

    return ;
}

#   endif       /* #ifndef HAVE_PROONGA_CLASS_TABLE */
#endif      /* #ifndef HAVE_PROONGA_CLASS_TABLE_H */
