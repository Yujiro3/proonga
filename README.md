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
    
### 接続 ###

```php
<?php
/* オブジェクト生成 */
$grn = new Groonga('./db/test.db');
```

### テーブルの作成 ###

```php
/* table_create --name Users --flags TABLE_HASH_KEY --key_type ShortText */
$Users = $grn->table(
    'Users', 
    Groonga::TABLE_HASH_KEY | Groonga::PERSISTENT,
    'ShortText'
);
```

### カラムの作成 ###

```php
/* column_create --table Users --name name --flags COLUMN_SCALAR --type ShortText */
$name = $Users->column(
    'name', 
    Groonga::COLUMN_SCALAR | Groonga::PERSISTENT,
    'ShortText'
);

/* column_create --table Users --name location_str --flags COLUMN_SCALAR --type ShortText */
$location_str = $Users->column(
    'location_str', 
    Groonga::COLUMN_SCALAR | Groonga::PERSISTENT,
    'ShortText'
);

```

### データのロード ###

```php
$data = array(
    array(
        "_key"=> "daijiro",
        "name"=> "hsiomaneki",
        "location"=> "127678039x502643091",
        "location_str"=> "神奈川県",
        "description"=> "groonga developer"
    ),
    array(
        "_key"=> "tasukuchan",
        "name"=> "グニャラくん",
        "location"=> "128423343x502929252",
        "location_str"=> "東京都渋谷区",
        "description"=> "エロいおっさん"
    ),
    array(
        "_key"=> "OffGao",
        "name"=> "OffGao",
        "location"=> "128544408x502801502",
        "location_str"=> "東京都中野区",
        "description"=> "がおがお"
    ),
);

$Users->load($data);

```

### データ一覧の取得 ###

```php
/* select --table Users */
$cursor = $Users->select();
while (0 != $cursor->next()) {
    echo $cursor->getKey()." => \n";
    $row = $cursor->getValue(true);
    print_r($row);
    echo ",\n";
}
```

### 出力結果 ###

```    
daijiro =>
Array
(
    [description] => groonga developer
    [location] => 127678039x502643091
    [location_str] => 神奈川県
    [name] => hsiomaneki
),
tasukuchan =>
Array
(
    [description] => エロいおっさん
    [location] => 128423343x502929252
    [location_str] => 東京都渋谷区
    [name] => グニャラくん
),
OffGao =>
Array
(
    [description] => がおがお
    [location] => 128544408x502801502
    [location_str] => 東京都中野区
    [name] => OffGao
),

```


### 検索データの取得 ###

```php
/* select --table Users --match_columns location_str,name --query 東京 */
$cursor = $Users->select();
$cursor->query('東京')->matchColumns->('location_str,name')->execute();
while (0 != $cursor->next()) {
    echo $cursor->getKey()." => \n";
    $row = $cursor->getValue(true);
    print_r($row);
    echo ",\n";
}
```

### 出力結果 ###

```    
2 =>
Array
(
    [description] => エロいおっさん
    [location] => 128423343x502929252
    [location_str] => 東京都渋谷区
    [name] => グニャラくん
),
3 =>
Array
(
    [description] => がおがお
    [location] => 128544408x502801502
    [location_str] => 東京都中野区
    [name] => OffGao
),

```


### フィルタリングデータの取得 ###

```php
/* select --table Users --filter geo_in_circle(location,"128484216x502919856",5000) */
$Users  = $grn->Users;
$cursor = $Users->select()->filter('geo_in_circle(location,"128484216x502919856",5000)');
$row = $cursor->execute();
while (0 != $cursor->next()) {
    echo $cursor->getKey()." => \n";
    $row = $cursor->getValue(true);
    print_r($row);
    echo ",\n";
}
```

### 出力結果 ###

```    
2 =>
Array
(
    [description] => エロいおっさん
    [location] => 128423343x502929252
    [location_str] => 東京都渋谷区
    [name] => グニャラくん
),
3 =>
Array
(
    [description] => がおがお
    [location] => 128544408x502801502
    [location_str] => 東京都中野区
    [name] => OffGao
),

```
    
    

ライセンス
----------
Copyright &copy; 2014 Yujiro Takahashi  
Licensed under the [MIT License][MIT].  
Distributed under the [MIT License][MIT].  

[MIT]: http://www.opensource.org/licenses/mit-license.php
