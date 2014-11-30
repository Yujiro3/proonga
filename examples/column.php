<?php
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

/*
php -d extension=modules/groonga.so -f examples/table.php
*/

exec('rm -rf ./db/; mkdir ./db');

/* DB接続 */
$gdb = new Groonga('./db/test.db');

/* table_create --name Sample --flags TABLE_HASH_KEY --key_type ShortText */
$table = $gdb->table('Sample');
$table->flags    = "TABLE_HASH_KEY";
$table->key_type = "ShortText";
$table->create();


/* column_create --table Sample --name name --flags COLUMN_SCALAR --type ShortText */
$column = $table->column('name');
$column->flags = 'COLUMN_SCALAR';
$column->type  = 'ShortText';
$column->create();

echo '/_/_/_/_ dump _/_/_/_/'."\n";
echo $table->dump();
echo "\n-----------\n";


