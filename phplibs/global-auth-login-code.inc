<?

if ($umcgloballoginid=="") $focusfield="auth_user_name";
else $focusfield="auth_user_pass";
$alt_global_var="umcgloballoginid$application_name";
eval("\$alt_global_login=\$$alt_global_var;");
if ($alt_global_login!="") $umcgloballoginid=$alt_global_login;

///////////////////////////////////////////////////////////////////
// This is used to mark where we should be going back to after any
// authentication madness that may occur
///////////////////////////////////////////////////////////////////
if ($auth_backend_target_uri!="") {
	$auth_backend_target_uri=addslashes($auth_backend_target_uri);
	} else {
	$auth_backend_target_uri=base64_encode($GLOBALS['REQUEST_URI']);
	}
// Include the actual login page's html
include('global-auth-login-html.inc');
exit;
?>
