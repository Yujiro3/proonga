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

#ifndef HAVE_GROONGA_OBJECT
#define HAVE_GROONGA_OBJECT

#include <groonga.h>
#include "php.h"

#ifndef HAVE_PHP_GROONGA_H
#   include "php_groonga.h"
#endif

#ifndef HAVE_GROONGA_OBJECT_H
#   include "src/object.h"
#endif

/**
 * Groongaクラスのメモリ解放
 *
 * @param groonga_t *self Groongaオブジェクト
 * @return void
 */
static void groonga_free_storage(groonga_t *self TSRMLS_DC)
{
    efree(self->ctx);
    self->ctx = NULL;

    /* オブジェクトの破棄 */
    zend_object_std_dtor(&self->std TSRMLS_CC);
    efree(self);
}

/**
 * Groongaクラスのメモリ確保
 *
 * @param zend_class_entry *ce クラスエントリ
 * @return zend_object_value
 */
zend_object_value groonga_ctor(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    groonga_t *self;

    self = ecalloc(1, sizeof(*self));

    zend_object_std_init(&self->std, ce TSRMLS_CC);
    object_properties_init(&self->std, ce);
    rebuild_object_properties(&self->std);

    self->ctx   = emalloc(sizeof(*self->ctx));
    self->db    = NULL;

    retval.handle = zend_objects_store_put(
        self, 
        (zend_objects_store_dtor_t)zend_objects_destroy_object, 
        (zend_objects_free_object_storage_t)groonga_free_storage, 
        NULL TSRMLS_CC
    );
    retval.handlers = zend_get_std_object_handlers();

    return retval;
}

/**
 * GCommandクラスのメモリ解放
 *
 * @param roonga_column_t *self カラムオブジェクト
 * @return void
 */
static void groonga_command_free_storage(groonga_command_t *self TSRMLS_DC)
{
    /* カラムリストの開放 */
    zend_object_std_dtor(&self->std TSRMLS_CC);
    efree(self);
}

/**
 * GCommandクラスのメモリ確保
 *
 * @param zend_class_entry *ce クラスエントリ
 * @return zend_object_value
 */
zend_object_value groonga_command_ctor(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    groonga_t *self;

    self = ecalloc(1, sizeof(*self));

    zend_object_std_init(&self->std, ce TSRMLS_CC);
    object_properties_init(&self->std, ce);
    rebuild_object_properties(&self->std);


    retval.handle = zend_objects_store_put(
        self, 
        (zend_objects_store_dtor_t)zend_objects_destroy_object, 
        (zend_objects_free_object_storage_t)groonga_command_free_storage, 
        NULL TSRMLS_CC
    );
    retval.handlers = zend_get_std_object_handlers();

    return retval;
}

#endif
