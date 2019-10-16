<?php

$action = $_GET['action'];

switch($action) {
case 'init_data_list':
	init_data_list();
	break;
case 'add_row':
	add_row();
	break;
case 'del_row':
	del_row();
	break;
case 'edit_row':
	edit_row();
	break;
}

function add_row(){
	if(empty($_POST)){
		echo "METHOD NOT ALLOWED >>>> ";	
	}
	$sql = "INSERT INTO `et_data` (";
	$v = "(";
	for($i = 0;$i < 8 ; $i++){
		$sql .= "`c_" . chr(97+$i) . '`,';
		$v .= '"' . addslashes($_POST[ 'c_' . $i]) . '",';
	}
	$sql = trim($sql,",");
	$v = trim($v,",");
	$sql .= ") VALUES " . $v . ")";
	$res = query_sql($sql,"SELECT LAST_INSERT_ID() as LD");
	$d = $res->fetch_assoc();
	echo $d["LD"];
}

function del_row(){
	if(empty($_POST)){
		echo "METHOD NOT ALLOWED >>>> ";	
	}
	$rowId = $_POST["delId"];
	$sql = "DELETE FROM `et_data` WHERE `id` = " . $rowId;
	$res = query_sql($sql);
	if($res){
		echo "OK";
	} else {
		echo "failed";	
	}
}

function edit_row(){
	if(empty($_POST)){
		echo "METHOD NOT ALLOWED >>>> ";	
	}
	$sql = "UPDATE `et_data` SET ";
	for($i = 0;$i < 8 ; $i++){
		
		$n = "`c_" . chr(97+$i) . '` ';
		$v = ' "' . addslashes($_POST[ 'c_' . $i]) . '"';
		$sql .= $n . ' = ' . $v . " , ";
	}
	$sql = trim($sql,", ");
	$sql .= " WHERE `id` = " . $_POST["id"];
	$res = query_sql($sql);
	if($res){
		echo "OK";	
	} else {
		echo "failed";	
	}
}

function init_data_list(){
	$data = array();
	$query = query_sql('select * from `et_data` order by `id` asc');
	while($row = $query->fetch_assoc()){
		$data[] = $row;
	}
	echo json_encode($data,true);
}

function query_sql(){
	$mysqli = new mysqli("127.0.0.1", "shaddow", "123456", "etable");
	$sqls = func_get_args();
	foreach($sqls as $s){
		$query = $mysqli->query($s);
	}
	$mysqli->close();
	return $query;
}


