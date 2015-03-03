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
 * @package         proonga-php-driver
 * @copyright       Copyright (c) 2011-2014 sheeps.me
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 * @filesource
 */
#ifndef HAVE_PHP_PROONGA
#define HAVE_PHP_PROONGA

#include <groonga.h>

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_smart_str.h"
#include "ext/json/php_json.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"

#ifndef HAVE_PHP_GROONGA_H
#   include "php_groonga.h"
#endif

#ifndef HAVE_PROONGA_OBJECT_H
#   include "src/object.h"
#endif

#ifndef HAVE_PROONGA_COMMAND_H
#   include "src/command.h"
#endif

/* グローバル変数宣言 */
ZEND_DECLARE_MODULE_GLOBALS(groonga)

/* True global resources - no need for thread safety here */
static int le_groonga;

/* クラス構造体 */
zend_class_entry *groonga_database_ce = NULL;
zend_class_entry *groonga_command_ce = NULL;
zend_class_entry *groonga_delete_ce = NULL;
zend_class_entry *groonga_table_ce = NULL;
zend_class_entry *groonga_load_ce = NULL;
zend_class_entry *groonga_column_ce = NULL;
zend_class_entry *groonga_select_ce = NULL;
zend_class_entry *groonga_exception_ce = NULL;

/**
 * モジュール情報リスト
 *
 * @var zend_module_entry モジュール情報リスト
 */
zend_module_entry groonga_module_entry = {
    STANDARD_MODULE_HEADER,
    "groonga",
    NULL,
    PHP_MINIT(groonga),
    PHP_MSHUTDOWN(groonga),
    PHP_RINIT(groonga),        /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(groonga),    /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(groonga),
    PHP_GROONGA_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_GROONGA
    ZEND_GET_MODULE(groonga)
#endif

/**
 * 実行時設定
 *
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("groonga.dbpath", "/tmp", PHP_INI_ALL, OnUpdateString, dbpath, zend_groonga_globals, groonga_globals)
PHP_INI_END()

/**
 * グローバル変数の初期化処理
 *
 * @param module
 * @return int
 */
static void php_groonga_init_globals(zend_groonga_globals *groonga_globals)
{
    groonga_globals->initialized = 0;
    groonga_globals->gqtpConnected = 0;
}

/**
 * モジュールメイン初期化処理
 *
 * @param module
 * @return int
 */
PHP_MINIT_FUNCTION(groonga)
{
    zend_class_entry ce;

    REGISTER_INI_ENTRIES();
    ZEND_INIT_MODULE_GLOBALS(groonga, php_groonga_init_globals, NULL);

    /* groongaライブラリを初期化 */
    if (0 == GROONGA_G(initialized)) {
        if (GRN_SUCCESS != grn_init()) {
            return FAILURE;
        }
        GROONGA_G(initialized) = 1;
    }

    /* Groonga(db)クラスの登録 */
    INIT_CLASS_ENTRY(ce, "Groonga", groonga_class_methods);
    groonga_database_ce = zend_register_internal_class(&ce TSRMLS_CC);
    groonga_database_ce->create_object = groonga_ctor;

    /* GCommandクラスの登録 */
    INIT_CLASS_ENTRY(ce, "GCommand", groonga_command_class_methods);
    groonga_command_ce = zend_register_internal_class(&ce TSRMLS_CC);
    groonga_command_ce->create_object = groonga_command_ctor;

    /* GTableクラスの登録 */
    INIT_CLASS_ENTRY(ce, "GTable", groonga_table_class_methods);
    groonga_table_ce = zend_register_internal_class(&ce TSRMLS_CC);
    groonga_table_ce->create_object = groonga_table_ctor;

    /* GDeleteクラスの登録 */
    INIT_CLASS_ENTRY(ce, "GDelete", groonga_delete_class_methods);
    groonga_delete_ce = zend_register_internal_class(&ce TSRMLS_CC);
    groonga_delete_ce->create_object = groonga_delete_ctor;

    /* GLoadクラスの登録 */
    INIT_CLASS_ENTRY(ce, "GLoad", groonga_load_class_methods);
    groonga_load_ce = zend_register_internal_class(&ce TSRMLS_CC);
    groonga_load_ce->create_object = groonga_load_ctor;

    /* GColumnクラスの登録 */
    INIT_CLASS_ENTRY(ce, "GColumn", groonga_column_class_methods);
    groonga_column_ce = zend_register_internal_class(&ce TSRMLS_CC);
    groonga_column_ce->create_object = groonga_column_ctor;

    /* GSelectクラスの登録 */
    INIT_CLASS_ENTRY(ce, "GSelect", groonga_select_class_methods);
    groonga_select_ce = zend_register_internal_class(&ce TSRMLS_CC);
    groonga_select_ce->create_object = groonga_select_ctor;

    /* 例外用クラスの登録 */
    INIT_CLASS_ENTRY(ce, "GroongaException", NULL);
    groonga_exception_ce = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
    groonga_exception_ce->ce_flags |= ZEND_ACC_FINAL;
    groonga_exception_ce->constructor->common.fn_flags |= ZEND_ACC_PROTECTED;

    return SUCCESS;
}

/**
 * モジュールメインシャットダウン処理
 *
 * @param module
 * @return int
 */
PHP_MSHUTDOWN_FUNCTION(groonga)
{
    if (1 == GROONGA_G(initialized)) {
        grn_fin();
        GROONGA_G(initialized) = 0;
    }
    UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}

/**
 * モジュールリムーブ処理
 *
 * @param module
 * @return int
 */
PHP_RINIT_FUNCTION(groonga)
{
    return SUCCESS;
}

/**
 * モジュールリムーブ・シャットダウン処理
 *
 * @param module
 * @return int
 */
PHP_RSHUTDOWN_FUNCTION(groonga)
{
    return SUCCESS;
}

/**
 * モジュール情報設定
 *
 * @param module
 * @return int
 */
PHP_MINFO_FUNCTION(groonga)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "Groonga support", "enabled");
    php_info_print_table_row(2,    "Version",          PHP_GROONGA_VERSION);
    php_info_print_table_end();
}

#ifndef HAVE_PROONGA_CLASS_GROONGA
#   include "classes/groonga.h"
#endif

#ifndef HAVE_PROONGA_CLASS_COMMAND
#   include "classes/command.h"
#endif

#ifndef HAVE_PROONGA_CLASS_TABLE
#   include "classes/table.h"
#endif

#ifndef HAVE_PROONGA_CLASS_DELETE
#   include "classes/delete.h"
#endif

#ifndef HAVE_PROONGA_CLASS_LOAD
#   include "classes/load.h"
#endif

#ifndef HAVE_PROONGA_CLASS_COLUMN
#   include "classes/column.h"
#endif

#ifndef HAVE_PROONGA_CLASS_SELECT
#   include "classes/select.h"
#endif

#endif      // #ifndef HAVE_PHP_PROONGA
