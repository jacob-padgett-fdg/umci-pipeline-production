<?

session_start();
/* TEMP CODE; REMOVE ($_SESSION debugging)
echo "Value was {$_SESSION['ABC']}<br/>";
$_SESSION['ABC'] = microtime();
echo "Value is {$_SESSION['ABC']}<br/>";
*/
if (isset($auth_backend_target_uri)) {
    $temp_uri = base64_decode($auth_backend_target_uri);
}

require_once("settings.cfg");
require_once("global-auth.inc");

//if ($adminuser) echo "ADMIN!";
if (isset($mode)) {
	$err=include($mode . ".phtml");
	if (!($err)) include("badmode.phtml");
	} else {
	include ("my_home.phtml");
	}
?>
