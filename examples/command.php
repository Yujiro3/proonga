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
php -d extension=modules/groonga.so -f examples/command.php
*/

exec('rm -rf ./db/; mkdir ./db');

/* DB接続 */
$gdb = new Groonga('./db/test.db');

/**
 * table_createコマンド
 */
$table_create = $gdb->command('table_create');

/* table_create --name Users --flags TABLE_HASH_KEY --key_type ShortText */
$table_create->name     = 'Users';
$table_create->flags    = 'TABLE_HASH_KEY';
$table_create->key_type = 'ShortText';
$table_create->exec();

/* table_create --name Comments --flags TABLE_HASH_KEY --key_type ShortText */
$table_create->name     = 'Comments';
$table_create->flags    = 'TABLE_HASH_KEY';
$table_create->key_type = 'ShortText';
$table_create->exec();

/* table_create --name HashTags --flags TABLE_HASH_KEY --key_type ShortText */
$table_create->name     = 'HashTags';
$table_create->flags    = 'TABLE_HASH_KEY';
$table_create->key_type = 'ShortText';
$table_create->exec();

/* table_create --name Bigram --flags TABLE_PAT_KEY|KEY_NORMALIZE --key_type ShortText --default_tokenizer TokenBigram */
$table_create->name     = 'Bigram';
$table_create->flags    = 'TABLE_PAT_KEY|KEY_NORMALIZE';
$table_create->key_type = 'ShortText';
$table_create->default_tokenizer = 'TokenBigram';
$table_create->exec();

/* table_create --name GeoIndex --flags TABLE_PAT_KEY --key_type WGS84GeoPoint */
$table_create->name     = 'GeoIndex';
$table_create->flags    = 'TABLE_PAT_KEY';
$table_create->key_type = 'WGS84GeoPoint';
$table_create->exec();

/**
 * column_createコマンド
 */
$column_create = $gdb->command('column_create');

/* column_create --table Users --name name --flags COLUMN_SCALAR --type ShortText */
$column_create->table = 'Users';
$column_create->name  = 'name';
$column_create->flags = 'COLUMN_SCALAR';
$column_create->type  = 'ShortText';
$column_create->exec();

/* column_create --table Users --name follower --flags COLUMN_VECTOR --type Users */
$column_create->table = 'Users';
$column_create->name  = 'follower';
$column_create->flags = 'COLUMN_VECTOR';
$column_create->type  = 'Users';
$column_create->exec();

/* column_create --table Users --name favorites --flags COLUMN_VECTOR --type Comments */
$column_create->table = 'Users';
$column_create->name  = 'favorites';
$column_create->flags = 'COLUMN_VECTOR';
$column_create->type  = 'Comments';
$column_create->exec();

/* column_create --table Users --name location --flags COLUMN_SCALAR --type WGS84GeoPoint */
$column_create->table = 'Users';
$column_create->name  = 'location';
$column_create->flags = 'COLUMN_SCALAR';
$column_create->type  = 'WGS84GeoPoint';
$column_create->exec();

/* column_create --table Users --name location_str --flags COLUMN_SCALAR --type ShortText */
$column_create->table = 'Users';
$column_create->name  = 'location_str';
$column_create->flags = 'COLUMN_SCALAR';
$column_create->type  = 'ShortText';
$column_create->exec();

/* column_create --table Users --name description --flags COLUMN_SCALAR --type ShortText */
$column_create->table = 'Users';
$column_create->name  = 'description';
$column_create->flags = 'COLUMN_SCALAR';
$column_create->type  = 'ShortText';
$column_create->exec();

/* column_create --table Users --name followee --flags COLUMN_INDEX --type Users --source follower */
$column_create->table  = 'Users';
$column_create->name   = 'followee';
$column_create->flags  = 'COLUMN_INDEX';
$column_create->type   = 'Users';
$column_create->source = 'follower';
$column_create->exec();



/* column_create --table Comments --name comment --flags COLUMN_SCALAR --type ShortText */
$column_create->table = 'Comments';
$column_create->name  = 'comment';
$column_create->flags = 'COLUMN_SCALAR';
$column_create->type  = 'ShortText';
$column_create->exec();

/* column_create --table Comments --name last_modified --flags COLUMN_SCALAR --type Time */
$column_create->table = 'Comments';
$column_create->name  = 'last_modified';
$column_create->flags = 'COLUMN_SCALAR';
$column_create->type  = 'Time';
$column_create->exec();

/* column_create --table Comments --name replied_to --flags COLUMN_SCALAR --type Comments */
$column_create->table = 'Comments';
$column_create->name  = 'replied_to';
$column_create->flags = 'COLUMN_SCALAR';
$column_create->type  = 'Comments';
$column_create->exec();

/* column_create --table Comments --name replied_users --flags COLUMN_VECTOR --type Users */
$column_create->table = 'Comments';
$column_create->name  = 'replied_users';
$column_create->flags = 'COLUMN_VECTOR';
$column_create->type  = 'Users';
$column_create->exec();

/* column_create --table Comments --name hash_tags --flags COLUMN_VECTOR --type HashTags */
$column_create->table = 'Comments';
$column_create->name  = 'hash_tags';
$column_create->flags = 'COLUMN_VECTOR';
$column_create->type  = 'HashTags';
$column_create->exec();

/* column_create --table Comments --name location --flags COLUMN_SCALAR --type WGS84GeoPoint */
$column_create->table = 'Comments';
$column_create->name  = 'location';
$column_create->flags = 'COLUMN_SCALAR';
$column_create->type  = 'WGS84GeoPoint';
$column_create->exec();

/* column_create --table Comments --name posted_by --flags COLUMN_SCALAR --type Users */
$column_create->table = 'Comments';
$column_create->name  = 'posted_by';
$column_create->flags = 'COLUMN_SCALAR';
$column_create->type  = 'Users';
$column_create->exec();

/* column_create --table Comments --name favorited_by --flags COLUMN_INDEX --type Users --source favorites */
$column_create->table  = 'Comments';
$column_create->name   = 'favorited_by';
$column_create->flags  = 'COLUMN_INDEX';
$column_create->type   = 'Users';
$column_create->source = 'favorites';
$column_create->exec();


/* column_create --table HashTags --name hash_index --flags COLUMN_INDEX --type Comments --source hash_tags */
$column_create->table  = 'HashTags';
$column_create->name   = 'hash_index';
$column_create->flags  = 'COLUMN_INDEX';
$column_create->type   = 'Comments';
$column_create->source = 'hash_tags';
$column_create->exec();


/* column_create --table Bigram --name users_index --flags COLUMN_INDEX|WITH_POSITION|WITH_SECTION --type Users --source name,location_str,description */
$column_create->table  = 'Bigram';
$column_create->name   = 'users_index';
$column_create->flags  = 'COLUMN_INDEX|WITH_POSITION|WITH_SECTION';
$column_create->type   = 'Users';
$column_create->source = 'name,location_str,description';
$column_create->exec();

/* column_create --table Bigram --name comment_index --flags COLUMN_INDEX|WITH_POSITION --type Comments --source comment */
$column_create->table  = 'Bigram';
$column_create->name   = 'comment_index';
$column_create->flags  = 'COLUMN_INDEX|WITH_POSITION';
$column_create->type   = 'Comments';
$column_create->source = 'comment';
$column_create->exec();

/* column_create --table GeoIndex --name users_location --type Users --flags COLUMN_INDEX --source location */
$column_create->table  = 'GeoIndex';
$column_create->name   = 'users_location';
$column_create->flags  = 'COLUMN_INDEX';
$column_create->type   = 'Users';
$column_create->source = 'location';
$column_create->exec();

/* column_create --table GeoIndex --name comments_location --type Comments --flags COLUMN_INDEX --source location */
$column_create->table  = 'GeoIndex';
$column_create->name   = 'comments_location';
$column_create->flags  = 'COLUMN_INDEX';
$column_create->type   = 'Comments';
$column_create->source = 'location';
$column_create->exec();

/**
 * column_createコマンド
 */
$load = $gdb->command('load');

/* load --table Users --values [[...],[...],...] */
$load->table  = 'Users';
$load->values = <<< JSON
[
  {
    "_key": "alice",
    "name": "Alice",
    "follower": ["bob"],
    "favorites": [],
    "location": "152489000x-255829000",
    "location_str": "Boston, Massachusetts",
    "description": "Groonga developer"
  },
  {
    "_key": "bob",
    "name": "Bob",
    "follower": ["alice","charlie"],
    "favorites": ["alice:1","charlie:1"],
    "location": "146249000x-266228000",
    "location_str": "Brooklyn, New York City",
    "description": ""
  },
  {
    "_key": "charlie",
    "name": "Charlie",
    "follower": ["alice","bob"],
    "favorites": ["alice:1","bob:1"],
    "location": "146607190x-267021260",
    "location_str": "Newark, New Jersey",
    "description": "Hmm,Hmm"
  }
]
JSON;
$load->exec();

/* $load --table Comments --values [[...],[...],...] */
$load->table  = 'Comments';
$load->values = <<< JSON
[
  {
    "_key": "alice:1",
    "comment": "I've created micro-blog!",
    "last_modified": "2010/03/17 12:05:00",
    "posted_by": "alice",
  },
  {
    "_key": "bob:1",
    "comment": "First post. test,test...",
    "last_modified": "2010/03/17 12:00:00",
    "posted_by": "bob",
  },
  {
    "_key": "alice:2",
    "comment": "@bob Welcome!!!",
    "last_modified": "2010/03/17 12:05:00",
    "replied_to": "bob:1",
    "replied_users": ["bob"],
    "posted_by": "alice",
  },
  {
    "_key": "bob:2",
    "comment": "@alice Thanks!",
    "last_modified": "2010/03/17 13:00:00",
    "replied_to": "alice:2",
    "replied_users": ["alice"],
    "posted_by": "bob",
  },
  {
    "_key": "bob:3",
    "comment": "I've just used 'Try-Groonga' now! #groonga",
    "last_modified": "2010/03/17 14:00:00",
    "hash_tags": ["groonga"],
    "location": "146566000x-266422000",
    "posted_by": "bob",
  },
  {
    "_key": "bob:4",
    "comment": "I'm come at city of New York for development camp! #groonga #travel",
    "last_modified": "2010/03/17 14:05:00",
    "hash_tags": ["groonga", "travel"],
    "location": "146566000x-266422000",
    "posted_by": "bob",
  },
  {
    "_key": "charlie:1",
    "comment": "@alice @bob I've tried to register!",
    "last_modified": "2010/03/17 15:00:00",
    "replied_users": ["alice", "bob"],
    "location": "146607190x-267021260",
    "posted_by": "charlie",
  }
  {
    "_key": "charlie:2",
    "comment": "I'm at the Museum of Modern Art in NY now!",
    "last_modified": "2010/03/17 15:05:00",
    "location": "146741340x-266319590",
    "posted_by": "charlie",
  }
]
JSON;
$load->exec();

/**
 * selectコマンド
 */
$select = $gdb->command('select');

/* select --table Users --match_columns name,location_str,description --query "New York" --output_columns _key,name */
$select->table          = 'Users';
$select->match_columns  = 'name,location_str,description';
$select->query          = '"New York"';
$select->output_columns = '_key,name';
$result = $select->exec(true);

echo json_encode($result, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE | JSON_BIGINT_AS_STRING);
