<?
include("querylib.inc");
include("global_connect_rw.phtml");

$otp=mysql_real_escape_string($otp);
$login=mysql_real_escape_string($login);
$contact_info=getoneb("select * from contacts where login_name = '$login'");
if (!($contact_info)) {
	die_security("One time password connection attempted on invalid login_name.  This may be an application error, or someone trying to reverse engineer the access control system. fd2_access/reset.php.");
	}
$otp_query="select * from fd2_access_otp where pepper = '$otp' and contacts_id = '$contact_info->contacts_id' and expires > now() and accesses_remaining > 0";
$otp_info=getoneb($otp_query);


if (!($otp_info)) {
	security_log_email($contact_info->contacts_id,0,0,"Failed single use password attempt","fd2_access/reset.php");
	echo "I'm sorry, but the link that you've followed is either incorrect, or has expired.  Please request another reset link and try again. If you continue to have problems contact help@umci.com.";
	die();
	}

$newcount=$otp_info->accesses_remaining - 1;
//@mysql_query("update fd2_access_otp set accesses_remaining = '$newcount' where otp_id = '$otp_info->otp_id'");

echo "
<script>
function simple_validate() {
	pw1=document.password_reset.newpassword;
	pw2=document.password_reset.verifynewpassword;
	if (pw2!=pw1) {
		return(FALSE);
		alert('Passwords do not match! Please try again.');
		}
	}

</script>


Welcome to the reset screen

<hr>
<form onsubmit=\"simple_validate()\" name=password_reset method=post action=$pagename>
<input type=hidden name=otp value=\"$otp_info->pepper\">
<input type=hidden name=login value=\"$contact_info->login_name\">

<table border=0 cellspacing=0 cellpadding=5>
<tr><td>
	Login Name: 
</td><td>
	$contact_info->login_name
</td></tr>

<tr><td>
	New Password:
</td><td>
	<input type=password name=newpassword>
</td></tr>

<tr><td>
	Verify Password:
</td><td>
	<input type=password name=verifynewpassword>
</td></tr>

<tr><td colspan=2 align=right>
	<input type=submit>
</td></tr>
</form>

";








?>
