<?
if (!($CUSTLIB)) {
$CUSTLIB=1;
 #-------------------------------------------------------------------------
 #-------------------------------------------------------------------------
 #-------------------------------------------------------------------------
 #-------------------------------------------------------------------------

function custbox($query,$defaultid="",$size="1",$onchange="",$blank=0){
$result=@mysql_query("$query");
$fields=@mysql_num_fields($result);
$row=(@mysql_fetch_row($result));
$valuename=mysql_field_name($result,0);
$selectlist=" ";
while ($row) {
	$mark="";
	$selecttext="";
	if ("$defaultid" == "$row[0]") { 
		$mark=" selected"; 
		}
	$fcount=1;
	$selecttext="";
	while ($fcount <= $fields) {
		$selecttext="$selecttext$row[$fcount]";
		$fcount++;
		}
	
	$selectlist= $selectlist . "<option value=\"$row[0]\"$mark>$selecttext";
	$row=(@mysql_fetch_row($result));
	}

$firstline="";
if ($blank >= 1){
	$firstline="<option value=\"\"> ";
	}

if (($defaultid == "")||($defaultid == "0")) {
	echo "	<select name=\"$valuename\" onchange=\"$onchange\" size=$size>
		$firstline
		$selectlist
		</select>";
	} else {
	echo "	<select name=\"$valuename\" onchange=\"$onchange\" size=$size>
		$selectlist
		</select>";
	}
}

#-------------------------------------------------------------------------

function custgetid($name) {
global $customers;
//$name=htmlspecialchars($name);
//$name=escapeshellcmd($name);
$result=mysql_query("select * from $customers where customer_name = '$name'");
if (mysql_num_rows($result)==1) {
	@mysql_data_seek($result,0);
	$data=mysql_fetch_object($result);
	return $data->customer_id;
	}
return "";
}

#-------------------------------------------------------------------------

function custgetname($id) {
global $customers;
$result=mysql_query("select * from $customers where customer_id = '$id'");
if (mysql_num_rows($result)==1) {
	@mysql_data_seek($result,0);
	$data=mysql_fetch_object($result);
	return $data->customer_name;
	}
return "";
}

#-------------------------------------------------------------------------

function custcheck($name) {
global $customers;

$result=mysql_query("select * from $customers where customer_name = '$name'");
//echo "select * from $customers where customer_name = '$name'";exit;

if (mysql_num_rows($result)==1) {
	return 1;
	}
return 0;
}

#-------------------------------------------------------------------------

function customerguess ($name) {
global $customers;
$shortname=substr($name,0,3);
$query="select customer_name,customer_name from $customers where 
		customer_name like '$shortname%'
		or
		customer_name like '%$name%'
		or
		soundex(customer_name) = soundex('$name')";
custbox($query,"",7);
}


#-------------------------------------------------------------------------
#-------End curly brace to mark CUSTLIB "if"------------------------------
#-------------------------------------------------------------------------
}
?>
