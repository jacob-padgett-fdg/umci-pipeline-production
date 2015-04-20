<?
include("settings.cfg");
include("global_connect_rw.phtml");
include("querylib.inc");
include("access_libs.php");

/*
web_pass_set_try(2,'bunghole2','bunghole');

if (web_pass_validate(2,'bunghole')) {
	echo "password is valid";
	} else {
	echo "password is invalid";
	}
*/
//$otp_res=web_pass_send_otp_link("jeffb@umci.com"); // failed - 3 contacts
// $otp_res=web_pass_send_otp_link("cbond@umci.com"); // failed - umc employee
//$otp_res=web_pass_send_otp_link("tim@hudsonbayins.com","testing the request system");
/*$otp_res=web_pass_send_otp_link("jrbuck@gmail.com","testing the request system");

if ($otp_res) {
	echo "succeeded";
	} else {
	echo "failed";
	}
*/

echo "<tr><td colspan=2><a href=$pagename?mode=group_management><font color=blue>Manage Groups</font></a></td></tr>";







?>
