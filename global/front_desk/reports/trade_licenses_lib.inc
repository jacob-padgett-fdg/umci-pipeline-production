<?
function check_for_missing_licenses() {
	global $global_user_id;
	$typelist=@mysql_query("select * from front_desk_trade_license_types where required_by_trade != ''");
	while ($type=@mysql_fetch_object($typelist)) {
		$query="select * from contacts where umc_emp = 'Y' and current = 'Y' and employee_trade = '$type->required_by_trade'";
		$emplist_res=@mysql_query($query);
		while ($emprow=@mysql_fetch_object($emplist_res)) {
			if (!(getoneb("select * from front_desk_trade_licenses where license_type_id = '$type->license_type_id' and employee_id = '$emprow->contacts_id' limit 1"))) {
				@mysql_query("insert into front_desk_trade_licenses set license_type_id = '$type->license_type_id', employee_id = '$emprow->contacts_id', expiration_date = '1980/5/18', creator = '$global_user_id'");
				}
			}
		}
	}

function show_next_20($filter="") {
	global $fd_color_4;
	if ($filter!="") $filter="and " . $filter;
	//if ($filter!="") die($filter);
	//die($filter);
	$query="select * from front_desk_trade_licenses left join contacts on (employee_id = contacts_id) left join front_desk_trade_license_types on (front_desk_trade_license_types.license_type_id = front_desk_trade_licenses.license_type_id) where current = 'Y' $filter and expiration_date != '1980/05/18' order by expiration_date,alphasort limit 20";
	$res=@mysql_query($query);
	echo "<table border=1 cellspacing=0 cellpadding=4>
	<tr bgcolor=$fd_color_4 align=center><td colspan=4><h3>Next 20 expiring licenses</h3></td></tr>
	<tr bgcolor=$fd_color_4 align=center><td>
		<b>Employee</b>
	</td><td>
		<b>License Type</b>
	</td><td>
		<b>Expires</b>
	</td><td>
		&nbsp;
	</td></tr>";
	while ($row=@mysql_fetch_object($res)) {
		echo "<tr><td>
				$row->display_name
			</td><td>
				$row->name
			</td><td>
				" . invali_date($row->expiration_date,'/') . "
			</td><td>
				<a href=$pagename?mode=report_show&report_name=trade_license_edit&license_id=$row->license_id><font color=blue><i>edit</i></font></a>
			</td></tr>";
		}
	//tabledump($query);
	echo "</table>";
	
	}
?>
