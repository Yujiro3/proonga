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
    grn_obj *table;
    char *name;
    HashTable *info;
    HashTable *value;
} groonga_table_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(GTable, __construct);
PHP_METHOD(GTable, __destruct);
PHP_METHOD(GTable, __set);

PHP_METHOD(GTable, setFlags);
PHP_METHOD(GTable, setKeyType);
PHP_METHOD(GTable, setValueType);
PHP_METHOD(GTable, setDefaultTokenizer);
PHP_METHOD(GTable, setNormalizer);
PHP_METHOD(GTable, setTokenFilters);

PHP_METHOD(GTable, load);
PHP_METHOD(GTable, create);
PHP_METHOD(GTable, remove);
PHP_METHOD(GTable, rename);
PHP_METHOD(GTable, delete);
PHP_METHOD(GTable, dump);
PHP_METHOD(GTable, truncate);


ZEND_BEGIN_ARG_INFO_EX(GTable___construct, 1, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, gobject)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable___destruct, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GTable_delete, 1, ZEND_RETURN_VALUE, 0)
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

    PHP_ME(GTable, setFlags,            GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, setKeyType,          GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, setValueType,        GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, setDefaultTokenizer, GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, setNormalizer,       GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, setTokenFilters,     GTable_1_param,     ZEND_ACC_PUBLIC)

    PHP_ME(GTable, create,              GTable_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, remove,              GTable_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, rename,              GTable_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, delete,              GTable_delete,      ZEND_ACC_PUBLIC)
    PHP_ME(GTable, dump,                GTable_0_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GTable, truncate,            GTable_0_param,     ZEND_ACC_PUBLIC)

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
    if (!proonga_command(self->ctx, &self->table, "table_create" TSRMLS_CC)) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of table.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    /* 変数へ文字列を設定 */
    if (!proonga_command_set(self->ctx, self->table, "name", self->name TSRMLS_CC)) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of table.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    /* テーブル値プロパティの初期化 */
    zvalue = zend_read_property(groonga_table_ce, getThis(), ZEND_STRL("value"), 1 TSRMLS_CC);
    object_init_ex(zvalue, ZEND_STANDARD_CLASS_DEF_PTR);
    self->value = HASH_OF(zvalue);
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
    zval **zobject;
    HashPosition hpos;

    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* テーブル値プロパティの破棄 */
    zend_hash_internal_pointer_reset_ex(self->value, &hpos);
    while (zend_hash_get_current_data_ex(self->value, (void **)&zobject, &hpos) == SUCCESS) {        
        zval_dtor(*zobject);
        zend_hash_move_forward_ex(self->value, &hpos);
    }

    /* テーブル情報プロパティの破棄 */
    zend_hash_internal_pointer_reset_ex(self->info, &hpos);
    while (zend_hash_get_current_data_ex(self->info, (void **)&zobject, &hpos) == SUCCESS) {        
        zval_dtor(*zobject);
        zend_hash_move_forward_ex(self->info, &hpos);
    }

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
    zval *zvalues;
    char *name, *value;
    uint name_len, value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &name, &name_len, &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* テーブルリストプロパティの取得 */
    zvalues = zend_read_property(groonga_table_ce, getThis(), ZEND_STRL("value"), 1 TSRMLS_CC);

    /* 値リストへ追加 */
    add_property_stringl(zvalues, name, value, value_len, 1);

    /* テーブルリストプロパティの更新 */
    zend_update_property(groonga_table_ce, getThis(), ZEND_STRL("value"), zvalues TSRMLS_CC);

    RETURN_TRUE;
}


/**
 * 変数の設定
 *
 * @access public
 * @param string value
 * @return object self
 */
PHP_METHOD(GTable, setFlags)
{
    groonga_table_t *self;
    char *value = NULL;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->table, "flags", value TSRMLS_CC)) {
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
PHP_METHOD(GTable, setKeyType)
{
    groonga_table_t *self;
    char *value = NULL;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->table, "key_type", value TSRMLS_CC)) {
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
PHP_METHOD(GTable, setValueType)
{
    groonga_table_t *self;
    char *value = NULL;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->table, "value_type", value TSRMLS_CC)) {
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
PHP_METHOD(GTable, setDefaultTokenizer)
{
    groonga_table_t *self;
    char *value = NULL;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->table, "default_tokenizer", value TSRMLS_CC)) {
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
PHP_METHOD(GTable, setNormalizer)
{
    groonga_table_t *self;
    char *value = NULL;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->table, "normalizer", value TSRMLS_CC)) {
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
PHP_METHOD(GTable, setTokenFilters)
{
    groonga_table_t *self;
    char *value = NULL;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_table_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!proonga_command_set(self->ctx, self->table, "token_filters", value TSRMLS_CC)) {
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
    if (!proonga_command_exec(self->ctx, self->table, &retval, 1 TSRMLS_CC)) {
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

    /* 変数へ文字列を設定 */
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

    RETURN_ZVAL(getThis(), 1, 0);
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

    RETURN_ZVAL(getThis(), 1, 0);
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

#   endif       /* #ifndef HAVE_PROONGA_CLASS_TABLE */
#endif      /* #ifndef HAVE_PROONGA_CLASS_TABLE_H */
