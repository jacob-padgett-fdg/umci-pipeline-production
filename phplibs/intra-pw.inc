<?

$auth_object->check_web_pw='Y';
$auth_object->check_smb_pw='Y';
$auth_object->check_webfile_exceptions='N';
$auth_object->check_access_permission='N';

$auth_object->require_login='N';
$auth_object->require_access_permissions='N';
$auth_object->require_employee='N';
$auth_object->require_local_networks='N';
$auth_object->require_local_or_logged_in='Y';
$auth_object->require_local_or_logged_in_employee='N';

$auth_object->include_local_auth='N';
$auth_object->always_attach_db='Y';

$dbdescription="UMC Intranet";
require('global-auth.inc');
?>
