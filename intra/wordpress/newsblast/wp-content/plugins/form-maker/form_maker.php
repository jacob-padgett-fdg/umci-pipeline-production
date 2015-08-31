<?php
/*
Plugin Name: Form Maker
Plugin URI: http://web-dorado.com/products/form-maker-wordpress.html
Version: 1.2.5
Author: http://web-dorado.com/
License: GNU/GPLv3 http://www.gnu.org/licenses/gpl-3.0.html
*/

//// load languages
add_action( 'init', 'form_maker_language_load' );

function form_maker_language_load() {
	 load_plugin_textdomain('form_maker', false, basename( dirname( __FILE__ ) ) . '/languages' );
}
add_action('init', 'do_output_buffer');
function do_output_buffer() {
        ob_start();
}


function form_shotrcode($atts) {
     extract(shortcode_atts(array(
	      'id' => 'no Form',
     ), $atts));
     return front_end_Form_Maker($id);
}
add_shortcode('Form', 'form_shotrcode');


function my_scripts_method() {
    			wp_enqueue_script("main__js",plugins_url("main.js",__FILE__),false);
				wp_enqueue_script("Gmap","http://maps.google.com/maps/api/js?sensor=false",false);
				wp_enqueue_script("Calendar",plugins_url("js/calendar.js",__FILE__),false);
 			    wp_enqueue_script("calendar-setup",plugins_url("js/calendar-setup.js",__FILE__),false);
				wp_enqueue_script("calendar_function",plugins_url("js/calendar_function.js",__FILE__),false);
				wp_enqueue_style("Css",plugins_url("js/calendar-jos.css",__FILE__),false); 
				wp_enqueue_style("gmap_styles_",plugins_url("style.css",__FILE__),false);         
}    
 
add_action('wp_enqueue_scripts', 'my_scripts_method');



///////////////////////////// FORNT END Print message



function print_massage($content)
{
	       @session_start();
			if( isset($_SESSION['message_after_submit']))
			{
				if($_SESSION['message_after_submit']!="")
				{

				$message=$_SESSION['message_after_submit'];
				$_SESSION['message_after_submit']="";
				

			$returned_content="<div style='background-color:#FFFFE0; padding:4px; border-color: #E6DB55;'><h2><strong>".$message."</strong></h2></div>".$content;// modified content
			return $returned_content;
				}
				else
				{
					return $content;
				}
			}
			else
			{
				return $content;
			}
}


add_filter('the_content', 'print_massage'); 


///////////////////////////// FORNT END FUNCTION  


function front_end_Form_Maker($id)  
{
				global $wpdb;
				@session_start();
				$_SESSION['wd_captcha_code'];	
				$FC_frontend="";
				$all_form_ids=$wpdb->get_col("SELECT id FROM ".$wpdb->prefix."formmaker");
				$b=false;
				foreach($all_form_ids as $all_form_id)
				{
					if($all_form_id==$id)
					$b=true;
				}
				if(!$b)
				return "";				
				$ok		= savedata($id,$FC_frontend);				
				if(is_numeric($ok))	
				{	
						remove($ok);
				}
				$all_id_form=$wpdb->get_col("SELECT id FROM  ".$wpdb->prefix."formmaker",0);
				$if_id_exisst=false;
				foreach($all_id_form as $_id_form)
				{
					if($_id_form==$id)
					{
						$if_id_exisst=true;
					}
				}
				if(!$if_id_exisst)
				{
					return;
				}				
				$row=$wpdb->get_row("SELECT * FROM  ".$wpdb->prefix."formmaker WHERE id='".$id."'",0);
					$FC_frontend.='<script type="text/javascript">'.str_replace ("
"," ",$row->javascript).'</script>';
					$FC_frontend.='<style>'.str_replace ("
"," ",$row->css ).'</style>';
					$FC_frontend.="<form name=\"form\" action=\"".$_SERVER['REQUEST_URI']."\" method=\"post\" id=\"form\" enctype=\"multipart/form-data\">
									<input type=\"hidden\" id=\"counter\" value=\"".$row->counter."\" name=\"counter\" />
									<input type=\"hidden\" id=\"Itemid\" value=\"".$Itemid."\" name=\"Itemid\" />";
                   $captcha_url=plugins_url("wd_captcha.php",__FILE__).'?digit=';
				   $captcha_rep_url=plugins_url("wd_captcha.php",__FILE__).'?r2='.mt_rand(0,1000).'&digit=';
				   			$rep1=array(
			"<!--repstart-->Title<!--repend-->",
			"<!--repstart-->First<!--repend-->",
			"<!--repstart-->Last<!--repend-->",
			"<!--repstart-->Middle<!--repend-->",
			"<!--repstart-->January<!--repend-->",
			"<!--repstart-->February<!--repend-->",
			"<!--repstart-->March<!--repend-->",
			"<!--repstart-->April<!--repend-->",
			"<!--repstart-->May<!--repend-->",
			"<!--repstart-->June<!--repend-->",
			"<!--repstart-->July<!--repend-->",
			"<!--repstart-->August<!--repend-->",
			"<!--repstart-->September<!--repend-->",
			"<!--repstart-->October<!--repend-->",
			"<!--repstart-->November<!--repend-->",
			"<!--repstart-->December<!--repend-->",
			$captcha_url,
			'class="captcha_img"',
			 plugins_url('images/refresh.png',__FILE__),
			 plugins_url('images/delete_el.png',__FILE__),
			 plugins_url('images/up.png',__FILE__),
			 plugins_url('images/down.png',__FILE__),
			 plugins_url('images/left.png',__FILE__),
			 plugins_url('images/right.png',__FILE__),
			 plugins_url('images/edit.png',__FILE__));
			$rep2=array(
			addslashes(__("Title","form_maker")),
			addslashes(__("First","form_maker")),
			addslashes(__("Last","form_maker")),
			addslashes(__("Middle","form_maker")),
			addslashes(__("January","form_maker")),
			addslashes(__("February","form_maker")),
			addslashes(__("March","form_maker")),
			addslashes(__("April","form_maker")),
			addslashes(__("May","form_maker")),
			addslashes(__("June","form_maker")),
			addslashes(__("July","form_maker")),
			addslashes(__("August","form_maker")),
			addslashes(__("September","form_maker")),
			addslashes(__("October","form_maker")),
			addslashes(__("November","form_maker")),
			addslashes(__("December","form_maker")),
			$captcha_rep_url,
			'class="captcha_img" style="display:none"',
			 plugins_url('images/refresh.png',__FILE__),
			'','','','','','');
			$untilupload = str_replace($rep1,$rep2,$row->form);
				   while(strpos($untilupload, "***destinationskizb")>0)
			{
				$pos1 = strpos($untilupload, "***destinationskizb");
				$pos2 = strpos($untilupload, "***destinationverj");
				$untilupload=str_replace(substr($untilupload, $pos1, $pos2-$pos1+22), "", $untilupload);
			}
				   $FC_frontend.=$untilupload;
				   $FC_frontend.="<script type=\"text/javascript\">
							function formOnload()
							{
								if(document.getElementById(\"wd_captcha_input\"))
									captcha_refresh('wd_captcha');
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
							";							
				if(isset($_POST["captcha_input"]))
				{						
					$captcha_input=$_POST["captcha_input"];
				}
				if(isset($_POST["counter"]))
				{						
					$counter=$_POST["counter"];
				}
				if(isset($counter))
				if (isset($captcha_input) or is_numeric($ok))
				{
				$session_wd_captcha_code=isset($_SESSION['wd_captcha_code'])?$_SESSION['wd_captcha_code']:'-';
				if($captcha_input!=$session_wd_captcha_code or is_numeric($ok))
				{
				for($i=0; $i<$counter; $i++)
				{
					if(isset($_POST[$i."_type"]))
					{						
						$type=$_POST[$i."_type"];
					}
					if(isset($_POST[$i."_type"]))
					{	
						switch ($type)
						{
						case "type_text":
						
						case "type_submitter_mail":{
											 $FC_frontend.= 
				"if(document.getElementById('".$i."_element"."').title!='".addslashes($_POST[$i."_element"])."')
				{	document.getElementById('".$i."_element"."').value='".addslashes($_POST[$i."_element"])."';
					document.getElementById('".$i."_element"."').style.color='#000000';
					document.getElementById('".$i."_element"."').style.fontStyle='normal !important';
				}
				";
											break;
										}
												
						case "type_textarea":{
											 $FC_frontend.= 
				"if(document.getElementById('".$i."_element"."').title!='".addslashes($_POST[$i."_element"])."')
				{	document.getElementById('".$i."_element"."').innerHTML='".addslashes($_POST[$i."_element"])."';
					document.getElementById('".$i."_element"."').style.color='#000000';
					document.getElementById('".$i."_element"."').style.fontStyle='normal';
				}
				";
									break;
										}
						case "type_password":{
											 $FC_frontend.= 
				"document.getElementById('".$i."_element"."').value='';
				";						break;
										}
						case "type_name":{
											if(isset($_POST[$i."_element_title"]))
											{
												 $FC_frontend.= 
				"document.getElementById('".$i."_element_title"."').value='".addslashes($_POST[$i."_element_title"])."';
				document.getElementById('".$i."_element_first"."').value='".addslashes($_POST[$i."_element_first"])."';
				document.getElementById('".$i."_element_last"."').value='".addslashes($_POST[$i."_element_last"])."';
				document.getElementById('".$i."_element_middle"."').value='".addslashes($_POST[$i."_element_middle"])."';
				";
											}
											else
											{
											 $FC_frontend.= 
				"document.getElementById('".$i."_element_first"."').value='".addslashes($_POST[$i."_element_first"])."';
				document.getElementById('".$i."_element_last"."').value='".addslashes($_POST[$i."_element_last"])."';
				";						}
											break;
										}
						case "type_checkbox":{
											 $FC_frontend.=
				"for(k=0; k<20; k++)
					if(document.getElementById('".$i."_element'+k))
						document.getElementById('".$i."_element'+k).removeAttribute('checked');
					else break;	";			for($j=0; $j<100; $j++)
											{
												if(isset($_POST[$i."_element".$j]))
															{
															 $FC_frontend.=
				"document.getElementById('".$i."_element".$j."').setAttribute('checked', 'checked');
				";									}
											}
											break;
											}
						case "type_radio":{
											 $FC_frontend.=
				"for(k=0; k<100; k++)
					if(document.getElementById('".$i."_element'+k))
					{
						document.getElementById('".$i."_element'+k).removeAttribute('checked');
						if(document.getElementById('".$i."_element'+k).value=='".addslashes($_POST[$i."_element"])."')
							document.getElementById('".$i."_element'+k).setAttribute('checked', 'checked');
					}
					else break;
				";						break;
										}
						case "type_time":{
											if(isset($_POST[$i."_ss"]))
											{
												 $FC_frontend.= 
				"document.getElementById('".$i."_hh"."').value='".$_POST[$i."_hh"]."';
				document.getElementById('".$i."_mm"."').value='".$_POST[$i."_mm"]."';
				document.getElementById('".$i."_ss"."').value='".$_POST[$i."_ss"]."';
				";					}
											else
											{
												 $FC_frontend.= 
				"document.getElementById('".$i."_hh"."').value='".$_POST[$i."_hh"]."';
				document.getElementById('".$i."_mm"."').value='".$_POST[$i."_mm"]."';
				";
											}
											if(isset($_POST[$i."_am_pm"]))
												 $FC_frontend.= 
				"document.getElementById('".$i."_am_pm').value='".$_POST[$i."_am_pm"]."';
				";						break;
										}										
						case "type_date":{	 $FC_frontend.="document.getElementById('".$i."_element"."').value='".$_POST[$i."_element"]."';
				";						break;
										}										
						case "type_date_fields":{
							$date_fields=explode('-',$_POST[$i."_element"]);
												 $FC_frontend.= 
				"document.getElementById('".$i."_day"."').value='".$date_fields[0]."';
				document.getElementById('".$i."_month"."').value='".$date_fields[1]."';
				document.getElementById('".$i."_year"."').value='".$date_fields[2]."';
				";						break;
										}										
					case "type_country":{
											$FC_frontend.="document.getElementById('".$i."_element').value='".addslashes($_POST[$i."_element"])."';
				";						break;
										}									
						case "type_own_select":{
												 $FC_frontend.=
				"document.getElementById('".$i."_element').value='".addslashes($_POST[$i."_element"])."';
				";
								break;
										}										
						case "type_file":{
										break;
									}				
						}
					}
				}
			}
		}
		
 $FC_frontend.="n=".$row->counter.";
	for(i=0; i<n; i++)
	{
		if(document.getElementById(i))
		{	
			for(z=0; z<document.getElementById(i).childNodes.length; z++)
				if(document.getElementById(i).childNodes[z].nodeType==3)
					document.getElementById(i).removeChild(document.getElementById(i).childNodes[z]);		
			if(document.getElementById(i).childNodes[7])
			{			
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
			}
			else
			{
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
				document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
			}
		}
	}	
	for(i=0; i<=n; i++)
	{	
		if(document.getElementById(i))
		{
			type=document.getElementById(i).getAttribute(\"type\");
				switch(type)
				{	case \"type_text\":
					case \"type_password\":
					case \"type_submitter_mail\":
					case \"type_own_select\":
					case \"type_country\":
					case \"type_hidden\":
					case \"type_map\":
					{
						remove_add_(i+\"_element\");
						break;
					}					
					case \"type_submit_reset\":
					{
						remove_add_(i+\"_element_submit\");
						if(document.getElementById(i+\"_element_reset\"))
							remove_add_(i+\"_element_reset\");
						break;
					}					
					case \"type_captcha\":
					{	remove_add_(\"wd_captcha\");
						remove_add_(\"element_refresh\");
						remove_add_(\"wd_captcha_input\");
						break;
					}						
					case \"type_file_upload\":
						{	remove_add_(i+\"_element\");
							if(document.getElementById(i+\"_element\").value==\"\")
							{	
								seted=false;
								break;
							}
							ext_available=getfileextension(i);
							if(!ext_available)
								seted=false;										
								break;
						}						
					case \"type_textarea\":
						{
						remove_add_(i+\"_element\");							if(document.getElementById(i+\"_element\").innerHTML==document.getElementById(i+\"_element\").title || document.getElementById(i+\"_element\").innerHTML==\"\")
								seted=false;
								break;
						}						
					case \"type_name\":
						{						
						if(document.getElementById(i+\"_element_title\"))
							{
							remove_add_(i+\"_element_title\");
							remove_add_(i+\"_element_first\");
							remove_add_(i+\"_element_last\");
							remove_add_(i+\"_element_middle\");
								if(document.getElementById(i+\"_element_title\").value==\"\" || document.getElementById(i+\"_element_first\").value==\"\" || document.getElementById(i+\"_element_last\").value==\"\" || document.getElementById(i+\"_element_middle\").value==\"\")
									seted=false;
							}
							else
							{
							remove_add_(i+\"_element_first\");
							remove_add_(i+\"_element_last\");
								if(document.getElementById(i+\"_element_first\").value==\"\" || document.getElementById(i+\"_element_last\").value==\"\")
									seted=false;
							}
							break;
						}						
					case \"type_checkbox\":
					case \"type_radio\":
						{	is=true;
							for(j=0; j<100; j++)
								if(document.getElementById(i+\"_element\"+j))
								{
							remove_add_(i+\"_element\"+j);
									if(document.getElementById(i+\"_element\"+j).checked)
									{
										is=false;										
										break;
									}
								}
							if(is)
							seted=false;
							break;
						}						
					case \"type_button\":
						{
							for(j=0; j<100; j++)
								if(document.getElementById(i+\"_element\"+j))
								{
									remove_add_(i+\"_element\"+j);
								}
							break;
						}						
					case \"type_time\":
						{	
						if(document.getElementById(i+\"_ss\"))
							{
							remove_add_(i+\"_ss\");
							remove_add_(i+\"_mm\");
							remove_add_(i+\"_hh\");
								if(document.getElementById(i+\"_ss\").value==\"\" || document.getElementById(i+\"_mm\").value==\"\" || document.getElementById(i+\"_hh\").value==\"\")
									seted=false;
							}
							else
							{
							remove_add_(i+\"_mm\");
							remove_add_(i+\"_hh\");
								if(document.getElementById(i+\"_mm\").value==\"\" || document.getElementById(i+\"_hh\").value==\"\")
									seted=false;
							}
							break;
						}						
					case \"type_date\":
						{	
						remove_add_(i+\"_element\");
						remove_add_(i+\"_button\");						
							if(document.getElementById(i+\"_element\").value==\"\")
								seted=false;
							break;
						}
					case \"type_date_fields\":
						{	
						remove_add_(i+\"_day\");
						remove_add_(i+\"_month\");
						remove_add_(i+\"_year\");
						if(document.getElementById(i+\"_day\").value==\"\" || document.getElementById(i+\"_month\").value==\"\" || document.getElementById(i+\"_year\").value==\"\")
							seted=false;
								break;
					}
				}						
		}
	}	
function check_year2(id)
{
	year=document.getElementById(id).value;	
	from=parseFloat(document.getElementById(id).getAttribute('from'));	
	year=parseFloat(year);	
	if(year<from)
	{
		document.getElementById(id).value='';
		alert('".addslashes(__('The value of year is not valid','form_maker'))."');
	}
}	
function remove_add_(id)
{
attr_name= new Array();
attr_value= new Array();
var input = document.getElementById(id); 
atr=input.attributes;
for(v=0;v<30;v++)
	if(atr[v] )
	{
		if(atr[v].name.indexOf(\"add_\")==0)
		{
			attr_name.push(atr[v].name.replace('add_',''));
			attr_value.push(atr[v].value);
			input.removeAttribute(atr[v].name);
			v--;
		}
	}
for(v=0;v<attr_name.length; v++)
{
	input.setAttribute(attr_name[v],attr_value[v])
}
}	
function getfileextension(id) 
{ 
 var fileinput = document.getElementById(id+\"_element\"); 
 var filename = fileinput.value; 
 if( filename.length == 0 ) 
 return true; 
 var dot = filename.lastIndexOf(\".\"); 
 var extension = filename.substr(dot+1,filename.length); 
 var exten = document.getElementById(id+\"_extension\").value.replace(\"***extensionverj\"+id+\"***\", \"\").replace(\"***extensionskizb\"+id+\"***\", \"\");
 exten=exten.split(','); 
 for(x=0 ; x<exten.length; x++)
 {
  exten[x]=exten[x].replace(/\./g,'');
  exten[x]=exten[x].replace(/ /g,'');
  if(extension.toLowerCase()==exten[x].toLowerCase())
  	return true;
 }
 return false; 
} 
function check_required(but_type)
{
	if(but_type=='reset')
	{
	window.location.reload( true );
	return;
	}	
	n=".$row->counter.";
	ext_available=true;
	seted=true;
	for(i=0; i<=n; i++)
	{	
		if(seted)
		{		
			if(document.getElementById(i))
			    if(document.getElementById(i+\"_required\"))
				if(document.getElementById(i+\"_required\").value==\"yes\")
				{
					type=document.getElementById(i).getAttribute(\"type\");
					switch(type)
					{
						case \"type_text\":
						case \"type_password\":
						case \"type_submitter_mail\":
						case \"type_own_select\":
						case \"type_country\":
							{
								if(document.getElementById(i+\"_element\").value==document.getElementById(i+\"_element\").title || document.getElementById(i+\"_element\").value==\"\")
									seted=false;
									break;
							}							
						case \"type_file_upload\":
							{
								if(document.getElementById(i+\"_element\").value==\"\")
								{	
									seted=false;
									break;
								}
								ext_available=getfileextension(i);
								if(!ext_available)
									seted=false;											
									break;
							}							
						case \"type_textarea\":
							{
								if(document.getElementById(i+\"_element\").innerHTML==document.getElementById(i+\"_element\").title || document.getElementById(i+\"_element\").innerHTML==\"\")
									seted=false;
									break;
							}							
						case \"type_name\":
							{	
							if(document.getElementById(i+\"_element_title\"))
								{
									if(document.getElementById(i+\"_element_title\").value==\"\" || document.getElementById(i+\"_element_first\").value==\"\" || document.getElementById(i+\"_element_last\").value==\"\" || document.getElementById(i+\"_element_middle\").value==\"\")
										seted=false;
								}
								else
								{
									if(document.getElementById(i+\"_element_first\").value==\"\" || document.getElementById(i+\"_element_last\").value==\"\")
										seted=false;
								}
								break;	
							}							
						case \"type_checkbox\":
						case \"type_radio\":
							{
								is=true;
								for(j=0; j<100; j++)
									if(document.getElementById(i+\"_element\"+j))
										if(document.getElementById(i+\"_element\"+j).checked)
										{
											is=false;										
											break;
										}
								if(is)
								seted=false;
								break;
							}					
						case \"type_time\":
							{	
							if(document.getElementById(i+\"_ss\"))
								{
									if(document.getElementById(i+\"_ss\").value==\"\" || document.getElementById(i+\"_mm\").value==\"\" || document.getElementById(i+\"_hh\").value==\"\")
										seted=false;
								}
								else
								{
									if(document.getElementById(i+\"_mm\").value==\"\" || document.getElementById(i+\"_hh\").value==\"\")
										seted=false;
								}
								break;	
							}							
						case \"type_date\":
							{	
								if(document.getElementById(i+\"_element\").value==\"\")
									seted=false;
								break;
							}
						case \"type_date_fields\":
							{	
								if(document.getElementById(i+\"_day\").value==\"\" || document.getElementById(i+\"_month\").value==\"\" || document.getElementById(i+\"_year\").value==\"\")
									seted=false;
								break;
							}
							}						
				}
				else
				{	
					type=document.getElementById(i).getAttribute(\"type\");
					if(type==\"type_file_upload\")
						ext_available=getfileextension(i);
							if(!ext_available)
							seted=false;											
				}
		}
		else
		{		
			if(!ext_available)
				{alert('".addslashes(__('Sorry, you are not allowed to upload this type of file','form_maker'))."');
				break;}			
			x=document.getElementById(i-1+'_element_label');
			while(x.firstChild)
			{
				x=x.firstChild;
			}
			alert(x.nodeValue+' ".addslashes(__('field is required','form_maker'))."');
			break;
		}		
	}
	if(seted)
	for(i=0; i<=n; i++)
	{	
		if(document.getElementById(i))
			if(document.getElementById(i).getAttribute(\"type\")==\"type_submitter_mail\")
				if (document.getElementById(i+\"_element\").value!='')	if(document.getElementById(i+\"_element\").value.search(/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/) == -1)
				{		alert( \"".addslashes(__('This is not a valid email address','form_maker'))."\" );	
							return;
				}	
	}
	if(seted)
		create_headers();
}	
function create_headers()
{	form_=document.getElementById('form');
	n=".$row->counter.";
	for(i=0; i<n; i++)
	{	if(document.getElementById(i))
		{if(document.getElementById(i).getAttribute(\"type\")!=\"type_map\")
		if(document.getElementById(i).getAttribute(\"type\")!=\"type_captcha\")
		if(document.getElementById(i).getAttribute(\"type\")!=\"type_submit_reset\")
		if(document.getElementById(i).getAttribute(\"type\")!=\"type_button\")
			if(document.getElementById(i+'_element_label'))
			{	var input = document.createElement('input');
				input.setAttribute(\"type\", 'hidden');
				input.setAttribute(\"name\", i+'_element_label');
				input.value=i;
				form_.appendChild(input);
				if(document.getElementById(i).getAttribute(\"type\")==\"type_date_fields\")
				{		var input = document.createElement('input');
						input.setAttribute(\"type\", 'hidden');
						input.setAttribute(\"name\", i+'_element');					input.value=document.getElementById(i+'_day').value+'-'+document.getElementById(i+'_month').value+'-'+document.getElementById(i+'_year').value;
					form_.appendChild(input);
				}
			}
		}
	}
form_.submit();
}	
</script>
</form>";

/*for($i=0;$i<5;$i++)
$FC_frontend=str_replace ("

" , "
", $FC_frontend);*/

return  $FC_frontend;

}





//// add front end



//// add editor new mce button
add_filter('mce_external_plugins', "Form_Maker_register");
add_filter('mce_buttons', 'Form_Maker_add_button', 0);

/// function for add new button
function Form_Maker_add_button($buttons)
{
    array_push($buttons, "Form_Maker_mce");
    return $buttons;
}
 /// function for registr new button
function Form_Maker_register($plugin_array)
{
    $url = plugins_url( 'js/editor_plugin.js' , __FILE__ ); 
    $plugin_array["Form_Maker_mce"] = $url;
    return $plugin_array;
}











function add_button_style1()
{
echo '<style type="text/css">
.wp_themeSkin span.mce_Form_Maker_mce {background:url('.plugins_url( 'images/formmakerLogo.png' , __FILE__ ).') no-repeat !important;}
.wp_themeSkin .mceButtonEnabled:hover span.mce_Form_Maker_mce,.wp_themeSkin .mceButtonActive span.mce_Form_Maker_mce
{background:url('.plugins_url( 'images/formmakerLogoHover.png' , __FILE__ ).') no-repeat !important;}
</style>';
}

add_action('admin_head', 'add_button_style1');










































require_once("functions.php");

add_action('admin_menu', 'Form_maker_options_panel');
function Form_maker_options_panel(){
	 $icon_url=plugins_url( 'images/formmakerLogoHover.png' , __FILE__ );
  add_menu_page('Theme page title', 'Form Maker', 'manage_options', 'Form_maker', 'Manage_Form_maker', $icon_url);
  add_submenu_page( 'Form_maker', 'Form Maker Manager', 'Manager', 'manage_options', 'Form_maker', 'Manage_Form_maker');
  add_submenu_page( 'Form_maker', 'Form Maker  submissions', 'Submissions', 'manage_options', 'Form_maker_Submits', 'Form_maker_Submits');
  add_submenu_page( 'Form_maker', 'Uninstall Form Maker ', 'Uninstall Form Maker', 'manage_options', 'Uninstall_Form_Maker', 'Uninstall_Form_Maker');
}

function Manage_Form_maker()
{
	global $wpdb;
	wp_enqueue_script('word-count');
	  wp_enqueue_script('post');
	  wp_enqueue_script('editor');
	  wp_enqueue_script('media-upload');
	  wp_admin_css('thickbox');
	  wp_print_scripts('media-upload');
	  wp_print_scripts('editor-functions');
	  do_action('admin_print_styles');
   	wp_enqueue_script( 'common' );
	wp_enqueue_script( 'jquery-color' );
	wp_print_scripts('editor');
	if (function_exists('add_thickbox')) add_thickbox();
	if (function_exists('wp_tiny_mce')) wp_tiny_mce();
	wp_enqueue_script('utils');
	if(isset($_GET["task"]))
	{
		$task=$_GET["task"];
	}
	else
	{
		$task="show";
	}
	if(isset($_GET["id"]))
	{
		$id=$_GET["id"];
	}
	else
	{
		$id=-1;
	}

	switch($task){

case 'add':

		add();
		
		break;
		
case 'show':

		show_forms();
		
		break;
case 'Edit_CSS':
		if($id==-1)
		{
		save();
		$max_id="SELECT MAX( id ) FROM ".$wpdb->prefix."formmaker";
		$max_id=$wpdb->get_col($max_id);
		$id=$max_id[0];
		}
		else
		{
			apply($id);
		}

		edit_css($id);
		
		break;
case 'Edit_JavaScript':
		if($id==-1)
		{
		save();
		$max_id="SELECT MAX( id ) FROM ".$wpdb->prefix."formmaker";
		$max_id=$wpdb->get_col($max_id);
		$id=$max_id[0];
		}
		else
		{
			apply($id);
		}

		Edit_JavaScript($id);
		
		break;
case 'Custom_text_in_email':
		if($id==-1)
		{
		save();
		$max_id="SELECT MAX( id ) FROM ".$wpdb->prefix."formmaker";
		$max_id=$wpdb->get_col($max_id);
		$id=$max_id[0];
		}
		else
		{
			apply($id);
		}

		text_in_email($id);
		
		break;
case 'delete':

		delete($id);
		show_forms();
		
		break;
case 'edit_form':

		edit($id);
		
		break;
		
case 'gotoedit':

		gotoedit($id);
		edit($id);
		break;
		
case 'Save':
		if($id==-1)
		{
		save();
		}
		else
		{
			apply($id);
		}
		show_forms();
		
			
		break;
case 'Apply_edit_css':

		save_edit_css($id);
		edit_css($id);
    	break;
case 'Apply_edit_JavaScript':

		save_javascript($id);
		Edit_JavaScript($id);
		
			
		break;
case 'Apply_mail':

		save();
		show_forms();
		
			
		break;
case 'Save_edit_css':

		save_edit_css($id);
		edit($id);
		
			
		break;
case 'Save_edit_JavaScript':

		save_javascript($id);
		edit($id);
		
			
		break;
case 'Save_egfsddit_css':

		save();
		
		
			
		break;
case 'Apply':
		if($id==-1)
		{
		  $_count_filds=save();
		$max_id="SELECT MAX( id ) FROM ".$wpdb->prefix."formmaker";
		$max_id=$wpdb->get_col($max_id);
		$id=$max_id[0];
		if(!$_count_filds)
		{
		show_forms();
		break;
		}
		forchrome($id);
		}
		else
		{
			
				$_count_filds=apply($id);
				if(!$_count_filds)
				{
					show_forms();
					break;
				}
			forchrome($id);
		}
		

			
		break;
case 'custom_text_Save':
		update_custom_text($id);
		edit($id);
					
		break;
		
case 'Custom_text_apply':

		update_custom_text($id);
		text_in_email($id);					
		break;
		
}

}



function Form_maker_Submits()
{
	
	global $wpdb;
	if(isset($_GET["id"]))
	{
		$id=$_GET["id"];
	}
	else
	{
		$id=0;
	}
	delete_submishions();
	?>
    <form method="post" action="admin.php?page=Form_maker_Submits&id=<?php echo $id; ?>" id="main_show_form" name="main_show_form">
	<table width="100%" style="display:block">
    <tr>
    <td style="width:170px;">
    <?php $Submishion_title='Submissions'; echo "<h2>".__($Submishion_title). "</h2>"; ?>
    </td>
	    <td width="70%">&nbsp;
    </td>
<td style="text-align:right;font-size:16px;padding:20px; padding-right:50px">
		<a href="http://web-dorado.com/files/fromFormMaker.php" target="_blank" style="color:red; text-decoration:none;">
		<img src="<?php echo plugins_url( 'images/header.png' , __FILE__ ); ?>" border="0" alt="www.web-dorado.com" width="215"><br>
		Get the full version&nbsp;&nbsp;&nbsp;&nbsp;
		</a>
	</td>
    </tr>
    <?php if($id!=0){ ?>
    <tr>
    <td colspan="2">&nbsp;
    </td>
    <td style="text-align:right; padding-right:50px">
    
	Export to
	   <input type="button" value="CSV" onclick="alert('This functionality is disabled in free version. If you need this functionality, you need to buy the commercial version.')">&nbsp;
		<input type="button" value="XML" onclick="alert('This functionality is disabled in free version. If you need this functionality, you need to buy the commercial version.')">
	</td>
    </tr>
    <tr>
    <td colspan="2">&nbsp;
    </td>
    <td style="text-align:right; padding-right:50px"><input type="button" onclick="alert('This functionality is disabled in free version. If you need this functionality, you need to buy the commercial version.')" value="Add/Remove Columns"></td>
    </tr>
    <?php }?>
    </table>
    
    <?php
	/////////////////////////////////////////////////////
	$sql_text="";//text for filtring 
		 if(isset( $_POST['serch_or_not']))
		 {
			 if($_POST['serch_or_not']=="search")
			 {
				 if($_POST["startdate"] && $_POST["enddate"])
				 {
			 			$sql_text=" WHERE form_id='".$id."' AND date >='".$_POST["startdate"]."  00:00:00' AND date <= '".$_POST["enddate"]." 23:59:59'";
				 }
				 else
				 {
					 if($_POST["startdate"] && !$_POST["enddate"])
					 {
						 $sql_text=" WHERE form_id='".$id."' AND date >='".$_POST["startdate"]."  00:00:00'";
					 }
					 else
					 {
						 if(!$_POST["startdate"] && $_POST["enddate"])
						 {
							 $sql_text=" WHERE form_id='".$id."' AND date <= '".$_POST["enddate"]." 23:59:59'";
						 }
						 else
						 {
							 $sql_text=" WHERE form_id='".$id."'";
						 }
						 
					 }
				 }
				 
			 
			 }
			 else
			 {
				 $sql_text=" WHERE form_id='".$id."'";
			 }
		 }
		 else
		 {
			$sql_text=' WHERE form_id="'.$id.'"'; 
		 }

		 $i=0;//counts elements intu Form_submishions;
		 $ids_Form_grup_id=$wpdb->get_col("SELECT group_id FROM  ".$wpdb->prefix."formmaker_submits  ".$sql_text." group by group_id",0);		 
		foreach($ids_Form_grup_id as $id)
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
				 function clear_search()
				 {
					 document.getElementById("serch_or_not").value="";
				 }
				 function submit_form_id(x)
				 {
					 
					 var val=x.options[x.selectedIndex].value;
					 window.location.href="admin.php?page=Form_maker_Submits&id="+val;
				 }
				 
				 </script>
<?php

			 ?>
<div class="tablenav top" style="width:95%">
	<div class="alignleft actions" style="width:150px;">
    	<label for="form_id" style="font-size:14px">View submissions for: </label>
        </div>
        <div class="alignleft actions">
       <select name="form_id" id="form_id" onchange="submit_form_id(this)"  style="width:130px">
            <option value="0" <?php if(isset($_GET["id"])){ if($_GET["id"]==0) echo 'selected="selected"';} ?>> Select a Form </option>
            <?php
			 $Form_Maker_forms=$wpdb->get_results("SELECT * FROM  ".$wpdb->prefix."formmaker order by title",0);
	   foreach($Form_Maker_forms as $row_form)
	   {
		   ?>
            <option value="<?php echo $row_form->id; ?>" <?php if(isset($_GET["id"])){ if($_GET["id"]== $row_form->id) echo 'selected="selected"';} ?>><?php echo  $row_form->title; ?></option>
           <?php
	   }
			 ?>          
            </select>
    </div>
</div>
<div class="tablenav top" style="width:95%">

     <div class="alignleft actions" >

			
				    From:<input class="inputbox" type="text" name="startdate" id="startdate" size="10" maxlength="10" value="<?php if(isset($_POST['startdate'])) echo $_POST['startdate']; ?>"> 
<input type="reset" class="button" value="..." onclick="return showCalendar('startdate','%Y-%m-%d');"> To:  <input class="inputbox" type="text" name="enddate" id="enddate" size="10" maxlength="10" value="<?php if(isset($_POST['enddate'])) echo $_POST['enddate']; ?>"> 
<input type="reset" class="button" value="..." onclick="return showCalendar('enddate','%Y-%m-%d');">
    </div>
	<div class="alignleft actions">
   		<input type="button" value="Search" onclick="document.getElementById('page_number').value='1';document.getElementById('serch_or_not').value='search'; document.getElementById('main_show_form').submit();" class="button-secondary action" /><input type="button" value="Reset" onclick="window.location.href='admin.php?page=Form_maker_Submits<?php if(isset($_GET["id"])){echo "&id=".$_GET["id"];} ?>'" class="button-secondary action" />
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
	$sql_ascdesc='';
	if($_POST['asc_or_desc_by']=='id')	
	{
		if($_POST['asc_or_desc']==1)
		{
			$sql_ascdesc=' ORDER BY group_id ASC';
			$style_class_title="manage-column column-title sortable desc";
			$style_class_id="manage-column column-autor sorted asc";
			$style_class_Email="manage-column column-autor sortable desc";
			$sort_title=1;
			$sort_id=2;
			$sort_Email=1;
		 
		}
		if($_POST['asc_or_desc']==2)
		{
			$sql_ascdesc=' ORDER BY group_id DESC';
			$style_class_title="manage-column column-title sortable desc";
			$style_class_id="manage-column column-autor sorted desc";
			$style_class_Email="manage-column column-autor sortable desc";
			$sort_title=1;
			$sort_id=1;
			$sort_Email=1;
		}
	}
		if($_POST['asc_or_desc_by']=='date')
		{
		
		if($_POST['asc_or_desc']==1)
		{
			$sql_ascdesc=' ORDER BY date ASC';
			$style_class_title="manage-column column-title sorted asc";
			$style_class_id="manage-column column-autor sortable desc";
			$style_class_Email="manage-column column-autor sortable desc";
			$sort_title=2;
			$sort_id=1;
			$sort_Email=1;
		 
		}
		 if($_POST['asc_or_desc']==2)
		 {
		 	$sql_ascdesc=' ORDER BY date DESC';
		 	$style_class_title="manage-column column-title sorted desc";
			$style_class_id="manage-column column-autor sortable desc";
			$style_class_Email="manage-column column-autor sortable desc";
			$sort_title=1;
			$sort_id=1;
			$sort_Email=1;
		 }	

		
	}
		if($_POST['asc_or_desc_by']=='ip')	
	{
		if($_POST['asc_or_desc']==1)
		{
			$sql_ascdesc=' ORDER BY ip ASC';
			$style_class_title="manage-column column-title sortable desc";
			$style_class_id="manage-column column-autor sortable desc";
			$style_class_Email="manage-column column-autor sorted asc";
			$sort_title=1;
			$sort_id=1;
			$sort_Email=2;
		 
		}
		 if($_POST['asc_or_desc']==2)
		 {
		 	$sql_ascdesc=' ORDER BY ip DESC';
		 	$style_class_title="manage-column column-title sortable desc";
			$style_class_id="manage-column column-autor sortable desc";
			$style_class_Email="manage-column column-autor sorted desc";
			$sort_title=1;
			$sort_id=1;
			$sort_Email=1;
		 }	
	}
	if(!($_POST['asc_or_desc_by']=='id' || $_POST['asc_or_desc_by']=='date' || $_POST['asc_or_desc_by']=='ip'))
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
					 	 document.getElementById("startdate").value='';
						 document.getElementById("enddate").value='';
						 				 
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
  <input type="hidden" id="hide_label_list" name="hide_label_list" value="" />
  <input type="hidden" id="boxchecked" name="boxchecked" value="" />
   <input type="hidden" id="delete" name="delete" value="0" />
  <input type="hidden" id="idd" name="idd" value="0" />
   
      <?php  
  $limi[0]=($page_number-1)*20;
 $limi[1]=20;
 $limit=" LIMIT ".$limi[0].",".$limi[1];

 $count_row_show=count($ids_Form_grup_id); 
////////////////////////////////////////////////////////////////////////////
$defult_class="manage-column column-autor sortable desc";
$defult_orderr=1;
if(isset($_POST['asc_or_desc_by']) && $sql_ascdesc=="")
{
	if($_POST['asc_or_desc_by'])
	{
		
$wpdb->query($wpdb->prepare(
"insert into ".$wpdb->prefix."formmaker_submits (form_id,	element_label, element_value, group_id,`date`,ip) select %s,'%s', '', group_id,`date`,ip from  ". $wpdb->prefix."formmaker_submits where `form_id`=%s and group_id not in (select group_id from ". $wpdb->prefix."formmaker_submits where `form_id`=%s and element_label='%s' group by  group_id) group by group_id",
	$_GET["id"], 
	$_POST['asc_or_desc_by'], 
	$_GET["id"],
	$_GET["id"],
	$_POST['asc_or_desc_by']
	 
)
);
		
	if($_POST["asc_or_desc"]==1)
	{
		$custom_orderr=2;
		$custom_style="manage-column column-autor sorted asc";
		$order="DESC";
	}
	else
	{
		$custom_orderr=1;
		$custom_style="manage-column column-autor sorted desc";
		$order="ASC";
	}
	if($_POST['asc_or_desc_by']!="")
	{
	$sql_order=" AND element_label='".$_POST['asc_or_desc_by']."'";
	$sql_ascdesc=" ORDER BY element_value ".$order;
	}
	else
	{
		$sql_order="";
	}
	
	
	}

}
if($sql_ascdesc=="")
{
	$sql_ascdesc="ORDER BY group_id DESC";
}
 
 
 
 
 if(isset($_GET["id"]))
 {
	 $id=$_GET["id"];
 }
 else
 {
	 $id=0;
 }
 

 
 $query = "SELECT * FROM ".$wpdb->prefix."formmaker_submits WHERE form_id='". $id."'";
	//echo $query;

	$rows = $wpdb->get_results($query);


	
	$n=count($rows);
	$labels= array();
	for($i=0; $i < $n ; $i++)

	{
		$row = &$rows[$i];
		if(!in_array($row->element_label, $labels))
		{
			array_push($labels, $row->element_label);
		}
	}
	
	$sorted_labels_id= array();
	$sorted_labels= array();
	$label_titles=array();
	if($labels)
	{
		
		$label_id= array();
		$label_order= array();
		$label_order_original= array();
		$label_type= array();
		
		$this_form =$wpdb->get_row("SELECT * FROM ".$wpdb->prefix."formmaker WHERE id='". $id."'");
		
		$label_all	= explode('#****#',$this_form->label_order);
		$label_all 	= array_slice($label_all,0, count($label_all)-1);   
		
		foreach($label_all as $key => $label_each) 
		{
			$label_id_each=explode('#**id**#',$label_each);
			array_push($label_id, $label_id_each[0]);
			$label_oder_each=explode('#**label**#', $label_id_each[1]);
			
			array_push($label_order_original, $label_oder_each[0]);
			
			$ptn = "/[^a-zA-Z0-9_]/";
			$rpltxt = "";
			$label_temp=preg_replace($ptn, $rpltxt, $label_oder_each[0]);
			array_push($label_order, $label_temp);
			
			array_push($label_type, $label_oder_each[1]);
		}
		
		foreach($label_id as $key => $label) 
			if(in_array($label, $labels))
			{
				array_push($sorted_labels, $label_order[$key]);
				array_push($sorted_labels_id, $label);
				array_push($label_titles, $label_order_original[$key]);
			}
			$i=0;
			foreach($sorted_labels_id as $idd)
			{
				
				$labelll[$idd]=$label_titles[$i];
				$i++;
			}

	}
	$labels_id=$sorted_labels_id ;
	if(isset($_POST["hide_label_list"]))  
	{
    $lists['hide_label_list']=$_POST["hide_label_list"];
	}
 ?>

 <script type="text/javascript">
 function renderColumns()
{
allTags=document.getElementsByTagName('*');

	for(curTag in allTags)
	{
		if(typeof(allTags[curTag].className)!="undefined")
			if(allTags[curTag].className.indexOf('_fc')>0)
			{		
				aaa=allTags[curTag].classList;
			
				curLabel=aaa[aaa.length-1].replace('_fc','');
				if(document.forms.main_show_form.hide_label_list.value.indexOf('@'+curLabel+'@')>=0)
					allTags[curTag].style.display = 'none';
				else
					allTags[curTag].style.display = '';
			}
	}
}

function clickLabChB(label, ChB)
{
	
	document.forms.main_show_form.hide_label_list.value=document.forms.main_show_form.hide_label_list.value.replace('@'+label+'@','');
	if(document.forms.main_show_form.hide_label_list.value=='') document.getElementById('ChBAll').checked=true;
	
	if(!(ChB.checked)) 
	{
		document.forms.main_show_form.hide_label_list.value+='@'+label+'@';
		document.getElementById('ChBAll').checked=false;
	}
	renderColumns();
}



function clickLabChBAll(ChBAll)
{
<?php
if(isset($labels))
{
	$templabels=array_merge(array('submitid','submitdate','submitterip'),$labels_id);
	$label_titles=array_merge(array('ID','Submit date', 'Submitter\'s IP Address'),$label_titles);
}
?>
if(ChBAll.checked)
	{ 
		document.forms.main_show_form.hide_label_list.value='';

		for(i=0; i<=ChBAll.form.length; i++)
			if(typeof(ChBAll.form[i])!="undefined")
				if(ChBAll.form[i].type=="checkbox")
					ChBAll.form[i].checked=true;
	}
	else
	{
		document.forms.main_show_form.hide_label_list.value='@<?php echo implode($templabels,'@@') ?>@';

		for(i=0; i<=ChBAll.form.length; i++)
			if(typeof(ChBAll.form[i])!="undefined")
				if(ChBAll.form[i].type=="checkbox")
					ChBAll.form[i].checked=false;
	}

	renderColumns();

}
	function checkAll( n, fldName ) {
  if (!fldName) {
     fldName = 'cb';
  }
	var f = document.main_show_form;
	var c = f.toggle.checked;
	var n2 = 0;
	for (i=0; i < n; i++) {
		cb = eval( 'f.' + fldName + '' + i );
		if (cb) {
			cb.checked = c;
			n2++;
		}
	}
	if (c) {
		document.main_show_form.boxchecked.value = n2;
	} else {
		document.main_show_form.boxchecked.value = 0;
	}}
	
	
function isChecked(isitchecked){
	if (isitchecked == true){
		document.main_show_form.boxchecked.value++;
	}
	else {
		document.main_show_form.boxchecked.value--;
	}
}
function submit1()
{
	var answer = confirm("Selected rows will be deleted. Are you sure?")
		if (answer){
			document.getElementById('idd').value=1;
			document.getElementById('main_show_form').submit();
		}	
}
function submit2(x)
{
	document.getElementById('delete').value=x;
	document.getElementById('main_show_form').submit();
}
</script>
<div id="sbox-overlay" style="z-index: 65555; position: fixed; top: 0px; left: 0px; visibility: visible; zoom: 1; background-color:#000000; opacity: 0.7; filter: alpha(opacity=70); display:none;" onclick="toggleChBDiv(false)"></div>
<div style="background-color:#FFFFFF; width:250px; padding:20px;display:none; position:fixed; top:200px; border:2px solid #AAAAAA;  z-index:65556" id="ChBDiv">
<p style="font-weight:bold; font-size:18px;margin-top: 0px;">
Select Columns
</p>

<input type="checkbox" <?php if($lists['hide_label_list']==='') echo 'checked="checked"' ?> onclick="clickLabChBAll(this)" id="ChBAll" />All</br>

<?php 

	foreach($templabels as $key => $curlabel)
	{
	if(strpos($lists['hide_label_list'],'@'.$curlabel.'@')===false)
	echo '<input type="checkbox" checked="checked" onclick="clickLabChB(\''.$curlabel.'\', this)" />'.$label_titles[$key].'<br />';
	else
	echo '<input type="checkbox" onclick="clickLabChB(\''.$curlabel.'\', this)" />'.$label_titles[$key].'<br />';
	}


?>
<br />
<div style="text-align:center;">
<input type="button" onclick="toggleChBDiv(false);" value="Done" /> 
</div>
</div>
<?php $ids_Form_grup_id=$wpdb->get_col("SELECT group_id FROM  ".$wpdb->prefix."formmaker_submits  ".$sql_text.$sql_order." group by group_id ".$sql_ascdesc." ".$limit."",0); ?>
  <table class="wp-list-table widefat fixed pages" style="width:95%; table-layout:inherit">
 <thead>
 <TR>
 <th scope="col" class="<?php echo $style_class_id; ?> submitid_fc" style=" width:50px" ><a href="javascript:ordering('id',<?php echo $sort_id ?>)"><span>Id</span><span class="sorting-indicator"></span></a></th>
 <th style=" width:30px" >
 <input type="checkbox" name="toggle" id="toggle" value="" onclick="checkAll(<?php echo count($ids_Form_grup_id) ?>)" style="margin:0px; padding:0px">
 </th>
 <th scope="col"  class="<?php echo $style_class_title; ?>  submitdate_fc" style="" ><a href="javascript:ordering('date',<?php echo $sort_title ?>)"><span>Submit date</span><span class="sorting-indicator"></span></a></th>
 <th scope="col"  class="<?php echo $style_class_Email; ?> submitterip_fc" style="" ><a href="javascript:ordering('ip',<?php echo $sort_Email?>)"><span>Submitter's IP Address</span><span class="sorting-indicator"></span></a></th>
 <?php

  $row_fields=$wpdb->get_col("SELECT element_label FROM  ".$wpdb->prefix."formmaker_submits  ".$sql_text." group by element_label");
 		foreach($labels_id as $label_id )
		{
		?>
        <th scope="col"  class="<?php  if($_POST['asc_or_desc_by']==$label_id){echo $custom_style." ".$label_id."_fc";}else {echo $defult_class." ".$label_id."_fc"; } ?>" style="" ><a href="javascript:ordering('<?php echo $label_id ?>',<?php if($_POST['asc_or_desc_by']==$label_id){echo $custom_orderr;} else {echo $defult_orderr; } ?>)"><span><?php echo $labelll[$label_id]; ?></span><span class="sorting-indicator"></span></a></th>
 
 
 <?php }?>
 <th style="width:80px"><a href="javascript:submit1()">Delete</a></th>
 </TR>
 </thead>
 <tbody>
 
 <?php
  
 
 $ids_Form_grup_id=$wpdb->get_col("SELECT group_id FROM  ".$wpdb->prefix."formmaker_submits  ".$sql_text.$sql_order." group by group_id ".$sql_ascdesc." ".$limit."",0);
 foreach($ids_Form_grup_id as $key => $id_Form_grup_id)
 {

 $row1=$wpdb->get_row("SELECT * FROM  ".$wpdb->prefix."formmaker_submits  ".$sql_text." AND group_id='".$id_Form_grup_id."'") ?>
 <tr>
 <td class="submitid_fc"><?php echo $id_Form_grup_id; ?></td>
 <td style=""><input type="checkbox" id="cb<?php echo  $key  ?>" name="cid[]" value="<?php echo  $row1->group_id;  ?>" onclick="isChecked(this.checked);"></td>
 <td class="submitdate_fc"><?php  echo $row1->date; ?> </td>
 <td class="submitterip_fc"><?php echo  $row1->ip;  ?></td>
 
 <?php
  foreach($labels_id as $label_id )
  { 
 $element_value=$wpdb->get_var("SELECT element_value FROM  ".$wpdb->prefix."formmaker_submits WHERE element_label='".$label_id."' AND group_id='".$id_Form_grup_id."'");	
  ?>
     <td class="<?php echo $label_id."_fc" ?>"><?php
	 if(strpos($element_value,"*@@url@@*"))
					{
						$new_file=str_replace("*@@url@@*",'', $element_value);
						$new_filename=explode('/', $new_file);
						$element_value='<a target="_blank" href="'.$new_file.'">'.$new_filename[count($new_filename)-1]."</a>";
					} 
						echo $element_value;  ?></td>
 <?php } ?>
    <td><a href="javascript:submit2('<?php echo $id_Form_grup_id ?>')">Delete</a> </td>
 </tr>
 <?php
 }
	?>
 </tbody>
 </table>

 <?php
?>
    
    
   
 </form>
 <link type="text/css" rel="stylesheet" href="<?php echo plugins_url("js/calendar-jos.css",__FILE__) ?>" />
	<script type="text/javascript" src="<?php echo plugins_url("js/calendar_function.js",__FILE__) ?>"></script>
	  <script type="text/javascript" src="<?php echo plugins_url("js/calendar.js",__FILE__) ?>"></script>
	  <script type="text/javascript" src="<?php echo plugins_url("js/calendar-setup.js",__FILE__) ?>"></script>
	
	
	<?php
	
	
	
	
	
	
	
}




function Uninstall_Form_Maker()
{
global $wpdb;
$base_name = plugin_basename('Form_maker');
$base_page = 'admin.php?page='.$base_name;
$mode = trim($_GET['mode']);


if(!empty($_POST['do'])) {

	if($_POST['do']=="UNINSTALL Form Maker") {
			check_admin_referer('Form Maker_uninstall');
			if(trim($_POST['uninstall_Form_yes']) == 'yes') {
				echo '<div id="message" class="updated fade">';
				echo '<p>';
				echo "Table 'formmaker' has been deleted.";
				$wpdb->query("DROP TABLE ".$wpdb->prefix."formmaker");
				echo '<font style="color:#000;">';
				echo '</font><br />';
				echo '</p>';
				echo '<p>';
				echo "Table 'formmaker_submits' has been deleted.";
				$wpdb->query("DROP TABLE ".$wpdb->prefix."formmaker_submits");
				echo '<font style="color:#000;">';
				echo '</font><br />';
				echo '</p>';
				echo '</div>'; 
				$mode = 'end-UNINSTALL';
			}
		}
}



switch($mode) {

		case 'end-UNINSTALL':
			$deactivate_url = wp_nonce_url('plugins.php?action=deactivate&amp;plugin='.plugin_basename(__FILE__), 'deactivate-plugin_'.plugin_basename(__FILE__));
			echo '<div class="wrap">';
			echo '<div id="icon-Form_maker" class="icon32"><br /></div>';
			echo '<h2>Uninstall Form Maker</h2>';
			echo '<p><strong>'.sprintf('<a href="%s">Click Here</a> To Finish The Uninstallation And Form Maker Will Be Deactivated Automatically.', $deactivate_url).'</strong></p>';
			echo '</div>';
			break;
	// Main Page
	default:
?>
<form method="post" action="<?php echo admin_url('admin.php?page=Uninstall_Form_Maker'); ?>">
<?php wp_nonce_field('Form Maker_uninstall'); ?>
<div class="wrap">
	<div id="icon-Form_maker" class="icon32"><br /></div>
	<h2><?php echo 'Uninstall Form Maker'; ?></h2>
	<p>
		<?php echo 'Deactivating Form Maker plugin does not remove any data that may have been created, such as the Forms and the Submissions. To completely remove this plugin, you can uninstall it here.'; ?>
	</p>
	<p style="color: red">
		<strong><?php echo'WARNING:'; ?></strong><br />
		<?php echo 'Once uninstalled, this cannot be undone. You should use a Database Backup plugin of WordPress to back up all the data first.'; ?>
	</p>
	<p style="color: red">
		<strong><?php echo 'The following WordPress Options/Tables will be DELETED:'; ?></strong><br />
	</p>
	<table class="widefat">
		<thead>
			<tr>
				<th><?php echo 'WordPress Tables'; ?></th>
			</tr>
		</thead>
		<tr>
			<td valign="top">
				<ol>
				<?php
						echo '<li>formmaker</li>'."\n";
						echo '<li>formmaker_submits</li>'."\n";
					
				?>
				</ol>
			</td>
		</tr>
	</table>
	<p style="text-align: center;">
		<?php echo 'Do you really want to uninstall Form Maker?'; ?><br /><br />
		<input type="checkbox" name="uninstall_Form_yes" value="yes" />&nbsp;<?php echo 'Yes'; ?><br /><br />
		<input type="submit" name="do" value="<?php echo 'UNINSTALL Form Maker'; ?>" class="button-primary" onclick="return confirm('<?php echo 'You Are About To Uninstall Form Maker From WordPress.\nThis Action Is Not Reversible.\n\n Choose [Cancel] To Stop, [OK] To Uninstall.'; ?>')" />
	</p>
</div>
</form>
<?php
} // End switch($mode)

}









function formmaker_activate()
{
	global $wpdb;
/// creat database tables
$sql_Form_Maker="
CREATE TABLE IF NOT EXISTS `".$wpdb->prefix."formmaker` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(127) NOT NULL,
  `mail` varchar(128) NOT NULL,
  `form` longtext NOT NULL,
  `css` text NOT NULL,
  `javascript` text NOT NULL,
  `script1` text NOT NULL,
  `script2` text NOT NULL,
  `data` text NOT NULL,
  `counter` int(11) NOT NULL,
  `article_id` int(11) NOT NULL,
  `label_order` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2";


 $table_name=$wpdb->prefix."formmaker";

$sql_Form_Maker_contact=<<<query1
INSERT INTO `$table_name` (`id`, `title`, `mail`, `form`, `css`, `javascript`, `script1`, `script2`, `data`, `counter`, `article_id`, `label_order`) VALUES

(1, 'Contact', '', '<table border="0" cellpadding="4" cellspacing="0" class="form_view"><tbody id="form_view"><tr><td id="column_0" valign="top"><table><tbody><tr id="13" type="type_name"><td valign="top" align="left" id="13_label_section" class=""><span id="13_element_label" class="label">Name:</span><span id="13_required_element" class="required">&nbsp;*</span></td><td valign="top" align="left" id="13_element_section" class=""><input type="hidden" value="type_name" name="13_type"><input type="hidden" value="yes" name="13_required" id="13_required"><table id="13_table_name" cellpadding="0" cellspacing="0"><tr id="13_tr_name1"><td id="13_td_name_input_first"><input type="text" style="border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px; margin-top: 0px; margin-right: 10px; margin-bottom: 0px; margin-left: 0px; padding-top: 0px; padding-right: 0px; padding-bottom: 0px; padding-left: 0px; width: 95px; " id="13_element_first" name="13_element_first" onchange="change_value(''13_element_first'')"></td><td id="13_td_name_input_last"><input type="text" style="border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px; margin-top: 0px; margin-right: 10px; margin-bottom: 0px; margin-left: 0px; padding-top: 0px; padding-right: 0px; padding-bottom: 0px; padding-left: 0px; width: 95px; " id="13_element_last" name="13_element_last" onchange="change_value(''13_element_last'')"></td></tr><tr id="13_tr_name2"><td id="13_td_name_label_first" align="left"><label class="mini_label"><!--repstart-->First<!--repend--></label></td><td id="13_td_name_label_last" align="left"><label class="mini_label"><!--repstart-->Last<!--repend--></label></td></tr></table></td><td id="X_13" valign="middle" align="right"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/delete_el.png" style="cursor: pointer; margin-left: 30px; " onclick="remove_row(&quot;13&quot;)"></td><td id="left_13" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/left.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="left_row(&quot;13&quot;)"></td><td id="up_13" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/up.png" style="cursor: pointer; " onclick="up_row(&quot;13&quot;)"></td><td id="down_13" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/down.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="down_row(&quot;13&quot;)"></td><td id="right_13" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/right.png" style="cursor: pointer; " onclick="right_row(&quot;13&quot;)"></td><td id="edit_13" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/edit.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="edit(&quot;13&quot;)"></td></tr><tr id="14" type="type_submitter_mail"><td valign="middle" align="left" id="14_label_section" class=""><span id="14_element_label" class="label">E-mail:</span><span id="14_required_element" class="required"></span></td><td valign="middle" align="left" id="14_element_section" class=""><input type="hidden" value="type_submitter_mail" name="14_type"><input type="hidden" value="no" name="14_required" id="14_required"><input type="hidden" value="no" name="14_send" id="14_send"><input type="text" style="width: 200px; " class="input_deactive" id="14_element" name="14_element" value="" title="" onfocus="delete_value(''14_element'')" onblur="return_value(''14_element'')" onchange="change_value(''14_element'')"></td><td id="X_14" valign="middle" align="right"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/delete_el.png" style="cursor: pointer; margin-left: 30px; " onclick="remove_row(&quot;14&quot;)"></td><td id="left_14" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/left.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="left_row(&quot;14&quot;)"></td><td id="up_14" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/up.png" style="cursor: pointer; " onclick="up_row(&quot;14&quot;)"></td><td id="down_14" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/down.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="down_row(&quot;14&quot;)"></td><td id="right_14" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/right.png" style="cursor: pointer; " onclick="right_row(&quot;14&quot;)"></td><td id="edit_14" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/edit.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="edit(&quot;14&quot;)"></td></tr><tr id="15" type="type_text"><td valign="middle" align="left" id="15_label_section" class=""><span id="15_element_label" class="label">Subject:</span><span id="15_required_element" class="required"></span></td><td valign="middle" align="left" id="15_element_section" class=""><input type="hidden" value="type_text" name="15_type"><input type="hidden" value="no" name="15_required" id="15_required"><input type="text" style="width: 200px; " class="input_deactive" id="15_element" name="15_element" value="" title="" onfocus="delete_value(&quot;15_element&quot;)" onblur="return_value(&quot;15_element&quot;)" onchange="change_value(&quot;15_element&quot;)"></td><td id="X_15" valign="middle" align="right"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/delete_el.png" style="cursor: pointer; margin-left: 30px; " onclick="remove_row(&quot;15&quot;)"></td><td id="left_15" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/left.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="left_row(&quot;15&quot;)"></td><td id="up_15" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/up.png" style="cursor: pointer; " onclick="up_row(&quot;15&quot;)"></td><td id="down_15" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/down.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="down_row(&quot;15&quot;)"></td><td id="right_15" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/right.png" style="cursor: pointer; " onclick="right_row(&quot;15&quot;)"></td><td id="edit_15" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/edit.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="edit(&quot;15&quot;)"></td></tr><tr id="16" type="type_textarea"><td valign="top" align="left" id="16_label_section" class=""><span id="16_element_label" style="border-top-style: none; border-right-style: none; border-bottom-style: none; border-left-style: none; border-width: initial; border-color: initial; border-image: initial; vertical-align: top; ">Message:</span><span id="16_required_element" class="required"></span></td><td valign="top" align="left" id="16_element_section" class=""><input type="hidden" value="type_textarea" name="16_type"><input type="hidden" value="no" name="16_required" id="16_required"><textarea style="width: 200px; height: 100px; " class="input_deactive" id="16_element" name="16_element" title="" value="" onfocus="delete_value(''16_element'')" onblur="return_value(''16_element'')" onchange="change_value(''16_element'')"></textarea></td><td id="X_16" valign="middle" align="right"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/delete_el.png" style="cursor: pointer; margin-left: 30px; " onclick="remove_row(&quot;16&quot;)"></td><td id="left_16" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/left.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="left_row(&quot;16&quot;)"></td><td id="up_16" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/up.png" style="cursor: pointer; " onclick="up_row(&quot;16&quot;)"></td><td id="down_16" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/down.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="down_row(&quot;16&quot;)"></td><td id="right_16" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/right.png" style="cursor: pointer; " onclick="right_row(&quot;16&quot;)"></td><td id="edit_16" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/edit.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="edit(&quot;16&quot;)"></td></tr><tr id="11" type="type_submit_reset"><td colspan="2" id="11_label_and_element_section"><table id="11_elemet_table"><tbody><tr><td valign="middle" align="left" id="11_label_section" style="display: none; " class=""><span id="11_element_label" style="display: none; ">type_submit_reset_11</span></td><td valign="middle" align="left" id="11_element_section" class=""><input type="hidden" value="type_submit_reset" name="11_type"><button type="button" class="button_submit" id="11_element_submit" value="Submit" onclick="check_required(''submit'');">Submit</button><button type="button" class="button_reset" id="11_element_reset" value="Reset" onclick="check_required(''reset'');">Reset</button></td></tr></tbody></table></td><td id="X_11" valign="middle" align="right"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/delete_el.png" style="cursor: pointer; margin-left: 30px; " onclick="remove_row(&quot;11&quot;)"></td><td id="left_11" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/left.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="left_row(&quot;11&quot;)"></td><td id="up_11" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/up.png" style="cursor: pointer; " onclick="up_row(&quot;11&quot;)"></td><td id="down_11" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/down.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="down_row(&quot;11&quot;)"></td><td id="right_11" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/right.png" style="cursor: pointer; " onclick="right_row(&quot;11&quot;)"></td><td id="edit_11" valign="middle"><img src="http://demo.web-dorado.com/wp2/wp-content/plugins/form_maker/images/edit.png" style="margin-top: 2px; margin-right: 2px; margin-bottom: 2px; margin-left: 2px; cursor: pointer; " onclick="edit(&quot;11&quot;)"></td></tr></tbody></table></td></tr></tbody></table>', '.form_view, .form_view table\r\n{\r\nwidth:inherit !important;\r\n-webkit-border-horizontal-spacing: 0px;\r\n-webkit-border-vertical-spacing: 0px;\r\nborder-bottom-color: gray;\r\nborder:0px  !important;\r\nborder-bottom-width: 0px;\r\nborder-collapse: separate;\r\nborder-left-color: gray;\r\nborder-left-width: 0px;\r\nborder-right-color: gray;\r\nborder-right-width: 0px;\r\nborder-top-color: gray;\r\nborder-top-width: 0px;\r\ncolor: black;\r\ndisplay: table;\r\nfont-family: Helvetica, Arial, sans-serif;\r\nfont-size: 14px !important;\r\nfont-weight: normal;\r\nheight: inherit !important;\r\nline-height: 15px;\r\nmargin-bottom: 0px;\r\nmargin-left: 0px;\r\nmargin-right: 0px;\r\nmargin-top: 0px;\r\npadding-bottom: 0px;\r\npadding-left: 0px;\r\npadding-right: 0px;\r\npadding-top: 0px;\r\ntext-align: left !important;\r\n\r\n}\r\n\r\n.form_view, .form_view tr\r\n{\r\n-webkit-border-horizontal-spacing: 0px;\r\n-webkit-border-vertical-spacing: 0px;\r\nborder:0px  !important;\r\nborder-bottom-color: gray;\r\nborder-collapse: separate;\r\nborder-left-color: gray;\r\nborder-right-color: gray;\r\nborder-top-color: gray;\r\ncolor: black;\r\ndisplay: table-row;\r\nfont-family: Helvetica, Arial, sans-serif;\r\nfont-size: 14px;\r\nfont-weight: normal;\r\nheight: inherit !important;\r\nline-height: 15px;\r\nmargin-bottom: 0px;\r\nmargin-left: 0px;\r\nmargin-right: 0px;\r\nmargin-top: 0px;\r\npadding-bottom: 0px;\r\npadding-left: 0px;\r\npadding-right: 0px;\r\npadding-top: 0px;\r\ntext-align: left;\r\nvertical-align: middle;\r\nwidth:inherit !important;\r\n}\r\n\r\n.form_view, .form_view td\r\n{\r\n-webkit-border-horizontal-spacing: 2px;\r\n-webkit-border-vertical-spacing: 2px;\r\nborder-bottom-color: black;\r\nborder-collapse: separate;\r\nborder-left-color: black;\r\nborder-right-color: black;\r\nborder-top-color: black;\r\nborder:0px !important;\r\ncolor: black;\r\ndisplay: table-cell;\r\nfont-family: Helvetica, Arial, sans-serif;\r\nfont-size: 14px;\r\nfont-weight: normal;\r\nheight:inherit !important;\r\nline-height: 15px;\r\nmargin-bottom: 0px;\r\nmargin-left: 0px;\r\nmargin-right: 0px;\r\nmargin-top: 0px;\r\npadding-bottom: 1px !important;\r\npadding-left: 1px !important;\r\npadding-right: 1px !important;\r\npadding-top: 3px !important;\r\ntext-align: left !important;\r\nwidth:inherit !important;\r\nvertical-align:top;\r\n}\r\n.form_view, .form_view tr\r\n{\r\n-webkit-border-horizontal-spacing: 0px;\r\n-webkit-border-vertical-spacing: 0px;\r\nborder:0px  !important;\r\nborder-bottom-color: gray;\r\nborder-collapse: separate;\r\nborder-left-color: gray;\r\nborder-right-color: gray;\r\nborder-top-color: gray;\r\ncolor: black;\r\ndisplay: table-row;\r\nfont-family: Helvetica, Arial, sans-serif;\r\nfont-size: 14px;\r\nfont-weight: normal;\r\nheight: inherit !important;\r\nline-height: 15px;\r\nmargin-bottom: 0px;\r\nmargin-left: 0px;\r\nmargin-right: 0px;\r\nmargin-top: 0px;\r\npadding-bottom: 0px;\r\npadding-left: 0px;\r\npadding-right: 0px;\r\npadding-top: 0px;\r\ntext-align: left;\r\nvertical-align: middle;\r\nwidth:inherit !important;\r\n}\r\n\r\n.form_view, .form_view input,  .form_view  textarea\r\n{\r\nline-height:inherit  !important;\r\nmargin:0px !important;\r\nmin-height: 18px !important;\r\n font-size: 14px !important;\r\n}\r\n.form_view, .form_view select\r\n{\r\nmargin:0px !important;\r\nfont-size: 14px !important;\r\n}\r\n.form_view, .form_view label\r\n{\r\nfont-size: 14px;\r\n vertical-align:inherit !important;\r\n}\r\n.time_box\r\n{\r\nborder-width:1px;\r\nmargin: 0px;\r\npadding: 0px;\r\ntext-align:right;\r\nwidth:30px;\r\nvertical-align:middle\r\n}\r\n\r\n\r\n.mini_label\r\n{\r\ncolor: #000 !important;\r\nfont-size:14px;\r\nfont-family: Lucida Grande, Tahoma, Arial, Verdana, sans-serif;\r\n}\r\n\r\n.ch_rad_label\r\n{\r\ncolor:#000 !important;\r\ndisplay:inline;\r\nmargin-left:5px;\r\nmargin-right:15px;\r\nfloat:none;\r\n}\r\n\r\n.label\r\n{\r\n-webkit-border-horizontal-spacing: 2px;\r\n-webkit-border-vertical-spacing: 2px;\r\nborder-bottom-color: black;\r\nborder-bottom-style: none;\r\nborder-collapse: separate;\r\nborder-left-color: black;\r\nborder-left-style: none;\r\nborder-right-color: black;\r\nborder-right-style: none;\r\nborder-top-color: black;\r\nborder-top-style: none;\r\ncolor: black;\r\ndisplay: inline;\r\nfont-family: Helvetica, Arial, sans-serif;\r\nfont-size: 14px;\r\nfont-weight: normal;\r\nheight: auto;\r\nline-height: 15px;\r\nmargin-bottom: 0px;\r\nmargin-left: 0px;\r\nmargin-right: 0px;\r\nmargin-top: 0px;\r\npadding-bottom: 0px;\r\npadding-left: 0px;\r\npadding-right: 0px;\r\npadding-top: 0px;\r\ntext-align: -webkit-left;\r\nwidth: auto;\r\n}\r\n\r\n\r\n.td_am_pm_select\r\n{\r\npadding-left:5;\r\n}\r\n\r\n.am_pm_select\r\n{\r\nheight: 16px;\r\nmargin:0;\r\npadding:0\r\n}\r\n\r\n.input_deactive\r\n{\r\nbackground-color: #FFFFFF;\r\nborder-bottom-style: inset;\r\nborder-bottom-width: 1px;\r\nborder-collapse: separate;\r\nborder-left-color: #EEE;\r\nborder-left-style: inset;\r\nborder-left-width: 1px;\r\nborder-right-color: #EEE;\r\nborder-right-style: inset;\r\nborder-right-width: 1px;\r\nborder-top-color: #EEE;\r\nborder-top-style: inset;\r\nborder-top-width: 1px;\r\nfont-style: italic;\r\ncolor: #999;\r\ncursor: auto;\r\ndisplay: inline-block;\r\nfont-family: Arial;\r\nfont-size: 14px !important;\r\nfont-weight: normal;\r\nletter-spacing: normal;\r\nline-height: normal;\r\nmargin-bottom: 0px;\r\nmargin-left: 0px;\r\nmargin-right: 0px;\r\nmargin-top: 0px;\r\npadding-bottom: 0px;\r\npadding-left: 0px;\r\npadding-right: 0px;\r\npadding-top: 0px;\r\ntext-align: -webkit-auto;\r\ntext-indent: 0px;\r\ntext-shadow: none;\r\ntext-transform: none;\r\nword-spacing: 0px;\r\n}\r\n\r\n.input_active\r\n{\r\nbackground-color: #FFFFFF;\r\n-webkit-appearance: none;\r\n-webkit-border-horizontal-spacing: 2px;\r\n-webkit-border-vertical-spacing: 2px;\r\n-webkit-rtl-ordering: logical;\r\n-webkit-user-select: text;\r\nbackground-color: white;\r\nborder-bottom-color: #EEE;\r\nborder-bottom-style: inset;\r\nborder-bottom-width: 1px;\r\nborder-collapse: separate;\r\nborder-left-color: #EEE;\r\nborder-left-style: inset;\r\nborder-left-width: 1px;\r\nborder-right-color: #EEE;\r\nborder-right-style: inset;\r\nborder-right-width: 1px;\r\nborder-top-color: #EEE;\r\nborder-top-style: inset;\r\nborder-top-width: 1px;\r\ncolor: black;\r\ncursor: auto;\r\ndisplay: inline-block;\r\nfont-family: Arial;\r\nfont-size: 14px !important;\r\nfont-style: normal;\r\nfont-weight: normal;\r\nheight: 16px;\r\nletter-spacing: normal;\r\nline-height: normal;\r\nmargin-bottom: 0px;\r\nmargin-left: 0px;\r\nmargin-right: 0px;\r\nmargin-top: 0px;\r\npadding-bottom: 0px;\r\npadding-left: 0px;\r\npadding-right: 0px;\r\npadding-top: 0px;\r\ntext-align: -webkit-auto;\r\ntext-indent: 0px;\r\ntext-shadow: none;\r\ntext-transform: none;\r\nwidth: 200px;\r\nword-spacing: 0px;\r\n}\r\n\r\n.required\r\n{\r\nborder:none;\r\ncolor:red\r\n}\r\n\r\n.captcha_img\r\n{\r\nborder-width:0px;\r\nmargin: 0px;\r\npadding: 0px;\r\ncursor:pointer;\r\n\r\n\r\n}\r\n\r\n.captcha_refresh\r\n{\r\nwidth:18px;\r\nborder-width:0px;\r\nmargin: 0px;\r\npadding: 0px;\r\nvertical-align:middle;\r\ncursor:pointer;\r\n}\r\n\r\n.captcha_input\r\n{\r\nheight:20px;\r\nborder-width:1px;\r\nmargin: 0px;\r\npadding: 0px;\r\nvertical-align:middle;\r\n}\r\n\r\n.file_upload\r\n{\r\n-webkit-appearance: none;\r\n-webkit-border-horizontal-spacing: 2px;\r\n-webkit-border-vertical-spacing: 2px;\r\n-webkit-box-align: baseline;\r\n-webkit-rtl-ordering: logical;\r\n-webkit-user-select: text;\r\nbackground-color: transparent;\r\nborder-bottom-color: black;\r\nborder-bottom-style: none;\r\nborder-bottom-width: 0px;\r\nborder-collapse: separate;\r\nborder-left-color: black;\r\nborder-left-style: none;\r\nborder-left-width: 0px;\r\nborder-right-color: black;\r\nborder-right-style: none;\r\nborder-right-width: 0px;\r\nborder-top-color: black;\r\nborder-top-style: none;\r\nborder-top-width: 0px;\r\ncolor: black;\r\ncursor: auto;\r\ndisplay: inline-block;\r\nfont-family: Arial;\r\nfont-size: 13px;\r\nfont-weight: normal;\r\nheight: 22px;\r\nletter-spacing: normal;\r\nline-height: normal;\r\nmargin-bottom: 0px;\r\nmargin-left: 0px;\r\nmargin-right: 0px;\r\nmargin-top: 0px;\r\npadding-bottom: 0px;\r\npadding-left: 0px;\r\npadding-right: 0px;\r\npadding-top: 0px;\r\ntext-align: start;\r\ntext-indent: 0px;\r\ntext-shadow: none;\r\ntext-transform: none;\r\nwidth: 238px;\r\nword-spacing: 0px;\r\n}         \r\n.captcha_table , .captcha_table input\r\n{\r\n  font-size: 15px !important;\r\n}  \r\n', '', '', '', '', 17, 0, '13#**id**#Name:#**label**#type_name#****#14#**id**#E-mail:#**label**#type_submitter_mail#****#15#**id**#Subject:#**label**#type_text#****#16#**id**#Message:#**label**#type_textarea#****#11#**id**#type_submit_reset_11#**label**#type_submit_reset#****#7#**id**#map_7#**label**#type_map#****#8#**id**#Text:#**label**#type_text#****#9#**id**#Textarea:#**label**#type_textarea#****#10#**id**#Date:#**label**#type_date#****#12#**id**#Upload a File:#**label**#type_file_upload#****#1#**id**#Name:#**label**#type_name#****#2#**id**#E-mail:#**label**#type_submitter_mail#****#3#**id**#Subject:#**label**#type_text#****#4#**id**#Message:#**label**#type_textarea#****#5#**id**#type_submit_reset_5#**label**#type_submit_reset#****#');
query1;


$sql_Form_Maker_submits="CREATE TABLE IF NOT EXISTS `".$wpdb->prefix."formmaker_submits` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `form_id` int(11) NOT NULL,
  `element_label` varchar(128) NOT NULL,
  `element_value` varchar(600) NOT NULL,
  `group_id` int(11) NOT NULL,
  `date` datetime NOT NULL,
  `ip` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1" ;

$wpdb->query($sql_Form_Maker);
$wpdb->query($sql_Form_Maker_contact);
$wpdb->query($sql_Form_Maker_submits);

}


register_activation_hook( __FILE__, 'formmaker_activate' );