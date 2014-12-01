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

/* DB接続 */
$gdb = new Groonga('./db/test.db');

/**
 * table_createコマンド
 */
echo '/_/_/_/_ create _/_/_/_/'."\n";

$table = $gdb->table('Sample');
$table->flags    = "TABLE_HASH_KEY";
$table->key_type = "ShortText";

echo 'flags:'.$table->flags . "\n";
echo 'key_type:'.$table->key_type . "\n";

$table->create();
echo "\n-----------\n";


/**
 * table_renameコマンド
 */
echo '/_/_/_/_ rename _/_/_/_/'."\n";
$result = $table->rename('Test');
if (false !== $result) {
    echo "renamed\n";
}
echo "\n-----------\n";

/**
 * truncateコマンド
 */
echo '/_/_/_/_ truncate _/_/_/_/'."\n";
$result = $table->truncate();
if (false !== $result) {
    echo "truncated\n";
}
echo "\n-----------\n";

/**
 * columnListコマンド
 */
echo '/_/_/_/_ columnList _/_/_/_/'."\n";
$reslut = $table->columnList();
print_r($reslut);
echo "\n-----------\n";


/**
 * removeコマンド
 */
echo '/_/_/_/_ remove _/_/_/_/'."\n";
$result = $table->remove();
if (false !== $result) {
    echo "removed\n";
}
echo "\n-----------\n";


/**
 * dumpコマンド
 */
echo '/_/_/_/_ dump _/_/_/_/'."\n";
echo $table->dump();
echo "\n-----------\n";


/**
 * loadコマンド
 */
echo '/_/_/_/_ load _/_/_/_/'."\n";
$load = $table->load();
print_r($load);
echo "\n-----------\n";

/**
 * deleteコマンド
 */
echo '/_/_/_/_ delete _/_/_/_/'."\n";
$delete = $table->delete();
print_r($delete);
echo "\n-----------\n";

/**
 * selectコマンド
 */
echo '/_/_/_/_ select _/_/_/_/'."\n";
$select = $table->select();
print_r($select);
echo "\n-----------\n";


