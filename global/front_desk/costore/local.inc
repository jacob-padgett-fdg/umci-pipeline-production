<?
$today=date('m/d/Y');
function admin_list($query,$text) {
	$query=ereg_replace(" where "," left join contacts on (contacts.contacts_id = costore_cart_invoice.contacts_id) where ", $query);
	$query=$query . " order by alphasort, invoice_id ";
	//tabledump($query);
	$res=@mysql_query($query);
	$rowcount=@mysql_num_rows($res);
	if ($rowcount<1) return FALSE;
	//$res=@mysql_query("select * from costore_cart_invoice left join contacts on (contacts.contacts_id = costore_cart_invoice.contacts_id) where cart_status = 'processing' order by alphasort, invoice_id");
	echo "<h3>$text...</h3><table border=0 cellspacing=0 cellpadding=4><tr><td><b>Invoice #</b></td><td><b>Employee</b></td><td><b>Invoice Date</b></td><td><b>Items</b></td><td><b>Total</b></td><td><b>Job</b></td></tr>";

	while ($row=@mysql_fetch_object($res)) {
		//if ($row->billing_jobinfo_id>0) $bjinfo=getoneb("select * from jobinfo where jobinfo_id = '$row->billing_jobinfo_id'");
		//else $bjinfo="";
		$job_items=getoneb("select * from costore_cart where jobinfo_id != 0 and invoice_id = '$row->invoice_id' limit 1");
		if ($job_items) $job_display='<b>Y</b>';
		else $job_display='<font color="#aaaaaa">N</font>';
		
		$row->invoice_date=invali_date($row->invoice_date,'/');
		//$job_display="";
		//if ($bjinfo!="") $job_display="<a href=https://pipeline.umci.com/global/front_desk-devel/index.html?mode=main&global_jobinfo_id_set=260><font color=blue>$bjinfo->contract_num&nbsp;($bjinfo->job_name)</font></a>";
		echo "<tr><td>
				&nbsp;&nbsp;&nbsp;<a href=$pagename?mode=invoice_manage&invoice_id=$row->invoice_id>$row->invoice_id</a>
			</td><td>
				$row->first_name $row->last_name
			</td><td>
				$row->invoice_date
			</td><td>
				$row->cart_count
			</td><td>
				$row->cart_total
			</td><td>
				$job_display
			</td></tr>";
		}
	echo "</table>";
	}





?>
