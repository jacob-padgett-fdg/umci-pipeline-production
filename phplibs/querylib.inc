<?
require_once('classes/Authorization/AccessControl.php');
use classes\Authorization\AccessControl;

if (!isset($querylib) || ($querylib != 1)) {
    $querylib = 1;
    srand(microtime(true) * 10000000);
    $security_contact = "7774";
    $pepper = "asol";

#######################################################################
#	BS class to get rid of error messages (like the name says :)
#	...used to let us address hash arrays without scalar warnings.
#	**NOTE** This fix is actually obsolete, but some code out there might
#      still use it, so if removed, it might freak some things out.
#######################################################################
    class bsclass
    {
        var $foo;
    }

    ;

    function reg_usage($func)
    {
        global $mode, $application_name;

        return (@mysql_query("insert into access_web_functions_history set application_mode = '$mode',application_name = '$application_name', function_name = '$func'"));
    }

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Potentially unused functions check "access_web_functions_history"
// table to view a record of their usage. (Only functions with the
// reg_usage(__FUNCTION__) call in them are tracked. Ones without
// that call have already been determined to be in use somewhere.
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

    function check_group_permissions($type)
    {
        reg_usage(__FUNCTION__);
        global $global_user_id, $global_jobinfo_id;
    }


    function ldap_test_pw($user, $pass)
    {
        // GPH TEMP CODE - Bypass LDAP for testing purposes
//    return(TRUE);

        reg_usage(__FUNCTION__);
        if ($pass == "") return FALSE;
        $con1 = ldap_connect("10.0.0.134");       // GPH NOTE: Virtual machine
        $con2 = ldap_connect("10.0.0.31");        // GPH NOTE: Physical box
        $status = ldap_bind($con1, "UMC\\$user", "$pass");
        if ($status) return (TRUE);
        $status = ldap_bind($con2, "UMC\\$user", "$pass");
        if ($status) return (TRUE);
        //die('foo');
        return (FALSE);
    }

    function table_info($table)
    {
        reg_usage(__FUNCTION__);
        // Editable will mean the a primary key was found.
        // It has nothing to do with SQL permissions...
        $info->editable = FALSE;
        $info->primary_key = FALSE;
        $info->field_count = 0;

        $res = @mysql_query("describe $table");
        if (!($res)) return FALSE;
        while ($row = @mysql_fetch_object($res)) {
            if ($row->Key == "PRI") {
                $info->editable = TRUE;
                $info->primary_key = $row->Field;
                $info->field_count++;
            }
        }
        return ($info);
    }

    function generic_edit_tabledump($table, $field, $value)
    {
        reg_usage(__FUNCTION__);
        global $pagename;
        $res = @mysql_query("select * from $table where $field = '$value'");
        $count = @mysql_num_rows($res);
        $t_info = table_info($table);

        $query = "select * from $table where $field = '$value'";

        $results = mysql_query("$query");
        $width = mysql_num_fields($results);

        echo "<table border=1>";

        $column = 0;
        while ($column < $width) {
            if ($header_text != "") echo "<tr><td colspan=$width align=center><b>$header_text</b></td></tr>";
            $field = mysql_field_name($results, $column);
            if ($field != $t_info->primary_key) echo "<td bgcolor=#dddddd><b><font color=#000000>$field</font></b></td>";
            else $pk_column = $column;

            $column++;
        }

        $column = 0;
        while ($row = (@mysql_fetch_row($results))) {
            $column = 0;
            echo "<tr>";

            while ($column < $width) {
                if ($column != $pk_column) {
                    if ($row[$column] == "") $row[$column] = '&nbsp;';

                    if ($t_info->editable) {
                        echo "<td><a href=$pagename?mode=generic_edit_record&table=$table&primary_key=$t_info->primary_key&value=" . $row[$pk_column] . "><font color=blue>$row[$column]</font></a></td>";
                    }
                }
                $column++;
            }
            echo "</tr>";
        }
        echo "</table>";
    }

    function  month_list($start_month, $end_month)
    {
        reg_usage(__FUNCTION__);
        $start = vali_date($start_month);
        $end = vali_date($end_month);
        $start_u = (strtotime("$start") + 7200);
        $end_u = (strtotime("$end") + 7200);
        $start_som = date('Y-m-1', $start_u);
        $end_som = date('Y-m-1', $end_u);
        $end_som_u = (strtotime($end_som) + 7200);
        $end_som_plus = strtotime("+1 month", $end_som_u);
        $end_eom_u = (strtotime("-1 day", $end_som_plus));
        $end_eom = date('Y-m-d', $end_eom_u);
    }

    function contacts_phone_book($query = "", $typemask = 0, $target_win_name = "contacts_pb_window")
    {
        reg_usage(__FUNCTION__);
        /////////////////////////////////////////////////////////////////////////
        // Typemask is an integer between 0 & 255 in a form similar to a chmod
        // mask, with the bits meaning:
        // bit#  Value		  Result
        //  0	  (1)		= undefined
        //  1	  (2)		= undefined
        //  2 	  (4)		= undefined
        //  3 	  (8)		= undefined
        //  4 	  (16)		= undefined
        //  5 	  (32)		= undefined
        //  6 	  (64)		= undefined
        //  7 	  (128)		= undefined
        //
        // Simply add all of the values (in parenthesis) for the options
        // that you'd like, and pass that number as the parameter.
        //$undefined=bit_info($typemask,0);
        /////////////////////////////////////////////////////////////////////////

        global $_contacts_pb_javascript_popup, $_urlencode_javascript;
        if ($_contacts_pb_javascript_popup != "1") {
            $_contacts_pb_javascript_popup = "1";

            if ($_urlencode_javascript != "1") {
                $_urlencode_javascript = "1";
                echo "<script src=\"/javascript/urlencode.js\"></script>";
            }

            echo "
		<script>
		function jc() {
			var default_id;
			field=formname + '.' + fieldname;
			eval('default_id=document.' + field + '.value');
			field=urlencode(field);
			query=urlencode(query);
			onchange=urlencode(onchange);
			open(\"/global/contacts/contact_selection.phtml?query=\" + query + \"&field=\" + field + \"&contacts_id=\" + default_id + \"&onchange=\" + onchange + \"&settings=\" + settings,\"$target_win_name\",\"width=500,height=500,scrollbars=yes\");
			}
		</script>";
        }

        $query = eregi_replace("^.* where", "", $query);
        $query = eregi_replace("order by .* limit", " limit", $query);
        $query = eregi_replace("order by .* group", " group", $query);
        $query = eregi_replace("order by .*", " ", $query);
        $query = base64_encode($query);
        $onchange = base64_encode($onchange);

        if ($value != "") {
            $value = escapeshellcmd($value);
            $contact_info = getoneb("select * from contacts where contacts_id = '" . escapeshellcmd($value) . "'");
        }
        if ($contact_info->contact_type != "") $contact_type = "$contact_info->contact_type";
        else $contact_type = "unknown";

        if ($readonly == "off") {
            $jslink = "jc(this.form.name,\"$field\",\"$query\",\"$onchange\",\"$typemask\")";
            $linkstyle = "";
        } else {
            if (!($contact_info)) {
                echo "<i><b>No Contact Info</b> (read only)</i>";
                return FALSE;
            }
            $jslink = "jcro(this.form.name,\"$field\")";
            $linkstyle = ' style="background: #eeeeee" ';
        }

        if ($hide_image == "on") {
            $image_link = "";
        } else {
            if ($contact_info->photo_file_group_id > 0) {
                $file_info = getoneb("select * from webfile_files where file_group_id = '$contact_info->photo_file_group_id'");
                $imgurl = webfile_tiny_image_icon($file_info->files_id);
                $image_link = "<input title=\"List All\" type=\"button\" onclick=$jslink border=0 style=\"background-image: url('$imgurl'); width: 24px;background-repeat:no-repeat;background-position:center;\">";
            } else {
                $image_link = "<input title=\"List All\" type=\"button\" onclick=$jslink border=0 style=\"background-image: url('/images/${contact_type}_icon.png'); width: 24px;background-repeat:no-repeat;background-position:center;\">";
            }
        }

        echo "
	<input type=hidden name=\"$field\" value=\"$contact_info->contacts_id\">
	<table border=0 cellpadding=0 cellspacing=0><tr><td>
	<input title=\"List All\" type=text size=25 name=\"${field}_disp\" value=\"$contact_info->display_name\" $linkstyle onclick=$jslink readonly></td><td>$image_link</td></tr>";
        if ($show_phone_in_form == "on") {
            if ($contact_info->title != "") echo "<tr><td colspan=2><b>Title:&nbsp;</b>$contact_info->title</td></td>";
            if ($contact_info->phone != "") echo "<tr><td colspan=2><b>Phone:&nbsp;</b>$contact_info->phone&nbsp;$contact_info->phone_ext</td></td>";
            if ($contact_info->phone_fax != "") echo "<tr><td colspan=2><b>Fax:&nbsp;</b>$contact_info->phone_fax</td></td>";
            if ($contact_info->address_1 != "") echo "<tr><td colspan=2><b>Address:&nbsp;</b>";
            map_link_this_contact_here($contact_info->contacts_id);
            echo "</td></td>";
        }
        echo "</table>";
    }


    function radio_box($text, $name, $value = "N", $otherletter = "P")
    {
        reg_usage(__FUNCTION__);
        if ($value == "Y") $yes_check = " checked";
        else $yes_check = "";
        if ($value == "N") $no_check = " checked";
        else $no_check = "";
        if ($value == $otherletter) $other_check = " checked";
        else $other_check = "";
        echo "<table border=0 cellspacing=0 cellpadding=0><tr><td valign=center>$text</td><td valign=center>Y</td><td valign=center><input type=radio name=$name value='Y'$yes_check></td><td valign=center>N</td><td valign=center><input type=radio name=$name value='N'$no_check></td><td valign=center>$otherletter</td><td valign=center><input type=radio name=$name value='$otherletter'$other_check></td></tr></table>";
    }

    function excel_dump($query, $filename = "data_dl.xls")
    {
        reg_usage(__FUNCTION__);
        $result = mysql_query($query);
        $count = mysql_num_fields($result);

        for ($i = 0; $i < $count; $i++) {
            $header .= mysql_field_name($result, $i) . "\t";
        }

        while ($row = mysql_fetch_row($result)) {
            $line = '';
            foreach ($row as $value) {
                if (!isset($value) || $value == "") {
                    $value = "\t";
                } else {
                    $value = str_replace('"', '""', $value);
                    $value = '"' . $value . '"' . "\t";
                }
                $line .= $value;
            }
            $data .= trim($line) . "\n";
        }
        $data = str_replace("\r", "", $data);

        if ($data == "") $data = "\nno matching records found\n";

        header("Content-type: application/octet-stream");
        header("Content-Disposition: attachment; filename=$filename");
        header("Pragma: no-cache");
        header("Expires: 0");

        echo $header . "\n" . $data;
        return 0;
    }

    function ms_excel_dump($query, $filename = "data_dl.xls")
    {
        reg_usage(__FUNCTION__);
        $result = mssql_query($query);
        $count = mssql_num_fields($result);

        for ($i = 0; $i < $count; $i++) {
            $header .= mssql_field_name($result, $i) . "\t";
        }

        while ($row = mssql_fetch_row($result)) {
            $line = '';
            foreach ($row as $value) {
                if (!isset($value) || $value == "") {
                    $value = "\t";
                } else {
                    $value = str_replace('"', '""', $value);
                    $value = '"' . $value . '"' . "\t";
                }
                $line .= $value;
            }
            $data .= trim($line) . "\n";
        }
        $data = str_replace("\r", "", $data);

        if ($data == "") $data = "\nno matching records found\n";

        header("Content-type: application/octet-stream");
        header("Content-Disposition: attachment; filename=$filename");
        header("Pragma: no-cache");
        header("Expires: 0");

        echo $header . "\n" . $data;
        return 0;
    }

#######################################################################
#
#	A Function to refresh the permissions in mysql
#
#######################################################################

    function flushit()
    {
        reg_usage(__FUNCTION__);
        mysql_query("flush privileges");
    }

#######################################################################
#
#	Employee list dropbox..Should be useable from any database
#
#######################################################################
    function employeebox($default_id = 0, $uservar = "user", $size = "1", $onchange = "")
    {
        reg_usage(__FUNCTION__);
        contactbox("select contacts_id as '$uservar',first_name,last_name,company from global.contacts where umc_emp = 'Y' order by last_name,first_name,company", $default_id, $size, $onchange);
    }

#######################################################################
#
#	Employee information available from any page, connected to a
#	session or not, without any login information.
#
#######################################################################
    function queryuser($default_id = 0, $customquery = "")
    {
        reg_usage(__FUNCTION__);
        $templink = mysql_connect("localhost", "globalread", "qazxsw");
        mysql_select_db("global", $templink);
        if ($customquery == "") {
            $results = getoneb("select * from contacts where contacts_id = '$default_id'", $templink);
        } else {
            $results = getoneb($customquery, $templink);
        }
        mysql_close($templink);
        return ($results);
    }

/////////////////////////////
// SAM uses this stuff
/////////////////////////////

    function is_admin()
    {
        global $adminuser;
        return ($adminuser);
    }

    function break_float($float)
    {
        return float_break($float);
    }

    function float_clean($var, $places = 2)
    {
        return clean_float($var, $places);
    }

    function clean_float($var, $places = 2)
    {
        $var = ereg_replace('[^0-9.\-]', '', $var);
        $end = ereg_replace('^.*\.', '', $var);
        $end = substr($end, 0, $places);
        if (!(ereg('\.', $var))) $end = '0';
        $end = str_pad($end, $places, '0');
        $var = ereg_replace('\..*', '', $var);
        $var = escapeshellcmd($var);
        return ("$var.$end");
    }


#######################################################################
#
#	Functions for chaneging dates to/from Unix Epoch time in seconds
#
#######################################################################
    function epoch2date($epoch)
    {
        reg_usage(__FUNCTION__);
        return (date("m-d-Y", $epoch));
    }

    function date2epoch($date)
    {
        reg_usage(__FUNCTION__);
        $year = ereg_replace("^.*-", "", $date);
        $month = ereg_replace("-.*$", "", $date);
        $day = ereg_replace("-[0-9]*$", "", (ereg_replace("^[0-9]*-", "", $date)));
        return (mktime(0, 0, 0, $month, $day, $year));
    }

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// Begin development functions..
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

    function datebox_test2($value, $javapath, $onchange = "", $show_year = "0")
    {
        reg_usage(__FUNCTION__);
        $value = invali_date($value, '/');
        $input_id = rand();
        if ($cal_image == "") $cal_image = "/images/show-calendar2.gif";
        if ($yearly_image == "")
            $yearly_image = "/images/show-calendar-yearly.gif";
        echo "<script src='/javascript/testcal.js'></script>";
        $varname = ereg_replace("^.*\.", "", $javapath);

        echo "
	<input type=text id='$input_id' title='mm-dd-YYYY' name='$varname' value='$value' size=10 onchange=\"$onchange\" style=\"display: inline; vertical-align: top;\">
	<a href=javascript:void(0); onclick=\"BrowseDate('$input_id');\" title='Choose Date'>
	<img src='$cal_image' border=0 style=\"display: inline;\"></a>";

        if ($show_year != "0") {
            echo "
		<a href=\"javascript:BrowseDate('$input_id');\" title=\"Choose Date\">
		<img src='$yearly_image' border=0 style=\"display: inline;\"></a></td>";
        }
        echo "</tr></table>";
    }


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// Begin confirmed functions..
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

    /*
function calendar_register_event($jobinfo_id,$category,$start_date,$unique_id="",$options="") {
	$jobinfo_id=addslashes($jobinfo_id);
	$job_info=getoneb("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
	$category=addslashes($category);
	$category_info=getoneb("select * from front_desk_calendar_categories where category_name = '$category'");
	$start_date=invali_date($start_date);
	$start_time=addslashes($start_time);
	$unique_id=addslashes($unique_id);
	$existing_event=getoneb("select * from front_desk_calendar where unique_id = '$unique_id' and jobinfo_id = '$jobinfo_id'");

	if ($existing_event) {
		$query_start="update calendar";
		$query_end="where jobinfo_id = '$jobinfo_id' and unique_id = '$unique_id'";
		} else {
		$query_start="insert into calendar";
		$query_end="";
		}
	}

*/

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//	Access Controls libraries for FD2_access.
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

    function web_pass_set($contacts_id, $password)
    {
        global $pepper;
        $salt_starter = genrandstring(22);
        $salt = '$2a$07$' . $salt_starter . '$';
        $crypted = crypt($password . $pepper, $salt);
        $res = @mysql_query("update contacts set fd2_pass_salt = '$salt', fd2_pass_hash = '$crypted' where contacts_id = '$contacts_id'");
        if ($res) {
            security_log_email($contacts_id, 0, 0, "hashed password changed", "fd2-access");
            return (TRUE);
        } else {
            security_log_email($contacts_id, 0, 0, "hashed password set attempted but failed - application problem?", "fd2-access");
            return (FALSE);
        }
        return (FALSE);
    }

    function web_pass_set_try($contacts_id, $current_password, $password)
    {
        if (web_pass_validate($contacts_id, $current_password)) {
            web_pass_set($contacts_id, $password);
            return (TRUE);
        }
        security_log_email($contacts_id, 0, 0, "hashed password change attempted - failed - authentication failure", "fd2-access");
        return (FALSE);
    }

    function web_pass_validate($contacts_id, $password)
    {
        global $pepper;
        $contact_info = getoneb("select * from contacts where contacts_id = '$contacts_id'");
        $crypted = crypt($password . $pepper, $contact_info->fd2_pass_salt);
        if ($crypted == $contact_info->fd2_pass_hash) return (TRUE);
        return (FALSE);
    }

    function genrandstring($length)
    {
        static $seed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        $pepper = "";
        $i = 0;
        while ($i < $length) {
            $pepper .= $seed[mt_rand(0, 61)];
            $i++;
        }
        return ($pepper);
    }

    function web_pass_send_otp_link($email_address, $comment = "")
    {
        if ($email_address == "") return (FALSE);
        $query = "select * from contacts where email = '" . mysql_real_escape_string($email_address) . "'";
        $contact_info = getoneb("select * from contacts where email = '$email_address'");
        //////////////////////////////////////////////////////
        // Why did I fail to look up contact info????
        //////////////////////////////////////////////////////
        if (!($contact_info)) {
            $res = @mysql_query($query);
            $count = @mysql_num_rows($res);
            if ($count > 1) {
                security_log_email(0, 0, 0, "OTP recovery link attempted - failed - multiple contacts with same email - " . mysql_real_escape_string($email_address) . " has $count contacts", "fd2_access");
            }
            return (FALSE);
        }
        //////////////////////////////////////////////////////
        // We're not trying to reset/recover a UMC Employee's
        // password with this are we?
        //////////////////////////////////////////////////////
        if ($contact_info->umc_emp == 'Y') {
            security_log_email($contact_info->contacts_id, 0, 0, "OTP recovery link attempted on UMC Employee!!!! NOT ALLOWED!!!", "fd2_access");
            return (FALSE);
        }
        //////////////////////////////////////////////////////
        // Make sure login and email are identical, which is
        // required for third party users.
        //////////////////////////////////////////////////////
        if ($contact_info->email != $contact_info->login_name) {
            security_log_email($contact_info->contacts_id, 0, 0, "OTP recovery attempted on account with invalid login name (doesn't match email address)", "fd2_access");
            return (FALSE);
        }
        $yesterday = date('Y-m-d', strtotime('yesterday'));
        $res = @mysql_query("update fd2_access_otp set expires = '$yesterday' where contacts_id = '$contact_info->contacts_id' and expires > '$yesterday'");
        if (!($res)) {
            echo "query executed, but seems to have failed..<br>";
        }
        $otp = genrandstring(60);
        $expires = date('Y-m-d', strtotime("+5 days"));
        $insert_query = "insert into fd2_access_otp set
			contacts_id = '$contact_info->contacts_id',
			expires = '$expires',
			accesses_remaining = 5,
			pepper = '$otp',
			notes = '" . mysql_real_escape_string($comment) . "'";
        $res = @mysql_query($insert_query);

        if (!($res)) {
            //die("failed query:<hr>" . $insert_query);
            return (FALSE);
        }


        $link = "<a href=https://pipeline.umci.com/global/fd2-access/reset.php?otp=$otp&login=$contact_info->login_name>Password Reset</a>";

        $message = "
	<html>
	<head><title>UMC Password Reset</title></head>
	<body>
	Please use the link provided below, to set or reset your password
	on University Mechanical Contractor's web site.<br>
	<p>

	$link
	<p>
	If you do not know why you are recieving this email then please contact
	help@umci.com immediately to report this issue.
	</body>
	</html>
	";

        $headers = 'MIME-Version: 1.0' . "\r\n";
        $headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
        $headers .= 'From: UMC Login Reset <noreply@umci.com>' . "\r\n";
        mail($contact_info->email, "UMC Password Reset", $message, $headers);
        return (TRUE);
    }

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


    function is_umc_principal($user_id = "")
    {
        global $global_user_id;
        if ($user_id == "") $user_id = $global_user_id;
        switch ($user_id) {
            case 3:
            case 4:
            case 22:
            case 20:
                return (TRUE);
                break;;
            default:
                return (FALSE);
                break;;
        }
    }


    function default_permissions($application)
    {
        $application = addslashes($application);
        $perms = getoneb("select * from webfile_permissions where application_name = '$application'");
        if (!($perms)) {
            $perms = getoneb("select * from webfile_permissions where application_name = 'default'");
            $perms->default = 'Y';
        }
        return ($perms);
    }

    function apply_default_file_group_permissions($file_group_id, $application)
    {
        if ($file_group_id < 5) return 0;
        $query = "select * from webfile_files where file_group_id = '$file_group_id'";
        //tabledump($query);
        $res = @mysql_query($query);

        while ($row = @mysql_fetch_object($res)) {
            //echo "<tr><td colspan=15>$query<hr>";tabledump($query);echo "</td></tr>";
            apply_default_permissions($row->files_id, $application);
            //echo "<tr><td colspan=15>apply_default_permissions($row->files_id,$application);</td></tr>";
        }
    }

    function apply_default_permissions($files_id, $application)
    {
        $application = addslashes($application);
        $files_id = addslashes($files_id);
        $perms = default_permissions($application);
        if ($perms->default == 'Y') {
            $finfo = getoneb("select * from webfile_files where files_id = '$files_id'");
            $ginfo = getoneb("select * from webfile_groups where file_group_id = '$finfo->file_group_id'");
            $perms = default_permissions($ginfo->application);
        }
        /*echo "<tr><td colspan=15>update webfile_files set
	access_public = '$perms->access_public',
	access_intranet = '$perms->access_intranet',
	access_any_user = '$perms->access_any_user',
	access_employees = '$perms->access_employees',
	write_public = '$perms->write_public',
	write_intranet = '$perms->write_intranet',
	write_any_user = '$perms->write_any_user',
	write_employees = '$perms->write_employees'
	where files_id = '$files_id'</td></tr>";
	*/
        @mysql_query("update webfile_files set
	access_public = '$perms->access_public',
	access_intranet = '$perms->access_intranet',
	access_any_user = '$perms->access_any_user',
	access_employees = '$perms->access_employees',
	write_public = '$perms->write_public',
	write_intranet = '$perms->write_intranet',
	write_any_user = '$perms->write_any_user',
	write_employees = '$perms->write_employees'
	where files_id = '$files_id'");

        return TRUE;
    }

    function calendar_add_button($jobinfo_id, $category, $start_date, $unique_id = "", $options = "")
    {
        global $job_calendar_javascript, $devel;
        if ($job_calendar_javascript != TRUE) {
            $job_calendar_javascript = TRUE;
            echo "
		<script>
		function oce (jobinfo_id,category,start_date,unique_id,options) {
			open('/global/front_desk$devel/?mode=calendar_edit' + '&cal_jobinfo_id=' + jobinfo_id + '&category=' + category + '&start_date=' + start_date + '&unique_id=' + unique_id + options,'cal_edit_event$unique_id','width=500,height=400');

			}
		</script>
		";
        }
        if ($jobinfo_id != "") {
            $event_info = getoneb("select * from front_desk_calendar where jobinfo_id = '$jobinfo_id' and unique_id = '$unique_id' limit 1");
        } else {
            $event_info = getoneb("select * from front_desk_calendar where jobinfo_id = '$jobinfo_id' and unique_id = '$unique_id' limit 1");
        }
        if ($event_info) echo "<img onclick=\"oce('$jobinfo_id','$category','$start_date','$unique_id','$options');\" title=\"Edit this front desk calendar event\" style=\"cursor: hand; cursor: pointer;\" src=/images/edit_the_date.png>";
        else echo "<img onclick=\"oce('$jobinfo_id','$category','$start_date','$unique_id','$options');\" title=\"Add this to the front desk calendar\" style=\"cursor: hand; cursor: pointer;\" src=/images/mark_the_date.png>";


        return 0;
    }


    function date_info($date = "")
    {
        if ($date == "") $date = date("m-d-Y");
        $vali_date = vali_date($date);

        $day = new bsclass;
        $day->unixtime = strtotime($vali_date);
        $day->unixtime2 = $day->unixtime + 14400;
        $day->day_of_week = date("D", $day->unixtime2);
        $day->year = date("Y", $day->unixtime2);
        $day->day_of_week_long = date("l", $day->unixtime2);
        $day->date = $date;
        $day->date_slashes = ereg_replace("-", "/", $date);
        $day->day_of_week_num = date("w", $day->unixtime2);

        $day->week_end_unixtime = strtotime("saturday", $day->unixtime2);
        $day->week_end_unixtime2 = $day->week_end_unixtime + 14400;
        $day->week_end = date("m-d-Y", $day->week_end_unixtime2);
        $day->week_end_slashes = date("m/d/Y", $day->week_end_unixtime2);
        $day->week_end_mysql = date("Y-m-d", $day->week_end_unixtime2);
        $day->next_week_end_unixtime2 = $day->week_end_unixtime2 + 604800;
        $day->next_week_end = date("m-d-Y", $day->next_week_end_unixtime2);
        $day->next_week_end_mysql = date("Y-m-d", $day->next_week_end_unixtime2);
        $day->last_week_end_unixtime2 = $day->week_end_unixtime2 - 604800;
        $day->last_week_end = date("m-d-Y", $day->last_week_end_unixtime2);
        $day->last_week_end_mysql = date("Y-m-d", $day->last_week_end_unixtime2);

        $day->week_start_unixtime = strtotime("-6 days", $day->week_end_unixtime2);
        $day->week_start_unixtime2 = $day->week_start_unixtime + 14400;
        $day->week_start = date("m-d-Y", $day->week_start_unixtime2);
        $day->week_start_slashes = date("m/d/Y", $day->week_start_unixtime2);
        $day->week_start_mysql = date("Y-m-d", $day->week_start_unixtime2);
        $day->next_week_start_unixtime2 = $day->week_start_unixtime2 + 604800;
        $day->next_week_start = date("m-d-Y", $day->next_week_start_unixtime2);
        $day->next_week_start_mysql = date("Y-m-d", $day->next_week_start_unixtime2);
        $day->last_week_start_unixtime2 = $day->week_start_unixtime2 - 604800;
        $day->last_week_start = date("m-d-Y", $day->last_week_start_unixtime2);
        $day->last_week_start_mysql = date("Y-m-d", $day->last_week_start_unixtime2);

        $day->date_mysql = date("Y-m-d", $day->unixtime2);
        $day->date_short = date("m-d", $day->unixtime2);
        $day->date_short_slash = date("m/d", $day->unixtime2);
        $day->next_unixtime = strtotime("+1 days", $day->unixtime2);
        $day->next_unixtime2 = $day->next_unixtime + 14400;
        $day->next = date("m-d-Y", $day->next_unixtime2);
        $day->previous_unixtime = strtotime("-1 days", $day->unixtime2);
        $day->previous_unixtime2 = $day->previous_unixtime + 14400;
        $day->previous = date("m-d-Y", $day->previous_unixtime2);
        $day->prev = $day->previous;

        return $day;
    }

#######################################################################
#	Generic Variable DB Sterilization and reformatting.
#######################################################################

    function sync_rfi_logs()
    {
        global $doc_id;
        $my_info = getoneb("select * from documents where doc_id = '$doc_id'");
        if (!($my_info)) {
            echo "WARNING: Couldn't sync due to missing document id. Please set doc_id before running this function the sync_rfi_logs() function.";
            return FALSE;
        }
        $started_with = $my_info->doc_type;

        switch ($my_info->doc_type) {
            case "gcrfi":
                $gcrfi_info = getoneb("select * from gcrfilog where gcrfi_id = '$my_info->app_record_id'");
                $friend_query = "select * from documents_links right join documents on (documents.doc_id = documents_links.docb) where
							doca = '$my_info->doc_id' and
							documents.doc_type = 'rfi'";
                $friend_res = @mysql_query($friend_query);
                $friendly_count = @mysql_num_rows($friend_res);
                if ($friendly_count > 1) {
                    echo "<script>alert('WARNING: could not sync RFI and GCRFI. Please do it manually.');</script>";
                    return FALSE;
                }
                $friend_info = getoneb($friend_query);
                if (!($friend_info)) return TRUE;
                $introspection_query = "select * from documents_links right join documents on (documents.doc_id = documents_links.docb) where
							doca = '$friend_info->doc_id' and
							documents.doc_type = 'gcrfi'";
                $ego = getoneb($introspection_query);
                if (!($ego)) {
                    echo "<script>alert('WARNING: could not sync RFI and GCRFI. Please do it manually.');</script>";
                    return FALSE;
                }
                $rfi_info = getoneb("select * from rfilog where rfi_id = '$friend_info->app_record_id'");
                if (!($rfi_info)) {
                    echo "<script>alert('WARNING: could not sync RFI and GCRFI. Please do it manually.');</script>";
                    return FALSE;
                }
                if (($gcrfi_info->pm_complete_date != "") && ($gcrfi_info->pm_complete_date != "0000-00-00")) {
                    $update_query = "update rfilog set pm_complete_date = '$gcrfi_info->pm_complete_date', pm_complete_user = '$gcrfi_info->pm_complete_user' where rfi_id = '$rfi_info->rfi_id'";
                    @mysql_query($update_query);
                    //echo "<hr>$update_query<hr>";die();
                }
                break;;
            case "rfi":
                $rfi_info = getoneb("select * from rfilog where rfi_id = '$my_info->app_record_id'");
                $friend_query = "select * from documents_links right join documents on (documents.doc_id = documents_links.docb) where
							doca = '$my_info->doc_id' and
							documents.doc_type = 'gcrfi'";
                $friend_res = @mysql_query($friend_query);
                $friendly_count = @mysql_num_rows($friend_res);
                if ($friendly_count > 1) {
                    echo "<script>alert('WARNING: could not sync RFI and GCRFI. Please do it manually.');</script>";
                    return FALSE;
                }
                $friend_info = getoneb($friend_query);
                if (!($friend_info)) return TRUE;
                $introspection_query = "select * from documents_links right join documents on (documents.doc_id = documents_links.docb) where
							doca = '$friend_info->doc_id' and
							documents.doc_type = 'rfi'";
                $ego = getoneb($introspection_query);
                if (!($ego)) {
                    echo "<script>alert('WARNING: could not sync RFI and GCRFI. Please do it manually.');</script>";
                    return FALSE;
                }
                $gcrfi_info = getoneb("select * from gcrfilog where gcrfi_id = '$friend_info->app_record_id'");
                if (!($gcrfi_info)) {
                    echo "<script>alert('WARNING: could not sync RFI and GCRFI. Please do it manually.');</script>";
                    return FALSE;
                }
                if (($rfi_info->pm_complete_date != "") && ($rfi_info->pm_complete_date != "0000-00-00")) {
                    $update_query = "update gcrfilog set pm_complete_date = '$rfi_info->pm_complete_date', pm_complete_user = '$rfi_info->pm_complete_user' where gcrfi_id = '$gcrfi_info->gcrfi_id'";
                    @mysql_query($update_query);
                    //echo "<hr>$update_query<hr>";die();
                }
                break;;
            default:
                echo "Warning! Tried to 'rfi sync' non (gc)rfi!!!";
                return FALSE;
        }
    }

    function sterilize($variable, $data, $table)
    {
        $variable = escapeshellcmd($variable);
        $result = mysql_query("select * from $table where 1 = 2");

        $tableinfores = @mysql_query("describe $table");
        while ($rowinfo = @mysql_fetch_object($tableinfores)) {
            $loopexit = "clean";
            if ($rowinfo->Field == "$variable") break;
            $loopexit = "dirty";
        }

        if ($loopexit == "dirty")
            return false;

        $ftype = $rowinfo->Type;
        switch (true) {
            case ($ftype == 'date'):
                return (vali_date($data));
                break;
            case ($ftype == 'char(5)'):
                $data = strtoupper($data);
                if (($data == 'Y') || ($data == 'YES') || ($data == "CHECKED") || ($data == "ON")) return ("Y");
                else return "N";
                break;
            case (substr($ftype, 0, 4) == 'int('):
                if ($rowinfo->Extra == "auto_increment") die ("Potentially serious  application error! Attempt to sterilize a table's key index! Please contact your system administrator.<p>");
                $data = ereg_replace("\.*$", "", $data);
                $data = ereg_replace("[^0-9]", "", $data);
                return ($data);
                break;
            case ((substr($ftype, 0, 5) == 'char(') && ($ftype != 'char(5)')):
                return (escapeshellcmd($data));
                break;
            case (substr($ftype, 0, 8) == 'decimal('):
                return ereg_replace("[^0-9\.]", "", $data);
                break;
            default:
                echo "Error: Variable type to be sterilized not matched<p> Variable Type: \"$ftype\"<p> Variable Name: $variable<p> From Table: $table<p>";
                exit;
                break;
        }
    }

    function batch_this_document($the_doc_id = "")
    {
        global $doc_id, $global_user_id, $global_jobinfo_id;
        if ($the_doc_id == "") $the_doc_id = $doc_id;
        $the_doc_id = addslashes($the_doc_id);
        $doc = getoneb("select * from documents where doc_id = '$the_doc_id'");
        if (!($doc)) return ("&nbsp;");
        if (getoneb("select * from front_desk_doc_queue where doc_id = '$the_doc_id' and contacts_id = '$global_user_id' and jobinfo_id = '$global_jobinfo_id' limit 1")) {
            return ("<div style='background: black;' onclick='javascript:add_doc_to_batch(" . $the_doc_id. ",this)'>" .
		                "<img title='Bake a batch' src=/images/cookie_batch.png border=0>" .
		            "</div>");
        } else {
            return ("<div onclick='javascript:add_doc_to_batch(" . $the_doc_id .",this)'>" .
                        "<img title='Bake a batch' src=/images/cookie_batch.png border=0>" .
                    "</div>");
        }
    }


    function webfile_paperclip_view($file_group_id, $falsevalue = "&nbsp;", $outputX = false)
    {
        $link = webfile_possible_view_link($file_group_id);
        if ($link) {
            //return ("<a target='paperclip_" . $file_group_id . "' href='" . $link . "'>je</a>");
            if ($outputX)
                return ("<a target='paperclip_" . $file_group_id . "' href='" . $link . "'><span style='font-size:1px'>X</span><img border=0 src='/images/paperclip.gif'></a>");
            else
                return ("<a target='paperclip_" . $file_group_id . "' href='" . $link . "'><img border=0 src='/images/paperclip.gif'></a>");
        } else {
            return ($falsevalue);
        }
    }

    function webfile_paperclip_download($file_group_id, $falsevalue = "&nbsp;")
    {
        $link = webfile_possible_download_link($file_group_id);
        if ($link) {
            return ("<a href='".$link."'><img border=0 src='/images/paperclip.gif'></a>");
        } else {
            return ($falsevalue);
        }
    }

    function webfile_possible_view_link($file_group_id)
    {
        global $devel;
        // This function takes a file_group_id, and returns a download url
        // If there is more than one file, then the function send you to a
        // page that tries to let you select.
        if ($file_group_id == 0) return (FALSE);
        $file_info = getoneb("select * from webfile_files where file_group_id = '$file_group_id'");
        if ($file_info) {
            $file_info->file_name = ereg_replace("[ #!?&%]", "_", $file_info->file_name);
            return ("/global/webfile$devel/index.html/files/" . ereg_replace("'", "", $file_info->file_name) . "?mode=webfile_view&files_id=$file_info->files_id");
        } else {
            $count_files = @mysql_query("select * from webfile_files where file_group_id = '$file_group_id'");
            $count = @mysql_num_rows($count_files);
            if ($count > 1) {
                return ("/global/webfile$devel/index.html?mode=webfile_view_multi&file_group_id=$file_group_id");
            } else {
                return (FALSE);
            }
        }
    }

    function webfile_possible_flashview_link($file_group_id)
    {
        global $devel;
        // This function takes a file_group_id, and returns a download url
        // If there is more than one file, then the function send you to a
        // page that tries to let you select.
        if ($file_group_id == 0) return (FALSE);
        $file_info = getoneb("select * from webfile_files where file_group_id = '$file_group_id'");
        if ($file_info) {
            $file_info->file_name = ereg_replace("[ #!?&]", "_", $file_info->file_name);
            return ("/global/webfile$devel/webfile_flashview.phtml/files/$file_info->file_name?files_id=$file_info->files_id");
        } else {
            $count_files = @mysql_query("select * from webfile_files where file_group_id = '$file_group_id'");
            $count = @mysql_num_rows($count_files);
            if ($count > 1) {
                return ("/global/webfile$devel/index.html?mode=webfile_view_multi&file_group_id=$file_group_id");
            } else {
                return (FALSE);
            }
        }
    }

    function webfile_possible_download_link($file_group_id)
    {
        global $devel;
        // This function takes a file_group_id, and returns a download url
        // If there is more than one file, then the function send you to a
        // page that tries to let you select.
        if ($file_group_id == 0) return (FALSE);
        $file_info = getoneb("select * from webfile_files where file_group_id = '$file_group_id'");
        if ($file_info) {
            $file_info->file_name = ereg_replace("[ #!?&]", "_", $file_info->file_name);
            return ("/global/webfile$devel/index.html/files/
		$file_info->file_name?mode=webfile_download&files_id=$file_info->files_id");
        } else {
            $count_files = @mysql_query("select * from webfile_files where file_group_id = '$file_group_id'");
            $count = @mysql_num_rows($count_files);
            if ($count > 1) {
                return ("/global/webfile$devel/index.html?mode=webfile_download_multi&file_group_id=$file_group_id");
            } else {
                return (FALSE);
            }
        }
    }


    function get_return_to_link($return_to = "")
    {
        if ($return_to == "") return 0;
        $return_to = addslashes($return_to);
        $return_to_url = base64_decode($return_to);
        if (($return_to == "close_popup_win") || ($return_to_url == "close_popup_win")) {
            return ("javascript:self.close();");
        }
        if ($return_to_url != "") {
            return ("$return_to_url");
        } else {
            return "";
        }
    }

    function vertical_text($text, $size = 18, $bgcolor = "white", $textcolor = "black")
    {
        $text = urlencode($text);
        $size = urlencode($size);
        $bgcolor = urlencode($bgcolor);
        $textcolor = urlencode($textcolor);
        echo "<img border=0 src=\"/images/vert_text.phtml/text.png?text=$text&size=$size&bgcolor=$bgcolor&textcolor=$textcolor\">";
    }

    function contact_edit_in_popup($contacts_id, $link_text = "", $dontclose = 1)
    {
        $contacts_id = addslashes($contacts_id);
        if ($link_text == "") {
            $contact_info = getoneb("select display_name from contacts where contacts_id = '$contacts_id'");
            if (!($contact_info)) $link_text = "ERROR";
            else $link_text = $contact_info->display_name;
        }
        if ($link_text == "") $link_text = "???";
        if ($dontclose == 0) $return_to = "close_popup_win";
        else $return_to = $GLOBALS['REQUEST_URI'];
        $return_to = base64_encode($return_to);
        echo "<a href=index.php?mode=contacts_edit&contacts_id=$contacts_id&return_to=$return_to><font color=blue>$link_text</font></a>";
    }

    function contact_rulecheck_die($rule, $text)
    {
        die("ERROR: Your contact has failed a keyword restriction check! Keyword: $rule->restext ");
    }

    function check_contact_kw($one = "", $two = "", $three = "")
    {
        $rules = @mysql_query("select * from contacts_restricted");
        while ($rule = @mysql_fetch_object($rules)) {
            if (!(contact_rulecheck($rule, $one))) contact_rulecheck_die($rule, $one);
            if (!(contact_rulecheck($rule, $two))) contact_rulecheck_die($rule, $two);
            if (!(contact_rulecheck($rule, $three))) contact_rulecheck_die($rule, $three);
        }
    }

    function contact_rulecheck($rule, $text)
    {
        if ($text == "") return true;
        $ruletype = $rule->resmode;
        $error = true;
        switch ($ruletype) {
            case 'keyword':
                if (eregi("$rule->restext", $text)) $error = false;
                break;
            default:
                echo "<p>Warning: Failed to match contact rule type. Check not performed.<p>";
                break;
        }
        return $error;
    }

    function webfile_image_show($files_id)
    {
        $height = "";
        $width = "";
        return ("/global/webfile/index.html/images/image$files_id.jpg?mode=webfile_preview&files_id=$files_id&width=$width&height=$height");
    }

    function webfile_file_download($files_id)
    {
        $files_id = addslashes($files_id);
        $file_info = getone("select * from webfile_files where files_id = '$files_id'");
        $file_name = urlencode($file_info->file_name);
        return ("/global/webfile/index.html/files/$file_name?mode=webfile_download&files_id=$files_id");
    }

    function webfile_view_link($files_id)
    {
        // This function takes a file_group_id, and returns a download url
        // If there is more than one file, then the function returns false
        $file_info = getoneb("select * from webfile_files where files_id = '$files_id'");
        if ($file_info) {
            $file_name = urlencode(ereg_replace("#", "", $file_info->file_name));
            return ("/global/webfile/index.html/files/$file_name?mode=webfile_view&files_id=$file_info->files_id");
        } else return (FALSE);
    }

    function javascript_hotkeys($keylist)
    {
        $keys = array_keys($keylist);
        echo "
	<script>
	function check_key(e) {
		if(typeof e == 'undefined') { e = event; }
		var nav4 = window.Event ? true : false;
		if ((typeof e.ctrlKey != 'undefined') ? e.ctrlKey : e.modifiers & Event.CONTROL_MASK > 0) { return true; }
		";
        while ($key = array_shift($keys)) {
            $event_type = "which";
            $item = $keylist[$key];
            $keylen = strlen($key);
            if ($keylen > 1) {
                if ($key == 'esc') {
                    $keynum = "27";
                    $event_type = "keyCode";
                }
                // Arrow Keys l,r,u,d "ar"rows
                if ($key == 'lar') {
                    $keynum = "37";
                    $event_type = "keyCode";
                }
                if ($key == 'rar') {
                    $keynum = "39";
                    $event_type = "keyCode";
                }
                if ($key == 'uar') {
                    $keynum = "38";
                    $event_type = "keyCode";
                }
                if ($key == 'dar') {
                    $keynum = "40";
                    $event_type = "keyCode";
                }
                if ($key == 'lf') {
                    $keynum = "13";
                    $event_type = "keyCode";
                }
                if ($key == 'ret') {
                    $keynum = "13";
                    $event_type = "keyCode";
                }
            } else {
                $keynum = ord($key);
            }
            echo "
			if (e.$event_type == $keynum ) {
	        $item ;
			cancelEvent(e);
			}";
        }
        echo "
		}
	function cancelEvent(event) {
		if (event.preventDefault) {
			event.preventDefault();
			event.stopPropagation();
		} else {
			event.returnValue = false;
		}
	}

	function hotkeys_init() {
		if( document.captureEvents && Event.KEYPRESS ) {
			document.captureEvents(Event.KEYPRESS );
			}
		if (navigator.appName=='Microsoft Internet Explorer')
			document.onkeydown=check_key;
			else
			document.onkeypress=check_key;
		}
	hotkeys_init();
	</script>";
    }


    function show_team($jobinfo_id, $callback = "", $callback_text = "OK")
    {
        global $fd_color_4, $_urlencode_javascript, $fd_color_1;
        ajax_load();

        if ($_urlencode_javascript != "1") {
            $_urlencode_javascript = "1";
            echo "<script src=\"/javascript/urlencode.js\"></script>";
        }

        $jobinfo_id = addslashes($jobinfo_id);
        $jobinfo = getone("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
        $pm = getoneb("select * from contacts where contacts_id = '$jobinfo->project_manager_id'");
        $det = getoneb("select * from contacts where contacts_id = '$jobinfo->detailing_lead_id'");
        $c_admin = getoneb("select * from contacts where contacts_id = '$jobinfo->contract_admin'");
        $safety = getoneb("select * from contacts where contacts_id = '$jobinfo->safetyrep'");

        $res = @mysql_query("
	select * from contacts_groups
	right join contacts_groups_members on
	(contacts_groups.group_id = contacts_groups_members.group_id)
	right join contacts on
	(contacts_groups_members.contacts_id = contacts.contacts_id)
	where jobinfo_id = '$jobinfo_id' order by alphasort");
        echo "
	<div style=\"background-color: #ffffff; border: 2px solid;\" id=\"project_team_list\"><table border=0 cellspacing=0 cellpadding=3>
	<tr><td colspan=3 align=center bgcolor=$fd_color_4><b>Project Team</b></td></tr>";

        if ($pm) {
            $link = contact_view_link($pm->contacts_id);
            echo "<tr bgcolor=$fd_color_1><td>";
            if ($callback != "") echo "</td>&nbsp;<td>";
            echo "
			<a href=$link><font color=blue>$pm->display_name</font></a>
		</td><td>
			<font size=-1><i>PM</i></font>
		</td></tr>";
        }

        if ($det) {
            $link = contact_view_link($det->contacts_id);
            echo "<tr bgcolor=$fd_color_1><td>";
            if ($callback != "") echo "</td>&nbsp;<td>";
            echo "
			<a href=$link><font color=blue>$det->display_name</font></a>
		</td><td>
			<font size=-1><i>Detailing</i></font>
		</td></tr>";
        }

        if ($c_admin) {
            $link = contact_view_link($c_admin->contacts_id);
            echo "<tr bgcolor=$fd_color_1><td>";
            if ($callback != "") echo "</td>&nbsp;<td>";
            echo "
			<a href=$link><font color=blue>$c_admin->display_name</font></a>
		</td><td>
			<font size=-1><i>Contracts</i></font>
		</td></tr>";
        }

        if ($safety) {
            $link = contact_view_link($safety->contacts_id);
            echo "<tr bgcolor=$fd_color_1><td>";
            if ($callback != "") echo "</td>&nbsp;<td>";
            echo "
			<a href=$link><font color=blue>$safety->display_name</font></a>
		</td><td>
			<font size=-1><i>Safety</i></font>
		</td></tr>";
        }


        while ($row = @mysql_fetch_object($res)) {
            $link = contact_view_link($row->contacts_id);
            $team_members_jscript = $team_members_jscript . "\r\nteam_members[$row->contacts_id]='off';";
            echo "<tr><td>";
            if ($callback != "") echo "
				<input id=\"team_member_checkbox_$row->contacts_id\" type=checkbox onclick=team_toggle_check($row->contacts_id)>
			</td><td>";
            echo "
				<a href=$link><font color=blue>$row->display_name</font></a>
			</td><td>
				<font size=-1><i>$row->description</i></font>
			</td></tr>";
        }
        if ($callback != "") {
            $callback_row = "<tr><td colspan=3 bgcolor=#dddddd align=center><a href=javascript:run_team_callback('start');><font color=blue>$callback_text</font></a></td></tr>";
        } else {
            $callback_row = "";
        }
        echo "
	$callback_row
	</table></div><div style=\"border: 1px solid; display: none;\" id=\"project_team\"></div>
	<script>
	var team_members=new Array();
	var team_callback_started=0;
	$team_members_jscript

	function ajax_trigger() {
		run_team_callback();
		}
	function team_toggle_check(contact) {
		if (team_members[contact]=='off') {
			team_members[contact]='on';
			document.getElementById('team_member_checkbox_' + contact).checked=true;
			} else {
			team_members[contact]='off';
			document.getElementById('team_member_checkbox_' + contact).checked=false;
			}
		}
	function run_team_callback(mode) {
		if (mode=='start') {
			team_callback_started='1';
			document.getElementById('project_team').innerHTML='Working...';
			}
		if (team_callback_started!='1') return;
		document.getElementById('project_team_list').style.display='none';
		document.getElementById('project_team').style.display='block';
		for (it in team_members) {
			if (team_members[it]=='on') {
				if ($callback(it)) {
					team_toggle_check(it);
					return;
				} else {
					team_callback_started='0';
					document.getElementById('project_team').style.display='none';
					document.getElementById('project_team_list').style.display='block';
					return;
					}
				}
			}
		team_callback_started='0';
		document.getElementById('project_team').style.display='none';
		document.getElementById('project_team_list').style.display='block';
		}
	</script>
	";
    }

    function fd_job_selection()
    {
        global $pagename, $mode, $global_jobinfo_id, $global_user_id, $global_user, $devel;
        if ($mode != "") $mode = addslashes($mode);
        else $mode = "main";

        echo "<form style=\"display: inline;\" name=global_job_select method=post action=\"/global/front_desk$devel/\">
	<b>Select Job:</b>&nbsp;";

        if ($global_user->umc_emp != 'Y') {
            dropbox("select jobinfo.jobinfo_id as global_jobinfo_id_set,contract_num,' - ',display_name from jobinfo,contacts_groups,contacts_groups_members where
		jobinfo.jobinfo_id = contacts_groups.jobinfo_id and
		contacts_groups.group_id = contacts_groups_members.group_id and
		contacts_groups_members.contacts_id = '$global_user_id'
		order by contract_num", $global_jobinfo_id, 1, 'submit()', 1);
        } else {
            $qry = "select jobinfo.jobinfo_id as global_jobinfo_id_set,contract_num,' - ',display_name from jobinfo where
		active = 'Y'

		order by contract_num";
            dropbox($qry, $global_jobinfo_id, 1, 'submit()', 1);
        }
        echo "</form>";
    }

    function scan_count($jobinfo_id)
    {
        global $global_jobinfo;
        include("../webfile/storage_settings.cfg");
        if ($global_jobinfo->jobinfo_id != $jobinfo_id) {
            $jobinfo_id = addslashes($jobinfo_id);
            $jobinfo = getoneb("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
            $contract_num = $jobinfo->contract_num;
        } else {
            $contract_num = $global_jobinfo->contract_num;
        }
        if ($contract_num == "") {
            echo "Failed to find job information for jobinfo_id: $jobinfo_id<br>No scan count available.";
            return FALSE;
        }
        $webfile_group = $storage_queues['front_desk'];
        $sum = getoneb("select sum(1) as total from webfile_files where file_group_id = '$webfile_group' and ( file_name like '%-$contract_num-%' or  file_name like '%-${contract_num}_%')");
        if ($sum->total == "") return (0);
        return ($sum->total);
    }

    function global_job_selection()
    {
        fd_job_selection();
    }

    function pm_list_my_jobs_query($pm_id = "")
    {
        global $global_user_id;
        if ($pm_id != "") $pm_id = addslashes($pm_id);
        else $pm_id = $global_user_id;
        return ("select jobinfo.jobinfo_id as jobinfo_id, jobinfo.contract_num as contract_num,job_name,display_name,project_manager_id from jobinfo left join front_desk_job_pms on (jobinfo.jobinfo_id = front_desk_job_pms.jobinfo_id) where jobinfo.active = 'Y' and (jobinfo.project_manager_id = '$pm_id' or front_desk_job_pms.contacts_id = '$pm_id') group by contract_num order by contract_num");
    }

    function i_am_a_pm()
    {
        global $global_user_id;
        //return(getoneb("select * from jobinfo left join front_desk_job_pms on (jobinfo.jobinfo_id = front_desk_job_pms.jobinfo_id) where jobinfo.active = 'Y' and (jobinfo.project_manager_id = '$global_jobinfo_id' or front_desk_job_pms.contacts_id = '$global_jobinfo_id') limit 1"));
        //$query=pm_list_my_jobs_query();
        return (getoneb(pm_list_my_jobs_query() . " limit 1"));
    }

/////////////////////////////////////////////////////////////////
//	Takes an optional argument of which job to check for. if this
//	is not specified, then it's whatever job we're in..
/////////////////////////////////////////////////////////////////
    function pm_for_this_job($jobinfo_id = "")
    {
        global $global_user_id, $global_jobinfo;
        if ($jobinfo_id != "") {
            $jobinfo = getoneb("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
        } else {
            $jobinfo = $global_jobinfo;
        }
        //if (($global_user_id==11)||($global_user_id==352)||($global_user_id==133)||($global_user_id==22)||($global_user_id==3)||($global_user_id==4)||($global_user_id==20)) return (TRUE);
        if (AccessControl::isProjectManager($global_user_id)) {
            return (TRUE);
        }
        if ($jobinfo->project_manager_id == $global_user_id) return (TRUE);
        return (getoneb("select * from front_desk_job_pms where jobinfo_id = '$jobinfo->jobinfo_id' and contacts_id = '$global_user_id'"));
        //return (FALSE);
    }


    function is_member_of($group)
    {
        global $global_user_id;
        $group = addslashes($group);
        if (getoneb("select * from contacts_groups_members where contacts_id = '$global_user_id' and group_id = '$group'")) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    function member_of_global_group_named($groupname)
    {
        global $global_user_id;
        $groupname = addslashes($groupname);
        if (getoneb("select * from contacts_groups_members right join contacts_groups on (contacts_groups.group_id = contacts_groups_members.group_id ) where jobinfo_id = 0 and contacts_id = '$global_user_id' and description = '$groupname' limit 1")) {
            return TRUE;
        } else {
            return FALSE;
        }
    }


    function eom_scripts()
    {
        global $eom_scripts;
        if ($eom_scripts != 1) {
            $eom_scripts = 1;
            echo "
		<script>
			function sp_load(id) {
			open('http://pipeline.umci.com/intra/sp_popup.phtml?eom_id=' + id,'sp_popup_win','width=600,height=600,scrollbars=1');
			}
		</script>
		";
        }
        return TRUE;
    }

    function eom_blurb($eom_id = 0)
    {
        eom_scripts();
        if ($eom_id == 0) {
            $today_sql = date('Y-m-d');
            $query = "select * from contacts_eom where starting_date <= '$today_sql'
		order by starting_date desc, eom_id desc limit 1";
            $res = getoneb($query);
            if (!($res)) return 0;
            $eom_id = $res->eom_id;
        }
        $eom_id = addslashes($eom_id);
        $eom_info = getone("select * from contacts_eom where eom_id = '$eom_id'");
        $contact_info = getoneb("select * from contacts where contacts_id = '$eom_info->contacts_id'");
        if (!($contact_info)) {
            echo "<table border=1 cellspacing=0 cellpadding=10><tr><td align=center valign=middle>UMC's Star Performer: To be announced.</td></tr></table>";
            return 0;
        }
        if ($contact_info->photo_file_group_id > 0) {
            $file_info = getoneb("select * from webfile_files where file_group_id = '$contact_info->photo_file_group_id' limit 1");
            if ($file_info) {
                $imgurl = webfile_eom_image_icon($file_info->files_id);
                $imglink = "<img border=0 align=left src=\"$imgurl\">";
            }
        }
        if ($eom_info->additional_picture > 0) {
            $file_info = getoneb("select * from webfile_files where file_group_id = '$eom_info->additional_picture' limit 1");
            if ($file_info) {
                $imgurl2 = webfile_eom_image_icon($file_info->files_id);
                $imglink2 = "<img border=0 align=right src=\"$imgurl2\">";
            }
        }


        echo "

	<table border=0 style=\"border: 1px solid black; \" cellspacing=0 cellpadding=3 width=350>
	<tr><td bgcolor=white valign=middle align=center>
		<a href=javascript:sp_load($eom_id);>$imglink</a>
	</td><td bgcolor=white valign=middle align=center>
		<a style=\"text-decoration: none;\" href=javascript:sp_load($eom_id);><font size=-1 style=\"color: blue;\">$eom_info->text_blurb</font></a>
	</td><td>
		<a href=javascript:sp_load($eom_id);>$imglink2</a>
	</td></tr>
	</table>
	";
    }


    function eom_long($eom_id = 0)
    {
        if ($eom_id == 0) return 0;
        $eom_id = addslashes($eom_id);
        $eom_info = getone("select * from contacts_eom where eom_id = '$eom_id'");
        $eom_info->text_long = ereg_replace("\n", "<p>", $eom_info->text_long);
        $contact_info = getone("select * from contacts where contacts_id = '$eom_info->contacts_id'");
        if ($contact_info->photo_file_group_id > 0) {
            $file_info = getoneb("select * from webfile_files where file_group_id = '$contact_info->photo_file_group_id' limit 1");
            $imgurl = webfile_medium_image_icon($file_info->files_id);
        }
        echo "
	<table border=1 cellspacing=0 cellpadding=4 width=570><tr><td>
	<img border=0 align=left src=$imgurl><font color=black>$eom_info->text_long</font>
	</td></tr></table>";
    }

    function follow_return_to($return_to = "")
    {
        if ($return_to == "") return 0;
        $return_to = addslashes($return_to);
        $return_to_url = base64_decode($return_to);
        if (($return_to == "close_popup_win") || ($return_to_url == "close_popup_win")) {
            echo "<script>
				self.close();
			</script>
			";
            exit;
        }
        if ($return_to_url != "") {
            echo "<script>
		document.location=\"$return_to_url\";
		</script>";
            exit;
        } else {
            return 0;
        }
    }


    function feature_request($link_text = "Innovation Suggestion")
    {
        global $pagename, $report_name, $mode, $application_name, $feature_request_js_included;
        if ($application_name == "") return 0;

        if ($feature_request_js_included != 1) {
            $feature_request_js_included = 1;
            echo "<script>
		function todo_add(app_name) {
			open('/global/global_todo_list/?mode=todo_add&application_name=' + app_name,'global_app_todos','width=700,height=500');
			}
		</script>
		";
        }

        echo "<a href=javascript:todo_add('$application_name');><font color=blue>$link_text</font></a>";
    }

    function feature_request_old($link_text = "Feature Request")
    {
        global $application_name, $feature_request_js_included;
        if ($application_name == "") return 0;

        if ($feature_request_js_included != 1) {
            $feature_request_js_included = 1;
            echo "<script>
		function todo_add(app_name) {
			open('/global/global_todo_list/?mode=todo_add&application_name=' + app_name,'global_app_todos','width=700,height=500');
			}
		</script>
		";
        }

        echo "<a href=javascript:todo_add('$application_name');><font color=blue>$link_text</font></a>";
    }

    function die_security($extra_info = "")
    {
        global $global_user_id, $application_name, $mode;
        $contact = getoneb("select * from contacts where contacts_id = '$global_user_id'");
        $message = "Some stupid smeghead tripped a security warning on a web page. The application in question is: " . $application_name . ", and the smeghead in question is $contact->display_name ($contact->first_name $contact->last_name... user id: $global_user_id).<p>This message is just a flag to help spot potentially abusive users, or potentially broken web applications that redirect users to invalid areas.<p>Extra application specific info:<hr>" . $extra_info;

        queue_email("2", "WEB APP SECURITY WARNING", $message, 'Y');
        //queue_email("1","WEB APP SECURITY WARNING",$message,'Y');
        die("ERROR: Security Violation! The authorities have been notified!!!");
    }

    function check_cal_init($jobinfo_id)
    {
        $init = getoneb("select * from front_desk_calendar_categories where jobinfo_id = '$jobinfo_id' limit 1");
        if (!($init)) {
            @mysql_query("insert into front_desk_calendar_categories set category_name = 'BIM', jobinfo_id = '$jobinfo_id', active = 'Y'");
            @mysql_query("insert into front_desk_calendar_categories set category_name = 'Cost Item', jobinfo_id = '$jobinfo_id', active = 'Y'");
            @mysql_query("insert into front_desk_calendar_categories set category_name = 'GC Sched Item', jobinfo_id = '$jobinfo_id', active = 'Y'");
            @mysql_query("insert into front_desk_calendar_categories set category_name = 'O&M', jobinfo_id = '$jobinfo_id', active = 'Y'");
            @mysql_query("insert into front_desk_calendar_categories set category_name = 'Reminder', jobinfo_id = '$jobinfo_id', active = 'Y'");
            @mysql_query("insert into front_desk_calendar_categories set category_name = 'Rentals', jobinfo_id = '$jobinfo_id', active = 'Y'");
            @mysql_query("insert into front_desk_calendar_categories set category_name = 'Subcontractor', jobinfo_id = '$jobinfo_id', active = 'Y'");
            @mysql_query("insert into front_desk_calendar_categories set category_name = 'Submittals', jobinfo_id = '$jobinfo_id', active = 'Y'");
            @mysql_query("insert into front_desk_calendar_categories set category_name = 'Warranty', jobinfo_id = '$jobinfo_id', active = 'Y'");
        }
        return TRUE;
    }

    function bit_info($value, $bit)
    {
        return ((pow(2, $bit) & $value) ? 'on' : 'off');
    }

    function contactsbox($query, $value, $field, $onchange = "", $typemask = 0)
    {
        /////////////////////////////////////////////////////////////////////////
        // Typemask is an integer between 0 & 255 in a form similar to a chmod
        // mask, with the bits meaning:
        // bit#  Value		  Result
        //  0	  (1)		= readonly record (only view contact info) no changing selection)
        //  1	  (2)		= locked contact list.. can't search outside it
        //  2 	  (4)		= disable links to add contacts // needs to be fixed
        //  3 	  (8)		= hide image
        //  4 	  (16)		= short/no extra contact info in popup (must hit view)
        //  5 	  (32)		= add limitted contact info (phone #) inside form also
        //	6 	  (64)		= hide additional tabs // tabs to be implimented still
        //  7 	  (128)		= hidden input box (for wizard-style contacts)
        //
        // Simply add all of the values (in parenthesis) for the options
        // that you'd like, and pass that number as the parameter.
        $readonly = bit_info($typemask, 0);
        $locked = bit_info($typemask, 1);
        $allow_add = bit_info($typemask, 2);
        $hide_image = bit_info($typemask, 3);
        $show_no_details_in_popup = bit_info($typemask, 4);
        $show_phone_in_form = bit_info($typemask, 5);
        //$tiny_tiny_form=bit_info($typemask,5);
        $hidden_input = bit_info($typemask, 7);
        /////////////////////////////////////////////////////////////////////////
        $input_box_size = 25;
        $input_type = "text";
        if ($hidden_input == "on") $input_type = "hidden";

        //global $_cont5Aacts_javascript_popup,$_urlencode_javascript,$fd_color_4;
        global $_contacts_javascript_popup, $_urlencode_javascript, $fd_color_4, $devel;
        if ($_contacts_javascript_popup != "1") {
            $_contacts_javascript_popup = "1";

            if ($_urlencode_javascript != "1") {
                $_urlencode_javascript = "1";
                echo "<script src=\"/javascript/urlencode.js\"></script>";
            }
            echo "<script src=\"/javascript/contacts_1${devel}.js\"></script>";
        }

        $query = eregi_replace("^.* where", "", $query);
        $query = eregi_replace("order by .* limit", " limit", $query);
        $query = eregi_replace("order by .* group", " group", $query);
        $query = eregi_replace("order by .*", " ", $query);
        $query = base64_encode($query);
        $onchange = base64_encode($onchange);

        if ($value != "") {
            $value = escapeshellcmd($value);
            $contact_info = getoneb("select * from contacts where contacts_id = '" . escapeshellcmd($value) . "'");
        }

        if ($contact_info->umc_emp == 'Y') {
            $background = "background: $fd_color_4;";
            if ($contact_info->current == 'N') $background = "border: 2px solid black; background: #000000;";
        }

        if ($contact_info->contact_type != "") $contact_type = "$contact_info->contact_type";
        else $contact_type = "unknown";

        if ($readonly == "off") {
            $jslink = "jc(this.form.name,\"$field\",\"$query\",\"$onchange\",\"$typemask\")";
            $linkstyle = "";
        } else {
            if (!($contact_info)) {
                echo "<i><b>No Contact Info</b> (read only)</i>";
                return FALSE;
            }
            $jslink = "jcro(this.form.name,\"$field\")";
            $linkstyle = ' style="background: #dddddd" ';
        }


        $image_title = "List All";
        if ($hidden_input) {
            $image_title = $contact_info->display_name;
        }

        if ($hide_image == "on") {
            $image_link = "";
        } else {
            if ($contact_info->photo_file_group_id > 0) {
                $file_info = getoneb("select * from webfile_files where file_group_id = '$contact_info->photo_file_group_id' limit 1");
                $imgurl = webfile_tiny_image_icon($file_info->files_id);
                $image_link = "<input title=\"$image_title\" type=\"button\" onclick=$jslink border=0 style=\"$background background-image: url('$imgurl'); width: 24px;background-repeat:no-repeat;background-position:center;\">";
            } else {
                $image_link = "<input title=\"$image_title\" type=\"button\" onclick=$jslink border=0 style=\"$background background-image: url('/images/${contact_type}_icon.png'); width: 24px;background-repeat:no-repeat;background-position:center;\">";
            }
        }

        $disp_len = strlen($contact_info->display_name);
        if (($disp_len > $input_box_size) && ($contact_info->contact_type == 'site')) {
            $contact_info->display_name[25] = '.';
            $contact_info->display_name[26] = '.';
            $contact_info->display_name[27] = '.';
            $contact_info->display_name[28] = '.';
            $contact_info->display_name[29] = '.';
            $contact_info->display_name[30] = '.';
            $html_comp = ereg_replace("'", "&lsquo;", $contact_info->company);
            $html_disp = ereg_replace("'", "&lsquo;", $contact_info->display_name);
            $extra_input_field = $extra_input_field . " onMouseOver=\"value='$html_comp';\" onMouseOut=\"value='$html_disp';\" ";
        }


        echo "
	<input id=\"hidden_contactsbox_$field\" type=hidden name=\"$field\" value=\"$contact_info->contacts_id\">
	<table border=0 cellpadding=0 cellspacing=0 style=\"display: inline;\"><tr><td>
	<input title=\"List All\" type=\"$input_type\" size=$input_box_size name=\"${field}_disp\" value=\"$contact_info->display_name\" $linkstyle onclick=$jslink readonly $extra_input_field></td><td align=left>$image_link</td></tr>";
        if ($show_phone_in_form == "on") {
            if ($contact_info->title != "") echo "<tr><td colspan=2><b>Title:&nbsp;</b>$contact_info->title</td></td>";
            if ($contact_info->phone != "") echo "<tr><td colspan=2><b>Phone:&nbsp;</b>$contact_info->phone&nbsp;$contact_info->phone_ext</td></td>";
            if ($contact_info->phone_fax != "") echo "<tr><td colspan=2><b>Fax:&nbsp;</b>$contact_info->phone_fax</td></td>";
            if ($contact_info->address_1 != "") echo "<tr><td colspan=2><b>Address:&nbsp;</b>";
            map_link_this_contact_here($contact_info->contacts_id);
            echo "</td></td>";
        }
        echo "</table>";
    }

    function contact_popup_javascript()
    {
        global $devel, $_urlencode_javascript, $_contacts_javascript_popup2;
        if ($_contacts_javascript_popup2 == "1") return 0;
        echo "
		<script type=\"text/javascript\">
		function watch_contact(id) {
			obj1=document.getElementById(id);
			obj2=document.getElementById(id + 'orig_value');
			if (obj1.value!=obj2.value) {
				obj2.onchange();
				} else {
				setTimeout(\"watch_contact('\" + id + \"')\",100);
				}
			}

		function jcnew(id,query,onchange,options) {

			default_contact=document.getElementById(id).value;
			query=urlencode(query);
			if (onchange!=\"\") watch_contact(id);
			onchange=urlencode(onchange);
			//alert('/global$devel/contacts$devel/contact_selector.phtml?query=' + query + '&id=' + id + '&contacts_id=' + default_contact + '&onchange=' + onchange + '&options=' + options + '&teamfirst=Y');
			open('/global$devel/contacts$devel/contact_selector.phtml?query=' + query + '&id=' + id + '&contacts_id=' + default_contact + '&onchange=' + onchange + '&options=' + options + '&teamfirst=Y','contactsbox_window','width=550,height=550,scrollbars=yes');
			}
		function jcro_new(id) {
			var default_contact;
			default_contact=document.getElementById(id).value;
			open('/global$devel/contacts$devel/contactview.phtml?read_only_warning=yes&contacts_id=' + default_id,'contactsbox_window_info_win' + default_id,'width=550,height=550,scrollbars=yes,resizable=yes');
			}
		</script>
		";


        if ($_urlencode_javascript != "1") {
            $_urlencode_javascript = "1";
            echo "<script src=\"/javascript/urlencode.js\"></script>";
        }
    }


    function contact($query, $value, $name, $onchange = "", $options = array())
    {
        global $devel;
        /////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////
        // GPH MARK - absolute path
        include("/data/web/pipeline/global$devel/contacts$devel/settings_option_defaults.phtml");
        $input_type = 'text';
        if ($options['hidden_input'] == "Y") $input_type = "hidden";

        $rand = rand('10000', '30000');
        $id = 'contact_input_' . $rand;

        /////////////////////////////////////////////////////////////
        // Load javascript if needed...
        /////////////////////////////////////////////////////////////

        contact_popup_javascript();

        $query = eregi_replace("^.* where", "", $query);
        $query = eregi_replace("order by .* limit", " limit", $query);
        $query = eregi_replace("order by .* group", " group", $query);
        $query = eregi_replace("order by .*", " ", $query);
        //echo "<hr>$query<hr>";
        $query = base64_encode($query);
        $onchange_orig = $onchange;
        $onchange = base64_encode($onchange);

        if ($value != "") {
            $value = escapeshellcmd($value);
            $contact_info = getoneb("select * from contacts where contacts_id = '" . escapeshellcmd($value) . "'");
        }
        if ($contact_info->contact_type != "") $contact_type = "$contact_info->contact_type";
        else $contact_type = "unknown";

        if ($options['readonly'] != "Y") {
            $jslink = "jcnew(\"$id\",\"$query\",\"$onchange\",\"$options_encoded\")";
            //$decoded=base64_decode($query);
            //echo "jcnew(\"$id\",/$decoded/\"$query\",\"$onchange\",\"$options_encoded\")";
            $linkstyle = "";
        } else {
            if (!($contact_info)) {
                echo "<i><b>No Contact Info</b> (read only)</i>";
                return FALSE;
            }
            $jslink = "jcronew(\"$id\")";
            $linkstyle = ' style="background: #dddddd" ';
        }

        if ($options['hidden_input'] == 'Y') $image_title = $contact_info->display_name;


        if ($options['hidden_image'] == "Y") {
            $image_link = "";
        } else {
            if ($contact_info->photo_file_group_id > 0) {
                $file_info = getoneb("select * from webfile_files where file_group_id = '$contact_info->photo_file_group_id' limit 1");
                $imgurl = webfile_tiny_image_icon($file_info->files_id);
                $image_link = "<input id=\"image_link_$id\" title=\"$image_title\" type=\"button\" onclick=$jslink border=0 style=\"background-image: url('$imgurl'); width: 24px;background-repeat:no-repeat;background-position:center;\">";
            } else {
                $image_link = "<input id=\"image_link_$id\" title=\"$image_title\" type=\"button\" onclick=$jslink border=0 style=\"background-image: url('/images/${contact_type}_icon.png'); width: 24px;background-repeat:no-repeat;background-position:center;\">";
            }
        }

        //echo "<hr>$jslink<hr>";
        $disp_len = strlen($contact_info->display_name);
        if (($disp_len > $options['input_size']) && ($contact_info->contact_type == 'site')) {
            $contact_info->display_name[$options['input_size']] = '.';
            $contact_info->display_name[$options['input_size'] + 1] = '.';
            $contact_info->display_name[$options['input_size'] + 2] = '.';
            $contact_info->display_name[$options['_input_size'] + 3] = '.';
            $contact_info->display_name[$options['input_size'] + 4] = '.';
            $contact_info->display_name[$option['input_size'] + 5] = '.';
            $html_comp = ereg_replace("'", "&lsquo;", $contact_info->company);
            $html_disp = ereg_replace("'", "&lsquo;", $contact_info->display_name);
            $extra_input_field = $extra_input_field . " onMouseOver=\"value='$html_comp';\" onMouseOut=\"value='$html_disp';\" ";
        }


        echo "
	<input id=\"$id\" type=hidden name=\"$name\" value=\"$contact_info->contacts_id\">
	<input id=\"${id}orig_value\" onchange=\"$onchange_orig\" type=hidden name=\"${name}orig_value\" value=\"$contact_info->contacts_id\">

	<table border=0 cellpadding=0 cellspacing=0 style=\"display: inline;\"><tr><td>
	<input title=\"List All\" type=\"$input_type\" size=" . $options['input_size'] . " id=\"disp_${id}\" value=\"$contact_info->display_name\" $linkstyle onclick=$jslink readonly $extra_input_field></td><td align=left>$image_link</td></tr>";
        if ($options['extra_info'] == "Y") {
            if ($contact_info->title != "") echo "<tr><td colspan=2><b>Title:&nbsp;</b>$contact_info->title</td></td>";
            if ($contact_info->phone != "") echo "<tr><td colspan=2><b>Phone:&nbsp;</b>$contact_info->phone&nbsp;$contact_info->phone_ext</td></td>";
            if ($contact_info->phone_fax != "") echo "<tr><td colspan=2><b>Fax:&nbsp;</b>$contact_info->phone_fax</td></td>";
            if ($contact_info->address_1 != "") echo "<tr><td colspan=2><b>Address:&nbsp;</b>";
            map_link_this_contact_here($contact_info->contacts_id);
            echo "</td></td>";
        }
        echo "</table>";
    }


    function webfile_group_vert_preview($file_group_id)
    {
        if (!($file_group_id)) return 0;
        $query = "select * from webfile_files where file_group_id = '$file_group_id' order by file_name,upload_date";
        $res = @mysql_query($query);
        if (!($res)) {
            echo "<b>Warning:</b> File group preview error!";
            return FALSE;
        }
        while ($row = @mysql_fetch_object($res)) {
            $width = 120;
            $height = "";
            if ($row->description != "") $description = $row->description;
            else $description = $row->file_name;
            $jscript_command = "javascript:open('/global/webfile/?mode=webfile_file_tools&files_id=$row->files_id','traxx_file_detail_win','width=500,height=500,scrollbars=yes')";
            $img = "/global/webfile/index.html/images/image$row->files_id.jpg?mode=webfile_preview&files_id=$row->files_id&width=$width&height=$height";
            echo "<p><table border=1 cellspacing=0 cellpadding=1><tr><td>$description<br><img onclick=\"$jscript_command\" border=0 src=$img></td></tr></table>";
        }

    }

    function webfile_group_horiz_preview($file_group_id)
    {
        if (!($file_group_id)) return 0;
        $query = "select * from webfile_files where file_group_id = '$file_group_id' order by file_name,upload_date";
        $res = @mysql_query($query);
        if (!($res)) {
            echo "<b>Warning:</b> File group preview error!";
            return FALSE;
        }
        while ($row = @mysql_fetch_object($res)) {
            $width = 120;
            $height = "";
            $jscript_command = "javascript:open('/global/webfile/?mode=webfile_file_tools&files_id=$row->files_id','traxx_file_detail_win','width=500,height=500,scrollbars=yes')";
            $img = "/global/webfile/index.html/images/image$row->files_id.jpg?mode=webfile_preview&files_id=$row->files_id&width=$width&height=$height";
            echo "<img onclick=\"$jscript_command\" border=0 src=$img><font size=-2>&nbsp;</font>
		";
        }

    }

    function webfile_image_icon($files_id, $options = "")
    {
        $height = "150";
        $width = "";
        if ($options['width'] != "") {
            $height = "";
            $width = $options['width'];
        }
        if ($options['height'] != "") $height = $options['height'];
        return ("/global/webfile/index.html/images/image$files_id.jpg?mode=webfile_preview&files_id=$files_id&width=$width&height=$height");
    }

    function webfile_medium_image_icon($files_id)
    {
        return (webfile_image_icon($files_id, array('height' => '150')));
    }

    function webfile_small_image_icon($files_id)
    {
        return (webfile_image_icon($files_id, array('height' => '45')));
    }

    function webfile_eom_image_icon($files_id)
    {
        return (webfile_image_icon($files_id, array('height' => '75')));
    }

    function webfile_tiny_image_icon($files_id)
    {
        return (webfile_image_icon($files_id, array('height' => '18')));
    }

    function load_file_group_info($file_group_id)
    {
        $file_group_id = addslashes($file_group_id);
        $file_group_info = getoneb("select * from webfile_groups where file_group_id = '$file_group_id'");
        if (!($file_group_info)) return FALSE;
        $count = getoneb("select sum(1) as total,file_name,files_id from webfile_files where file_group_id = '$file_group_info->file_group_id' group by file_group_id");
        if (!($count)) $file_group_info->file_count = 0;
        else $file_group_info->file_count = $count->total;

        if ($file_group_info->file_count < 1) {
            $file_group_info->file_group_text = "";
            $file_group_info->tiny_icon_image = "";
        }
        if ($file_group_info->file_count == 1) {
            //$file_group_info->file_group_text=$count->file_name . " ($file_group_info->file_count)";
            $file_group_info->file_group_text = ereg_replace('[^a-zA-Z0-9]', '', htmlspecialchars($count->file_name)) . " ($file_group_info->file_count)";
            $file_group_info->tiny_icon_image = webfile_tiny_image_icon($count->files_id);
            $file_group_info->small_icon_image = webfile_small_image_icon($count->files_id);
        }
        if ($file_group_info->file_count > 1) {
            $file_group_info->file_group_text = "($file_group_info->file_count) files...";
            $file_group_info->tiny_icon_image = "/images/webfile_icon_full.png";
        }

        if ($file_group_info->description == "") $file_group_info->description = $file_group_info->file_group_text;
        return ($file_group_info);
    }

    function webfilebox($value, $field, $onchange = "", $typemask = 0, $target_win_name = "webfile_popup")
    {
        /////////////////////////////////////////////////////////////////////////
        // Typemask is an integer between 0 & 255 in a form similar to a chmod
        // mask, with the bits meaning:
        // bit#  Value		  Result
        //  0	  (1)		= Limit box to only 1 file (+revisions)
        //  1	  (2)		= readonly record (only view contact info) no changing selection)
        //  2	  (4)		= invisible (make it completely hidden in the form
        //  3 	  (8)		= small box (hide image, shrink textbox)
        //  4 	  (16)		= no box (only show image)
        //  5 	  (32)		= undefined
        //  6 	  (64)		= undefined
        //  // maybe obsolete..7 	  (128)		= fancy icon (tiny preview/thumbnail)
        //
        // Simply add all of the values (in parenthesis) for the options
        // that you'd like, and pass that number as the parameter.
        $onefilelimit = bit_info($typemask, 0);
        $readonly = bit_info($typemask, 1);
        $invisible = bit_info($typemask, 2);
        $small_box = bit_info($typemask, 3);
        $no_box = bit_info($typemask, 4);
        //$fancy_icon=bit_info($typemask,7); // This is always on for now
        /////////////////////////////////////////////////////////////////////////
        if (($smal_box == 'on') && ($no_box == 'on')) die('Webfile error: Incompatible options used!');

        global $_webfile_javascript_popup, $_urlencode_javascript, $application_name, $devel;

        load_webfile_javascript();
        $onchange = base64_encode($onchange);
        if ($value != "") {
            $value = escapeshellcmd($value);
            $file_info = load_file_group_info($value);
        }

        if ($file_info->file_count < 1) {
            $file_icon = "/images/webfile_icon.png";
        }
        if ($file_info->file_count == 1) {
            $file = getone("select * from webfile_files where file_group_id = '$file_info->file_group_id'");
            $file_icon = webfile_tiny_image_icon($file->files_id);
        }
        if ($file_info->file_count > 1) {
            $file_icon = "/images/webfile_icon_full.png";
        }

        $jslink = "wfpu(this.form.name,\"$field\",\"$onchange\",\"$typemask\")";

        $linkstyle = "style=\"display: inline;\"";
        if ($small_box == "on") {
            $box_size = "12";
            $image_link = "";
        } else {
            $box_size = "25";
            $image_link = "<input title=\"List All\" type=\"button\" name=\"${field}_button\" onclick=$jslink border=0 style=\"background-image: url('$file_icon'); width: 24px;background-repeat:no-repeat;background-position:center;display:inline;\">";
        }
        if ($no_box == 'on') {
            $text_box_type = 'hidden';
        } else {
            $text_box_type = 'text';
        }

        echo "<input type=hidden name=\"${field}_webfile_application_name\" value=\"$application_name\">";

        if ($invisible != "on") {
            echo "<input type=hidden id=\"${field}_webfile_id_$file_info->file_group_id\" name=\"$field\" value=\"$file_info->file_group_id\"><input title=\"Select File(s)\" type=\"$text_box_type\" size=\"$box_size\" name=\"${field}_disp\" value=\"$file_info->file_group_text\" $linkstyle onclick=$jslink readonly>&nbsp;$image_link";
        } else {
            echo "<input type=hidden name=\"$field\" value=\"$file_info->file_group_id\">";
        }
    }

    function load_javascript_generic_watch_form_element()
    {
        global $javascript_generic_watch_form_element_loaded;
        if ($javascript_generic_watch_form_element_loaded == 'Y') return 0;
        $javascript_generic_watch_form_element_loaded = 'Y';
        echo "
	<script>
	function generic_watch_form_element(id,onchange) {
		obj=document.getElementById(id);
		if (obj!=null) {
			if (obj.watch_initialized!='Y') {
				obj.value_orig=obj.value;
				obj.watch_initialized='Y';
				if (typeof onchange != 'undefined') {
					obj.onchange_string=onchange;
					}
				}
			if (obj.value!=obj.value_orig) {
				obj.value_orig=obj.value;
				if (typeof obj.onchange != 'function') {
					tmp_oc=obj.onchange.replace('--object_element_id--',id);
					obj.onchange=tmp_oc;
					eval(obj.onchange);
					} else {
					tmp_oc_string=obj.onchange.toString();
					tmp_oc_string=tmp_oc_string.replace('--object_element_id--',id);
					tmp_oc_string=tmp_oc_string.replace('function onchange(event) {','');
					tmp_oc_string=tmp_oc_string.replace('function onchange()','');
					tmp_oc_string=tmp_oc_string.replace('}','');
					tmp_oc_string=tmp_oc_string.replace('{','');
					//obj.onchange=tmp_oc_string;
					//alert(tmp_oc_string);
					eval(tmp_oc_string);
					//eval(obj.onchange);
					}
				setTimeout(\"generic_watch_form_element('\" + id + \"')\",100);
				} else {
				setTimeout(\"generic_watch_form_element('\" + id + \"')\",100);
				}
			}
		}
	</script>
	";
        return 0;
    }


    function load_webfile_javascript()
    {
        global $devel, $_webfile_javascript_popup, $_urlencode_javascript;

        if ($_webfile_javascript_popup != "1") {
            $_webfile_javascript_popup = "1";

            if ($_urlencode_javascript != "1") {
                $_urlencode_javascript = "1";
                echo "<script src=\"/javascript/urlencode.js\"></script>";
            }
            load_javascript_generic_watch_form_element();
            /*function watch_form_element(id) {
			obj1=document.getElementById(id);
			obj2=document.getElementById(id + 'orig_value');
			if (obj1.value!=obj2.value) {
				obj2.onchange();
				} else {
				setTimeout(\"watch_form_element('\" + id + \"')\",100);
				}
			}
			*/
            echo "<script>
		function watch_form_element(id) {
			generic_watch_form_element(id);
			}
		function wfpu(formname,fieldname,onchange,settings) {
			field=formname + '.' + fieldname;
			eval('default_id=document.' + field + '.value');
			eval('object_id=document.' + field + '.id');
			if (object_id!='') {
				watch_obj=document.getElementById(object_id + 'orig_value');
				if(watch_obj){
					if (document.getElementById(object_id + 'orig_value')!='') {
						if (document.getElementById(object_id + 'orig_value').onchange!='') {
							watch_form_element(object_id);
							}
						}
					}
				}
			field=urlencode(field);
			onchange=urlencode(onchange);
			open(\"/global/webfile$devel/?mode=webfile_popup&field=\" + field + \"&file_group_id=\" + default_id + \"&settings=\" + settings + \"&onchange=\" + onchange,\"webfile_popup_window\",\"width=500,height=500,scrollbars=yes\");
			}
		function wfpu2(formname,fieldname,onchange,settings) {
			field=formname + '.' + fieldname;
			eval('default_id=document.' + field + '.value');
			eval('object_id=document.' + field + '.id');
			obj=document.getElementById(object_id);
			if (obj.onchange_string!=\"\") {
				watch_form_element(object_id);
				//alert('watching ' + object_id);
				//onchange='';
				}
			//if (object_id!='') {
			//	watch_obj=document.getElementById(object_id);
			//	if(watch_obj){
			//		if (document.getElementById(object_id + 'orig_value')!='') {
			//			if (document.getElementById(object_id + 'orig_value').onchange!='') {
			//				watch_form_element(object_id);
			//				}
			//			}
			//		}
			//	}
			field=urlencode(field);
			onchange=urlencode(onchange);
			//open(\"/global/webfile$devel/?mode=webfile_popup&field=\" + field + \"&file_group_id=\" + default_id + \"&settings=\" + settings + \"&onchange=\" + onchange,\"webfile_popup_window\",\"width=500,height=500,scrollbars=yes\");
			open(\"/global/webfile$devel/?mode=webfile_popup&field=\" + field + \"&file_group_id=\" + default_id + \"&settings=\" + settings,\"webfile_popup_window\",\"width=500,height=500,scrollbars=yes\");
			}
		</script>";
        }

    }


    function fd_document_associator_set_default_options($array)
    {
        global $doc_id;
        $default_options = array(
            'quiet_errors' => 'Y',
            'show_types' => '', // Comma separated list of types to show. Blank = all
            'hide_types' => 'pal,inbound',
            'show_attachments' => 'N',
            'doc_id' => '',
            'application_name' => '',
            'one_selection_only' => 'N',
            'div_max_width' => '300px',
            'div_max_height' => '200px',
            'div_border' => '2px solid black',
            'show_sections' => '', // Not fully implimented in standard linker (is in OIC log)
            'hide_sections' => '', // Not fully implimented in standard linker (is in OIC log)
            'read_only' => 'N'
        );

        //if ($array['onchange']!='') $default_options['onchange']=$array['onchange'];
        //if (isset($array['onchange'])) $default_options['onchange']=$array['onchange'];
        if ($array['quiet_errors'] != '') $default_options['quiet_errors'] = $array['quiet_errors'];
        if ($array['show_types'] != '') $default_options['show_types'] = $array['show_types'];
        if ($array['hide_types'] != '') $default_options['hide_types'] = $array['hide_types'];
        if ($array['show_sections'] != '') $default_options['show_sections'] = $array['show_sections'];
        if ($array['hide_sections'] != '') $default_options['hide_sections'] = $array['hide_sections'];
        if ($array['show_attachments'] != '') $default_options['show_attachments'] = $array['show_attachments'];
        if ($array['doc_id'] != '') $default_options['doc_id'] = $array['doc_id'];
        if ($array['list_id'] != '') $default_options['list_id'] = $array['list_id'];
        if ($default_options['doc_id'] == '') {
            if ($doc_id != '') $default_options['doc_id'] = $doc_id;
        } else {
            if ($default_options['quiet_errors'] == 'N') echo "Error: fd_document_associator_set_default_options: detecting document information!";
            return (FALSE);
        }
        if ($array['list_id'] == '') {
            if ($default_options['quiet_errors'] == 'N') echo "Error: fd_document_associator_set_default_options: list_id error.";
            return (FALSE);
        }
        if ($array['application_name'] != '') $default_options['application_name'] = $array['application_name'];
        if ($array['one_selection_only'] != '') $default_options['one_selection_only'] = $array['one_selection_only'];
        if ($array['div_max_height'] != '') $default_options['div_max_height'] = $array['div_max_height'];
        if ($array['read_only'] == 'Y') $default_options['read_only'] = $array['read_only'];

        if (array_key_exists('onchange', $array)) {
            $default_options['onchange'] = $array['onchange'];
        } else {
            //$options_encoded=base64_encode(serialize($options));
            $default_options['onchange'] = "fd_doc_box_refresh(" . $array['list_id'] . ",'" . (base64_encode(serialize($default_options))) . "')";
        }

        return ($default_options);
    }


    function fd_doc_box_with_attachments($options = "")
    {
        global $application_name, $devel;
        ajax_load();
        $list_id = rand();
        $options['list_id'] = $list_id;
        $options = fd_document_associator_set_default_options($options);
        $options_encoded = base64_encode(serialize($options));
        //echo "<pre>";print_r($options);echo"</pre>";
        load_javascript_generic_watch_form_element();
        load_javascript_fd_doc_box();
        echo "<div style=\"max-width: " . $options['div_max_width'] . "; overflow: auto; border:" . $options['div_border'] . "\" id=\"" . $options['list_id'] . "\">";
        include("../front_desk$devel/short_doc_list_with_attachments.phtml");
        echo "
	</div>
	<script>
	";
        //if ($options['onchange']!='') {
        echo "document.getElementById('$list_id').onchange=\"" . $options['onchange'] . "\"
	document.getElementById('$list_id').value=1;
	generic_watch_form_element($list_id);
	";
        //	}
        echo "
	document.getElementById('$list_id').ajax_trigger='never';
	</script>";
        return 0;
    }

    function load_javascript_fd_doc_box()
    {
        global $javascript_fd_doc_box_loaded, $devel;
        if ($javascript_fd_doc_box_loaded == 'Y') return 0;
        $javascript_fd_doc_box_loaded = 'Y';
        echo "
	<script>
	function fd_doc_box_refresh(id,options_encoded) {
		ajaxManager('load_page','/global/front_desk$devel/?mode=short_doc_list_with_attachments&options_encoded=' + options_encoded,id);
		}
	</script>
	";
        return 0;
    }

    function fd_doc_box_default_where($options)
    {
        $where_add = "";
        if ($options['show_types'] != "") {
            $sep = "";
            $type_list = explode(',', ereg_replace(" ", "", $options['show_types']));
            $where_add = $where_add . " and ( ";
            while ($type = array_pop($type_list)) {
                $where_add = $where_add . " $sep " . " doc_type = '$type' ";
                $sep = "or";
            }
            $where_add = $where_add . " ) ";

            $sep = "";
            $type_list = "";
            $type = "";
        }

        if ($options['hide_types'] != "") {
            $type_list = explode(',', ereg_replace(" ", "", $options['hide_types']));
            while ($type = array_pop($type_list)) {
                $where_add = $where_add . " and doc_type != '$type' ";
            }
            $type_list = "";
            $type = "";
        }

        if ($options['show_sections'] != "") {
            $section_list = explode(',', ereg_replace(" ", "", $options['show_sections']));
            while ($section = array_pop($section_list)) {
                $where_add = $where_add . " and section = '$section' ";
            }
            $section_list = "";
            $section = "";
        }

        return ($where_add);

    }

    function fd_short_doc_list_with_attachments($options)
    {
        global $devel;
        $where_add = "";
        $options_encoded = base64_encode(serialize($options));


        $where_add = fd_doc_box_default_where($options);
        $query = "select * from documents_links right join documents on (documents_links.docb = documents.doc_id) where doca = '" . $options['doc_id'] . "' $where_add order by doc_type, section, sort_rank, app_reference + 0 ASC,app_reference";

        $res = @mysql_query($query);
        $count = @mysql_num_rows($res);
        if ($count < 1) return ("<font color=blue style=\"cursor: hand; cursor: pointer;\" onclick=\"open('/global/front_desk$devel/index.html?mode=fd_doclist_popup2&options_encoded=$options_encoded','fd_docs_win','width=600,height=600,scrollbars=yes');\" style=\"text-decoration: none;\">Link Doc(s)</font>");
        $return_text = "<table width=100% border=0 cellspacing=0 cellpadding=1>";
        while ($row = @mysql_fetch_object($res)) {
            if ($row->doc_type == "rfi") $row->doc_type = 'umcrfi';
            $row->description = htmlspecialchars($row->description);
            $return_text = $return_text . "<tr><td width=100%><font color=blue style=\"cursor: hand; cursor: pointer;\" onclick=\"open('/global/front_desk$devel/index.html?mode=fd_doclist_popup2&options_encoded=$options_encoded','fd_docs_win2','width=600,height=600,scrollbars=yes');\" title=\"$row->description\" style=\"text-decoration: none;\">$row->doc_type $row->app_reference</a></td><td>&nbsp;&nbsp;<i><a href=\"/global/front_desk$devel/?mode=open_doc&doc_id=$row->doc_id\" target=\"show_doc_$row->doc_id\" style=\"text-decoration: none;\"><font style=\"font-weight: bold; background: #bb0000; color: white; border: 1px solid black; cursor: hand; cursor: pointer;\">&lt;&#8209;jump&nbsp;to&#8209;</font></a></i></td></tr>";
        }
        $return_text = $return_text . "</table>";
        return ($return_text);
    }

    function fd_doclist_doc_row($options, $row)
    {
        global $fd_color_5, $global_user_id, $fd_color_4;
        $error = 0;
        $removal_error = 0;
        $link = getoneb("select * from documents_links where doca = '" . $options['doc_id'] . "' and docb = '$row->doc_id' limit 1");
        if ($options['toggle'] == 'Y') {
            if ($link) {
                if ($link->file_group_id > 0) {
                    if (getoneb("select * from webfile_files where file_group_id = '$link->file_group_id' limit 1")) $removal_error = 1;
                    else $update_query = "call doc_link_delete(" . $options['doc_id'] . ",$row->doc_id)";
                } else {
                    $update_query = "call doc_link_delete(" . $options['doc_id'] . ",$row->doc_id)";
                }
            } else {
                $update_query = "call doc_link(" . $options['doc_id'] . ",$row->doc_id,$global_user_id,'')";
            }
            if ($removal_error != '1') @mysql_query($update_query);
            $link = getoneb("select * from documents_links where doca = '" . $options['doc_id'] . "' and docb = '$row->doc_id' limit 1");
        }

        $bstart = "";
        $bend = "";
        $row_bg = "";
        if ($link) {
            $row_bg = "$fd_color_4";
            $bstart = "<b>";
            $bend = "</b>";
        }
        if ($removal_error == 1) $row_bg = $fd_color_5;
        echo "
		<td style=\"width: 120px; border-top: 1px solid black;\" id=\"1_$row->doc_id\" bgcolor=\"$row_bg\"\>
			<font size=\"-1\"><b>$row->app_reference</b>&nbsp;&nbsp;</font>
		</td><td style=\"border-top: 1px solid black;\" id=\"2_$row->doc_id\" bgcolor=\"$row_bg\">
			<font size=\"-1\">$row->description</font>
		</td>";
    }


    function webfilebox2($value, $field, $onchange = "", $options = array(), $target_win_name = "webfile_popup")
    {
        global $_webfile_javascript_popup, $_urlencode_javascript, $application_name, $devel;
        // GPH MARK - absolute path
        include("/data/web/pipeline/global$devel/webfile$devel/settings_option_defaults.phtml");
        $rand = rand('10000', '30000');
        $id = 'form_input_' . $rand;

        load_webfile_javascript();
        if ($_webfile_javascript_popup != "1") {
            $_webfile_javascript_popup = "1";

            if ($_urlencode_javascript != "1") {
                $_urlencode_javascript = "1";
                echo "<script src=\"/javascript/urlencode.js\"></script>";
            }

        }

        $onchange_orig = $onchange;
        $onchange = base64_encode($onchange);

        if ($value != "") {
            $value = addslashes($value);
            $file_info = load_file_group_info($value);
        }

        //////////////////////////////////////////////////////////////////////////////
        // Choose a preview on the tiny button next to the input field
        //////////////////////////////////////////////////////////////////////////////
        if ($file_info->file_count < 1) {
            $file_icon = "/images/webfile_icon.png";
        }
        if ($file_info->file_count > 1) {
            $file_icon = "/images/webfile_icon_full.png";
        }
        if ($file_info->file_count == 1) {
            $file = getone("select * from webfile_files where file_group_id = '$file_info->file_group_id'");
            $file_icon = webfile_tiny_image_icon($file->files_id);
        }


        $jslink = "wfpu2(this.form.name,\"$field\",'','')";

        $linkstyle = "style=\"display: inline;\"";
        if ($options['small_box'] == 'Y') {
            $image_link = "";
        } else {
            $image_link = "<input title=\"List All\" type=\"button\" name=\"${field}_button\" onclick=$jslink border=0 style=\"padding: 0px; background-image: url('$file_icon'); width: 24px;background-repeat:no-repeat;background-position:center;display:inline;\">";
        }

        if ($options['no_box'] == 'Y') {
            $text_box_type = 'hidden';
        } else {
            $text_box_type = 'text';
        }

        $box_size = $options['box_size'];
        echo "<input type=hidden name=\"${field}_webfile_application_name\" value=\"$application_name\">";

        //<input type=hidden id=\"${id}orig_value\" onchange=\"$onchange_orig\" value=\"$file_info->file_group_id\">
        if ($options['invisible'] != 'Y') {
            echo "<table style=\"display: inline; padding: 0px;\" border=0 cellspacing=0 cellpadding=0><tr><td>
		<input type=hidden id=\"$id\" name=\"$field\" onload=\"this.onchange_string='$onchange_orig'\" onchange=\"$onchange_orig\" value=\"$file_info->file_group_id\">
		<input style=\"padding: 0px;\" title=\"Select File(s)\" type=\"$text_box_type\" size=\"$box_size\" name=\"${field}_disp\" value=\"$file_info->file_group_text\" $linkstyle onclick=$jslink readonly></td><td>
		$image_link</td></tr></table>";

        } else {
            echo "<input type=hidden name=\"$field\" value=\"$file_info->file_group_id\">";
        }
        return ($id);
    }


    function recent_contacts_expire($userid)
    {
        $expire_line = date('Ymd', strtotime('-59 days'));
        $expire_line = str_pad($expire_line, 14, "0", STR_PAD_RIGHT);
        @mysql_query("delete from contacts_history where loadtime <= '$expire_line'");
    }


    function recent_jobs_expire($userid)
    {
        $expire_line = date('Ymd', strtotime('-29 days'));
        $expire_line = str_pad($expire_line, 14, "0", STR_PAD_RIGHT);
        @mysql_query("delete from jobinfo_history where accessed <= '$expire_line'");
    }

    function recent_contacts_query()
    {
        //reg_usage(__FUNCTION__);
        $global_user_id = $GLOBALS['global_user_id'];
        if ($global_user_id == "") return false;
        recent_contacts_expire($global_user_id);
        $query = "select * from contacts_history left join contacts on (contacts_history.contact_id = contacts.contacts_id) where global_user_id = '$global_user_id' group by contacts.contacts_id order by loadtime desc limit 5";
        return $query;
    }

    function contacts_hist_add($contacts_id, $mode = "auto")
    {
        $global_user_id = $GLOBALS['global_user_id'];
        $application_name = $GLOBALS['application_name'];
        if ($global_user_id == "") return 0;
        if ($contacts_id == "") return 0;
        recent_contacts_expire($global_user_id);

        @mysql_query("delete from contacts_history where global_user_id = '$global_user_id' and contact_id = '$contacts_id'");
        $query = "insert into contacts_history set global_user_id = '$global_user_id', contact_id = '$contacts_id',application_name = '$application_name',mode = '$mode'";
        @mysql_query($query);

    }

    function map_link_this_contact_here($contacts_id)
    {
        $contact_info = getoneb("select * from contacts where contacts_id = '$contacts_id'");
        if (($contact_info->address_1 != "") && ($contact_info->address_city != "")) {
            if ($contact_info->address_state == "") $contact_info->address_state == "WA";
            $address = urlencode($contact_info->address_1);
            $city = urlencode($contact_info->address_city);
            $state = $contact_info->address_state;
            echo "<a target=contact_mapquest_win href=\"http://www.mapquest.com/maps/map.adp?address=$address&city=$city&state=$state&zipcode=$zipcode&submit=Get+Map\"><font color=blue>Map it</font></a>";
        } else {
            return 1;
        }
    }

    function radio_box2($text, $name, $value = "N", $otherletter = "P", $other_otherletter = "")
    {
        if ($value == "Y") $yes_check = " checked";
        else $yes_check = "";
        if ($value == "N") $no_check = " checked";
        else $no_check = "";
        if ($value == $otherletter) $other_check = " checked";
        else $other_check = "";

        if ($other_otherletter == "") {
            echo "<table border=0 cellspacing=0 cellpadding=0><tr><td valign=center>$text</td><td valign=center>Y</td><td valign=center><input type=radio name=$name value='Y'$yes_check></td><td valign=center>N</td><td valign=center><input type=radio name=$name value='N'$no_check></td><td valign=center>$otherletter</td><td valign=center><input type=radio name=$name value='$otherletter'$other_check></td></tr></table>";
        } else {

            if ($value == $other_otherletter) $other_other_check = " checked";
            else $other_other_check = "";

            echo "<table border=0 cellspacing=0 cellpadding=0><tr><td valign=center>$text</td><td valign=center>Y</td><td valign=center><input type=radio name=$name value='Y'$yes_check></td><td valign=center>N</td><td valign=center><input type=radio name=$name value='N'$no_check></td>
		<td valign=center>$otherletter</td><td valign=center>
		<input type=radio name=$name value='$otherletter'$other_check></td>

		<td valign=center>$other_otherletter</td><td valign=center>
		<input type=radio name=$name value='$other_otherletter'$other_other_check></td>
		</tr></table>";
        }
    }

    function contact_view_javascript()
    {
        if ($GLOBALS['contact_view_js'] != 'Y') {
            $GLOBALS['contact_view_js'] = 'Y';
            echo "
		<script>
		function contact_view(contact) {
			open('/global/contacts/contactview.phtml?contacts_id=' + contact,'contact_info_win' + contact,'width=500,height=640,scrollbars=yes,resizable=yes');
			}
		</script>
		";
        }
    }

    function contact_view($name, $contact)
    {
        if ($contact <= 0) {
            echo $name;
            return false;
        }
        contact_view_javascript();
        $c_display_obj = getoneb("select display_name from contacts where contacts_id = '$contact'");
        if ($name == "") $name = $c_display_obj->display_name;
        echo "<a title=\"View\" href=javascript:contact_view($contact);><font color=blue>$name</font></a>";
        return $c_display_obj->display_name;
    }

    function contact_view_link($contact)
    {
        if ($contact <= 0) return false;
        $c_display_obj = getoneb("select display_name from contacts where contacts_id = '$contact'");
        if (!($c_display_obj)) return false;

        if ($GLOBALS['contact_view_js'] != 'Y') {
            $GLOBALS['contact_view_js'] = 'Y';
            echo "
		<script>
		function contact_view(contact) {
			open('/global/contacts/contactview.phtml?contacts_id=' + contact,'contact_info_win' + contact,'width=500,height=640,scrollbars=yes,resizable=yes');
			}
		</script>
		";
        }
        return ("javascript:contact_view($contact);");
    }

    function security_log_email($user, $group, $job, $action, $application)
    {
        global $global_user, $security_contact;
        $security_info = getoneb("select * from contacts where contacts_id = '$security_contact'");
        $user_info = getoneb("select * from contacts where contacts_id = '$user'");
        $group_info = getoneb("select * from contacts_groups where group_id = '$group'");
        $job_info = getoneb("select * from jobinfo where jobinfo_id = '$job'");
        $from = "From: $global_user->email";
        $for_who = "";
        $sep = "";
        if ($user_info) {
            $for_who = "(USER $user_info->login_name)";
            $sep = "/";
        }
        if ($group_info) {
            $for_who = "${for_who}${sep}(GROUP $group_info->description)";
            $sep = "/";
        }
        if ($job_info) {
            $for_who = "${for_who}${sep}(JOB $job_info->contract_num)";
            $sep = "/";
        }

        $subject = "permission change for $for_who";

        $message = "$global_user->login_name ($global_user->contacts_id) changed permissions for $for_who
$action
Application: $application
";
        // GPH TEMP CODE: Commented out
//	mail($security_info->email,$subject,$message,$from);
//	mail("dseriales@umci.com",$subject,$message,$from);
//	mail("jeffb@umci.com",$subject,$message,$from);


    }


    function queue_email($contact_id, $subject, $message, $send_now = "N", $from_contact = "")
    {
        $global_user_id = $GLOBALS['global_user_id'];
        if ($global_user_id == "") $global_user_id = "362";
        $info = getone("select * from contacts where contacts_id = '$contact_id'");
        if ($info->email == "") {
            $subject = "Contact ERROR: no email address! contacts_id=2,display_name=$info->display_name";
            $info = getone("select * from contacts where contacts_id = 2");
            $message = "Subject: $subject


" . $message;
            $subject = "Contact ERROR: no email address!";
        }

        $message = addslashes($message);
        $subject = addslashes($subject);

        $query = "insert into webmail_queue set
	creator = '$global_user_id',
	contacts_id = $info->contacts_id,
	email = '$info->email',
	display_name = '$info->display_name',
	subject = '$subject',
	message = '$message',
	send_now = '$send_now',
	create_time = now()";
        @mysql_query($query);
    }

    function is_reminder_day($date, $hz, $before = 0, $on = 1, $after = 1)
    {
        $today = date('m-d-Y');
        $today_time = strtotime(vali_date($today));
        $date_time = strtotime(vali_date($date));
        $time_diff = $date_time - $today_time;

        $days_diff = $time_diff / 86400;
        $remind = $days_diff % $hz;

        if ($remind == 0) {
            if (($on > 0) && ($days_diff == 0)) return true;
            if (($before > 0) && ($today_time < $date_time)) {
                echo "<hr>$today/$date/$days_diff<hr>";
                return true;
            }
            if (($after > 0) && ($today_time > $date_time)) return true;
        }
        return false;
    }

///////////////////////////////
// SAM uses this
///////////////////////////////
    function float_break($float)
    {
        if (ereg('.', "$float")) {
            $float = ereg_replace("0*$", "", $float);
            $float = ereg_replace("\.0*$", "", $float);
        }
        if ($float == 0) return ("");
        return ($float);
    }

    function ynmselect($fieldname, $default = "", $maybeval = "?", $onchange = "")
    {
        if ($onchange == "") $selectonchange = "";
        else $selectonchange = "onchange=\"$onchange\"";
        if ($maybeval == "") $maybeval = "?";
        if ($default == "") $default = $maybeval;
        if ($default == "Y") $yes = " selected";
        if ($default == "N") $no = " selected";
        if ($default == "$maybeval") $maybe = " selected";
        if ("$yes$no$maybe" == "") $maybe = " selected";

        echo "<select name=\"$fieldname\" $selectonchange>
	<option value=\"Y\"$yes>Y
	<option value=\"N\"$no>N
	<option value=\"$maybeval\"$maybe>$maybeval
	</select>";
    }


    function timefix($timetext, $quotes = 'true')
    {
        if ($timetext == "") {
            if ($quotes == 'true') return "NULL";
            else return "";
        }
        $time = strtotime($timetext);
        $time_mil = date('H:i:s', $time);
        if ($quotes == 'true') return "'$time_mil'";
        else return $time_mil;
    }

    function timebreak($time_mil, $seconds = 0)
    {
        if (($time_mil == "") || ($time_mil == "NULL")) return "";
        $time = strtotime($time_mil);
        if ($seconds != 0) $time_mil = date('g:i:s a', $time);
        else $time_mil = date('g:i a', $time);
        return $time_mil;
    }

    function clean_int($var)
    {
        $var = ereg_replace('[^0-9.]', '', $var);
        $var = ereg_replace('\..*', '', $var);
        $var = escapeshellcmd($var);
        return ($var);
    }

    function intbreak($var, $fln = 0)
    {
        if ($var == 0) return "";
        else {
            if ($fln == 0) return $var;
            else return (format_long_num($var));
        }
    }

    function format_long_num($var)
    {
        $newvar = "";
        $var = ereg_replace("\..*$", "", $var);
        $length = strlen($var);

        $count = 0;
        while ($count < ($length - 1)) {
            $count++;
            $newvar = $var[$length - $count] . $newvar;
            $mod = $count % 3;
            if (($count != 0) && ($mod == 0)) {
                $newvar = "," . $newvar;
            }
        }
        $newvar = $var[0] . $newvar;
        return $newvar;
    }

    function format_long_decimal($var, $decimal_places = 0)
    {
        if ($decimal_places != 0) $var = round($var, $decimal_places);
        $newvar = "";
        $sub_one = ereg_replace("^.*\.", "", $var);
        if ($sub_one == $var) $sub_one = "";
        else {
            if ($decimal_places != 0) $sub_one = str_pad($sub_one, $decimal_places, "0", STR_PAD_RIGHT);
            $sub_one = "." . "$sub_one";
        }
        $var = ereg_replace("\..*$", "", $var);
        $length = strlen($var);

        $count = 0;
        while ($count < ($length - 1)) {
            $count++;
            $newvar = $var[$length - $count] . $newvar;
            $mod = $count % 3;
            if (($count != 0) && ($mod == 0)) {
                $newvar = "," . $newvar;
            }
        }
        $newvar = $var[0] . $newvar;
        return $newvar . $sub_one;
    }


#######################################################################
#	Function for returning miscelaneous data
#######################################################################

    function get_misc_data($field)
    {
        $field = escapeshellcmd($field);
        $query = "select * from misc_data where data_type = '$field'";
        $data = getoneb($query);

        if ($data) return $data->data;
        else return $data;
    }

    function get_misc_description($field)
    {
        $field = escapeshellcmd($field);
        $query = "select * from misc_data where data_type = '$field'";
        $data = getoneb($query);
        if ($data->description != "") return $data->description;
        else return $field;
    }

#######################################################################
#	Function for showing or editing misc data
#######################################################################

    function misc_data_show($fieldlist, $edit = 0)
    {
        $list = explode(",", $fieldlist);
        echo "<table border=0 cellspacing=0 cellpadding=3>";
        $counter = 0;
        while ($item = array_shift($list)) {
            if ($counter == 1) {
                $counter = 0;
                $bgcolor = "#dddddd";
            } else {
                $counter = 1;
                $bgcolor = "#ffffff";
            }
            $data = get_misc_data($item);
            $text = get_misc_description($item);

            echo "<tr bgcolor=$bgcolor><td>
				<font title=\"$item\">$text</font>
			</td><td>
				$data
			";
            if ($edit == 1) echo "</td><td><form>
			<input type=button onclick=open('$pagename?mode=misc_data_edit&data_type=$item','misc_data_edit_win','width=600,height=100'); value=Modify></form>";
            echo "</td></tr>";
        }
        echo "</table>";
    }

#######################################################################
#	Function to edit a data field
#######################################################################

    function misc_data_form($data_type)
    {
        $data_type = escapeshellcmd($data_type);
        $data_info = getone("select * from misc_data where data_type = '$data_type'");
        echo "<form name=misc_data method=post action=$pagename>
	<input type=hidden name=mode value=misc_data_submit>
	<input type=hidden name=data_type value=\"$data_type\">
	<b>$data_type:&nbsp;<input type=text size=50 name=data value=\"$data_info->data\">&nbsp;<input type=submit value=Save>
	</form>";
    }

#######################################################################
#	Function to save a data field
#######################################################################

    function misc_data_update($data_type, $data)
    {
        $data_type = addslashes($data_type);
        $data = addslashes($data);
        $query = "update misc_data set data = '$data' where data_type = '$data_type'";
        $res = @mysql_query($query);
        if (!($res)) die ("ERROR: Misc Data update failed. Contact your network administrator!<p>Failed Query:<hr>$query");

        echo "
	<script>
	opener.document.location.reload();
	self.close();
	</script>
	";
    }


#######################################################################
#
#	Function just like "dropbox()", but it has built-in defaults for
#	showing contacts. They are exactly the same except "$query"
#	has been replaced with "$sqlappend". The sqlappend parameter
#	can be used to specify sql parameters to follow the normal
#	contact select command (can't change select, but can change
#	second half of from, and where clause).
#	Oh yeah, it take "as $variable" for the varname for the form..
#
#######################################################################

    function contactbox($query, $default_id = "", $size = "1", $onchange = "", $blank = 1, $multiple = "")
    {
        $result = @mysql_query($query);
        if (!($result)) {
            echo "<select name=error>
			<option selected>Error referencing lookup table!!!!</option>
			<option>Please inform the system administrator!!!</option>
			</select>";
            return 1;
        }

        $rowcount = mysql_num_rows($result);
        if ($rowcount == 0)
            return 0;

        if ($size != 1) {
            if ($size > $rowcount) $size = $rowcount;
            if ($blank >= 1) $size++;
        }

        $fieldname = mysql_field_name($result, 0);
        echo "<select name=\"$fieldname\" size=\"$size\" onchange=\"$onchange\" $multiple>";
        if ($blank > 0) echo "<option> </option>";
        while ($record = @mysql_fetch_object($result)) {
            eval("\$record_id=\$record->$fieldname;");
            $name = "";
            if ($record_id == $default_id)
                $selected = "selected";
            else
                $selected = "";

            if (($record->current != 'N') || ($record_id == $default_id))
                echo "<option value=\"$record_id\" $selected>$record->display_name</option>";
        }
        echo "</select>";
        return ($rowcount);
    }


#######################################################################
#	Function to dump a table that results from $query
#	in an HTML format...
#######################################################################

    function tabledump($query, $header = "1", $header_text = "", $link = "")
    {
        if ($link == "") $results = mysql_query("$query");
        else $results = mysql_query("$query", $link);

        if (!($results)) {
            die("Failed query sent to function tabledump()!<br>Please contact your system administrator!!!<p>Failed Query:<hr>$query");
            exit;
        }
        $width = mysql_num_fields($results);

        echo "<table border=1 cellspacing=0 cellpadding=3>";
        if ($header == "1") {
            if ($header_text != "") echo "<tr bgcolor=#dddddd><td colspan=$width align=center><b>$header_text</b></td></tr>";
            echo "<tr bgcolor=#dddddd>";
            while ($column < $width) {
                $field = mysql_field_name($results, $column);
                echo "<td><b><font color=#000000>$field</font></b></td>";
                $column++;
            }
            echo "</tr>";
        }
        $column = 0;

        while ($row = (@mysql_fetch_row($results))) {
            $column = 0;
            echo "<tr>";
            while ($column < $width) {
                if ($row[$column] == "") $row[$column] = '&nbsp;';
                echo "<td>$row[$column]</td>";
                $column++;
            }
            echo "</tr>";
        }
        echo "</table>";

    }


#######################################################################
#	Gets and returns the first record from $query (or errors
#	out if more than one record is returned)
#######################################################################

    function getone($query, $link = "")
    {
        if ($link == "")
            $result = (mysql_query($query));
        else
            $result = (@mysql_query($query, $link));

        $rowcount = @mysql_num_rows($result);
        if ($rowcount != 1) {
            if ($result)
                echo "Error: Non unique or missing record in function \"getone()\"<p>The following query returned $rowcount rows:<hr>$query";
            else
                echo "Error: Invalid query in function \"getone()\"<p>The following query could not be run:<hr>$query";
            exit;
        }
        return (mysql_fetch_object($result));
    }

#######################################################################
#	Gets and returns the first record from $query (or errors
#	out if more than one record is returned)
#######################################################################

    function getoneb($query, $link = "")
    {
        //die($query);

        if ($link == "")
        {
            $link = mysql_connect("127.0.0.1:3306", "globalreadwrite", "") or die(mysql_error());
            mysql_select_db("global");
            $result = mysql_query($query, $link);
        }
        else
            $result = mysql_query($query, $link);
        
        if ((mysql_num_rows($result)) != 1) {
            return FALSE;
        }
        return (mysql_fetch_object($result));
    }


#######################################################################
#
#	Changes the date format.. Dies if input is invalid...
#
#######################################################################

    function vali_date($day)
    {
        $day_debug = $day;
        if ($day == "") {
            return "";
        }

        if (ereg("^[0-9][0-9][0-9][0-9][-/][0-9][0-9][-/][0-9][0-9]$", $day)) return ($day);
        $day = ereg_replace("\.", "-", $day);
        $day = ereg_replace("/", "-", $day);
        $day = ereg_replace(",", "-", $day);
        $day = ereg_replace(" ", "-", $day);
        $day = ereg_replace("--", "-", $day);

        $day = eregi_replace("january", "1", $day);
        $day = eregi_replace("february", "2", $day);
        $day = eregi_replace("march", "3", $day);
        $day = eregi_replace("april", "4", $day);
        $day = eregi_replace("may", "5", $day);
        $day = eregi_replace("june", "6", $day);
        $day = eregi_replace("july", "7", $day);
        $day = eregi_replace("august", "8", $day);
        $day = eregi_replace("september", "9", $day);
        $day = eregi_replace("october", "10", $day);
        $day = eregi_replace("november", "11", $day);
        $day = eregi_replace("december", "12", $day);

        $day = eregi_replace("jan", "1", $day);
        $day = eregi_replace("feb", "2", $day);
        $day = eregi_replace("mar", "3", $day);
        $day = eregi_replace("apr", "4", $day);
        $day = eregi_replace("jun", "6", $day);
        $day = eregi_replace("jul", "7", $day);
        $day = eregi_replace("aug", "8", $day);
        $day = eregi_replace("sept", "9", $day);
        $day = eregi_replace("sep", "9", $day);
        $day = eregi_replace("oct", "10", $day);
        $day = eregi_replace("nov", "11", $day);
        $day = eregi_replace("dec", "12", $day);

        $day = eregi_replace("first", "1", $day);
        $day = eregi_replace("second", "2", $day);
        $day = eregi_replace("third", "3", $day);
        $day = eregi_replace("fourth", "4", $day);
        $day = eregi_replace("fifth", "5", $day);
        $day = eregi_replace("sixth", "6", $day);
        $day = eregi_replace("seventh", "7", $day);
        $day = eregi_replace("eighth", "8", $day);
        $day = eregi_replace("nineth", "9", $day);
        $day = eregi_replace("tenth", "10", $day);

        $day = eregi_replace("th", "", $day);
        $day = eregi_replace("st", "", $day);

        $day = eregi_replace("-[0-9]*:.*$", "", $day);

        if (ereg("^([0-9]{1,2})[-]([0-9]{1,2})$", $day, $sdate)) {
            $month = $sdate[1];
            $current_month = date("m");
            $current_year = date("Y");
            $month++;
            if ($month >= $current_month) {
                $year = $current_year;
            } else {
                $year = $current_year + 1;
            }
            if (($current_month == 1) && ($month == 11)) $year--;
            $day = "$day-$year";
        }

        if (ereg("^([0-9]{1,2})[-]([0-9]{1,2})[-]([0-9]{2,4})$", $day, $argv)) {
            if ("$argv[2]" == "00") $argv[2] = 2000;
            $argv[1] = str_pad($argv[1], 2, "0", STR_PAD_LEFT);
            $argv[2] = str_pad($argv[2], 2, "0", STR_PAD_LEFT);
            return "$argv[3]-$argv[1]-$argv[2]";
        } else {
            echo "vali_date: incorrect function usage!<p>\nPlease use MM-DD-YYYY format<p>\n";
            trigger_error("incorrect date format: \"$day_debug\"");
            exit;
        }
    }


#######################################################################
#
#	Changes the date format back to normal for display...
#
#######################################################################

    function invali_date($day, $seperator = "-", $short_year = 'N')
    {
//return $seperator;
        if ($day != "") {
            if (ereg("^([0-9]{2,4})[/-]([0-9]{1,2})[/-]([0-9]{1,2})$", $day, $argv)) {
                if ($short_year != 'N') $out = "$argv[2]$seperator$argv[3]$seperator" . substr("$argv[1]", -2, 2);
                else $out = "$argv[2]$seperator$argv[3]$seperator$argv[1]";
                if ($out == "00${seperator}00${seperator}0000") return "";
                return $out;
            } else {
                if (ereg("^([0-9]{1,2})[/-]([0-9]{1,2})[/-]([0-9]{2,4})$", $day, $argv)) {
                    $day = ereg_replace('[/-]', "$seperator", $day);
                    return $day;
                } else {
                    echo "\n\n</script><script>alert(\"Invalid date format\");\nhistory.go(-1);\n</script>";
                    exit;
                }
            }
        } else {
            return "";
        }
    }

#######################################################################
#
#	A Function to fix the output from checkboxes to make it something
#	that a mysql permissions table can handle (aka a "Y" or an "N")
#
#######################################################################

    function checkfix($box)
    {
        if ($box == "on") return "Y";
        else return "N";
    }

    function checkbreak($var)
    {
        if ($var == "Y") return " checked";
        else return "";
    }

#######################################################################
#	dropbox();
#	usage:
#	dropbox("$query","$defaultid","$size","$onchange","1");
#	The last argument is a boolean to determine whetherr or not
#	to print an empty first entry or not.. It is compatible with,
#	but useless when using the "defaultid"...
#######################################################################

    function dropbox($query, $defaultid = "", $size = "1", $onchange = "", $blank = 1, $multiple = "", $make_array = 0)
    {
        if (($multiple == "multiple") && ($size > 1)) $blank = 0;
        $result = @mysql_query("$query");
        $rowscounted = mysql_num_rows($result);

        if ($rowscounted == 0) {
            return 0;
        }
        if ($rowscounted == 1) {
            $rowscounted++;
        }
        if ($size > $rowscounted) {
            $size = $rowscounted;
        }

        $fields = @mysql_num_fields($result);
        $offset = 0;
        if ($fields == 1) {
            $fields = 2;
            $offset = 1;
        }
        $row = (@mysql_fetch_row($result));
        $valuename = mysql_field_name($result, 0);
        $selectlist = " ";
        while ($row) {
            $mark = "";
            $selecttext = "";
            if ("$defaultid" == "$row[0]") {
                $mark = " selected";
                $found_default_id = TRUE;
            }
            $fcount = 1 - $offset;
            $selecttext = "";
            while ($fcount <= $fields) {
                $selecttext = "$selecttext$row[$fcount]";
                $fcount++;
            }

            if ($selecttext != "") $selectlist = $selectlist . "<option value=\"$row[0]\"$mark>$selecttext";
            $row = (@mysql_fetch_row($result));
        }

        $firstline = "";
        if ($blank >= 1) {
            $firstline = "<option value=\"\"> ";
        }

        if (($defaultid == "") || ($defaultid == "0")) {
            echo "	<select name=\"$valuename\" onchange=\"$onchange\" size=$size $multiple>
		$firstline
		$selectlist
		</select>";
        } else {
            if ($blank >= 2) {
                if (!($found_default_id) && ($blank >= 3)) {
                    echo "	<select name=\"$valuename\" onchange=\"$onchange\" size=$size $multiple>
				$firstline
				<option value=\"$defaultid\">$defaultid</option>
				$selectlist
				</select>";
                } else {
                    echo "	<select name=\"$valuename\" onchange=\"$onchange\" size=$size $multiple>
				$firstline
				$selectlist
				</select>";
                }
            } else {
                echo "	<select name=\"$valuename\" onchange=\"$onchange\" size=$size $multiple>
			$selectlist
			</select>";
            }
        }
    }


#######################################################################
#
#	Similar to dropbox, but takes an array of values. Pretty names
#	to go with the values can be stored in ['display_text'][value]
#
#######################################################################

    function dropbox_array($array, $field_name, $default_value = "", $tag_extras = "")
    {
        echo "<select name=\"$field_name\" $tag_extras>";
        foreach ($array as $value) {
            if (is_array($value)) continue;
            if (($value != 'display_text') && ($value != "Array ")) {
                $dt = $array['display_text'][$value];
                if ($dt == "") $dt = $value;
                $selected = "";
                if ($value == $default_value) $selected = "selected";
                echo "<option value=\"$value\" $selected>$dt</option>";
            }
        }
        echo "</select>";
    }

#######################################################################
#
#	Prints the select box for the search form (in all logs) by looking
#	at the values of "array"..
#
#######################################################################

    function dropbox_search($array)
    {
        global $search_dropbox_text;
        echo "<select style=\"width: 100px;\" name=\"search_dropbox_text\">";
        $selected = "";
        if ($search_dropbox_text == "") $selected = "selected";
        echo "<option value=\"\" $selected>Default (all)</option>";

        foreach ($array as $value) {
            if (is_array($value)) continue;
            if (($value != 'display_text') && ($value != "Array ")) {
                $dt = $array['display_text'][$value];
                if ($dt == "") $dt = $value;
                $selected = "";
                if ($value == $search_dropbox_text) $selected = "selected";
                echo "<option value=\"$value\" $selected>$dt</option>";
            }
        }
        echo "</select>";
    }

    function dropbox_search2($array)
    {
        global $search_dropbox_text2;
        echo "<select style=\"width: 100px;\" name=\"search_dropbox_text2\">";
        $selected = "";
        if ($search_dropbox_text2 == "") $selected = "selected";
        echo "<option value=\"\" $selected>Default (all)</option>";

        foreach ($array as $value) {
            if (is_array($value)) continue;
            if (($value != 'display_text') && ($value != "Array ")) {
                $dt = $array['display_text'][$value];
                if ($dt == "") $dt = $value;
                $selected = "";
                if ($value == $search_dropbox_text2) $selected = "selected";
                echo "<option value=\"$value\" $selected>$dt</option>";
            }
        }
        echo "</select>";
    }

    function query_search_add_text2()
    {
        global $search_array, $search_text, $search_text2, $dropbox_search_text,
               $search_persistent, $search_last_application;
        /*
	if ($search_reset=='Y') {
		$search_persistent="";
		$search_last_application="";
		$search_last_dropbox_search_text="";
		$search_last_dropbox2_search_text="";
		$search_last_text="";
		$search_last_text2="";
		}
	*/


        $dropbox_search_text2 = $dropbox_search_text;
        $where = "";
        if ($search_text != "") {
            $search_text = addslashes($search_text);
            $search_chunks = explode(" ", $search_text);
            //echo "<b>not null ($search_text)</b><br>";
            //print_r($search_chunks);
            $sep = "";
            while ($search_item = array_shift($search_chunks)) {
                //echo "<hr>$search_item/$search_text<hr>";
                if ($sep != "") {
                    if ($search_item[0] == "+") {
                        $sep = "and";
                        $search_item = substr($search_item, 1);
                    }
                    if ($search_item[0] == "-") {
                        $sep = "and not";
                        $search_item = substr($search_item, 1);
                    }
                }


                if ($dropbox_search_text != "") {
                    $where = $where . " $sep ( $dropbox_search_text like '%$search_item%' ) ";
                } else {
                    $temp_array = $search_array;
                    $wherebit = "";
                    $whereor = "";
                    $temp_array = "";
                    $dropbox_search_text = "";
                    $temp_array = $search_array;
                    //print_r($temp_array);
                    //$temp_array=array('a','b','c','d');
                    foreach ($temp_array as $dropbox_search_text) {
                        if (is_array($dropbox_search_text)) continue;
                        if (($dropbox_search_text != "display_text") && ($dropbox_search_text != "Array ")) {
                            $wherebit = $wherebit . " $whereor $dropbox_search_text like '%$search_item%' ";
                        }
                        $whereor = "or";
                    }
                    $where = $where . " $sep ( $wherebit ) ";
                    $dropbox_search_text = "";
                }
                $sep = "or";
            }
        }

        $where2 = "";
        if ($search_text2 != "") {
            $search_text2 = addslashes($search_text2);
            $search_chunks = explode(" ", $search_text2);
            //echo "<b>not null ($search_text)</b><br>";
            //print_r($search_chunks);
            $sep = "";
            while ($search_item = array_shift($search_chunks)) {
                //echo "<hr>$search_item/$search_text<hr>";
                if ($sep != "") {
                    if ($search_item[0] == "+") {
                        $sep = "and";
                        $search_item = substr($search_item, 1);
                    }
                    if ($search_item[0] == "-") {
                        $sep = "and not";
                        $search_item = substr($search_item, 1);
                    }
                }


                if ($dropbox_search_text2 != "") {
                    $where2 = $where2 . " $sep ( $dropbox_search_text2 like '%$search_item%' ) ";
                } else {
                    $temp_array = $search_array;
                    $wherebit = "";
                    $whereor = "";
                    $temp_array = "";
                    $dropbox_search_text2 = "";
                    $temp_array = $search_array;
                    //print_r($temp_array);
                    //$temp_array=array('a','b','c','d');
                    foreach ($temp_array as $dropbox_search_text2) {
                        if (is_array($dropbox_search_text2)) continue;
                        if (($dropbox_search_text2 != "display_text") && ($dropbox_search_text2 != "Array ")) {
                            $wherebit = $wherebit . " $whereor $dropbox_search_text2 like '%$search_item%' ";
                        }
                        $whereor = "or";
                    }
                    $where2 = $where2 . " $sep ( $wherebit ) ";
                    $dropbox_search_text2 = "";
                }
                $sep = "or";
            }
        }

        if (($where != "") && ($where2 != "")) $where = "(" . $where . ") and (" . $where2 . ")";
        else $where = $where . $where2;

        if ($where != "") return (" and (" . $where . ")");
        else return ("");


    }

    function search_div()
    {
        global $pagename, $search_text, $search_text2, $search_array, $fd_color_3, $search_form_big;
        search_session_maint();
        if ($search_form_big != "Y") {
            $view_status_advanced = "none";
            $view_status_simple = "block";
        } else {
            $view_status_advanced = "block";
            $view_status_simple = "none";
        }


        echo "<div id=\"search_div_simple\" style=\"width: 180px; display: $view_status_simple; background: $fd_color_3; text-align: left; position: absolute; border: 2px solid black;\" class=\"noprint\"><font class=\"noprint\" size=-1>
	<div class=\"noprint\" style=\"float: right;\"><i><a class=\"noprint\" href=$pagename?mode=$mode&search_form_big=Y><font class=\"noprint\" color=blue>advanced</font></a></i></div>
	<form class=\"noprint\" name=search method=post action=$pagename><input type=hidden name=search_session_reset value=Y><i>Search&nbsp;for:<br></i><input size=12 type=text name=search_text value=\"$search_text\">&nbsp;<input type=submit value='Go'></form></font></div>


	<div id=\"search_div_advanced\" class=\"noprint\" style=\"width: 240px; display: $view_status_advanced; background: $fd_color_3; text-align: left; position: absolute; border: 2px solid black;\"><font class=\"noprint\" size=-1><div class=\"noprint\" style=\"float: right;\"><i><a class=\"noprint\" href=$pagename?mode=$mode&search_session_reset=Y><font class=\"noprint\" color=blue>reset</font></a></i></div><form name=search method=post action=$pagename><input type=hidden name=search_session_reset value=Y><i>Search&nbsp;for:<br></i><input size=7 type=text name=search_text value=\"$search_text\">&nbsp;in&nbsp;";
        dropbox_search($search_array);
        echo "&<br><input size=7 type=text name=search_text2 value=\"$search_text2\">&nbsp;in&nbsp;";
        dropbox_search2($search_array);
        echo "<input type=submit value='Go'></form></font></div>";


    }

    function search_url_append()
    {
        global $search_text, $search_text2, $search_dropbox_text, $search_dropbox_text2;
        return ("&search_text=" . urlencode($search_text) . "&search_text2=" . urlencode($search_text2) . "&dropbox_search_text=" . urlencode($dropbox_search_text));

    }

    function search_form_insert()
    {
        global $search_text, $search_text2, $dropbox_search_text, $dropbox_search_text2;
        return ("
	<input type=hidden name=search_text value=\"" . urlencode($search_text) . "\">
	<input type=hidden name=search_text2 value=\"" . urlencode($search_text2) . "\">
	<input type=hidden name=search_dropbox_text value=\"" . urlencode($search_dropbox_text) . "\">
	<input type=hidden name=search_dropbox_text2 value=\"" . urlencode($search_search_text2) . "\">");
        //return ("&search_text=" . urlencode($search_text) . "&search_text2=" . urlencode($search_text2) . "&dropbox_search_text=" . urlencode($dropbox_search_text));

    }

    function search_session_maint()
    {
        global $search_session_reset, $search_persistent, $search_last_text, $search_last_text2,
               $search_last_dropbox_text, $search_last_dropbox_text2, $search_last_application_name, $search_last_global_jobinfo_id,
               $application_name, $global_jobinfo_id, $search_text, $search_text2, $search_dropbox_text, $search_dropbox_text2, $search_form_big;
        $wrong_screen = "N";
        if ($search_form_big != "Y") $search_form_big = "N";


        //if ($application_name!=$search_last_application_name) $search_session_reset='Y';
        //if ($global_jobinfo_id!=$search_last_global_jobinfo_id) $search_session_reset='Y';
        if ($application_name != $search_last_application_name) $wrong_screen = 'Y';
        if ($global_jobinfo_id != $search_last_global_jobinfo_id) $wrong_screen = 'Y';


        if ($search_session_reset == 'Y') {
            session_register('search_last_text');
            session_register('search_last_text2');
            session_register('search_last_dropbox_text');
            session_register('search_last_dropbox_text2');
            session_register('search_last_application_name');
            session_register('search_last_global_jobinfo_id');
            $search_last_text = $search_text;
            $search_last_text2 = $search_text2;
            $search_last_dropbox_text = $search_dropbox_text;
            $search_last_dropbox_text2 = $search_dropbox_text2;
            $search_last_application_name = $application_name;
            $search_last_global_jobinfo_id = $global_jobinfo_id;
        }

        if ($wrong_screen == 'N') {
            $search_last_application_name = $application_name;
            $search_last_global_jobinfo_id = $global_jobinfo_id;
            $search_text = $search_last_text;
            $search_text2 = $search_last_text2;
            $search_dropbox_text = $search_last_dropbox_text;
            $search_dropbox_text2 = $search_last_dropbox_text2;
        }

        $search_hybrid_blob = $search_text . $search_text2 . $search_dropbox_text . $search_dropbox_text2;
        if ($search_hybrid_blob != "") $search_form_big = "Y";
    }

    function query_search_add_text()
    {
        global $search_array, $search_text, $search_text2, $search_dropbox_text, $search_dropbox_text2,
               $search_persistent, $search_last_application;
        search_session_maint();
        /*
	if ($search_reset=='Y') {
		$search_persistent="";
		$search_last_application="";
		$search_last_dropbox_search_text="";
		$search_last_dropbox2_search_text="";
		$search_last_text="";
		$search_last_text2="";
		}
	*/


        //$dropbox_search_text2=$dropbox_search_text;
        $where = "1 = 1";
        if ($search_text != "") {
            $search_text = addslashes($search_text);
            $search_chunks = explode(" ", $search_text);
            //echo "<b>not null ($search_text)</b><br>";
            //print_r($search_chunks);
            $sep = "and";
            while ($search_item = array_shift($search_chunks)) {
                //echo "<hr>$search_item/$search_text<hr>";
                //if ($sep!="") {
                if ($search_item[0] == "+") {
                    $sep = "and";
                    $search_item = substr($search_item, 1);
                }
                if ($search_item[0] == "-") {
                    $sep = "and not";
                    $search_item = substr($search_item, 1);
                }
                //	}


                if ($search_dropbox_text != "") {
                    $where = $where . " $sep ( $search_dropbox_text like '%$search_item%' ) ";
                } else {
                    $temp_array = $search_array;
                    $wherebit = "";
                    $whereor = "";
                    $temp_array = "";
                    $search_dropbox_text = "";
                    $temp_array = $search_array;
                    //print_r($temp_array);
                    //$temp_array=array('a','b','c','d');
                    foreach ($temp_array as $search_dropbox_text) {
                        if (is_array($search_dropbox_text)) continue;
                        if (($search_dropbox_text != "display_text") && ($search_dropbox_text != "Array ")) {
                            $wherebit = $wherebit . " $whereor $search_dropbox_text like '%$search_item%' ";
                        }
                        $whereor = "or";
                    }
                    $where = $where . " $sep ( $wherebit ) ";
                    $search_dropbox_text = "";
                }
                $sep = "or";
            }
        }

        $where2 = "1 = 1";
        if ($search_text2 != "") {
            $search_text2 = addslashes($search_text2);
            $search_chunks = explode(" ", $search_text2);
            //echo "<b>not null ($search_text)</b><br>";
            //print_r($search_chunks);
            $sep = "and";
            while ($search_item = array_shift($search_chunks)) {
                //echo "<hr>$search_item/$search_text<hr>";
                //if ($sep!="") {
                if ($search_item[0] == "+") {
                    $sep = "and";
                    $search_item = substr($search_item, 1);
                }
                if ($search_item[0] == "-") {
                    $sep = "and not";
                    $search_item = substr($search_item, 1);
                }
                //	}


                if ($search_dropbox_text2 != "") {
                    $where2 = $where2 . " $sep ( $search_dropbox_text2 like '%$search_item%' ) ";
                } else {
                    $temp_array = $search_array;
                    $wherebit = "";
                    $whereor = "";
                    $temp_array = "";
                    $search_dropbox_text2 = "";
                    $temp_array = $search_array;
                    //print_r($temp_array);
                    //$temp_array=array('a','b','c','d');
                    foreach ($temp_array as $search_dropbox_text2) {
                        if (is_array($search_dropbox_text2)) continue;
                        if (($search_dropbox_text2 != "display_text") && ($search_dropbox_text2 != "Array ")) {
                            $wherebit = $wherebit . " $whereor $search_dropbox_text2 like '%$search_item%' ";
                        }
                        $whereor = "or";
                    }
                    $where2 = $where2 . " $sep ( $wherebit ) ";
                    $search_dropbox_text2 = "";
                }
                $sep = "or";
            }
        }

        if (($where != "") && ($where2 != "")) $where = "(" . $where . ") and (" . $where2 . ")";
        else $where = $where . $where2;

        if ($where != "") return (" and (" . $where . ")");
        else return ("");


    }


#######################################################################
#
#	Sets up a meta refresh page to forward to the "mode"
#
#######################################################################

    function forward($mode = "main", $seconds = "0")
    {
        global $SCRIPT_NAME;
        if ($GLOBALS['no_forward_please'] == "1") return 0;
        echo "<META HTTP-EQUIV=\"REFRESH\" CONTENT=\"$seconds; URL=$SCRIPT_NAME?mode=$mode\">
		<body bgcolor=white>
		Operation successful. If this page doesn't go away, click <a href=$SCRIPT_NAME?mode=$mode>
		<font color=blue>here</font></a> to continue.";
    }

    function ajax_load()
    {
        $arg_list = func_get_args();
        $arg_count = func_num_args();
        $count = 0;
        $jscript = "umc_javascript_loadlist=new Array($arg_count);\r\n";
        while ($arg = array_shift($arg_list)) {
            $jscript = $jscript . "umc_javascript_loadlist[$count]=\"$arg\";\r\n";
            $count++;
        }
        echo "<script>\r\n$jscript\r\n</script>\r\n";
        echo "<script language=\"JavaScript\" type=\"text/javascript\" src=\"/javascript/umc_ajax.js\">\r\n$jscript\r\n</script>";
        return 0;
    }

    function datebox($value, $javapath, $cal_image = "", $onchange = "", $show_year = "0")
    {
        datebox2($value, $javapath, $onchange, $show_year = "0");
    }

    function ms_tabledump($query, $link = "")
    {
        global $fd_color_4;
        if ($link == "") {
            $results = mssql_query("$query");
        } else {
            $results = mssql_query("$query", $link);
        }
        if ($fd_color_4 == "") $fd_color_4 = "#dddddd";
        /*if(!($results)) {
	echo "Query failed (ms_tabledump)!<hr>Failed query<br>" . $query;
	exit;
	}
*/
        $width = mssql_num_fields($results);

        echo "<table border=1><tr>";
        while ($column < $width) {
            $fieldtmp = mssql_fetch_field($results, $column);
            $field = $fieldtmp->name;
            echo "<td bgcolor=#dddddd><b><font color=#000000>$field</font></b></td>";
            $column++;
        }
        echo "</tr>";
        $column = 0;

        while ($row = (@mssql_fetch_row($results))) {
            $column = 0;
            echo "<tr>";
            while ($column < $width) {
                if ($row[$column] == "") $row[$column] = '&nbsp;';
                echo "<td>$row[$column]</td>";
                $column++;
            }
            echo "</tr>";
        }
        echo "</table>";
    }

    function job_list_output_format_1($query)
    {
        $res = @mysql_query($query);
        $count = @mysql_num_rows($res);
        if ($count <= 0) {
            echo "<b>No entries found...</b>";
            return $count;
        }
        echo "<table border=1 cellspacing=0 cellpadding=3>
	<tr bgcolor=#eeeeee><td>
		<b>Contract Number</b>
	</td><td>
		<b>Description</b>
	</td><td>
		<b>PM</b>
	</td><td>
		<b>Contract Admin</b>
	</td><td>
		<b>Bid Info</b>
	</td></tr>";

        while ($row = @mysql_fetch_object($res)) {
            $jobinfo = getoneb("select * from bidlist where jobinfo_id = '$row->jobinfo_id' limit 1");
            if ($jobinfo) {
                $bidinfo_link = "<b><a href=$pagename?mode=jobinfo_find_proposal&jobinfo_id=$row->jobinfo_id><font color=blue>*</font></a></b>";
            } else {
                $bidinfo_link = "&nbsp;";
            }
            echo "
		<tr><td>
			<a href=$pagename?mode=jobinfo_edit&jobinfo_id=$row->jobinfo_id><font color=blue>$row->contract_num</font></a>
		</td><td>
			$row->job_name
		</td><td>";
            contact_view("", $row->project_manager_id);
            echo "</td><td>";
            contact_view("", $row->contract_admin);
            echo "</td><td align=center>
			$bidinfo_link
		</td></tr>";
        }


    }

    function clicktable($query, $tabcolor = "#444444", $runfunction = "0")
    {
        global $findsubmitform;

        $results = mysql_query("$query");
        if (!($results)) {
            include("updateerror.phtml");
            exit;
        }

        $width = mysql_num_fields($results);

        $idx_field = mysql_field_name($results, 0);
        $formserno = rand();

        echo "
<input type=hidden name=$idx_field value=\"\">
<input type=hidden name=findsubmitform value=$formserno>
<table border=0 cellspacing=0 cellpadding=0><tr>";
        $column = 1;
        while ($column < $width) {
            $field = mysql_field_name($results, $column);
            echo "<td bgcolor=$tabcolor valign=top><table border=0 cellspacing=0 cellpadding=0><tr><td><img border=0 src=leftgreytab.gif width=8 height=20 border=0 alt=\"\"></td><td width=100%><font color=#ffffff>$field</font></td><td><img border=0 src=rightgreytab.gif width=8 height=20 border=0 alt=\"\"></td></tr></table></td>";
            $column++;
        }
        echo "</tr>";
        $column = 1;

        while ($row = (@mysql_fetch_row($results))) {
            $column = 1;
            echo "<tr>";
            while ($column < $width) {
                echo "\r<td>\r";
                echo "\t<a href=javascript:findsubmitform('$formserno','$idx_field','$row[0]','$runfunction');>$row[$column]</a>\r";
                echo "</td>\r";
                $column++;
            }
            echo "</tr><tr><td colspan=$width><hr width=100%></td></tr>";
        }

        echo "</table>";

        if ($findsubmitform != 1) {
            $findsubmitform = 1;
            echo "
<script>

if (findsf != '1') {
var findsf='1';
function findsubmitform (formid,fieldname,value,submit) {
	if (submit == '1') submit = 'document.forms[count].submit();';

    var count=0;
    while (count < 100) {
        checktype=(typeof document.forms[count]);
        if (checktype == 'object') {
            targetitem=(typeof document.forms[count].findsubmitform);
            if (targetitem == 'object') {
                if (document.forms[count].findsubmitform.value == formid) {
                	eval ('document.forms[count].' + fieldname + '.value=value;');
                	if (submit!='0') { eval (submit); }
        	        }
        	    }
        	} else {
            break;
            }
        count=count + 1;
        }
    }
}
</script>

";
        }
    }


//////////////////////////////////////////////////////////
//	This function isn't done, but I'll have to finish it
//	another day.. I've got more important things to do..
//////////////////////////////////////////////////////////

    function timeselect($value, $javapath, $onchange = "")
    {
        global $_time_javascript_loaded, $global_user_id;
        srand();
        $input_id = rand();
        $clock_image = "/images/show_clock.gif";

        if ($_clock_javascript_loaded != "true") {
            echo "<script src='/javascript/time.js'></script>";
            $_clock_javascript_loaded = "true";
        }
    }


    function datebox2($value, $javapath, $onchange = "", $show_year = "0", $special_position = "")
    {
        global $_calendar_javascript_loaded, $global_user_id;
        $value = invali_date($value, '/');
        srand();
        $input_id = rand();
        $cal_image = "/images/show-calendar2.gif";
        if ($_calendar_javascript_loaded != "true") {
            echo "<script src='/javascript/cal2.js'></script>";
            $_calendar_javascript_loaded = "true";
        }

        $formname = ereg_replace("\..*$", "", $javapath);
        $varname = ereg_replace("^.*\.", "", $javapath);

        //if ($global_user_id<3) {
        echo "
	<a href=javascript:void(0); onclick=\"BrowseDate('$input_id');\" title='Choose Date'>
	<img src='$cal_image' id=\"${input_id}_img\" border=0 style=\"display: inline;\"></a>
	<input type=text id='$input_id' title='mm-dd-YYYY' name='$varname' value='$value' size=10 onchange=\"$onchange\" style=\"display: inline; vertical-align: top;\">
	<script>document.getElementById('$input_id').special_position='$special_position';</script>";

        //} else {
        //	echo "
        //	<input type=text id='$input_id' title='mm-dd-YYYY' name='$varname' value='$value' size=10 onchange=\"$onchange\" style=\"display: inline; vertical-align: top;\">
        //	<a href=javascript:void(0); onclick=\"BrowseDate('$input_id');\" title='Choose Date'>
        //	<img src='$cal_image' id=\"${input_id}_img\" border=0 style=\"display: inline;\"></a>";
        //}
        //if ($show_year!="0") { echo "
        //	<a href=\"javascript:BrowseDate('$input_id');\" title=\"Choose Date\">
        //	<img src='$yearly_image' border=0 style=\"display: inline;\"></a></td>";
        //	}

    }

#######################################################################
#
#	Function to dump results $query as a Comma Seperated File
#
#######################################################################

    function csvdump($query, $header = "1")
    {
        $linefeedchar = chr(10);
        $returnchar = chr(13);
        $results = mysql_query("$query");
        if (!($results)) {
            include("updateerror.phtml");
            exit;
        }
        $width = mysql_num_fields($results);

        if ($header == "1") {
            $column = 1;
            $field = mysql_field_name($results, 0);
            echo ereg_replace(",", ";", ereg_replace($returnchar, "", ereg_replace($linefeedchar, "", $field)));
            while ($column < $width) {
                $field = mysql_field_name($results, $column);
                echo "," . ereg_replace(",", ";", ereg_replace($returnchar, "", ereg_replace($linefeedchar, "", $field)));
                $column++;
            }
            echo "
";
        }

        while ($row = (@mysql_fetch_row($results))) {
            $column = 1;
            if ($row[0] == "") $row[0] = '""';
            echo ereg_replace(",", ";", ereg_replace($returnchar, "", ereg_replace($linefeedchar, "", $row[0])));
            while ($column < $width) {
                if ($row[$column] == "") $row[$column] = '""';
                echo "," . ereg_replace(",", ";", ereg_replace($returnchar, "", ereg_replace($linefeedchar, "", $row[$column])));
                $column++;
            }
            echo "
";
        }
    }

    function fd_short_doc_list($doc_id, $show_type = "", $list_id = "")
    {
        global $devel;
        if ($doc_id == "") return FALSE;
        $doc_id = addslashes($doc_id);
        $list_id = addslashes($list_id);
        if ($show_type != "") {
            $show_type = addslashes($show_type);
            $query = "select * from documents_links right join documents on (documents_links.docb = documents.doc_id) where doca = '$doc_id' and doc_type = '$show_type' order by doc_type, section, app_reference + 0 ASC";
        } else {
            $query = "select * from documents_links right join documents on (documents_links.docb = documents.doc_id) where doca = '$doc_id' order by doc_type, section, app_reference + 0 ASC";
        }

        $res = @mysql_query($query);
        $count = @mysql_num_rows($res);
        if ($count < 1) return ("<font color=blue style=\"cursor: hand; cursor: pointer;\" onclick=\"open('/global/front_desk$devel/index.html?mode=fd_doclist_popup&list_id=$list_id&doc_id=$doc_id&show_type=$show_type&restrict=$restrict','fd_docs_win','width=600,height=600,scrollbars=yes');\" title=\"$row->description\" style=\"text-decoration: none;\">Link Doc(s)</font>");
        $return_text = "<table border=0 cellspacing=0 cellpadding=1>";
        while ($row = @mysql_fetch_object($res)) {
            if ($row->doc_type == "rfi") $row->doc_type = 'umcrfi';
            $row->description = htmlspecialchars($row->description);
            $return_text = $return_text . "<tr><td><font color=blue style=\"cursor: hand; cursor: pointer;\" onclick=\"open('/global/front_desk$devel/index.html?mode=fd_doclist_popup&list_id=$list_id&doc_id=$doc_id&show_type=$show_type&restrict=$restrict','fd_docs_win','width=600,height=600,scrollbars=yes');\" title=\"$row->description\" style=\"text-decoration: none;\">$row->doc_type $row->app_reference</a></td><td>&nbsp;&nbsp;<i><a href=\"/global/front_desk$devel/?mode=open_doc&doc_id=$row->doc_id\" target=\"show_doc_$row->doc_id\" style=\"text-decoration: none;\"><font style=\"font-weight: bold; background: #bb0000; color: white; border: 1px solid black; cursor: hand; cursor: pointer;\">&lt;&#8209;jump&nbsp;to&#8209;</font></a></i></td></tr>";
        }
        $return_text = $return_text . "</table>";
        return ($return_text);
    }

    function fd_text_doc_list($doc_id, $show_type = "", $list_id = "")
    {
        global $devel;
        if ($doc_id == "") return FALSE;
        $doc_id = addslashes($doc_id);
        $list_id = addslashes($list_id);
        if ($show_type != "") {
            $show_type = addslashes($show_type);
            $query = "select * from documents_links right join documents on (documents_links.docb = documents.doc_id) where doca = '$doc_id' and doc_type = '$show_type' order by doc_type, section, app_reference + 0 ASC";
        } else {
            $query = "select * from documents_links right join documents on (documents_links.docb = documents.doc_id) where doca = '$doc_id' order by doc_type, section, app_reference + 0 ASC";
        }
        $res = @mysql_query($query);
        $count = @mysql_num_rows($res);
        if ($count < 1) return ("&nbsp;");
        $return_text = "<table border=0 cellspacing=0 cellpadding=1>";
        while ($row = @mysql_fetch_object($res)) {
            if ($row->doc_type == "rfi") $row->doc_type = 'umcrfi';
            $row->description = ereg_replace(" ", "&nbsp;", htmlspecialchars($row->description));
            $return_text = $return_text . "<tr><td>$row->doc_type&nbsp;$row->app_reference</td></tr>";
        }
        $return_text = $return_text . "</table>";
        return ($return_text);
    }


    function fd_document_box_show_doc_line($doca, $docb, $checked = 3, $info = "", $update_display = "N", $only_show_type = "")
    {
        global $fd_color_4, $fd_color_1, $pagename;
        if ($info == "") {
            $info = getoneb("select * from documents where doc_id = '$docb'");
        }
        if ($checked == 3) {
            $checked = getoneb("select * from documents_links where doca = '$doca' and docb = '$docb'");
        }

        $info->description = ereg_replace(" ", "&nbsp;", $info->description);

        $onload_inclusion = "";
        if ($update_display != 'N') {
            $onload_inclusion = "onload=update_selectedlist('$doca')";
        }

        if ($checked == 1) {
            echo "
		<div $onload_inclusion onclick=\"fd_doc_make_selection($doca,$docb,'$info->app_reference','$info->doc_type');\" title=\"middle click to edit\" id=\"edit_doc_$info->doc_id\" style=\"height: 14px; border: 2px solid blue; overflow: visible; float: left; background-color: $fd_color_4; min-width: 60px;\">$info->app_reference</div>
		<div $onload_inclusion onclick=\"fd_doc_make_selection($doca,$docb,'$info->app_reference','$info->doc_type');\" style=\"height: 18px; overflow: hidden; background-color: blue; color: white;\">$info->description</div>
		";
        } else {
            echo "
		<div $onload_inclusion onclick=\"fd_doc_make_selection($doca,$docb,'$info->app_reference','$info->doc_type');\" title=\"middle click to edit\" id=\"edit_doc_$info->doc_id\" style=\"z-index: 1;  height: 16px; border: 1px solid $fd_color_1; overflow: visible; float: left; min-width: 60px; background-color: $fd_color_4;\">$info->app_reference</div>
		<div $onload_inclusion onclick=\"fd_doc_make_selection($doca,$docb,'$info->app_reference','$info->doc_type');\" style=\"z-index: 6; height: 18px; overflow: hidden; background-color: $fd_color_1;\">$info->description</div>
		";
        }
    }

    function fd_document_box_scripts()
    {
        global $fd_documents_box_scripts_loaded, $devel;
        if ($fd_documents_box_scripts_loaded != TRUE) {
            $fd_documents_box_scripts_loaded = TRUE;
            echo "
		<script>
		function if_middle_edit(event) {
			var delta = 0;
			if (!event) event = window.event;
			middle_click=0;
			if (event.which) if (event.button==1) middle_click=1;
				else if (event.button==4) middle_click=1;
			if (!(middle_click)) return 1;

			if (event.target) targ = event.target;
			 	else if (event.srcElement) targ = event.srcElement;

			if (targ.id!='') {
				doc_id=document.getElementById('fd_document_current_doc_id').value;
				doc_edit_id=targ.id.replace('edit_doc_','');
				doc_box_edit_document(doc_id,doc_edit_id);
				}
			return 1;
			}

		function wheel(event) {
			var delta = 0;
			if (!event) event = window.event;
			if (event.wheelDelta) {
				delta = event.wheelDelta/120;
				if (window.opera) delta = -delta;
			} else if (event.detail) {
				delta = -event.detail/3;
			}
		if (delta) handle(delta);
		if (event.preventDefault) event.preventDefault();
		event.returnValue = false;
		}

		/* Initialization code. */
		if (window.addEventListener)
		window.addEventListener('mousedown', if_middle_edit, false);
		//window.addEventListe5B5B5Bner('DOMMouseScroll', wheel, false);
		//window.onmousewheel = document.onmousewheel = wheel;



		var doc_box_js_loaded=new Array();

		function update_selectedlist(doc_id) {
			limit_to_type=document.getElementById('fd_document_box_opt_limit_to_type').value;
			ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_current_selectlist&doc_id=' + doc_id + '&limit_to_type=' + limit_to_type,'current_selected_files');
			}

		function fd_doc_box_load_filtered(doc_id,value,doc_type) {
			limit_to_type=document.getElementById('fd_document_box_opt_limit_to_type').value;
			ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_box_main&display_associator=1&doc_id=' + doc_id + '&doc_box_type_selection=' + doc_type + '&doc_box_filter_text=' + value + '&limit_to_type=' + limit_to_type,'fd_document_box');
			}

		function doc_box_test_js_load(doc_type) {
			if (doc_box_js_loaded[doc_type]!='loaded') {
				doc_box_js_loaded[doc_type]='loaded';
				ajaxManager('load_js','/global$devel/front_desk$devel/?mode=fd_document_box_load_app_js&doc_type=' + doc_type);
				}
			}

		function fd_document_box_set_search_value(value,doc_type) {
			document.getElementById('doc_box_type_selection').value=doc_type;
			document.getElementById('doc_box_search_text').value=value;
			}

		function doc_box_add_document(doc_type,doc_id) {
			if (doc_type=='') doc_type=document.getElementById('doc_box_type_selection').value;
			one_selection_only=document.getElementById('fd_document_box_opt_one_selection_only').value;
			if (doc_type!='') {
				load_text=encodeURI(document.getElementById('doc_box_search_text').value);
				ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_add&doc_id=' + doc_id + '&doc_type=' + doc_type + '&load_text=' + load_text + '&one_selection_only=' + one_selection_only,'fd_document_box');
				} else {
				alert('You must pick a document type first');
				}
			}

		function doc_box_edit_document(doc_id,doc_edit_id) {
				ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_add&doc_id=' + doc_id + '&doc_edit_id=' + doc_edit_id,'fd_document_box');
				}
		function show_doc_dialog(doc_id,doc_type) {
			docboxobj=document.getElementById('fd_document_box');
			limit_to_type=document.getElementById('fd_document_box_opt_limit_to_type').value;
			if (!(doc_type)) doc_type='';
			ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_box_main&display_associator=1&doc_id=' + doc_id + '&doc_box_type_selection=' + doc_type + '&limit_to_type=' + limit_to_type,'fd_document_box');
			}
		function hide_doc_dialog(doc_id) {
			limit_to_type=document.getElementById('fd_document_box_opt_limit_to_type').value;
			ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_box_main&display_associator=0&doc_id=' + doc_id + '&limit_to_type=' + limit_to_type,'fd_document_box');
			}
		function update_doc_filter(doc_id) {
			docboxobj=document.getElementById('fd_document_box');
			limit_to_type=document.getElementById('fd_document_box_opt_limit_to_type').value;
			filter_text=encodeURI(doc_get_filter_text());
			filter_cat=encodeURI(doc_get_filter_category());
			ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_box_main&display_associator=1&doc_box_type_selection=' + filter_cat + '&doc_box_filter_text=' + filter_text + '&doc_id=' + doc_id + '&limit_to_type=' + limit_to_type,'fd_document_box');
			}
		function doc_get_filter_text() {
			obj=document.getElementById('doc_box_search_text');
			return(obj.value);
			}
		function doc_get_filter_category() {
			obj=document.getElementById('doc_box_type_selection');
			return(obj.value);
			}
		function fd_doc_remove_selection(main_doc_id,doc_id) {
			if (confirm('Disconnect this document?')) {
				docboxobj=document.getElementById('fd_document_box');
				limit_to_type=document.getElementById('fd_document_box_opt_limit_to_type').value;
				ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_remove_link&doca=' + main_doc_id +'&docb=' + doc_id + '&limit_to_type=' + limit_to_type,'fd_document_box');
				}
			}
		function fd_doc_make_selection(main_doc_id,doc_id,value,doc_type) {
			id='fddocsel' + doc_id;
			elobj=document.getElementById(id);
			//alert(elobj.innerHTML);
			is_match=elobj.innerHTML.match(\"solid blue\");
			//alert(is_match);

			docboxobj=document.getElementById('fd_document_box');
			one_selection_only=document.getElementById('fd_document_box_opt_one_selection_only').value;
			if (!(is_match)) fd_document_box_set_search_value(value,doc_type);
			else fd_document_box_set_search_value(\"\",doc_type);
			if (one_selection_only!='Y') {
				ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_toggle_link&doca=' + main_doc_id +'&docb=' + doc_id + '&one_selection_only=' + one_selection_only,id);
				} else {
				document.getElementById('record_status_indicator').innerHTML='<table style=\"width: 200px;\"><tr><td bgcolor=yellow><center><b>working</b></center></td></tr></table>';
				elobj.ajax_trigger_function='update_doc_filter(' + main_doc_id + ')';
				ajaxManager('load_page','/global$devel/front_desk$devel/?mode=fd_document_toggle_link&doca=' + main_doc_id +'&docb=' + doc_id + '&one_selection_only=' + one_selection_only,id);
				}
			}
		</script>
		";
        }
    }

    function fd_document_box($options = "")
    {
        global $devel, $global_jobinfo, $fd_color_1;
        ////////////////////////////////////////////////
        // Everything defaults to 'N' or '', but these
        // options can be set:
        // $options=array(
        //	'doc_id'=>'<doc_id>',
        //	'one_selection_only'=>'Y',
        //	'limit_to_type'=>'<document_type>',
        //	'div_max_height'=>'####', // in pixels
        //	'include_mini_linker'=>'Y' // not done yet
        //	'read_only'=>'Y' // can't change linked docs
        //	);
        ////////////////////////////////////////////////
        if ($options['doc_id'] != "") $doc_id = $options['doc_id'];
        else global $doc_id;
        if ($options['border_width'] != "") $border_width = $options['border_width'];
        else $border_width = 3;

        $div_max_height = "400";
        if ($options['div_max_height'] != "") $div_max_height = $options['div_max_height'];

        $div_min_height = "0";
        if ($options['div_min_height'] != "") $div_min_height = $options['div_min_height'];

        $width = "205px";
        if ($options['width'] != "") $width = $options['width'];


        $doc_info = getoneb("select * from documents where doc_id = '$doc_id'");
        $doc_id = $doc_info->doc_id;
        if (!($doc_info)) return FALSE;
        fd_document_box_scripts();

        if ($options['limit_to_type'] != "") $limit_to_type = $options['limit_to_type'];
        if ($options['one_selection_only'] != "") $one_selection_only = $options['one_selection_only'];

        if ($options['read_only'] == 'Y') {
            echo "<div id=\"fd_document_box\" style=\"border: ${border_width}px solid black; background: $fd_color_1; max-height: ${div_max_height}px; min-height: ${div_min_height}px; overflow: auto; width: $width;\"><font onload=\"alert('$limit_to_type');\"></font>";
            // GPH MARK - absolute path
            include("/data/web/pipeline/global$devel/front_desk$devel/fd_document_box_main_read_only.phtml");
            echo "</div>";
            echo "<input type=hidden id='fd_document_box_opt_limit_to_type' value='$limit_to_type'>";
            echo "<input type=hidden id='fd_document_box_opt_one_selection_only' value='$one_selection_only'>";
        } else {
            echo "<div id=\"fd_document_box\" style=\"border: ${border_width}px solid black; background: $fd_color_1; max-height: ${div_max_height}px; min-height: ${div_min_height}px; overflow: auto; width: $width;\"><font onload=\"alert('$limit_to_type');\"></font>";
            // GPH MARK - absolute path
            include("/data/web/pipeline/global$devel/front_desk$devel/fd_document_box_main.phtml");
            echo "</div>";
            echo "<input type=hidden id='fd_document_box_opt_limit_to_type' value='$limit_to_type'>";
            echo "<input type=hidden id='fd_document_box_opt_one_selection_only' value='$one_selection_only'>";
        }
        return 0;
    }

    function fd_doc_box($id = "", $show_type = "", $restrict = 0, $options = array())
    {
        global $application_name, $devel;
        ajax_load();
        $list_id = rand();
        $doc_id = fd_get_doc_id($id);
        if ($show_type != "") {
            $count = getoneb("select sum(1) as count from documents_links right join documents on (documents.doc_id = documents_links.doca) where doca = $doc_id and documents.doc_type = '$show_type'");
        } else {
            $count = getoneb("select sum(1) as count from documents_links where doca = '$doc_id'");
        }
        if ($count->count < 1) $count->count = "0";
        echo "<div id=\"$list_id\">";
        $mode = "short_doc_list";
        include("../front_desk$devel/short_doc_list.phtml");

        echo "</div>
	<script>
	document.getElementById('$list_id').ajax_trigger='never';
	//ajaxManager('load_page','/global/front_desk$devel/?mode=short_doc_list&doc_id=$doc_id&list_id=$list_id&show_type=$show_type','$list_id');
	</script>";
        return 0;
    }

    function fd_doc_box2($doc_id, $show_type = "", $restrict = 0, $options = array())
    {
        global $application_name, $devel;
        ajax_load();
        $list_id = rand();
        if ($doc_type != "") {
            $count = getoneb("select sum(1) as count from documents_links right join documents on (documents.doc_id = documents_links.doca) where doca = $doc_id and documents.doc_type = '$doc_type'");
        } else {
            $count = getoneb("select sum(1) as count from documents_links where doca = '$doc_id'");
        }
        if ($count->count < 1) $count->count = "0";
        echo "<div id=\"$list_id\"></div>
	<script>document.getElementById('$list_id').ajax_trigger='never';ajaxManager('load_page','/global/front_desk$devel/?mode=short_doc_list&doc_id=$doc_id&list_id=$list_id&show_type=$show_type','$list_id');</script>";
        return 0;
    }

    function fd_get_doc_id($app_record_id)
    {
        global $application_name;
        if ($app_record_id == 0) return FALSE;
        $app_record_id = addslashes($app_record_id);
        $doc_type = $application_name;
        if ($doc_type == "rfilog") $doc_type = 'rfi';
        if ($doc_type == "gcrfilog") $doc_type = 'gcrfi';
        if ($doc_type == "transmittals") $doc_type = 'transmittal';
        $doc_info = getoneb("select * from documents where doc_type = '$doc_type' and app_record_id = '$app_record_id'");
        if (!($doc_info)) return ($doc_info);
        else {
            if ($doc_info->doc_id > 0) return ($doc_info->doc_id);
            else return FALSE;
        }
    }

    function fd_manager($jobinfo_id, $contacts_id = "")
    {
        global $global_user_id;
        if ($contacts_id == "") $contacts_id = addslashes($global_user_id);
        $admin = getoneb("select * from front_desk_job_admins where jobinfo_id = '$jobinfo_id' and contacts_id = '$contacts_id' limit 1");
        
        if (!($admin)) {
            
            //if ($jobinfo->project_manager_id==$contacts_id) $admin=TRUE;
            if (pm_for_this_job($jobinfo_id))
            {
                return TRUE;
            }
            else
            {
                $jobinfo=getoneb("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
                
                if ($jobinfo->project_sponsor_id == $contacts_id && $contacts_id != "")
                    return TRUE;
            }
        }
        return $admin;
    }


    function fd_print_global_nav()
    {
        global $fd_color_1, $fd_color_2, $global_user, $pagename_up, $devel, $fd_color_4;
        echo "
	<table class=noprint width=100% border=0 style=\"border: 1px solid black; border-collapse: collapse;\" cellspacing=0 cellpadding=8>
	<tr class=noprint bgcolor=$fd_color_2>

	<td class=normnav id=\"recent_jobs_button\" onmouseover=\"show_recent_jobs();\" onmouseout=\"hide_recent_jobs();\" align=center style=\"border-right: 0px solid black; cursor: pointer; cursor: hand;\" width=180px>
		<b>Recent&nbsp;Jobs</b>
	</td>

	<td class=normnav id=\"my_jobs_button\" onmouseover=\"show_my_jobs();\" onmouseout=\"hide_my_jobs();\" align=center style=\"cursor: hand;\" width=180px>
		<table style=\"display: inline;\" border=0 cellspacing=0 cellpadding=0><tr><td>";
        if ($global_user->photo_file_group_id > 0) {
            $file_info = getoneb("select * from webfile_files where file_group_id = '$global_user->photo_file_group_id' limit 1");
            $imgurl = "/global/webfile/index.html/images/image$file_info->files_id.jpg?mode=webfile_preview&files_id=$file_info->files_id&height=20";
            echo "<img style=\"display: inline; position: relative;\" border=0 src=\"$imgurl\">";
        }
        echo "</td><td><b>&nbsp;My&nbsp;Jobs</b></td></tr></table>
	</td>

	<td	width=180px valign=top><form style=\"display: inline;\" name=job_jump title=\"Enter job # to go directly to it\" method=post action=\"/global/front_desk$devel/\"><b>Load&nbsp;job:&nbsp;<input type=hidden name=mode value=fd_job_jump><input type=text size=6 name=contract_num onchange=\"submit()\"></form></td>

	<td valign=top>
		<a href=\"javascript:open_job_search()\"><font color=blue><b>Search</b></font></a>
	</td>
	<td style=\"width: 24px; padding: 0px;\">
	";
        // GPH modified to use new AccessControl API
        // if ($global_user->contacts_id <= 2)
        if (AccessControl::isAdmin($global_user->contacts_id)) {
            echo "
			<div style=\"padding: 0px; width: 24px; position: absolute; top: 5px; background: $fd_color_4;\"><center><img border=0 src=\"/images/lightbulb_small.png\"></center></div>
			<div style=\"padding: 0px; width: 24px; position: absolute; top: 7px;\"><center>?</center></div>
			";
        }
        echo "
	<td width=180px align=center style=\"border-top: 0px; border-bottom: 0px; padding-top: 0px; padding-bottom: 0px;\">
		Logged&nbsp;in&nbsp;as&nbsp;$global_user->login_name.&nbsp;<a href=$pagename?kill_session_info=1><font color=blue>Log&nbsp;Off</font></a>
	</td>

	</tr>
	</table>
	";
    }

    /*
function warn_chrome_plugin() {
	global $global_browser,$GOOGLE_CHROME_FRAME_WARNED,$GOOGLE_CHROME_FRAME_NEEDED,$fd_color_1,$mode;
	if ($global_browser!="IE") return 0;
	if ($GOOGLE_CHROME_FRAME_NEEDED) {
		//if ($GOOGLE_CHROME_FRAME_WARNED!=1) {
		//	$GOOGLE_CHROME_FRAME_WARNED=1;
		//$GOOGLE_CHROME_FRAME_WARNED="1";
		//echo "<hr>$GOOGLE_CHROME_FRAME_WARNED<hr>";
		if ($GOOGLE_CHROME_FRAME_WARNED!="1") {
			session_register('GOOGLE_CHROME_FRAME_WARNED');
			$GOOGLE_CHROME_FRAME_WARNED="1";
			echo "
			<script>function alert_ok_browsers() { alert('Firefox, Safari, Opera, or Chrome'); } </script>
			<center><div style=\"width: 400px; background: $fd_color_1; border: 10px solid red;\">
			<font size=+2>To view this page correctly, you need either:<center><p>
				<a href=\"http://www.google.com/chromeframe/eula.html\"><font color=blue>The Chrome Frame Plugin for Internet Explorer</font></a>
				<p>OR<p>
				<a href=javascript:alert_ok_browsers()>A standards compatible web browser</a>.
				<p>
			</font><hr>
			Alternatively, you can <a href=$pagename?mode=$mode><font color=blue>proceed at your own risk</font></a>, but things <b>WILL</b> work incorrectly.

			</div><p>&nbsp;<p></center>
			";
			}
		}

	}
*/


    function fd_navs_header()
    {
        global $devel, $fd_color_4, $fd_color_1, $mode, $application_name, $global_browser;
        $selectedcolor = "white";
        $height_drop_factor = 4;
        $my_home_color = $fd_color_1;
        if ($application_name == "front_desk") {
            $my_home_color = $selectedcolor;
            if (($mode == "") || ($mode == "main")) $my_home_color = $fd_color_1;
        }
        ajax_load();
        //$watermark="url(/images/fd_watermark.jpg)";
        $watermark = "url(/images/fd_watermark.jpg)";

        if ($GLOBALS['HTTP_HOST'] == "pipeline-devel.umci.com") {
            $watermark = "url(/images/fd_watermark_devel.jpg)";
        }

        echo "
	<div class=noprint style=\"z-index: 2; position: absolute; width: 350px; display: none; background: white; border: 1px solid black;\" id=\"my_jobs_drop\" onmouseout=\"hide_my_jobs();\" onmouseover=\"show_my_jobs();\"><b>Loading List..</b><br>please wait</div>
	<div class=noprint style=\"z-index: 2; position: absolute; width: 350px; display: none; background: white; border: 1px solid black;\" id=\"recent_jobs_drop\" onmouseout=\"hide_recent_jobs();\" onmouseover=\"show_recent_jobs();\"><b>Loading List..</b><br>please wait</div>

	<table class=noprint id='fd_main_application_window' style=\"border-collapse: collapse; border: 1px solid black;\" cellspacing=0 width=100% cellpadding=0>
	<tr class=noprint bgcolor=\"$my_home_color\"><td id=\"fd_my_home_nav_button\" style=\"border-right: 1px solid black;height:70px;\" align=center><a href=\"/global/front_desk$devel?mode=my_home\"><font class=noprint color=blue><b>My&nbsp;Home</b></font></a></td><td colspan=2 style=\"border-bottom: 1px solid black;\" class=noprint id=fd_top_nav>&nbsp;</td></tr>
	<tr class=noprint ><td width=5% valign=top bgcolor=\"$fd_color_1\" rowspan=2 id=\"fd_left_nav\">&nbsp;</td><td width=30px rowspan=2>&nbsp;</td><td valign=top height=30px id=\"application_menu\"><div id=\"fd_doc_tools\" class=noprint style=\"float: left;\"></div><div id=\"fd_doc_queue\" style=\"position: absolute; right: 40px; z-index: 4;\"></div></td></td></tr><tr><td style=\"background: $watermark; background-repeat: repeat;\" valign=top align=left>
	<script>
	document.getElementById('fd_top_nav').ajax_trigger='never';
	document.getElementById('fd_left_nav').ajax_trigger='never';
	document.fd_menus_positioned_already=0;

	function fix_fd_height() {
		var x,y;
		if (self.innerHeight) {
			// Most Browsers
			x = self.innerWidth;
			y = self.innerHeight;
			document.getElementById('fd_main_application_window').style.height=(y - 5) + 'px';
		} else if (document.documentElement && document.documentElement.clientHeight) {
			// IE6
			x = document.documentElement.clientWidth;
			y = document.documentElement.clientHeight;
		}
	apptable=document.getElementById('sub_application_main_table');
	";

        if ($global_browser != "IE") {
            echo "
		if (apptable) {
			apptable.style.height=(y - 250) + 'px';
			}
			";
        }
        echo "

	}
	//////////////////////////////////
	// This is the current name for
	// the cookie function, but it
	// should be renamed to have batch
	// at the begining.. so it's duped
	// below with the new correct name
	//////////////////////////////////
	function add_doc_to_batch(doc_id,obj) {
		if (obj.style.background!='') obj.style.background='';
		else obj.style.background='black';
		ajaxManager('load_page','/global/front_desk$devel/?mode=fd_doc_queue_main&add_doc_id=' + doc_id,'fd_doc_queue');
		}
	function batch_add_doc(doc_id,obj) {
		if (obj.style.background!='') obj.style.background='';
		else obj.style.background='black';
		ajaxManager('load_page','/global/front_desk$devel/?mode=fd_doc_queue_main&add_doc_id=' + doc_id,'fd_doc_queue');
		}
	function batch_move_doc_up(doc_id) {
		ajaxManager('load_page','/global/front_desk$devel/?mode=fd_doc_queue_main&move_up_doc_id=' + doc_id,'fd_doc_queue');
		}
	function batch_move_doc_down(doc_id) {
		ajaxManager('load_page','/global/front_desk$devel/?mode=fd_doc_queue_main&move_down_doc_id=' + doc_id,'fd_doc_queue');
		}
	function open_job_search() {
		open('/global/front_desk$devel/?mode=job_search','job_search','width=400,height=600,resizable=1,scrollbars=1,top=0,left=300');
		}
	function show_document_attachments(doc_id) {
		open('/global/front_desk$devel/?mode=fd_doc_attachments_show&doc_id=' + doc_id,'doc_attachments' + doc_id,'width=800,height=700,scrollbars=1,resizable=1');
		}
	function show_document_history(doc_id) {
		open('/global/front_desk$devel/?mode=fd_doc_history_show&doc_id=' + doc_id,'doc_history' + doc_id,'width=500,height=500,scrollbars=1,resizable=1');
		}
	function show_my_jobs() {
		position_menus();
		document.getElementById('my_jobs_drop').style.display='block';
		}
	function hide_my_jobs() {
		document.getElementById('my_jobs_drop').style.display='none';
		}
	function show_recent_jobs() {
		position_menus();
		document.getElementById('recent_jobs_drop').style.display='block';
		}
	function hide_recent_jobs() {
		document.getElementById('recent_jobs_drop').style.display='none';
		}

function load_job(jid) {
	document.location=\"/global/front_desk$devel/?mode=main&global_jobinfo_id_set=\" + jid;
	}

function position_menus() {
	if (document.fd_menus_positioned_already==1) return 0;
	document.fd_menus_positioned_already=1;
	////////////////////////////////////////////
	// Set position for 'my_jobs'
	////////////////////////////////////////////
	var x=0;
	var y=0;
	myjobs_button=document.getElementById('my_jobs_button');
	while ( myjobs_button != null) {
		//alert(myjobs_button.style.height);
		x+= myjobs_button.offsetLeft;
		myjobs_button = myjobs_button.offsetParent;
		}
	myjobs_button=document.getElementById('my_jobs_button');
	y=myjobs_button.offsetHeight;
	while ( myjobs_button != null) {
		y+= myjobs_button.offsetTop;
		myjobs_button = myjobs_button.offsetParent;
		}
	myjobs_div=document.getElementById('my_jobs_drop');
	myjobs_div.style.left = x+\"px\";
	myjobs_div.style.top = y+\"px\";


	////////////////////////////////////////////
	// Set position for 'recent_jobs'
	////////////////////////////////////////////
	var x=0;
	var y=0;
	recentjobs_button=document.getElementById('recent_jobs_button');
	while ( recentjobs_button != null) {
		x+= recentjobs_button.offsetLeft;
		recentjobs_button = recentjobs_button.offsetParent;
		}
	recentjobs_button=document.getElementById('recent_jobs_button');
	y=recentjobs_button.offsetHeight;
	while ( recentjobs_button != null) {
		y+= recentjobs_button.offsetTop;
		recentjobs_button = recentjobs_button.offsetParent;
		}
	recentjobs_div=document.getElementById('recent_jobs_drop');
	recentjobs_div.style.left = x+\"px\";
	recentjobs_div.style.top = y+\"px\";
	}

	function findPos(obj) {
		var curleft = curtop = 0;
		if (obj.offsetParent) {
			curleft = obj.offsetLeft
			curtop = obj.offsetTop
			while (obj = obj.offsetParent) {
				curleft += obj.offsetLeft
				curtop += obj.offsetTop
				}
			}
		return [curleft,curtop];
		}

	var my_tasks_menu_status=0;
	function my_tasks_menu_hold() {
		my_tasks_menu_status = my_tasks_menu_status + 1;
		//my_tasks_menu_status = 1;
		}

	function my_tasks_menu_release() {
		my_tasks_menu_status = my_tasks_menu_status - 1;
		setTimeout(\"my_tasks_menu_close_test()\",100)
		}

	function my_tasks_menu_close_test() {
		if (my_tasks_menu_status < 1) {
			document.getElementById('my_pal_details_div').style.display='none';
			document.getElementById('my_task_items_div').style.display='none';
			document.getElementById('my_tasks').style.display='none';
			//rootwin.style.display='none';
			}
		}

	function toggle_my_tasks_div(jobinfo_id) {
		rootwin=document.getElementById('my_tasks');
		if (rootwin.style.display!='none') {
			document.getElementById('my_pal_details_div').style.display='none';
			document.getElementById('my_task_items_div').style.display='none';
			rootwin.style.display='none';
			return 0;
			}
		button=document.getElementById('my_tasks_nav_button');
		buttonpos=findPos(button);
		rootwin.ajax_trigger='never';
		button.ajax_trigger='never';

		rootwin.style.left=buttonpos[0] + button.offsetWidth + 'px';
		rootwin.style.top=buttonpos[1] + (button.offsetHeight / $height_drop_factor) + 'px';
		rootwin.innerHtml='';
		ajaxManager('load_page','/global/front_desk$devel/pal$devel/?mode=my_task_apps&jobinfo_id=' + jobinfo_id,'my_tasks');
		rootwin.style.display='block';
		}
	function toggle_my_task_items(jobinfo_id,doc_type,parentwin,mode) {
		abort=0;
		rootwin=document.getElementById('my_task_items_div');
		rootwin.ajax_trigger='never';
		if (mode=='on') {
			if (rootwin.style.display!='none') {
				if (document.getElementById('my_task_items_div').my_data!=doc_type) {
					toggle_my_task_items(jobinfo_id,doc_type,parentwin);
					} else {
					abort=1;
					}
				}
			}

		if (abort<1) {
		parentpos=findPos(parentwin);
		if (rootwin.style.display!='none') {
			document.getElementById('my_pal_details_div').style.display='none';
			rootwin.style.display='none';
			return 0;
			}
		rootwin.style.left=parentpos[0] + parentwin.offsetWidth + 'px';
		rootwin.style.top=parentpos[1] + (parentwin.offsetHeight / $height_drop_factor) + 'px';
		rootwin.innerHTML='';
		document.getElementById('my_task_items_div').my_data=doc_type;
		ajaxManager('load_page','/global/front_desk$devel/pal$devel/?mode=my_task_items&jobinfo_id=' + jobinfo_id + '&doc_type=' + doc_type,'my_task_items_div');
		rootwin.style.display='block';
		}
		}


	function toggle_my_pal_details(pal_id,parentwin,mode) {
		rootwin=document.getElementById('my_pal_details_div');
		rootwin.ajax_trigger='never';
		if (mode=='on') {
		abort=0;
			if (rootwin.style.display!='none') {
				if (document.getElementById('my_pal_details_div').my_data!=pal_id) {
					//toggle_my_task_items(jobinfo_id,doc_type,parentwin);
					toggle_my_pal_details(pal_id,parentwin);
					} else {
					abort=1;
					}
				}

			}
		if (abort<1) {
		parentpos=findPos(parentwin);
		if (rootwin.style.display!='none') {
			rootwin.style.display='none';
			return 0;
			}
		rootwin.style.left=parentpos[0] + parentwin.offsetWidth + 'px';
		rootwin.style.top=parentpos[1] + (parentwin.offsetHeight / $height_drop_factor) + 'px';
		rootwin.innerHTML='';
		document.getElementById('my_pal_details_div').my_data=pal_id;
		ajaxManager('load_page','/global/front_desk$devel/pal$devel/?mode=my_pal_details&pal_id=' + pal_id,'my_pal_details_div');
		rootwin.style.display='block';
		}
		}
	function toggle_project_team(jobinfo_id) {
		pbdiv=document.getElementById('phone_book');
		pbdiv.ajax_trigger='never';
		if (pbdiv.style.display=='none') {
			ajaxManager('load_page','/global/front_desk$devel/?mode=show_team&jobinfo_id=' + jobinfo_id,'phone_book');
			close_menu_divs();
			pbbdiv=document.getElementById('phone_book_button');
			parentpos=findPos(pbbdiv);
			pbdiv.style.top=parentpos[1] + pbbdiv.offsetHeight + 'px';
			pbdiv.style.display='block';
			} else {
			pbdiv.style.display='none';
			pbdiv.innerHTML='';
			}
		}
	function show_material_phases(jobinfo_id,sub_job,no_close) {
		if(typeof sub_job == 'undefined') {
			sub_job=\"\";
			}
		if(typeof no_close == 'undefined') {
			no_close=\"\";
			}
		outdiv=document.getElementById('material_phases');
		outdiv.ajax_trigger='never';
		if ((outdiv.style.display=='none')||(no_close!='')) {
			ajaxManager('load_page','/global/front_desk$devel/?mode=material_phases&jobinfo_id=' + jobinfo_id + '&sub_job=' + sub_job, 'material_phases');
			close_menu_divs();
			butdiv=document.getElementById('material_phases_button');
			parentpos=findPos(butdiv);
			outdiv.style.top=parentpos[1] + butdiv.offsetHeight + 'px';
			outdiv.style.left=parentpos[0] + 'px';
			outdiv.style.display='block';
			} else {
			outdiv.style.display='none';
			outdiv.innerHTML='';
			}
		}
	function show_labor_phases(jobinfo_id,sub_job,no_close) {
		if(typeof sub_job == 'undefined') {
			sub_job=\"\";
			}
		if(typeof no_close == 'undefined') {
			no_close=\"\";
			}
		outdiv=document.getElementById('labor_phases');
		outdiv.ajax_trigger='never';
		if ((outdiv.style.display=='none')||(no_close!='')) {
			ajaxManager('load_page','/global/front_desk$devel/?mode=labor_phases&jobinfo_id=' + jobinfo_id + '&sub_job=' + sub_job, 'labor_phases');
			close_menu_divs();
			butdiv=document.getElementById('labor_phases_button');
			parentpos=findPos(butdiv);
			outdiv.style.top=parentpos[1] + butdiv.offsetHeight + 'px';
			outdiv.style.left=parentpos[0] + 'px';
			outdiv.style.display='block';
			} else {
			outdiv.style.display='none';
			outdiv.innerHTML='';
			}
		}
	function close_menu_divs() {
		document.getElementById('sub_phases').style.display='none';
		document.getElementById('material_phases').style.display='none';
		document.getElementById('labor_phases').style.display='none';
		document.getElementById('phone_book').style.display='none';
		}
	function show_sub_phases(jobinfo_id) {
		outdiv=document.getElementById('sub_phases');
		outdiv.ajax_trigger='never';
		if (outdiv.style.display=='none') {
			ajaxManager('load_page','/global/front_desk$devel/?mode=sub_phases&jobinfo_id=' + jobinfo_id,'sub_phases');
			close_menu_divs();
			butdiv=document.getElementById('sub_phases_button');
			parentpos=findPos(butdiv);
			outdiv.style.top=parentpos[1] + butdiv.offsetHeight + 'px';
			outdiv.style.left=parentpos[0] + 'px';
			outdiv.style.display='block';
			} else {
			outdiv.style.display='none';
			outdiv.innerHTML='';
			}
		}
	document.getElementById('my_jobs_drop').ajax_trigger='never';
	document.getElementById('recent_jobs_drop').ajax_trigger='never';
	</script>
	";
        contact_view_javascript();
        fd_load_navs();
    }

    function fd_navs_footer()
    {
        global $devel, $doc_id, $docid, $fd_color_4, $fd_pager_print_entire_log, $footer_loads, $REQUEST_URI, $report_name, $global_user_id, $global_jobinfo_id, $application_name, $mode, $REMOTE_ADDR, $fd_attachments_already_shown, $hide_the_cookie_sheet;
        $footer_loads = $footer_loads + 1;
        echo "</td></tr></table>
	<script>
	fix_fd_height();
	setTimeout(\"ajaxManager('load_page','/global/front_desk$devel?mode=my_jobs_menu','my_jobs_drop')\",100);
	setTimeout(\"ajaxManager('load_page','/global/front_desk$devel?mode=recent_jobs_menu','recent_jobs_drop')\",200);



	queueinfo=document.getElementById('fd_doc_queue');
	queueinfo.ajax_trigger='never';";

        if ($hide_the_cookie_sheet != 'Y') echo "ajaxManager('load_page','/global/front_desk$devel?mode=fd_doc_queue_main&jobinfo_id=$global_jobinfo_id','fd_doc_queue');";


        if ($doc_id > 0) {
            echo "
		docinfo=document.getElementById('fd_doc_tools');
		docinfo.ajax_trigger='never';
		docinfo.style.background='$fd_color_4';
		docinfo.style.border='2px solid black';
		ajaxManager('load_page','/global/front_desk$devel?mode=fd_doc_tools_main&doc_id=$doc_id&fd_attachments_already_shown=$fd_attachments_already_shown','fd_doc_tools');";
            $fd_attachments_already_shown = TRUE;
        } else {
            //$page_type='page';
            if ($mode == "report_show") {
                $page_type = 'report';
                $name = $report_name;
            } else {
                $page_type = 'page';
                $name = $mode;
                if ($name == "") $name = "main";
            }
            $date_as_int = date('Ymd');
            @mysql_query("insert into front_desk_page_history set page_type = '$page_type', application = '$application_name', name = '$name', contacts_id = '$global_user_id', jobinfo_id = '$global_jobinfo_id', review_time = now(), date_as_int = '$date_as_int', additional_info = '$REQUEST_URI', ip_address = '$REMOTE_ADDR'");
        }
        if ($fd_pager_print_entire_log == "TRUE") {
            echo "print();";
        }

        echo "
	</script>";
    }

    function fd_attachments_in_body()
    {
        global $devel, $doc_id, $docid, $fd_color_4, $fd_attachments_already_shown;
        if ($doc_id > 0) {
            $attachments = getoneb("select sum(1) as total from documents_attachments where doc_id = '$doc_id'");
            if ($attachments->total == "") {
                $attachments->total = '0';
            }
            //<script>
            //function show_document_attachments(doc_id) {
            //	open('/global/front_desk$devel/?mode=fd_doc_attachments_show&doc_id=' + doc_id,'doc_attachments' + doc_id,'width=800,height=700,scrollbars=1,resizable=1');
            //	}
            //</script>
            echo "
		<a style=\"background: $color;\" href=javascript:show_document_attachments($doc_id)><font color=blue><i>$attachments->total&nbsp;Attachments/Support Documents</i></font></a>";
            $fd_attachments_already_shown = TRUE;
        }

    }

    function fd_load_navs()
    {
        global $application_name, $global_jobinfo_id, $devel, $mode;
        ajax_load();
        echo "<script>ajaxManager('load_page','/global/front_desk$devel?mode=top_nav&jobinfo_id=$global_jobinfo_id','fd_top_nav');ajaxManager('load_page','/global/front_desk$devel?mode=left_nav&current_app=$application_name&jobinfo_id=$global_jobinfo_id&actualmode=$mode','fd_left_nav');</script>";
    }

    function pal_javascripts()
    {
        global $devel, $fd_color_3, $fd_color_1, $fd_color_4, $_calendar_javascript_loaded;
        contact_popup_javascript();

        if ($_calendar_javascript_loaded != "true") {
            echo "<script src='/javascript/cal2.js'></script>";
            $_calendar_javascript_loaded = "true";
        }


        echo "
	<script>
	function ajfoo(o) {
		document.getElementById(o).ajax_trigger='never';
		}
	function pal_delete(pal_id) {
		if(confirm('Are you sure you want to delete this pal and all it\'s history?')) {
			pobj=document.getElementById('pal_item' + pal_id);
			pobj.style.display='none';
			clickobj=document.getElementById('pal_header_item' + pal_id);
			clickobj.onclick='';
			ajaxManager('load_page','/global/pal$devel/?mode=pal_delete&pal_id=' + pal_id,'pal_header_item' + pal_id);
			}
		}
	function pal_force_mark_complete(pal_id) {
		ajfoo('pal_' + pal_id + '_status');
		if (confirm('Force complete this pal item?'))
		ajaxManager('load_page','/global/pal$devel/?mode=pal_force_mark_complete&pal_id=' + pal_id,'pal_' + pal_id + '_status');
		}
	function pal_change_priority(pal_id) {
		ajfoo('pal_' + pal_id + '_priority');
		ajaxManager('load_page','/global/pal$devel/?mode=pal_change_priority&pal_id=' + pal_id,'pal_' + pal_id + '_priority');
		}
	function pal_updated_header(pal_id,open) {
		ajfoo('pal_header_item' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_updated_header&pal_id=' + pal_id + '&open=' + open,'pal_header_item' + pal_id);
		}
	function pal_add(doc_id,jobinfo_id) {
		ajfoo('pal_box');
		ajaxManager('load_page','/global/pal$devel/?mode=pal_create&doc_id=' + doc_id + '&jobinfo_id=' + jobinfo_id,'pal_box');
		}
	function pal_update_short_description(pal_id) {
		ajfoo('pal_' + pal_id + '_short_description_div');
		input_text=document.getElementById('pal_item' + pal_id + '_short_description').value;
		ajaxManager('load_page','/global/pal$devel/?mode=pal_edit_ajax&pal_id=' + pal_id + '&short_description_set=' + input_text,'pal_' + pal_id + '_short_description_div');
		setTimeout('pal_updated_header(' + pal_id + ')',100);
		}
	function pal_add_note(pal_id) {
		ajfoo('pal_notes_area' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_add_notes&pal_id=' + pal_id + '&pal_note_to_add=' + encodeURI(document.getElementById('pal_note_box' + pal_id).value),'pal_notes_area' + pal_id);
		}
	function pal_mso_toggle(pal_id,pass_id) {
		ajfoo('pal_data_area' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_add_assignments&pal_id=' + pal_id + '&pass_id_toggle=' + pass_id,'pal_data_area' + pal_id);
		}
	function pal_sign_off_orig(pal_id,pass_id) {
		ajfoo('pal_data_area' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_current&pal_id=' + pal_id + '&pass_sign_off=' + pass_id,'pal_data_area' + pal_id);
		}
	function pal_sign_off(pal_id,pass_id) {
		ajfoo('pal_item' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_edit_ajax&pal_id=' + pal_id + '&pass_sign_off=' + pass_id,'pal_item' + pal_id);
		}
	function pal_update_full_description(pal_id) {
		ajfoo('pal_' + pal_id + '_full_description_div');
		ajaxManager('load_page','/global/pal$devel/?mode=pal_edit_ajax&pal_id=' + pal_id + '&full_description_set=' + encodeURI(document.getElementById('pal_item' + pal_id + '_full_description').value),'pal_' + pal_id + '_full_description_div');
		}
	function pal_update_due_date(pal_id,inputobject) {
		ajfoo('pal_item' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_edit_ajax&pal_id=' + pal_id + '&due_date_set=' + inputobject.value,'pal_item' + pal_id);
		}
	function pal_id_edit_toggle(pal_id,highlight) {
		obj=document.getElementById('pal_item' + pal_id);
		dataobj=document.getElementById('pal_item' + pal_id);
		if (highlight=='true') {
			dataobj.style.borderColor='red';
			dataobj.style.borderStyle='solid';
			dataobj.style.borderWidth='5px';
			} else {
			dataobj.style.borderColor='$fd_color_3';
			dataobj.style.borderStyle='solid';
			dataobj.style.borderWidth='4px';
			}
		if (obj.shrunk!='N') {
			obj.shrunk='N';
			obj.style.display='block';
			ajfoo('pal_item' + pal_id);
			ajaxManager('load_page','/global/pal$devel/?mode=pal_edit_ajax&pal_id=' + pal_id,'pal_item' + pal_id);
			pal_updated_header(pal_id,'true');
			} else {
			obj.shrunk='Y';
			obj.style.display='none';
			pal_updated_header(pal_id);
			}
		}
	function clean_up_contact_data() {
		document.contact_changed_value='';
		cobj=document.getElementById(document.contact_changed_id_last);
		cobj.value='';
		document.getElementById('image_link_' + document.contact_changed_id_last).style.border='';
		}
	function toggle_assignment(pal_id,contacts_id) {
		ajfoo('pal_data_area' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_add_assignments&pal_id=' + pal_id + '&toggle_contacts_id=' + contacts_id,'pal_data_area' + pal_id);
		clean_up_contact_data();
		}
	function pal_select_ass(pal_id,which) {
		ajfoo('pal_data_area' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_add_assignments&batch_select_users=true&pal_id=' + pal_id + '&which_ones=' + which,'pal_data_area' + pal_id);
		}
	function pal_batch_select_required(pal_id,which) {
		ajfoo('pal_data_area' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_add_assignments&batch_select_required=true&pal_id=' + pal_id + '&which_ones=' + which,'pal_data_area' + pal_id);
		}
	function owner_alert(owner) {
		alert(owner + ' created this assignment, and is the only one that can modify it.');
		}
	function mso_alert() {
		alert('This user is checked \"must sign off\". Their OK or admin override is required to clear this record.');
		}
	function pal_show_notes_tab(pal_id) {
		mainobj=document.getElementById('pal_notes_tab' + pal_id);
		obj2=document.getElementById('pal_current_tab' + pal_id);
		mainobj.style.background='$fd_color_1';
		mainobj.style.borderBottomColor='$fd_color_1';
		obj2.style.background='$fd_color_3';
		obj2.style.borderBottomColor='black';
		ajfoo('pal_data_area' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_add_notes&pal_id=' + pal_id,'pal_notes_area' + pal_id);
		}

	function pal_show_current_tab(pal_id) {
		mainobj=document.getElementById('pal_current_tab' + pal_id);
		obj3=document.getElementById('pal_notes_tab' + pal_id);
		mainobj.style.background='$fd_color_1';
		mainobj.style.borderBottomColor='$fd_color_1';
		obj3.style.background='$fd_color_3';
		obj3.style.borderBottomColor='black';
		ajfoo('pal_data_area' + pal_id);
		ajaxManager('load_page','/global/pal$devel/?mode=pal_current&pal_id=' + pal_id,'pal_data_area' + pal_id);
		}
	</script>
	";
    }

    function pal_box_queried($query)
    {
        global $devel, $fd_color_1, $fd_color_2, $fd_color_3, $fd_color_4;
        $doc_id = addslashes($doc_id);
        pal_javascripts();
        echo "
	<div style=\"border: 1px solid black;\" id=pal_box>";
        pal_box_filler("", $query);
        echo "</div>";
    }

    function pal_box($doc_id, $width = "140")
    {
        global $devel, $fd_color_1, $fd_color_2, $fd_color_3, $fd_color_4;
        if ($doc_id == "") return FALSE;
        $doc_id = addslashes($doc_id);
        pal_javascripts();
        echo "<table style=\"min-width: ${width}px; border: 1px solid black;\" cellspacing=0 cellpadding=0 id=pal_box><tr><td>";
        pal_box_filler($doc_id);
        echo "</td></tr></table>";
    }

    function pal_box_filler($doc_id = "", $query = "")
    {
        global $pal_open_bitte_schon, $fd_color_1, $fd_color_2, $fd_color_3, $fd_color_4, $global_jobinfo_id, $devel;
        if (($doc_id == "") && ($query == "")) return 0;
        if ($doc_id != "") {
            $doc_id = addslashes($doc_id);
            $query = "select * from documents_links right join documents on (documents.doc_id = documents_links.docb) where doca = '$doc_id' and doc_type = 'pal' order by app_reference + 0";
        }
        $res = @mysql_query($query);

        echo "<table style=\"padding: 0px;\" width=100% border=0 cellspacing=0 cellpadding=0><tr bgcolor=$fd_color_4><td><img style=\"cursor: pointer; cursor: hand;\" onclick=\"pal_add('$doc_id','$global_jobinfo_id');\" src=/images/pal_buddy.png></td><td align=center><b title=\"Click the puppy to add a task\">Pal Tasks</b></td></tr></table>";

        while ($row = @mysql_fetch_object($res)) {
            echo "<div style=\"min-width: 350px; cursor: pointer; cursor: hand; border-top: 2px solid black;\" id=\"pal_header_item$row->app_record_id\" onclick=\"pal_id_edit_toggle($row->app_record_id,this);\">";
            pal_item_show($row);
            echo "</div><div style=\"width: 350px; display: none;\" id=\"pal_item$row->app_record_id\"></div>";
        }
        if ($pal_open_bitte_schon != "") {
            $pal_open_bitte_schon = addslashes($pal_open_bitte_schon);
            echo "<script>pal_id_edit_toggle($pal_open_bitte_schon,'true');</script>";
        }
    }

    function pal_item_show($doc_info, $open = "")
    {
        global $fd_color_2, $fd_color_3, $fd_color_4;
        if ($doc_info->description == "") $doc_info->description = "&nbsp;";
        $pal_info = getone("select * from pal where pal_id = '$doc_info->app_record_id'");

        $bgcolor = '#ffffff';
        if ($pal_info->status == 'complete') {
            $show_date = invali_date($pal_info->pal_sign_off_date, '/');
            $bgcolor = $fd_color_2;
        } else {
            $show_date = invali_date($pal_info->due_date, '/');
        }
        if ($open == 'true') {
            $bgcolor = $fd_color_4;
        }

        echo "
	<div style=\"float: right; background: \"$bgcolor\";\">
	<font size=-1>&nbsp;&nbsp;$show_date</font></div>
	<div style=\"width: 100%; background: $bgcolor;\">
	<font style=\"background: $bgcolor;\" size=-1>
	<b>$doc_info->app_reference&nbsp;<font id=\"pal_{$pal_info->pal_id}_display_header_text\">
	$doc_info->description</b></font></font></div>";
    }

    function pasted_text_decoder($user_input)
    {
        $badwordchars = array(
            "\xe2\x80\x98", // left single quote
            "\xe2\x80\x99", // right single quote
            "\xe2\x80\x9c", // left double quote
            "\xe2\x80\x9d", // right double quote
            "\xe2\x80\x94", // em dash
            "\xe2\x80\x93" // em dash
        );
        $fixedwordchars = array(
            "'",
            "'",
            '"',
            '"',
            "-",
            "-"
        );
        return (str_replace($badwordchars, $fixedwordchars, $user_input));
    }

    function fast_connection()
    {
        return (!(slow_connection()));
    }

    function slow_connection($debug = "")
    {
        $addr = $GLOBALS['REMOTE_ADDR'];
        if ($debug == 1) echo "<hr>$addr<hr>";
        $addr = ereg_replace("[.][^.]*$", '', $addr);
        switch ($addr) {
            case "10.0.0":
            case "10.0.1":
            case "10.0.2":
            case "10.0.3":
                return FALSE;
                break;;
            default:
                return TRUE;
                break;;
        }
        return TRUE;
    }

    function update_all_weather_stations()
    {
        //return 0;
        include("weather_data.phtml");
        //$res=@mysql_query("select weather_station_code from jobinfo where active = 'Y' and weather_station_code != '' group by weather_station_code order by weather_station_code");
        $res = @mysql_query("select * from jobinfo where active = 'Y' and weather_station_code != '' group by weather_station_code order by weather_station_code");
        while ($row = @mysql_fetch_object($res)) {
            //uwsd($row->weather_station_code);
            //echo "$row->contract_num\r\n";
            $foo = check_weather_data($row);
        }
    }

    function check_weather_data($jobinfo)
    {
        //return 0;
        if ($jobinfo->weather_station_code == "") return FALSE;
        $last_update_time = strtotime($jobinfo->weather_update_time);
        $current_time = mktime();
        $time_diff = $current_time - $last_update_time;
        if ($time_diff > '4000') {
            //echo "updating weather data (time diff: $time_diff: $last_update_time/$current_time)";
            include("weather_data.phtml");
            $jobinfo = update_weather_data($jobinfo);
        }
        return ($jobinfo);
    }

    function standard_weather_string($jobinfo)
    {
        if ($jobinfo->weather_station_code == "") return ("");
        if ($jobinfo->active != 'Y') return ("<b title=\'I bet you care about sounds from trees when there are no people around too!!\'>Weather: Who cares? The job is not active!!!</b>");
        $jobinfo = check_weather_data($jobinfo);
        return ("$jobinfo->weather_condition @ $jobinfo->weather_temperature&deg; Winds: $jobinfo->weather_wind");
    }

    function getcraft($phase)
    {
        $phase = substr($phase, 0, 5);
        $phase_tiny = substr($phase, 0, 2);

        switch ($phase_tiny) {
            case "10";
                $craft = "Industrial";
                break;;
            case "15";
                $craft = "Plumbing";
                break;;
            case "16";
                $craft = "Heating";
                break;;
            case "17";
                $craft = "Sheet Metal";
                break;;
            default:
                $craft = "Other";
                break;;
        }

        switch ($phase) {
            case "10870";
            case "10871";
            case "10872";
            case "15870";
            case "16870";
            case "17870";
                $craft = "Supervision";
                break;;
            case "18880";
                $craft = "IT";
                break;;
            case "22100";
            case "22200";
            case "22300";
                $craft = "Service";
                break;;
            case "29050":
                $craft = "Helper/Laborer";
                break;;
            case "29060":
                $craft = "Teamster";
                break;;
            case "18915":
            case "20200":
            case "25030":
                $craft = "Estimating";
                break;;
            case "19708":
                $craft = "Vacation";
                break;;
            case "26040":
                $craft = "CAD";
                break;;
            case "26010":
            case "26020":
            case "26030":
            case "18810":
            case "18811":
                $craft = "Business Dev";
                break;;
            case "18840":
                $craft = "Safety";
                break;;
            case "26060":
                $craft = "Design/Eng Misc";
                break;;
            case "29030":
                $craft = "Light Duty";
                break;;
            case "25040":
                $craft = "Detailing";
                break;;
            case "18870":
            case "25070":
            case "25090":
                $craft = "Admin";
                break;;
            case "19724":
            case "29040":
                $craft = "Training";
                break;;
            case "25870":
            case "18917":
                $craft = "Superintendent";
                break;;
            case "25001":
                $craft = "Safety/Orientation/Drug Test";
                break;;
            case "25010":
                $craft = "Project Mgmt";
                break;;
            case "25015":
            case "25030":
                $craft = "Indirects";
                break;;

        }

        return ($craft);
    }

    function check_report_permissions()
    {
        global $report_name, $global_user_id, $adminuser, $global_jobinfo_id;
        /*if ($report_name_special!="") $rname=$report_name_special;
	else $rname=$report_name;
	if ($rname=="") die_security();
	$rname=addslashes($rname);
	$perms=getoneb("select * from front_desk_reports natural join front_desk_reports_permissions where contacts_id = '$global_user_id' and report_name = '$rname' and only_for_job = '$global_jobinfo_id' and only_for_job != 0");
	if (!($perms)) $perms=getoneb("select * from front_desk_reports natural join front_desk_reports_permissions where contacts_id = '$global_user_id' and report_name = '$rname'");
	if (!($perms)) die_security();
	if ($perms->admin_user=='Y') $adminuser=TRUE;
	else $adminuser=FALSE;
	return ($perms);
	*/
        $perms = get_report_permissions();
        if (!($perms)) die_security();
    }

    function get_report_permissions($report_name_special = "")
    {
        global $report_name, $global_user_id, $adminuser, $global_jobinfo_id;
        if ($report_name_special != "") $rname = $report_name_special;
        else $rname = $report_name;
        if ($rname == "") die_security();
        $rname = addslashes($rname);
        $perms = getoneb("select * from front_desk_reports natural join front_desk_reports_permissions where contacts_id = '$global_user_id' and report_name = '$rname' and only_for_job = '$global_jobinfo_id' and only_for_job != 0");
        if (!($perms)) $perms = getoneb("select * from front_desk_reports natural join front_desk_reports_permissions where contacts_id = '$global_user_id' and report_name = '$rname'");
        //if (!($perms)) die_security();
        if ($perms->admin_user == 'Y') $adminuser = TRUE;
        else $adminuser = FALSE;
        return ($perms);
    }

    function vp_attachment_download_link($php_attach_id)
    {
        global $devel;
        if ($php_attach_id == "") return ("");
        $link_url = "/global/front_desk$devel/?mode=vp_attachment_download&php_attach_id=$php_attach_id";
        $link = "<a href=$link_url><img border=0 src=\"/images/paperclip.gif\"></a>";
        return ($link);
    }

    function vp_attachment_view_link($php_attach_id)
    {
        global $devel;
        if ($php_attach_id == "") return ("");
        $link_url = "/global/front_desk$devel/?mode=vp_attachment_download&php_attach_id=$php_attach_id";
        $link = "<a href=$link_url><img border=0 src=\"/images/paperclip.gif\"></a>";
        return ($link);
    }

    function vp_attachment_path($php_attach_id)
    {
        $vp_at_id = vp_attachment_id($php_attach_id);
        $file_info = ms_getoneb("select * from HQAT with (NOLOCK) where UniqueAttchID = '$vp_at_id'");
        return ($file_info->DocName);
    }

    function vp_attachment_id($php_attach_id)
    {
        return ($php_attach_id);
        return (strtolower(ereg_replace("[^a-zA-Z0-9]", "", $php_attach_id)));
        $php_attach_id = ($php_attach_id);

        $vp_attachment_id = substr($php_attach_id, 6, 2) . substr($php_attach_id, 4, 2) . substr($php_attach_id, 2, 2) . substr($php_attach_id, 0, 2) . '-' . substr($php_attach_id, 10, 2) . substr($php_attach_id, 8, 2) . '-' . substr($php_attach_id, 14, 2) . substr($php_attach_id, 12, 2) . '-' . substr($php_attach_id, 16, 4) . '-' . substr($php_attach_id, 20, 12);

        return (strtolower(ereg_replace("[^a-zA-Z0-0]", "", $php_attachment_id)));
        return ($vp_attachment_id);
    }

    function update_trade($contacts_id)
    {
        $employee_info = getoneb("select * from contacts where contacts_id = '$contacts_id'");
        $employee_num = $employee_info->employee_num;
        if ($employee_num < 1) return FALSE;
        //ms_tabledump("select * from HRRM where HRCo = 1 and HRRef = '$employee_num'");
        $vp_info = ms_getoneb("select * from HRRM with (NOLOCK) where HRCo = 1 and HRRef = '$employee_num'");

        //echo "<hr>q<hr>";
        if (!($vp_info)) return FALSE;
        $vp_class_info = ms_getoneb("select * from PRCM with (NOLOCK) where PRCo = 1 and Craft = '$vp_info->StdCraft'");
        $trade = strtolower($vp_class_info->udtrade);
        $vp_info->StdCraft = strtoupper($vp_info->StdCraft);
        $vp_info->StdClass = strtoupper($vp_info->StdClass);

        $class = "";
        $class_first_two = (substr(strtoupper($vp_info->StdClass), 0, 2));

        if (($class == "") && ($class_first_two == "GF")) $class = "General Foreman";
        if (($class == "") && ($class_first_two == "FO")) $class = "Foreman";
        if (($class == "") && ($class_first_two == "JO")) $class = "Journeyman";
        if (($class == "") && (ereg('%', $vp_info->StdClass))) $class = "Apprentice";
        if ($class == "") $class = "Other";

        //echo "<hr>$trade<hr>";
        if ($trade == "plumbers & fitters") {
            $vp_pr_info = ms_getoneb("select * from PREH with (NOLOCK) where PRCo = 1 and Employee = '$employee_num'");
            if ($vp_pr_info->InsCode == "WA0306") $trade = "plumber";
            if ($vp_pr_info->InsCode == "WA0307") $trade = "fitter";
            if ($trade == "plumbers & fitters") {
                $halfyearback = date('m/d/Y', strtotime("6 months ago"));
                $plumb_hours = ms_getoneb("select sum(Hours) as total from PRTH with (NOLOCK) where PRCo = 1 and Employee = '$vp_info->HRRef' and Phase like '15%' and PostDate > '$halfyearback'");
                $fit_hours = ms_getoneb("select sum(Hours) as total from PRTH with (NOLOCK) where PRCo = 1 and Employee = '$vp_info->HRRef' and Phase like '16%' and PostDate > '$halfyearback'");
                if ($plumb_hours->total > $fit_hours->total) $trade = "plumber";
                if ($plumb_hours->total < $fit_hours->total) $trade = "fitter";
                if ($trade == "plumbers & fitters") {
                    $plumb_hours = ms_getoneb("select sum(Hours) as total from PRTH with (NOLOCK) where PRCo = 1 and Employee = '$vp_info->HRRef' and Phase like '15%'");
                    $fit_hours = ms_getoneb("select sum(Hours) as total from PRTH with (NOLOCK) where PRCo = 1 and Employee = '$vp_info->HRRef' and Phase like '16%'");
                    if ($plumb_hours->total > $fit_hours->total) $trade = "plumber";
                    if ($plumb_hours->total < $fit_hours->total) $trade = "fitter";
                }
            }
        }
        $trade_description = "$trade - $class";
        if ($vp_info->StdCraft == '9999') {
            switch ($vp_info->StdClass) {
                case "ADMN":
                    $trade_description = "Administration";
                    break;;
                case "DETL":
                    $trade_description = "Detailing";
                    break;;
                case "ENGN":
                    $trade_description = "Engineering";
                    break;;
                case "ESTM":
                    $trade_description = "Estimating";
                    break;;
                case "EXEC":
                    $trade_description = "Executive";
                    break;;
                case "INSY":
                    $trade_description = "IT";
                    break;;
                case "PENG":
                    $trade_description = "Project Engineer";
                    break;;
                case "PRAD":
                    $trade_description = "Project Admin";
                    break;;
                case "PRMG":
                    $trade_description = "Project Manager";
                    break;;
                case "PURC":
                    $trade_description = "Purchasing";
                    break;;
                case "SAFE":
                    $trade_description = "Safety";
                    break;;
                case "SPRM":
                    $trade_description = "Senior Project Manager";
                    break;;
            }
            $class = $trade_description;
        }
        $res = @mysql_query("update contacts set employee_trade = '$trade', employee_union_local = '$vp_info->StdCraft', employee_trade_description = '$trade_description', employee_class = '$class' where contacts_id = '$employee_info->contacts_id'");
        return ($res);
    }

    function pdf_mime_header($filename = "unknown.pdf")
    {
        header("Content-disposition: inline; filename=\"$filename\"");
        header("Content-type: application/pdf");
        header("Content-Transfer-Encoding: binary");
        //header("Content-Length: $file->file_size");
        header("Expires: 0");
        header("Cache-Control: private");
        header("Pragma: cache");

    }

    function jpg_mime_header($filename = "unknown.pdf")
    {
        header("Content-disposition: inline; filename=\"$filename\"");
        header("Content-type: image/jpg");
        header("Content-Transfer-Encoding: binary");
        header("Expires: 0");
        header("Cache-Control: private");
        header("Pragma: cache");
    }

    function pdf_generation_error_pdf($error = "Unknown error.")
    {
        global $pdf_font_path;
        require_once('pdf_lib2.inc');
        // GPH Changed &new to new (former is deprecated)
        $pdf = new Cezpdf("$global_user->pref_paper_size", "portrait");
        $pdf->selectFont("$pdf_font_path/Helvetica.afm");
        $pdf->ezText("<b>ERROR: Unable to view or include pdf document!</b>");
        $pdf->ezText($error);
        $pdf->ezStream();
    }
    
    function get_mileage_rate($workday)
    {
         $workday = date("Y-m-d", strtotime(str_replace('-', '/', $workday)));
        $sql = "SELECT * FROM timesheet_mileage_rates WHERE effective_date <= '".$workday."' ORDER BY effective_date DESC LIMIT 1";
        //echo $sql;
         $mileage_info = getoneb( $sql );
         if (!empty($mileage_info))
            return $mileage_info->rate;
        else
            return false;
    }


#######################################################################
#	End of functions, put the next one here.
#######################################################################
} ?>
