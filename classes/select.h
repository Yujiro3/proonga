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

#ifndef HAVE_PROONGA_CLASS_SELECT_H
#define HAVE_PROONGA_CLASS_SELECT_H

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/**
 * GSelectクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    grn_ctx *ctx;
    prn_cmd command;
} groonga_select_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(GSelect, __construct);
PHP_METHOD(GSelect, __destruct);
PHP_METHOD(GSelect, __set);
PHP_METHOD(GSelect, __get);
PHP_METHOD(GSelect, matchColumns);
PHP_METHOD(GSelect, query);
PHP_METHOD(GSelect, filter);
PHP_METHOD(GSelect, scorer);
PHP_METHOD(GSelect, sortby);
PHP_METHOD(GSelect, outputColumns);
PHP_METHOD(GSelect, offset);
PHP_METHOD(GSelect, limit);
PHP_METHOD(GSelect, drilldown);
PHP_METHOD(GSelect, drilldownSortby);
PHP_METHOD(GSelect, drilldownOutputColumns);
PHP_METHOD(GSelect, drilldownOffset);
PHP_METHOD(GSelect, drilldownLimit);
PHP_METHOD(GSelect, cache);
PHP_METHOD(GSelect, matchEscalationThreshold);
PHP_METHOD(GSelect, queryExpansion);
PHP_METHOD(GSelect, queryFlags);
PHP_METHOD(GSelect, queryExpander);
PHP_METHOD(GSelect, adjuster);
PHP_METHOD(GSelect, exec);

ZEND_BEGIN_ARG_INFO_EX(GSelect___construct, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, gtalbe)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GSelect___destruct, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GSelect___set, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GSelect___get, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GSelect_exec, 0, ZEND_RETURN_VALUE, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GSelect_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(GSelect_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

extern zend_function_entry groonga_select_class_methods[];

#else
#   ifndef HAVE_PROONGA_CLASS_SELECT
#   define HAVE_PROONGA_CLASS_SELECT

/**
 * クラスの実装部分
 */
zend_function_entry groonga_select_class_methods[] = {
    PHP_ME(GSelect, __construct,              GSelect___construct, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, __destruct,               GSelect___destruct,  ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, __set,                    GSelect___set,       ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, __get,                    GSelect___get,       ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, matchColumns,             GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, query,                    GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, filter,                   GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, scorer,                   GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, sortby,                   GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, outputColumns,            GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, offset,                   GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, limit,                    GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, drilldown,                GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, drilldownSortby,          GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, drilldownOutputColumns,   GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, drilldownOffset,          GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, drilldownLimit,           GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, cache,                    GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, matchEscalationThreshold, GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, queryExpansion,           GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, queryFlags,               GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, queryExpander,            GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, adjuster,                 GSelect_1_param,     ZEND_ACC_PUBLIC)
    PHP_ME(GSelect, exec,                     GSelect_exec,        ZEND_ACC_PUBLIC)

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
PHP_METHOD(GSelect, __construct)
{
    groonga_select_t *self;
    groonga_table_t *grn_p;
    zval *ztable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &ztable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* Groongaコンテキストを自身の変数へ保存 */
    grn_p = (groonga_table_t *) zend_object_store_get_object(ztable TSRMLS_CC);
    self->ctx = grn_p->ctx;

    /* Groonga組み込みコマンドの取得 */
    if (!prn_command(self->ctx, &self->command, "select")) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of select.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    /* テーブル名を設定 */
    if (!prn_command_set(self->ctx, &self->command, "table", Z_STRVAL(grn_p->name))) {
        zend_throw_exception(groonga_exception_ce, "Unable to initialize of select.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
}

/**
 * デストラクタ
 *
 * @access public
 * @return void
 */
PHP_METHOD(GSelect, __destruct)
{
    groonga_select_t *self;

    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

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
PHP_METHOD(GSelect, __set)
{
    groonga_select_t *self;
    char *key, *value;
    uint key_len, value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

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
PHP_METHOD(GSelect, __get)
{
    groonga_select_t *self;
    zval *retval;
    char *key;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_get(self->ctx, &self->command, key, (zval **)&retval)) {
        RETURN_FALSE;
    }
    
    RETURN_ZVAL(retval, 1, 1);
}

/**
 * matchColumnsの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, matchColumns)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "match_columns", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * queryの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, query)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "query", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * filterの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, filter)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "filter", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * scorerの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, scorer)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "scorer", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * sortbyの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, sortby)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "sortby", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * outputColumnsの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, outputColumns)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "output_columns", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * offsetの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, offset)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "offset", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * limitの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, limit)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "limit", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * drilldownの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, drilldown)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "drilldown", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * drilldownSortbyの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, drilldownSortby)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "drilldown_sortby", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * drilldownOutputColumnsの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, drilldownOutputColumns)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "drilldown_output_columns", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * drilldownOffsetの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, drilldownOffset)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "drilldown_offset", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * drilldownLimitの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, drilldownLimit)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "drilldown_limit", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * cacheの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, cache)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "cache", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * matchEscalationThresholdの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, matchEscalationThreshold)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "match_escalation_threshold", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * queryExpansionの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, queryExpansion)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "query_expansion", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * queryFlagsの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, queryFlags)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "query_flags", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * queryExpanderの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, queryExpander)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "query_expander", value)) {
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * adjusterの設定
 *
 * @access public
 * @param string   $value
 * @return self
 */
PHP_METHOD(GSelect, adjuster)
{
    groonga_select_t *self;
    char *value;
    uint value_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 変数の設定 */
    if (!prn_command_set(self->ctx, &self->command, "adjuster", value)) {
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
PHP_METHOD(GSelect, exec)
{
    groonga_select_t *self;
    zend_bool assoc = 0;
    zval *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &assoc) == FAILURE) {
        RETURN_FALSE;
    }
    self = (groonga_select_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コマンドの実行 */
    if (!prn_command_exec(self->ctx, &self->command, (int)assoc, (zval **)&retval)) {
        RETURN_FALSE;
    }
    RETURN_ZVAL(retval, 1, 1);
}

#   endif       /* #ifndef HAVE_PROONGA_CLASS_SELECT */
#endif      /* #ifndef HAVE_PROONGA_CLASS_SELECT_H */
