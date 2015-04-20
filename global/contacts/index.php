<?
require("settings.cfg");
//require("passwords.inc");
//require("global-pw.inc");
require("global-auth.inc");
require("contacts_lib.inc");

if ($contacts_last_main_mode=="") {
	session_register('contacts_last_main_mode');
	$contacts_last_main_mode="main";
	}

if (isset($mode)) {
	$err=include($mode . ".phtml");
	if (!($err)) include("badmode.phtml");
	} else {
	include ("main.phtml");
	}
?>
