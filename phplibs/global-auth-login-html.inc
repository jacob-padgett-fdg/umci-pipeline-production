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

$band_top=$fd_color_2;
$band_low=$fd_color_4;

$band_top="#666666";
$band_low="#CCCCCC";

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
<table cellspacing=0 cellpadding=0 style=\"outline: 1px solid black; cell-padding: 0px; padding: 0px;\">
<tr><td style=\"height: 40px;\" bgcolor=$band_top>&nbsp;</td></tr>

<tr><td align=center>
	<img src=\"
	";

// GPH MARK - absolute path
$dir = '/data/web/pipeline/_images/rotate/';
if (file_exists($dir) && is_dir($dir)) {
    $list=scandir($dir);
    while (($show==$last_shown)||($show=="")||("$show"=="thumbs.db")) {
        $len=sizeof($list);
        srand();
        $num=rand(3,$len);
        $show=$list[$num -1];
    }
    $last_shown=$show;
    echo "/_images/rotate/${show}";
}

	echo "\" width=750 height=200 border=0>
</td></tr>

<tr><td style=\"height: 30px;\" bgcolor=$band_low>&nbsp;</td></tr>

<tr><td>&nbsp;</td></tr>

<tr><td>

<div style=\"float: right;\"><img src=\"/images/umc_logo.png\" style='height:28px;width:28px;'></div>

<center>
<div>
	<table border=0 cellspacing=0 cellpadding=10>
	<tr><td>
		<b>Login</b>
	</td><td>
		<input type=text name=auth_user_name value=\"$umcgloballoginid\" size=10>
	</td><td>
		<b>Password</b>
	</td><td>
		<input type=password name=auth_user_pass size=10>
	</td><td colspan=2 align=center>
		<input type=submit value=\"Login\">
	</td></tr>
	</table>
</div>
</center>

</form>
</td></tr>
<tr><td>&nbsp;</td></tr>
</table>



</center>




<script>
document.loginform.$focusfield.focus();
</script>
</body>
</html>

";
// GPH TEMP CODE
////////exit;
