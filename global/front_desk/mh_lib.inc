<? 
function mh_navs_header($options="") {
	global $devel,$fd_color_4,$fd_color_1,$mode,$application_name,$REQUEST_URI,$report_name,$global_user_id,$global_browser;
	$selectedcolor="white";
	$height_drop_factor=4;
	$my_home_color=$fd_color_1;
	if ($application_name=="front_desk") {
		$my_home_color=$selectedcolor;
		if (($mode=="") || ($mode=="main")) $my_home_color=$fd_color_1;
		}
	if (isset($options['background']) && $options['background']!="") $background_image=$options['background'];
	else $background_image="url(/images/fd_watermark.jpg)";
	if ($GLOBALS['HTTP_HOST']=="pipeline-devel.umci.com") { 
		$background_image="url(/images/fd_watermark_devel.jpg)";
		}
	
	
	ajax_load();
	echo "
	<div style=\"position: absolute; z-index: 2; width: 350px; display: none; background: white; border: 1px solid black;\" id=\"my_jobs_drop\" onmouseout=\"hide_my_jobs();\" onmouseover=\"show_my_jobs();\"><b>Loading List..</b><br>please wait</div>
	<div style=\"position: absolute; z-index: 2; width: 350px; display: none; background: white; border: 1px solid black;\" id=\"recent_jobs_drop\" onmouseout=\"hide_recent_jobs();\" onmouseover=\"show_recent_jobs();\"><b>Loading List..</b><br>please wait</div>
	                                            
	
	<table style=\"border-collapse: collapse; border: 1px solid black;\" cellspacing=0 width=100% cellpadding=0>
	<tr class=noprint bgcolor=\"$fd_color_1\"><td class=noprint id=\"fd_fd_nav_button\" style=\"border: 1px solid black;\" align=center><a href=\"/global/front_desk$devel?mode=my_home\"><font color=blue><b>My&nbsp;Home</b></font></a></td><td colspan=2 style=\"border-bottom: 1px solid black;\" class=noprint id=fd_top_nav>";fd_print_global_nav();echo"</td></tr>
	
	";
	//if ($mode!='my_home') $app_menu_html="<a href=$pagename?mode=my_home><font color=blue>My&nbsp;Home&nbsp;Main</font></a>";
	//else $app_menu_html="&nbsp;";
	
	echo "
	<tr><td class=noprint colspan=2 style=\"height: 20px;\">$app_menu_html</td></tr>
	
	<tr>
		<td colspan=3 id=mh_main_application_window style=\"background: $background_image; background-repeat: repeat;\" valign=top align=left>
	<script>
	document.getElementById('fd_top_nav').ajax_trigger='never';
	document.fd_menus_positioned_already=0;

	function save_bookmark(id) {
		text=document.getElementById('link_text' + id);
		url=document.getElementById('link_url' + id);
		new_win=document.getElementById('new_window' + id);
		//alert(new_win.checked);
		//return 0;
		if (new_win.checked) new_window='Y';
		else new_window='N';
		
		if (url.value!='') {
			url_done=encodeURI(url.value);
			text_done=encodeURI(text.value);
			ajaxManager('load_page','$pagename?mode=my_bookmarks&update_bookmark=true&link_text=' + text_done + '&link_url=' + url_done + '&link_id=' + id + '&new_window=' + new_window,'my_bookmarks');
			} else {
			alert('Please enter something into the URL (web address) field.');
			}
		}
	
	function delete_bookmark(id) {
		if (id=='') return (FALSE);
		if (confirm('Are you sure you want to delete this bookmark?'))
			ajaxManager('load_page','$pagename?mode=my_bookmarks&delete_bookmark=true&link_id=' + id,'my_bookmarks');
		}
	
	function show_bookmark_form(id) {
		document.getElementById('bookmark_form_div' + id).style.display='block';
		document.getElementById('link_text' + id).focus();
		}
	
	function open_job_search() {
         open('/global/front_desk$devel/?mode=job_search','job_search','width=400,height=600,resizable=1,scrollbars=1,top=0,left=300'); 
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
	

	function fix_fd_height() {
		var x,y;
		if (self.innerHeight) {
			// Most Browsers
			x = self.innerWidth;
			y = self.innerHeight;
			document.getElementById('mh_main_application_window').style.height=(y - 80) + 'px';
		} else if (document.documentElement && document.documentElement.clientHeight) {
			// IE6
			x = document.documentElement.clientWidth;
			y = document.documentElement.clientHeight;
		}
	";
	
	if ($global_browser!="IE") {
		echo "
		apptable=document.getElementById('sub_application_main_table');
		if (apptable) {
			apptable.style.height=(y - 250) + 'px';
			}
		";
	}
	echo "
	}

	document.getElementById('my_jobs_drop').ajax_trigger='never';
	document.getElementById('recent_jobs_drop').ajax_trigger='never';
	</script>
	";
	}

function mh_navs_footer() {
	global $devel,$doc_id,$docid,$fd_color_4,$REQUEST_URI,$report_name,$global_user_id,$mode,$REMOTE_ADDR,$application_name;
	echo "</td></tr></table>
	<script>
	fix_fd_height();
	ajaxManager('load_page','/global/front_desk$devel?mode=my_jobs_menu','my_jobs_drop');
	ajaxManager('load_page','/global/front_desk$devel?mode=recent_jobs_menu','recent_jobs_drop');
	";
	if ($doc_id>0) {
		echo "
		docinfo=document.getElementById('fd_doc_tools');
		docinfo.ajax_trigger='never';
		docinfo.style.background='$fd_color_4';
		docinfo.style.border='2px solid black';
		ajaxManager('load_page','/global/front_desk$devel?mode=fd_doc_tools_main&doc_id=$doc_id','fd_doc_tools');";
		} else {
		if ($mode=="report_show") {
			$page_type='report';
			$name=$report_name;
			} else {
			$page_type='page';
			$name=$mode;
			}
		$date_as_int=date('Ymd');
		$activity_query="insert into front_desk_page_history set 
		page_type = '$page_type', application = '$application_name', name = '$name', 
		contacts_id = '$global_user_id', jobinfo_id = '$global_jobinfo_id', 
		review_time = now(), date_as_int = '$date_as_int', additional_info = '$REQUEST_URI', ip_address = '$REMOTE_ADDR'";
		@mysql_query($activity_query);
		}
	echo"</script>";
	}
function mh_load_navs() {
	global $application_name,$global_jobinfo_id,$devel,$mode;
	ajax_load();
	echo "<script>ajaxManager('load_page','/global/front_desk$devel?mode=top_nav&jobinfo_id=$global_jobinfo_id','fd_top_nav');ajaxManager('load_page','/global/front_desk$devel?mode=left_nav&current_app=$application_name&jobinfo_id=$global_jobinfo_id&actualmode=$mode','fd_left_nav');</script>";
	}
?>
