Groongaクライアント
======================
The PHP bindings of Groonga.

利用方法
------

### Groongaライブラリのインストール ###
    
    $ sudo aptitude install -y libgroonga0 libgroonga-dev

### proongaのインストール ###
    
    $ git clone https://github.com/Yujiro3/proonga.git
    $ cd ./proonga
    $ phpize
    $ ./configure
    $ make
    $ sudo -s
    # make install
    # cd /etc/php5/mods-available
    # echo extension=groonga.so > proonga.ini
    # cd /etc/php5/conf.d
    # ln -s ../mods-available/proonga.ini ./30-proonga.ini
    
内部でphp_json_decode_exを使用しています。

json-1.2.1を読み込んでからproongaを読み込むように
設定してください。
    
### 接続 ###

```php
<?php
/* オブジェクト生成 */
$gdb = new Groonga('./db/test.db');
```

### テーブルの作成 ###

```php
/* table_create --name Users --flags TABLE_HASH_KEY --key_type ShortText */
$gdb->table('Users')
    ->flags('TABLE_HASH_KEY')
    ->keyType('ShortText')
    ->create();
```

### カラムの作成 ###

```php
/* column_create --table Users --name name --flags COLUMN_SCALAR --type ShortText */
$gdb->table('Users')
    ->column('name')
    ->flags('COLUMN_SCALAR')
    ->type('ShortText')
    ->create();
```

### データのロード ###

```php
/* load --table Users --values [[...],[...],...] */
$values = <<< JSON
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
$gdb->table('Users')
    ->load($values);
```

### データ一覧の取得 ###

```php
/* select --table Users --match_columns name,location_str,description --query "New York" --output_columns _key,name */
$result = $gdb->table('Users')
    ->select()
    ->matchColumns('name,location_str,description')
    ->query('"New York"')
    ->outputColumns('_key,name')
    ->exec(true);

echo json_encode($result, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE | JSON_BIGINT_AS_STRING);
```

### 出力結果 ###

```    
[
    [
        [
            1
        ],
        [
            [
                "_key",
                "ShortText"
            ],
            [
                "name",
                "ShortText"
            ]
        ],
        [
            "bob",
            "Bob"
        ]
    ]
]

```

    
    

ライセンス
----------
Copyright &copy; 2014 Yujiro Takahashi  
Licensed under the [MIT License][MIT].  
Distributed under the [MIT License][MIT].  

[MIT]: http://www.opensource.org/licenses/mit-license.php
