<?
/*

//
//	This stuff has been moved to querylib.inc now..
//	

function web_pass_set ($contacts_id,$password) {
	global $pepper;
	$salt_starter=genrandstring(22);
	$salt='$2a$07$' . $salt_starter . '$';
	$crypted=crypt($password . $pepper,$salt);
	$res=@mysql_query("update contacts set fd2_pass_salt = '$salt', fd2_pass_hash = '$crypted' where contacts_id = '$contacts_id'");
	if ($res) {
		security_log_email($contacts_id,0,0,"hashed password changed","fd2-access");
		return (TRUE);
		} else {
		security_log_email($contacts_id,0,0,"hashed password set attempted but failed - application problem?","fd2-access");
		return (FALSE);
		}
	return (FALSE);
	}

function web_pass_set_try ($contacts_id,$current_password,$password) {
	if (web_pass_validate($contacts_id,$current_password)) {
		web_pass_set($contacts_id,$password);
		return (TRUE);
		}
	security_log_email($contacts_id,0,0,"hashed password change attempted - failed - authentication failure","fd2-access");
	return (FALSE);
	}

function web_pass_validate ($login_name,$password) {
	global $pepper;
	$login_name=@mysql_real_escape_string($login_name);
	$contact_info=getoneb("select * from contacts where login_name = '$login_name'");
	$crypted=crypt($password . $pepper,$contact_info->fd2_pass_salt);
	if ($crypted==$contact_info->fd2_pass_hash) return (TRUE);
	return (FALSE);
	}

function genrandstring($length) {
	static $seed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	$pepper="";
	$i=0;
	while($i<$length) {
		$pepper .= $seed[mt_rand(0, 61)];
		$i++;
		}
	return($pepper);
	}

function web_pass_send_otp_link($email_address,$comment="") {
	if ($email_address=="") return (FALSE);
	$query="select * from contacts where email = '" . mysql_real_escape_string($email_address) . "'";
	$contact_info=getoneb("select * from contacts where email = '$email_address'");
	//////////////////////////////////////////////////////
	// Why did I fail to look up contact info????
	//////////////////////////////////////////////////////
	if (!($contact_info)) {
		$res=@mysql_query($query);
		$count=@mysql_num_rows($res);
		if ($count > 1 ) {
			security_log_email(0,0,0,"OTP recovery link attempted - failed - multiple contacts with same email - " . mysql_real_escape_string($email_address) . " has $count contacts","fd2_access");
			}
		return (FALSE);
		}
	//////////////////////////////////////////////////////
	// We're not trying to reset/recover a UMC Employee's
	// password with this are we?
	//////////////////////////////////////////////////////
	if ($contact_info->umc_emp=='Y') {
		security_log_email($contact_info->contacts_id,0,0,"OTP recovery link attempted on UMC Employee!!!! NOT ALLOWED!!!","fd2_access");
		return (FALSE);
		}
	//////////////////////////////////////////////////////
	// Make sure login and email are identical, which is
	// required for third party users.
	//////////////////////////////////////////////////////
	if ($contact_info->email != $contact_info->login_name) {
		security_log_email($contact_info->contacts_id,0,0,"OTP recovery attempted on account with invalid login name (doesn't match email address)","fd2_access");
		return (FALSE);
		}
	$yesterday=date('Y-m-d',strtotime('yesterday'));
	$res=@mysql_query("update fd2_access_otp set expires = '$yesterday' where contacts_id = '$contact_info->contacts_id' and expires > '$yesterday'");
	if (!($res)) {
		echo "query executed, but seems to have failed..<br>";
		}
	$otp=genrandstring(60);
	$expires=date('Y-m-d',strtotime("+5 days"));
	$insert_query="insert into fd2_access_otp set 
			contacts_id = '$contact_info->contacts_id',
			expires = '$expires',
			accesses_remaining = 5,
			pepper = '$otp',
			notes = '" . mysql_real_escape_string($comment) . "'";
	$res=@mysql_query($insert_query);
	
	if (!($res)) {
		//die("failed query:<hr>" . $insert_query);
		return (FALSE);
		}
	
	
	$link="<a href=https://pipeline.umci.com/global/fd2-access/reset.php?otp=$otp&login=$contact_info->login_name>Password Reset</a>";
	
	$message="
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
	
	$headers  = 'MIME-Version: 1.0' . "\r\n";
	$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
	$headers .= 'From: UMC Login Reset <noreply@umci.com>' . "\r\n";
	mail($contact_info->email,"UMC Password Reset",$message,$headers);
	return (TRUE);
	}
*/




?>
