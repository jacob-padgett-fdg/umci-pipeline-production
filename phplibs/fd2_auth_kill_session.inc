<?
$tmpu=$auth_user_name;
$tmpp=$auth_user_pass;
@session_destroy();
fd2_session_start();
/*@session_name('sess-' . $auth_database_name);
@session_start();

session_register('auth_backend_user_logged_in');   
session_register('auth_backend_database_username');
session_register('auth_backend_database_password');
session_register('auth_backend_database_global_user_id');
session_register('global_jobinfo_id');
*/


$auth_backend_user_logged_in="";   
$auth_backend_database_username="";
$auth_backend_database_password="";
$auth_backend_database_global_user_id="";
$global_jobinfo_id="";
$auth_user_name=$tmpu;
$auth_user_pass=$tmpp;

?>
