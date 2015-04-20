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
<html>
<head>
<title>$dbdescription</title>
</head>

<body bgcolor=#ffffff>

<form name=loginform method=post action=$pagename>
<input type=hidden name=auth_backend_target_uri value=\"$auth_backend_target_uri\">
<input type=hidden name=SYSTEM_AUTH_REQUESTED value=1>
<input type=hidden name=kill_session_info value=1>




<center>
<table style=\"border: 1px solid black; padding: 20px; padding-bottom: 40px;\" cellspacing=0>
<tr><td colspan=2 align=center>
	<img src=/images/fd_umc_logo.png>
</td></tr>

<tr><td colspan=2>&nbsp;</td></tr>

<tr><td>
	Login
</td><td>
	<input type=text name=auth_user_name value=\"$umcgloballoginid\" size=10>
</td></tr>

<tr><td>
	Password
</td><td>
	<input type=password name=auth_user_pass size=10 onchange=submit();>
</td></tr>

<tr><td colspan=2>&nbsp;</td></tr>

<tr><td align=center colspan=2>
	<input type=submit value=\"Login\">
</td></tr>
</table>
<p>
<div style=\"background: #eeeeee; border: 1px solid black; width: 300px; padding: 20px;\">
<font color=red><b>This is the new login screen for Front Desk.  It functions just as it always has. Watch
for security enhancements to be enabled soon.</b>
</font>
</div>

			

</center>




<script>
document.loginform.$focusfield.focus();
</script>
</body>
</html>

";	
exit;
?>
