<?php
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                                                       Function Form Show


function show_forms()
{	
	global $wpdb;
	?>
    <form method="post" action="admin.php?page=Form_maker" id="main_show_form">
	<table cellspacing="10" width="100%">
    <tr>
    <td style="width:130px; height:50px; padding-left:65px; background:url('<?php echo plugins_url( 'images/formmakerLogo-48.png' , __FILE__ ); ?>') no-repeat;">
    <?php $Forms_title='Form Maker'; echo "<h2>".__('Form Maker'). "</h2>"; ?>
    </td>
    <td  style="width:90px; text-align:right;"><p class="submit" style="padding:0px;"><input type="button" value="Add a Form" name="custom_parametrs" onclick="window.location.href='admin.php?page=Form_maker&task=add'" /></p></td>
<td style="text-align:right;font-size:16px;padding:20px; padding-right:50px">
		<a href="http://web-dorado.com/files/fromFormMaker.php" target="_blank" style="color:red; text-decoration:none;">
		<img src="<?php echo plugins_url( 'images/header.png' , __FILE__ ); ?>" border="0" alt="www.web-dorado.com" width="215"><br>
		Get the full version&nbsp;&nbsp;&nbsp;&nbsp;
		</a>
	</td>
    </tr>
    </table>
    <?php
	/////////////////////////////////////////////////////
	$sql_text="";//text for filtring 
		 if(isset( $_POST['search_events_by_title']))
		 {
			 if($_POST['search_events_by_title']!="")
			 {
			 $sql_text=' WHERE title LIKE "%'.$_POST['search_events_by_title'].'%" ';
			 }
		 }

		 $i=0;//counts elements intu spiderfc_events;
		 $ids_SpiderFC=$wpdb->get_col("SELECT id FROM  ".$wpdb->prefix."formmaker  ".$sql_text,0);
		foreach($ids_SpiderFC as $id)
		{
			$i=$i+1;
		}
		if($i%20>0)
		$pages_count=($i-$i%20)/20+1;
		else
		$pages_count=$i/20;
		
 if(isset($_POST['page_number'])){
	 if($_POST['page_left_or_right']==-2)
 {
	 $page_number=1;
 }
	 if($_POST['page_left_or_right']==-1)
	 {
	 if($_POST['page_number']>1)
	 $page_number=$_POST['page_number']-1;
	 }
	 if($_POST['page_left_or_right']==1)
	 {
	 if($_POST['page_number']<$pages_count)
	 $page_number=$_POST['page_number']+1;
	 else 
	 $page_number=$_POST['page_number'];
	 }
	 if($_POST['page_left_or_right']==2)
	 {
	 $page_number=$pages_count;
	 }
	 if($page_number<=0)
	 {
		 $page_number=1;
	 }
	 if($page_number==($i-$i%20)/20+1 && $i>20)
	 {
		 $enable_disable_for_next_page='disabled';
	 }
	 else
	 {
		 $enable_disable_for_next_page='';
	 }
	  if($page_number=='1')
	  { $enble_disable='disabled';}
	   else {$enble_disable='';}
	   }
	    else {
			$page_number=1;
 			$enble_disable='disabled';
	}
	$first_page ='first-page '.$enble_disable;
	$prev_page = 'prev-page '.$enble_disable;
	$next_page='next-page '.$enable_disable_for_next_page;
	$last_page='last-page '.$enable_disable_for_next_page;
 
 
		?>
         
                 <script type="text/javascript">
				 function clear_serch_texts()
				 {
					 	 document.getElementById("search_events_by_title").value='';					 
				 }
				 function submit_href(x,y)
				 {
					 if(document.getElementById("serch_or_not").value!="search")
					 {
						clear_serch_texts();
					 }
					 document.getElementById('page_number').value=x;
					 document.getElementById('page_left_or_right').value=y;
					 document.getElementById('main_show_form').submit();					 
				 }
				 function clear_search()
				 {
					 document.getElementById("serch_or_not").value="";
				 }
				 
				 </script>


<div class="tablenav top" style="width:95%">
	<div class="alignleft actions" style="width:180px;">
    	<label for="search_events_by_title" style="font-size:14px">Title: </label>
        <input type="text" name="search_events_by_title" value="<?php if(isset($_POST['search_events_by_title'])) echo $_POST['search_events_by_title']; ?>"  id="search_events_by_title" onchange="clear_search()" />
    </div>
	<div class="alignleft actions">
   		<input type="button" value="Search" onclick="document.getElementById('page_number').value='1';document.getElementById('serch_or_not').value='search'; document.getElementById('main_show_form').submit();" class="button-secondary action" /><input type="button" value="Reset" onclick="window.location.href='admin.php?page=Form_maker'" class="button-secondary action" />
    </div>
	<div class="tablenav-pages">
    	<span class="displaying-num"><?php echo $i; ?> items</span>
		<?php if($i>20) {?> 
		<span class="pagination-links">
		<a class="<?php echo $first_page; ?>" title="Go to the first page" href="javascript:submit_href(<?php echo $page_number; ?>,-2);">«</a>
		<a class="<?php echo $prev_page; ?>" title="Go to the previous page" href="javascript:submit_href(<?php echo $page_number; ?>,-1);">‹</a>
			<span class="paging-input">
			<span class="total-pages"><?php echo $page_number; ?></span>
			of <span class="total-pages">
			<?php echo ($i-$i%20)/20+1; ?>
			</span>
		</span>
		<a class="<?php echo $next_page ?>" title="Go to the next page" href="javascript:submit_href(<?php echo $page_number; ?>,1);">›</a>
		<a class="<?php echo $last_page ?>" title="Go to the last page" href="javascript:submit_href(<?php echo $page_number; ?>,2);">»</a>
		<?php }
		
		
		 ?>
		</span>
	</div>

</div>


    
    
    
    <?php
	
	if(isset($_POST['asc_or_desc_by']))
{
	$sql_order='';
	if($_POST['asc_or_desc_by']=='id')	
	{
		if($_POST['asc_or_desc']==1)
		{
			$sql_order=' ORDER BY id ASC';
			$style_class_title="manage-column column-title sortable desc";
			$style_class_id="manage-column column-autor sorted asc";
			$style_class_Email="manage-column column-autor sortable desc";
			$sort_title=1;
			$sort_id=2;
			$sort_Email=1;
		 
		}
		if($_POST['asc_or_desc']==2)
		{
			$sql_order=' ORDER BY id DESC';
			$style_class_title="manage-column column-title sortable desc";
			$style_class_id="manage-column column-autor sorted desc";
			$style_class_Email="manage-column column-autor sortable desc";
			$sort_title=1;
			$sort_id=1;
			$sort_Email=1;
		}
	}
		if($_POST['asc_or_desc_by']=='title')
		{
		
		if($_POST['asc_or_desc']==1)
		{
			$sql_order=' ORDER BY title ASC';
			$style_class_title="manage-column column-title sorted asc";
			$style_class_id="manage-column column-autor sortable desc";
			$style_class_Email="manage-column column-autor sortable desc";
			$sort_title=2;
			$sort_id=1;
			$sort_Email=1;
		 
		}
		 if($_POST['asc_or_desc']==2)
		 {
		 	$sql_order=' ORDER BY title DESC';
		 	$style_class_title="manage-column column-title sorted desc";
			$style_class_id="manage-column column-autor sortable desc";
			$style_class_Email="manage-column column-autor sortable desc";
			$sort_title=1;
			$sort_id=1;
			$sort_Email=1;
		 }	

		
	}
		if($_POST['asc_or_desc_by']=='mail')	
	{
		if($_POST['asc_or_desc']==1)
		{
			$sql_order=' ORDER BY mail ASC';
			$style_class_title="manage-column column-title sortable desc";
			$style_class_id="manage-column column-autor sortable desc";
			$style_class_Email="manage-column column-autor sorted asc";
			$sort_title=1;
			$sort_id=1;
			$sort_Email=2;
		 
		}
		 if($_POST['asc_or_desc']==2)
		 {
		 	$sql_order=' ORDER BY mail DESC';
		 	$style_class_title="manage-column column-title sortable desc";
			$style_class_id="manage-column column-autor sortable desc";
			$style_class_Email="manage-column column-autor sorted desc";
			$sort_title=1;
			$sort_id=1;
			$sort_Email=1;
		 }	
	}
	if(!($_POST['asc_or_desc_by']=='id' || $_POST['asc_or_desc_by']=='title' || $_POST['asc_or_desc_by']=='mail'))
	{
		$style_class_title="manage-column column-title sortable desc";
		$style_class_id="manage-column column-autor sortable desc";
		$style_class_Email="manage-column column-autor sortable desc";
		$sort_title=1;
		$sort_id=1;
		$sort_Email=1;		
	}
	
}
else
{
	$style_class_title="manage-column column-title sortable desc";
	$style_class_id="manage-column column-autor sortable desc";
	$style_class_Email="manage-column column-autor sortable desc";
	$sort_title=1;
	$sort_id=1;
	$sort_Email=1;
}
 ?>
 <script type="text/javascript">
 function clear_serch_texts()
				 {
					 	 document.getElementById("search_events_by_title").value='';				 
				 }
				 function submit_href(x,y)
				 {
					 if(document.getElementById("serch_or_not").value!="search")
					 {
						clear_serch_texts();
					 }
					 document.getElementById('page_number').value=x;
					 document.getElementById('page_left_or_right').value=y;
					 document.getElementById('main_show_form').submit();					 
				 }
				 function clear_search()
				 {
					 document.getElementById("serch_or_not").value="";
				 }
 function ordering(x,y)
 {
	 if(document.getElementById("serch_or_not").value!="search")
		 {
			clear_serch_texts();
		 }
	document.getElementById('asc_or_desc_by').value=x;
	document.getElementById('asc_or_desc').value=y;
	document.getElementById('main_show_form').submit();
 }
 	function confirmation(href,title) {
		var answer = confirm("Are you sure you want to delete '"+title+"'?")
		if (answer){
			document.getElementById('main_show_form').action=href;
			document.getElementById('main_show_form').submit();
		}
	}
 
 </script>
  <input type="hidden" name="serch_or_not" id="serch_or_not" value="<?php echo $_POST['serch_or_not']; ?>" />
  <input type="hidden" id="page_number" name="page_number" value="<?php echo $page_number; ?>"/>
  <input type="hidden" id="page_left_or_right" name="page_left_or_right" value=""/>
  <input type="hidden" id="asc_or_desc_by" name="asc_or_desc_by" value="<?php echo $_POST['asc_or_desc_by']; ?>"/>
  <input type="hidden" id="asc_or_desc" name="asc_or_desc" value="<?php echo $_POST['asc_or_desc']; ?>"/> 
  <table class="wp-list-table widefat fixed pages" style="width:95%">
 <thead>
 <TR>
 <th scope="col" id="id" class="<?php echo $style_class_id; ?>" style=" width:120px" ><a href="javascript:ordering('id',<?php echo $sort_id ?>)"><span>Id</span><span class="sorting-indicator"></span></a></th>
 <th scope="col" id="title" class="<?php echo $style_class_title; ?>" style="" ><a href="javascript:ordering('title',<?php echo $sort_title ?>)"><span>Title</span><span class="sorting-indicator"></span></a></th>
 <th scope="col" id="mail" class="<?php echo $style_class_Email; ?>" style="" ><a href="javascript:ordering('mail',<?php echo $sort_Email?>)"><span>Email to send submissions to</span><span class="sorting-indicator"></span></a></th>
 <th style="width:80px">Edit</th>
 <th style="width:80px">Delete</th>
 </TR>
 </thead>
 <tbody>
 
 <?php  
  $limi[0]=($page_number-1)*20;
 $limi[1]=$page_number*20;
 $limit=" LIMIT ".$limi[0].",".$limi[1];
 if($sql_order=="")
 {
	$sql_order=' ORDER BY id DESC';
 }
 
 
 
 
 
 
	
 
 
 
 
   $ids_form=$wpdb->get_col("SELECT id FROM  ".$wpdb->prefix."formmaker".$sql_text.$sql_order.$limit,0);
	   foreach($ids_form as $id_form)
	   {
		   $row=$wpdb->get_row("SELECT * FROM  ".$wpdb->prefix."formmaker WHERE id = ".$id_form);
		   
	   
 ?>
 <tr>
 <td><a  href="#"><?php echo $row->id; ?></a> </td>
 <td><a  href="admin.php?page=Form_maker&task=edit_form&id=<?php echo $row->id?>"><?php if($row->title!="") echo $row->title; else echo "(No Title)" ?></a> </td>
 <td><?php echo  $row->mail;  ?></td>
  <td><a  href="admin.php?page=Form_maker&task=edit_form&id=<?php echo $row->id?>">Edit</a> </td>
    <td><a href="javascript:confirmation('admin.php?page=Form_maker&task=delete&id=<?php echo $row->id ?>','<?php if($row->title!="") echo addslashes($row->title); else echo "(No Title)" ?>')">Delete</a> </td>
 </tr>
 <?php
    }
	?>
 </tbody>
 </table>

 <?php
?>
    
    
   
 </form>
    <?php
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                                                       Function add a element





















function add()
{
  $Forms_title='Adding New Form';

  ?>
  <table width="95%">
  <tr><td colspan="7" align="right" style="font-size:16px;">
  		<a href="http://web-dorado.com/files/fromFormMaker.php" target="_blank" style="color:red; text-decoration:none;">
		<img src="<?php echo plugins_url( 'images/header.png' , __FILE__ ); ?>" border="0" alt="www.web-dorado.com" width="215"><br>
		Get the full version&nbsp;&nbsp;&nbsp;&nbsp;
		</a>
  </td></tr>
  <tr>
  <td width="100%"><?php echo "<h2>".__($Forms_title). "</h2>"; ?></td>
  <td><input type="button" onclick="submitbutton('Edit_JavaScript')" value="Edit JavaScript" class="button-primary" /> </td>
  <td> <input type="button" onclick="submitbutton('Edit_CSS')" value="Edit CSS" class="button-primary" /> </td>  
  <td style="width:300px"><input type="button" onclick="submitbutton('Custom_text_in_email')" value="Custom text in email" class="button-primary" /> </td>
  <td align="right"><input type="button" onclick="submitbutton('Save')" value="Save" class="button-secondary action" /> </td>  
  <td align="right"><input type="button" onclick="submitbutton('Apply')" value="Apply"  class="button-secondary action"/> </td> 
  <td align="right"><input type="button" onclick="window.location.href='admin.php?page=Form_maker'" value="Cancel" class="button-secondary action" /> </td> 
  </tr>
  </table>
  
   <input type="hidden" id="file_location_root" value="<?php echo ''.plugins_url("",__FILE__); ?>"  />
   <input type="hidden" id="upload_location" value="<?php $xx=str_replace ( site_url()."/" ,"",plugins_url("",__FILE__)); echo $xx; ?>"  />
   
   <script type="text/javascript" language="javascript">
</script>
   
   
   
   <script type="text/javascript" language="javascript">

   function showCalendar(id, dateFormat) {
	var el = document.getElementById(id);
	if (calendar != null) {
		// we already have one created, so just update it.
		calendar.hide();		// hide the existing calendar
		calendar.parseDate(el.value); // set it to a new date
	} else {
		// first-time call, create the calendar
		var cal = new Calendar(true, null, selected, closeHandler);
		calendar = cal;		// remember the calendar in the global
		cal.setRange(1900, 2070);	// min/max year allowed

		if ( dateFormat )	// optional date format
		{
			cal.setDateFormat(dateFormat);
		}

		calendar.create();		// create a popup calendar
		calendar.parseDate(el.value); // set it to a new date
	}
	calendar.sel = el;		// inform it about the input field in use
	calendar.showAtElement(el);	// show the calendar next to the input field

	// catch mousedown on the document
	Calendar.addEvent(document, "mousedown", checkCalendar);
	return false;
}
function submit_in(pressbutton)
{
	document.getElementById('all_Form_Maker').action="admin.php?page=Form_maker&task="+pressbutton;
	document.getElementById('all_Form_Maker').submit();
}
function submitbutton(pressbutton) 
{
	
	var form = document.all_Form_Maker;
	if (form.title.value == "")
	{
		

				alert( "The form must have a title." );	
				return;

	}		
	else
	if(form.mail.value!='')
	{
		subMailArr=form.mail.value.split(',');
		emailListValid=true;
		for(subMailIt=0; subMailIt<subMailArr.length; subMailIt++)
		{
		trimmedMail = subMailArr[subMailIt].replace(/^\s+|\s+$/g, '') ;
		if (trimmedMail.search(/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/) == -1)
		{
					alert( "This is not a list of valid email addresses." );	
					emailListValid=false;
					break;
					return;
		}
   
		}
		if(!emailListValid)	
		return;

	}
	
	form_view=document.getElementById('form_view');
	GLOBAL_tr=form_view.firstChild;
	tox='';
	for (x=0; x < GLOBAL_tr.childNodes.length; x++)
	{
		td=GLOBAL_tr.childNodes[x];
		tbody=td.firstChild.firstChild;
		for (y=0; y < tbody.childNodes.length; y++)
		{
			tr=tbody.childNodes[y];
			l_label = document.getElementById( tr.id+'_element_label').innerHTML;
			l_label = l_label.replace(/(\r\n|\n|\r)/gm," ");
			tox=tox+tr.id+'#**id**#'+l_label+'#**label**#'+tr.getAttribute('type')+'#****#';
		}
	}
	document.getElementById('label_order').value=tox;
	submit_in( pressbutton );
}
function str_replace(haystack, needle, replacement) { 
	var temp = haystack.split(needle); 
	return temp.join(replacement); 
} 

</script>

    <script type="text/javascript">
    gen=1;//add main form  id
       function enable()
	{
		document.getElementById('formmakerDiv').style.display	=(document.getElementById('formmakerDiv').style.display=='block'?'none':'block');
		document.getElementById('formmakerDiv1').style.display	=(document.getElementById('formmakerDiv1').style.display=='block'?'none':'block');
		if(document.getElementById('formmakerDiv').offsetWidth)
			document.getElementById('formmakerDiv1').style.width	=(document.getElementById('formmakerDiv').offsetWidth - 60)+'px';
		document.getElementById('when_edit').style.display		='none';
	}
	 function enable2()
	{
		document.getElementById('formmakerDiv').style.display	=(document.getElementById('formmakerDiv').style.display=='block'?'none':'block');
		document.getElementById('formmakerDiv1').style.display	=(document.getElementById('formmakerDiv1').style.display=='block'?'none':'block');
		if(document.getElementById('formmakerDiv').offsetWidth)
			document.getElementById('formmakerDiv1').style.width	=(document.getElementById('formmakerDiv').offsetWidth - 60)+'px';
		document.getElementById('when_edit').style.display		='block';
		if(document.getElementById('field_types').offsetWidth)
			document.getElementById('when_edit').style.width	=document.getElementById('field_types').offsetWidth+'px';
		
		if(document.getElementById('field_types').offsetHeight)
			document.getElementById('when_edit').style.height	=document.getElementById('field_types').offsetHeight+'px';
		
		//document.getElementById('when_edit').style.position='none';
		
	}
	
	function submit_form_postid(x)
				 {
					 
					 var val=x.options[x.selectedIndex].value;
					 document.getElementById("post_id").value=val;
					
				 }
    </script>
<style>

#when_edit
{
position:absolute;
background-color:#666;
z-index:101;
display:none;
width:100%;
height:100%;
opacity: 0.7;
filter: alpha(opacity = 70);
}

#formmakerDiv
{
position:fixed;
background-color:#666;
z-index:100;
display:none;
left:0;
top:0;
width:100%;
height:100%;
opacity: 0.7;
filter: alpha(opacity = 70);
}
#formmakerDiv1, #formmakerDiv1.td
{
font-size:12px;
position:fixed;
z-index:100;
background-color:transparent;
top:0;
left:0;
display:none;
margin-left:30px;
margin-top:40px;
}

/*#fonti, td , div , input , textarea
{
	font-size:11px !important;
}*/
</style>
   <br />
   <br />
   <form id="all_Form_Maker" action="#" method="post" name="all_Form_Maker" enctype="multipart/form-data">
<table style="border:6px #00aeef solid; background-color:#00aeef; min-width:800px"  width="95%" cellpadding="0" cellspacing="0">
<tbody><tr style="height:27px;">


    <td align="left" valign="middle" rowspan="3" style="padding:10px;">
    <img src="<?php echo plugins_url('images/formmaker.png',__FILE__) ?>">
	</td>

    <td width="350px" align="right" valign="middle">

    <span style="font-size:16.76pt; font-family:BauhausItcTEEMed; color:#FFFFFF; vertical-align:middle;">Form title:&nbsp;&nbsp;</span>

    </td>

    <td width="153px" align="center" valign="middle">

    <div style="background-image:url(<?php echo plugins_url('images/input.png',__FILE__) ?>); height:19px; width:153px; vertical-align:top">

    <input id="title" name="title" style="background:none; width:150px; height:15px; border:none; font-size:10px;">

    </div>

    </td>
	
</tr>


<tr>

    <td width="300" align="right" valign="middle">

    <span style="font-size:16.76pt; font-family:BauhausItcTEEMed; color:#FFFFFF; vertical-align:middle;">Email to send submissions to:&nbsp;&nbsp;</span>

    </td>

    <td width="153" align="center" valign="middle">

    <div style="background-image:url(<?php echo plugins_url('images/input.png',__FILE__) ?>); height:19px; width:153px">

    <input id="mail" name="mail" style="background:none; width:151px; height:15px; border:none; font-size:11px">

    </div>

    </td>

    </tr>






<tr>
    <td width="400" align="right" valign="top" style="padding-top:7px">
    <span style="font-size:16.76pt; font-family:BauhausItcTEEMed; color:#FFFFFF; vertical-align:middle;">The Post, which appears after submission:&nbsp;&nbsp;</span>
    </td>
<td class="paramlist_value" width="153" valign="middle" align="left" style="padding-top:7px">

<select name="post_name" style="width:153px; font-size:11px;" onchange="submit_form_postid(this)">
<option value="- Select Menu -">- Select Post -</option>
<?php
 
// The Query
query_posts($args );
 
// The Loop
while ( have_posts() ) : the_post(); ?>
	<option value="<?php the_ID(); ?>"><?php the_title();?></option>
	
    <?php
endwhile;
 
// Reset Query
wp_reset_query();
 
?>
</select>

</td>




    </tr>





  <tr>
  <td align="left" colspan="3">
  
  <img src="<?php echo plugins_url('images/addanewfield.png',__FILE__) ?>" onclick="enable()" style="cursor:pointer;margin:10px;">

  </td>
  </tr>
  </tbody></table>
  <div id="formmakerDiv" onclick="enable()"></div>
  <div id="formmakerDiv1" align="center">
    
<table border="0" width="100%" cellpadding="0" cellspacing="0" height="100%" style="border:6px #00aeef solid; background-color:#FFF; font-size:10px">
  <tr>
    <td style="padding:0px">
    <table border="0" cellpadding="0" cellspacing="0" width="100%" height="100%" id="fonti">
        <tr valign="top">
         <td width="92" height="100%" style="border-right:dotted black 1px;" id="field_types">
         <div id="when_edit" style="display:none"></div>
          	<table border="0" cellpadding="0" cellspacing="0" width="100%">
              <tr>
               <td align="center" onClick="addRow('editor')" style="cursor:pointer" id="table_editor"><img src="<?php echo plugins_url('images/customHTML.png',__FILE__) ?> " style="margin:5px"/></td>
            
                <td align="center" onClick="addRow('text')" style="cursor:pointer" id="table_text"><img src="<?php echo plugins_url('images/text.png',__FILE__) ?>" style="margin:5px"/></td>
              </tr>
               <tr>
                <td align="center" onClick="addRow('time_and_date')" style="cursor:pointer" id="table_time_and_date"><img src="<?php echo plugins_url('images/time_and_date.png',__FILE__) ?>" style="margin:5px"/></td>
              
                <td align="center" onClick="addRow('select')" style="cursor:pointer" id="table_select"><img src="<?php echo plugins_url('images/select.png',__FILE__) ?>"style="margin:5px"/></td>
              </tr>
			  <tr>             
				 <td align="center" onClick="addRow('checkbox')" style="cursor:pointer" id="table_checkbox"><img src="<?php echo plugins_url('images/checkbox.png',__FILE__) ?>"style="margin:5px"/></td>
 
				<td align="center" onClick="addRow('radio')" style="cursor:pointer" id="table_radio"><img src="<?php echo plugins_url('images/radio.png',__FILE__) ?>"style="margin:5px"/></td>
             </tr>
              <tr>
                <td align="center" onClick="addRow('file_upload')" style="cursor:pointer" id="table_file_upload"><img src="<?php echo plugins_url('images/file_upload.png',__FILE__) ?>"style="margin:5px"/></td>
              
                <td align="center" onClick="addRow('captcha')" style="cursor:pointer" id="table_captcha"><img src="<?php echo plugins_url('images/captcha.png',__FILE__) ?>"style="margin:5px"/></td>
              </tr>
              <tr>
                  <td align="center" onClick="addRow('map')" style="cursor:pointer" id="table_map"><img src="<?php echo plugins_url('images/map.png',__FILE__) ?>"style="margin:5px"/></td>  

				  <td align="center" onClick="addRow('button')" style="cursor:pointer" id="table_button"><img src="<?php echo plugins_url('images/button.png',__FILE__) ?>"style="margin:5px"/></td>
              </tr>
            </table>
         </td>
         <td width="30%" height="100%" align="left"><div id="edit_table" style="padding:0px; overflow-y:scroll; height:520px" ></div></td>
   <td align="center" valign="top" style="background:url(<?php echo plugins_url('images/border2.png',__FILE__) ?>) repeat-y;">&nbsp;</td>
         <td width="60%" style="padding:15px">
         <table border="0" cellpadding="0" cellspacing="0" width="100%" height="100%" style="font-size:11px">
         
            <tr>
                <td align="right"><input type="radio" value="end" name="el_pos" checked="checked" id="pos_end" onclick="Disable()"/>
                  At The End
                  <input type="radio" value="begin" name="el_pos" id="pos_begin" onclick="Disable()" style=""/>
                  At The Beginning
                  <input type="radio" value="before" name="el_pos" id="pos_before" onclick="Enable()"/>
                  Before
                  <select style="width:100px; margin-left:5px" id="sel_el_pos" disabled="disabled">
                  </select>
                  <img alt="ADD" title="add" style="cursor:pointer; vertical-align:middle; margin:5px" src="<?php echo plugins_url('images/save.png',__FILE__) ?>" onClick="add(0)"/>
                  <img alt="CANCEL" title="cancel"  style=" cursor:pointer; vertical-align:middle; margin:5px" src="<?php echo plugins_url('images/cancel_but.png',__FILE__) ?>" onClick="close_window()"/>
				
                	<hr style=" margin-bottom:10px" />
                  </td>
              </tr>
              
              <tr height="100%" valign="top">
                <td  id="show_table"></td>
              </tr>
              
            </table>
         </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<input type="hidden" id="old" />
<input type="hidden" id="old_selected" />
<input type="hidden" id="element_type" />
<input type="hidden" id="editing_id" />
<input type="hidden" id="label_order" name="label_order" value="" />
<input type="hidden" id="post_id" name="post_id">
<div id="main_editor" style="position:absolute; display:none; z-index:140;padding:10px;">
<div  style=" max-width:500px; height:300px;text-align:left" id="poststuff">
<div id="<?php echo user_can_richedit() ? 'postdivrich' : 'postdiv'; ?>" class="postarea"><?php the_editor("","editor","title",$media_buttons = true, $tab_index = 1, $extended = true ); ?>
</div>
</div>
</div>
</div>
  <br />
  <br />


<fieldset>

    <legend>

    <h2 style="color:#00aeef">Form</h2>
    
    </legend>

     <style>..form_view, .form_view table
{
width:inherit !important;
-webkit-border-horizontal-spacing: 0px;
-webkit-border-vertical-spacing: 0px;
border-bottom-color: gray;
border:0px  !important;
border-bottom-width: 0px;
border-collapse: separate;
border-left-color: gray;
border-left-width: 0px;
border-right-color: gray;
border-right-width: 0px;
border-top-color: gray;
border-top-width: 0px;
color: black;
display: table;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px !important;
font-weight: normal;
height: inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: left !important;

}

.form_view, .form_view tr
{
-webkit-border-horizontal-spacing: 0px;
-webkit-border-vertical-spacing: 0px;
border:0px  !important;
border-bottom-color: gray;
border-collapse: separate;
border-left-color: gray;
border-right-color: gray;
border-top-color: gray;
color: black;
display: table-row;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height: inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: left;
vertical-align: middle;
width:inherit !important;
}

.form_view, .form_view td
{
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
border-bottom-color: black;
border-collapse: separate;
border-left-color: black;
border-right-color: black;
border-top-color: black;
border:0px !important;
color: black;
display: table-cell;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height:inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 1px !important;
padding-left: 1px !important;
padding-right: 1px !important;
padding-top: 3px !important;
text-align: left !important;
width:inherit !important;
vertical-align:top;
}
.form_view, .form_view tr
{
-webkit-border-horizontal-spacing: 0px;
-webkit-border-vertical-spacing: 0px;
border:0px  !important;
border-bottom-color: gray;
border-collapse: separate;
border-left-color: gray;
border-right-color: gray;
border-top-color: gray;
color: black;
display: table-row;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height: inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: left;
vertical-align: middle;
width:inherit !important;
}

.form_view, .form_view input,  .form_view  textarea
{
line-height:inherit  !important;
margin:0px !important;
min-height: 18px !important;
 font-size: 14px !important;
}
.form_view, .form_view select
{
margin:0px !important;
font-size: 14px !important;
}
.form_view, .form_view label
{
font-size: 14px;
 vertical-align:inherit !important;
}
.time_box
{
border-width:1px;
margin: 0px;
padding: 0px;
text-align:right;
width:30px;
vertical-align:middle
}


.mini_label
{
color: #000 !important;
font-size:14px;
font-family: Lucida Grande, Tahoma, Arial, Verdana, sans-serif;
}

.ch_rad_label
{
color:#000 !important;
display:inline;
margin-left:5px;
margin-right:15px;
float:none;
}

.label
{
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
border-bottom-color: black;
border-bottom-style: none;
border-collapse: separate;
border-left-color: black;
border-left-style: none;
border-right-color: black;
border-right-style: none;
border-top-color: black;
border-top-style: none;
color: black;
display: inline;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height: auto;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: -webkit-left;
width: auto;
}


.td_am_pm_select
{
padding-left:5;
}

.am_pm_select
{
height: 16px;
margin:0;
padding:0
}

.input_deactive
{
background-color: #FFFFFF;
border-bottom-style: inset;
border-bottom-width: 1px;
border-collapse: separate;
border-left-color: #EEE;
border-left-style: inset;
border-left-width: 1px;
border-right-color: #EEE;
border-right-style: inset;
border-right-width: 1px;
border-top-color: #EEE;
border-top-style: inset;
border-top-width: 1px;
font-style: italic;
color: #999;
cursor: auto;
display: inline-block;
font-family: Arial;
font-size: 14px !important;
font-weight: normal;
letter-spacing: normal;
line-height: normal;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: -webkit-auto;
text-indent: 0px;
text-shadow: none;
text-transform: none;
word-spacing: 0px;
}

.input_active
{
background-color: #FFFFFF;
-webkit-appearance: none;
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
-webkit-rtl-ordering: logical;
-webkit-user-select: text;
background-color: white;
border-bottom-color: #EEE;
border-bottom-style: inset;
border-bottom-width: 1px;
border-collapse: separate;
border-left-color: #EEE;
border-left-style: inset;
border-left-width: 1px;
border-right-color: #EEE;
border-right-style: inset;
border-right-width: 1px;
border-top-color: #EEE;
border-top-style: inset;
border-top-width: 1px;
color: black;
cursor: auto;
display: inline-block;
font-family: Arial;
font-size: 14px !important;
font-style: normal;
font-weight: normal;
height: 16px;
letter-spacing: normal;
line-height: normal;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: -webkit-auto;
text-indent: 0px;
text-shadow: none;
text-transform: none;
width: 200px;
word-spacing: 0px;
}

.required
{
border:none;
color:red
}

.captcha_img
{
border-width:0px;
margin: 0px;
padding: 0px;
cursor:pointer;


}

.captcha_refresh
{
width:18px;
border-width:0px;
margin: 0px;
padding: 0px;
vertical-align:middle;
cursor:pointer;
}

.captcha_input
{
height:20px;
border-width:1px;
margin: 0px;
padding: 0px;
vertical-align:middle;
}

.file_upload
{
-webkit-appearance: none;
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
-webkit-box-align: baseline;
-webkit-rtl-ordering: logical;
-webkit-user-select: text;
background-color: transparent;
border-bottom-color: black;
border-bottom-style: none;
border-bottom-width: 0px;
border-collapse: separate;
border-left-color: black;
border-left-style: none;
border-left-width: 0px;
border-right-color: black;
border-right-style: none;
border-right-width: 0px;
border-top-color: black;
border-top-style: none;
border-top-width: 0px;
color: black;
cursor: auto;
display: inline-block;
font-family: Arial;
font-size: 13px;
font-weight: normal;
height: 22px;
letter-spacing: normal;
line-height: normal;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: start;
text-indent: 0px;
text-shadow: none;
text-transform: none;
width: 238px;
word-spacing: 0px;
}         
.captcha_table , .captcha_table input
{
  font-size: 15px !important;
} 
    </style>



    <div id="take"><table border="0" cellpadding="4" cellspacing="0" class="form_view"><tbody  id="form_view" ><tr><td id="column_0" valign="top"><table><tbody></tbody></table></td></tr></tbody></table></div>

    </fieldset>
  

    <input type="hidden" name="form" id="form" value='<table border="0" cellpadding="4" cellspacing="0" class="form_view"><tbody  id="form_view" ><tr><td id="column_0" valign="top"><table><tbody></tbody></table></td></tr></tbody></table>'>

    <input type="hidden" name="counter" id="counter">


    <input type="hidden" name="option" value="com_formmaker" />

    <input type="hidden" name="task" value="" />
    </form>
    <link type="text/css" rel="stylesheet" href="<?php echo plugins_url("js/calendar-jos.css",__FILE__) ?>" />
	  <script type="text/javascript" src="<?php echo plugins_url("js/formmaker.js",__FILE__) ?>"></script>
	  <script type="text/javascript" src="<?php echo plugins_url("js/calendar_function.js",__FILE__) ?>"></script>
	  <script type="text/javascript" src="<?php echo plugins_url("js/calendar.js",__FILE__) ?>"></script>
	  <script type="text/javascript" src="<?php echo plugins_url("js/calendar-setup.js",__FILE__) ?>"></script>
<?php


}


















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                                                       Function edit css

function edit_css($id)
{
	global $wpdb;
  $row=$wpdb->get_row("SELECT * FROM  ".$wpdb->prefix."formmaker WHERE id = '".$id."'");
  if(!$row)
  {
	  die("error to canecting databese please unistal this plugin");
  }
  
  $Forms_title='Edit CSS - '.$row->title;
  
  ?>
  <script type="text/javascript" language="javascript">
  function submit_in(x)
  {

	  document.getElementById('edit_css').action="admin.php?page=Form_maker&task="+x+"&id=<?php echo  $row->id;?>";
	  document.getElementById('edit_css').submit();
	  
  }
  </script>
  <form action="#" method="post" name="edit_css" id="edit_css">
  <table width="95%">
  <tr>
  <td width="100%"><?php echo "<h2>".__($Forms_title). "</h2>"; ?></td>
  <td align="right"><input type="button" onclick="submit_in('Save_edit_css')" value="Save" class="button-secondary action" /> </td>  
  <td align="right"><input type="button" onclick="submit_in('Apply_edit_css')" value="Apply"  class="button-secondary action"/> </td> 
  <td align="right"><input type="button" onclick="window.location.href='admin.php?page=Form_maker&task=edit_form&id=<?php echo  $row->id;?>'" value="Cancel" class="button-secondary action" /> </td> 
  </tr>
  </table>
  <br />
  <br />
  
  <table>

        <tbody><tr>

            <th align="left">

                <label for="message">CSS</label>

                <button onclick="document.getElementById('css').value=document.getElementById('def').innerHTML; return false;" style="margin-left:15px;">Restore default CSS</button>

            </th>

        </tr>

        <tr>

            <td>

                <textarea style="margin: 0px;" cols="110" rows="25" name="css" id="css"><?php echo $row->css; ?></textarea>

            </td>

        </tr>

    </tbody></table>
  </form>
  <textarea style="visibility:hidden" id="def">.form_view, .form_view table
{
width:inherit !important;
-webkit-border-horizontal-spacing: 0px;
-webkit-border-vertical-spacing: 0px;
border-bottom-color: gray;
border:0px  !important;
border-bottom-width: 0px;
border-collapse: separate;
border-left-color: gray;
border-left-width: 0px;
border-right-color: gray;
border-right-width: 0px;
border-top-color: gray;
border-top-width: 0px;
color: black;
display: table;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px !important;
font-weight: normal;
height: inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: left !important;

}

.form_view, .form_view tr
{
-webkit-border-horizontal-spacing: 0px;
-webkit-border-vertical-spacing: 0px;
border:0px  !important;
border-bottom-color: gray;
border-collapse: separate;
border-left-color: gray;
border-right-color: gray;
border-top-color: gray;
color: black;
display: table-row;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height: inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: left;
vertical-align: middle;
width:inherit !important;
}

.form_view, .form_view td
{
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
border-bottom-color: black;
border-collapse: separate;
border-left-color: black;
border-right-color: black;
border-top-color: black;
border:0px !important;
color: black;
display: table-cell;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height:inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 1px !important;
padding-left: 1px !important;
padding-right: 1px !important;
padding-top: 3px !important;
text-align: left !important;
width:inherit !important;
vertical-align:top;
}
.form_view, .form_view tr
{
-webkit-border-horizontal-spacing: 0px;
-webkit-border-vertical-spacing: 0px;
border:0px  !important;
border-bottom-color: gray;
border-collapse: separate;
border-left-color: gray;
border-right-color: gray;
border-top-color: gray;
color: black;
display: table-row;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height: inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: left;
vertical-align: middle;
width:inherit !important;
}

.form_view, .form_view input,  .form_view  textarea
{
line-height:inherit  !important;
margin:0px !important;
min-height: 18px !important;
 font-size: 14px !important;
}
.form_view, .form_view select
{
margin:0px !important;
font-size: 14px !important;
}
.form_view, .form_view label
{
font-size: 14px;
 vertical-align:inherit !important;
}
.time_box
{
border-width:1px;
margin: 0px;
padding: 0px;
text-align:right;
width:30px;
vertical-align:middle
}


.mini_label
{
color: #000 !important;
font-size:14px;
font-family: Lucida Grande, Tahoma, Arial, Verdana, sans-serif;
}

.ch_rad_label
{
color:#000 !important;
display:inline;
margin-left:5px;
margin-right:15px;
float:none;
}

.label
{
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
border-bottom-color: black;
border-bottom-style: none;
border-collapse: separate;
border-left-color: black;
border-left-style: none;
border-right-color: black;
border-right-style: none;
border-top-color: black;
border-top-style: none;
color: black;
display: inline;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height: auto;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: -webkit-left;
width: auto;
}


.td_am_pm_select
{
padding-left:5;
}

.am_pm_select
{
height: 16px;
margin:0;
padding:0
}

.input_deactive
{
background-color: #FFFFFF;
border-bottom-style: inset;
border-bottom-width: 1px;
border-collapse: separate;
border-left-color: #EEE;
border-left-style: inset;
border-left-width: 1px;
border-right-color: #EEE;
border-right-style: inset;
border-right-width: 1px;
border-top-color: #EEE;
border-top-style: inset;
border-top-width: 1px;
font-style: italic;
color: #999;
cursor: auto;
display: inline-block;
font-family: Arial;
font-size: 14px !important;
font-weight: normal;
letter-spacing: normal;
line-height: normal;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: -webkit-auto;
text-indent: 0px;
text-shadow: none;
text-transform: none;
word-spacing: 0px;
}

.input_active
{
background-color: #FFFFFF;
-webkit-appearance: none;
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
-webkit-rtl-ordering: logical;
-webkit-user-select: text;
background-color: white;
border-bottom-color: #EEE;
border-bottom-style: inset;
border-bottom-width: 1px;
border-collapse: separate;
border-left-color: #EEE;
border-left-style: inset;
border-left-width: 1px;
border-right-color: #EEE;
border-right-style: inset;
border-right-width: 1px;
border-top-color: #EEE;
border-top-style: inset;
border-top-width: 1px;
color: black;
cursor: auto;
display: inline-block;
font-family: Arial;
font-size: 14px !important;
font-style: normal;
font-weight: normal;
height: 16px;
letter-spacing: normal;
line-height: normal;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: -webkit-auto;
text-indent: 0px;
text-shadow: none;
text-transform: none;
width: 200px;
word-spacing: 0px;
}

.required
{
border:none;
color:red
}

.captcha_img
{
border-width:0px;
margin: 0px;
padding: 0px;
cursor:pointer;


}

.captcha_refresh
{
width:18px;
border-width:0px;
margin: 0px;
padding: 0px;
vertical-align:middle;
cursor:pointer;
}

.captcha_input
{
height:20px;
border-width:1px;
margin: 0px;
padding: 0px;
vertical-align:middle;
}

.file_upload
{
-webkit-appearance: none;
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
-webkit-box-align: baseline;
-webkit-rtl-ordering: logical;
-webkit-user-select: text;
background-color: transparent;
border-bottom-color: black;
border-bottom-style: none;
border-bottom-width: 0px;
border-collapse: separate;
border-left-color: black;
border-left-style: none;
border-left-width: 0px;
border-right-color: black;
border-right-style: none;
border-right-width: 0px;
border-top-color: black;
border-top-style: none;
border-top-width: 0px;
color: black;
cursor: auto;
display: inline-block;
font-family: Arial;
font-size: 13px;
font-weight: normal;
height: 22px;
letter-spacing: normal;
line-height: normal;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: start;
text-indent: 0px;
text-shadow: none;
text-transform: none;
width: 238px;
word-spacing: 0px;
}         
.captcha_table , .captcha_table input
{
  font-size: 15px !important;
}      

</textarea>
  
  <?php
	
	
}
















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                                                       Function edit javascript

function Edit_JavaScript($id)
{
	
  global $wpdb;
  $row=$wpdb->get_row("SELECT * FROM  ".$wpdb->prefix."formmaker WHERE id = '".$id."'");
  if(!$row)
  {
	  die("error to canecting databese");
  }
  
  $Forms_title='Edit JavaScript - '.$row->title;
  
  ?>
  <script type="text/javascript" language="javascript">
  function submit_in(x)
  {

	  document.getElementById('edit_js').action="admin.php?page=Form_maker&task="+x+"&id=<?php echo  $row->id;?>";
	  document.getElementById('edit_js').submit();
	  
  }
  </script>
  <form action="#" method="post" name="edit_js" id="edit_js">
  <table width="95%">
  <tr>
  <td width="100%"><?php echo "<h2>".__($Forms_title). "</h2>"; ?></td>
  <td align="right"><input type="button" onclick="submit_in('Save_edit_JavaScript')" value="Save" class="button-secondary action" /> </td>  
  <td align="right"><input type="button" onclick="submit_in('Apply_edit_JavaScript')" value="Apply"  class="button-secondary action"/> </td> 
  <td align="right"><input type="button" onclick="window.location.href='admin.php?page=Form_maker&task=edit_form&id=<?php echo  $row->id;?>'" value="Cancel" class="button-secondary action" /> </td> 
  </tr>
  </table>
  <br />
  <br />
    
    
    
	<table width="95%">

        <tbody><tr>

            <th align="left">

                <label for="message">Javascript</label>

            </th>

        </tr>

        <tr>

            <td>

                <textarea style="margin: 0px;" cols="110" rows="25" name="javascript" id="javascript"><?php echo $row->javascript; ?></textarea>

            </td>

        </tr>

    </tbody></table>
    <?php
	
}

















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                                                       Function Custom text in email

function text_in_email($id)
{
	global $wpdb;
	$row=$wpdb->get_row("SELECT * FROM  ".$wpdb->prefix."formmaker WHERE id = '".$id."'");
	?>
    <table width="95%">
  <tr>
  <td width="100%"> <?php $mail_title='Custom text in email - '.$row->title;   echo "<h2>" . __($mail_title) . "</h2>"; ?></td>
  <td align="right"><input type="button" onclick="submit_in('custom_text_Save')" value="Save" class="button-secondary action"> </td>  
  <td align="right"><input type="button" onclick="submit_in('Custom_text_apply')" value="Apply" class="button-secondary action"> </td> 
  <td align="right"><input type="button" onclick="window.location.href='admin.php?page=Form_maker&task=edit_form&id=<?php echo $id ?>'" value="Cancel" class="button-secondary action"> </td> 
  </tr>
  </table>
  <br />
  <br />
  <br />
 
    <form action="admin.php?page=Form_maker&task=Custom_text_in_email&id=<?php echo $id ?>" method="post" id="all_Form_Maker">
    <table width="95%" style="border-color:#000; border:medium;" >

        <tbody>
        <tr>

            <th style="text-align:left">

                <label for="message"  style="text-align:left"> Text before Message </label>
                <br />
             </th>
            </tr>
            <tr>
            
             <td style="width:95%; min-width:500px"><?php if(function_exists(wp_editor)){ the_editor ( $row->script1, $idd = 'text_mail_befor', $prev_id = 'Mail_script1', $media_buttons = true, $tab_index = 1, $extended = true );} else {?>
 
			<textarea style="width:100%" name="text_mail_befor" id><?php echo $row->script1 ?></textarea>
			<?php } ?>
            <br />
            </td>
			</tr>
            <tr>
            <td>
             <hr />
             <h2 align="center">MESSAGE</h2>
             <hr />
             <br />
            </td>
            </tr>
             <tr>

            <th style="text-align:left">

                <label for="message"  style="text-align:left"> Text after Message </label>
                <br />
            </th>
            </tr>
                        <tr>
            
           <td style="width:70%; min-width:500px"><?php if(function_exists(wp_editor)){ the_editor ( $row->script2, $idd = 'text_mail_after', $prev_id = 'Mail_title2', $media_buttons = true, $tab_index = 2, $extended = true );}else { ?>
			
			<textarea style="width:100%" name="text_mail_after"><?php echo $row->script2 ?></textarea>
			<?php } ?></td>
			</tr>
        </tbody>
        </table>
    
    </form>
    <script type="text/ecmascript" language="javascript">
    function submit_in(x)
{
	document.getElementById('all_Form_Maker').action="admin.php?page=Form_maker&task="+x+"&id=<?php echo $id; ?>";
	document.getElementById('all_Form_Maker').submit();
}
</script>
    
    <?php
	
	
	
}
















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                                                       Function DElETE

function delete($id)
{
	global $wpdb;
	if(0<=$id)
	{
	$sqll="DELETE FROM ".$wpdb->prefix."formmaker WHERE id='".$id."'";
	 $wpdb->query($sqll);
	$sqll="DELETE FROM ".$wpdb->prefix."formmaker_submits WHERE form_id='".$id."'";
	 $wpdb->query($sqll);
	 ?> <div class="updated"><p><strong><?php _e('Item Deleted.' ); ?></strong></p></div> <?php
	}
	else
	{
		die("Error");		
	}
		
}

















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                                                       Function edit Form

function edit($id)
{
  global $wpdb;
  
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  $row=$wpdb->get_row("SELECT * FROM  ".$wpdb->prefix."formmaker WHERE id = '".$id."'");
  $Forms_title='Form - '.$row->title;
  
  ?>
  <table width="95%">
  <tr>
  <td width="100%"><?php echo "<h2>".__($Forms_title). "</h2>"; ?></td>
  <td><input type="button" onclick="submitbutton('Edit_JavaScript')" value="Edit JavaScript" class="button-primary" /> </td>
  <td> <input type="button" onclick="submitbutton('Edit_CSS')" value="Edit CSS" class="button-primary" /> </td>  
  <td style="width:300px"><input type="button" onclick="submitbutton('Custom_text_in_email')" value="Custom text in email" class="button-primary" /> </td>
  <td align="right"><input type="button" onclick="submitbutton('Save')" value="Save" class="button-secondary action" /> </td>  
  <td align="right"><input type="button" onclick="submitbutton('Apply')" value="Apply"  class="button-secondary action"/> </td> 
  <td align="right"><input type="button" onclick="window.location.href='admin.php?page=Form_maker'" value="Cancel" class="button-secondary action" /> </td> 
  </tr>
  </table>
  
 
   <input type="hidden" id="file_location_root" value="<?php echo ''.plugins_url("",__FILE__); ?>"  />
   <input type="hidden" id="upload_location" value="<?php $xx=str_replace ( site_url()."/" ,"",plugins_url("",__FILE__)); echo $xx; ?>"  />
   
   <script type="text/javascript" language="javascript">
</script>
   <?php
  //////////////////////////////////////////       GET LABELS
  		$labels= array();
		
		$label_id= array();
		$label_order_original= array();
		$label_type= array();
		
		$label_all	= explode('#****#',$row->label_order);
		$label_all 	= array_slice($label_all,0, count($label_all)-1);   
		
		
		
		foreach($label_all as $key => $label_each) 
		{
			$label_id_each=explode('#**id**#',$label_each);
			array_push($label_id, $label_id_each[0]);
			
			$label_oder_each=explode('#**label**#', $label_id_each[1]);
			array_push($label_order_original, $label_oder_each[0]);
			array_push($label_type, $label_oder_each[1]);

		
			
		}
		
	$labels['id']='"'.implode('","',$label_id).'"';
	$labels['label']='"'.implode('","',$label_order_original).'"';
	$labels['type']='"'.implode('","',$label_type).'"';
   
   
   
    ?>
   
   
   <script type="text/javascript" language="javascript">

   function showCalendar(id, dateFormat) {
	var el = document.getElementById(id);
	if (calendar != null) {
		// we already have one created, so just update dit.
		calendar.hide();		// hide the existing calendar
		calendar.parseDate(el.value); // set it to a new date
	} else {
		// first-time call, create the calendar
		var cal = new Calendar(true, null, selected, closeHandler);
		calendar = cal;		// remember the calendar in the global
		cal.setRange(1900, 2070);	// min/max year allowed

		if ( dateFormat )	// optional date format
		{
			cal.setDateFormat(dateFormat);
		}

		calendar.create();		// create a popup calendar
		calendar.parseDate(el.value); // set it to a new date
	}
	calendar.sel = el;		// inform it about the input field in use
	calendar.showAtElement(el);	// show the calendar next to the input field

	// catch mousedown on the document
	Calendar.addEvent(document, "mousedown", checkCalendar);
	return false;
}
function submit_in(pressbutton)
{
	if(!document.getElementById('load_or_no'))
	{
		alert('Please wait while page loading');
		return;
	}
	else
		if(document.getElementById('load_or_no').value=='0')
		{
			alert('Please wait while page loading');
			return;
		}
	document.getElementById('all_Form_Maker').action="admin.php?page=Form_maker&task="+pressbutton+'&id=<?php echo $row->id; ?>';
	document.getElementById('all_Form_Maker').submit();
}
function submitbutton(pressbutton) 

{

	var form = document.all_Form_Maker;

	if (pressbutton == 'cancel') 

	{

		submit_in( pressbutton );

		return;

	}

	if (form.title.value == "")

	{

				alert( "The form must have a title." );	
				return;

	}		

	if(form.mail.value!='')
	{
		subMailArr=form.mail.value.split(',');
		emailListValid=true;
		for(subMailIt=0; subMailIt<subMailArr.length; subMailIt++)
		{
		trimmedMail = subMailArr[subMailIt].replace(/^\s+|\s+$/g, '') ;
		if (trimmedMail.search(/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/) == -1)
		{
					alert( "This is not a list of valid email addresses." );	
					emailListValid=false;
					break;
		}
		}
		if(!emailListValid)	
		return

	}		

	form_view=document.getElementById('form_view');
	GLOBAL_tr=form_view.firstChild;
	tox='';

	l_id_array=[<?php echo $labels['id']?>];
	l_id_removed=[];
	for(x=0; x< l_id_array.length; x++)
		{
			l_id_removed[x]=true;
		}

	l_label_array=[<?php echo str_replace("
","",$labels['label'])?>];
	l_type_array=[<?php echo $labels['type']?>];
	for (x=0; x < GLOBAL_tr.childNodes.length; x++)
	{
		td=GLOBAL_tr.childNodes[x];
		tbody=td.firstChild.firstChild;
		for (y=0; y < tbody.childNodes.length; y++)
		{
			is_in_old=false;
			tr=tbody.childNodes[y];
			l_id=tr.id;
			
			l_label = document.getElementById( tr.id+'_element_label').innerHTML;
			l_label = l_label.replace(/(\r\n|\n|\r)/gm," ");
			l_type=tr.getAttribute('type');
			for(z=0; z< l_id_array.length; z++)
			{
				if(l_id_array[z]==l_id)
					l_id_removed[z]=false;
			}
			tox=tox+l_id+'#**id**#'+l_label+'#**label**#'+l_type+'#****#';
		}
	}
	for(x=0; x< l_id_array.length; x++)
	{
		if(l_id_removed[x])
			tox=tox+l_id_array[x]+'#**id**#'+l_label_array[x]+'#**label**#'+l_type_array[x]+'#****#';
	}
	document.getElementById('label_order').value=tox;
		submit_in( pressbutton );
}
function str_replace(haystack, needle, replacement) { 
	var temp = haystack.split(needle); 
	return temp.join(replacement); 
} 

</script>

    <script language='javascript'>
    gen=<?php echo $row->counter;?>;
       function enable()
	{
		document.getElementById('formmakerDiv').style.display	=(document.getElementById('formmakerDiv').style.display=='block'?'none':'block');
		document.getElementById('formmakerDiv1').style.display	=(document.getElementById('formmakerDiv1').style.display=='block'?'none':'block');
		if(document.getElementById('formmakerDiv').offsetWidth)
			document.getElementById('formmakerDiv1').style.width	=(document.getElementById('formmakerDiv').offsetWidth - 60)+'px';
		document.getElementById('when_edit').style.display		='none';
	}
	 function enable2()
	{
		document.getElementById('formmakerDiv').style.display	=(document.getElementById('formmakerDiv').style.display=='block'?'none':'block');
		document.getElementById('formmakerDiv1').style.display	=(document.getElementById('formmakerDiv1').style.display=='block'?'none':'block');
		if(document.getElementById('formmakerDiv').offsetWidth)
			document.getElementById('formmakerDiv1').style.width	=(document.getElementById('formmakerDiv').offsetWidth - 60)+'px';
		document.getElementById('when_edit').style.display		='block';
		if(document.getElementById('field_types').offsetWidth)
			document.getElementById('when_edit').style.width	=document.getElementById('field_types').offsetWidth+'px';
		
		if(document.getElementById('field_types').offsetHeight)
			document.getElementById('when_edit').style.height	=document.getElementById('field_types').offsetHeight+'px';
		
		//document.getElementById('when_edit').style.position='none';
		
	}
	function submit_form_postid(x)
				 {
					 
					 var val=x.options[x.selectedIndex].value;
					 document.getElementById("post_id").value=val;
					
				 }
    </script>
<style>
#when_edit
{
position:absolute;
background-color:#666;
z-index:101;
display:none;
width:100%;
height:100%;
opacity: 0.7;
filter: alpha(opacity = 70);
}

#formmakerDiv
{
position:fixed;
background-color:#666;
z-index:100;
display:none;
left:0;
top:0;
width:100%;
height:100%;
opacity: 0.7;
filter: alpha(opacity = 70);
}
#formmakerDiv1, #formmakerDiv1.td
{
font-size:12px;
position:fixed;
z-index:100;
background-color:transparent;
top:0;
left:0;
display:none;
margin-left:30px;
margin-top:40px;
}

/*#fonti, td , div , input , textarea
{
	font-size:11px !important;
}*/
</style>
   <br />
   <br />
   <form id="all_Form_Maker" action="#" method="post" name="all_Form_Maker" enctype="multipart/form-data">
<table style="border:6px #00aeef solid; background-color:#00aeef; min-width:800px"  width="95%" cellpadding="0" cellspacing="0">
<tbody><tr style="height:27px;">


    <td align="left" valign="middle" rowspan="3" style="padding:10px;">
    <img src="<?php echo plugins_url('images/formmaker.png',__FILE__) ?>">
	</td>

    <td width="350px" align="right" valign="middle">

    <span style="font-size:16.76pt; font-family:BauhausItcTEEMed; color:#FFFFFF; vertical-align:middle;">Form title:&nbsp;&nbsp;</span>

    </td>

    <td width="153px" align="center" valign="middle">

    <div style="background-image:url(<?php echo plugins_url('images/input.png',__FILE__) ?>); height:19px; width:153px; vertical-align:top">

    <input id="title" name="title" style="background:none; width:150px; height:15px; border:none; font-size:10px;" value="<?php echo $row->title;?>">

    </div>

    </td>
	
</tr>


<tr>

    <td width="300" align="right" valign="middle">

    <span style="font-size:16.76pt; font-family:BauhausItcTEEMed; color:#FFFFFF; vertical-align:middle;">Email to send submissions to:&nbsp;&nbsp;</span>

    </td>

    <td width="153" align="center" valign="middle">

    <div style="background-image:url(<?php echo plugins_url('images/input.png',__FILE__) ?>); height:19px; width:153px">

    <input id="mail" name="mail" style="background:none; width:151px; height:15px; border:none; font-size:11px" value="<?php echo $row->mail; ?>">

    </div>

    </td>

    </tr>






<tr>
    <td width="400" align="right" valign="top" style="padding-top:7px">
    <span style="font-size:16.76pt; font-family:BauhausItcTEEMed; color:#FFFFFF; vertical-align:middle;">The Post, which appears after submission:&nbsp;&nbsp;</span>
    </td>
<td class="paramlist_value" width="153" valign="middle" align="left" style="padding-top:7px">

<select name="post_name" style="width:153px; font-size:11px;" onchange="submit_form_postid(this)">
<option value="- Select Menu -">- Select Post -</option>
<?php
 
// The Query
query_posts($args );
 
// The Loop
while ( have_posts() ) : the_post(); ?>
	<option value="<?php $x=get_the_ID(); echo  $x; ?>" <?php if($row->article_id==$x){echo '  selected="selected"';} ?>>   <?php the_title();	?>	</option>
    <?php
endwhile;
 
// Reset Query
wp_reset_query();
 
?>
</select>

</td>




    </tr>





  <tr>
  <td align="left" colspan="3">
  
  <img src="<?php echo plugins_url('images/addanewfield.png',__FILE__) ?>" onclick="enable()" style="cursor:pointer;margin:10px;">

  </td>
  </tr>
  </tbody></table>
  <div id="formmakerDiv" onclick="enable()"></div>
  <div id="formmakerDiv1" align="center">
    
<table border="0" width="100%" cellpadding="0" cellspacing="0" height="100%" style="border:6px #00aeef solid; background-color:#FFF; font-size:10px">
  <tr>
    <td style="padding:0px">
    <table border="0" cellpadding="0" cellspacing="0" width="100%" height="100%" id="fonti">
        <tr valign="top">
         <td width="92" height="100%" style="border-right:dotted black 1px;" id="field_types">
         <div id="when_edit" style="display:none"></div>
          	<table border="0" cellpadding="0" cellspacing="0" width="100%">
              <tr>
               <td align="center" onClick="addRow('editor')" style="cursor:pointer" id="table_editor"><img src="<?php echo plugins_url('images/customHTML.png',__FILE__) ?> " style="margin:5px"/></td>
            
                <td align="center" onClick="addRow('text')" style="cursor:pointer" id="table_text"><img src="<?php echo plugins_url('images/text.png',__FILE__) ?>" style="margin:5px"/></td>
              </tr>
               <tr>
                <td align="center" onClick="addRow('time_and_date')" style="cursor:pointer" id="table_time_and_date"><img src="<?php echo plugins_url('images/time_and_date.png',__FILE__) ?>" style="margin:5px"/></td>
              
                <td align="center" onClick="addRow('select')" style="cursor:pointer" id="table_select"><img src="<?php echo plugins_url('images/select.png',__FILE__) ?>"style="margin:5px"/></td>
              </tr>
			  <tr>             
				 <td align="center" onClick="addRow('checkbox')" style="cursor:pointer" id="table_checkbox"><img src="<?php echo plugins_url('images/checkbox.png',__FILE__) ?>"style="margin:5px"/></td>
 
				<td align="center" onClick="addRow('radio')" style="cursor:pointer" id="table_radio"><img src="<?php echo plugins_url('images/radio.png',__FILE__) ?>"style="margin:5px"/></td>
             </tr>
              <tr>
                <td align="center" onClick="addRow('file_upload')" style="cursor:pointer" id="table_file_upload"><img src="<?php echo plugins_url('images/file_upload.png',__FILE__) ?>"style="margin:5px"/></td>
              
                <td align="center" onClick="addRow('captcha')" style="cursor:pointer" id="table_captcha"><img src="<?php echo plugins_url('images/captcha.png',__FILE__) ?>"style="margin:5px"/></td>
              </tr>
              <tr>
                  <td align="center" onClick="addRow('map')" style="cursor:pointer" id="table_map"><img src="<?php echo plugins_url('images/map.png',__FILE__) ?>"style="margin:5px"/></td>  

				  <td align="center" onClick="addRow('button')" style="cursor:pointer" id="table_button"><img src="<?php echo plugins_url('images/button.png',__FILE__) ?>"style="margin:5px"/></td>
              </tr>
            </table>
         </td>
         <td width="30%" height="100%" align="left"><div id="edit_table" style="padding:0px; overflow-y:scroll; height:520px" ></div></td>
   <td align="center" valign="top" style="background:url(<?php echo plugins_url('images/border2.png',__FILE__) ?>) repeat-y;">&nbsp;</td>
         <td width="60%" style="padding:15px">
         <table border="0" cellpadding="0" cellspacing="0" width="100%" height="100%" style="font-size:11px">
         
            <tr>
                <td align="right"><input type="radio" value="end" name="el_pos" checked="checked" id="pos_end" onclick="Disable()"/>
                  At The End
                  <input type="radio" value="begin" name="el_pos" id="pos_begin" onclick="Disable()" style=""/>
                  At The Beginning
                  <input type="radio" value="before" name="el_pos" id="pos_before" onclick="Enable()"/>
                  Before
                  <select style="width:100px; margin-left:5px" id="sel_el_pos" disabled="disabled">
                  </select>
                  <img alt="ADD" title="add" style="cursor:pointer; vertical-align:middle; margin:5px" src="<?php echo plugins_url('images/save.png',__FILE__) ?>" onClick="add(0)"/>
                  <img alt="CANCEL" title="cancel"  style=" cursor:pointer; vertical-align:middle; margin:5px" src="<?php echo plugins_url('images/cancel_but.png',__FILE__) ?>" onClick="close_window()"/>
				
                	<hr style=" margin-bottom:10px" />
                  </td>
              </tr>
              
              <tr height="100%" valign="top">
                <td  id="show_table"></td>
              </tr>
              
            </table>
         </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<input type="hidden" id="old" />
<input type="hidden" id="old_selected" />
<input type="hidden" id="element_type" />
<input type="hidden" id="editing_id" />
<input type="hidden" id="post_id" name="post_id">
<div id="main_editor" style="position:absolute; display:none; z-index:140;padding:10px;">
<div  style=" max-width:500px height:300px;text-align:left" id="poststuff">
<div id="<?php echo user_can_richedit() ? 'postdivrich' : 'postdiv'; ?>" class="postarea"><?php the_editor("","editor","title",$media_buttons = true, $tab_index = 1, $extended = true ); ?>
</div>
</div>
</div>
</div>
  <br />
  <br />


<fieldset>

    <legend>

    <h2 style="color:#00aeef">Form</h2>
    
    </legend>

     <style><?php echo $row->css; ?></style>



    <div id="take"><?php if($row->form)

	    echo $row->form;

	  else 

	    echo '<table border="0" cellpadding="4" cellspacing="0" class="form_view"><tbody  id="form_view" ><tr><td id="column_0" valign="top"><table><tbody></tbody></table></td></tr></tbody></table>'; ?></div>

    </fieldset>
  

    <input type="hidden" name="form" id="form" >

    <input type="hidden" id="label_order" name="label_order" value="<?php echo $row->label_order;?>" />
    <input type="hidden" name="counter" id="counter" value="<?php echo $row->counter;?>">
    <input type="hidden"  value="0" id="load_or_no" />
    </form>
   <script type="text/javascript">

function formOnload()
{
for(t=0; t<<?php echo $row->counter;?>; t++)
	if(document.getElementById(t+"_type"))
	{
		if(document.getElementById(t+"_type").value=="type_date")
				Calendar.setup({
						inputField: t+"_element",
						ifFormat: document.getElementById(t+"_button").getAttribute('format'),
						button: t+"_button",
						align: "Tl",
						singleClick: true,
						firstDay: 0
						});

	}
	
document.getElementById("load_or_no").value=1;
document.getElementById('form').value=document.getElementById('take').innerHTML;

}

function formAddToOnload()
{ 
	if(formOldFunctionOnLoad){ formOldFunctionOnLoad(); }
	formOnload();
}

function formLoadBody()
{
	formOldFunctionOnLoad = window.onload;
	window.onload = formAddToOnload;
}

var formOldFunctionOnLoad = null;

formLoadBody();


</script>
 <link type="text/css" rel="stylesheet" href="<?php echo plugins_url("js/calendar-jos.css",__FILE__) ?>" />
	  <script type="text/javascript" src="<?php echo plugins_url("js/formmaker.js",__FILE__) ?>"></script>
	  <script type="text/javascript" src="<?php echo plugins_url("js/calendar_function.js",__FILE__) ?>"></script>
	  <script type="text/javascript" src="<?php echo plugins_url("js/calendar.js",__FILE__) ?>"></script>
	  <script type="text/javascript" src="<?php echo plugins_url("js/calendar-setup.js",__FILE__) ?>"></script>


<?php


  
	
	
	
	
	
}






function forchrome($id){
?>
<script type="text/javascript">


window.onload=val; 

function val()
{
	  document.getElementById('adminForm').action="admin.php?page=Form_maker&task=gotoedit&id=<?php echo  $id;?>";
	  document.getElementById('adminForm').submit();
}

</script>
<form action="index.php" method="post" name="adminForm" id="adminForm">

    <input type="hidden" name="option" value="com_formmaker" />

    <input type="hidden" name="id" value="<?php echo $id;?>" />

    <input type="hidden" name="task" value="gotoedit" />
</form>
<?php
}










////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                                                       Function edit Form

function apply($id)
{
	global $wpdb;
	$form_no_slash=stripslashes($_POST["form"]);

		$count_words_in_form =count(explode("_element_section",$_POST["form"]))-count(explode("and_element_section",$_POST["form"]));
		if($count_words_in_form>5)
		{
			?>
			<div class="updated"><p><strong>The free version is limited up to 5 fields to add. If you need this functionality, you need to buy the commercial version</strong></p></div>
            <?php 
			return false;
		}
	?>
	<div class="updated"><p><strong><?php _e('Item Saved' ); ?></strong></p></div>
	<?php
	if($_POST["post_name"]=='- Select Menu -')
	{
		$article=0;
	}
	else
	{
		$article=$_POST["post_name"];
	}
		$savedd=$wpdb->update($wpdb->prefix."formmaker", array(
             'title'=>$_POST["title"],
             'mail'=>$_POST["mail"],
             'form'=>$form_no_slash,
			 'counter'=>$_POST["counter"],
			 'article_id'=>$article,
			 'label_order'=>$_POST["label_order"]
              ), 
              array('id'=>$id),
			  array(  '%s',
					  '%s',
					  '%s',
					  '%d',
					  '%s',
					  '%s')
			  
  );		

	
	return true;

}
















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                                                       Function edit Form

function save()
{
	global $wpdb;

	
$first_css='.form_view, .form_view table
{
width:inherit !important;
-webkit-border-horizontal-spacing: 0px;
-webkit-border-vertical-spacing: 0px;
border-bottom-color: gray;
border:0px  !important;
border-bottom-width: 0px;
border-collapse: separate;
border-left-color: gray;
border-left-width: 0px;
border-right-color: gray;
border-right-width: 0px;
border-top-color: gray;
border-top-width: 0px;
color: black;
display: table;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px !important;
font-weight: normal;
height: inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: left !important;

}

.form_view, .form_view tr
{
-webkit-border-horizontal-spacing: 0px;
-webkit-border-vertical-spacing: 0px;
border:0px  !important;
border-bottom-color: gray;
border-collapse: separate;
border-left-color: gray;
border-right-color: gray;
border-top-color: gray;
color: black;
display: table-row;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height: inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: left;
vertical-align: middle;
width:inherit !important;
}

.form_view, .form_view td
{
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
border-bottom-color: black;
border-collapse: separate;
border-left-color: black;
border-right-color: black;
border-top-color: black;
border:0px !important;
color: black;
display: table-cell;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height:inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 1px !important;
padding-left: 1px !important;
padding-right: 1px !important;
padding-top: 3px !important;
text-align: left !important;
width:inherit !important;
vertical-align:top;
}
.form_view, .form_view tr
{
-webkit-border-horizontal-spacing: 0px;
-webkit-border-vertical-spacing: 0px;
border:0px  !important;
border-bottom-color: gray;
border-collapse: separate;
border-left-color: gray;
border-right-color: gray;
border-top-color: gray;
color: black;
display: table-row;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height: inherit !important;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: left;
vertical-align: middle;
width:inherit !important;
}

.form_view, .form_view input,  .form_view  textarea
{
line-height:inherit  !important;
margin:0px !important;
min-height: 18px !important;
 font-size: 14px !important;
}
.form_view, .form_view select
{
margin:0px !important;
font-size: 14px !important;
}
.form_view, .form_view label
{
font-size: 14px;
 vertical-align:inherit !important;
}
.time_box
{
border-width:1px;
margin: 0px;
padding: 0px;
text-align:right;
width:30px;
vertical-align:middle
}


.mini_label
{
color: #000 !important;
font-size:14px;
font-family: Lucida Grande, Tahoma, Arial, Verdana, sans-serif;
}

.ch_rad_label
{
color:#000 !important;
display:inline;
margin-left:5px;
margin-right:15px;
float:none;
}

.label
{
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
border-bottom-color: black;
border-bottom-style: none;
border-collapse: separate;
border-left-color: black;
border-left-style: none;
border-right-color: black;
border-right-style: none;
border-top-color: black;
border-top-style: none;
color: black;
display: inline;
font-family: Helvetica, Arial, sans-serif;
font-size: 14px;
font-weight: normal;
height: auto;
line-height: 15px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: -webkit-left;
width: auto;
}


.td_am_pm_select
{
padding-left:5;
}

.am_pm_select
{
height: 16px;
margin:0;
padding:0
}

.input_deactive
{
background-color: #FFFFFF;
border-bottom-style: inset;
border-bottom-width: 1px;
border-collapse: separate;
border-left-color: #EEE;
border-left-style: inset;
border-left-width: 1px;
border-right-color: #EEE;
border-right-style: inset;
border-right-width: 1px;
border-top-color: #EEE;
border-top-style: inset;
border-top-width: 1px;
font-style: italic;
color: #999;
cursor: auto;
display: inline-block;
font-family: Arial;
font-size: 14px !important;
font-weight: normal;
letter-spacing: normal;
line-height: normal;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: -webkit-auto;
text-indent: 0px;
text-shadow: none;
text-transform: none;
word-spacing: 0px;
}

.input_active
{
background-color: #FFFFFF;
-webkit-appearance: none;
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
-webkit-rtl-ordering: logical;
-webkit-user-select: text;
background-color: white;
border-bottom-color: #EEE;
border-bottom-style: inset;
border-bottom-width: 1px;
border-collapse: separate;
border-left-color: #EEE;
border-left-style: inset;
border-left-width: 1px;
border-right-color: #EEE;
border-right-style: inset;
border-right-width: 1px;
border-top-color: #EEE;
border-top-style: inset;
border-top-width: 1px;
color: black;
cursor: auto;
display: inline-block;
font-family: Arial;
font-size: 14px !important;
font-style: normal;
font-weight: normal;
height: 16px;
letter-spacing: normal;
line-height: normal;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: -webkit-auto;
text-indent: 0px;
text-shadow: none;
text-transform: none;
width: 200px;
word-spacing: 0px;
}

.required
{
border:none;
color:red
}

.captcha_img
{
border-width:0px;
margin: 0px;
padding: 0px;
cursor:pointer;


}

.captcha_refresh
{
width:18px;
border-width:0px;
margin: 0px;
padding: 0px;
vertical-align:middle;
cursor:pointer;
}

.captcha_input
{
height:20px;
border-width:1px;
margin: 0px;
padding: 0px;
vertical-align:middle;
}

.file_upload
{
-webkit-appearance: none;
-webkit-border-horizontal-spacing: 2px;
-webkit-border-vertical-spacing: 2px;
-webkit-box-align: baseline;
-webkit-rtl-ordering: logical;
-webkit-user-select: text;
background-color: transparent;
border-bottom-color: black;
border-bottom-style: none;
border-bottom-width: 0px;
border-collapse: separate;
border-left-color: black;
border-left-style: none;
border-left-width: 0px;
border-right-color: black;
border-right-style: none;
border-right-width: 0px;
border-top-color: black;
border-top-style: none;
border-top-width: 0px;
color: black;
cursor: auto;
display: inline-block;
font-family: Arial;
font-size: 13px;
font-weight: normal;
height: 22px;
letter-spacing: normal;
line-height: normal;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
margin-top: 0px;
padding-bottom: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 0px;
text-align: start;
text-indent: 0px;
text-shadow: none;
text-transform: none;
width: 238px;
word-spacing: 0px;
}         
.captcha_table , .captcha_table input
{
  font-size: 15px !important;
}  
';
if(isset($_POST["label_order"]) && isset($_POST["title"]) && isset($_POST["form"])){
	$no_slash_form = stripslashes($_POST['form']);
	 $count_words_in_form = count(explode("_element_section",$_POST["form"]))-count(explode("and_element_section",$_POST["form"]));	 
	if($count_words_in_form>5)
	{
		?>
		<div class="updated"><p><strong>The free version is limited up to 5 fields to add. If you need this functionality, you need to buy the commercial version.</strong></p></div>
        <?php
		return false;
	}
	
	 $save_or_no= $wpdb->insert($wpdb->prefix.'formmaker', array(
		'id'	=> NULL,
        'title'     => $_POST["title"],
        'mail'    => $_POST["mail"],
        'form'  =>$no_slash_form,
        'css'   =>$first_css,
        'javascript'  => '',
        'script1'    => '',
        'script2' => '',
        'data'   => '',
		'counter' => $_POST["counter"],
		'article_id' => $_POST["post_id"],
		'label_order' => $_POST["label_order"]
                ),
				array(
				'%d',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%d',
				'%d',
				'%s'				
				)
                );

	
	if(!$save_or_no)
	{
		?>
	<div class="updated"><p><strong><?php _e('Error. Please install plugin again'); ?></strong></p></div>
	<?php
		return false;
	}
	?>
	<div class="updated"><p><strong><?php _e('Item Saved'); ?></strong></p></div>
	<?php
	
    return true;
}
else
{
	?>
    <h1>Error</h1>
    <
    <?php
	exit;
}

	
    

	
}

function gotoedit(){

	?>
	<div class="updated"><p><strong><?php _e('Item Saved' ); ?></strong></p></div>
    <?php

}














/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Save edith css



function save_edit_css($id)
{
	global $wpdb;
	if($id)
	{
		if(!isset($_POST["css"]))
		{
			die("error not found css textarea");
		}
	$wpdb->update($wpdb->prefix."formmaker", array(
             'css'=>$_POST["css"],
              ), 
              array('id'=>$id),
			  array('%s')
			  
  );
		?>
		<div class="updated"><p><strong><?php _e('CSS Successfully Saved'); ?></strong></p></div>
		<?php
	}
	else
	{
		die("cannot get or genereted id");
	}
					
}



function save_javascript($id)
{
	global $wpdb;
	if($id)
	{
		if(!isset($_POST["javascript"]))
		{
			die("error not found javascript textarea");
		}

		$wpdb->update($wpdb->prefix."formmaker", array(
             'javascript'=>stripslashes ($_POST["javascript"]),
              ), 
              array('id'=>$id),
			  array('%s')
  			);
		?>
		<div class="updated"><p><strong><?php _e('JavaScript Successfully Saved'); ?></strong></p></div>
		<?php
	}
	else
	{
		die("cannot get or genereted id");
	}
					
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                       update custon text in email
function update_custom_text($id)
{
	global $wpdb;
	
	if(isset($_POST["text_mail_befor"]))
	{
		if(isset($_POST["text_mail_after"]))
		{
			$wpdb->update($wpdb->prefix."formmaker", array(
             'script1'=>$_POST["text_mail_befor"],
             'script2'=>$_POST["text_mail_after"]
              ), 
              array('id'=>$id),
			  array('%s',
			  		'%s')
 			 );

			?>
			<div class="updated"><p><strong><?php _e('Custom text in email successfully saved' ); ?></strong></p></div>
            <?php
			
		}
			else
			{
				echo "error after text massage not found";
			}
	}
	else
	{
				echo "error befor text massage not found";
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

































	function savedata($id,$front_end)
	{
		global $wpdb;
		$all_files=array();
		@session_start();
		if(isset($_POST["captcha_input"]))
		{
			$captcha_input=$_POST["captcha_input"];
		}
		
		if(isset($_POST["counter"]))
		{
			$counter=$_POST["counter"];
		}

		if(isset($_POST["counter"]))
		{	
			if (isset($_POST["captcha_input"]))
			{	
			$session_wd_captcha_code=isset($_SESSION['wd_captcha_code'])?$_SESSION['wd_captcha_code']:'-';

				if($captcha_input==$session_wd_captcha_code)
				{
					$all_files=save_db($counter,$id,$front_end);
					if(is_numeric($all_files))		
						remove($all_files);
					else
						if(isset($counter))
							sendmail($counter, $all_files,$id,$front_end);

				}
				else
				{
							$front_end.="<script> alert('".addslashes(__('Error, incorrect Security code','form_maker'))."');
						</script>";
				}
			}	
			else	
			{
				$all_files=save_db($counter,$id,$front_end);
				if(is_numeric($all_files))		
					remove($all_files);
				else
					if(isset($counter))
						sendmail($counter, $all_files,$id,$front_end);
	
			}


			return $all_files;
		}

		return $all_files;
			
			
	}
	
	
	
	
	
	
	
	
	
	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	function save_db($counter,$id,$front_end)
	{

		$chgnac=true;	
		$all_files=array();
		global $wpdb;
		
		$max = $wpdb->get_var("SELECT MAX( group_id ) FROM ".$wpdb->prefix."formmaker_submits" ); 

		for($i=0; $i<$counter; $i++)
		{
			if(isset($_POST[$i."_type"]))
			{
				$type=$_POST[$i."_type"];
			}
			else
			{
				$type="";
			}
			

			if($type!="type_map" and $type!="type_captcha" and $type!="type_submit_reset" and $type!="type_button")
			{
				if(isset($_POST[$i."_element_label"]))
				{
					$element_label=$_POST[$i."_element_label"];
				}
				if(isset($_POST[$i."_element_label"]))
				{	
					$value='';
					if(isset($_POST[$i."_element"]))
					{
						$element=$_POST[$i."_element"];
					}
					if($type=="type_hidden")
					{
						$value=$_POST[$element_label];
					}
					else
					if(isset($_POST[$i."_element"]))
						$value=$element;
					else
					{
						if(isset($_POST[$i."_hh"]))
					{
						$hh=$_POST[$i."_hh"];
					}
						if(isset($_POST[$i."_hh"]))
						{
							$ss=$_POST[$i."_ss"];
							if(isset($_POST[$i."_ss"]))
								$value=$_POST[$i."_hh"].':'.$_POST[$i."_mm"].':'.$_POST[$i."_ss"];
							else
								$value=$_POST[$i."_hh"].':'.$_POST[$i."_mm"];
								
							if(isset($_POST[$i."_am_pm"]))
							{
							$am_pm=$_POST[$i."_am_pm"];
							}
							if(isset($_POST[$i."_am_pm"]))
								$value=$value.' '.$_POST[$i."_am_pm"];
						}
						else
						{
							if(isset($_POST[$i."_element_first"]))
							{
							$element_first=$_POST[$i."_element_first"];
							}
								if(isset($_POST[$i."_element_first"]))
								{
									if(isset($_POST[$i."_element_title"]))
									{
										$element_title=$_POST[$i."_element_title"];
									}
									if(isset($_POST[$i."_element_title"]))
										$value=$_POST[$i."_element_title"].' '.$_POST[$i."_element_first"].' '.$_POST[$i."_element_last"].' '.$_POST[$i."_element_middle"];
									else
										$value=$_POST[$i."_element_first"].' '.$_POST[$i."_element_last"];
								}
								else
								{			
										if(isset($_FILES[$i.'_file']))
										{
											$file=$_FILES[$i.'_file'];
										}			

									if(isset($_FILES[$i.'_file']))
									{
										if($file['name'])
										{	
											$form=$wpdb->get_row("SELECT * FROM  ".$wpdb->prefix."formmaker WHERE id='".$id."'");

											$untilupload = $form->form;
											
											$pos1 = strpos($untilupload, "***destinationskizb".$i."***");
											$pos2 = strpos($untilupload, "***destinationverj".$i."***");
											$destination = substr($untilupload, $pos1+(23+(strlen($i)-1)), $pos2-$pos1-(23+(strlen($i)-1)));
											$pos1 = strpos($untilupload, "***extensionskizb".$i."***");
											$pos2 = strpos($untilupload, "***extensionverj".$i."***");
											$extension = substr($untilupload, $pos1+(21+(strlen($i)-1)), $pos2-$pos1-(21+(strlen($i)-1)));
											$pos1 = strpos($untilupload, "***max_sizeskizb".$i."***");
											$pos2 = strpos($untilupload, "***max_sizeverj".$i."***");
											$max_size = substr($untilupload, $pos1+(20+(strlen($i)-1)), $pos2-$pos1-(20+(strlen($i)-1)));
											
											$fileName = $file['name'];
											/*$destination = JPATH_SITE.DS.JRequest::getVar($i.'_destination');
											$extension = JRequest::getVar($i.'_extension');
											$max_size = JRequest::getVar($i.'_max_size');*/

											if($fileSize > $max_size*1024)
											{
												$front_end.="<script> alert('".addslashes(__('The file exceeds the allowed size of','form_maker'))." ".$max_size."'); </script>";
												return ($max+1);
											}
											
											$uploadedFileNameParts = explode('.',$fileName);
											$uploadedFileExtension = array_pop($uploadedFileNameParts);
											$to=strlen($fileName)-strlen($uploadedFileExtension)-1;
											
											$fileNameFree= substr($fileName,0, $to);
											$invalidFileExts = explode(',', $extension);
											$extOk = false;

											foreach($invalidFileExts as $key => $value)
											{
											if(  is_numeric(strpos(strtolower($value), strtolower($uploadedFileExtension) )) )
												{
													$extOk = true;
												}
											}
											 
											if ($extOk == false) 
											{
												$front_end.="<script> alert(".addslashes(__('Sorry, you are not allowed to upload this type of file','form_maker','form_maker')).");</script>";
												return ($max+1);
											}
											
											$fileTemp = $file['tmp_name'];
											$p=1;
											while(file_exists( $destination.DS.$fileName))
											{
											$to=strlen($file['name'])-strlen($uploadedFileExtension)-1;
											$fileName= substr($fileName,0, $to).'('.$p.').'.$uploadedFileExtension;
											$p++;
											}
											if(!move_uploaded_file($fileTemp, $destination."/".$fileName)) 
											{	
												$front_end.="<script> alert(".addslashes(__('Error, file cannot be moved','form_maker')).";</script>";
												return ($max+1);
											}

											$value= site_url().'/'.$destination.'/'.$fileName.'*@@url@@*';
							
											$file['tmp_name']=$destination."/".$fileName;
											array_push($all_files,$file);

										}
									}

									else
									{
										$start=-1;
										for($j=0; $j<100; $j++)
										{
										
											if(isset($_POST[$i."_element".$j]))
											{
												$start=$j;
												break;
											}
										}	
										if($start!=-1)
										{
											for($j=$start; $j<100; $j++)
											{
												if(isset($_POST[$i."_element".$j]))
												{													
													$value=$value.$_POST[$i."_element".$j].'<br/>';
												}
											}
										}
									}
								
							}
						}
					}
					
				    $date=date('r');
					$ip=$_SERVER['REMOTE_ADDR'];
					
					$ptn = "/[^a-zA-Z0-9_]/";
					$rpltxt = "";
					$element_label= preg_replace($ptn, $rpltxt, $element_label);
					
					$a=addslashes($element_label);
					$b=addslashes($value);
					$c=($max+1);
					$d="NOW()";
					$r=$wpdb->prefix."formmaker_submits";
			$wpdb->insert($r, array(
					'form_id'     => $id,
					'element_label'    => $a,
					'element_value'  => $b,
					'group_id'   => $c,
					'date'  => date('Y-m-d H:i:s'),
					'ip'    => $ip,
							),
							array(
				'%d',
				'%s',
				'%s',
				'%d',
				'%s',
				'%s'
			
				)
							);
			$chgnac=false;
				}
			}
		}
	if($chgnac)
	{		

			if(count($all_files)==0);
			{
				@session_start();
				 $_SESSION['message_after_submit']=addslashes(__('Nothing was submitted','form_maker'));
			wp_redirect( $_SERVER["REQUEST_URI"]);
			}
	}
	
	return $all_files;

	}
	
	function sendmail($counter, $all_files,$id,$front_end)
	{
		global $wpdb;
				$row=$wpdb->get_row("SELECT * FROM  ".$wpdb->prefix."formmaker WHERE id='".$id."'",0);
					if($row->mail)
					{
						$cc=array();
						$label_order_original= array();
						$label_order_ids= array();
						
						$label_all	= explode('#****#',$row->label_order);
						$label_all 	= array_slice($label_all,0, count($label_all)-1);   
						foreach($label_all as $key => $label_each) 
						{
							$label_id_each=explode('#**id**#',$label_each);
							$label_id=$label_id_each[0];
							array_push($label_order_ids,$label_id);
							
							$label_oder_each=explode('#**label**#', $label_id_each[1]);							
							$label_order_original[$label_id]=$label_oder_each[0];
						}
					
						$list='<table border="0" cellpadding="3" cellspacing="0" style="width:600px; border-top:1px solid #888888; border-left:1px solid #888888;">';
						foreach($label_order_ids as $key => $label_order_id)
						{
							$i=$label_order_id;
							if(isset($_POST[$i."_element_label"]))
							{
							$element_label=$_POST[$i."_element_label"];
							}
							if(isset($_POST[$i."_element_label"]))
							{	
								$element_label=$label_order_original[$element_label];
								
								$type=$_POST[$i."_type"];
								if($type=="type_submitter_mail")
									if($_POST[$i."_send"]=="yes")
											array_push($cc, $_POST[$i."_element"]);
							if(isset($_POST[$i."_element"]))
							{
								$element=$_POST[$i."_element"];
							}
								if(isset($_POST[$i."_element"]))
									$list=$list.'<tr valign="top"><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$element_label.'</td><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$_POST[$i."_element"].'</td></tr>';
								else
								{
									if(isset($_POST[$i."_hh"]))
							{
								$hh=$_POST[$i."_hh"];
							}
								if(isset($_POST[$i."_hh"]))
								{
							if(isset($_POST[$i."_ss"]))
							{
								$ss=$_POST[$i."_ss"];
							}
									if(isset($_POST[$i."_ss"]))
										$list=$list.'<tr valign="top"><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$element_label.'</td><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$_POST[$i."_hh"].':'.$_POST[$i."_mm"].':'.$_POST[$i."_ss"];
									else
										$list=$list.'<tr valign="top"><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$element_label.'</td><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$_POST[$i."_hh"].':'.$_POST[$i."_mm"];
							if(isset($_POST[$i."_am_pm"]))
							{
									$am_pm=$_POST[$i."_am_pm"];
							}
									if(isset($_POST[$i."_am_pm"]))
										$list=$list.' '.$_POST[$i."_am_pm"].'</td></tr>';
									else
										$list=$list.'</td></tr>';
								}
								else
								{
							if(isset($_POST[$i."_element_first"]))
							 {
								$element_first=$_POST[$i."_element_first"];
							 }
								if(isset($_POST[$i."_element_first"]))
								{
							if(isset($_POST[$i."_element_title"]))
							 {
								$element_title=$_POST[$i."_element_title"];
							 }
									if(isset($_POST[$i."_element_title"]))
										$list=$list.'<tr valign="top"><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$element_label.'</td><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$_POST[$i."_element_title"].' '.$_POST[$i."_element_first"].' '.$_POST[$i."_element_last"].' '.$_POST[$i."_element_middle"].'</td></tr>';
									else
										$list=$list.'<tr valign="top"><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$element_label.'</td><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$_POST[$i."_element_first"].' '.$_POST[$i."_element_last"].'</td></tr>';
								}
								else
								{
									if(isset($_FILES[$i.'_file']))
							 	{							
									$file = $_FILES[$i.'_file'];
								}
								
								if(isset($_FILES[$i.'_file']))
								{
								}
								else
								{
									$list=$list.'<tr valign="top"><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">'.$element_label.'</td><td style="border-right:1px solid #888888; border-bottom:1px solid #888888;">';
						
									$start=-1;
									for($j=0; $j<100; $j++)
									{
										if(isset($_POST[$i."_element".$j]))
										{
											$element=$_POST[$i."_element".$j];
										}
			
										if(isset($_POST[$i."_element".$j]))
										{
											$start=$j;
											break;
										}
									}	
									if($start!=-1)
									{
										for($j=$start; $j<100; $j++)
										{
										if(isset($_POST[$i."_element".$j]))
										{
											$element=$_POST[$i."_element".$j];
										}
											if(isset($_POST[$i."_element".$j]))
												$list=$list.$_POST[$i."_element".$j].'<br>';
										}
										$list=$list.'</td></tr>';
									}
								}
							}}}}
							
						}
						$list=$list.'</table>';
							$body   = $row->script1.'<br>'.$list.'<br>'.$row->script2;
							add_filter('wp_mail_content_type',create_function('', 'return "text/html";'));
							for($k=0;$k<count($all_files);$k++)
							{
							$attachments[$k] = dirname(__FILE__). '/uploads/'.$all_files[$k]['name'];
							}
							array_push($cc,$row->mail);
							
							$body = wordwrap($body, 70, "\n", true);
							
							$send =wp_mail( $cc, $row->title, $body,"",$attachments);
								if ( $send != true ) 
								{								  
										  if($row->article_id)
										  {	
										  @session_start();
										  $_SESSION['message_after_submit']=addslashes(__('Your form was successfully submitted','form_maker'));
											wp_redirect( get_permalink($row->article_id));
										  }
										  else
										  {
											  @session_start();
										  $_SESSION['message_after_submit']=addslashes(__('Error, email was not sent','form_maker'));
											wp_redirect($_SERVER["REQUEST_URI"]);
										  }
			
			
								} 
								else 
								{
										 if($row->article_id)
										 {
											 @session_start();
										  $_SESSION['message_after_submit']=addslashes(__('Your form was successfully submitted','form_maker'));
											  wp_redirect( get_permalink($row->article_id));
										  }
										  else
										  {
											  @session_start();
										  $_SESSION['message_after_submit']=addslashes(__('Your form was successfully submitted','form_maker'));
											   wp_redirect( $_SERVER["REQUEST_URI"]);
										  }				
								}								
							
						}
							
						
						else
						{
							if($row->article_id)
										 {
											 @session_start();
										  $_SESSION['message_after_submit']=addslashes(__('Your form was successfully submitted','form_maker'));
											  wp_redirect( get_permalink($row->article_id));
										 }
										  else
										  {
											  @session_start();
										  $_SESSION['message_after_submit']=addslashes(__('Your form was successfully submitted','form_maker'));
											   wp_redirect( $_SERVER["REQUEST_URI"]);
										  }	
			
						
						}	
					
			

	}
	function remove($group_id)
	{
		global $wpdb;
		$wpdb->query('DELETE FROM '.$wpdb->prefix.'formmaker_submits WHERE group_id="'.$group_id.'"');

	}
    function delete_submishions()
	{
		global $wpdb;
		if(isset($_POST["delete"]))
		if($_POST["delete"]!='0')
		{
			if($wpdb->query('DELETE FROM '.$wpdb->prefix.'formmaker_submits WHERE group_id="'.$_POST["delete"].'"'))
				{
					?>
					<div class="updated"><p><strong><?php _e('Item Deleted.' ); ?></strong></p></div> 
					<?php
				}
		}
		if(isset($_POST["cid"]))
		{
			if($_POST["idd"]!='0')
			{
			$b=true;
			foreach($_POST["cid"] as $delete_id)
			{
			
				if($wpdb->query('DELETE FROM '.$wpdb->prefix.'formmaker_submits WHERE group_id="'.$delete_id.'"'))
				{
					
				}
				else
				{
					$b=false;
				}
			}
			if($b)
			{
				?>
                <div class="updated"><p><strong><?php _e('Items Deleted.' ); ?></strong></p></div> 
                <?php
			}
			
		}
		}
		
		
	}
	
	
?>