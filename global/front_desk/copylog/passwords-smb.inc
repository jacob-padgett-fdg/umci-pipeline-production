<?php

#---------------------------------------------------------
#
#	Check authentication and return login box
#	if no password was already given
#
#---------------------------------------------------------

session_name("$dbname");

if ($kill_session_info) {
	$tmpu=$user_name;
	$tmpp=$user_pass;
	session_start();
	session_destroy();
	session_name("$dbname");
	session_start();
	session_register("user_name");
	session_register("user_pass");
	session_register("user_name_real");
	session_register("login_status");
	$user_name=$tmpu;
	$user_pass=$tmpp;
	$login_status="";
	}

session_register("user_name");
session_register("user_pass");
session_register("login_status");

$smb_user_name=escapeshellcmd($smb_user_name);
$smb_user_pass=escapeshellcmd($smb_user_pass);

if ($login_status!="logged in") {
	putenv('DISPLAY=');
	//echo "/usr/bin/smbcheckpass \"$smb_user_name\" \"$smb_user_pass\"";
	$smb_auth_response=exec("/usr/bin/smbcheckpass \"$smb_user_name\" \"$smb_user_pass\"");
	if ($smb_auth_response=="YES") $login_status="logged in";
	}
	
if ($login_status!="logged in") {
	echo "<html><head><title>$dbdescription</title></head>
	<body bgcolor=#ffffff>
	<center><h2>Please log in to $dbdescription</h2></center><p><hr>
	<form name=loginform method=post action=$pagename>
	Username:<br>
	<input type=text name=smb_user_name size=10><br>
	Password:<br>
	<input type=password name=smb_user_pass size=10 onchange=submit();><br>
	<input type=hidden name=SMB_AUTH_REQUEST value=\"true\">
	<input type=hidden name=kill_session_info value=1>
	<input type=submit value=\"Login\">
	<script>
	document.loginform.smb_user_name.focus();
	</script>
	";	
	exit;
	}

?>
