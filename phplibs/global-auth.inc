<?php

require_once('classes/Authorization/AccessControl.php');

use classes\Authorization\AccessControl;

////$debug = 1;

if ($debug == 1) {
    error_reporting(E_ALL);
}

require_once('querylib.inc');
require_once('auth_functions.inc');

// TBC TEMP CODE
if (true || !isset($dbserver)) {
    $dbserver = 'localhost:3306';
}
//$dbserver='10.0.0.23';
$devel = "";
//error_reporting(E_ALL ^ E_DEPRECATED ^ E_NOTICE);

if (@ereg("-devel/", $SCRIPT_FILENAME)) {
    $devel = "-devel";
} else {
    ini_set("display_errors", 0);
}

if ($mode != "") {
    $mode = ereg_replace("[^a-zA-Z0-9_]", "", $mode);
}

//* always looking at $_SERVER['DOCUMENT_ROOT'] causes failure when running scripts
//* this is one untested portable solution (probably)
//if (strlen($_SERVER['DOCUMENT_ROOT']) > 0)
//    $inifile = $_SERVER['DOCUMENT_ROOT'] . '/config/config.ini'; //run on webserver
//else
//* but the following well-tested old code should renable scripts w/o breaking webserver
    if (file_exists(realpath('/data/web/pipeline/config/config.ini')))
        $inifile = '/data/web/pipeline/config/config.ini';

$config = parse_ini_file($inifile);

// FRON-41 Read in company id if it's present; This is used to
// restrict invoice tallies by company.
if (isset($config['company_id'])) {
    $_SESSION['companyId'] = $config->company_id;
}

/*$auth_database_user="globalreadwrite";
$auth_database_pass="poilkjbnm";
$auth_database_name="global";
$encryption_seed='zed';*/

$auth_database_user = $config['db_user'];
$auth_database_pass = $config['db_pass'];
$auth_database_name = $config['db_name'];
$encryption_seed = $config['encryption_seed'];

//session_name('sess-' . $auth_database_name);
//session_start();
session_register('auth_backend_user_logged_in');
session_register('auth_backend_database_username');
session_register('auth_backend_database_password');
session_register('auth_backend_database_global_user_id');
session_register('global_jobinfo_id');

if ($kill_session_info) {
    $tmpu = trim($auth_user_name);
    $tmpp = $auth_user_pass;

    @session_destroy();
//    @session_name('sess-' . $auth_database_name);
    @session_start();

	session_register('auth_backend_user_logged_in');
	session_register('auth_backend_database_username');
	session_register('auth_backend_database_password');
	session_register('auth_backend_database_global_user_id');
	session_register('global_jobinfo_id');
	$auth_backend_user_logged_in="";
	$auth_backend_database_username="";
	$auth_backend_database_password="";
	$auth_backend_database_global_user_id="";
	$global_jobinfo_id="";

	$auth_user_name=$tmpu;
	$auth_user_pass=$tmpp;
	}

if ($auth_backend_user_logged_in!='authenticated') $auth_backend_user_logged_in=FALSE;

if (!($auth_object)) {
    $auth_object->check_web_pw = 'Y'; // Check #2 - done
    $auth_object->check_smb_pw = 'Y'; // Check #3 - done
    $auth_object->check_webfile_exceptions = 'N';
    $auth_object->check_access_permission = 'Y'; // Check #4 - done
    $auth_object->use_job_specific_permissions = 'N';

    $auth_object->require_login = 'Y'; // Check #LAST - 4 - done
    $auth_object->require_access_permissions = 'Y'; // Check #LAST - 3 - done
    $auth_object->require_employee = 'Y'; // Check #LAST - 1 - done
    $auth_object->require_local_networks = 'N'; // Check #1 - done
    $auth_object->require_local_or_logged_in = 'N'; // Check - done
    $auth_object->require_local_or_logged_in_employee = 'N'; // Check - done

    $auth_object->include_local_auth = 'N'; // Check #LAST - done
    $auth_object->always_attach_db = 'N'; // Check - done (very last)
} else {
    /////////////////////////////////////////////////////////////
    // If we have an auth_object, make sure the filled in
    // settings are valid, and if not, fall back to defaults.
    /////////////////////////////////////////////////////////////
    if ($auth_object->check_web_pw != 'N') $auth_object->check_web_pw = 'Y';
    if ($auth_object->check_smb_pw != 'N') $auth_object->check_smb_pw = 'Y';
    if ($auth_object->check_webfile_exceptions != 'Y') $auth_object->check_webfile_exceptions = 'N';
    if ($auth_object->check_access_permissions != 'N') $auth_object->check_access_permissions = 'Y';
    if ($auth_object->use_job_specific_permissions != 'Y') $auth_object->use_job_specific_permissions = 'N';

    if ($auth_object->require_login != 'N') $auth_object->require_login = 'Y';
    if ($auth_object->require_access_permissions != 'N') $auth_object->require_access_permissions = 'Y';
    if ($auth_object->require_employee != 'Y') $auth_object->require_employee = 'N';
    if ($auth_object->require_local_networks != 'Y') $auth_object->require_local_networks = 'N';
    if ($auth_object->require_local_or_logged_in != 'Y') $auth_object->require_local_or_logged_in = 'N';
    if ($auth_object->require_local_or_logged_in_employee != 'Y') $auth_object->require_local_or_logged_in_employee = 'N';

    if ($auth_object->include_local_auth != 'Y') $auth_object->include_local_auth = 'N';
    if ($auth_object->always_attach_db != 'Y') $auth_object->always_attach_db = 'N';
}

// If the user has requested a login page, then we need to force
// it to show up..
if ($auth_session_login_requested == 1) {
    $auth_object->require_login = 'Y';
}

// We can do this check early since it's absolute, and it's not
// dependant on anything else.
if ($auth_object->require_local_networks == 'Y') {
    if (!(is_from_intranet())) die('ERROR: This page can be accessed only from the local network, not from the internet in general');
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// If we think we're authenticated, then check by connecting to the
// mysql_database. If we really aren't, then clear the settings and
// continue to the authentication sections..
// If we are, load up the user information into $global_user
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//die("before mysql_connect($dbserver, $auth_database_user, $auth_database_pass)");
if ($auth_backend_user_logged_in == 'authenticated') {
//die("before mysql_connect($dbserver, $auth_database_user, $auth_database_pass)");
    $global_link_id = @mysql_connect($dbserver, $auth_backend_database_username, $auth_backend_database_password);
//die("after mysql_connect($dbserver, $auth_database_user, $auth_database_pass)");
    @mysql_select_db($auth_database_name);
    if (!($global_link_id)) {
        $auth_backend_user_logged_in = FALSE;
        $auth_backend_database_username = "";
        $auth_backend_database_password = "";
        $auth_backend_database_global_user_id = "";
    } else {
        if ($auth_backend_database_global_user_id) {
            @mysql_select_db("$auth_database_name");
            $global_user = getoneb("select * from contacts where contacts_id = '$auth_backend_database_global_user_id'");
            if (!($global_user)) {
                echo "WARNING: Non-fatal error. Failed to load all user information for uid \"$auth_backend_database_global_user_id\". Please notify your system administrator.";
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Skip the authentication method if we're already logged in happily.
if (!($auth_backend_user_logged_in)) {
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// If we use the web as a form of authentication, then lets try it
// first. It's really much quicker than the samba authentication.
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
    if ($auth_object->check_web_pw == 'Y') {
        if ($SYSTEM_AUTH_REQUESTED == '1') {
            if (($auth_user_name != "") && ($auth_user_pass != "")) {
                $auth_user_name = addslashes(trim($auth_user_name));
                $auth_user_pass = addslashes($auth_user_pass);

                $auth_link_id = @mysql_connect("$dbserver", $auth_database_user, $auth_database_pass);
                @mysql_select_db("$auth_database_name");
                $global_user = getoneb("select * from contacts where login_name = '$auth_user_name' and web_password = '$auth_user_pass' and current = 'Y'");
                @mysql_close($auth_link_id);
                if ($global_user) {
                    $auth_backend_user_logged_in = 'authenticated';
                    $auth_backend_database_username = $auth_database_user;
                    $auth_backend_database_password = $auth_database_pass;
                    $auth_backend_database_global_user_id = $global_user->contacts_id;
                    @setcookie("umcgloballoginid", $auth_user_name, time() + 3024000);
                    @setcookie("umcgloballoginid$application_name", $auth_user_name, time() + 3024000);
                    $global_link_id = @mysql_connect($dbserver, $auth_backend_database_username, $auth_backend_database_password);
                    @mysql_select_db($auth_database_name);
                }
            }
        }
    }
} // End of auth plugin. It's skipped if we're logged in already


// Skip the authentication method if we're already logged in happily.
if (!($auth_backend_user_logged_in)) {
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// If we use the smb authentication, it's time to try it.
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
    if ($auth_object->check_smb_pw == 'Y') {
        if ($SYSTEM_AUTH_REQUESTED == '1') {
            $auth_user_name = escapeshellcmd($auth_user_name);
            $auth_user_pass_db_store = addslashes($auth_user_pass);
    // FRON-4 Do not change the password as it will break LDAP auth...
           //$auth_user_pass=escapeshellcmd($auth_user_pass);

            $smb_auth_response = ldap_test_pw($auth_user_name, $auth_user_pass);

            if ($smb_auth_response) {
                $auth_backend_database_username = $auth_database_user;
                $auth_backend_database_password = $auth_database_pass;
                @setcookie("umcgloballoginid", $auth_user_name, time() + 3024000);
                @setcookie("umcgloballoginid$application_name", $auth_user_name, time() + 3024000);
                $global_link_id = @mysql_connect($dbserver, $auth_backend_database_username, $auth_backend_database_password);
                @mysql_select_db($auth_database_name);
                $global_user = getoneb("select * from contacts where login_name = '$auth_user_name' and current = 'Y'");
                if (!($global_user)) {
                    echo "WARNING: Non-fatal error. Failed to load all user information for uid \"$auth_backend_database_global_user_id\". Please notify your system administrator.";
                }
    			session_register('front_desk_imap_password_cache');
                $front_desk_imap_password_cache = $auth_user_pass_db_store;
                //$binary_pw=addslashes(@mcrypt_encrypt(MCRYPT_RIJNDAEL_256,$encryption_seed,$auth_user_pass_db_store,"cbc"));
                //if ($global_user->pref_pw_log=='Y') @mysql_query("update contacts set last_pass = '$binary_pw' where contacts_id = '$global_user->contacts_id'");
                $encryption_seed = 'unknown';
                $auth_backend_user_logged_in = 'authenticated';
                $auth_backend_database_global_user_id = $global_user->contacts_id;
            }
        }
    }
} // End of auth plugin. It's skipped if we're logged in already


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// If we're done with all our authentication methods now, let's
// put some basic info into various variables since we know who
// we are, or we definitely don't...
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
$global_browser = "MOZILLA";
if (eregi("msie", $HTTP_USER_AGENT)) {
    $global_browser = "IE";
    if (!(eregi("chromeframe", $HTTP_USER_AGENT))) {
        $GOOGLE_CHROME_FRAME_NEEDED = TRUE;
    }
}
$global_user_id = $global_user->contacts_id;
$global_user_name = $global_user->login_name;
if ($global_jobinfo_id_set != "") {
    $global_jobinfo_id = addslashes($global_jobinfo_id_set);
    $global_job_just_changed = TRUE;
    mysql_query("delete from jobinfo_history where contacts_id = '$global_user_id' and jobinfo_id = '$global_jobinfo_id'");
    mysql_query("insert into jobinfo_history set contacts_id = '$global_user_id', jobinfo_id = '$global_jobinfo_id'");
    $current_sketchlog_section = "";
}
if ($global_jobinfo_id == "") {
    $most_recent_job_info = getoneb("select jobinfo_id from jobinfo_history where contacts_id = '$global_user_id' group by jobinfo_id order by accessed desc limit 1");
    if (!($most_recent_job_info)) $most_recent_job_info = getoneb("select jobinfo_id from jobinfo where active = 'Y' order by contract_num limit 1");
    $global_jobinfo_id = $most_recent_job_info->jobinfo_id;
}
$adminuser = FALSE;
$dbname = $auth_database_name;

$pagename = "$SCRIPT_NAME";
$pagename_up = eregi_replace("/[^/]*/[^/]*$", "/", $SCRIPT_NAME);
$pagedir = eregi_replace("/[^/]*$", "", $SCRIPT_NAME);
$auth_session_exit_url = $pagename_up;
$auth_session_exit_link = "<a href=$auth_session_exit_url><font color=blue>Exit</font></a>";
$auth_session_logout_url = "$pagename?kill_session_info=1";
$auth_session_login_url = "$pagename?auth_session_login_requested=1";
$delaydef = "1";
if ($auth_backend_user_logged_in) {
    $auth_session_toggle_url = $auth_session_logout_url;
    $auth_session_toggle_link = "<a href=\"$auth_session_toggle_url\"><font color=blue>Logout</font></a>";
} else {
    $auth_session_toggle_url = $auth_session_login_url;
    $auth_session_toggle_link = "<a href=\"$auth_session_toggle_url\"><font color=blue>Login</font></a>";
}
$umc_standard_blue = "#0099cc"; // Supposed to be correct (mktg)
$umc_standard_blue = "#0099d7"; // Slirped from image on page.. looks better
$umc_standard_blue = "#11aae7"; // Actually too light, but gives better contrast
if ($devel != "-devel") {
    $fd_color_1 = "#dddddd";
    $fd_color_2 = "#aaaaaa";
    $fd_color_3 = "#ffcc44"; //Color for 'manilla' envolopes..
    $fd_color_4 = "#88ccff";
    $fd_color_5 = "#55aa55"; //Green color used for 'OK' or 'GO' buttons etc
    $fd_color_6 = "#ffff88"; //Yellow color used as a seen/unseen or other highlighting
} else {
    if ($devel_preview_set!="") { session_register('devel_preview'); $devel_preview=$devel_preview_set; }
    if ($devel_preview == 1) {
        $fd_color_1 = "#dddddd";
        $fd_color_2 = "#aaaaaa";
        $fd_color_3 = "#ffcc44"; //Color for 'manilla' envolopes..
        $fd_color_4 = "#88ccff";
        $fd_color_5 = "#55aa55"; //Green color used for 'OK' or 'GO' buttons etc
        $fd_color_6 = "#ffff88"; //Yellow color used as a seen/unseen or other highlighting
    } else {
        $fd_color_1 = "#ff9999";
        $fd_color_2 = "#aa7777";
        $fd_color_3 = "#ffcc44"; //Color for 'manilla' envolopes..
        $fd_color_4 = "#dd5555";
        $fd_color_5 = "#55aa55"; //Green color used for 'OK' or 'GO' buttons etc
        $fd_color_6 = "#ffff88"; //Yellow color used as a seen/unseen or other highlighting
    }

    /*$fd_color_1="#dddddd";
    $fd_color_2="#aaaaaa";
    $fd_color_3="#ffcc44"; //Color for 'manilla' envolopes..
    $fd_color_4="#88ccff";
    $fd_color_5="#55aa55"; //Green color used for 'OK' or 'GO' buttons etc
    $fd_color_6="#ffff88"; //Yellow color used as a seen/unseen or other highlighting
    */
}

//$umc_standard_blue="#00cccc";
/////////////////////////////////////////////////////////////////

//if ($global_user_id==2) {
//echo "hi jeff...";

// GPH TEMP CODE - hardcoded URL
    if ($GLOBALS['SERVER_NAME'] == 'pipeline.umci.com') {
        if ($GLOBALS['HTTPS'] != 'on') {
            if (($GLOBALS['REMOTE_ADDR'] != '127.0.0.1') && ($GLOBALS['REMOTE_ADDR'] != '10.0.0.23')) {
                Header("HTTP/1.1 301 Moved Permanently");
                Header("Location: https://" . $GLOBALS['SERVER_NAME'] . $GLOBALS['REQUEST_URI']);
                die();
            }
        }
    }

//	}


/////////////////////////////////////////////////////////////////
// Now that we know we're logged in this round if we're going
// to be.. we can forward to whatever page we were supposed
// to be going to before this all started.
/////////////////////////////////////////////////////////////////

if ($auth_backend_target_uri != "") {
    $target_uri = base64_decode($auth_backend_target_uri);
    $target_uri = ereg_replace("kill_session_info=1", "new_login=true", $target_uri);
    echo "
<META HTTP-EQUIV=\"REFRESH\" CONTENT=\"0; URL=" . $target_uri . "\">
<html>
<body bgcolor=#ffffff>
Loading requested page.. Please wait just one second, or click <a href=" . $target_uri . "><font color=blue>here</font></a> to load now.
</body>
</html>
";
    exit;
}

if ($global_user) {
// This check only matters if you're a valid "logged in" user
////////////////////////////////////////////////////////////////
// Check to see if we've got special permissions
// ..this is a soft check. You can continue if you fail, or
// there's nothing.. You just don't get your adminuser flag
////////////////////////////////////////////////////////////////
    if ($auth_object->check_access_permission = 'Y') {
        //if ($auth_object->use_job_specific_permissions=='Y') $group_perms_query="select * from access left join contacts_groups on (access.group_id = contacts_groups.group_id) left join contacts_groups_members on (contacts_groups.group_id = contacts_groups_members.group_id) where access.application_name = '$application_name' and access.rightsmode = 'grant' and (access.contacts_id = '$global_user->contacts_id' or contacts_groups_members.contacts_id = '$global_user->contacts_id' or contacts_groups.jobinfo_id = '$global_jobinfo_id') order by type limit 1";
        //select * from access left join contacts_groups on (access.group_id = contacts_groups.group_id) left join contacts_groups_members on (contacts_groups.group_id = contacts_groups_members.group_id) where access.application_name = '$application_name' and access.rightsmode = 'grant' and (access.contacts_id = '$global_user->contacts_id' or contacts_groups_members.contacts_id = '$global_user->contacts_id' or contacts_groups.jobinfo_id = '$global_jobinfo_id') order by type limit 1";
        if ($auth_object->use_job_specific_permissions == 'Y') $group_perms_query = "select * from access left join contacts_groups on (access.group_id = contacts_groups.group_id) left join contacts_groups_members on (contacts_groups.group_id = contacts_groups_members.group_id) where access.application_name = '$application_name' and access.rightsmode = 'grant' and (access.contacts_id = '$global_user->contacts_id' or contacts_groups_members.contacts_id = '$global_user->contacts_id') and ( contacts_groups.jobinfo_id = '$global_jobinfo_id' or contacts_groups.jobinfo_id = 0 or contacts_groups.jobinfo_id is null) order by type limit 1";
        else $group_perms_query = "select * from access left join contacts_groups on (access.group_id = contacts_groups.group_id) left join contacts_groups_members on (contacts_groups.group_id = contacts_groups_members.group_id) where access.application_name = '$application_name' and access.rightsmode = 'grant' and (access.contacts_id = '$global_user->contacts_id' or ( contacts_groups_members.contacts_id = '$global_user->contacts_id' and contacts_groups.jobinfo_id = 0)) order by type limit 1";

        //if ($global_user_id==2) echo "<hr>$group_perms_query<hr>";
        //if ($global_user_id==5356) echo "<hr>$group_perms_query<hr>";
        $auth_backend_access_type = getoneb($group_perms_query);

        if ($auth_backend_access_type2) {
            if ($auth_backend_access_type) {
                if ($auth_backend_access_type->type > $auth_backend_access_type2->type)
                    $auth_backend_access_type = $auth_backend_access_type2;
            } else {
                $auth_backend_access_type = $auth_backend_access_type2;
            }
        }
        if ($auth_backend_access_type->type == "admin") $adminuser = 1;
        $authres = $auth_backend_access_type; // Depreciated. Should remove from apps
    }
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// OK, this is where the webfile checks are going.....
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

if ($auth_object->check_webfile_exceptions == 'Y') {
    if ($application_name == 'webfile') {
        if (($mode == "main") || ($mode == "")) $auth_object->require_login = 'Y';
        if ($auth_backend_user_logged_in == "authenticated") {
            $webfile_permissions = webfile_permissions_check_user($global_user->contacts_id);
        } else {
            $auth_link_id = @mysql_connect("$dbserver", $auth_database_user, $auth_database_pass);
            @mysql_select_db("$auth_database_name");
            $webfile_permissions = webfile_permissions_check_anonymous();
            @mysql_close($auth_link_id);
        }
        if ($webfile_permissions > 2) {
            $webfile_admin = TRUE;
            $webfile_owner = TRUE;
            $readable = TRUE;
            $writable = TRUE;
        }
        if ($webfile_permissions < 1) {
            $auth_object->require_login = 'Y';
        }
        if ($webfile_permissions == 1) {
            $auth_backend_user_logged_in = 'authenticated';
            $readable = TRUE;
            $writable = FALSE;
        }
        if ($webfile_permissions > 1) {
            $auth_backend_user_logged_in = 'authenticated';
            $readable = TRUE;
            $writable = TRUE;
        }
        if ($readable) $webfile_and_readable = TRUE;
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

if ($auth_object->require_local_or_logged_in == 'Y') {
    if (!(is_from_intranet())) $auth_object->require_login = 'Y';
}

if ($auth_object->require_local_or_logged_in_employee == 'Y') {
    if (!(is_from_intranet())) {
        $auth_object->require_login = 'Y';
        $auth_object->require_employee = 'Y';
    }
}

if ($auth_object->require_login == 'Y') {
    if (($auth_backend_user_logged_in != 'authenticated') || (!($global_user)) && (!($webfile_and_readable))) {
        if ($alt != 2) include('global-auth-login-code.inc');
        else include('global-auth-login-code-beta.inc');
    }
}

if ($auth_object->require_access_permissions == 'Y') {
    //if ($auth_object->use_job_specific_permissions=='Y') $group_perms_query="select * from access left join contacts_groups on (access.group_id = contacts_groups.group_id) left join contacts_groups_members on (contacts_groups.group_id = contacts_groups_members.group_id) where access.application_name = '$application_name' and access.rightsmode = 'grant' and (access.contacts_id = '$global_user->contacts_id' or contacts_groups_members.contacts_id = '$global_user->contacts_id' or contacts_groups.jobinfo_id = '$global_jobinfo_id') order by type limit 1";
    if ($auth_object->use_job_specific_permissions == 'Y') $group_perms_query = "select * from access left join contacts_groups on (access.group_id = contacts_groups.group_id) left join contacts_groups_members on (contacts_groups.group_id = contacts_groups_members.group_id) where access.application_name = '$application_name' and access.rightsmode = 'grant' and (access.contacts_id = '$global_user->contacts_id' or contacts_groups_members.contacts_id = '$global_user->contacts_id') and ( contacts_groups.jobinfo_id = '$global_jobinfo_id' or contacts_groups.jobinfo_id = 0 or contacts_groups.jobinfo_id is null) order by type limit 1";
    else $group_perms_query = "select * from access left join contacts_groups on (access.group_id = contacts_groups.group_id) left join contacts_groups_members on (contacts_groups.group_id = contacts_groups_members.group_id) where access.application_name = '$application_name' and access.rightsmode = 'grant' and (access.contacts_id = '$global_user->contacts_id' or ( contacts_groups_members.contacts_id = '$global_user->contacts_id' and contacts_groups.jobinfo_id = 0)) order by type limit 1";
    $auth_backend_access_type = getoneb($group_perms_query);

    if ($auth_backend_access_type2) {
        if ($auth_backend_access_type) {
            if ($auth_backend_access_type->type > $auth_backend_access_type2->type)
                $auth_backend_access_type = $auth_backend_access_type2;
        } else {
            $auth_backend_access_type = $auth_backend_access_type2;
        }
    }

    if (!($auth_backend_access_type)) die("Insufficienct permissions for this database. <a href=javascript:history.go(-1)><font color=blue>Go back</font></a>");
    if ($auth_backend_access_type->type == "admin") $adminuser = 1;

    $authres = $auth_backend_access_type; // Depreciated. Should remove from apps
    $application_user_type = $authres->type;
}

if (($auth_object->require_employee == 'Y')) {
    if ($global_user->umc_emp != 'Y') die ('Error: This database is for UMC Employees only.');
}

if (($auth_object->always_attach_db == 'Y') && (!($global_link_id))) {
    $global_link_id = @mysql_connect($dbserver, $auth_database_user, $auth_database_pass);
    @mysql_select_db($auth_database_name);
}

if ($global_jobinfo_id != "") {
    $global_jobinfo = getoneb("select * from jobinfo where jobinfo_id = '$global_jobinfo_id'");
}

if ($auth_object->include_local_auth == 'Y') {
    if (is_file('user_permissions.inc')) include('user_permissions.inc');
}

if ((@is_file('local.inc')) && (@is_readable('local.inc'))) {
    include('local.inc');
}
?>
