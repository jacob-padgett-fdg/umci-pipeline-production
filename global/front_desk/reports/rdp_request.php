<?
$format_string="Y-m-d H:i:s";
$pretty_format_string="H:i:s m/d/Y";
$IP_TABLES_COMMAND_PREFIX="-t nat";
$now_hour=date('G');

function is_trusted_address() {
	$local_net="10.";
	$address=$GLOBALS['REMOTE_ADDR'];
	return(!(strncmp($address,$local_net,3)));
	}

function expire_check() {
	$query="select * from front_desk_remote_access_requests where expired = 'N' and request_time_end < now()";
	$res=@mysql_query($query);
	while ($row=@mysql_fetch_object($res)) {
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -D $row->ip_tables_rule");
		@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where request_id = '$row->request_id'");
		}
	exec("/usr/sbin/conntrack -F conntrack");
	}

//////////////////////////////////////////////////////////////////////////////////////////////
if ($man_overboard!="") { 
	$man_overboard=addslashes($man_overboard);
	require("global_connect_rw.phtml");
	@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where request_id = '$man_overboard'");
	$fw_profile="crazy_ivan";
	}
//////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="torpedo") { 
	require("global_connect_rw.phtml");
	@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where expired != 'Y'");
		$fw_profile="crazy_ivan";
	if ($gui!='Y') {
		echo "torpedo ran";
		}
	}
//////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="icbm") { 
	require("global_connect_rw.phtml");
	@mysql_query("delete from front_desk_remote_access_requests");
	$fw_profile="crazy_ivan";
	if ($gui!='Y') {
		echo "icbm ran";
		}
	}
//////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="ping") { 
	require("global_connect_rw.phtml"); expire_check(); 
	if ($gui!='Y') {
		echo "ping ran";
		exit; 
		}
	}
//////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="crazy_ivan") {
	require("global_connect_rw.phtml");
	expire_check();
	$query="select * from front_desk_remote_access_requests where enabled = 'Y' and expired = 'N'";
	$res=@mysql_query($query);
	exec("/sbin/iptables -t nat -F");
	while ($row=@mysql_fetch_object($res)) {
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $row->ip_tables_rule");
		}
	# Global rule for all users inside the network so they never need the web page..
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A PREROUTING -s 10.0.0.1/8 -p tcp --dport 3389 -j DNAT --to 10.0.0.66");
	# SNAT for normal users since Lister wouldn't normally be in the routing path..
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A POSTROUTING -p tcp --dport 3389 -d 10.0.0.66/32 -j SNAT --to 10.0.0.23");
	# SNAT for 10.0.1.93 just like 10.0.0.66 had
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A POSTROUTING -p tcp --dport 1022 -d 10.0.1.93/32 -j SNAT --to 10.0.0.23");
	# Viewpoint stuff
		# SNAT for RDP
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A POSTROUTING -p tcp --dport 3389 -d 10.0.0.26/32 -j SNAT --to 10.0.0.23");
		# SNAT for SQL
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A POSTROUTING -p tcp --dport 1433 -d 10.0.0.26/32 -j SNAT --to 10.0.0.23");
	# Fieldcentrix stuff
		# SNAT for RDP
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A POSTROUTING -p tcp --dport 3389 -d 10.0.0.96/32 -j SNAT --to 10.0.0.23");
		# SNAT for SQL
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A POSTROUTING -p tcp --dport 1433 -d 10.0.0.96/32 -j SNAT --to 10.0.0.23");
		# SNAT for PC Anywhere
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A POSTROUTING -p tcp --dport 5631 -d 10.0.0.96/32 -j SNAT --to 10.0.0.23");
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A POSTROUTING -p tcp --dport 5632 -d 10.0.0.96/32 -j SNAT --to 10.0.0.23");
	# Reset already established connections.. If they're still in the table, they'll be OK, but
	# if not, they'll get hung up on immediately. Otherwise it will last until their session closes	
	exec("/usr/sbin/conntrack -F conntrack");
	if ($gui!='Y') {
		echo "crazy ivan ran";
		exit;
		}
	}
//////////////////////////////////////////////////////////////////////////////////////////////
$eol="";
$RDP_FILE="screen mode id:i:2$eol
desktopwidth:i:$eol
desktopheight:i:$eol
session bpp:i:16$eol
auto connect:i:1$eol
autoreconnection enabled:i:1$eol
compression:i:1$eol
connect to console:i:0$eol
full address:s:pipeline.umci.com$eol
keyboardhook:i:2$eol
audiomode:i:0$eol
redirectdrives:i:1$eol
redirectprinters:i:1$eol
redirectcomports:i:0$eol
redirectsmartcards:i:0$eol
displayconnectionbar:i:1$eol
username:s:$global_user->login_name$eol
domain:s:UMC$eol
alternate shell:s:$eol
shell working directory:s:$eol
disable wallpaper:i:1$eol
disable full window drag:i:1$eol
disable menu anims:i:1$eol
disable themes:i:0$eol
bitmapcachepersistenable:i:1$eol
$eol";
//////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="rdpfile") { 
	//header("Content-Type: application/octet-stream");
	header("Content-Type: application/rdp");
	header("Content-Disposition: inline; filename=UMC.rdp");
	echo "$RDP_FILE";
	exit; 
	}










require("report_lib.inc");
require("mh_lib.inc");
check_report_permissions();

if ($now_hour>1) { 
	$crazy_ivan_time=date($format_string,strtotime("tomorrow 2 am"));
	} else { 
	$crazy_ivan_time=date($format_string,strtotime("2 am"));
	}
$request_window_time=date($format_string,strtotime("+ 10 hours"));

if ($request_window_time>$crazy_ivan_time) $max_request_time=$crazy_ivan_time;
else $max_request_time=$request_window_time;






///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Firewall profile for standard user requests
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*(if ($fw_profile=="rdp") {
	$delres=@mysql_query("select * from front_desk_remote_access_requests where expired = 'N' and ip_address = '" . $GLOBALS['REMOTE_ADDR'] . "'");
	while ($delrow=@mysql_fetch_object($delres)) {
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -D $delrow->ip_tables_rule");
		@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where request_id = '$delrow->request_id'");
		}
	$IP_TABLES_COMMAND=" PREROUTING -s " . $GLOBALS['REMOTE_ADDR'] . " -p tcp --dport 3389 -j DNAT --to 10.0.0.66";
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
	@mysql_query("insert into front_desk_remote_access_requests set 
	contacts_id = '$global_user_id',
	fw_profile = 'rdp',
	ip_address = '" . $GLOBALS['REMOTE_ADDR'] . "',
	request_time = now(),
	request_time_start = now(),
	request_time_end = '$max_request_time',
	ip_tables_rule = '$IP_TABLES_COMMAND',
	enabled = 'Y',
	expired = 'N',
	request_description = 'User requested access from web page'");
	forward("$mode&report_name=$report_name");
	exit;
	}
*/








///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Devel Web Server Forwarding
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="www-dev") {
	if (!($adminuser)) die_security();
	$queue_info=getoneb("select * from front_desk_remote_access_request_queue where queue_id = '" . mysql_real_escape_string($queue_id) . "'");
	if (!($queue_info)) die("application error: unable to load queue info");
	$requestor=getoneb("select * from contacts where contacts_id = '$queue_info->contacts_id'");
	
	////////////////////////////////////////
	// Figure out when to end this thing
	////////////////////////////////////////
	$end_request_time=date($format_string,strtotime("+ $queue_info->requested_duration hours"));
	if ($end_request_time>$crazy_ivan_time) $end_request_time=$crazy_ivan_time;
	////////////////////////////////////////
	//die("so far so good");
	$delres=@mysql_query("select * from front_desk_remote_access_requests where expired = 'N' and ip_address = '$queue_info->ip_address'");
	while ($delrow=@mysql_fetch_object($delres)) {
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -D $delrow->ip_tables_rule");
		@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where request_id = '$delrow->request_id'");
		}
	$IP_TABLES_COMMAND=" PREROUTING -s $queue_info->ip_address -p tcp --dport 1022 -j DNAT --to 10.0.1.93";
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
	@mysql_query("insert into front_desk_remote_access_requests set 
	contacts_id = '$queue_info->contacts_id',
	fw_profile = 'www-dev',
	ip_address = '$queue_info->ip_address',
	request_time = now(),
	request_time_start = now(),
	request_time_end = '$end_request_time',
	ip_tables_rule = '$IP_TABLES_COMMAND',
	enabled = 'Y',
	expired = 'N',
	request_description = 'Queue request ($queue_info->queue_id) approved by $global_user->login_name($global_user->contacts_id)'");
	
	@mysql_query("update front_desk_remote_access_request_queue set approved_time = now(), status = 'approved', approved_by = '$global_user->contacts_id' where queue_id = '$queue_info->queue_id'");
	
	queue_email($queue_info->contacts_id,"Request Accepted","Your access request has been accepted. Your firewall profile will remain open until $end_request_time.",'Y');
	$group_info=getoneb("select * from contacts_groups where jobinfo_id = 0 and description = 'IT'");
	$group_users_res=@mysql_query("select * from contacts_groups_members where group_id = '$group_info->group_id'");
	while ($gu_row=@mysql_fetch_object($group_users_res)) {
		queue_email($gu_row->contacts_id,"ACCESS REQUEST GRANTED: $global_user->display_name for $fw_profile - $queue_info->requested_duration hrs","$global_user->first_name $global_user->last_name has approved of the access request from $requestor->display_name",'Y');
		}
	
	forward("$mode&report_name=$report_name");
	exit;
	}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Devel Web Server Forwarding for Mantis from their CO
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="www-dev-mantis-co") {
	if (!($adminuser)) die_security();
	$queue_info=getoneb("select * from front_desk_remote_access_request_queue where queue_id = '" . mysql_real_escape_string($queue_id) . "'");
	if (!($queue_info)) die("application error: unable to load queue info");
	$requestor=getoneb("select * from contacts where contacts_id = '$queue_info->contacts_id'");
	
	////////////////////////////////////////
	// Figure out when to end this thing
	////////////////////////////////////////
	$end_request_time=date($format_string,strtotime("+ $queue_info->requested_duration hours"));
	if ($end_request_time>$crazy_ivan_time) $end_request_time=$crazy_ivan_time;
	////////////////////////////////////////
	//die("so far so good");
	$delres=@mysql_query("select * from front_desk_remote_access_requests where expired = 'N' and ip_address = '$queue_info->ip_address'");
	while ($delrow=@mysql_fetch_object($delres)) {
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -D $delrow->ip_tables_rule");
		@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where request_id = '$delrow->request_id'");
		}
	// CO Location #1
	$IP_TABLES_COMMAND=" PREROUTING -s 72.11.65.146 -p tcp --dport 1022 -j DNAT --to 10.0.1.93";
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
	@mysql_query("insert into front_desk_remote_access_requests set 
	contacts_id = '$queue_info->contacts_id',
	fw_profile = 'www-dev-mantis-co',
	ip_address = '72.11.65.146',
	request_time = now(),
	request_time_start = now(),
	request_time_end = '$end_request_time',
	ip_tables_rule = '$IP_TABLES_COMMAND',
	enabled = 'Y',
	expired = 'N',
	request_description = 'Queue request ($queue_info->queue_id) approved by $global_user->login_name($global_user->contacts_id)'");
	
	// CO Location #2
	$IP_TABLES_COMMAND=" PREROUTING -s 208.26.195.98 -p tcp --dport 1022 -j DNAT --to 10.0.1.93";
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
	@mysql_query("insert into front_desk_remote_access_requests set 
	contacts_id = '$queue_info->contacts_id',
	fw_profile = 'www-dev-mantis-co',
	ip_address = '208.26.195.98',
	request_time = now(),
	request_time_start = now(),
	request_time_end = '$end_request_time',
	ip_tables_rule = '$IP_TABLES_COMMAND',
	enabled = 'Y',
	expired = 'N',
	request_description = 'Queue request ($queue_info->queue_id) approved by $global_user->login_name($global_user->contacts_id)'");
	
	@mysql_query("update front_desk_remote_access_request_queue set approved_time = now(), status = 'approved', approved_by = '$global_user->contacts_id' where queue_id = '$queue_info->queue_id'");
	
	queue_email($queue_info->contacts_id,"Request Accepted","Your access request has been accepted. Your firewall profile will remain open until $end_request_time.",'Y');
	$group_info=getoneb("select * from contacts_groups where jobinfo_id = 0 and description = 'IT'");
	$group_users_res=@mysql_query("select * from contacts_groups_members where group_id = '$group_info->group_id'");
	while ($gu_row=@mysql_fetch_object($group_users_res)) {
		queue_email($gu_row->contacts_id,"ACCESS REQUEST GRANTED: $global_user->display_name for $fw_profile - $queue_info->requested_duration hrs","$global_user->first_name $global_user->last_name has approved of the access request from $requestor->display_name",'Y');
		}
	
	forward("$mode&report_name=$report_name");
	exit;
	}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Adding their request for development access to the queue..
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="Development") {
	$duration=mysql_real_escape_string($duration);
	@mysql_query("update front_desk_remote_access_request_queue set
		status = 'aborted' where status = 'new' and contacts_id = '$global_user->contacts_id'");
	$insqry="insert into front_desk_remote_access_request_queue set
	contacts_id = '$global_user->contacts_id',
	fw_profile = 'www-dev',
	ip_address = '" . $GLOBALS['REMOTE_ADDR'] . "',
	request_time = now(),
	requested_duration = '$duration',
	status = 'new'";
	$insres=mysql_query($insqry);
	if (!($insres)) {
		die("Query failed<hr>$insqry");
		}
	queue_email($global_user->contacts_id,"Request Received","Your access request has been received. When a member of UMC's IT team approves it, you'll receive another email.",'Y');
	$group_info=getoneb("select * from contacts_groups where jobinfo_id = 0 and description = 'IT'");
	$group_users_res=@mysql_query("select * from contacts_groups_members where group_id = '$group_info->group_id'");
	while ($gu_row=@mysql_fetch_object($group_users_res)) {
		queue_email($gu_row->contacts_id,"ACCESS REQUEST: $global_user->display_name for $fw_profile - $duration hrs","Please <a href=https://pipeline.umci.com/global/front_desk/?mode=report_show&report_name=rdp_request><font color=blue>review</font></a> any open request and determine a course of action.",'Y');
		}
	}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Adding request for mantis development from their CO to the queue..
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="Development-Mantis-CO") {
	$duration=mysql_real_escape_string($duration);
	@mysql_query("update front_desk_remote_access_request_queue set
		status = 'aborted' where status = 'new' and contacts_id = '$global_user->contacts_id'");
	$insqry="insert into front_desk_remote_access_request_queue set
	contacts_id = '$global_user->contacts_id',
	fw_profile = 'www-dev-mantis-co',
	ip_address = '" . $GLOBALS['REMOTE_ADDR'] . "',
	request_time = now(),
	requested_duration = '$duration',
	status = 'new'";
	$insres=mysql_query($insqry);
	if (!($insres)) {
		die("Query failed<hr>$insqry");
		}
	queue_email($global_user->contacts_id,"Request Received","Your access request has been received. When a member of UMC's IT team approves it, you'll receive another email.",'Y');
	$group_info=getoneb("select * from contacts_groups where jobinfo_id = 0 and description = 'IT'");
	$group_users_res=@mysql_query("select * from contacts_groups_members where group_id = '$group_info->group_id'");
	while ($gu_row=@mysql_fetch_object($group_users_res)) {
		queue_email($gu_row->contacts_id,"ACCESS REQUEST: $global_user->display_name for $fw_profile - $duration hrs","Please <a href=https://pipeline.umci.com/global/front_desk/?mode=report_show&report_name=rdp_request><font color=blue>review</font></a> any open request and determine a course of action.",'Y');
		}
	}














///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Production server forwarding for third parties
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="www") {
	if (!($adminuser)) die_security();
	$queue_info=getoneb("select * from front_desk_remote_access_request_queue where queue_id = '" . mysql_real_escape_string($queue_id) . "'");
	if (!($queue_info)) die("application error: unable to load queue info");
	$requestor=getoneb("select * from contacts where contacts_id = '$queue_info->contacts_id'");
	
	////////////////////////////////////////
	// Figure out when to end this thing
	////////////////////////////////////////
	$end_request_time=date($format_string,strtotime("+ $queue_info->requested_duration hours"));
	if ($end_request_time>$crazy_ivan_time) $end_request_time=$crazy_ivan_time;
	////////////////////////////////////////
	$delres=@mysql_query("select * from front_desk_remote_access_requests where expired = 'N' and ip_address = '$queue_info->ip_address'");
	while ($delrow=@mysql_fetch_object($delres)) {
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -D $delrow->ip_tables_rule");
		@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where request_id = '$delrow->request_id'");
		}
	$IP_TABLES_COMMAND=" PREROUTING -s $queue_info->ip_address -p tcp --dport 1022 -j DNAT --to 10.0.0.23:22";
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
	@mysql_query("insert into front_desk_remote_access_requests set 
	contacts_id = '$queue_info->contacts_id',
	fw_profile = 'www',
	ip_address = '$queue_info->ip_address',
	request_time = now(),
	request_time_start = now(),
	request_time_end = '$end_request_time',
	ip_tables_rule = '$IP_TABLES_COMMAND',
	enabled = 'Y',
	expired = 'N',
	request_description = 'Queue request ($queue_info->queue_id) approved by $global_user->login_name($global_user->contacts_id)'");
	
	@mysql_query("update front_desk_remote_access_request_queue set approved_time = now(), status = 'approved', approved_by = '$global_user->contacts_id' where queue_id = '$queue_info->queue_id'");
	
	queue_email($queue_info->contacts_id,"Request Accepted","Your access request has been accepted. Your firewall profile will remain open until $end_request_time.",'Y');
	$group_info=getoneb("select * from contacts_groups where jobinfo_id = 0 and description = 'IT'");
	$group_users_res=@mysql_query("select * from contacts_groups_members where group_id = '$group_info->group_id'");
	while ($gu_row=@mysql_fetch_object($group_users_res)) {
		queue_email($gu_row->contacts_id,"ACCESS REQUEST GRANTED: $global_user->display_name for $fw_profile - $queue_info->requested_duration hrs","$global_user->first_name $global_user->last_name has approved of the access request from $requestor->display_name",'Y');
		}
	
	forward("$mode&report_name=$report_name");
	exit;
	}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Production server forwarding for Mantis from their CO
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="www-mantis-co") {
	if (!($adminuser)) die_security();
	$queue_info=getoneb("select * from front_desk_remote_access_request_queue where queue_id = '" . mysql_real_escape_string($queue_id) . "'");
	if (!($queue_info)) die("application error: unable to load queue info");
	$requestor=getoneb("select * from contacts where contacts_id = '$queue_info->contacts_id'");
	
	////////////////////////////////////////
	// Figure out when to end this thing
	////////////////////////////////////////
	$end_request_time=date($format_string,strtotime("+ $queue_info->requested_duration hours"));
	if ($end_request_time>$crazy_ivan_time) $end_request_time=$crazy_ivan_time;
	////////////////////////////////////////
	//die("so far so good");
	$delres=@mysql_query("select * from front_desk_remote_access_requests where expired = 'N' and ip_address = '$queue_info->ip_address'");
	while ($delrow=@mysql_fetch_object($delres)) {
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -D $delrow->ip_tables_rule");
		@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where request_id = '$delrow->request_id'");
		}
	// CO Location #1
	$IP_TABLES_COMMAND=" PREROUTING -s 72.11.65.146 -p tcp --dport 1022 -j DNAT --to 10.0.0.23:22";
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
	@mysql_query("insert into front_desk_remote_access_requests set 
	contacts_id = '$queue_info->contacts_id',
	fw_profile = 'www-mantis-co',
	ip_address = '72.11.65.146',
	request_time = now(),
	request_time_start = now(),
	request_time_end = '$end_request_time',
	ip_tables_rule = '$IP_TABLES_COMMAND',
	enabled = 'Y',
	expired = 'N',
	request_description = 'Queue request ($queue_info->queue_id) approved by $global_user->login_name($global_user->contacts_id)'");
	
	// CO Location #2
	$IP_TABLES_COMMAND=" PREROUTING -s 208.26.195.98 -p tcp --dport 1022 -j DNAT --to 10.0.0.23:22";
	exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
	@mysql_query("insert into front_desk_remote_access_requests set 
	contacts_id = '$queue_info->contacts_id',
	fw_profile = 'www-mantis-co',
	ip_address = '208.26.195.98',
	request_time = now(),
	request_time_start = now(),
	request_time_end = '$end_request_time',
	ip_tables_rule = '$IP_TABLES_COMMAND',
	enabled = 'Y',
	expired = 'N',
	request_description = 'Queue request ($queue_info->queue_id) approved by $global_user->login_name($global_user->contacts_id)'");
	
	@mysql_query("update front_desk_remote_access_request_queue set approved_time = now(), status = 'approved', approved_by = '$global_user->contacts_id' where queue_id = '$queue_info->queue_id'");
	
	queue_email($queue_info->contacts_id,"Request Accepted","Your access request has been accepted. Your firewall profile will remain open until $end_request_time.",'Y');
	$group_info=getoneb("select * from contacts_groups where jobinfo_id = 0 and description = 'IT'");
	$group_users_res=@mysql_query("select * from contacts_groups_members where group_id = '$group_info->group_id'");
	while ($gu_row=@mysql_fetch_object($group_users_res)) {
		queue_email($gu_row->contacts_id,"ACCESS REQUEST GRANTED: $global_user->display_name for $fw_profile - $queue_info->requested_duration hrs","$global_user->first_name $global_user->last_name has approved of the access request from $requestor->display_name",'Y');
		}
	
	forward("$mode&report_name=$report_name");
	exit;
	}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Adding a request for third party access to production server.
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="Production") {
	$duration=mysql_real_escape_string($duration);
	@mysql_query("update front_desk_remote_access_request_queue set
		status = 'aborted' where status = 'new' and contacts_id = '$global_user->contacts_id'");
	$insqry="insert into front_desk_remote_access_request_queue set
	contacts_id = '$global_user->contacts_id',
	fw_profile = 'www',
	ip_address = '" . $GLOBALS['REMOTE_ADDR'] . "',
	request_time = now(),
	requested_duration = '$duration',
	status = 'new'";
	$insres=mysql_query($insqry);
	if (!($insres)) {
		die("Query failed<hr>$insqry");
		}
	queue_email($global_user->contacts_id,"Request Received","Your access request has been received. When a member of UMC's IT team approves it, you'll receive another email.",'Y');
	$group_info=getoneb("select * from contacts_groups where jobinfo_id = 0 and description = 'IT'");
	$group_users_res=@mysql_query("select * from contacts_groups_members where group_id = '$group_info->group_id'");
	while ($gu_row=@mysql_fetch_object($group_users_res)) {
		queue_email($gu_row->contacts_id,"ACCESS REQUEST: $global_user->display_name for $fw_profile - $duration hrs","Please <a href=https://pipeline.umci.com/global/front_desk/?mode=report_show&report_name=rdp_request><font color=blue>review</font></a> any open request and determine a course of action.",'Y');
		}
	}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Adding request for mantis to access the production server form their CO
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="Production-Mantis-CO") {
	$duration=mysql_real_escape_string($duration);
	@mysql_query("update front_desk_remote_access_request_queue set
		status = 'aborted' where status = 'new' and contacts_id = '$global_user->contacts_id'");
	$insqry="insert into front_desk_remote_access_request_queue set
	contacts_id = '$global_user->contacts_id',
	fw_profile = 'www-mantis-co',
	ip_address = '" . $GLOBALS['REMOTE_ADDR'] . "',
	request_time = now(),
	requested_duration = '$duration',
	status = 'new'";
	$insres=mysql_query($insqry);
	if (!($insres)) {
		die("Query failed<hr>$insqry");
		}
	queue_email($global_user->contacts_id,"Request Received","Your access request has been received. When a member of UMC's IT team approves it, you'll receive another email.",'Y');
	$group_info=getoneb("select * from contacts_groups where jobinfo_id = 0 and description = 'IT'");
	$group_users_res=@mysql_query("select * from contacts_groups_members where group_id = '$group_info->group_id'");
	while ($gu_row=@mysql_fetch_object($group_users_res)) {
		queue_email($gu_row->contacts_id,"ACCESS REQUEST: $global_user->display_name for $fw_profile - $duration hrs","Please <a href=https://pipeline.umci.com/global/front_desk/?mode=report_show&report_name=rdp_request><font color=blue>review</font></a> any open request and determine a course of action.",'Y');
		}
	}
































///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Rejecting third party request for access to devel server
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="www-dev-deny") {
	if (!($adminuser)) die_security();
	$queue_info=getoneb("select * from front_desk_remote_access_request_queue where queue_id = '" . mysql_real_escape_string($queue_id) . "'");
	if (!($queue_info)) die("application error: unable to load queue info");
	$requestor_info=getoneb("select * from contacts where contacts_id = '$queue_info->contacts_id'");
	@mysql_query("update front_desk_remote_access_request_queue set status = 'denied' where queue_id = '$queue_info->queue_id'");
	queue_email($queue_info->contacts_id,"Request Rejected","Your access request has been denied. Please contact a member of the UMC IT team in order to resolve this issue.",'Y');
	$group_info=getoneb("select * from contacts_groups where jobinfo_id = 0 and description = 'IT'");
	$group_users_res=@mysql_query("select * from contacts_groups_members where group_id = '$group_info->group_id'");
	while ($gu_row=@mysql_fetch_object($group_users_res)) {
		queue_email($gu_row->contacts_id,"ACCESS REQUEST DENIED: $global_user->display_name for $fw_profile - $queue_info->requested_duration hrs","$global_user->first_name $global_user->last_name has denied the remote access request from $requestor_info->login_name.",'Y');
		}
	
	forward("$mode&report_name=$report_name");
	exit;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Firewall profile for Viewpoint
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="vp") {
	if (!($adminuser)) die_security();
//  VP Portland address
	$VENDOR_SOURCE_IP_ADDRESS="63.95.90.130";
//  VP Charlotte address
#	$VENDOR_SOURCE_IP_ADDRESS="209.12.103.66";

	$VENDOR_TARGET_IP_ADDRESS="10.0.0.26";
	
	if ($rdp_connections_vendor_hours>8) $rdp_connections_vendor_hours=8;
	$vendor_request_window_time=date($format_string,strtotime("+ $rdp_connections_vendor_hours hours"));

	if ($vendor_request_window_time>$crazy_ivan_time) $vendor_max_request_time=$crazy_ivan_time;
	else $vendor_max_request_time=$vendor_request_window_time;
	$delres=@mysql_query("select * from front_desk_remote_access_requests where expired = 'N' and fw_profile = '$fw_profile'");
	while ($delrow=@mysql_fetch_object($delres)) {
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -D $delrow->ip_tables_rule");
		@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where request_id = '$delrow->request_id'");
		}
	
	if ($rdp_connections_vendor_hours>0) {
		$IP_TABLES_COMMAND=" PREROUTING -s $VENDOR_SOURCE_IP_ADDRESS -p tcp --dport 1433 -j DNAT --to $VENDOR_TARGET_IP_ADDRESS";
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
		@mysql_query("insert into front_desk_remote_access_requests set contacts_id = '$global_user_id', fw_profile = '$fw_profile', ip_address = '$VENDOR_SOURCE_IP_ADDRESS', request_time = now(), request_time_start = now(), request_time_end = '$vendor_max_request_time', ip_tables_rule = '$IP_TABLES_COMMAND', enabled = 'Y', expired = 'N', request_description = 'SQL  1433: " . addslashes($request_description) . "'");
		$IP_TABLES_COMMAND=" PREROUTING -s $VENDOR_SOURCE_IP_ADDRESS -p tcp --dport 3389 -j DNAT --to $VENDOR_TARGET_IP_ADDRESS";
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
		@mysql_query("insert into front_desk_remote_access_requests set contacts_id = '$global_user_id', fw_profile = '$fw_profile', ip_address = '$VENDOR_SOURCE_IP_ADDRESS', request_time = now(), request_time_start = now(), request_time_end = '$vendor_max_request_time', ip_tables_rule = '$IP_TABLES_COMMAND', enabled = 'Y', expired = 'N', request_description = 'RDP  3389: " . addslashes($request_description) . "'");
		}
	
	forward("$mode&report_name=$report_name&fw_profile=crazy_ivan&gui=Y");
	exit;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Firewall profile for Fieldcentrix
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
if ($fw_profile=="fx") {
	if (!($adminuser)) die_security();
	$VENDOR_SOURCE_IP_ADDRESS="66.210.52.251";
	$VENDOR_TARGET_IP_ADDRESS="10.0.0.96";
	
	if ($rdp_connections_vendor_hours>8) $rdp_connections_vendor_hours=8;
	$vendor_request_window_time=date($format_string,strtotime("+ $rdp_connections_vendor_hours hours"));

	if ($vendor_request_window_time>$crazy_ivan_time) $vendor_max_request_time=$crazy_ivan_time;
	else $vendor_max_request_time=$vendor_request_window_time;
	$delres=@mysql_query("select * from front_desk_remote_access_requests where expired = 'N' and fw_profile = '$fw_profile'");
	while ($delrow=@mysql_fetch_object($delres)) {
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -D $delrow->ip_tables_rule");
		@mysql_query("update front_desk_remote_access_requests set expired = 'Y', request_time_end = now() where request_id = '$delrow->request_id'");
		}
	
	if ($rdp_connections_vendor_hours>0) {
		$IP_TABLES_COMMAND=" PREROUTING -s $VENDOR_SOURCE_IP_ADDRESS -p tcp --dport 1433 -j DNAT --to $VENDOR_TARGET_IP_ADDRESS";
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
		@mysql_query("insert into front_desk_remote_access_requests set contacts_id = '$global_user_id', fw_profile = '$fw_profile', ip_address = '$VENDOR_SOURCE_IP_ADDRESS', request_time = now(), request_time_start = now(), request_time_end = '$vendor_max_request_time', ip_tables_rule = '$IP_TABLES_COMMAND', enabled = 'Y', expired = 'N', request_description = 'SQL  1433: " . addslashes($request_description) . "'");
		$IP_TABLES_COMMAND=" PREROUTING -s $VENDOR_SOURCE_IP_ADDRESS -p tcp --dport 3389 -j DNAT --to $VENDOR_TARGET_IP_ADDRESS";
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
		@mysql_query("insert into front_desk_remote_access_requests set contacts_id = '$global_user_id', fw_profile = '$fw_profile', ip_address = '$VENDOR_SOURCE_IP_ADDRESS', request_time = now(), request_time_start = now(), request_time_end = '$vendor_max_request_time', ip_tables_rule = '$IP_TABLES_COMMAND', enabled = 'Y', expired = 'N', request_description = 'RDP  3389: " . addslashes($request_description) . "'");
		$IP_TABLES_COMMAND=" PREROUTING -s $VENDOR_SOURCE_IP_ADDRESS -p tcp --dport 5631 -j DNAT --to $VENDOR_TARGET_IP_ADDRESS";
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
		@mysql_query("insert into front_desk_remote_access_requests set contacts_id = '$global_user_id', fw_profile = '$fw_profile', ip_address = '$VENDOR_SOURCE_IP_ADDRESS', request_time = now(), request_time_start = now(), request_time_end = '$vendor_max_request_time', ip_tables_rule = '$IP_TABLES_COMMAND', enabled = 'Y', expired = 'N', request_description = 'PCAW 5631: " . addslashes($request_description) . "'");
		$IP_TABLES_COMMAND=" PREROUTING -s $VENDOR_SOURCE_IP_ADDRESS -p tcp --dport 5632 -j DNAT --to $VENDOR_TARGET_IP_ADDRESS";
		exec("/sbin/iptables $IP_TABLES_COMMAND_PREFIX -A $IP_TABLES_COMMAND");
		@mysql_query("insert into front_desk_remote_access_requests set contacts_id = '$global_user_id', fw_profile = '$fw_profile', ip_address = '$VENDOR_SOURCE_IP_ADDRESS', request_time = now(), request_time_start = now(), request_time_end = '$vendor_max_request_time', ip_tables_rule = '$IP_TABLES_COMMAND', enabled = 'Y', expired = 'N', request_description = 'PCAW 5632: " . addslashes($request_description) . "'");
		}
	
	forward("$mode&report_name=$report_name&fw_profile=crazy_ivan&gui=Y");
	exit;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
//	End of firewall profile sections...
///////////////////////////////////////////////////////////////////////////////////////////////////



/*
echo "crazy ivan time: $crazy_ivan_time<br>
request window time: $request_window_time<br>
max request time: $max_request_time<br>
";
*/




$current_lease=getoneb("select * from front_desk_remote_access_requests where enabled = 'Y' and expired = 'N' and contacts_id = '$global_user_id' and ip_address = '" . $GLOBALS['REMOTE_ADDR'] . "' limit 1");


require("header.phtml");
mh_navs_header();
echo "<a href=$pagename?mode=my_home><font color=blue>Return to My Home</font></a><p>";



echo "
<form name=remote_access_queue_request method=post action=$pagename>
<input type=hidden name=mode value=\"$mode\">
<input type=hidden name=report_name value=\"$report_name\">

<table  width=400px border=1 cellspacing=0 cellpadding=1>
<tr><td bgcolor=$fd_color_4 align=center colspan=2><font size=+2>Remote Access Request</font></td></tr>
<tr><td>
	User:
</td><td>
	$global_user->display_name
</td></tr>

<tr><td>
	Access Requested:
</td><td>
	<select size=4 name=fw_profile>
	<option value=\"Development-Mantis-CO\">Development - Mantis (Office)</option>
	<option>Development</option>
	<option>Production</option>
	<option value=\"Production-Mantis-CO\">Production - Mantis (Office)</option>
	</select>
</td></tr>

<tr><td>
	Duration
</td><td>
	<select name=duration>
	<option>1</option>
	<option>2</option>
	<option>3</option>
	<option>4</option>
	<option>5</option>
	<option>6</option>
	<option>7</option>
	<option>8</option>
	</select>&nbsp;hrs
</td></tr>

<tr><td colspan=2 align=right>
	<input type=submit>
</td></tr>

</table>
</form>
";

//if (!($adminuser)) {
//	echo "</table></table>";
//	mh_navs_footer();
//	die();
//	}


if($adminuser) {
$pending_requests_query="select * from front_desk_remote_access_request_queue where status = 'new'";
//tabledump($pending_requests_query);
$pending_res=@mysql_query($pending_requests_query);
echo "<table border=1 cellspacing=0 cellpadding=1>
<tr><td colspan=7 align=center bgcolor=$fd_color_4>
	<font size=+2>Pending requests</font>
</td></tr>

<tr bgcolor=$fd_color_1><td>
	<i>Who</i>
</td><td>
	<i>Type</i>
</td><td>
	<i>Hours</i>
</td><td>
	<i>Time</i>
</td><td>
	<i>Status</i>
</td><td>
	&nbsp;
</td><td>
	&nbsp;
</td></tr>
";

while ($prow=@mysql_fetch_object($pending_res)) {
	$contact_info=getoneb("select * from contacts where contacts_id = '$prow->contacts_id'");
	echo "<tr><td>
			$contact_info->display_name
		</td><td title=\"$prow->ip_address\">
			$prow->fw_profile
		</td><td>
			$prow->requested_duration
		</td><td>
			$prow->request_time
		</td><td>
			$prow->status
		</td><td>
			<a href=$pagename?mode=$mode&report_name=$report_name&queue_id=$prow->queue_id&fw_profile=$prow->fw_profile><font color=blue>approve</font></a>
		</td><td>
			<a href=$pagename?mode=$mode&report_name=$report_name&queue_id=$prow->queue_id&fw_profile=www-dev-deny><font color=blue>deny</font></a>
		</td></tr>";
	}
}
echo "
</table>


<table border=0 bgcolor=$fd_color_2 cellspacing=0 cellpadding=5>
<tr><td valign=top>
<table  width=300px border=1 cellspacing=0 cellpadding=10>";


if ($current_lease) {
	echo "
	<tr><td bgcolor=$fd_color_4 align=center>
		<b>Firewall Access Status</b>
	</td></tr>
	
	<tr><td align=center>
		<img src=https://pipeline.umci.com/images/stoplight_green.png>
	</td></tr>
	
	<tr><td bgcolor=$fd_color_3 align=center>
		<i>Remote access is enabled until<br>" . date($pretty_format_string,strtotime($current_lease->request_time_end)) . ".</i>
	</td></tr>
	
	";
	} else {
	//<table  width=300px border=1 cellspacing=0 cellpadding=10>
	echo "
	<tr><td bgcolor=$fd_color_4 align=center>
		<b>Firewall Access Status</b>
	</td></tr>
	";
	if (!(is_trusted_address())) {
		echo "
		<tr><td align=center>
			<img src=https://pipeline.umci.com/images/stoplight_red.png>
		</td></tr>
		
		<tr><td bgcolor=$fd_color_3 align=center>
			<i>Remote access is disabled.</i>
		</td></tr>
		
		<tr><td align=center>
			<a href=$pagename?mode=" . urlencode($mode) . "&report_name=" . urlencode($report_name) . "&fw_profile=rdp><font color=blue>Request Access Now</font></a>
		</td></tr>";
		} else {
		echo "
		<tr><td align=center>
			<img src=https://pipeline.umci.com/images/stoplight_green.png>
		</td></tr>
		
		<tr><td bgcolor=$fd_color_3 align=center>
			<i>Remote access is enabled.</i>
		</td></tr>
		
		<tr><td bgcolor=$fd_color_6>
			Note, it is not necessary to come to this page first when connecting from inside UMC's network. Just use remote desktop to connect to <b>pipeline.umci.com</b>.<p>
			It *might* work to connect by clicking this <a href=$pagename/link.rdp?mode=$mode&report_name=$report_name&fw_profile=rdpfile&blah=blah/download/umc.rdp><font color=blue>link</font></a>. Your mileage may vary.
		</td></tr>";
		}
	echo "
	";
	}
echo "</table>
</td>
";




//echo "<hr>$rdp_connections_expired";
//$rdp_connections_expired="on";
//echo "<hr>$rdp_connections_expired";

//$adminuser=FALSE;
if (!($adminuser)) {
	echo "</tr></table>";
	
	} else {
	echo "<td valign=top>";
	$rdp_connections_expired=checkfix($rdp_connections_expired);
	$rdp_connections_fw_profile=addslashes($rdp_connections_fw_profile);
	
	
	$query_start="select * from front_desk_remote_access_requests";
	$query_end=" where 1 = 1";
	
	if ($rdp_connections_contacts_id!="") {
		$rdp_connections_contacts_id=addslashes($rdp_connections_contacts_id);
		$query_end=$query_end . " and contacts_id = '$rdp_connections_contacts_id'";
		}
	
	if ($rdp_connections_fw_profile!="") {
		$rdp_connections_fw_profile=addslashes($rdp_connections_fw_profile);
		$query_end=$query_end . " and fw_profile = '$rpd_connections_fw_profile'";
		}
	if ($rdp_connections_expired=="N") { $query_end=$query_end . " and expired = 'N'"; }
	
	if ($rdp_connections_start!="") {
		$rdp_connections_start=vali_date($rdp_connections_start);
		} else {
		$rdp_connections_start=date('Y-m-d',strtotime("14 days ago"));
		}
	$query_end=$query_end . " and (
		(date(request_time_end) >= '$rdp_connections_start')  or 
		
		(date(request_time_start) >= '$rdp_connections_start')
		)";

	if ($rdp_connections_end!="") {
		$rdp_connections_end=vali_date($rdp_connections_end);
		$query_end=$query_end . " and (
			(date(request_time_end) <= '$rdp_connections_end')  or 
			
			(date(request_time_start) <= '$rdp_connections_end')
			)";
		}
	
	$res=@mysql_query($query_start . $query_end . " order by request_time_end desc");
	
	echo "
	<form connection_list_selection method=post action=$pagename>
	<input type=hidden name=mode value=$mode>
	<input type=hidden name=report_name value=$report_name>
	<input type=hidden name=gui value=Y>
	
	
	
	
	
	<table border=1 cellspacing=0 cellpadding=2>
	<tr><td colspan=2 bgcolor=$fd_color_4 align=center>
		<b>Administrator Options</b>
	</td></tr>

	<tr><td colspan=2 bgcolor=$fd_color_3 align=center>
		<font size=-1><b>Vendor Access</b></font>
	</td></tr>
	
	
	<tr><td colspan=2>
		Grant <select name=fw_profile>
		<option></option>
		<option value=fx>Fieldcentrix</option>
		<option value=vp>Viewpoint</option>
		</select>&nbsp;access for <input type=text size=2 name=rdp_connections_vendor_hours> hours.
	</td></tr>
	
	<tr><td colspan=2>
		Reason/Description:<input type=text name=request_description size=20>
	</td></tr>
	
	<tr><td colspan=2 align=right><input type=submit value=Add></td></tr>
	


	<tr><td colspan=2>&nbsp;</td></tr>



	<tr><td colspan=6 bgcolor=$fd_color_3 align=center>
		<font size=-1><b>Display Filter</b></font>
	</td></tr>
	
	<tr><td>
		User:
	</td><td>
		";contact("select * from contacts where umc_emp = 'Y' and current = 'Y'",$rdp_connections_contacts_id,'rdp_connections_contacts_id');echo"
	</td></tr>
	
	<tr><td>
		Show&nbsp;Expired:
	</td><td>
		<input type=checkbox name=rdp_connections_expired $rpd_connections_expired>
	</td></tr>
	
	<tr><td>
		Start:
	</td><td>
		";datebox($rdp_connections_start,'rdp_connections_start');echo"
	</td></tr>
	
	<tr><td>
		End:
	</td><td>
		";datebox($rdp_connections_end,'rdp_connections_end');echo"
	</td></tr>
	
	</td><td>
		Firewall&nbsp;Profile:
	</td><td>
		<select name=rdp_connections_fw_profile>
		<option>$rdp_connections_fw_profile</option>
		<option>rdp</option>
		<option>viewpoint</option>
		<option>fx</option>
		<option></option>
		</select>
	</td></tr>
	
	<tr><td colspan=2 align=right>
		<input type=submit value=Search>
	</td></tr>
	</table>	



















	
</td></tr></table>

	
	<p>
	<table border=1 cellspacing=0 cellpadding=4>
	<tr bgcolor=$fd_color_4><td>
		<b>Name</b>
	</td><td>
		<b>Address</b>
	</td><td>
		<b>Profile</b>
	</td><td>
		<b>Start</b>
	</td><td>
		<b>End</b>
	</td><td>
		<b>Started</b>
	</td><td>
		<b>Expired</b>
	</td><td>
		<b>Action</b>
	</td><td>
		<b>Description</b>
	</td></tr>
	";
	$rdp_connections_expired=checkbreak($rdp_connections_expired);
	while ($row=@mysql_fetch_object($res)) {
		$user=getoneb("select * from contacts where contacts_id = '$row->contacts_id'");
		echo "<tr><td>
				$user->display_name
			</td><td>
				$row->ip_address
			</td><td title=\"$row->ip_tables_rule\">
				$row->fw_profile
			</td><td>
				<font size=-1>$row->request_time_start</font>
			</td><td>
				<font size=-1>$row->request_time_end</font>
			</td><td>
				$row->enabled
			</td><td>
				$row->expired
			</td><td>
				";
				if ($row->expired=='N') {
					echo "<a href=$pagename?mode=$mode&report_name=$report_name&rdp_connection_contacts_id=$rpd_connections_contacts_id&rdp_connections_start=$rdp_connections_start&rdp_connections_end=$rdp_connections_end&rdp_connections_expired=$rdp_connections_expired&rdp_connections_fw_profile=$rdp_connections_fw_profile&gui=Y&man_overboard=$row->request_id><font color=blue><i>expire&nbsp;now</i></font></a>";
					}
				echo"
				&nbsp;
			</td><td>
				$row->request_description&nbsp;
			</td></tr>";
		}
	echo "
	</table>
	</form>";
	
	
	
	
	}


























mh_navs_footer();
?>
