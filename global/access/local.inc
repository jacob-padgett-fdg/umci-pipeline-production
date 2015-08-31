<?

function server_report_html_full ($server_id,$log_entry_count) {
	$server_id=addslashes($server_id);
	if ($log_entry_count!="") $log_entry_count=addslashes($log_entry_count);
	$server_info=getone("select * from access_servers where server_id = '$server_id'");
	$os_info=getoneb("select * from access_servers_oslist where os_id = '$server_info->os_id'");
	$storage_type="unreliable";
	if ($server_info->reliable_storage=='Y') $storage_type="reliable";
	$storage_text="$server_info->storage_capacity $storage_type";
	$bu_style_info=getone("select * from access_servers_backup_styles where backup_style_id = '$server_info->backup_style_id'");
	$sptext="";
	if ($server_info->service_pack > 0) $sptext=" SP $server_info->service_pack";
	echo "<center><font size=+2><b>$server_info->name</b></font><hr>$server_info->description</center>
	<p>
	<table border=0>
	<tr><td align=right>
		<b>Name&nbsp;Origin:</b>
	</td><td>
		$server_info->name_description
	</td></tr>
	
	<tr><td align=right>
		<b>Admin&nbsp;Account:</b>
	</td><td>
		$server_info->admin_account
	</td></tr>
	
	<tr><td align=right>
		<b>Admin&nbsp;Password:</b>
	</td><td>
		$server_info->admin_password
	</td></tr>
	
	<tr><td align=right>
		<b>Operating&nbsp;System:</b>
	</td><td>
		$os_info->name$sptext
	</td></tr>
	
	<tr><td align=right>
		<b>IP&nbsp;Address:</b>
	</td><td>
		$server_info->ip_addr
	</td></tr>
	
	<tr><td align=right>
		<b>Netmask:</b>
	</td><td>
		$server_info->ip_mask
	</td></tr>
	
	<tr><td align=right>
		<b>Storage:</b>
	</td><td>
		$storage_text
	</td></tr>
	
	<tr><td align=right>
		<b>Remote&nbsp;Admin&nbsp;Method:</b>
	</td><td>
		$server_info->remote_admin_method
	</td></tr>
	
	<tr><td align=right>
		<b>Backup&nbsp;Method:</b>
	</td><td>
		$bu_style_info->name
	</td></tr>
	
	<tr><td align=right>
		<b>BU&nbsp;Procedure:</b>
	</td><td>
		$bu_style_info->bu_procedure
	</td></tr>
	
	<tr><td align=right>
		<b>BU&nbsp;Notes:</b>
	</td><td>
		$server_info->backup_notes
	</td></tr>
	
	<tr><td align=right>
		<b>Active:</b>
	</td><td>
		$server_info->active&nbsp;&nbsp;&nbsp;<i><font size=-1>(N=\"server no longer exists\")</font></i>
	</td></tr>
	
	<tr><td align=right>
		<b>In&nbsp;Production:</b>
	</td><td>
		$server_info->in_production
	</td></tr>
	
	<tr><td align=right>
		<b>Job&nbsp;Site&nbsp;Server:</b>
	</td><td>
		$server_info->job_site_server
	</td></tr>
	
	";
	
	if ($server_info->job_site_server=='Y') {
		echo "
		<tr><td align=right>
			<b>VPN&nbsp;IP:</b>
		</td><td>
			$server_info->vpn_ip
		</td></tr>
		
		<tr><td align=right>
			<b>VPN&nbsp;Username:</b>
		</td><td>
			$server_info->vpn_username
		</td></tr>
		
		<tr><td align=right>
			<b>VPN&nbsp;Password:</b>
		</td><td>
			$server_info->vpn_password
		</td></tr>
		
		<tr><td align=right>
			<b>Remote&nbsp;Network:</b>
		</td><td>
			$server_info->remote_network
		</td></tr>
		
		<tr><td align=right>
			<b>Job&nbsp;Site&nbsp;Notes:</b>
		</td><td>
			$server_info->job_site_notes
		</td></tr>
		";
		}
	
	//tabledump("select * from access_service_types,access_servers_services where access_service_types.service_type_id = access_servers_services.service_type_id and server_id = '$server_info->server_id' and active = 'Y' order by name");
	$servicesres=@mysql_query("select * from access_service_types,access_servers_services where access_service_types.service_type_id = access_servers_services.service_type_id and server_id = '$server_info->server_id' and active = 'Y' order by name");
	echo "
	<tr><td colspan=2>
		<b><font size=+1>Services&nbsp;Provided</font></b>
	</td></tr>
	";
	while ($service_info=@mysql_fetch_object($servicesres)) {
		echo "
		<tr><td align=right valign=top>
			<b>$service_info->name:</b>
		</td><td>
			$service_info->description
		</td></tr>
		";
		if ($service_info->config_details!="") {
			echo "
			<tr><td align=right>
				&nbsp;
			</td><td>
				<b>Config&nbsp;Details:</b><br>
				<pre>$service_info->config_details</pre>
			</td></tr>
			";
			}
		
		}
	echo "
	<tr><td colspan=2>
		<b><font size=+1>Recent&nbsp;History ($log_entry_count)</font></b>
	</td></tr>
	";
	$hist_res=@mysql_query("select * from access_servers_log where server_id = '$server_info->server_id' order by last_touched desc limit $log_entry_count");
	while ($histrow=@mysql_fetch_object($hist_res)) {
		$tech_info=getone("select * from contacts where contacts_id = '$histrow->tech'");
		$service_info=getoneb("select * from access_service_types where service_type_id = '$histrow->service_type_id'");
		echo "<tr bgcolor=#eeeeee><td align=right valign=top>
				<table border=1 cellspacing=0 cellpadding=1 width=100%><tr><td>$tech_info->display_name<br>$histrow->entry_date<br>$service_info->name</td></tr></table>
			</td><td valign=top>
				<table border=0 cellspacing=0 cellpadding=1 width=100%><tr><td><pre>$histrow->description</pre></td></tr></table>
			</td></tr>
				";
		
		}
	
	echo "</table><br style=\"page-break-after:always;\">";
	}

function server_report_html_medium ($server_id) {
	echo "<hr>report detail level not implimented yet.. please use 'full' for now.<hr>";
	return 0;
	}

function server_report_html_terse ($server_id) {
	echo "<hr>report detail level not implimented yet.. please use 'full' for now.<hr>";
	return 0;
	}

function server_log_interface($server_id,$list_length=10) {
	global $mode,$global_user;
	if ($server_id<1) return 0;
	if ($list_length==0) $limit_line="";
	else $limit_line=" limit $list_length ";
	
	$server_id=addslashes($server_id);
	$server_info=getone("select * from access_servers where server_id = '$server_id'");
	//echo "server_id: $server_id";
	
	$log_query="select * from access_servers_log where server_id = '$server_info->server_id' order by entry_date desc, last_touched desc $limit_line";
	//tabledump($log_query);
	echo "
	<form name=log_entry method=post action=$pagename>
	<input type=hidden name=mode value=server_log_submit>
	<input type=hidden name=return_mode value=\"$mode\">
	<input type=hidden name=server_id value=\"$server_info->server_id\">
	<table border=1 cellspacing=0 cellpadding=4>
	<tr bgcolor=#dddddd><td>
		<b>Tech</b>
	</td><td>
		<b>Service</b>
	</td><td>
		<b>Change/Comment</b>
	</td><td>
		<b>Date</b>
	</td></tr>
	
	<tr><td valign=top>
		<b>$global_user->login_name</b>
	</td><td valign=top>";
		dropbox("select access_service_types.service_type_id,name from access_servers_services,access_service_types where access_service_types.service_type_id = access_servers_services.service_type_id and server_id = '$server_info->server_id' order by name",'','','',1);echo"
	</td><td valign=top>
		<textarea name=description rows=3 cols=40></textarea>
	</td><td>
		<input type=submit>
	</td></tr>
	</form>
	";
	
	$logres=@mysql_query($log_query);
	while ($logrow=@mysql_fetch_object($logres)) {
		$tech_info=getoneb("select * from contacts where contacts_id = '$logrow->tech'");
		$service_type=getoneb("select * from access_service_types where service_type_id = '$logrow->service_type_id'");
		$logdate=invali_date($logrow->entry_date);
		if ($service_type->name == "") $service_type->name="N/A";
		$logrow->description=ereg_replace("\r","<br>",$logrow->description);
		$logrow->description=ereg_replace("\n","<br>",$logrow->description);
		$logrow->description=ereg_replace("<br><br>","<br>",$logrow->description);
		echo "<tr><td>
				$tech_info->login_name
			</td><td>
				$service_type->name
			</td><td>
				$logrow->description
			</td><td>
				$logdate
			</td></tr>
			";
		}
	echo "</table>";
	}	

function group_members_link($server_id,$group_name) {
	if ($GLOBALS['group_members_show_js'] != "done") {
		$GLOBALS['group_members_show_js']="done";
		
		echo "
		<script>
		function group_members_show(server_id,group_name) {
			javascript:open('$pagename?mode=group_members_show&server_id=' + server_id + '&group_name=' + group_name,'server_group_members','width=400,height=700,scrollbars=1')
			}
		</script>
		";
		}
	echo "<a href=\"javascript:group_members_show('$server_id','$group_name')\"><font color=blue>$group_name</font></a>";
	}

?>
