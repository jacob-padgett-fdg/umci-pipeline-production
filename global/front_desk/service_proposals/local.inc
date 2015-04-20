<?

function custom_csvdump($query,$header="1"){
$linefeedchar=chr(10);
$returnchar=chr(13);
$results=mssql_query($query);
if(!($results)) die ("Query Error: Could not run query:<br>$query");
	
$lastcust="";

header("Content-type: text/comma-seperated-file");
$width=mssql_num_fields($results);
while($row=(@mssql_fetch_row($results))) {
	$column=1;
	if ($row[1]==$lastcust) continue;
	$lastcust=$row[1];
	echo '"' . ereg_replace('"','""',ereg_replace($returnchar,"",ereg_replace($linefeedchar,"",$row[0]))) . '"';
	while ( $column < $width ){
		//if ($row[$column]=="") $row[$column]='""';
		echo "," . '"' . ereg_replace('"','""',ereg_replace($returnchar,"",ereg_replace($linefeedchar,"",$row[$column]))) . '"';
		$column++;
		}
	echo "";
	}
}

function moneytext($var) {
	$newvar="";
	$var=ereg_replace("\..*$","",$var);
	$length=strlen($var);
	
	$count=0;
	while($count<($length - 1)) {
		$count++;
		$newvar=$var[$length - $count] . $newvar;
		$mod=$count % 3;
		if (($count !=0) && ($mod==0)) {
			$newvar="," . $newvar;
			}
		}
	$newvar=$var[0] . $newvar;
	return '$' . $newvar;
	}

function proposal_info($proposal_id) {
	if ($proposal_id=="") {
		echo "ERROR in proposal info. Null record requested! Please contact your system administrator.";
		exit;
		}
	$proposal_id=escapeshellcmd($proposal_id);
	$query="select * from contacts,svc_proposals where contacts_id = sales_person and svc_proposals_id = '$proposal_id'";

	$prop_info=getoneb($query);
	if (!($prop_info)) { echo "Invalid Proposal: \"$proposal_id\"<p>"; exit; }
	
	$proposal_mm=ereg_replace('^[0-9]*-','',ereg_replace('-[0-9]*$','',$prop_info->proposal_date));
	$proposal_dd=ereg_replace('^.*-','',$prop_info->proposal_date);
	$proposal_yy=ereg_replace('-.*$','',$prop_info->proposal_date);
	$proposal_cnt=str_pad($prop_info->proposal_daily_count,2,'0',STR_PAD_LEFT);
	
	$prop_info->proposal_date_mysql=$prop_info->proposal_date;
	$prop_info->projected_bill_date_mysql=$prop_info->projected_bill_date_mysql;
	$prop_info->proposal_date=invali_date($prop_info->proposal_date);
	$prop_info->projected_bill_date=invali_date($prop_info->projected_bill_date);
	$prop_info->projected_bill_month=ereg_replace("-.*-..","/",$prop_info->projected_bill_date);
	$prop_info->proposal_number="$proposal_mm$proposal_dd$proposal_yy$prop_info->initials$proposal_cnt";

	$prop_info->probable_proposal_amount=$prop_info->proposal_amount * $prop_info->probability / 100;
	$prop_info->probable_proposal_amount_dollars=ereg_replace("\..*$","",$prop_info->probable_proposal_amount);

	$prop_info->estimated_margin_percent_weighted=$prop_info->estimated_margin_percent * $prop_info->proposal_amount;

	$prop_info->probability_weighted=$prop_info->probability * $prop_info->proposal_amount;
	$prop_info->probable_estimated_hours=$prop_info->estimated_hours * $prop_info->probability / 100;
	$prop_info->possible_estimated_profit=$prop_info->proposal_amount * $prop_info->estimated_margin_percent / 100;
	$prop_info->possible_estimated_profit_dollars=ereg_replace('\..*$','',$prop_info->possible_estimated_profit);
	$prop_info->probable_estimated_profit=$prop_info->possible_estimated_profit * $prop_info->estimated_margin_percent / 100;
	$prop_info->probable_estimated_profit_dollars=ereg_replace("\..*$","",$prop_info->probable_estimated_profit);
	$prop_info->probable_margin_weighted=$prop_info->estimated_margin_percent * $prop_info->probable_estimated_profit;
	return $prop_info;
}



function proplist($query) {
	//echo "$query";exit;
	//tabledump($query);exit;
	$bs1="#ffffff";
	$bs2="#dddddd";
	$bs3="#bbbbbb";

	$rs=0;
	if ($rs==0) { $rs=1; $cb_a="$bs1"; $cb_b="$bs2"; } else { $rs=0; $cb_a="$bs2"; $cb_b="$bs3"; }

	$res=@mysql_query($query);
	$rowcount=@mysql_num_rows($res);
	if ($rowcount==0) {
		echo "<hr><b>No Records Found....</b>";
		return 0;
		}

	echo "
	<table border=0 cellpadding=0 cellspacing=0 width=100%><tr><td width=25%>&nbsp;</td><td width=50% align=center><h2>Proposal Log</h2></td><td width=25% align=right><img src=umc_service_logo.png></td></tr></table>
	<table border=1 cellpadding=4 cellspacing=0 width=100%><tr><td bgcolor=#dddddd>
	<table border=0 cellspacing=0 cellpadding=6>
	<tr><td bgcolor='$cb_a'>Customer
	</td><td bgcolor='$cb_b' valign=top><font size=-2>Description</font>
	</td><td bgcolor='$cb_a' valign=top><font size=-2>Type</font>
	</td><td bgcolor='$cb_b' valign=top><font size=-2>Proposal Amount</font>
	</td><td bgcolor='$cb_a' valign=top><font size=-2>DM%</font>
	</td><td bgcolor='$cb_b' valign=top><font size=-2>Prob</font>
	</td><td bgcolor='$cb_a' valign=top><font size=-2>Est. Hours</font>
	</td><td bgcolor='$cb_b' valign=top><font size=-2>Proj Bill Month</font>
	</td><td bgcolor='$cb_a' valign=top><font size=-2>SP</font>
	</td><td bgcolor='$cb_b' valign=top><font size=-2>Status</font>
	</td><td bgcolor='$cb_a' valign=top><font size=-2>Prop Date</font>
	</td><td bgcolor='$cb_b' valign=top><font size=-2>Frcst Sales</font>
	</td><td bgcolor='$cb_a' valign=top><font size=-2>Frcst Margin</font>
	</td><td bgcolor='$cb_b' valign=top><font size=-2>W.O.</font>
	</td><td bgcolor='$cb_a' valign=top><font size=-2>Competition</font>
	</td><td bgcolor='$cb_b' valign=top><font size=-2>Prop&nbsp;#</font>
	</td></tr>
	";
	
	$totals->proposal_amount=0;
	$totals->probable_proposal_amount=0;
	$totals->estimated_margin_percent=0;
	$totals->estimated_margin_percent_weighted=0;
	$totals->probability=0;
	$totals->probability_weighted=0;
	$totals->estimated_hours=0;
	$totals->probable_estimated_hours=0;
	$totals->possible_estimated_profit=0;
	$totals->probable_estimated_profit=0;
	$totals->probabl_margin_weighted=0;
	$totals->record_count=0;
	$totals->dollars_won=0;
	$totals->dollars_not_won=0;

	while($row=@mysql_fetch_object($res)) {
		$temp=proposal_info($row->svc_proposals_id);
		if ($rs==0) { $rs=1; $cb_a="$bs1"; $cb_b="$bs2"; } else { $rs=0; $cb_a="$bs2"; $cb_b="$bs3"; }
		$temp->proposal_amount=ereg_replace('\..*','',$temp->proposal_amount);
		echo "
		<tr><td bgcolor='$cb_a'>
			<font size=-2>
			$temp->customer
			</font>
		</td><td bgcolor='$cb_b'>
			<font size=-2>
			$temp->description
			</font>
		</td><td bgcolor='$cb_a'>
			<font size=-2>
			$temp->type
			</font>
		</td><td bgcolor='$cb_b' align=right>
			<font size=-2>
			\$$temp->proposal_amount
			</font>
		</td><td bgcolor='$cb_a'>
			<font size=-2>
			$temp->estimated_margin_percent%
			</font>
		</td><td bgcolor='$cb_b'>
			<font size=-2>
			$temp->probability%
			</font>
		</td><td bgcolor='$cb_a'>
			<font size=-2>
			$temp->estimated_hours
			</font>
		</td><td bgcolor='$cb_b'>
			<font size=-2>
			$temp->projected_bill_month
			</font>
		</td><td bgcolor='$cb_a'>
			<font size=-2>
			<acronym title=\"$temp->first_name $temp->last_name\">$temp->initials</acronym>
			</font>
		</td><td bgcolor='$cb_b'>
			<font size=-2>
			";echo ereg_replace(" ","&nbsp;",$temp->status);echo"
			</font>
		</td><td bgcolor='$cb_a'>
			<font size=-2>
			";echo ereg_replace("200","0",ereg_replace("-","/",($temp->proposal_date)));echo"
			</font>
		</td><td bgcolor='$cb_b' align=right>
			<font size=-2>
			\$$temp->probable_proposal_amount_dollars
			</font>
		</td><td bgcolor='$cb_a' align=right>
			<font size=-2>
			\$$temp->possible_estimated_profit_dollars
			</font>
		</td><td bgcolor='$cb_b'>
			<font size=-2>
			$temp->workorder_num
			</font>
		</td><td bgcolor='$cb_a'>
			<font size=-2>
			$temp->beaten_by
			</font>
		</td><td bgcolor='$cb_b'>
			<font size=-2>
			<a href=$pagename?mode=proposal_edit&svc_proposals_id=$row->svc_proposals_id><font color=blue>$temp->proposal_number</font></a>
			</font>
		</td></tr>
		";

		
		$totals->proposal_amount=$totals->proposal_amount + $temp->proposal_amount;
		$totals->probable_proposal_amount=$totals->probable_proposal_amount + $temp->probable_proposal_amount;
		$totals->estimated_margin_percent=$totals->estimated_margin_percent + $temp->estimated_margin_percent;
		$totals->estimated_margin_percent_weighted=$totals->estimated_margin_percent_weighted + $temp->estimated_margin_percent_weighted;
		$totals->probability=$totals->probability + $temp->probability;
		$totals->probability_weighted=$totals->probability_weighted + $temp->probability_weighted;
		$totals->estimated_hours=$totals->estimated_hours + $temp->estimated_hours;
		$totals->probable_estimated_hours=$totals->probable_estimated_hours + $temp->probable_estimated_hours;
		$totals->possible_estimated_profit=$totals->possible_estimated_profit + $temp->possible_estimated_profit;
		$totals->probable_estimated_profit=$totals->probable_estimated_profit + $temp->probable_estimated_profit;
		$totals->probable_margin_weighted=$totals->probable_margin_weighted + $temp->probable_margin_weighted;
		if ($temp->status=="Won") $totals->dollars_won=$totals->dollars_won + $temp->proposal_amount;
		$totals->record_count++;
		}
	echo "</table></td></tr></table></body></html>";
	
	if ($totals->proposal_amount>0) $totals->dollars_won_percent=round(100 * ($totals->dollars_won / $totals->proposal_amount));
	$totals->probability_avg=round($totals->probability / $totals->record_count);
	if ($totals->proposal_amount>0) $totals->probability_weighted_avg=round($totals->probability_weighted / $totals->proposal_amount);
	$totals->estimated_margin_percent_avg=round($totals->estimated_margin_percent / $totals->record_count);
	if ($totals->proposal_amount>0) $totals->estimated_margin_percent_weighted_avg=round($totals->estimated_margin_percent_weighted / $totals->proposal_amount);
	$totals->proposal_amount_avg=moneytext($totals->proposal_amount / $totals->record_count);
	$totals->probable_proposal_amount=moneytext($totals->probable_proposal_amount);
	$totals->probable_margin_weighted_avg=@moneytext($totals->probable_margin_weighted / $totals->probable_estimated_profit);
	$totals->estimated_hours_avg=round($totals->estimated_hours / $totals->record_count);

	$totals->possible_estimated_profit_avg=moneytext($totals->possible_estimated_profit / $totals->record_count);
	$totals->probable_estimated_profit_avg=moneytext($totals->probable_estimated_profit / $totals->record_count);

	$totals->possible_estimated_profit=moneytext($totals->possible_estimated_profit);
	$totals->probable_estimated_profit=moneytext($totals->probable_estimated_profit);

	$totals->proposal_amount=moneytext($totals->proposal_amount);
	
	$totals->dollars_won=moneytext(round($totals->dollars_won));


	echo "
	
	<p>Total proposals: $totals->record_count<p>

	<table border=1>
	
	<tr><td colspan=2 bgcolor=#dddddd align=center><b>Summary Information</b></td></tr>
	<tr><td width=50%>
		<b>Total Proposal Amount</b>
	</td><td width=50%>
		 $totals->proposal_amount
	</td></tr>	

	<tr><td>
		<b>Total Won Amount</b>
	</td><td>
		$totals->dollars_won
	</td></tr>

	<tr><td>
		<b>Won Percentage (by dollars)</b>
	</td><td>
		$totals->dollars_won_percent%
	</td></tr>

	<tr><td>
		<b>Proposal Avg</b>
	</td><td>
		$totals->proposal_amount_avg
	</td></tr>
	
	<tr><td>
		<b>Avg margin</b>
	</td><td>
		$totals->estimated_margin_percent_avg%
	</td></tr>
	
	<tr><td>	
		<b>Avg Probability</b>
	</td><td>
		$totals->probability_avg%
	</td></tr>
	
	<tr><td>
		<b>Total Estimated Hours</b>
	</td><td>
		$totals->estimated_hours
	</td></tr>
	
	<tr><td>
		<b>Avg Estimated Hours</b>
	</td><td>
		$totals->estimated_hours_avg
	</td></tr>
	
	<tr><td>
		<b>Total Probable Hours</b>
	</td><td>
		$totals->probable_estimated_hours
	</td></tr>

	<tr><td>
		<b>Total Direct Margin</b>
	</td><td>
		$totals->possible_estimated_profit
	</td></tr>
	
	<tr><td>
		<b>Total Direct Margin Avg</b>
	</td><td>
		$totals->possible_estimated_profit_avg
	</td></tr>
	
	<tr bgcolor=#dddddd><td align=center>
		<b>Forecasted Sales</b>
	</td><td align=center>
		<b>Forecasted Margin</b>
	</td></tr>
	
	<tr><td align=center>
		$totals->probable_proposal_amount
	</td><td align=center>
		$totals->probable_estimated_profit
	</td></tr>
	</table>
	";
	}
?>
