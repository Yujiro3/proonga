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
extern zend_class_entry *groonga_table_ce;
extern zend_class_entry *groonga_column_ce;

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

/* In every utility function you add that needs to use variables 
   in php_groonga_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as GROONGA_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

/**
 * メンバー定数定義:定型マクロ
 */
#define REGISTER_CLASS_CONST_LONG(pce, const_name, value) zend_declare_class_constant_long(pce, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);
#define REGISTER_CLASS_CONST_STRING(pce, const_name, value) zend_declare_class_constant_stringl(pce, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);


#ifdef ZTS
#   define GROONGA_G(v) TSRMG(groonga_globals_id, zend_groonga_globals *, v)
#else
#   define GROONGA_G(v) (groonga_globals.v)
#endif

#ifndef HAVE_GROONGA_CLASS_GROONGA_H
#   include "classes/groonga.h"
#endif

#ifndef HAVE_GROONGA_CLASS_COMMAND_H
#   include "classes/command.h"
#endif

#endif    /* HAVE_PHP_PROONGA_H */

