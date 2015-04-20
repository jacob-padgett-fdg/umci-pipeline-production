<?php

#---------------------------------------------------------
#
#	Check authentication and return login box
#	if no password was already given
#
#---------------------------------------------------------

session_name("$dbname-copylog");
session_register('lastjob');
session_register('lastuser');

$subnet=ereg_replace("\..*$","",$REMOTE_ADDR);

//echo "IP: $subnet<p>";

if ($subnet != $localnet) {
	echo "
	<body bgcolor=white>
	Error, This utility may only be used from UMC's local network.<p>
	Please contact a system administrator if you believe this is an error.<p>
	</body>";
	exit;
	}

if (!@mysql_connect($dbserver,$user_name,$user_pass)) {
	echo "<html><head><title>$dbdescription</title></head>
	<body bgcolor=#ffffff>
	<center><h2>Error Connecting to the database</h2></center><hr><p>
	<li>Please contact your network administrator for assistance.</li>
	</body>
	";	
	exit;
	}

mysql_select_db("$dbname");

?>
