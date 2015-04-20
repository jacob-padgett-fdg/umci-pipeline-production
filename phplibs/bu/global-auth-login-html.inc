<?
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Below is the actual rendered page. This can be modified to change the
// look of the login page as needed (and it's really needed honestly).
//
// Just be carefull not to mess with any variable names, or variables
// (name = , or $<whatever>'s). And leave the tiny script at the end
// to improve usability..
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
echo "


<html><head><title>$dbdescription</title></head>
<body bgcolor=#ffffff>

<form name=loginform method=post action=$pagename>
<input type=hidden name=auth_backend_target_uri value=\"$auth_backend_target_uri\">
<input type=hidden name=SYSTEM_AUTH_REQUESTED value=1>
<input type=hidden name=kill_session_info value=1>






<center><h2>Please log in to $dbdescription</h2></center><p><hr>
Username:<br>
<input type=text name=auth_user_name value=\"$umcgloballoginid\" size=10><br>
Password:<br>
<input type=password name=auth_user_pass size=10 onchange=submit();><br>
<input type=submit value=\"Login\">


<script>
document.loginform.$focusfield.focus();
</script>
";	
exit;
?>
