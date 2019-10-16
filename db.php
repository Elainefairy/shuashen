<meta charset="UTF-8">
 
<?php


$dbName = 'ginsheng';

$host = '127.0.0.1';

$user = 'root';

$password = '123456';

 

$dsn = "mysql:host=$host;dbname=$dbName";

$pdo = new PDO($dsn, $user, $password);

 

function sql($table, $field = '*', $where = '')

{

    global $pdo;

    $sql = 'select' . ' ' . $field . ' ' . 'from' . ' ' . $table . ' where ' . $where;

    $data = $pdo->query($sql)->fetch();

    return $data;

}