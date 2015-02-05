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
 * @package         groonga-php-driver
 * @copyright       Copyright (c) 2011-2014 sheeps.me
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 * @filesource
 */

#ifndef HAVE_PHP_GROONGA_H
#define HAVE_PHP_GROONGA_H

#define PRN_MATURITY                0
#define TABLE_MAX_KEY_SIZE          256                 // GRN_TABLE_MAX_KEY_SIZEだと大きすぎるので
#define COLUMN_MAX_KEY_SIZE         TABLE_MAX_KEY_SIZE
#define TYPE_TABLE_MASK             (0x3C)              // 
#define TYPE_IS_TABLE               (0x30)
#define PRIMARY_KEY_NAME            "_primary"
#define PRIMARY_KEY_LEN             (sizeof(PRIMARY_KEY_NAME) - 1)
#define GEO_POINT_SEPARATOR         "x"
#define JSON_PARSER_GROONGA_DEPTH   512

#define PHP_GROONGA_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#   define PHP_GROONGA_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#   define PHP_GROONGA_API __attribute__ ((visibility("default")))
#else
#   define PHP_GROONGA_API
#endif

#ifdef ZTS
#   include "TSRM.h"
#endif

extern zend_class_entry *groonga_database_ce;
extern zend_class_entry *groonga_command_ce;
extern zend_class_entry *groonga_delete_ce;
extern zend_class_entry *groonga_table_ce;
extern zend_class_entry *groonga_load_ce;
extern zend_class_entry *groonga_column_ce;
extern zend_class_entry *groonga_select_ce;

PHP_MINIT_FUNCTION(groonga);
PHP_MSHUTDOWN_FUNCTION(groonga);
PHP_RINIT_FUNCTION(groonga);
PHP_RSHUTDOWN_FUNCTION(groonga);
PHP_MINFO_FUNCTION(groonga);

/* 
    Declare any global variables you may need between the BEGIN
    and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(groonga)
    long  global_value;
    char *global_string;
ZEND_END_MODULE_GLOBALS(groonga)
*/

/**
 * 自身のオブジェクトを返すマクロ
 */
#define RETURN_CHAIN() do {\
    Z_SET_ISREF_P(return_value); \
    Z_TYPE_P(return_value) = IS_OBJECT; \
    ZVAL_ZVAL(return_value, getThis(), 1, 0);\
} while(0)

/**
 * メンバー定数定義:定型マクロ
 */
#define REGISTER_CLASS_CONST_LONG(pce, const_name, value) zend_declare_class_constant_long(pce, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);
#define REGISTER_CLASS_CONST_STRING(pce, const_name, value) zend_declare_class_constant_stringl(pce, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);

/**
 * メンバー関数呼び出し:定型マクロ
 */
#define PUSH_PARAM(arg) zend_vm_stack_push(arg TSRMLS_CC)
#define POP_PARAM() (void)zend_vm_stack_pop(TSRMLS_C)
#define PUSH_EO_PARAM()
#define POP_EO_PARAM()

#define CALL_METHOD_BASE(classname, name) zim_##classname##_##name

#define CALL_METHOD_HELPER(classname, name, retval, thisptr, num, param) \
    PUSH_PARAM(param); PUSH_PARAM((void*)num); \
    PUSH_EO_PARAM(); \
    CALL_METHOD_BASE(classname, name)(num, retval, NULL, thisptr, 0 TSRMLS_CC); \
    POP_EO_PARAM(); \
    POP_PARAM(); POP_PARAM();

#define CALL_METHOD(classname, name, retval, thisptr) \
    CALL_METHOD_BASE(classname, name)(0, retval, NULL, thisptr, 0 TSRMLS_CC);

#define CALL_METHOD1(classname, name, retval, thisptr, param1) \
    CALL_METHOD_HELPER(classname, name, retval, thisptr, 1, param1);

#define CALL_METHOD2(classname, name, retval, thisptr, param1, param2) \
    PUSH_PARAM(param1); \
    CALL_METHOD_HELPER(classname, name, retval, thisptr, 2, param2); \
    POP_PARAM();

#define CALL_METHOD3(classname, name, retval, thisptr, param1, param2, param3) \
    PUSH_PARAM(param1); PUSH_PARAM(param2); \
    CALL_METHOD_HELPER(classname, name, retval, thisptr, 3, param3); \
    POP_PARAM(); POP_PARAM();


#ifdef ZTS
#   define GROONGA_G(v) TSRMG(groonga_globals_id, zend_groonga_globals *, v)
#else
#   define GROONGA_G(v) (groonga_globals.v)
#endif

#ifndef HAVE_PROONGA_CLASS_GROONGA_H
#   include "classes/groonga.h"
#endif

#ifndef HAVE_PROONGA_CLASS_COMMAND_H
#   include "classes/command.h"
#endif

#ifndef HAVE_PROONGA_CLASS_DELETE_H
#   include "classes/delete.h"
#endif

#ifndef HAVE_PROONGA_CLASS_TABLE_H
#   include "classes/table.h"
#endif

#ifndef HAVE_PROONGA_CLASS_LOAD_H
#   include "classes/load.h"
#endif

#ifndef HAVE_PROONGA_CLASS_COLUMN_H
#   include "classes/column.h"
#endif

#ifndef HAVE_PROONGA_CLASS_SELECT_H
#   include "classes/select.h"
#endif

#endif    /* HAVE_PHP_GROONGA_H */

