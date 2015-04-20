<?
include("settings.cfg");
include("global_connect_rw.phtml");
include("querylib.inc");
include("access_libs.php");

$otp_res=web_pass_send_otp_link("jrbuck@gmail.com","testing the request system");

if ($otp_res) {
	echo "succeeded";
	} else {
	echo "failed";
	}

?>
