<?
$bgcolor="#ffffff";
//$title="UMC Office Supplies Ordering Page";


$application_name="supplies";
$dbdescription="UMC Office Supplies Order Page";
$sitedescription="UMC Office Supplies";
$oddrowtint="#dddddd";

//$vendid="10825";
//$vendpw="office";

$auth_object->check_web_pw='N';
$auth_object->check_smb_pw='Y';
$auth_object->check_webfile_exceptions='N';
$auth_object->check_access_permission='Y';

$auth_object->require_login='Y';
$auth_object->require_access_permissions='N';
$auth_object->require_employee='Y';
$auth_object->require_local_networks='N';
$auth_object->require_local_or_logged_in='N';
$auth_object->require_local_or_logged_in_employee='Y';

$auth_object->include_local_auth='N';
$auth_object->always_attach_db='Y';
?>
