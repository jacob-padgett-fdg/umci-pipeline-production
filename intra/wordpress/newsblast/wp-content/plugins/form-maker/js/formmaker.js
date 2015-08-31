j=2;//choices id
var c;
var z=0;
var main_location=document.getElementById('file_location_root').value;
var upload_location=document.getElementById('upload_location').value;
var a=new Array();
function active_reset(val, id)
{
	if(val)
	{
		document.getElementById(id+'_element_reset').style.display="inline";
	}
	else
	{
		document.getElementById(id+'_element_reset').style.display="none";
	}
}

function check_required(a)
{
	alert('"Submit" and "Reset" buttons are disabled in back end.');
}
function change_field_name(id, x)
{
	value=x.value;
	if(value==id+"_element")
	{
	alert('"Field Name" should differ from "Field Id".')
	x.value="";
	}
	else
	{
	document.getElementById(id+'_element').name=value;
	document.getElementById(id+'_element_label').innerHTML=value;
	}
}
function change_field_value(id, value)
{
	document.getElementById(id+'_element').value=value;
}
function return_attributes(id)
{
	attr_names= new Array();
	attr_value= new Array();
	var input=document.getElementById(id);
	if(input)
	{
		atr=input.attributes;
			for(i=0;i<30;i++)
				if(atr[i] )
				{
					if(atr[i].name.indexOf("add_")==0)
					{
						attr_names.push(atr[i].name.replace('add_',''));
						attr_value.push(atr[i].value);
					}
				}
	}
		return Array(attr_names, attr_value);
}

function refresh_attr(x,type)
{
	switch(type)
	{
		case "type_text":
			
		{
			id_array=Array();
			id_array[0]=x+'_element';
			break;
		}
		
		case "type_name":
			
		{
			id_array=Array();
			id_array[0]=x+'_element_first';
			id_array[1]=x+'_element_last';
			id_array[2]=x+'_element_title';
			id_array[3]=x+'_element_middle';
			break;
		}
		
		case "type_checkbox":
			
		{
			id_array=Array();
			for(z=0;z<50;z++)
				id_array[z]=x+'_element'+z;
			break;
		}
		
		case "type_time":
			
		{
			id_array=Array();
			id_array[0]=x+'_hh';
			id_array[1]=x+'_mm';
			id_array[2]=x+'_ss';
			id_array[3]=x+'_am_pm';
			break;
		}
		case "type_date":
			
		{
			id_array=Array();
			id_array[0]=x+'_element';
			id_array[1]=x+'_button';
			break;
		}
		
		case "type_date_fields":
			
		{
			id_array=Array();
			id_array[0]=x+'_day';
			id_array[1]=x+'_month';
			id_array[2]=x+'_year';
			break;
		}
		
		case "type_captcha":
			
		{
			id_array=Array();
			id_array[0]='wd_captcha';
			id_array[1]='wd_captcha_input';
			id_array[2]='element_refresh';
			break;
		}
		
		case "type_submit_reset":
			
		{
			id_array=Array();
			id_array[0]=x+'_element_submit';
			id_array[1]=x+'_element_reset';
			break;
		}
	}
		
	for(q=0; q<id_array.length;q++)
	{
		id=id_array[q];
		var input=document.getElementById(id);
		if(input)
		{
			atr=input.attributes;
			for(i=0;i<30;i++)
				if(atr[i])
					{
						if(atr[i].name.indexOf("add_")==0)
						{
							input.removeAttribute(atr[i].name);
							i--;
						}
					}
				
			for(i=0;i<10;i++)
				if(document.getElementById("attr_name"+i))
				{
					try{input.setAttribute("add_"+document.getElementById("attr_name"+i).value, document.getElementById("attr_value"+i).value)}
					catch(err)
					{
						alert('Only letters, numbers, hyphens and underscores are allowed.');
					}
				}
		}
	}
}
function add_attr(i, type)
{
		
	var el_attr_table=document.getElementById('attributes');
	j=parseInt(el_attr_table.lastChild.getAttribute('idi'))+1;
	w_attr_name[j]="attribute";
	w_attr_value[j]="value";
	var el_attr_tr = document.createElement('tr');
		el_attr_tr.setAttribute("id", "attr_row_"+j);
		el_attr_tr.setAttribute("idi", j);
	var el_attr_td_name = document.createElement('td');
		el_attr_td_name.style.cssText = 'width:100px';
	var el_attr_td_value = document.createElement('td');
		el_attr_td_value.style.cssText = 'width:100px';
	
	var el_attr_td_X = document.createElement('td');
	var el_attr_name = document.createElement('input');
		el_attr_name.setAttribute("type", "text");
		el_attr_name.style.cssText = "width:100px";
		el_attr_name.setAttribute("value", w_attr_name[j]);
		el_attr_name.setAttribute("id", "attr_name"+j);
		el_attr_name.setAttribute("onChange", "change_attribute_name('"+i+"', this, '"+type+"')");	
		
	var el_attr_value = document.createElement('input');
		el_attr_value.setAttribute("type", "text");
		el_attr_value.style.cssText = "width:100px";
		el_attr_value.setAttribute("value", w_attr_value[j]);
		el_attr_value.setAttribute("id", "attr_value"+j);
		el_attr_value.setAttribute("onChange", "change_attribute_value('"+i+"', "+j+", '"+type+"')");
	
	var el_attr_remove = document.createElement('img');
		el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
		el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
		el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
		el_attr_remove.setAttribute("align", 'top');
		el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", '"+type+"')");
	el_attr_table.appendChild(el_attr_tr);
	el_attr_tr.appendChild(el_attr_td_name);
	el_attr_tr.appendChild(el_attr_td_value);
	el_attr_tr.appendChild(el_attr_td_X);
	el_attr_td_name.appendChild(el_attr_name);
	el_attr_td_value.appendChild(el_attr_value);
	el_attr_td_X.appendChild(el_attr_remove);
refresh_attr(i, type);
}
function change_attribute_value(id, x, type)
{
	if(!document.getElementById("attr_name"+x).value)
	{
		alert('The name of the attribute is required.');
		return
	}
	
	if(document.getElementById("attr_name"+x).value.toLowerCase()=="style")
	{
		alert('Sorry, you cannot add a style attribute here. Use "Class name" instead.');
		return
	}
	
	refresh_attr(id, type);
}
function change_attribute_name(id, x, type)
{
	value=x.value;
	if(!value)
	{
		alert('The name of the attribute is required.');
		return;
	}
	
	if(value.toLowerCase()=="style")
	{
		alert('Sorry, you cannot add a style attribute here. Use "Class name" instead.');
		return;
	}
	
	if(value==parseInt(value))
	{
		alert('The name of the attribute cannot be a number.');
		return;
	}
	
	if(value.indexOf(" ")!=-1)
	{	
		var regExp = /\s+/g;
		value=value.replace(regExp,''); 
		x.value=value;
		alert("The name of the attribute cannot contain a space.");
		refresh_attr(id);
		return;
	}	
	
	refresh_attr(id, type);
	
}
function remove_attr(id, el_id,type)
{
	tr=document.getElementById("attr_row_"+id);
	tr.parentNode.removeChild(tr);
	refresh_attr(el_id, type);
}
function change_attributes(id, attr)
{
	
var div = document.createElement('div');
var element=document.getElementById(id);
	element.setAttribute(attr, '');
}
function add_button(i)
{
	edit_main_td4=document.getElementById('buttons');
	if(buttons.lastChild)
		j=parseInt(buttons.lastChild.getAttribute("idi"))+1;
	else
		j=1;
	var table_button = document.createElement('table');
	
	table_button.setAttribute("width", "100%");
	table_button.setAttribute("border", "0");
	table_button.setAttribute("id", "button_opt"+j);
	table_button.setAttribute("idi", j);
	var tr_button = document.createElement('tr');
	var tr_hr = document.createElement('tr');
	
	var td_button = document.createElement('td');
	var td_X = document.createElement('td');
	var td_hr = document.createElement('td');
	td_hr.setAttribute("colspan", "3");
	
	tr_hr.appendChild(td_hr);
	tr_button.appendChild(td_button);
	tr_button.appendChild(td_X);
	table_button.appendChild(tr_hr);
	table_button.appendChild(tr_button);
	
	var br1 = document.createElement('br');
	
	var hr = document.createElement('hr');
	
	hr.setAttribute("id", "br"+j);
	
	
	
	
	var el_title_label = document.createElement('label');
	
		el_title_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		
		el_title_label.innerHTML = "Button name";
	
	var el_title = document.createElement('input');
	
		el_title.setAttribute("id", "el_title"+j);
		
		el_title.setAttribute("type", "text");
	
		el_title.setAttribute("value", "Button");
		
		el_title.style.cssText =   "width:100px; margin-left:43px; padding:0; border-width: 1px";
		
		el_title.setAttribute("onKeyUp", "change_label('"+i+"_element"+j+"', this.value);");
	
	
	
	var el_func_label = document.createElement('label');
	
		el_func_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		
		el_func_label.innerHTML = "OnClick function";
	
	var el_func = document.createElement('input');
	
		el_func.setAttribute("id", "el_func"+j);
		
		el_func.setAttribute("type", "text");
		
		el_func.setAttribute("value", "");
		
		el_func.style.cssText =   "width:100px; margin-left:20px;; padding:0; border-width: 1px";
		
		el_func.setAttribute("onKeyUp", "change_func('"+i+"_element"+j+"', this.value);");
	
	var el_choices_remove = document.createElement('img');
	
		el_choices_remove.setAttribute("id", "el_button"+j+"_remove");
		
		el_choices_remove.setAttribute("src", main_location+'/images/delete.png');
		
		el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
		
		el_choices_remove.setAttribute("align", 'top');
		
		el_choices_remove.setAttribute("onClick", "remove_button("+j+","+i+")");
	
	
	
	td_hr.appendChild(hr);
	
	td_button.appendChild(el_title_label);
	
	td_button.appendChild(el_title);
	td_button.appendChild(br1);
	td_button.appendChild(el_func_label);
	
	td_button.appendChild(el_func);
	td_X.appendChild(el_choices_remove);
	edit_main_td4.appendChild(table_button);
	
	element='button';	type='button'; 
	
	td2=document.getElementById(i+"_element_section");
	
	var adding = document.createElement(element);
			adding.setAttribute("type", type);
			adding.setAttribute("id", i+"_element"+j);
			adding.setAttribute("name", i+"_element"+j);
			adding.setAttribute("value", "Button");
			adding.innerHTML =  "Button";
			adding.setAttribute("onclick", "");
			
			
	td2.appendChild(adding);
	
refresh_attr(i,'type_checkbox');
}
function remove_button(j,i)
{
	table=document.getElementById('button_opt'+j);
	button=document.getElementById(i+'_element'+j);
	table.parentNode.removeChild(table);
	button.parentNode.removeChild(button);
}
function change_date_format(value, id)
{
	input_p=document.getElementById(id+'_button');
		input_p.setAttribute("onclick", "return showCalendar('"+id+"_element' , '"+value+"')");
		input_p.setAttribute("format", value);
}

function set_send(id)
{	
if(document.getElementById(id).value=="yes")
	document.getElementById(id).setAttribute("value", "no")
else
	document.getElementById(id).setAttribute("value", "yes")
}
function change_class(x,id)
{
	if(document.getElementById(id+'_label_section'))
	document.getElementById(id+'_label_section').setAttribute("class",x);
	if(document.getElementById(id+'_element_section'))
	document.getElementById(id+'_element_section').setAttribute("class",x);
}
function set_required(id)
{	
	if(document.getElementById(id).value=="yes")
	{
		document.getElementById(id).setAttribute("value", "no");
		document.getElementById(id+"_element").innerHTML="";
	}	
	else
	{
		document.getElementById(id).setAttribute("value", "yes")
		document.getElementById(id+"_element").innerHTML="&nbsp*";
	}
}
function flow_hor(id)
{
	tbody=document.getElementById(id+'_table_little');
	td_array= new Array();
	n=tbody.childNodes.length;
	for(k=0; k<n;k++)
		td_array[k]=tbody.childNodes[k].childNodes[0];
		
	for(k=0; k<n;k++)
		tbody.removeChild(tbody.childNodes[0]);
		
	var tr = document.createElement('tr');
           	tr.setAttribute("id", id+"_hor");
			
	tbody.appendChild(tr);
	for(k=0; k<n;k++)
		tr.appendChild(td_array[k]);
}
function flow_ver(id)
{	
	tbody=document.getElementById(id+'_table_little');
	tr=document.getElementById(id+'_hor');
	td_array= new Array();
	n=tr.childNodes.length;
	
	for(k=0; k<n;k++)
		td_array[k]=tr.childNodes[k];
			
	tbody.removeChild(tr);
	
	for(k=0; k<n;k++)
	{      	
		var tr_little = document.createElement('tr');
			tr_little.setAttribute("id", id+"_element_tr"+td_array[k].getAttribute("idi"));
		tr_little.appendChild(td_array[k]);
		tbody.appendChild(tr_little);
	}			
}
function check_isnum_3_10(e)
{
	
   	var chCode1 = e.which || e.keyCode;
    	if (chCode1 > 31 && (chCode1 < 51 || chCode1 > 57))
        return false
	else if((document.getElementById('captcha_digit').value+(chCode1-48))>9)
        return false;
	return true;
}

function set_sel_am_pm(select_)
{
	if(select_.options[0].selected) 
	{
		select_.options[0].setAttribute("selected", "selected");
		select_.options[1].removeAttribute("selected");
	}
	else
	{
		select_.options[1].setAttribute("selected", "selected");
		select_.options[0].removeAttribute("selected");
	}

}
function change_captcha_digit(digit)
{
	captcha=document.getElementById('wd_captcha');
	if(document.getElementById('captcha_digit').value)
	{	
		captcha.setAttribute("digit", digit);
	
		captcha.setAttribute("src", main_location+"/wd_captcha.php?digit="+digit);
		document.getElementById('wd_captcha_input').style.width=(document.getElementById('captcha_digit').value*10+15)+"px";
	}
	else
	{
		captcha.setAttribute("digit", "6");
		captcha.setAttribute("src", main_location+"/wd_captcha.php?digit=6");
		document.getElementById('wd_captcha_input').style.width=(6*10+15)+"px";
	}

	

}
function second_no(id)
{	
	time_box=document.getElementById(id+'_tr_time1');
	text_box=document.getElementById(id+'_tr_time2');
	second_box=document.getElementById(id+'_td_time_input3');
	second_text=document.getElementById(id+'_td_time_label3');
	document.getElementById(id+'_td_time_input2').parentNode.removeChild(document.getElementById(id+'_td_time_input2').nextSibling);
	time_box.removeChild(second_box);
	text_box.removeChild(second_text.previousSibling);
	text_box.removeChild(second_text);
	
}

function second_yes(id, w_ss)
{	
	time_box=document.getElementById(id+'_tr_time1');
	text_box=document.getElementById(id+'_tr_time2');
	
	var td_time_input2_ket = document.createElement('td');
           	td_time_input2_ket.setAttribute("align", "center");
	var td_time_input3 = document.createElement('td');
           	td_time_input3.setAttribute("id", id+"_td_time_input3");
			
      	var td_time_label2_ket = document.createElement('td');
	
      	var td_time_label3 = document.createElement('td');
           	td_time_label3.setAttribute("id", id+"_td_time_label3");

	var mm_ = document.createElement('span');
		mm_.style.cssText = "font-style:bold; vertical-align:middle";
		mm_.innerHTML="&nbsp;:&nbsp;";
	td_time_input2_ket.appendChild(mm_);
		
	var ss = document.createElement('input');

           	ss.setAttribute("type", 'text');
           	ss.setAttribute("value", w_ss);
		
           	ss.setAttribute("class", "time_box");
		ss.setAttribute("id", id+"_ss");
		ss.setAttribute("name", id+"_ss");
		ss.setAttribute("onKeyPress", "return check_second(event, '"+id+"_ss')");
		ss.setAttribute("onKeyUp", "change_second('"+id+"_ss')");
		ss.setAttribute("onBlur", "add_0('"+id+"_ss')");
	var ss_label = document.createElement('label');
           	ss_label.setAttribute("class", "mini_label");
		ss_label.innerHTML="SS";

	td_time_input3.appendChild(ss);
	td_time_label3.appendChild(ss_label);
	
	if(document.getElementById(id+'_am_pm_select'))
	{
		select_=document.getElementById(id+"_am_pm_select");
		select_text=document.getElementById(id+"_am_pm_label");
		
		time_box.insertBefore(td_time_input3, select_);
		time_box.insertBefore(td_time_input2_ket, td_time_input3);
		
		text_box.insertBefore(td_time_label3, select_text);
		text_box.insertBefore(td_time_label2_ket, td_time_label3);
	}
	else
	{
	time_box.appendChild(td_time_input2_ket);
	time_box.appendChild(td_time_input3);
	text_box.appendChild(td_time_label2_ket);
	text_box.appendChild(td_time_label3);
	}
refresh_attr(id, 'type_time');
}

function check_isnum(e)
{
	
   	var chCode1 = e.which || e.keyCode;
    	if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57))
        return false;
	return true;
}

function change_w_style(id, w)
{
	if(document.getElementById(id))
	document.getElementById(id).style.width=w+"px";
}

function change_w_label(id, w)
{
	if(document.getElementById(id))
	document.getElementById(id).innerHTML=w;
}

function change_h_style(id, h)
{
	document.getElementById(id).style.height=h+"px";
}
function change_w(id, w)
{
	document.getElementById(id).setAttribute("width", w)
}

function change_h(id, h)
{
	document.getElementById(id).setAttribute("height", h);
}
function captcha_refresh(id)
{	
	srcArr=document.getElementById(id).src.split("&r=");
	document.getElementById(id).src=srcArr[0]+'&r='+Math.floor(Math.random()*100);
	document.getElementById(id+"_input").value='';
}


function up_row(id)
{
	form=document.getElementById(id).parentNode;
	k=0;
	while(form.childNodes[k])
	{
	if(form.childNodes[k].getAttribute("id"))
	if(id==form.childNodes[k].getAttribute("id"))
		break;
	k++;
	}
	if(k!=0)
	{
		up=form.childNodes[k-1];
		down=form.childNodes[k];
		form.removeChild(down);
		form.insertBefore(down, up);
		refresh_();
	}
}

function down_row(id)
{
	form=document.getElementById(id).parentNode;
	l=form.childNodes.length;
	k=0;
	while(form.childNodes[k])
	{
	if(id==form.childNodes[k].id)
		break;
	k++;
	}

	if(k!=l-1)
	{
		up=form.childNodes[k];
		down=form.childNodes[k+2];
		form.removeChild(up);
if(!down)
down=null;
		form.insertBefore(up, down);
		refresh_();
	}
}
function right_row(id)
{
	tr=document.getElementById(id);
	td_big=tr.parentNode.parentNode.parentNode;
	tr_big=tr.parentNode.parentNode.parentNode.parentNode;
	if(td_big.nextSibling!=null)
	{
		td_next=td_big.nextSibling;
		td_next.firstChild.firstChild.appendChild(tr);
	
	}
	else
	{
		
	    var new_td = document.createElement('td');
		new_td.setAttribute("valign", "top");
		
	    var new_table = document.createElement('table');
	    var new_tbody = document.createElement('tbody');
	
	    tr_big.appendChild(new_td);
	
	    new_td.appendChild(new_table);
	
	    new_table.appendChild(new_tbody);
	
	    new_tbody.appendChild(tr);
	    
	}
	
	if(td_big.firstChild.firstChild.firstChild==null)
		tr_big.removeChild(td_big);
	refresh_();
}
function left_row(id)
{
	tr=document.getElementById(id);
	td_big=tr.parentNode.parentNode.parentNode;
	tr_big=tr.parentNode.parentNode.parentNode.parentNode;
	if(td_big.previousSibling!=null)
	{
		
		td_previous=td_big.previousSibling;
		td_previous.firstChild.firstChild.appendChild(tr);
	if(td_big.firstChild.firstChild.firstChild==null)
		tr_big.removeChild(td_big);
	}
	refresh_();
}
function Disable()
{	
	select_=document.getElementById('sel_el_pos');
	select_.setAttribute("disabled", "disabled");
	select_.innerHTML="";
}

function Enable()
{
	var pos=document.getElementsByName("el_pos");
			pos[0].setAttribute("checked", "checked");

	select_ = document.getElementById('sel_el_pos');
	select_.innerHTML="";
	form_view=document.getElementById('form_view');
	GLOBAL_tr=form_view.firstChild;
	
	for (x=0; x < GLOBAL_tr.childNodes.length; x++)
	{
		td=GLOBAL_tr.childNodes[x];
		tbody=td.firstChild.firstChild;
		for (y=0; y < tbody.childNodes.length; y++)
		{
			tr=tbody.childNodes[y];
			var option = document.createElement('option');
					option.setAttribute("id", tr.id+"_sel_el_pos");
					option.setAttribute("value", tr.id);
				option.innerHTML=document.getElementById( tr.id+'_element_label').innerHTML;
			select_.appendChild(option);
	
		}
	}
	
	select_.removeAttribute("disabled");
}

function set_checked(id)
{
	checking=document.getElementById(id);
	if(checking.checked)
		checking.setAttribute("checked", "checked");
	if(!checking.checked)
		checking.removeAttribute("checked");
}

function set_default(id, j)
{
	for(k=0; k<100; k++)
		if(document.getElementById(id+"_element"+k))
			if(!document.getElementById(id+"_element"+k).checked)
				document.getElementById(id+"_element"+k).removeAttribute("checked");
			else
				document.getElementById(id+"_element"+j).setAttribute("checked", "checked");
}

function set_select(select_)
{
	for (p = select_.length - 1; p>=0; p--) 
	    if (select_.options[p].selected) 
		select_.options[p].setAttribute("selected", "selected");
	    else
  		select_.options[p].removeAttribute("selected");
}

function add_0(id)
{
	input=document.getElementById(id);
	if(input.value.length==1)
	{
		input.value='0'+input.value;
		input.setAttribute("value", input.value);
	}
}

function change_hour(ev, id, hour_interval)
{
	if(check_hour(ev, id, hour_interval))
	{
		input=document.getElementById(id);
		input.setAttribute("value", input.value);
	}
}

function change_minute(ev, id)
{
	if(check_minute(ev, id))
	{
		input=document.getElementById(id);
		input.setAttribute("value", input.value);
	}
}

function change_second(ev, id)

{
	if(check_second(ev, id))
	{
		input=document.getElementById(id);
		input.setAttribute("value", input.value);
	}
}

function check_hour(e, id, hour_interval)
{
   	var chCode1 = e.which || e.keyCode;
    	if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57))
        return false;
	hour=""+document.getElementById(id).value+String.fromCharCode(chCode1);

	hour=parseFloat(hour);
	if((hour<0) || (hour>hour_interval))
        	return false;
	return true;
} 

function check_minute(e, id)
{	
		
	
   	var chCode1 = e.which || e.keyCode;
    	if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57))
        return false;
	minute=""+document.getElementById(id).value+String.fromCharCode(chCode1);

	minute=parseFloat(minute);
	if((minute<0) || (minute>59))
        	return false;
	return true;
} 

function check_second(e, id)
{	
		
	
   	var chCode1 = e.which || e.keyCode;
    	if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57))
        return false;
	second=""+document.getElementById(id).value+String.fromCharCode(chCode1);

	second=parseFloat(second);
	if((second<0) || (second>59))
        	return false;
	return true;
} 
function change_day(ev, id)
{
	if(check_day(ev, id))
	{
		input=document.getElementById(id);
		input.setAttribute("value", input.value);
	}
}

function change_month(ev, id)
{
	if(check_month(ev, id))
	{
		input=document.getElementById(id);
		input.setAttribute("value", input.value);
	}
}

function change_year(id)
{
	year=document.getElementById(id).value;
	
	from=parseFloat(document.getElementById(id).getAttribute('from'));
	to=parseFloat(document.getElementById(id).getAttribute('to'));
	
	year=parseFloat(year);
	
	if((year>=from) && (year<=to))
		document.getElementById(id).setAttribute("value", year);
	else
		document.getElementById(id).setAttribute("value", '');
}


function check_day(e, id)
{	
   	var chCode1 = e.which || e.keyCode;
    	if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57))
        return false;
	day=""+document.getElementById(id).value+String.fromCharCode(chCode1);

	if(day.length>2)
        	return false;
			
	if(day=='00')
        	return false;
			
	day=parseFloat(day);
	if((day<0) || (day>31))
        	return false;
	return true;
} 

function check_month(e, id)
{	
		
	
   	var chCode1 = e.which || e.keyCode;
    	if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57))
        return false;
	month=""+document.getElementById(id).value+String.fromCharCode(chCode1);
	
	if(month.length>2)
        	return false;
			
	if(month=='00')
        	return false;
			
	month=parseFloat(month);
	if((month<0) || (month>12))
        	return false;
	return true;
} 
function check_year2(id)
{
	year=document.getElementById(id).value;
	
	from=parseFloat(document.getElementById(id).getAttribute('from'));
	
	year=parseFloat(year);
	
	if(year<from)
	{
		document.getElementById(id).value='';
		alert('The value of "year" is not valid.');
	}
}

function check_year1(e, id)
{	
   	var chCode1 = e.which || e.keyCode;
    	if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57))
        return false;

	year=""+document.getElementById(id).value+String.fromCharCode(chCode1);
	
	to=parseFloat(document.getElementById(id).getAttribute('to'));
	
	year=parseFloat(year);
	
	if(year>to)
        	return false;
	return true;
} 

function label_top(num)
{	
	table=document.getElementById(num+'_elemet_table');
	td1=document.getElementById(num+'_label_section');
	td2=document.getElementById(num+'_element_section');
    var table_t = document.createElement('tbody');
    var new_td1 = document.createElement('td');
    	new_td1 = td1;
    var new_td2 = document.createElement('td');
    	new_td2 = td2;
    var tr1 = document.createElement('tr');
    var tr2 = document.createElement('tr');
	//table.innerHTML=" ";
 while (table.firstChild)
      table.removeChild(table.firstChild);
    tr1.appendChild(new_td1);
    tr2.appendChild(new_td2);
    table_t.appendChild(tr1);
    table_t.appendChild(tr2);
    table.appendChild(table_t);
}

function label_left(num)
{
	table=document.getElementById(num+'_elemet_table');
	td1=document.getElementById(num+'_label_section');
	td2=document.getElementById(num+'_element_section');
       var table_t = document.createElement('tbody');
    var new_td1 = document.createElement('td');
    	new_td1 = td1;
    var new_td2 = document.createElement('td');
    	new_td2 = td2;
    var tr = document.createElement('tr');
	//table.innerHTML=" ";
 while (table.firstChild)
      table.removeChild(table.firstChild);
    tr.appendChild(new_td1);
    tr.appendChild(new_td2);
    table_t.appendChild(tr);
    table.appendChild(table_t);
}

function delete_value(id)
{
	if( window.getComputedStyle ) 
		{
		  ofontStyle = window.getComputedStyle(document.getElementById(id),null).fontStyle;
		} else if( document.getElementById(id).currentStyle ) {
		  ofontStyle = document.getElementById(id).currentStyle.fontStyle;
		}
	if(ofontStyle=="italic")
	{
		document.getElementById(id).value="";
		destroyChildren(document.getElementById(id));
		document.getElementById(id).setAttribute("class", "input_active");
		document.getElementById(id).className='input_active';
	}
}

function return_value(id)
{
	input=document.getElementById(id);
	if(input.value=="")
	{
			input.value=input.title;
		input.className='input_deactive';
		input.setAttribute("class", 'input_deactive');
	}
}

function change_value(id)

{
	input=document.getElementById(id);
	 
	tag=input.tagName;
	if(tag=="TEXTAREA")
	{
// destroyChildren(input)
	input.innerHTML=input.value;
	}
	else
	input.setAttribute("value", input.value);

}

function change_input_value(first_value, id)
{	
	input=document.getElementById(id);
	input.title=first_value;
	
if( window.getComputedStyle ) 
{
  ofontStyle = window.getComputedStyle(input,null).fontStyle;
} else if( input.currentStyle ) {
  ofontStyle = input.currentStyle.fontStyle;
}
	if(ofontStyle=="italic")
	{	
		input.value=first_value;
		input.setAttribute("value", first_value);
	}
}
function change_file_value(destination, id, prefix , postfix )
{	
	if(typeof(prefix)=='undefined') {prefix=''; postfix=''};
	input=document.getElementById(id);
	input.value=prefix+destination+postfix;
	input.setAttribute("value", prefix+destination+postfix);
	
}
function close_window() 
{
enable();
document.getElementById('edit_table').innerHTML="";
document.getElementById('show_table').innerHTML="";
document.getElementById('main_editor').style.display="none";
if(document.getElementsByTagName("iframe")[0])
{
ifr_id=document.getElementsByTagName("iframe")[0].id;
ifr=getIFrameDocument(ifr_id)
ifr.body.innerHTML="";
}
document.getElementById('editor').value="";
document.getElementById('editing_id').value="";
document.getElementById('element_type').value="";

}

function change_label(id, label)
{
	document.getElementById(id).innerHTML=label;
	document.getElementById(id).value=label;
}

function change_func(id, label)
{
	document.getElementById(id).setAttribute("onclick", label);
}

function change_in_value(id, label)
{
	document.getElementById(id).setAttribute("value", label);
}

function change_size(size, num)
{
	document.getElementById(num+'_element').style.width=size+'px';
	if(document.getElementById(num+'_element_input'))
		document.getElementById(num+'_element_input').style.width=size+'px';
	switch(size)
	{
		case '111':
		{
			document.getElementById(num+'_element').setAttribute("rows", "2"); break;
		}
		case '222':
		{
			document.getElementById(num+'_element').setAttribute("rows", "4");break;
		}
		case '444':
		{
			document.getElementById(num+'_element').setAttribute("rows", "8");break;
		}
	}
}

function add_choise(type, num)
{
var q=0;
	if(document.getElementById(num+'_hor'))
	{
		q=1;
		flow_ver(num);
	}
	j++;	
	if(type=='radio' || type=='checkbox')
	{
		element='input';
	
		var table = document.getElementById(num+'_table_little');
		var tr = document.createElement('tr');
			tr.setAttribute("id", num+"_element_tr"+j);
		var td = document.createElement('td');
			td.setAttribute("valign", "top");
			td.setAttribute("id", num+"_td_little"+j);
			td.setAttribute("idi", j);
		
		var adding = document.createElement(element);
		adding.setAttribute("type", type);
		adding.setAttribute("value", "");
			adding.setAttribute("id", num+"_element"+j);
		if(type=='checkbox')
		{	
			adding.setAttribute("onClick", "set_checked('"+num+"_element"+j+"')");
			adding.setAttribute("name", num+"_element"+j);
		}
			
		if(type=='radio')
		{
			adding.setAttribute("onClick", "set_default('"+num+"','"+j+"')");
			adding.setAttribute("name", num+"_element");
		}
		
		
		var label_adding = document.createElement('label');
			label_adding.setAttribute("id", num+"_label_element"+j);
			label_adding.setAttribute("class", "ch_rad_label");
		    td.appendChild(adding);
		    td.appendChild(label_adding);
		    tr.appendChild(td);
		    table.appendChild(tr);
		
		var choices_td= document.getElementById('choices');
		var br = document.createElement('br');
		br.setAttribute("id", "br"+j);
		var el_choices = document.createElement('input');
		el_choices.setAttribute("id", "el_choices"+j);
		el_choices.setAttribute("type", "text");
		el_choices.setAttribute("value", "");
		el_choices.style.cssText =   "width:100px; margin:0; padding:0; border-width: 1px";
		el_choices.setAttribute("onKeyUp", "change_label('"+num+"_label_element"+j+"', this.value)");
		el_choices.setAttribute("onKeyUp", "change_label('"+num+"_label_element"+j+"', this.value); change_in_value('"+num+"_element"+j+"', this.value)");
	
		var el_choices_remove = document.createElement('img');
			el_choices_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_choices_remove.setAttribute("src", main_location+'/images/delete.png');
			el_choices_remove.style.cssText =  'cursor:pointer;vertical-align:middle; margin:3px';
			el_choices_remove.setAttribute("align", 'top');
			el_choices_remove.setAttribute("onClick", "remove_choise('"+j+"','"+num+"')");
	
	    choices_td.appendChild(br);
	    choices_td.appendChild(el_choices);
	    choices_td.appendChild(el_choices_remove);
	    
	refresh_attr(num, 'type_checkbox');
	
	}
	
	if(type=='select')
	{
		var select_ = document.getElementById(num+'_element');
		var option = document.createElement('option');
			option.setAttribute("id", num+"_option"+j);
			
		    select_.appendChild(option);
		
		var choices_td= document.getElementById('choices');
		var br = document.createElement('br');
		br.setAttribute("id", "br"+j);
		var el_choices = document.createElement('input');
			el_choices.setAttribute("id", "el_option"+j);
			el_choices.setAttribute("type", "text");
			el_choices.setAttribute("value", "");
			el_choices.style.cssText =   "width:100px; margin:0; padding:0; border-width: 1px";
			el_choices.setAttribute("onKeyUp", "change_label('"+num+"_option"+j+"', this.value)");
			
		var el_choices_remove = document.createElement('img');
			el_choices_remove.setAttribute("id", "el_option"+j+"_remove");
			el_choices_remove.setAttribute("src", main_location+'/images/delete.png');
			el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_choices_remove.setAttribute("align", 'top');
			el_choices_remove.setAttribute("onClick", "remove_option('"+j+"','"+num+"')");
			
		var el_choices_dis = document.createElement('input');
			el_choices_dis.setAttribute("type", 'checkbox');
			el_choices_dis.setAttribute("id", "el_option"+j+"_dis");
			el_choices_dis.setAttribute("onClick", "dis_option('"+num+"_option"+j+"', this.checked)");


	    choices_td.appendChild(br);
	    choices_td.appendChild(el_choices);
	    choices_td.appendChild(el_choices_dis);
	    choices_td.appendChild(el_choices_remove);
    }
	if(q==1)
	{
		flow_hor(num);
	}

}
function remove_choise(id, num)
{
var q=0;
	if(document.getElementById(num+'_hor'))
	{
		q=1;
		flow_ver(num);
	}
	j++;	
		var table = document.getElementById(num+'_table_little');
		var tr = document.getElementById(num+'_element_tr'+id);
		table.removeChild(tr);
		
		var choices_td= document.getElementById('choices');
		var el_choices = document.getElementById('el_choices'+id);
		var el_choices_remove = document.getElementById('el_choices'+id+'_remove');
		var br = document.getElementById('br'+id);
		
		choices_td.removeChild(el_choices);
		choices_td.removeChild(el_choices_remove);
		choices_td.removeChild(br);
			
		if(q==1)
		{
			flow_hor(num);
		}

}

function remove_option(id, num)
{
		var select_ = document.getElementById(num+'_element');
		var option = document.getElementById(num+'_option'+id);
			
		select_.removeChild(option);
		
		var choices_td= document.getElementById('choices');
		var el_choices = document.getElementById('el_option'+id);
		var el_choices_dis = document.getElementById('el_option'+id+'_dis');
		var el_choices_remove = document.getElementById('el_option'+id+'_remove');
		var br = document.getElementById('br'+id);
		
		choices_td.removeChild(el_choices);
		choices_td.removeChild(el_choices_dis);
		choices_td.removeChild(el_choices_remove);
		choices_td.removeChild(br);
}
function getIFrameDocument(aID){ 
var rv = null; 
// if contentDocument exists, W3C compliant (Mozilla) 
if (document.getElementById(aID).contentDocument){ 
rv = document.getElementById(aID).contentDocument; 
} else { 
// IE 
rv = document.frames[aID].document; 
} 
return rv; 
}
function delete_last_child()
{
	if(document.getElementsByTagName("iframe")[0]){
ifr_id=document.getElementsByTagName("iframe")[0].id;
ifr=getIFrameDocument(ifr_id);
ifr.body.innerHTML="";
	}
	document.getElementById('main_editor').style.display="none";
	document.getElementById('editor').value="";
	if(document.getElementById('show_table').lastChild)
	{
		var del1 = document.getElementById('show_table').lastChild;
		var del2 = document.getElementById('edit_table').lastChild;
		var main1 = document.getElementById('show_table');
		var main2 = document.getElementById('edit_table');
		main1.removeChild(del1);
		main2.removeChild(del2);
	}
}

function format_12(num, am_or_pm, w_hh, w_mm, w_ss)
{
    	tr_time1 = document.getElementById(num+'_tr_time1')
    	tr_time2 = document.getElementById(num+'_tr_time2')
   	var td1 = document.createElement('td');
        	td1.setAttribute("id", num+"_am_pm_select");
        	td1.setAttribute("class", "td_am_pm_select");
   	var td2 = document.createElement('td');
        	td2.setAttribute("id", num+"_am_pm_label");
        	td2.setAttribute("class", "td_am_pm_select");
		
	var am_pm_select = document.createElement('select');
        	am_pm_select.setAttribute("class", "am_pm_select");
        	am_pm_select.setAttribute("name",  num+"_am_pm");
        	am_pm_select.setAttribute("id",  num+"_am_pm");
        	am_pm_select.setAttribute("onchange", "set_sel_am_pm(this)");
		
	var am_option = document.createElement('option');
        	am_option.setAttribute("value", "am");
        	am_option.innerHTML="AM";
		
	var pm_option = document.createElement('option');
        	pm_option.setAttribute("value", "pm");
        	pm_option.innerHTML="PM";
	
	if(am_or_pm=="pm")
	        pm_option.setAttribute("selected", "selected");
	else
	        am_option.setAttribute("selected", "selected");

		
	var am_pm_label = document.createElement('label');
		am_pm_label.setAttribute("class", "mini_label");
		am_pm_label.innerHTML="AM/PM";
		
   	am_pm_select.appendChild(am_option);
   	am_pm_select.appendChild(pm_option);
   	td1.appendChild(am_pm_select);
   	td2.appendChild(am_pm_label);
   	tr_time1.appendChild(td1);
   	tr_time2.appendChild(td2);
	document.getElementById(num+'_hh').setAttribute("onKeyPress", "return check_hour(event, '"+num+"_hh',"+"'12'"+")");

    	document.getElementById(num+'_hh').value=w_hh;
    	document.getElementById(num+'_mm').value=w_mm;
	if(document.getElementById(num+'_ss'))
    	document.getElementById(num+'_ss').value=w_ss;
	
refresh_attr(num, 'type_time');
}

function format_24(num)
{
    	tr_time1 = document.getElementById(num+'_tr_time1')
    	td1 = document.getElementById(num+'_am_pm_select')
    	tr_time2 = document.getElementById(num+'_tr_time2')
    	td2 = document.getElementById(num+'_am_pm_label')
	tr_time1.removeChild(td1);
	tr_time2.removeChild(td2);
	document.getElementById(num+'_hh').setAttribute("onKeyPress", "return check_hour(event, '"+num+"_hh', '23')");
    	document.getElementById(num+'_hh').value="";
    	document.getElementById(num+'_mm').value="";
	if(document.getElementById(num+'_ss'))
    	document.getElementById(num+'_ss').value="";
}

function format_extended(num)
{
	w_size=document.getElementById(num+'_element_first').style.width;
    	tr_name1 = document.getElementById(num+'_tr_name1');
    	tr_name2 = document.getElementById(num+'_tr_name2');
	
   	var td_name_input1 = document.createElement('td');
        	td_name_input1.setAttribute("id", num+"_td_name_input_title");
		
   	var td_name_input4 = document.createElement('td');
        	td_name_input4.setAttribute("id", num+"_td_name_input_middle");
		
   	var td_name_label1 = document.createElement('td');
        	td_name_label1.setAttribute("id", num+"_td_name_label_title");
        	td_name_label1.setAttribute("align", "left");
		
   	var td_name_label4 = document.createElement('td');
        	td_name_label4.setAttribute("id", num+"_td_name_label_middle");
        	td_name_label4.setAttribute("align", "left");
		
	var title = document.createElement('input');
            title.setAttribute("type", 'text');
	    title.style.cssText = "border-width:1px; margin: 0px 10px 0px 0px; padding: 0px; width:40px";
	    title.setAttribute("id", num+"_element_title");
	    title.setAttribute("name", num+"_element_title");
	    title.setAttribute("onChange", "change_value('"+i+"_element_title')");
			
	var title_label = document.createElement('label');
	    title_label.setAttribute("class", "mini_label");
	    title_label.innerHTML="<!--repstart-->Title<!--repend-->";
			
	var middle = document.createElement('input');
		middle.setAttribute("type", 'text');
		middle.style.cssText = "border-width:1px; padding: 0px; width:"+w_size;
		middle.setAttribute("id", num+"_element_middle");
		middle.setAttribute("name", num+"_element_middle");
	        middle.setAttribute("onChange", "change_value('"+i+"_element_middle')");
			
	var middle_label = document.createElement('label');
		middle_label.setAttribute("class", "mini_label");
		middle_label.innerHTML="<!--repstart-->Middle<!--repend-->";
		
    	first_input = document.getElementById(num+'_td_name_input_first');
    	last_input = document.getElementById(num+'_td_name_input_last');
    	first_label = document.getElementById(num+'_td_name_label_first');
    	last_label = document.getElementById(num+'_td_name_label_last');
	
      	td_name_input1.appendChild(title);
      	td_name_input4.appendChild(middle);
		
		tr_name1.insertBefore(td_name_input1, first_input);
		tr_name1.insertBefore(td_name_input4, null);
		
      	td_name_label1.appendChild(title_label);
      	td_name_label4.appendChild(middle_label);
		tr_name2.insertBefore(td_name_label1, first_label);
		tr_name2.insertBefore(td_name_label4, null);
		
refresh_attr(num, 'type_name');
}

function format_normal(num)
{
    	tr_name1 = document.getElementById(num+'_tr_name1');
    	tr_name2 = document.getElementById(num+'_tr_name2');
   	 	td_name_input1 = document.getElementById(num+'_td_name_input_title');
		
   		td_name_input4 = document.getElementById(num+'_td_name_input_middle');
		
   		td_name_label1 = document.getElementById(num+'_td_name_label_title');
		
   	 	td_name_label4 =document.getElementById(num+'_td_name_label_middle');
		
		tr_name1.removeChild(td_name_input1);
		tr_name1.removeChild(td_name_input4);
		tr_name2.removeChild(td_name_label1);
		tr_name2.removeChild(td_name_label4);
}

function type_editor(i, w_editor){

    document.getElementById("element_type").value="type_editor";
	delete_last_child();
// edit table	
	oElement=document.getElementById('table_editor');
	var iReturnTop = 0;
	var iReturnLeft = 0;
	while( oElement != null ) 
	{
	iReturnTop += oElement.offsetTop;
	iReturnLeft += oElement.offsetLeft;
	oElement = oElement.offsetParent;
	}
	
		document.getElementById('main_editor').style.display="block";
		document.getElementById('main_editor').style.left=iReturnLeft+195+"px";
		document.getElementById('main_editor').style.top=iReturnTop+70+"px";
		
		
		
		if(document.getElementById('editor').style.display=="none")
		{
			ifr_id=document.getElementsByTagName("iframe")[0].id;
			ifr=getIFrameDocument(ifr_id);
			ifr.body.innerHTML=w_editor;
		}
		else
		{
			document.getElementById('editor').value=w_editor;
		}
		
			element='div';
	
		
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");		
      	var main_td  = document.getElementById('show_table');
      	main_td.appendChild(div);
		
     	var div = document.createElement('div');
      	    div.style.width="550px";				
		document.getElementById('edit_table').appendChild(div);
		
		
}

function type_submit_reset(i, w_submit_title , w_reset_title , w_class, w_act, w_attr_name, w_attr_value){

    document.getElementById("element_type").value="type_submit_reset";
    
	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");

	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
	
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		  
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_submit_title_label = document.createElement('label');
	                el_submit_title_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_submit_title_label.innerHTML = "Submit button label";
	
	var el_submit_title_textarea = document.createElement('input');
                el_submit_title_textarea.setAttribute("id", "edit_for_title");
                el_submit_title_textarea.setAttribute("type", "text");
                el_submit_title_textarea.style.cssText = "margin-left: 16px; width:160px";
                el_submit_title_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_submit', this.value)");
		el_submit_title_textarea.value = w_submit_title;
	var el_submit_func_label = document.createElement('label');
	                el_submit_func_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_submit_func_label.innerHTML = "Submit function";
	var el_submit_func_textarea = document.createElement('input');
                el_submit_func_textarea.setAttribute("type", "text");
                el_submit_func_textarea.setAttribute("disabled", "disabled");
                el_submit_func_textarea.style.cssText = "margin-left: 33px; width:160px";
		el_submit_func_textarea.value = "check_required('submit')";

	var el_reset_title_label = document.createElement('label');
	                el_reset_title_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_reset_title_label.innerHTML = "Reset button label";
	
	var el_reset_title_textarea = document.createElement('input');
                el_reset_title_textarea.setAttribute("id", "edit_for_title");
                el_reset_title_textarea.setAttribute("type", "text");
                el_reset_title_textarea.style.cssText = "margin-left: 25px; width:160px";
                el_reset_title_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_reset', this.value)");
		el_reset_title_textarea.value = w_reset_title;
	
	
	var el_reset_active = document.createElement('input');
                el_reset_active.setAttribute("type", "checkbox");
                el_reset_active.style.cssText = "";
				el_reset_active.setAttribute("onClick", "active_reset(this.checked, "+i+")");
	if(w_act)
				el_reset_active.setAttribute("checked", "checked");

				
	var el_reset_active_label = document.createElement('label');
	                el_reset_active_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
					el_reset_active_label.innerHTML = "Display Reset button";
	
	
	
	
	var el_reset_func_label = document.createElement('label');
	                el_reset_func_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_reset_func_label.innerHTML = "Reset function";
	var el_reset_func_textarea = document.createElement('input');
                el_reset_func_textarea.setAttribute("type", "text");
                el_reset_func_textarea.setAttribute("disabled", "disabled");
                el_reset_func_textarea.style.cssText = "margin-left: 42px; width:160px";
		el_reset_func_textarea.value = "check_required('reset')";

	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_submit_reset')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_submit_reset')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_submit_reset')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_submit_reset')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var hr = document.createElement('hr');
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	edit_main_td1.appendChild(el_submit_title_label);
	edit_main_td1.appendChild(el_submit_title_textarea);
	edit_main_td1.appendChild(br1);
	edit_main_td1.appendChild(el_submit_func_label);
	edit_main_td1.appendChild(el_submit_func_textarea);
	
	
	edit_main_td2.appendChild(el_reset_active);
	edit_main_td2.appendChild(el_reset_active_label);
	edit_main_td2.appendChild(br5);
	edit_main_td2.appendChild(el_reset_title_label);
	edit_main_td2.appendChild(el_reset_title_textarea);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_reset_func_label);
	edit_main_td2.appendChild(el_reset_func_textarea);

	edit_main_td3.appendChild(el_style_label);
	edit_main_td3.appendChild(el_style_textarea);
	
	edit_main_td4.appendChild(el_attr_label);
	edit_main_td4.appendChild(el_attr_add);
	edit_main_td4.appendChild(br3);
	edit_main_td4.appendChild(el_attr_table);

	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);

	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='button';	type1='button';   	type2='button'; 
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_submit_reset");
            adding_type.setAttribute("name", i+"_type");
	    
	var adding_submit = document.createElement(element);
		    adding_submit.setAttribute("type", type1);
		
			adding_submit.setAttribute("class", "button_submit");
			
			adding_submit.setAttribute("id", i+"_element_submit");
			adding_submit.setAttribute("value", w_submit_title);
			adding_submit.innerHTML=w_submit_title;
			adding_submit.setAttribute("onClick", "check_required('submit');");

	var adding_reset = document.createElement(element);
		    adding_reset.setAttribute("type", type2);
		
			adding_reset.setAttribute("class", "button_reset");
			if(!w_act)
				adding_reset.style.display="none";
				
			adding_reset.setAttribute("id", i+"_element_reset");
			adding_reset.setAttribute("value", w_reset_title );
			adding_reset.setAttribute("onClick", "check_required('reset');");
			adding_reset.innerHTML=w_reset_title;

     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
					
      	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'middle');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
		td1.style.cssText = 'display:none';
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
			
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      

	    
      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.style.cssText = 'display:none';
			label.innerHTML = "type_submit_reset_"+i;
      	var main_td  = document.getElementById('show_table');
      
      	td1.appendChild(label);
      	td2.appendChild(adding_type);
      	td2.appendChild(adding_submit);
      	td2.appendChild(adding_reset);
	
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
change_class(w_class, i);
refresh_attr(i, 'type_submit_reset');
}

function type_hidden(i, w_name, w_value, w_attr_name, w_attr_value){

    document.getElementById("element_type").value="type_hidden";
    
	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");

	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
	
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		  
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
	var el_field_id_label = document.createElement('label');
	                el_field_id_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_field_id_label.innerHTML = "Field Id";
	
	var el_field_id_input= document.createElement('input');
                el_field_id_input.setAttribute("type", "text");
                el_field_id_input.setAttribute("disabled", "disabled");
                el_field_id_input.setAttribute("value", i+"_element");
                el_field_id_input.style.cssText = "margin-left: 41px; width:160px";

	var el_field_name_label = document.createElement('label');
	                el_field_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_field_name_label.innerHTML = "Field Name";
	
	var el_field_name_input= document.createElement('input');
                el_field_name_input.setAttribute("type", "text");
		
                el_field_name_input.setAttribute("value", w_name);
                el_field_name_input.style.cssText = "margin-left: 16px; width:160px";
                el_field_name_input.setAttribute("onKeyUp", "change_field_name('"+i+"', this)");

	var el_field_value_label = document.createElement('label');
	                el_field_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_field_value_label.innerHTML = "Field Value";
	
	var el_field_value_input= document.createElement('input');
                el_field_value_input.setAttribute("type", "text");
                el_field_value_input.setAttribute("value", w_value);
                el_field_value_input.style.cssText = "margin-left: 16px; width:160px";
                el_field_value_input.setAttribute("onKeyUp", "change_field_value('"+i+"', this.value)");

	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_text')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_text')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_text')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_text')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var hr = document.createElement('hr');
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	edit_main_td1.appendChild(el_field_id_label);
	edit_main_td1.appendChild(el_field_id_input);
	edit_main_td2.appendChild(el_field_name_label);
	edit_main_td2.appendChild(el_field_name_input);
	edit_main_td3.appendChild(el_field_value_label);
	edit_main_td3.appendChild(el_field_value_input);
	edit_main_td4.appendChild(el_attr_label);
	edit_main_td4.appendChild(el_attr_add);
	edit_main_td4.appendChild(br3);
	edit_main_td4.appendChild(el_attr_table);
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);

	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='input';	type='hidden';  
	
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_hidden");
            adding_type.setAttribute("name", i+"_type");
	    
	var adding = document.createElement(element);
            adding.setAttribute("type", type);
            adding.setAttribute("value", w_value);
            adding.setAttribute("id", i+"_element");
            adding.setAttribute("name", w_name);

     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
					
      	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
           	table.setAttribute("cellpadding", '0');
           	table.setAttribute("cellspacing", '0');
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'middle');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
		td1.style.cssText = 'display:none';
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
			
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      

	    
      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.style.cssText = 'display:none';
			label.innerHTML = w_name;
      	var main_td  = document.getElementById('show_table');
      
      	td1.appendChild(label);
      	td2.appendChild(adding);
      	td2.appendChild(adding_type);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
refresh_attr(i, 'type_text');
}

function type_button(i, w_title , w_func , w_class, w_attr_name, w_attr_value){
	document.getElementById("element_type").value="type_button";
	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.setAttribute("id", "buttons");
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		  

	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
 		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");
	var el_choices_add_label = document.createElement('label');
				el_choices_add_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
				el_choices_add_label.innerHTML = "<br />Add a new button";
	var el_choices_add = document.createElement('img');
                el_choices_add.setAttribute("id", "el_choices_add");
           	el_choices_add.setAttribute("src", main_location+'/images/add.png');
            	el_choices_add.style.cssText = 'cursor:pointer; margin-left:90px';
            	el_choices_add.setAttribute("title", 'add');
                el_choices_add.setAttribute("onClick", "add_button("+i+")");
	
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_checkbox')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_checkbox')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_checkbox')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_checkbox')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	
	edit_main_td1.appendChild(el_style_label);
	edit_main_td1.appendChild(el_style_textarea);
	edit_main_td2.appendChild(el_attr_label);
	edit_main_td2.appendChild(el_attr_add);
	edit_main_td2.appendChild(br3);
	edit_main_td2.appendChild(el_attr_table);
	
	edit_main_td3.appendChild(el_choices_add_label);
	edit_main_td3.appendChild(el_choices_add);
	
	n=w_title.length;
	for(j=0; j<n; j++)
	{	
		var table_button = document.createElement('table');
			table_button.setAttribute("width", "100%");
			table_button.setAttribute("border", "0");
			table_button.setAttribute("id", "button_opt"+j);
			table_button.setAttribute("idi", j+1);
		var tr_button = document.createElement('tr');
		var tr_hr = document.createElement('tr');
		
		var td_button = document.createElement('td');
		var td_X = document.createElement('td');
		var td_hr = document.createElement('td');
		    td_hr.setAttribute("colspan", "3");
		tr_hr.appendChild(td_hr);
		tr_button.appendChild(td_button);
		tr_button.appendChild(td_X);
		table_button.appendChild(tr_hr);
		table_button.appendChild(tr_button);
		
		var br1 = document.createElement('br');
		
		var hr = document.createElement('hr');
		hr.setAttribute("id", "br"+j);


		var el_title_label = document.createElement('label');
	
			el_title_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
	
			el_title_label.innerHTML = "Button name";
		
		var el_title = document.createElement('input');
			el_title.setAttribute("id", "el_title"+j);
			el_title.setAttribute("type", "text");
			el_title.setAttribute("value", w_title[j]);
			el_title.style.cssText =   "width:100px;  margin-left:43px;  padding:0; border-width: 1px";
			el_title.setAttribute("onKeyUp", "change_label('"+i+"_element"+j+"', this.value);");
	
		var el_func_label = document.createElement('label');
	
			el_func_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
	
			el_func_label.innerHTML = "OnClick function";
		
		var el_func = document.createElement('input');
			el_func.setAttribute("id", "el_func"+j);
			el_func.setAttribute("type", "text");
			el_func.setAttribute("value", w_func[j]);
			el_func.style.cssText =   "width:100px;  margin-left:20px;  padding:0; border-width: 1px";
			el_func.setAttribute("onKeyUp", "change_func('"+i+"_element"+j+"', this.value);");
		var el_choices_remove = document.createElement('img');
			el_choices_remove.setAttribute("id", "el_button"+j+"_remove");
			el_choices_remove.setAttribute("src", main_location+'/images/delete.png');
			el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_choices_remove.setAttribute("align", 'top');
			el_choices_remove.setAttribute("onClick", "remove_button("+j+","+i+")");
			
		td_hr.appendChild(hr);
		td_button.appendChild(el_title_label);
		td_button.appendChild(el_title);
		td_button.appendChild(br1);
		td_button.appendChild(el_func_label);
		td_button.appendChild(el_func);
		td_X.appendChild(el_choices_remove);
		edit_main_td4.appendChild(table_button);
	
	}

	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr5.appendChild(edit_main_td5);

	edit_main_table.appendChild(edit_main_tr1);

	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);

	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='button';	type='button'; 
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_button");
            adding_type.setAttribute("name", i+"_type");
    var div = document.createElement('div');
       	div.setAttribute("id", "main_div");
//tbody sarqac
		
		
	var table = document.createElement('table');
		table.setAttribute("id", i+"_elemet_table");
	
    var tr = document.createElement('tr');
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'middle');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
		td1.style.cssText = 'display:none';
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'top');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");

      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
	//	table_little -@ sarqaca tbody table_little darela table_little_t
      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.innerHTML = "button_"+i;
			label.style.cssText = 'display:none';
	    
	n=w_title.length;
	for(j=0; j<n; j++)
	{      	
	
		var adding = document.createElement(element);
				adding.setAttribute("type", type);
				adding.setAttribute("id", i+"_element"+j);
				adding.setAttribute("name", i+"_element"+j);
				adding.setAttribute("value", w_title[j]);
				adding.innerHTML = w_title[j];
				adding.setAttribute("onclick", w_func[j]);
				
				
		td2.appendChild(adding);
	}			
      	var main_td  = document.getElementById('show_table');
	
      	td1.appendChild(label);
      
        td2.appendChild(adding_type);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      

      	div.appendChild(table);
      	div.appendChild(br1);
      	main_td.appendChild(div);
change_class(w_class, i);
refresh_attr(i, 'type_checkbox');
}

function type_text(i, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_class, w_attr_name, w_attr_value) {

 	element_ids=[ 'option1', 'option2'];
    document.getElementById("element_type").value="type_text";
    
	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");

	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
	
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
	                el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
		el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
	        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";
		el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
	Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');

                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
		el_label_position2.style.cssText = "margin-left:15px";
                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
	Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");

	var el_size_label = document.createElement('label');
	        el_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_size_label.innerHTML = "Field size(px) ";
	var el_size = document.createElement('input');
		   el_size.setAttribute("id", "edit_for_input_size");
		   el_size.setAttribute("type", "text");
		   el_size.setAttribute("value", w_size);
		   el_size.style.cssText ="margin-left:18px";
			el_size.setAttribute("name", "edit_for_size");
			el_size.setAttribute("onKeyPress", "return check_isnum(event)");
            el_size.setAttribute("onKeyUp", "change_w_style('"+i+"_element', this.value)");

	var el_first_value_label = document.createElement('label');
	        el_first_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_first_value_label.innerHTML = "Value if empty ";
	
	var el_first_value_input = document.createElement('input');
                el_first_value_input.setAttribute("id", "el_first_value_input");
                el_first_value_input.setAttribute("type", "text");
                el_first_value_input.setAttribute("value", w_title);
                el_first_value_input.style.cssText = "width:200px; margin-left:4px";
                el_first_value_input.setAttribute("onKeyUp", "change_input_value(this.value,'"+i+"_element')");
	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:33px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");

	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
			
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_text')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_text')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_text')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_text')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

		
	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td3.appendChild(el_size_label);
	edit_main_td3.appendChild(el_size);
	
	edit_main_td4.appendChild(el_first_value_label);
	edit_main_td4.appendChild(el_first_value_input);
	
	edit_main_td5.appendChild(el_style_label);
	edit_main_td5.appendChild(el_style_textarea);
	edit_main_td6.appendChild(el_required_label);
	edit_main_td6.appendChild(el_required);
	
	edit_main_td7.appendChild(el_attr_label);
	edit_main_td7.appendChild(el_attr_add);
	edit_main_td7.appendChild(br6);
	edit_main_td7.appendChild(el_attr_table);

	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr7.appendChild(edit_main_td7);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_main_table.appendChild(edit_main_tr7);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='input';	type='text'; 
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_text");
            adding_type.setAttribute("name", i+"_type");
	    
	var adding_required= document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
			
	var adding = document.createElement(element);
            adding.setAttribute("type", type);
		
		if(w_title==w_first_val)
		{
			adding.style.cssText = "width:"+w_size+"px;";
			adding.setAttribute("class", "input_deactive");
		}
		else
		{
			adding.style.cssText = "width:"+w_size+"px;";
			adding.setAttribute("class", "input_active");
		}
			adding.setAttribute("id", i+"_element");
			adding.setAttribute("name", i+"_element");
			adding.setAttribute("value", w_first_val);
			adding.setAttribute("title", w_title);
			adding.setAttribute("onFocus", 'delete_value("'+i+'_element")');
			adding.setAttribute("onBlur", 'return_value("'+i+'_element")');
			adding.setAttribute("onChange", 'change_value("'+i+'_element")');
			
	 
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
					
      	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'middle');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
			
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      

	    
      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.innerHTML = w_field_label;
			label.setAttribute("class", "label");
	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
      	var main_td  = document.getElementById('show_table');
      
      	td1.appendChild(label);
      	td1.appendChild(required);
      	td2.appendChild(adding_type);
      	td2.appendChild(adding_required);
      	td2.appendChild(adding);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
	if(w_field_label_pos=="top")
				label_top(i);
change_class(w_class, i);
refresh_attr(i, 'type_text');
}

function type_password(i, w_field_label, w_field_label_pos, w_size, w_required, w_class, w_attr_name, w_attr_value) {

    document.getElementById("element_type").value="type_password";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";

	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
		
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
		        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";

                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
	el_label_position2.style.cssText = "margin-left:15px";

                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
	
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");

	var el_size_label = document.createElement('label');
	        el_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_size_label.innerHTML = "Field size(px) ";
	
	var el_size = document.createElement('input');
		   el_size.setAttribute("id", "edit_for_input_size");
		   el_size.setAttribute("type", "text");
		   el_size.setAttribute("value", w_size);
		   el_size.style.cssText ="margin-left:18px";
			el_size.setAttribute("name", "edit_for_size");
			el_size.setAttribute("onKeyPress", "return check_isnum(event)");
            el_size.setAttribute("onKeyUp", "change_w_style('"+i+"_element', this.value)");

	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
		
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr('"+i+"_element')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_text')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_text')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_text')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td3.appendChild(el_size_label);
	edit_main_td3.appendChild(el_size);
	
	edit_main_td4.appendChild(el_style_label);
	edit_main_td4.appendChild(el_style_textarea);
	
	edit_main_td5.appendChild(el_required_label);
	edit_main_td5.appendChild(el_required);
	edit_main_td6.appendChild(el_attr_label);
	edit_main_td6.appendChild(el_attr_add);
	edit_main_td6.appendChild(br3);
	edit_main_td6.appendChild(el_attr_table);

	
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='input';	type='password'; 
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_password");
            adding_type.setAttribute("name", i+"_type");
	var adding_required= document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");

	var adding = document.createElement(element);
            adding.setAttribute("type", type);
		adding.setAttribute("id", i+"_element");
	
		adding.setAttribute("name", i+"_element");
	
		adding.style.cssText = "width:"+w_size+"px; border-width:1px; margin: 0px; padding: 0px";
		
			
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
		
	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'middle');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");

      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      
	    
      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.innerHTML = w_field_label;
			label.setAttribute("class", "label");
	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
      	var main_td  = document.getElementById('show_table');
	
      
      	td1.appendChild(label);
      	td1.appendChild(required);
      	td2.appendChild(adding_type);
      	td2.appendChild(adding_required);
      	td2.appendChild(adding);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
		
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
	if(w_field_label_pos=="top")
				label_top(i);
change_class(w_class, i);
refresh_attr(i, 'type_text');
}

function type_textarea(i, w_field_label, w_field_label_pos, w_size_w, w_size_h, w_first_val, w_title, w_required, w_class, w_attr_name, w_attr_value){
    
	document.getElementById("element_type").value="type_textarea";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");
	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";

	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
			        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";

                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
	el_label_position2.style.cssText = "margin-left:15px";

                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");

	var el_size_label = document.createElement('label');
	        el_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_size_label.innerHTML = "Field size(px) ";
		
	var el_size_w = document.createElement('input');
		   el_size_w.setAttribute("id", "edit_for_input_size");
		   el_size_w.setAttribute("type", "text");
		   el_size_w.setAttribute("value", w_size_w);
		   el_size_w.style.cssText = "margin-left:14px; margin-right:2px; width: 60px";
		   el_size_w.setAttribute("name", "edit_for_size");
		   el_size_w.setAttribute("onKeyPress", "return check_isnum(event)");
           el_size_w.setAttribute("onKeyUp", "change_w_style('"+i+"_element', this.value)");
		   
		X = document.createTextNode("x");
		
	var el_size_h = document.createElement('input');
		   el_size_h.setAttribute("id", "edit_for_input_size");
		   el_size_h.setAttribute("type", "text");
		   el_size_h.setAttribute("value", w_size_h);
		   el_size_h.style.cssText = "margin-left:2px;  width:60px";
			el_size_h.setAttribute("name", "edit_for_size");
			el_size_h.setAttribute("onKeyPress", "return check_isnum(event)");
            el_size_h.setAttribute("onKeyUp", "change_h_style('"+i+"_element', this.value)");
	var el_first_value_label = document.createElement('label');
	        el_first_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_first_value_label.innerHTML = "Value if empty";
	
	var el_first_value_input = document.createElement('input');
                el_first_value_input.setAttribute("id", "el_first_value_input");
                el_first_value_input.setAttribute("type", "text");
                el_first_value_input.setAttribute("value", w_title);
                el_first_value_input.style.cssText = "width:200px; margin-top:5px";
                el_first_value_input.setAttribute("onKeyUp", "change_input_value(this.value,'"+i+"_element')");
	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");

	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
			
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr('"+i+"_element')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_text')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_text')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_text')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td3.appendChild(el_size_label);
	
	edit_main_td3.appendChild(el_size_w);
	edit_main_td3.appendChild(X);
	edit_main_td3.appendChild(el_size_h);
	
	edit_main_td4.appendChild(el_first_value_label);
//	edit_main_td4.appendChild(br3);
	edit_main_td4.appendChild(el_first_value_input);
	edit_main_td5.appendChild(el_style_label);
	edit_main_td5.appendChild(el_style_textarea);
	
	edit_main_td6.appendChild(el_required_label);
	edit_main_td6.appendChild(el_required);
	
	edit_main_td7.appendChild(el_attr_label);
	edit_main_td7.appendChild(el_attr_add);
	edit_main_td7.appendChild(br6);
	edit_main_td7.appendChild(el_attr_table);

	
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr7.appendChild(edit_main_td7);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_main_table.appendChild(edit_main_tr7);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='textarea';
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_textarea");
            adding_type.setAttribute("name", i+"_type");
	var adding_required= document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
			
		var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'top');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'top');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
			
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      
      	var label = document.createElement('span');
		label.setAttribute("id", i+"_element_label");
		label.innerHTML = w_field_label;
		label.setAttribute("class", "label");	
	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
	var adding = document.createElement(element);
		if(w_title==w_first_val)
		{
			adding.style.cssText = "width:"+w_size_w+"px; height:"+w_size_h+"px;";
			adding.setAttribute("class", "input_deactive");
		}
		else
		{
			adding.style.cssText = "width:"+w_size_w+"px; height:"+w_size_h+"px;";
			adding.setAttribute("class", "input_active");
		}
		adding.setAttribute("id", i+"_element");
		adding.setAttribute("name", i+"_element");
		adding.setAttribute("title", w_title);
		adding.setAttribute("value",w_first_val);
		adding.setAttribute("onFocus", "delete_value('"+i+"_element')");
		adding.setAttribute("onBlur", "return_value('"+i+"_element')");
		adding.setAttribute("onChange", "change_value('"+i+"_element')");
		adding.innerHTML=w_first_val;
		
			
		var main_td  = document.getElementById('show_table');
	
      
      	td1.appendChild(label);
      	td1.appendChild(required);
      	td2.appendChild(adding_type);
	
      	td2.appendChild(adding_required);
      	td2.appendChild(adding);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
	if(w_field_label_pos=="top")
				label_top(i);
change_class(w_class, i);
refresh_attr(i, 'type_text');
}

function type_name(i, w_field_label, w_field_label_pos, w_size, w_name_format, w_required, w_class, w_attr_name, w_attr_value) {
	document.getElementById("element_type").value="type_name";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");
		
	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		  
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
			        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";

                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
	el_label_position2.style.cssText = "margin-left:15px";

                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
	
				el_label_position2.setAttribute("checked", "checked");
	else

				el_label_position1.setAttribute("checked", "checked");

	var el_size_label = document.createElement('label');
	        el_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_size_label.innerHTML = "Field size(px) ";
	var el_size = document.createElement('input');
		   el_size.setAttribute("id", "edit_for_input_size");
		   el_size.setAttribute("type", "text");
		   el_size.setAttribute("value", w_size);
		   el_size.style.cssText ="margin-left:18px";
			el_size.setAttribute("name", "edit_for_size");
			el_size.setAttribute("onKeyPress", "return check_isnum(event)");
            el_size.setAttribute("onKeyUp", "change_w_style('"+i+"_element_first', this.value); change_w_style('"+i+"_element_last', this.value); change_w_style('"+i+"_element_middle', this.value)");


	var el_format_label = document.createElement('label');
	        el_format_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_format_label.innerHTML = "Name Format";
	
	var el_format_normal = document.createElement('input');
                el_format_normal.setAttribute("id", "el_format_normal");
                el_format_normal.setAttribute("type", "radio");
                el_format_normal.setAttribute("value", "normal");
       		el_format_normal.style.cssText = "margin-left:15px";
		el_format_normal.setAttribute("name", "edit_for_name_format");
                el_format_normal.setAttribute("onchange", "format_normal("+i+")");
		el_format_normal.setAttribute("checked", "checked");
		Normal = document.createTextNode("Normal");
		
	var el_format_extended = document.createElement('input');
                el_format_extended.setAttribute("id", "el_format_extended");
                el_format_extended.setAttribute("type", "radio");
                el_format_extended.setAttribute("value", "extended");
                el_format_extended.style.cssText = "margin-left:15px";
		el_format_extended.setAttribute("name", "edit_for_name_format");
                el_format_extended.setAttribute("onchange", "format_extended("+i+")");
		Extended = document.createTextNode("Extended");
		
	if(w_name_format=="normal")
	
				el_format_normal.setAttribute("checked", "checked");
	else
				el_format_extended.setAttribute("checked", "checked");

	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");	
		
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");

	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_name')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_name')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_name')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_name')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

		
	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td7.appendChild(el_size_label);
	edit_main_td7.appendChild(el_size);
	
	edit_main_td3.appendChild(el_format_label);
	edit_main_td3.appendChild(br5);
	edit_main_td3.appendChild(el_format_normal);
	edit_main_td3.appendChild(Normal);
	edit_main_td3.appendChild(br6);
	edit_main_td3.appendChild(el_format_extended);
	edit_main_td3.appendChild(Extended);
	
	edit_main_td4.appendChild(el_style_label);
	edit_main_td4.appendChild(el_style_textarea);
	
	edit_main_td5.appendChild(el_required_label);
	edit_main_td5.appendChild(el_required);
		
	edit_main_td6.appendChild(el_attr_label);
	edit_main_td6.appendChild(el_attr_add);
	edit_main_td6.appendChild(br3);
	edit_main_td6.appendChild(el_attr_table);
	
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr7.appendChild(edit_main_td7);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr7);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_name");
            adding_type.setAttribute("name", i+"_type");
	var adding_required= document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
	    
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
					
      	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");

			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'top');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'top');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
			
      	var table_name = document.createElement('table');
           	table_name.setAttribute("id", i+"_table_name");
           	table_name.setAttribute("cellpadding", '0');
           	table_name.setAttribute("cellspacing", '0');
			
      	var tr_name1 = document.createElement('tr');
           	tr_name1.setAttribute("id", i+"_tr_name1");
			
      	var tr_name2 = document.createElement('tr');
           	tr_name2.setAttribute("id", i+"_tr_name2");
			
      	var td_name_input1 = document.createElement('td');
           	td_name_input1.setAttribute("id", i+"_td_name_input_first");
			
      	var td_name_input2 = document.createElement('td');
           	td_name_input2.setAttribute("id", i+"_td_name_input_last");
		
      	var td_name_label1 = document.createElement('td');
           	td_name_label1.setAttribute("id", i+"_td_name_label_first");
           	td_name_label1.setAttribute("align", "left");
			
      	var td_name_label2 = document.createElement('td');
           	td_name_label2.setAttribute("id", i+"_td_name_label_last");
           	td_name_label2.setAttribute("align", "left");
			
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      
	    
      	var label = document.createElement('span');
		label.setAttribute("id", i+"_element_label");
		label.innerHTML = w_field_label;
		label.setAttribute("class", "label");
	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";			
	var first = document.createElement('input');
            first.setAttribute("type", 'text');
	    first.style.cssText = "border-width:1px; margin: 0px 10px 0px 0px; padding: 0px; width:"+w_size+"px";
	    first.setAttribute("id", i+"_element_first");
	    first.setAttribute("name", i+"_element_first");
	    first.setAttribute("onChange", "change_value('"+i+"_element_first')");
			
	var first_label = document.createElement('label');
	    first_label.setAttribute("class", "mini_label");
	    first_label.innerHTML="<!--repstart-->First<!--repend-->";
			
	var last = document.createElement('input');
            	last.setAttribute("type", 'text');
		last.style.cssText = "border-width:1px; margin: 0px 10px 0px 0px; padding: 0px; width:"+w_size+"px";
		last.setAttribute("id", i+"_element_last");
	   	last.setAttribute("name", i+"_element_last");
		last.setAttribute("onChange", "change_value('"+i+"_element_last')");

	var last_label = document.createElement('label');
		last_label.setAttribute("class", "mini_label");
		last_label.innerHTML="<!--repstart-->Last<!--repend-->";
			
      	var main_td  = document.getElementById('show_table');
      
      	td1.appendChild(label);
      	td1.appendChild(required );
		
      	td_name_input1.appendChild(first);
      	td_name_input2.appendChild(last);
      	tr_name1.appendChild(td_name_input1);
      	tr_name1.appendChild(td_name_input2);
		
      	td_name_label1.appendChild(first_label);
      	td_name_label2.appendChild(last_label);
      	tr_name2.appendChild(td_name_label1);
      	tr_name2.appendChild(td_name_label2);
      	table_name.appendChild(tr_name1);
      	table_name.appendChild(tr_name2);
		
       	td2.appendChild(adding_type);
       	td2.appendChild(adding_required);
     	td2.appendChild(table_name);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
		
	if(w_field_label_pos=="top")
				label_top(i);
	
	if(w_name_format=="extended")
				format_extended(i);
change_class(w_class, i);
refresh_attr(i, 'type_name');
}

function type_submitter_mail(i, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_send, w_required, w_class, w_attr_name, w_attr_value){
    document.getElementById("element_type").value="type_submitter_mail";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");
	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_tr8  = document.createElement('tr');
      		edit_main_tr8.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
	
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
	var edit_main_td8 = document.createElement('td');
		edit_main_td8.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
	                el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
		el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
	        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";
		el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
	Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
		el_label_position2.style.cssText = "margin-left:15px";
                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
	Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");

	var el_size_label = document.createElement('label');
	        el_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_size_label.innerHTML = "Field size(px) ";
	var el_size = document.createElement('input');
		   el_size.setAttribute("id", "edit_for_input_size");
		   el_size.setAttribute("type", "text");
		   el_size.setAttribute("value", w_size);
		   el_size.style.cssText ="margin-left:18px";
			el_size.setAttribute("name", "edit_for_size");
			el_size.setAttribute("onKeyPress", "return check_isnum(event)");
            el_size.setAttribute("onKeyUp", "change_w_style('"+i+"_element', this.value)");

	var el_first_value_label = document.createElement('label');
	        el_first_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_first_value_label.innerHTML = "Value if empty";
	
	var el_first_value_input = document.createElement('input');
                el_first_value_input.setAttribute("id", "el_first_value_input");
                el_first_value_input.setAttribute("type", "text");
                el_first_value_input.setAttribute("value", w_title);
                el_first_value_input.style.cssText = "width:200px; margin-top:5px";
                el_first_value_input.setAttribute("onKeyUp", "change_input_value(this.value,'"+i+"_element')");
				
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
 		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");
	
	
	var el_send_label = document.createElement('label');
	        el_send_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_send_label.innerHTML = "Send mail to submitter ";
	
	var el_send = document.createElement('input');
                el_send.setAttribute("id", "el_send");
                el_send.setAttribute("type", "checkbox");
                el_send.setAttribute("value", "yes");
                el_send.setAttribute("onclick", "set_send('"+i+"_send')");
	if(w_send=="yes")
			
                el_send.setAttribute("checked", "checked");
	
	
	
	
	
	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");

	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_text')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_text')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_text')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_text')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td3.appendChild(el_size_label);
	edit_main_td3.appendChild(el_size);
	
	edit_main_td4.appendChild(el_first_value_label);
	edit_main_td4.appendChild(br3);
	edit_main_td4.appendChild(el_first_value_input);

	edit_main_td5.appendChild(el_style_label);
	edit_main_td5.appendChild(el_style_textarea);
	
	edit_main_td6.appendChild(el_send_label);
	edit_main_td6.appendChild(el_send);
	
	edit_main_td7.appendChild(el_required_label);
	edit_main_td7.appendChild(el_required);
	edit_main_td8.appendChild(el_attr_label);
	edit_main_td8.appendChild(el_attr_add);
	edit_main_td8.appendChild(br4);
	edit_main_td8.appendChild(el_attr_table);
	
/*	edit_main_td5.appendChild(el_guidelines_label);
	edit_main_td5.appendChild(br4);
	edit_main_td5.appendChild(el_guidelines_textarea);
*/	
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr7.appendChild(edit_main_td7);
	edit_main_tr8.appendChild(edit_main_td8);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	
	edit_main_table.appendChild(edit_main_tr6);
	edit_main_table.appendChild(edit_main_tr7);
	edit_main_table.appendChild(edit_main_tr8);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='input';	type='text'; 
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_submitter_mail");
            adding_type.setAttribute("name", i+"_type");
	    
	var adding_required = document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
	    
	var adding_send = document.createElement("input");
            adding_send.setAttribute("type", "hidden");
            adding_send.setAttribute("value", w_send);
            adding_send.setAttribute("name", i+"_send");
			
            adding_send.setAttribute("id", i+"_send");
			
	var adding = document.createElement(element);
            adding.setAttribute("type", type);
		
		
		if(w_title==w_first_val)
		{
			adding.style.cssText = "width:"+w_size+"px;";
			adding.setAttribute("class", "input_deactive");
		}
		else
		{
			adding.style.cssText = "width:"+w_size+"px;";
			adding.setAttribute("class", "input_active");
		}
			adding.setAttribute("id", i+"_element");
			adding.setAttribute("name", i+"_element");
			adding.setAttribute("value", w_first_val);
			adding.setAttribute("title", w_title);
			
			adding.setAttribute("onFocus", "delete_value('"+i+"_element')");
			adding.setAttribute("onBlur", "return_value('"+i+"_element')");
			adding.setAttribute("onChange", "change_value('"+i+"_element')");
			

     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
					
      	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'middle');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
			
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      

	    
      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.innerHTML = w_field_label;
			label.setAttribute("class", "label");
	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
      	var main_td  = document.getElementById('show_table');
      
      	td1.appendChild(label);
      	td1.appendChild(required);
      	td2.appendChild(adding_type);
	
      	td2.appendChild(adding_required);
		
      	td2.appendChild(adding_send);
      	td2.appendChild(adding);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
	if(w_field_label_pos=="top")
				label_top(i);
change_class(w_class, i);
refresh_attr(i, 'type_text');
}

function type_checkbox(i, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_class, w_attr_name, w_attr_value) {

	document.getElementById("element_type").value="type_checkbox";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");
	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";

		
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
      	edit_main_td4.setAttribute("id", "choices");
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
			        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";
                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
			el_label_position2.style.cssText = "margin-left:15px";
                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
	
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");
	var el_label_flow = document.createElement('label');
			        el_label_flow.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_flow.innerHTML = "Relative Position";
	
	var el_flow_vertical = document.createElement('input');
                el_flow_vertical.setAttribute("id", "edit_for_flow_vertical");
                el_flow_vertical.setAttribute("type", "radio");
                el_flow_vertical.setAttribute("value", "ver");
                el_flow_vertical.style.cssText = "margin-left:15px";
                el_flow_vertical.setAttribute("name", "edit_for_flow");
                el_flow_vertical.setAttribute("onchange", "flow_ver("+i+")");
		Vertical = document.createTextNode("Vertical");
		
	var el_flow_horizontal = document.createElement('input');
                el_flow_horizontal.setAttribute("id", "edit_for_flow_horizontal");
                el_flow_horizontal.setAttribute("type", "radio");
                el_flow_horizontal.setAttribute("value", "hor");
				el_flow_horizontal.style.cssText = "margin-left:15px";
                el_flow_horizontal.setAttribute("name", "edit_for_flow");
                el_flow_horizontal.setAttribute("onchange", "flow_hor("+i+")");
		Horizontal = document.createTextNode("Horizontal");
		
	if(w_flow=="hor")
				el_flow_horizontal.setAttribute("checked", "checked");
	else
				el_flow_vertical.setAttribute("checked", "checked");
				
				
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
 		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");

	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
		
		
		
	
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_checkbox')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_checkbox')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_checkbox')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_checkbox')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var el_choices_label = document.createElement('label');
			        el_choices_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_choices_label.innerHTML = "Options ";
	var el_choices_add = document.createElement('img');
                el_choices_add.setAttribute("id", "el_choices_add");
           		el_choices_add.setAttribute("src", main_location+'/images/add.png');
            	el_choices_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_choices_add.setAttribute("title", 'add');
                el_choices_add.setAttribute("onClick", "add_choise('checkbox',"+i+")");
	
	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);

	edit_main_td3.appendChild(el_label_flow);
	edit_main_td3.appendChild(br3);
	edit_main_td3.appendChild(el_flow_vertical);
	edit_main_td3.appendChild(Vertical);
	edit_main_td3.appendChild(br4);
	edit_main_td3.appendChild(el_flow_horizontal);
	edit_main_td3.appendChild(Horizontal);
	
	edit_main_td5.appendChild(el_required_label);
	edit_main_td5.appendChild(el_required);
	
	edit_main_td6.appendChild(el_style_label);
	edit_main_td6.appendChild(el_style_textarea);
	
	edit_main_td7.appendChild(el_attr_label);
	edit_main_td7.appendChild(el_attr_add);
	edit_main_td7.appendChild(br6);
	edit_main_td7.appendChild(el_attr_table);
	
	edit_main_td4.appendChild(el_choices_label);
	edit_main_td4.appendChild(el_choices_add);
	
	n=w_choices.length;
	for(j=0; j<n; j++)
	{	
		var br = document.createElement('br');
		br.setAttribute("id", "br"+j);
		var el_choices = document.createElement('input');
			el_choices.setAttribute("id", "el_choices"+j);
			el_choices.setAttribute("type", "text");
			el_choices.setAttribute("value", w_choices[j]);
			el_choices.style.cssText =   "width:100px; margin:0; padding:0; border-width: 1px";
			el_choices.setAttribute("onKeyUp", "change_label('"+i+"_label_element"+j+"', this.value); change_in_value('"+i+"_element"+j+"', this.value)");
	
		var el_choices_remove = document.createElement('img');
			el_choices_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_choices_remove.setAttribute("src", main_location+'/images/delete.png');
			el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_choices_remove.setAttribute("align", 'top');
			el_choices_remove.setAttribute("onClick", "remove_choise("+j+","+i+")");
			
		edit_main_td4.appendChild(br);
		edit_main_td4.appendChild(el_choices);
		edit_main_td4.appendChild(el_choices_remove);
	
	}

	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr7.appendChild(edit_main_td7);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	
	edit_main_table.appendChild(edit_main_tr6);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr7);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='input';	type='checkbox'; 
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");

            adding_type.setAttribute("value", "type_checkbox");
            adding_type.setAttribute("name", i+"_type");
	var adding_required = document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
    var div = document.createElement('div');
       	div.setAttribute("id", "main_div");
//tbody sarqac
		
		
	var table = document.createElement('table');
		table.setAttribute("id", i+"_elemet_table");
	
    var tr = document.createElement('tr');
			
    var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'top');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'top');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");

      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
	//	table_little -@ sarqaca tbody table_little darela table_little_t
	var table_little_t = document.createElement('table');
			
	var table_little = document.createElement('tbody');
           	table_little.setAttribute("id", i+"_table_little");
	table_little_t.appendChild(table_little);
	

	    
      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.innerHTML = w_field_label;
			label.setAttribute("class", "label");

	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
	n=w_choices.length;
	for(j=0; j<n; j++)
	{      	
		var tr_little = document.createElement('tr');
			tr_little.setAttribute("id", i+"_element_tr"+j);
				
		var td_little = document.createElement('td');
			td_little.setAttribute("valign", 'top');
			td_little.setAttribute("id", i+"_td_little"+j);
			td_little.setAttribute("idi", j);
	
		var adding = document.createElement(element);
				adding.setAttribute("type", type);
				adding.setAttribute("id", i+"_element"+j);
				adding.setAttribute("name", i+"_element"+j);
				adding.setAttribute("value", w_choices[j]);
				adding.setAttribute("onclick", "set_checked('"+i+"_element"+j+"')");
		if(w_choices_checked[j]=='1')
				adding.setAttribute("checked", "checked");
				
				
				
		var label_adding = document.createElement('label');
				label_adding.setAttribute("id", i+"_label_element"+j);
				label_adding.setAttribute("class","ch_rad_label");
				label_adding.innerHTML = w_choices[j];
				
		td_little.appendChild(adding);
		td_little.appendChild(label_adding);
		tr_little.appendChild(td_little);
		table_little.appendChild(tr_little);
	
	}			
      	var main_td  = document.getElementById('show_table');
	
      
      	td1.appendChild(label);
      	td1.appendChild(required);
        td2.appendChild(adding_type);
  
        td2.appendChild(adding_required);
	td2.appendChild(table_little_t);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      

      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
		
	if(w_field_label_pos=="top")
				label_top(i);
				
	if(w_flow=="hor")
				
				flow_hor(i);
change_class(w_class, i);
refresh_attr(i, 'type_checkbox');
}

function type_radio(i, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_class, w_attr_name, w_attr_value ){

	document.getElementById("element_type").value="type_radio";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");
	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";

	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
		edit_main_td4.setAttribute("id", "choices");
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
			        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";

                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
				el_label_position1.setAttribute("checked", "checked");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");

                el_label_position2.setAttribute("value", "top");
	el_label_position2.style.cssText = "margin-left:15px";

                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
	
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");
	
	var el_label_flow = document.createElement('label');
			        el_label_flow.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_flow.innerHTML = "Relative Position";

	var el_flow_vertical = document.createElement('input');
                el_flow_vertical.setAttribute("id", "edit_for_flow_vertical");
                el_flow_vertical.setAttribute("type", "radio");
                el_flow_vertical.setAttribute("value", "ver");
                el_flow_vertical.style.cssText = "margin-left:15px";
                el_flow_vertical.setAttribute("name", "edit_for_flow");
                el_flow_vertical.setAttribute("onchange", "flow_ver("+i+")");
		Vertical = document.createTextNode("Vertical");
		
	var el_flow_horizontal = document.createElement('input');
                el_flow_horizontal.setAttribute("id", "edit_for_flow_horizontal");
                el_flow_horizontal.setAttribute("type", "radio");
                el_flow_horizontal.setAttribute("value", "hor");
				el_flow_horizontal.style.cssText = "margin-left:15px";
                el_flow_horizontal.setAttribute("name", "edit_for_flow");
                el_flow_horizontal.setAttribute("onchange", "flow_hor("+i+")");
		Horizontal = document.createTextNode("Horizontal");
		
	if(w_flow=="hor")
				el_flow_horizontal.setAttribute("checked", "checked");
	else
				el_flow_vertical.setAttribute("checked", "checked");
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");
	
	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_checkbox')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_checkbox')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_checkbox')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_checkbox')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var el_choices_label = document.createElement('label');
			        el_choices_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_choices_label.innerHTML = "Options ";

	
	var el_choices_add = document.createElement('img');
                el_choices_add.setAttribute("id", "el_choices_add");
           	el_choices_add.setAttribute("src", main_location+'/images/add.png');
            	el_choices_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_choices_add.setAttribute("title", 'add');
                el_choices_add.setAttribute("onClick", "add_choise('radio',"+i+")");
				
	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	

	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td3.appendChild(el_label_flow);
	edit_main_td3.appendChild(br3);
	edit_main_td3.appendChild(el_flow_vertical);
	edit_main_td3.appendChild(Vertical);
	edit_main_td3.appendChild(br4);
	edit_main_td3.appendChild(el_flow_horizontal);
	edit_main_td3.appendChild(Horizontal);

	edit_main_td6.appendChild(el_style_label);
	edit_main_td6.appendChild(el_style_textarea);
	
	edit_main_td5.appendChild(el_required_label);
	edit_main_td5.appendChild(el_required);
	
	edit_main_td7.appendChild(el_attr_label);
	edit_main_td7.appendChild(el_attr_add);
	edit_main_td7.appendChild(br6);
	edit_main_td7.appendChild(el_attr_table);
	
	edit_main_td4.appendChild(el_choices_label);
	edit_main_td4.appendChild(el_choices_add);

	
	n=w_choices.length;
	for(j=0; j<n; j++)
	{	
		var br = document.createElement('br');
		br.setAttribute("id", "br"+j);
		var el_choices = document.createElement('input');
			el_choices.setAttribute("id", "el_choices"+j);
			el_choices.setAttribute("type", "text");
			el_choices.setAttribute("value", w_choices[j]);
			el_choices.style.cssText =   "width:100px; margin:0; padding:0; border-width: 1px";
			el_choices.setAttribute("onKeyUp", "change_label('"+i+"_label_element"+j+"', this.value); change_in_value('"+i+"_element"+j+"', this.value)");
	
		var el_choices_remove = document.createElement('img');
			el_choices_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_choices_remove.setAttribute("src", main_location+'/images/delete.png');
			el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_choices_remove.setAttribute("align", 'top');
			el_choices_remove.setAttribute("onClick", "remove_choise("+j+","+i+")");
			
		edit_main_td4.appendChild(br);
		edit_main_td4.appendChild(el_choices);
		edit_main_td4.appendChild(el_choices_remove);
	
	}


	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr7.appendChild(edit_main_td7);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	
	edit_main_table.appendChild(edit_main_tr6);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr7);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='input';	type='radio'; 
		var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_radio");
            adding_type.setAttribute("name", i+"_type");
	var adding_required = document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
	    
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
			
	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
	
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'top');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'top');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");

      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
//tbody sarqac		
		var table_little_t = document.createElement('table');
			
		var table_little = document.createElement('tbody');
           	table_little.setAttribute("id", i+"_table_little");
			
		table_little_t.appendChild(table_little);
	
      	var tr_little1 = document.createElement('tr');
	        tr_little1.setAttribute("id", i+"_element_tr1");
		
      	var tr_little2 = document.createElement('tr');
 	        tr_little2.setAttribute("id", i+"_element_tr2");
			
      	var td_little1 = document.createElement('td');
         	td_little1.setAttribute("valign", 'top');
           	td_little1.setAttribute("id", i+"_td_little1");
			
      	var td_little2 = document.createElement('td');
        	td_little2.setAttribute("valign", 'top');
           	td_little2.setAttribute("id", i+"_td_little2");
			

	    
      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.innerHTML = w_field_label;
			label.setAttribute("class", "label");
	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
	n=w_choices.length;
	for(j=0; j<n; j++)
	{      	
		var tr_little = document.createElement('tr');
			tr_little.setAttribute("id", i+"_element_tr"+j);
				
		var td_little = document.createElement('td');
			td_little.setAttribute("valign", 'top');
			td_little.setAttribute("id", i+"_td_little"+j);
			td_little.setAttribute("idi", j);
	
		var adding = document.createElement(element);
				adding.setAttribute("type", type);
				adding.setAttribute("id", i+"_element"+j);
				adding.setAttribute("name", i+"_element");
				adding.setAttribute("value", w_choices[j]);
				adding.setAttribute("onclick", "set_default('"+i+"','"+j+"')");
		if(w_choices_checked[j]=='1')
				adding.setAttribute("checked", "checked");
				
				
		var label_adding = document.createElement('label');
				label_adding.setAttribute("id", i+"_label_element"+j);
				label_adding.setAttribute("class","ch_rad_label");
				label_adding.innerHTML = w_choices[j];
				
		td_little.appendChild(adding);
		td_little.appendChild(label_adding);
		tr_little.appendChild(td_little);
		table_little.appendChild(tr_little);
	
	}			
	    
      	var main_td  = document.getElementById('show_table');
	
      
      	td1.appendChild(label);
      	td1.appendChild(required);
       	td2.appendChild(adding_type);
	
       	td2.appendChild(adding_required);
	td2.appendChild(table_little_t);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);

		if(w_field_label_pos=="top")
					label_top(i);
		
		if(w_flow=="hor")
				
				flow_hor(i);
				
change_class(w_class, i);
refresh_attr(i, 'type_checkbox');
}

function type_time(i, w_field_label, w_field_label_pos, w_time_type, w_am_pm, w_sec, w_hh, w_mm, w_ss, w_required, w_class, w_attr_name, w_attr_value) {
	
	document.getElementById("element_type").value="type_time";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");
	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
			        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";

                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
				el_label_position1.setAttribute("checked", "checked");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
		el_label_position2.style.cssText = "margin-left:15px";
                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
	
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");

	var el_label_time_type_label = document.createElement('label');
	        el_label_time_type_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_time_type_label.innerHTML = "Time Format";
	
	var el_label_time_type1 = document.createElement('input');
                el_label_time_type1.setAttribute("id", "el_label_time_type1");
                el_label_time_type1.setAttribute("type", "radio");
                el_label_time_type1.setAttribute("value", "format_24");
                el_label_time_type1.style.cssText = "margin-left:15px";
                el_label_time_type1.setAttribute("name", "edit_for_time_type");
                el_label_time_type1.setAttribute("onchange", "format_24("+i+")");
		el_label_time_type1.setAttribute("checked", "checked");
		hour_24 = document.createTextNode("24 hour");
		
	var el_label_time_type2 = document.createElement('input');
                el_label_time_type2.setAttribute("id", "el_label_time_type2");
                el_label_time_type2.setAttribute("type", "radio");
                el_label_time_type2.setAttribute("value", "format_12");
                el_label_time_type2.style.cssText = "margin-left:15px";
                el_label_time_type2.setAttribute("name", "edit_for_time_type");
                el_label_time_type2.setAttribute("onchange", "format_12("+i+", 'am','', '','')");
		am_pm = document.createTextNode("12 hour");
		
	if(w_time_type=="24")
	
				el_label_time_type1.setAttribute("checked", "checked");
	else
				el_label_time_type2.setAttribute("checked", "checked");

	var el_label_second_label = document.createElement('label');
	        el_label_second_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_second_label.innerHTML = "Display Seconds";
	
	var el_second_yes = document.createElement('input');
                el_second_yes.setAttribute("id", "el_second_yes");
                el_second_yes.setAttribute("type", "radio");
                el_second_yes.setAttribute("value", "yes");
                el_second_yes.style.cssText = "margin-left:15px";
                el_second_yes.setAttribute("name", "edit_for_time_second");
                el_second_yes.setAttribute("onchange", "second_yes("+i+",'"+w_ss+"')");
		el_second_yes.setAttribute("checked", "checked");
		display_seconds = document.createTextNode("Yes");
		
	var el_second_no = document.createElement('input');
                el_second_no.setAttribute("id", "el_second_no");
                el_second_no.setAttribute("type", "radio");
                el_second_no.setAttribute("value", "no");
                el_second_no.style.cssText = "margin-left:15px";
                el_second_no.setAttribute("name", "edit_for_time_second");
                el_second_no.setAttribute("onchange", "second_no("+i+")");
		dont_display_seconds = document.createTextNode("No");
		
	if(w_sec=="1")
	
				el_second_yes.setAttribute("checked", "checked");
	else
				el_second_no.setAttribute("checked", "checked");
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");

	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
			
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_time')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_time')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_time')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_time')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td3.appendChild(el_label_time_type_label);
	edit_main_td3.appendChild(br5);
	edit_main_td3.appendChild(el_label_time_type1);
	edit_main_td3.appendChild(hour_24);
	edit_main_td3.appendChild(br6);
	edit_main_td3.appendChild(el_label_time_type2);
	edit_main_td3.appendChild(am_pm);
	
	edit_main_td4.appendChild(el_label_second_label);
	edit_main_td4.appendChild(br3);
	edit_main_td4.appendChild(el_second_yes);
	edit_main_td4.appendChild(display_seconds);
	edit_main_td4.appendChild(br4);
	edit_main_td4.appendChild(el_second_no);
	edit_main_td4.appendChild(dont_display_seconds);
	
	edit_main_td5.appendChild(el_style_label);
	edit_main_td5.appendChild(el_style_textarea);
	
	edit_main_td6.appendChild(el_required_label);
	edit_main_td6.appendChild(el_required);

	
	edit_main_td7.appendChild(el_attr_label);
	edit_main_td7.appendChild(el_attr_add);
	edit_main_td7.appendChild(br6);
	edit_main_td7.appendChild(el_attr_table);
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr7.appendChild(edit_main_td7);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_main_table.appendChild(edit_main_tr7);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_time");
            adding_type.setAttribute("name", i+"_type");
	var adding_required = document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
					
      	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'top');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
			
      	var table_time = document.createElement('table');
           	table_time.setAttribute("id", i+"_table_time");
           	table_time.setAttribute("cellpadding", '0');
           	table_time.setAttribute("cellspacing", '0');
      	var tr_time1 = document.createElement('tr');
           	tr_time1.setAttribute("id", i+"_tr_time1");
			
      	var tr_time2 = document.createElement('tr');
           	tr_time2.setAttribute("id", i+"_tr_time2");
			
      	var td_time_input1 = document.createElement('td');
           	td_time_input1.setAttribute("id", i+"_td_time_input1");
	        td_time_input1.style.cssText ="width:32px";
		
      	var td_time_input1_ket = document.createElement('td');
           	td_time_input1_ket.setAttribute("align", "center");
		
		
		
      	var td_time_input2 = document.createElement('td');
           	td_time_input2.setAttribute("id", i+"_td_time_input2");
 	        td_time_input2.style.cssText ="width:32px";
     	var td_time_input2_ket = document.createElement('td');
           	td_time_input2_ket.setAttribute("align", "center");
		
      	var td_time_input3 = document.createElement('td');
           	td_time_input3.setAttribute("id", i+"_td_time_input3");
 	        td_time_input3.style.cssText ="width:32px";

      	var td_time_label1 = document.createElement('td');
           	td_time_label1.setAttribute("id", i+"_td_time_label1");
      	var td_time_label1_ket = document.createElement('td');
      	var td_time_label2 = document.createElement('td');
           	td_time_label2.setAttribute("id", i+"_td_time_label2");
      	var td_time_label2_ket = document.createElement('td');
      	var td_time_label3 = document.createElement('td');
           	td_time_label3.setAttribute("id", i+"_td_time_label3");
			
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      

      	var label = document.createElement('span');
		label.setAttribute("id", i+"_element_label");
		label.innerHTML = w_field_label;
		label.setAttribute("class", "label");
	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
			
	var hh = document.createElement('input');
	hh.setAttribute("type", 'text');
	hh.setAttribute("value", w_hh);
	    hh.setAttribute("class", "time_box");
	    hh.setAttribute("id", i+"_hh");
	    hh.setAttribute("name", i+"_hh");
	    hh.setAttribute("onKeyPress", "return check_hour(event, '"+i+"_hh', '23')");
	    hh.setAttribute("onKeyUp", "change_hour(event, '"+i+"_hh','23')");
	    hh.setAttribute("onBlur", "add_0('"+i+"_hh')");
			
	var hh_label = document.createElement('label');
	    hh_label.setAttribute("class", "mini_label");
	    hh_label.innerHTML="HH";
			
	var hh_ = document.createElement('span');
	    hh_.style.cssText = "font-style:bold; vertical-align:middle";
	    hh_.innerHTML="&nbsp;:&nbsp;";
		
	var mm = document.createElement('input');
            	mm.setAttribute("type", 'text');
		mm.setAttribute("value", w_mm);
		mm.setAttribute("class", "time_box");
		
		mm.setAttribute("id", i+"_mm");
	    	mm.setAttribute("name", i+"_mm");
		mm.setAttribute("onKeyPress", "return check_minute(event, '"+i+"_mm')");
	        mm.setAttribute("onKeyUp", "change_minute(event, '"+i+"_mm')");
		mm.setAttribute("onBlur", "add_0('"+i+"_mm')");
		
	var mm_label = document.createElement('label');
		mm_label.setAttribute("class", "mini_label");
		mm_label.innerHTML="MM";
			
	var mm_ = document.createElement('span');
		mm_.style.cssText = "font-style:bold; vertical-align:middle";
		mm_.innerHTML="&nbsp;:&nbsp;";
		
	var ss = document.createElement('input');
           	ss.setAttribute("type", 'text');
		ss.setAttribute("value", w_ss);
		ss.setAttribute("class", "time_box");
		
		ss.setAttribute("id", i+"_ss");
		ss.setAttribute("name", i+"_ss");
		ss.setAttribute("onKeyPress", "return check_second(event, '"+i+"_ss')");
		ss.setAttribute("onKeyUp", "change_second('"+i+"_ss')");
		ss.setAttribute("onBlur", "add_0('"+i+"_ss')");

	var ss_label = document.createElement('label');
		ss_label.setAttribute("class", "mini_label");
		ss_label.innerHTML="SS";
			
      	var main_td  = document.getElementById('show_table');
	
      
      	td1.appendChild(label);
      	td1.appendChild(required);
		
      	td_time_input1.appendChild(hh);
      	td_time_input1_ket.appendChild(hh_);
      	td_time_input2.appendChild(mm);
      	td_time_input2_ket.appendChild(mm_);
      	td_time_input3.appendChild(ss);
      	tr_time1.appendChild(td_time_input1);
      	tr_time1.appendChild(td_time_input1_ket);
      	tr_time1.appendChild(td_time_input2);
      	tr_time1.appendChild(td_time_input2_ket);
      	tr_time1.appendChild(td_time_input3);
		
      	td_time_label1.appendChild(hh_label);
      	td_time_label2.appendChild(mm_label);
      	td_time_label3.appendChild(ss_label);
      	tr_time2.appendChild(td_time_label1);
      	tr_time2.appendChild(td_time_label1_ket);
      	tr_time2.appendChild(td_time_label2);
      	tr_time2.appendChild(td_time_label2_ket);
      	tr_time2.appendChild(td_time_label3);
      	table_time.appendChild(tr_time1);
      	table_time.appendChild(tr_time2);
		
        td2.appendChild(adding_type);
	
        td2.appendChild(adding_required);
	td2.appendChild(table_time);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);

	if(w_field_label_pos=="top")
				label_top(i);
	if(w_time_type=="12")
				format_12(i, w_am_pm,w_hh, w_mm,w_ss);

	if(w_sec=="0")
				second_no(i);
change_class(w_class, i);
refresh_attr(i, 'type_time');
}

function type_date(i, w_field_label, w_field_label_pos, w_date, w_required, w_class, w_format, w_but_val, w_attr_name, w_attr_value) { 

	document.getElementById("element_type").value="type_date";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");

	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
			        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";

                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
				el_label_position1.setAttribute("checked", "checked");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
	el_label_position2.style.cssText = "margin-left:15px";

                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
	
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");

	var el_format_label = document.createElement('label');
	        el_format_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_format_label.innerHTML = "Date format";
	
	var el_format_textarea = document.createElement('input');
                el_format_textarea.setAttribute("id", "date_format");
		el_format_textarea.setAttribute("type", "text");
		el_format_textarea.setAttribute("value", w_format);
                el_format_textarea.style.cssText = "width:200px; margin-left:20px";
                el_format_textarea.setAttribute("onChange", "change_date_format(this.value,'"+i+"')");

	var el_button_value_label = document.createElement('label');
	        el_button_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_button_value_label.innerHTML = "Date Picker label";
	
	var el_button_value_textarea = document.createElement('input');
                el_button_value_textarea.setAttribute("id", "button_value");
		el_button_value_textarea.setAttribute("type", "text");
		el_button_value_textarea.setAttribute("value", w_but_val);
                el_button_value_textarea.style.cssText = "width:150px; margin-left:20px";
                el_button_value_textarea.setAttribute("onKeyUp", "change_file_value(this.value,'"+i+"_button')");

	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");

	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
		
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_date')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_date')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_date')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_date')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);

	edit_main_td3.appendChild(el_format_label);
	edit_main_td3.appendChild(el_format_textarea);
	
	edit_main_td4.appendChild(el_button_value_label);
	edit_main_td4.appendChild(el_button_value_textarea);
	
	edit_main_td5.appendChild(el_style_label);
	edit_main_td5.appendChild(el_style_textarea);
	
	edit_main_td6.appendChild(el_required_label);
	edit_main_td6.appendChild(el_required);
	
	edit_main_td7.appendChild(el_attr_label);
	edit_main_td7.appendChild(el_attr_add);
	edit_main_td7.appendChild(br3);
	edit_main_td7.appendChild(el_attr_table);
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr7.appendChild(edit_main_td7);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_main_table.appendChild(edit_main_tr7);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_date");
            adding_type.setAttribute("name", i+"_type");
	var adding_required = document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
			
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
					
      	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'middle');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
			
      	var table_date = document.createElement('table');
           	table_date.setAttribute("id", i+"_table_date");
           	table_date.setAttribute("cellpadding", '0');
           	table_date.setAttribute("cellspacing", '0');
			
      	var tr_date1 = document.createElement('tr');
           	tr_date1.setAttribute("id", i+"_tr_date1");
			
      	var tr_date2 = document.createElement('tr');
           	tr_date2.setAttribute("id", i+"_tr_date2");
			
      	var td_date_input1 = document.createElement('td');
           	td_date_input1.setAttribute("id", i+"_td_date_input1");
			
      	var td_date_input2 = document.createElement('td');
           	td_date_input2.setAttribute("id", i+"_td_date_input2");
			
      	var td_date_input3 = document.createElement('td');
           	td_date_input3.setAttribute("id", i+"_td_date_input3");

      	var td_date_label1 = document.createElement('td');
           	td_date_label1.setAttribute("id", i+"_td_date_label1");
			
      	var td_date_label2 = document.createElement('td');
           	td_date_label2.setAttribute("id", i+"_td_date_label2");
			
      	var td_date_label3 = document.createElement('td');
           	td_date_label3.setAttribute("id", i+"_td_date_label3");
			
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      

      	var label = document.createElement('span');
		label.setAttribute("id", i+"_element_label");
		label.innerHTML = w_field_label;
		label.setAttribute("class", "label");
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
			
	var adding = document.createElement('input');
            adding.setAttribute("type", 'text');
            adding.setAttribute("value", w_date);
            adding.setAttribute("class", 'inputbox');
			adding.setAttribute("id", i+"_element");
			adding.setAttribute("name", i+"_element");
			adding.setAttribute("maxlength", "10");
			adding.setAttribute("size", "10");
			adding.setAttribute("onChange", "change_value('"+i+"_element')");
		
	var adding_button = document.createElement('input');
 	    adding_button.setAttribute("id", i+"_button");
            adding_button.setAttribute("type", 'reset');
            adding_button.setAttribute("value", w_but_val);
            adding_button.setAttribute("format", w_format);
            adding_button.setAttribute("onclick", "return showCalendar('"+i+"_element' ,'"+w_format+"')");
            adding_button.setAttribute("class", 'button');
	    
      	var main_td  = document.getElementById('show_table');
      
      	td1.appendChild(label);
      	td1.appendChild(required);
		
      	td2.appendChild(adding_type);
	
      	td2.appendChild(adding_required);
		td2.appendChild(adding);
		td2.appendChild(adding_button);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);

	if(w_field_label_pos=="top")
				label_top(i);
change_class(w_class, i);
refresh_attr(i, 'type_date');
}

function field_to_select(id, type)
{

	switch(type)
	{
		case 'day': 
		{
			w_width=document.getElementById('edit_for_day_size').value!=''?document.getElementById('edit_for_day_size').value:30;
			w_day=document.getElementById(id+"_day").value;
			document.getElementById(id+"_td_date_input1").innerHTML='';	
			
			var select_day  = document.createElement('select');
				select_day.setAttribute("id", id+'_day');
				select_day.setAttribute("name", id+'_day');
				select_day.setAttribute("onChange", 'set_select(this)');
				select_day.style.width=w_width+'px';
				
			var options  = document.createElement('option');
				options.setAttribute("value",'');
				options.innerHTML= '';
				select_day.appendChild(options);
				
			for(k=1; k<=31;k++)
			{
				if(k<10)
					k='0'+k;
				var options  = document.createElement('option');
					options.setAttribute("value", k);
					options.innerHTML= k;
				 if (k==w_day) 
					options.setAttribute("selected", "selected");

					select_day.appendChild(options);
					
			}

			document.getElementById(id+"_td_date_input1").appendChild(select_day);

			break;	
		}
		case 'month': 
		{ 
			w_width=document.getElementById('edit_for_month_size').value!=''?document.getElementById('edit_for_month_size').value:60;
			w_month=document.getElementById(id+"_month").value;
			
			document.getElementById(id+"_td_date_input2").innerHTML='';
			
		var select_month = document.createElement('select');
				select_month.setAttribute("id", id+'_month');
				select_month.setAttribute("name", id+'_month');
				select_month.setAttribute("onChange", 'set_select(this)');
				select_month.style.width=w_width+'px';
				
		var options  = document.createElement('option');
				options.setAttribute("value",'');
				options.innerHTML= '';
				select_month.appendChild(options);
				
		var myMonths=new Array("<!--repstart-->January<!--repend-->","<!--repstart-->February<!--repend-->","<!--repstart-->March<!--repend-->","<!--repstart-->April<!--repend-->","<!--repstart-->May<!--repend-->","<!--repstart-->June<!--repend-->","<!--repstart-->July<!--repend-->","<!--repstart-->August<!--repend-->","<!--repstart-->September<!--repend-->","<!--repstart-->October<!--repend-->","<!--repstart-->November<!--repend-->","<!--repstart-->December<!--repend-->");
			for(k=1; k<=12;k++)
			{
				if(k<10)
					k='0'+k;
				var options  = document.createElement('option');
					options.setAttribute("value", k);
					options.innerHTML= myMonths[k-1];
				if (k==w_month) 
					options.setAttribute("selected", "selected");

					select_month.appendChild(options);
					
			}
			document.getElementById(id+"_td_date_input2").appendChild(select_month);
		break;	
		}	
		case 'year':
		{ 
			w_width=document.getElementById('edit_for_year_size').value!=''?document.getElementById('edit_for_year_size').value:60;
			w_year=document.getElementById(id+"_year").value;
			
		document.getElementById(id+"_td_date_input3").innerHTML=''; 
		var select_year  = document.createElement('select');
			select_year.setAttribute("id", id+'_year');
			select_year.setAttribute("name", id+'_year');
			select_year.setAttribute("onChange", 'set_select(this)');
			select_year.style.width=w_width+'px';
			
		var options  = document.createElement('option');
			options.setAttribute("value",'');
			options.innerHTML= '';
			select_year.appendChild(options);
			
		from= parseInt(document.getElementById("edit_for_year_interval_from").value);
		to= parseInt(document.getElementById("edit_for_year_interval_to").value);
		for(k=to; k>=from;k--)
		{
		var options  = document.createElement('option');
			options.setAttribute("value", k);
			options.innerHTML= k;
		if (k==w_year) 
			options.setAttribute("selected", "selected");
			
			select_year.appendChild(options);
		}
		select_year.value=w_year;
		select_year.setAttribute('from',from);
		select_year.setAttribute('to',to);
		document.getElementById(id+"_td_date_input3").appendChild(select_year);
		
		break;
		}
	}
	
refresh_attr(id, 'type_date_fields');

}

function field_to_text(id, type)
{

	switch(type)
	{
		case 'day': 
		{	
			w_width=document.getElementById('edit_for_day_size').value!=''?document.getElementById('edit_for_day_size').value:30;
			w_day=document.getElementById(id+"_day").value;
			document.getElementById(id+"_td_date_input1").innerHTML='';	

			var day = document.createElement('input');
			day.setAttribute("type", 'text');
			day.setAttribute("value", w_day);
			//day.setAttribute("class", "time_box");
			day.setAttribute("id", id+"_day");
			day.setAttribute("name", id+"_day");
			day.setAttribute("onChange", "change_value('"+ id+"_day')");
			day.setAttribute("onKeyPress", "return check_day(event, '"+id+"_day')");
		    day.setAttribute("onBlur", "if (this.value=='0') this.value=''; else add_0('"+id+"_day')");
		
			day.style.width=w_width+'px';

			document.getElementById(id+"_td_date_input1").appendChild(day);
			break;	
		}
		case 'month': 
		{ 
			w_width=document.getElementById('edit_for_month_size').value!=''?document.getElementById('edit_for_month_size').value:60;
			w_month=document.getElementById(id+"_month").value;
			
			document.getElementById(id+"_td_date_input2").innerHTML='';
			
		var month = document.createElement('input');
			month.setAttribute("type", 'text');
			month.setAttribute("value", w_month);
			//month.setAttribute("class", "time_box");
			month.setAttribute("id", id+"_month");
			month.setAttribute("name", id+"_month");
			month.style.width=w_width+'px';
			month.setAttribute("onKeyPress", "return check_month(event, '"+id+"_month')");
			month.setAttribute("onChange", "change_value('"+id+"_month')");
			month.setAttribute("onBlur", "if (this.value=='0') this.value=''; else add_0('"+id+"_month')");
			/*month.setAttribute("onKeyPress", "return check_minute(event, '"+i+"_mm')");
			month.setAttribute("onKeyUp", "change_minute(event, '"+i+"_mm')");
			month.setAttribute("onBlur", "add_0('"+i+"_mm')");*/

			document.getElementById(id+"_td_date_input2").appendChild(month);
			break;	
		}	
		case 'year':
		{ 
			w_width=document.getElementById('edit_for_year_size').value!=''?document.getElementById('edit_for_year_size').value:60;
			w_year=document.getElementById(id+"_year").value;
			
			document.getElementById(id+"_td_date_input3").innerHTML='';
			
			from= parseInt(document.getElementById("edit_for_year_interval_from").value);
			to= parseInt(document.getElementById("edit_for_year_interval_to").value);
			if((parseInt(w_year)<from) || (parseInt(w_year)>to))
				w_year='';
			var year = document.createElement('input');
			year.setAttribute("type", 'text');
			year.setAttribute("value", w_year);
			//year.setAttribute("class", "time_box");
			year.setAttribute("id", id+"_year");
			year.setAttribute("name", id+"_year");
			year.setAttribute("onChange", "change_year('"+id+"_year')");
			year.setAttribute("onKeyPress", "return check_year1(event, '"+id+"_year')");
			year.setAttribute("onBlur", "check_year2('"+id+"_year')");
			year.style.width=w_width+'px';
			year.setAttribute('from',from);
			year.setAttribute('to',to);

			document.getElementById(id+"_td_date_input3").appendChild(year);

			break;
		}
	}
	refresh_attr(id, 'type_date_fields');


}
function set_divider(id, divider)
{
	document.getElementById(id+"_separator1").innerHTML=divider;
	document.getElementById(id+"_separator2").innerHTML=divider;
}

function year_interval(id)
{
	from= parseInt(document.getElementById("edit_for_year_interval_from").value);
	to= parseInt(document.getElementById("edit_for_year_interval_to").value);
	if(to-from<0)
	{	
		alert('Invalid interval of years.');
		document.getElementById("edit_for_year_interval_from").value=to;
	}
	else
	{
		if(document.getElementById(id+"_year").tagName=='SELECT')
			field_to_select(id, 'year');
		else
			field_to_text(id, 'year');
	}
}

function type_date_fields(i, w_field_label, w_field_label_pos, w_day, w_month, w_year, w_day_type, w_month_type, w_year_type, w_day_label, w_month_label, w_year_label, w_day_size, w_month_size, w_year_size, w_required, w_class, w_from, w_to, w_divider, w_attr_name, w_attr_value) { 

	document.getElementById("element_type").value="type_date_fields";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr8  = document.createElement('tr');
      		edit_main_tr8.setAttribute("valing", "top");
			
	var edit_main_tr9  = document.createElement('tr');
      		edit_main_tr9.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");

	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";

	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td8 = document.createElement('td');
		edit_main_td8.style.cssText = "padding-top:10px";
		
	var edit_main_td9 = document.createElement('td');
		edit_main_td9.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
			        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";

                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
				el_label_position1.setAttribute("checked", "checked");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
				el_label_position2.style.cssText = "margin-left:15px";
                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
	
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");


	var el_fields_divider_label = document.createElement('label');
		el_fields_divider_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_fields_divider_label.innerHTML = "Fields separator";
	
	var el_fields_divider = document.createElement('input');
        el_fields_divider.setAttribute("id", "edit_for_fields_divider");
        el_fields_divider.setAttribute("type", "text");
        el_fields_divider.setAttribute("value", w_divider);
        el_fields_divider.style.cssText = "margin-left:15px; width:80px";
        el_fields_divider.setAttribute("onKeyUp", "set_divider('"+i+"', this.value)");
			
/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// D A Y //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
		
	var el_day_field_type_label = document.createElement('label');
				el_day_field_type_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
				el_day_field_type_label.innerHTML = "Day field type";
	
	var el_day_field_type_input1 = document.createElement('input');
                el_day_field_type_input1.setAttribute("id", "el_day_field_type_text");
                el_day_field_type_input1.setAttribute("type", "radio");
                el_day_field_type_input1.setAttribute("value", "text");
                el_day_field_type_input1.style.cssText = "margin-left:33px";
                el_day_field_type_input1.setAttribute("name", "edit_for_day_field_type");
                el_day_field_type_input1.setAttribute("onchange", "field_to_text("+i+", 'day')");
		Text_1 = document.createTextNode("Input");
		
	var el_day_field_type_input2 = document.createElement('input');
                el_day_field_type_input2.setAttribute("id", "el_day_field_type_select");
                el_day_field_type_input2.setAttribute("type", "radio");
                el_day_field_type_input2.setAttribute("value", "select");
				el_day_field_type_input2.style.cssText = "margin-left:15px";
                el_day_field_type_input2.setAttribute("name", "edit_for_day_field_type");
                el_day_field_type_input2.setAttribute("onchange", "field_to_select("+i+", 'day')");
		Select_1= document.createTextNode("Select");
		
	if(w_day_type=="SELECT")
				el_day_field_type_input2.setAttribute("checked", "checked");
	else
				el_day_field_type_input1.setAttribute("checked", "checked");
		
	var el_day_field_size_label = document.createElement('label');
	    el_day_field_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_day_field_size_label.innerHTML = "Day field size(px)";
	
	var el_day_field_size_input = document.createElement('input');
        el_day_field_size_input.setAttribute("id", "edit_for_day_size");
		el_day_field_size_input.setAttribute("type", "text");
		el_day_field_size_input.setAttribute("value", w_day_size);
		el_day_field_size_input.style.cssText ="margin-left:11px";
		el_day_field_size_input.setAttribute("onKeyPress", "return check_isnum(event)");
		el_day_field_size_input.setAttribute("onKeyUp", "change_w_style('"+i+"_day', this.value)");
       //el_day_field_size_input.setAttribute("onKeyUp", "change_w_style('"+i+"_element', this.value)");
				
	var el_day_field_text_label = document.createElement('label');
	    el_day_field_text_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_day_field_text_label.innerHTML = "Day label";
	
	var el_day_field_text_input = document.createElement('input');
        el_day_field_text_input.setAttribute("id", "edit_for_day_text");
		el_day_field_text_input.setAttribute("type", "text");
		el_day_field_text_input.setAttribute("value", w_day_label);
		el_day_field_text_input.style.cssText ="margin-left:60px";
		//el_day_field_size_input.setAttribute("onKeyPress", "return check_isnum(event)");
        el_day_field_text_input.setAttribute("onKeyUp", "change_w_label('"+i+"_day_label', this.value)");
				
				
/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// M O N T H //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
		
	var el_month_field_type_label = document.createElement('label');
				el_month_field_type_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
				el_month_field_type_label.innerHTML = "Month field type";
	
	var el_month_field_type_input1 = document.createElement('input');
                el_month_field_type_input1.setAttribute("id", "el_month_field_type_text");
                el_month_field_type_input1.setAttribute("type", "radio");
                el_month_field_type_input1.setAttribute("value", "text");
                el_month_field_type_input1.style.cssText = "margin-left:33px";
                el_month_field_type_input1.setAttribute("name", "edit_for_month_field_type");
                el_month_field_type_input1.setAttribute("onchange", "field_to_text("+i+", 'month')");
		Text_2 = document.createTextNode("Input");
		
	var el_month_field_type_input2 = document.createElement('input');
                el_month_field_type_input2.setAttribute("id", "el_month_field_type_select");
                el_month_field_type_input2.setAttribute("type", "radio");
                el_month_field_type_input2.setAttribute("value", "select");
				el_month_field_type_input2.style.cssText = "margin-left:15px";
                el_month_field_type_input2.setAttribute("name", "edit_for_month_field_type");
                el_month_field_type_input2.setAttribute("onchange", "field_to_select("+i+", 'month')");
		Select_2 = document.createTextNode("Select");
		
	if(w_month_type=="SELECT")
				el_month_field_type_input2.setAttribute("checked", "checked");
	else
				el_month_field_type_input1.setAttribute("checked", "checked");
		
	var el_month_field_size_label = document.createElement('label');
	    el_month_field_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_month_field_size_label.innerHTML = "Month field size(px)";
	
	var el_month_field_size_input = document.createElement('input');
        el_month_field_size_input.setAttribute("id", "edit_for_month_size");
		el_month_field_size_input.setAttribute("type", "text");
		el_month_field_size_input.setAttribute("value", w_month_size);
		el_month_field_size_input.style.cssText ="margin-left:11px";
		el_month_field_size_input.setAttribute("onKeyPress", "return check_isnum(event)");
		el_month_field_size_input.setAttribute("onKeyUp", "change_w_style('"+i+"_month', this.value)");
				
	var el_month_field_text_label = document.createElement('label');
	    el_month_field_text_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_month_field_text_label.innerHTML = "Month label";
	
	var el_month_field_text_input = document.createElement('input');
        el_month_field_text_input.setAttribute("id", "edit_for_month_text");
		el_month_field_text_input.setAttribute("type", "text");
		el_month_field_text_input.setAttribute("value", w_month_label);
		el_month_field_text_input.style.cssText ="margin-left:60px";
		//el_month_field_size_input.setAttribute("onKeyPress", "return check_isnum(event)");
        el_month_field_text_input.setAttribute("onKeyUp", "change_w_label('"+i+"_month_label', this.value)");
				
	


/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  Y E A R  //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
		
	var el_year_field_type_label = document.createElement('label');
				el_year_field_type_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
				el_year_field_type_label.innerHTML = "Year field type";
	
	var el_year_field_type_input1 = document.createElement('input');
                el_year_field_type_input1.setAttribute("id", "el_year_field_type_text");
                el_year_field_type_input1.setAttribute("type", "radio");
                el_year_field_type_input1.setAttribute("value", "text");
                el_year_field_type_input1.style.cssText = "margin-left:33px";
                el_year_field_type_input1.setAttribute("name", "edit_for_year_field_type");
                el_year_field_type_input1.setAttribute("onchange", "field_to_text("+i+", 'year')");
		Text_3 = document.createTextNode("Input");
		
	var el_year_field_type_input2 = document.createElement('input');
                el_year_field_type_input2.setAttribute("id", "el_year_field_type_select");
                el_year_field_type_input2.setAttribute("type", "radio");
                el_year_field_type_input2.setAttribute("value", "select");
				el_year_field_type_input2.style.cssText = "margin-left:15px";
                el_year_field_type_input2.setAttribute("name", "edit_for_year_field_type");
                el_year_field_type_input2.setAttribute("onchange", "field_to_select("+i+", 'year')");
		Select_3 = document.createTextNode("Select");
		
	if(w_year_type=="SELECT")
				el_year_field_type_input2.setAttribute("checked", "checked");
	else
				el_year_field_type_input1.setAttribute("checked", "checked");
		
	var el_year_field_interval_label = document.createElement('label');
	    el_year_field_interval_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_year_field_interval_label.innerHTML = "Year interval";
	
	var el_year_field_interval_from_input = document.createElement('input');
        el_year_field_interval_from_input.setAttribute("id", "edit_for_year_interval_from");
		el_year_field_interval_from_input.setAttribute("type", "text");
		el_year_field_interval_from_input.setAttribute("value", w_from);
		el_year_field_interval_from_input.style.cssText ="margin-left:44px; width:40px";
		el_year_field_interval_from_input.setAttribute("onKeyPress", "return check_isnum(event)");
        el_year_field_interval_from_input.setAttribute("onChange", "year_interval("+i+")");
		
	Line = document.createTextNode(" - ");
			
	var el_year_field_interval_to_input = document.createElement('input');
        el_year_field_interval_to_input.setAttribute("id", "edit_for_year_interval_to");
		el_year_field_interval_to_input.setAttribute("type", "text");
		el_year_field_interval_to_input.setAttribute("value", w_to);
		el_year_field_interval_to_input.style.cssText ="margin-left:0px; width:40px";
		el_year_field_interval_to_input.setAttribute("onKeyPress", "return check_isnum(event)");
        el_year_field_interval_to_input.setAttribute("onChange", "year_interval("+i+")");
				
	var el_year_field_size_label = document.createElement('label');
	    el_year_field_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_year_field_size_label.innerHTML = "Year field size(px)";
	
	var el_year_field_size_input = document.createElement('input');
        el_year_field_size_input.setAttribute("id", "edit_for_year_size");
		el_year_field_size_input.setAttribute("type", "text");
		el_year_field_size_input.setAttribute("value", w_year_size);
		el_year_field_size_input.style.cssText ="margin-left:11px";
		el_year_field_size_input.setAttribute("onKeyPress", "return check_isnum(event)");
        el_year_field_size_input.setAttribute("onKeyUp", "change_w_style('"+i+"_year', this.value)");
				
	var el_year_field_text_label = document.createElement('label');
	    el_year_field_text_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_year_field_text_label.innerHTML = "Year label";
	
	var el_year_field_text_input = document.createElement('input');
        el_year_field_text_input.setAttribute("id", "edit_for_year_text");
		el_year_field_text_input.setAttribute("type", "text");
		el_year_field_text_input.setAttribute("value", w_year_label);
		el_year_field_text_input.style.cssText ="margin-left:60px";
		//el_year_field_size_input.setAttribute("onKeyPress", "return check_isnum(event)");
        el_year_field_text_input.setAttribute("onKeyUp", "change_w_label('"+i+"_year_label', this.value)");
				
				
	
///////////////////////////////
///////////////////// End /////
			
				
				
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");

	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
		
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_date_fields')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_date_fields')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_date_fields')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_date_fields')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	var br7 = document.createElement('br');
	var br8 = document.createElement('br');
	var br9 = document.createElement('br');
	var br10 = document.createElement('br');
	var br11 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);

	edit_main_td3.appendChild(el_fields_divider_label);
	edit_main_td3.appendChild(el_fields_divider);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	
	edit_main_td4.appendChild(el_day_field_type_label);
	edit_main_td4.appendChild(el_day_field_type_input1);
	edit_main_td4.appendChild(Text_1);
	edit_main_td4.appendChild(el_day_field_type_input2);
	edit_main_td4.appendChild(Select_1);
	edit_main_td4.appendChild(br4);
	edit_main_td4.appendChild(el_day_field_size_label);
	edit_main_td4.appendChild(el_day_field_size_input);
	edit_main_td4.appendChild(br5);
	edit_main_td4.appendChild(el_day_field_text_label);
	edit_main_td4.appendChild(el_day_field_text_input);
	
	
	edit_main_td8.appendChild(el_month_field_type_label);
	edit_main_td8.appendChild(el_month_field_type_input1);
	edit_main_td8.appendChild(Text_2);
	edit_main_td8.appendChild(el_month_field_type_input2);
	edit_main_td8.appendChild(Select_2);
	edit_main_td8.appendChild(br6);
	edit_main_td8.appendChild(el_month_field_size_label);
	edit_main_td8.appendChild(el_month_field_size_input);
	edit_main_td8.appendChild(br7);
	edit_main_td8.appendChild(el_month_field_text_label);
	edit_main_td8.appendChild(el_month_field_text_input);



	edit_main_td9.appendChild(el_year_field_type_label);
	edit_main_td9.appendChild(el_year_field_type_input1);
	edit_main_td9.appendChild(Text_3);
	edit_main_td9.appendChild(el_year_field_type_input2);
	edit_main_td9.appendChild(Select_3);
	edit_main_td9.appendChild(br8);
	edit_main_td9.appendChild(el_year_field_interval_label);
	edit_main_td9.appendChild(el_year_field_interval_from_input);
	edit_main_td9.appendChild(Line);
	edit_main_td9.appendChild(el_year_field_interval_to_input);
	edit_main_td9.appendChild(br9);
	edit_main_td9.appendChild(el_year_field_size_label);
	edit_main_td9.appendChild(el_year_field_size_input);
	edit_main_td9.appendChild(br10);
	edit_main_td9.appendChild(el_year_field_text_label);
	edit_main_td9.appendChild(el_year_field_text_input);
	
	
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	edit_main_td5.appendChild(el_style_label);
	edit_main_td5.appendChild(el_style_textarea);
	
	edit_main_td6.appendChild(el_required_label);
	edit_main_td6.appendChild(el_required);
	
	edit_main_td7.appendChild(el_attr_label);
	edit_main_td7.appendChild(el_attr_add);
	edit_main_td7.appendChild(br3);
	edit_main_td7.appendChild(el_attr_table);
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr8.appendChild(edit_main_td8);
	edit_main_tr9.appendChild(edit_main_td9);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr7.appendChild(edit_main_td7);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr8);
	edit_main_table.appendChild(edit_main_tr9);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_main_table.appendChild(edit_main_tr7);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
//show table
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_date_fields");
            adding_type.setAttribute("name", i+"_type");
	var adding_required = document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
            adding_required.setAttribute("id", i+"_required");
			
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
					
      	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'top');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
			
      	var table_date = document.createElement('table');
           	table_date.setAttribute("id", i+"_table_date");
           	table_date.setAttribute("cellpadding", '0');
           	table_date.setAttribute("cellspacing", '0');
			
      	var tr_date1 = document.createElement('tr');
           	tr_date1.setAttribute("id", i+"_tr_date1");
			
      	var tr_date2 = document.createElement('tr');
           	tr_date2.setAttribute("id", i+"_tr_date2");
			
      	var td_date_input1 = document.createElement('td');
           	td_date_input1.setAttribute("id", i+"_td_date_input1");
			
      	var td_date_separator1 = document.createElement('td');
           	td_date_separator1.setAttribute("id", i+"_td_date_separator1");
			
      	var td_date_input2 = document.createElement('td');
           	td_date_input2.setAttribute("id", i+"_td_date_input2");
			
      	var td_date_separator2 = document.createElement('td');
           	td_date_separator2.setAttribute("id", i+"_td_date_separator2");
			
      	var td_date_input3 = document.createElement('td');
           	td_date_input3.setAttribute("id", i+"_td_date_input3");

      	var td_date_label1 = document.createElement('td');
           	td_date_label1.setAttribute("id", i+"_td_date_label1");
      	var td_date_label_empty1 = document.createElement('td');
			
      	var td_date_label2 = document.createElement('td');
           	td_date_label2.setAttribute("id", i+"_td_date_label2");
      	var td_date_label_empty2 = document.createElement('td');
			
      	var td_date_label3 = document.createElement('td');
           	td_date_label3.setAttribute("id", i+"_td_date_label3");
			
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      

      	var label = document.createElement('span');
		label.setAttribute("id", i+"_element_label");
		label.innerHTML = w_field_label;
		label.setAttribute("class", "label");
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";


	var day = document.createElement('input');
		day.setAttribute("type", 'text');
		day.setAttribute("value", w_day);
	    //day.setAttribute("class", "time_box");
	    day.setAttribute("id", i+"_day");
	    day.setAttribute("name", i+"_day");
		day.setAttribute("onChange", "change_value('"+i+"_day')");
		day.setAttribute("onKeyPress", "return check_day(event, '"+i+"_day')");
	    day.setAttribute("onBlur", "if (this.value=='0') this.value=''; else add_0('"+i+"_day')");
		day.style.width=w_day_size+'px';
	  

	  /* hh.setAttribute("onKeyPress", "return check_hour(event, '"+i+"_hh', '23')");
	    hh.setAttribute("onKeyUp", "change_hour(event, '"+i+"_hh','23')");
	    hh.setAttribute("onBlur", "add_0('"+i+"_hh')");*/
			
	var day_label = document.createElement('label');
	    day_label.setAttribute("class", "mini_label");
	    day_label.setAttribute("id", i+"_day_label");
	    day_label.innerHTML=w_day_label;
			
	var day_ = document.createElement('span');
	    day_.setAttribute("id", i+"_separator1");
	    day_.style.cssText = "font-style:bold; vertical-align:middle";
	    day_.innerHTML=w_divider;
		
	var month = document.createElement('input');
        month.setAttribute("type", 'text');
		month.setAttribute("value", w_month);
		//month.setAttribute("class", "time_box");
		month.setAttribute("id", i+"_month");
	    month.setAttribute("name", i+"_month");
		month.style.width=w_month_size+'px';
		month.setAttribute("onKeyPress", "return check_month(event, '"+i+"_month')");
		month.setAttribute("onChange", "change_value('"+i+"_month')");
	    month.setAttribute("onBlur", "if (this.value=='0') this.value=''; else add_0('"+i+"_month')");
		/*month.setAttribute("onKeyPress", "return check_minute(event, '"+i+"_mm')");
	    month.setAttribute("onKeyUp", "change_minute(event, '"+i+"_mm')");
		month.setAttribute("onBlur", "add_0('"+i+"_mm')");*/
		
	var month_label = document.createElement('label');
		month_label.setAttribute("class", "mini_label");
		month_label.setAttribute("class", "mini_label");
	    month_label.setAttribute("id", i+"_month_label");
		month_label.innerHTML=w_month_label;
			
	var month_ = document.createElement('span');
	    month_.setAttribute("id", i+"_separator2");
		month_.style.cssText = "font-style:bold; vertical-align:middle";
		month_.innerHTML=w_divider;
		
	var year = document.createElement('input');
        year.setAttribute("type", 'text');
        year.setAttribute("from", w_from);
        year.setAttribute("to", w_to);
		year.setAttribute("value", w_year);
		//year.setAttribute("class", "time_box");
		year.setAttribute("id", i+"_year");
		year.setAttribute("name", i+"_year");
		year.style.width=w_year_size+'px';
		year.setAttribute("onChange", "change_year('"+i+"_year')");
		year.setAttribute("onKeyPress", "return check_year1(event, '"+i+"_year')");
		year.setAttribute("onBlur", "check_year2('"+i+"_year')");
		/*year.setAttribute("onKeyPress", "return check_second(event, '"+i+"_ss')");
		year.setAttribute("onKeyUp", "change_second('"+i+"_ss')");
		year.setAttribute("onBlur", "add_0('"+i+"_ss')");*/

	var year_label = document.createElement('label');
		year_label.setAttribute("class", "mini_label");
	    year_label.setAttribute("id", i+"_year_label");
		year_label.innerHTML=w_year_label;
			
    var main_td  = document.getElementById('show_table');
      
      	td1.appendChild(label);
      	td1.appendChild(required);
		
		
		
		td_date_input1.appendChild(day);
      	td_date_separator1.appendChild(day_);
      	td_date_input2.appendChild(month);
      	td_date_separator2.appendChild(month_);
      	td_date_input3.appendChild(year);
      	tr_date1.appendChild(td_date_input1);
      	tr_date1.appendChild(td_date_separator1);
      	tr_date1.appendChild(td_date_input2);
      	tr_date1.appendChild(td_date_separator2);
      	tr_date1.appendChild(td_date_input3);
		
      	td_date_label1.appendChild(day_label);
      	td_date_label2.appendChild(month_label);
      	td_date_label3.appendChild(year_label);
      	tr_date2.appendChild(td_date_label1);
      	tr_date2.appendChild(td_date_label_empty1);
      	tr_date2.appendChild(td_date_label2);
      	tr_date2.appendChild(td_date_label_empty2);
      	tr_date2.appendChild(td_date_label3);
      	table_date.appendChild(tr_date1);
      	table_date.appendChild(tr_date2);
		
      	td2.appendChild(adding_type);
      	td2.appendChild(adding_required);
		td2.appendChild(table_date);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);

	if(w_field_label_pos=="top")
				label_top(i);
	
	if(w_day_type=="SELECT")
			field_to_select(i, 'day');
			
	if(w_month_type=="SELECT")
			field_to_select(i, 'month');
			
	if(w_year_type=="SELECT")
			field_to_select(i, 'year');
			
				
				
				
change_class(w_class, i);
refresh_attr(i, 'type_date_fields');
}

function type_own_select(i, w_field_label, w_field_label_pos, w_size, w_choices, w_choices_checked, w_required, w_class, w_attr_name, w_attr_value, w_choices_disabled){
	document.getElementById("element_type").value="type_own_select";
	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");
	var edit_main_tr7  = document.createElement('tr');
      		edit_main_tr7.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
		edit_main_td3.setAttribute("id", "choices");
		
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
	var edit_main_td7 = document.createElement('td');
		edit_main_td7.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
			        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";

                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
		el_label_position1.setAttribute("checked", "checked");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
	el_label_position2.style.cssText = "margin-left:15px";

                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
	
	if(w_field_label_pos=="top")
	
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");
	
	var el_size_label = document.createElement('label');
	        el_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_size_label.innerHTML = "Field size(px) ";
	var el_size = document.createElement('input');
		   el_size.setAttribute("id", "edit_for_input_size");
		   el_size.setAttribute("type", "text");
		   el_size.setAttribute("value", w_size);
		   el_size.style.cssText ="margin-left:18px";
			el_size.setAttribute("name", "edit_for_size");
			el_size.setAttribute("onKeyPress", "return check_isnum(event)");
            el_size.setAttribute("onKeyUp", "change_w_style('"+i+"_element', this.value)");
	
	
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");
	
	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
	
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_text')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_text')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_text')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_text')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}
		
	var el_choices_label = document.createElement('label');
	        el_choices_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_choices_label.innerHTML = "Options";
	var el_choices_add = document.createElement('img');
                el_choices_add.setAttribute("id", "el_choices_add");
           	el_choices_add.setAttribute("src", main_location+'/images/add.png');
            	el_choices_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_choices_add.setAttribute("title", 'add');
                el_choices_add.setAttribute("onClick", "add_choise('select',"+i+")");
	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
        br3.setAttribute("id", "br1");
	var br4 = document.createElement('br');
        br4.setAttribute("id", "br2");
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td6.appendChild(el_style_label);
	edit_main_td6.appendChild(el_style_textarea);
	
	edit_main_td7.appendChild(el_attr_label);
	edit_main_td7.appendChild(el_attr_add);
	edit_main_td7.appendChild(br3);
	edit_main_td7.appendChild(el_attr_table);
	edit_main_td4.appendChild(el_required_label);
	edit_main_td4.appendChild(el_required);
	
	edit_main_td5.appendChild(el_size_label);
	edit_main_td5.appendChild(el_size);
	
	edit_main_td3.appendChild(el_choices_label);
	edit_main_td3.appendChild(el_choices_add);
	
	n=w_choices.length;
	for(j=0; j<n; j++)
	{	
		var br = document.createElement('br');
		br.setAttribute("id", "br"+j);
		var el_choices = document.createElement('input');
			el_choices.setAttribute("id", "el_option"+j);
			el_choices.setAttribute("type", "text");
			el_choices.setAttribute("value", w_choices[j]);
			el_choices.style.cssText =   "width:100px; margin:0; padding:0; border-width: 1px";
			el_choices.setAttribute("onKeyUp", "change_label('"+i+"_option"+j+"', this.value)");
	
		var el_choices_remove = document.createElement('img');
			el_choices_remove.setAttribute("id", "el_option"+j+"_remove");
			el_choices_remove.setAttribute("src", main_location+'/images/delete.png');
			el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_choices_remove.setAttribute("align", 'top');
			el_choices_remove.setAttribute("onClick", "remove_option("+j+","+i+")");
			
		var el_choices_dis = document.createElement('input');
			el_choices_dis.setAttribute("type", 'checkbox');
			el_choices_dis.setAttribute("id", "el_option"+j+"_dis");
			el_choices_dis.setAttribute("onClick", "dis_option('"+i+"_option"+j+"', this.checked)");
			if(w_choices_disabled[j])
				el_choices_dis.setAttribute("checked", "checked");
			
		edit_main_td3.appendChild(br);
		edit_main_td3.appendChild(el_choices);
		edit_main_td3.appendChild(el_choices_dis);
		edit_main_td3.appendChild(el_choices_remove);
	
	}


	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr5.appendChild(edit_main_td5);
	
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr7.appendChild(edit_main_td7);
	
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_main_table.appendChild(edit_main_tr4);
	
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr7);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_own_select");
            adding_type.setAttribute("name", i+"_type");
	    
	var adding_required = document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
	    
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
			
		var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'middle');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			

      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");

      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
		
	var table_little = document.createElement('table');
           	table_little.setAttribute("id", i+"_table_little");
			
      	var tr_little1 = document.createElement('tr');
	        tr_little1.setAttribute("id", i+"_element_tr1");
		
      	var tr_little2 = document.createElement('tr');
 	        tr_little2.setAttribute("id", i+"_element_tr2");
			
      	var td_little1 = document.createElement('td');
         	td_little1.setAttribute("valign", 'top');
           	td_little1.setAttribute("id", i+"_td_little1");
			
      	var td_little2 = document.createElement('td');
        	td_little2.setAttribute("valign", 'top');
           	td_little2.setAttribute("id", i+"_td_little2");
			
   
      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.innerHTML = w_field_label;
			label.setAttribute("class", "label");
	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
	var select_ = document.createElement('select');
		select_.setAttribute("id", i+"_element");
		select_.setAttribute("name", i+"_element");
		select_.style.cssText = "width:"+w_size+"px";
		select_.setAttribute("onchange", "set_select(this)");
		
	for(j=0; j<n; j++)
	{      	
		var option = document.createElement('option');
		option.setAttribute("id", i+"_option"+j);
	if(w_choices_disabled[j])
		option.value="";
	else
		option.setAttribute("value", w_choices[j]);
		
		option.setAttribute("onselect", "set_select('"+i+"_option"+j+"')");
           	option.innerHTML = w_choices[j];
	if(w_choices_checked[j]==1)
		option.setAttribute("selected", "selected");
		select_.appendChild(option);
	}			
	
    
      	var main_td  = document.getElementById('show_table');
	
      
      	td1.appendChild(label);
      	td1.appendChild(required);
	td2.appendChild(adding_type);
	
	td2.appendChild(adding_required);
      	td2.appendChild(select_);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
	
	if(w_field_label_pos=="top")
				label_top(i);
change_class(w_class, i);
refresh_attr(i, 'type_text');
}

function dis_option(id, value)
{
	//document.getElementById(id).disabled=value;
	if(value)
		document.getElementById(id).value='';
	else
		document.getElementById(id).value=document.getElementById(id).innerHTML;
}

function type_country(i, w_field_label, w_field_label_pos, w_size, w_required, w_class, w_attr_name, w_attr_value) {
	
	document.getElementById("element_type").value="type_country";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
		
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
			        el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px;";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
				el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
			        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";

                el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
		el_label_position1.setAttribute("checked", "checked");
		Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
	el_label_position2.style.cssText = "margin-left:15px";


                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
		Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
	
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");
	
	var el_size_label = document.createElement('label');
	        el_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_size_label.innerHTML = "Field size(px) ";
	var el_size = document.createElement('input');
		el_size.setAttribute("id", "edit_for_input_size");

		el_size.setAttribute("type", "text");
		el_size.setAttribute("value", w_size);
		el_size.style.cssText ="margin-left:18px";
		el_size.setAttribute("name", "edit_for_size");
		el_size.setAttribute("onKeyPress", "return check_isnum(event)");
		el_size.setAttribute("onKeyUp", "change_w_style('"+i+"_element', this.value)");
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");
	var el_required_label = document.createElement('label');
	        el_required_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_required_label.innerHTML = "Required";
	
	var el_required = document.createElement('input');
                el_required.setAttribute("id", "el_send");
                el_required.setAttribute("type", "checkbox");
                el_required.setAttribute("value", "yes");
                el_required.setAttribute("onclick", "set_required('"+i+"_required')");
	if(w_required=="yes")
			
                el_required.setAttribute("checked", "checked");
	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_text')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_text')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_text')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_text')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}

	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
		br3.setAttribute("id", "br1");
	var br4 = document.createElement('br');
		br4.setAttribute("id", "br2");
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td3.appendChild(el_size_label);
	edit_main_td3.appendChild(el_size);
	
	edit_main_td4.appendChild(el_style_label);
	edit_main_td4.appendChild(el_style_textarea);
	edit_main_td5.appendChild(el_required_label);
	edit_main_td5.appendChild(el_required);
	edit_main_td6.appendChild(el_attr_label);
	edit_main_td6.appendChild(el_attr_add);
	edit_main_td6.appendChild(br3);
	edit_main_td6.appendChild(el_attr_table);
	
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_tr6.appendChild(edit_main_td6);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_main_table.appendChild(edit_main_tr6);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table
	var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_country");
            adding_type.setAttribute("name", i+"_type");
	var adding_required = document.createElement("input");
            adding_required.setAttribute("type", "hidden");
            adding_required.setAttribute("value", w_required);
            adding_required.setAttribute("name", i+"_required");
			
            adding_required.setAttribute("id", i+"_required");
			
     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
			
		var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'middle');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'middle');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");

      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
		
	var table_little = document.createElement('table');
           	table_little.setAttribute("id", i+"_table_little");
			
      	var tr_little1 = document.createElement('tr');
	        tr_little1.setAttribute("id", i+"_element_tr1");
		
      	var tr_little2 = document.createElement('tr');
 	        tr_little2.setAttribute("id", i+"_element_tr2");
			
      	var td_little1 = document.createElement('td');
         	td_little1.setAttribute("valign", 'top');
           	td_little1.setAttribute("id", i+"_td_little1");
			
      	var td_little2 = document.createElement('td');
        	td_little2.setAttribute("valign", 'top');
           	td_little2.setAttribute("id", i+"_td_little2");
			

      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.innerHTML = w_field_label;
			label.setAttribute("class", "label");
	    
      	var required = document.createElement('span');
			required.setAttribute("id", i+"_required_element");
			required.innerHTML = "";
			required.setAttribute("class", "required");
	if(w_required=="yes")
			required.innerHTML = "&nbsp*";
	var select_ = document.createElement('select');
		select_.setAttribute("id", i+"_element");
		select_.setAttribute("name", i+"_element");
		select_.style.cssText = "width:"+w_size+"px";
		
		var option_ = document.createElement('option');
			option_.setAttribute("value", "");
			option_.innerHTML="";
		select_.appendChild(option_);
		
		coutries=["Afghanistan","Albania",	"Algeria","Andorra","Angola","Antigua and Barbuda","Argentina","Armenia","Australia","Austria","Azerbaijan","Bahamas","Bahrain","Bangladesh","Barbados","Belarus","Belgium","Belize","Benin","Bhutan","Bolivia","Bosnia and Herzegovina","Botswana","Brazil","Brunei","Bulgaria","Burkina Faso","Burundi","Cambodia","Cameroon","Canada","Cape Verde","Central African Republic","Chad","Chile","China","Colombi","Comoros","Congo (Brazzaville)","Congo","Costa Rica","Cote d'Ivoire","Croatia","Cuba","Cyprus","Czech Republic","Denmark","Djibouti","Dominica","Dominican Republic","East Timor (Timor Timur)","Ecuador","Egypt","El Salvador","Equatorial Guinea","Eritrea","Estonia","Ethiopia","Fiji","Finland","France","Gabon","Gambia, The","Georgia","Germany","Ghana","Greece","Grenada","Guatemala","Guinea","Guinea-Bissau","Guyana","Haiti","Honduras","Hungary","Iceland","India","Indonesia","Iran","Iraq","Ireland","Israel","Italy","Jamaica","Japan","Jordan","Kazakhstan","Kenya","Kiribati","Korea, North","Korea, South","Kuwait","Kyrgyzstan","Laos","Latvia","Lebanon","Lesotho","Liberia","Libya","Liechtenstein","Lithuania","Luxembourg","Macedonia","Madagascar","Malawi","Malaysia","Maldives","Mali","Malta","Marshall Islands","Mauritania","Mauritius","Mexico","Micronesia","Moldova","Monaco","Mongolia","Morocco","Mozambique","Myanmar","Namibia","Nauru","Nepa","Netherlands","New Zealand","Nicaragua","Niger","Nigeria","Norway","Oman","Pakistan","Palau","Panama","Papua New Guinea","Paraguay","Peru","Philippines","Poland","Portugal","Qatar","Romania","Russia","Rwanda","Saint Kitts and Nevis","Saint Lucia","Saint Vincent","Samoa","San Marino","Sao Tome and Principe","Saudi Arabia","Senegal","Serbia and Montenegro","Seychelles","Sierra Leone","Singapore","Slovakia","Slovenia","Solomon Islands","Somalia","South Africa","Spain","Sri Lanka","Sudan","Suriname","Swaziland","Sweden","Switzerland","Syria","Taiwan","Tajikistan","Tanzania","Thailand","Togo","Tonga","Trinidad and Tobago","Tunisia","Turkey","Turkmenistan","Tuvalu","Uganda","Ukraine","United Arab Emirates","United Kingdom","United States","Uruguay","Uzbekistan","Vanuatu","Vatican City","Venezuela","Vietnam","Yemen","Zambia","Zimbabwe"];	
		for(r=0;r<coutries.length;r++)
		{
		var option_ = document.createElement('option');
			option_.setAttribute("value", coutries[r]);
			option_.innerHTML=coutries[r];
		select_.appendChild(option_);
		}
      	var main_td  = document.getElementById('show_table');
	
      
      	td1.appendChild(label);
      	td1.appendChild(required);
	td2.appendChild(adding_type);
	
	td2.appendChild(adding_required);
      	td2.appendChild(select_);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
	
	if(w_field_label_pos=="top")
				label_top(i);

change_class(w_class, i);
refresh_attr(i, 'type_text');
}


function type_captcha(i,w_field_label, w_field_label_pos, w_digit, w_class, w_attr_name, w_attr_value){
    document.getElementById("element_type").value="type_captcha";

	delete_last_child();
// edit table	
	var edit_div  = document.createElement('div');
		edit_div.setAttribute("id", "edit_div");
		edit_div.setAttribute("style", "padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
		
	var edit_main_table  = document.createElement('table');
		edit_main_table.setAttribute("id", "edit_main_table");
		
	var edit_main_tr1  = document.createElement('tr');
      		edit_main_tr1.setAttribute("valing", "top");
		
	var edit_main_tr2  = document.createElement('tr');
      		edit_main_tr2.setAttribute("valing", "top");
		
	var edit_main_tr3  = document.createElement('tr');
      		edit_main_tr3.setAttribute("valing", "top");
		
	var edit_main_tr4  = document.createElement('tr');
      		edit_main_tr4.setAttribute("valing", "top");
		
	var edit_main_tr5  = document.createElement('tr');
      		edit_main_tr5.setAttribute("valing", "top");
			
	var edit_main_tr6  = document.createElement('tr');
      		edit_main_tr6.setAttribute("valing", "top");

	var edit_main_td1 = document.createElement('td');
		edit_main_td1.style.cssText = "padding-top:10px";
	
	var edit_main_td2 = document.createElement('td');
		edit_main_td2.style.cssText = "padding-top:10px";

	var edit_main_td3 = document.createElement('td');
		edit_main_td3.style.cssText = "padding-top:10px";
	var edit_main_td4 = document.createElement('td');
		edit_main_td4.style.cssText = "padding-top:10px";
		
	var edit_main_td5 = document.createElement('td');
		edit_main_td5.style.cssText = "padding-top:10px";
				
	var edit_main_td6 = document.createElement('td');
		edit_main_td6.style.cssText = "padding-top:10px";
		  
	var el_label_label = document.createElement('label');
	                el_label_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_label_label.innerHTML = "Field label";
	
	var el_label_textarea = document.createElement('textarea');
                el_label_textarea.setAttribute("id", "edit_for_label");
                el_label_textarea.setAttribute("rows", "4");
                el_label_textarea.style.cssText = "width:200px";
                el_label_textarea.setAttribute("onKeyUp", "change_label('"+i+"_element_label', this.value)");
		el_label_textarea.innerHTML = w_field_label;
		
	var el_label_position_label = document.createElement('label');
	        el_label_position_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_label_position_label.innerHTML = "Field label position";
	
	var el_label_position1 = document.createElement('input');
                el_label_position1.setAttribute("id", "edit_for_label_position_top");
                el_label_position1.setAttribute("type", "radio");
                el_label_position1.setAttribute("value", "left");
                el_label_position1.style.cssText = "margin-left:15px";
		el_label_position1.setAttribute("name", "edit_for_label_position");
                el_label_position1.setAttribute("onchange", "label_left("+i+")");
	Left = document.createTextNode("Left");
		
	var el_label_position2 = document.createElement('input');
                el_label_position2.setAttribute("id", "edit_for_label_position_left");
                el_label_position2.setAttribute("type", "radio");
                el_label_position2.setAttribute("value", "top");
		el_label_position2.style.cssText = "margin-left:15px";
                el_label_position2.setAttribute("name", "edit_for_label_position");
                el_label_position2.setAttribute("onchange", "label_top("+i+")");
	Top = document.createTextNode("Top");
		
	if(w_field_label_pos=="top")
				el_label_position2.setAttribute("checked", "checked");
	else
				el_label_position1.setAttribute("checked", "checked");

	var el_size_label = document.createElement('label');
	        el_size_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_size_label.innerHTML = "Captcha size";
	
	var el_captcha_digit = document.createElement('input');
                el_captcha_digit.setAttribute("id", "captcha_digit");
                el_captcha_digit.setAttribute("type", "text");
                el_captcha_digit.setAttribute("value", w_digit);
                el_captcha_digit.style.cssText ="margin-left:18px";
		el_captcha_digit.setAttribute("name", "captcha_digit");
 		el_captcha_digit.setAttribute("onKeyPress", "return check_isnum_3_10(event)");
                el_captcha_digit.setAttribute("onKeyUp", "change_captcha_digit(this.value)");

	Digits = document.createTextNode("Digits (3 - 9)");
	var el_style_label = document.createElement('label');
	        el_style_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_style_label.innerHTML = "Class name";
	
	var el_style_textarea = document.createElement('input');
                el_style_textarea.setAttribute("id", "element_style");
		el_style_textarea.setAttribute("type", "text");
		el_style_textarea.setAttribute("value", w_class);
                el_style_textarea.style.cssText = "width:200px; margin-left:20px";
                el_style_textarea.setAttribute("onChange", "change_class(this.value,'"+i+"')");

	var el_attr_label = document.createElement('label');
	                el_attr_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
			el_attr_label.innerHTML = "Additional Attributes";
	var el_attr_add = document.createElement('img');
                el_attr_add.setAttribute("id", "el_choices_add");
           	el_attr_add.setAttribute("src", main_location+'/images/add.png');
            	el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
            	el_attr_add.setAttribute("title", 'add');
                el_attr_add.setAttribute("onClick", "add_attr("+i+", 'type_captcha')");
	var el_attr_table = document.createElement('table');
                el_attr_table.setAttribute("id", 'attributes');
                el_attr_table.setAttribute("border", '0');
        	el_attr_table.style.cssText = 'margin-left:0px';
	var el_attr_tr_label = document.createElement('tr');
                el_attr_tr_label.setAttribute("idi", '0');
	var el_attr_td_name_label = document.createElement('th');
            	el_attr_td_name_label.style.cssText = 'width:100px';
	var el_attr_td_value_label = document.createElement('th');
            	el_attr_td_value_label.style.cssText = 'width:100px';
	var el_attr_td_X_label = document.createElement('th');
            	el_attr_td_X_label.style.cssText = 'width:10px';
	var el_attr_name_label = document.createElement('label');
	                el_attr_name_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_name_label.innerHTML = "Name";
			
	var el_attr_value_label = document.createElement('label');
	                el_attr_value_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 11px";
			el_attr_value_label.innerHTML = "Value";
			
	el_attr_table.appendChild(el_attr_tr_label);
	el_attr_tr_label.appendChild(el_attr_td_name_label);
	el_attr_tr_label.appendChild(el_attr_td_value_label);
	el_attr_tr_label.appendChild(el_attr_td_X_label);
	el_attr_td_name_label.appendChild(el_attr_name_label);
	el_attr_td_value_label.appendChild(el_attr_value_label);
	
	n=w_attr_name.length;
	for(j=1; j<=n; j++)
	{	
		var el_attr_tr = document.createElement('tr');
			el_attr_tr.setAttribute("id", "attr_row_"+j);
			el_attr_tr.setAttribute("idi", j);
		var el_attr_td_name = document.createElement('td');
			el_attr_td_name.style.cssText = 'width:100px';
		var el_attr_td_value = document.createElement('td');
			el_attr_td_value.style.cssText = 'width:100px';
		
		var el_attr_td_X = document.createElement('td');
		var el_attr_name = document.createElement('input');
	
			el_attr_name.setAttribute("type", "text");
	
			el_attr_name.style.cssText = "width:100px";
			el_attr_name.setAttribute("value", w_attr_name[j-1]);
			el_attr_name.setAttribute("id", "attr_name"+j);
			el_attr_name.setAttribute("onChange", "change_attribute_name("+i+", this, 'type_captcha')");
			
		var el_attr_value = document.createElement('input');
	
			el_attr_value.setAttribute("type", "text");
	
			el_attr_value.style.cssText = "width:100px";
			el_attr_value.setAttribute("value", w_attr_value[j-1]);
			el_attr_value.setAttribute("id", "attr_value"+j);
			el_attr_value.setAttribute("onChange", "change_attribute_value("+i+", "+j+", 'type_captcha')");
	
		var el_attr_remove = document.createElement('img');
			el_attr_remove.setAttribute("id", "el_choices"+j+"_remove");
			el_attr_remove.setAttribute("src", main_location+'/images/delete.png');
			el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
			el_attr_remove.setAttribute("align", 'top');
			el_attr_remove.setAttribute("onClick", "remove_attr("+j+", "+i+", 'type_captcha')");
		el_attr_table.appendChild(el_attr_tr);
		el_attr_tr.appendChild(el_attr_td_name);
		el_attr_tr.appendChild(el_attr_td_value);
		el_attr_tr.appendChild(el_attr_td_X);
		el_attr_td_name.appendChild(el_attr_name);
		el_attr_td_value.appendChild(el_attr_value);
		el_attr_td_X.appendChild(el_attr_remove);
		
	}
		
	var t  = document.getElementById('edit_table');
	
	var br = document.createElement('br');
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	
	edit_main_td1.appendChild(el_label_label);
	edit_main_td1.appendChild(br);
	edit_main_td1.appendChild(el_label_textarea);

	edit_main_td2.appendChild(el_label_position_label);
	edit_main_td2.appendChild(br1);
	edit_main_td2.appendChild(el_label_position1);
	edit_main_td2.appendChild(Left);
	edit_main_td2.appendChild(br2);
	edit_main_td2.appendChild(el_label_position2);
	edit_main_td2.appendChild(Top);
	
	edit_main_td3.appendChild(el_size_label);
	edit_main_td3.appendChild(br3);
	edit_main_td3.appendChild(br4);
	edit_main_td3.appendChild(Digits);
	edit_main_td3.appendChild(el_captcha_digit);
	edit_main_td4.appendChild(el_style_label);
	edit_main_td4.appendChild(el_style_textarea);
	
	edit_main_td5.appendChild(el_attr_label);
	edit_main_td5.appendChild(el_attr_add);
	edit_main_td5.appendChild(br3);
	edit_main_td5.appendChild(el_attr_table);

	/*edit_main_td4.appendChild(el_first_value_label);
	edit_main_td4.appendChild(br3);
	edit_main_td4.appendChild(el_first_value_input);*/
	
/*	edit_main_td5.appendChild(el_guidelines_label);
	edit_main_td5.appendChild(br4);
	edit_main_td5.appendChild(el_guidelines_textarea);
*/	
	edit_main_tr1.appendChild(edit_main_td1);
	edit_main_tr2.appendChild(edit_main_td2);
	edit_main_tr3.appendChild(edit_main_td3);
	edit_main_tr4.appendChild(edit_main_td4);
	edit_main_tr5.appendChild(edit_main_td5);
	edit_main_table.appendChild(edit_main_tr1);
	edit_main_table.appendChild(edit_main_tr2);
	edit_main_table.appendChild(edit_main_tr3);
	edit_main_table.appendChild(edit_main_tr4);
	edit_main_table.appendChild(edit_main_tr5);
	edit_div.appendChild(edit_main_table);
	
	t.appendChild(edit_div);
	
//show table

	element='img';	type='captcha'; 
		var adding_type = document.createElement("input");
            adding_type.setAttribute("type", "hidden");
            adding_type.setAttribute("value", "type_captcha");
            adding_type.setAttribute("name", i+"_type");


	var adding = document.createElement(element);
           	 	adding.setAttribute("type", type);
           	 	adding.setAttribute("digit", w_digit);
           	 	adding.setAttribute("src", main_location+"/wd_captcha.php?digit="+w_digit);
			adding.setAttribute("id", "wd_captcha");
			adding.setAttribute("class", "captcha_img");
			adding.setAttribute("onClick", "captcha_refresh('wd_captcha')");
			
   
		
	var refresh_captcha = document.createElement("img");
           	 	refresh_captcha.setAttribute("src", main_location+"/images/refresh.png");
			refresh_captcha.setAttribute("class", "captcha_refresh");
			refresh_captcha.setAttribute("id", "element_refresh");
			refresh_captcha.setAttribute("onClick", "captcha_refresh('wd_captcha')");

	var input_captcha = document.createElement("input");
           	 	input_captcha.setAttribute("type", "text");
			input_captcha.style.cssText = "width:"+(w_digit*10+15)+"px;";
			input_captcha.setAttribute("class", "captcha_input");
			
			input_captcha.setAttribute("id", "wd_captcha_input");
			input_captcha.setAttribute("name", "captcha_input");

     	var div = document.createElement('div');
      	    div.setAttribute("id", "main_div");
					
      	var table = document.createElement('table');
           	table.setAttribute("id", i+"_elemet_table");
			
      	var tr = document.createElement('tr');
			
      	var td1 = document.createElement('td');
         	td1.setAttribute("valign", 'top');
         	td1.setAttribute("align", 'left');
           	td1.setAttribute("id", i+"_label_section");
			
      	var td2 = document.createElement('td');
        	td2.setAttribute("valign", 'top');
         	td2.setAttribute("align", 'left');
           	td2.setAttribute("id", i+"_element_section");
		
	var captcha_table = document.createElement('table');
		captcha_table.setAttribute("class", 'captcha_table');
	
      	var captcha_tr1 = document.createElement('tr');
      	var captcha_tr2 = document.createElement('tr');

      	var captcha_td1 = document.createElement('td');
		captcha_td1.setAttribute("valign", 'middle');

      	var captcha_td2 = document.createElement('td');
  		captcha_td2.setAttribute("valign", 'middle');
    	var captcha_td3 = document.createElement('td');
	
	captcha_table.appendChild(captcha_tr1);
      	captcha_table.appendChild(captcha_tr2);
      	captcha_tr1.appendChild(captcha_td1);
      	captcha_tr1.appendChild(captcha_td2);
      	captcha_tr2.appendChild(captcha_td3);
	
      	captcha_td1.appendChild(adding);
      	captcha_td2.appendChild(refresh_captcha);
      	captcha_td3.appendChild(input_captcha);

	
	
	
      	var br1 = document.createElement('br');
      	var br2 = document.createElement('br');
     	var br3 = document.createElement('br');
      	var br4 = document.createElement('br');
      

      	var label = document.createElement('span');
			label.setAttribute("id", i+"_element_label");
			label.innerHTML = w_field_label;
			label.setAttribute("class", "label");
	    
      	var main_td  = document.getElementById('show_table');
      
      	td1.appendChild(label);
      	td2.appendChild(adding_type);
      	td2.appendChild(captcha_table);
      	tr.appendChild(td1);
      	tr.appendChild(td2);
      	table.appendChild(tr);
      
      	div.appendChild(table);
      	div.appendChild(br3);
      	main_td.appendChild(div);
	if(w_field_label_pos=="top")
				label_top(i);
change_class(w_class, i);
refresh_attr(i, 'type_captcha');
}


function addRow(b) 
{
	if(document.getElementById('show_table').innerHTML)
	{
		document.getElementById('show_table').innerHTML="";
		document.getElementById('edit_table').innerHTML="";
	}
	
	switch(b)
	{
		case 'editor': { el_editor();  break;}
		case 'text': { el_text();  break;}
		case 'checkbox':{ el_checkbox(); break;}
		case 'radio':{ el_radio(); break;}
		case 'time_and_date':{ el_time_and_date(); break; }

		case 'select':{ el_select(); break; }
		case 'file_upload':{ el_file_upload(); break; }
		case 'captcha':{ el_captcha(); break; }
		case 'map':{ el_map(); break; }
		
		case 'button':{ el_button(); break; }
	}
	
	var pos=document.getElementsByName("el_pos");
			pos[0].checked="checked";

}

function el_button()
{
	
if(document.getElementById("editing_id").value)
	new_id=document.getElementById("editing_id").value;
else
	new_id=gen;
	


//edit table
	var td  = document.getElementById('edit_table');
	//type select		
	var el_type_label = document.createElement('label');
	
	el_type_label.style.cssText = "color: #00aeef; font-weight: bold; font-size: 13px";
	el_type_label.innerHTML = "<br />&nbsp;&nbsp;Field type";
	td.appendChild(el_type_label);
	var el_type_radio_submit_reset = document.createElement('input');
                el_type_radio_submit_reset.setAttribute("type", "radio");
		el_type_radio_submit_reset.style.cssText = "margin-left:15px";
                el_type_radio_submit_reset.setAttribute("name", "el_type");
                el_type_radio_submit_reset.setAttribute("onclick", "go_to_type_submit_reset('"+new_id+"')");
		el_type_radio_submit_reset.setAttribute("checked", "checked");
		Submit_and_Reset = document.createTextNode("Submit and Reset");
		
	var el_type_radio_custom = document.createElement('input');
                el_type_radio_custom.setAttribute("type", "radio");
		el_type_radio_custom.style.cssText = "margin-left:15px";
               
	       el_type_radio_custom.setAttribute("name", "el_type");
                el_type_radio_custom.setAttribute("onclick", "go_to_type_button('"+new_id+"')");
		Custom = document.createTextNode("Custom");
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	

	td.appendChild(br1);
	td.appendChild(el_type_radio_submit_reset);
	td.appendChild(Submit_and_Reset);
	td.appendChild(br2);
	td.appendChild(el_type_radio_custom);
	td.appendChild(Custom);

	var pos=document.getElementsByName("el_pos");
			pos[0].removeAttribute("disabled");
			pos[1].removeAttribute("disabled");
			pos[2].removeAttribute("disabled");

	go_to_type_submit_reset(new_id);
	
}

function go_to_type_hidden(new_id)
{
 	w_attr_name=[];
 	w_attr_value=[];
	type_hidden(new_id,'', '', '', w_attr_name, w_attr_value);
}
function go_to_type_submit_reset(new_id)
{
 	w_attr_name=[];
 	w_attr_value=[];
	type_submit_reset(new_id,'Submit', 'Reset', '', true, w_attr_name, w_attr_value);
}

function el_map()
{
	alert("This field type is disabled in free version. If you need this functionality, you need to buy the commercial version.");
	return;
}

function el_editor()
{
	if(document.getElementById("editing_id").value)
	new_id=document.getElementById("editing_id").value;
else
	new_id=gen;
	

	var pos=document.getElementsByName("el_pos");
			pos[0].removeAttribute("disabled");
			pos[1].removeAttribute("disabled");
			pos[2].removeAttribute("disabled");
	
	type_editor(new_id,'');
}
function el_text()
{
	
if(document.getElementById("editing_id").value)
	new_id=document.getElementById("editing_id").value;
else
	new_id=gen;
	

//edit table
	var td  = document.getElementById('edit_table');
	//type select		
	var el_type_label = document.createElement('label');
	
	el_type_label.style.cssText = "color: #00aeef; font-weight: bold; font-size: 13px";
	//el_type_label.setAttribute("style" , "color: #00aeef; font-weight: bold; font-size: 13px", 0 );
	el_type_label.innerHTML = "<br />&nbsp;&nbsp;Field type";
	td.appendChild(el_type_label);

	var el_type_radio_text = document.createElement('input');
                el_type_radio_text.setAttribute("id", "el_type_radio_text");
                el_type_radio_text.setAttribute("type", "radio");
               // el_type_radio_text.style.cssText = "margin-left:15px";
		el_type_radio_text.style.cssText = "margin-left:15px";
                el_type_radio_text.setAttribute("value", "text");
                el_type_radio_text.setAttribute("name", "el_type");
                el_type_radio_text.setAttribute("onclick", "go_to_type_text('"+new_id+"')");
		el_type_radio_text.setAttribute("checked", "checked");
		Text = document.createTextNode("Simple text");
		
	var el_type_radio_password = document.createElement('input');
                el_type_radio_password.setAttribute("id", "el_type_radio_password");
                el_type_radio_password.setAttribute("type", "radio");
                //el_type_radio_password.style.cssText = "margin-left:15px";
		el_type_radio_password.style.cssText = "margin-left:15px";
                el_type_radio_password.setAttribute("value", "password");
                el_type_radio_password.setAttribute("name", "el_type");
                el_type_radio_password.setAttribute("onclick", "go_to_type_password('"+new_id+"')");
		Password = document.createTextNode("Password");

	var el_type_radio_textarea = document.createElement('input');
                el_type_radio_textarea.setAttribute("id", "el_type_radio_textarea");
                el_type_radio_textarea.setAttribute("type", "radio");
                //el_type_radio_textarea.style.cssText = "margin-left:15px";
		el_type_radio_textarea.style.cssText = "margin-left:15px";
                el_type_radio_textarea.setAttribute("value", "textarea");
                el_type_radio_textarea.setAttribute("name", "el_type");
                el_type_radio_textarea.setAttribute("onclick", "go_to_type_textarea('"+new_id+"')");
		Textarea = document.createTextNode("Text area");
		
	var el_type_radio_name = document.createElement('input');
                el_type_radio_name.setAttribute("id", "el_type_radio_name");
                el_type_radio_name.setAttribute("type", "radio");
                //el_type_radio_name.style.cssText = "margin-left:15px";
		el_type_radio_name.style.cssText = "margin-left:15px";
                el_type_radio_name.setAttribute("value", "name");
                el_type_radio_name.setAttribute("name", "el_type");
                el_type_radio_name.setAttribute("onclick", "go_to_type_name('"+new_id+"')");
		Name = document.createTextNode("Name");
	var el_type_radio_submitter_mail= document.createElement('input');
                el_type_radio_submitter_mail.setAttribute("id", "el_type_radio_submitter_mail");
                el_type_radio_submitter_mail.setAttribute("type", "radio");
               // el_type_radio_submitter_mail.style.cssText = "margin-left:15px";
		el_type_radio_submitter_mail.style.cssText = "margin-left:15px";
                el_type_radio_submitter_mail.setAttribute("value", "submitter_mail");
                el_type_radio_submitter_mail.setAttribute("name", "el_type");
                el_type_radio_submitter_mail.setAttribute("onclick", "go_to_type_submitter_mail('"+new_id+"')");
		Submitter_mail = document.createTextNode("E-mail");

	var el_type_radio_hidden = document.createElement('input');
                el_type_radio_hidden.setAttribute("type", "radio");
		el_type_radio_hidden.style.cssText = "margin-left:15px";
                el_type_radio_hidden.setAttribute("name", "el_type");
                el_type_radio_hidden.setAttribute("onclick", "go_to_type_hidden('"+new_id+"')");
		Hidden = document.createTextNode("Hidden field");
/*var el_type_radio_address = document.createElement('input');
                el_type_radio_address.setAttribute("id", "el_type_radio_address");
                el_type_radio_address.setAttribute("type", "radio");
                el_type_radio_address.setAttribute("value", "address");
                el_type_radio_address.setAttribute("name", "el_type");
                el_type_radio_address.setAttribute("onchange", "type_address()");
		Address = document.createTextNode("Address");*/
	
	
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	var br4 = document.createElement('br');
	var br5 = document.createElement('br');
	var br6 = document.createElement('br');
	

	td.appendChild(br1);
	td.appendChild(el_type_radio_text);
	td.appendChild(Text);
	td.appendChild(br2);
	td.appendChild(el_type_radio_password);
	td.appendChild(Password);
	td.appendChild(br3);
	td.appendChild(el_type_radio_textarea);
	td.appendChild(Textarea);
	td.appendChild(br4);
	td.appendChild(el_type_radio_name);
	td.appendChild(Name);
	td.appendChild(br5);
	td.appendChild(el_type_radio_submitter_mail);
	td.appendChild(Submitter_mail);
	td.appendChild(br6);
	td.appendChild(el_type_radio_hidden);
	td.appendChild(Hidden);
	
	var pos=document.getElementsByName("el_pos");
			pos[0].removeAttribute("disabled");
			pos[1].removeAttribute("disabled");
			pos[2].removeAttribute("disabled");
			
	go_to_type_text(new_id);
	
}

function go_to_type_text(new_id)
{
 	w_attr_name=[];
 	w_attr_value=[];
	type_text(new_id,'Text:', 'left', '200', '', '', 'no', '',w_attr_name, w_attr_value);
}
function go_to_type_password(new_id)
{
 	w_attr_name=[];
 	w_attr_value=[];
	type_password(new_id,'Password:', 'left', '200', 'no', '',w_attr_name, w_attr_value);
}
function go_to_type_textarea(new_id)
{
 	w_attr_name=[];
 	w_attr_value=[];
	type_textarea(new_id,'Textarea:', 'left', '200', '100', '','', 'no', '',w_attr_name, w_attr_value)
}

function go_to_type_name(new_id)
{
 	w_attr_name=[];
 	w_attr_value=[];
	type_name(new_id,'Name:', 'left', '95', 'normal', 'no', '',w_attr_name, w_attr_value)
}


function go_to_type_submitter_mail(new_id)
{
 	w_attr_name=[];
 	w_attr_value=[];
	type_submitter_mail(new_id,'E-mail:', 'left', '200', '', '', 'no', 'no', '', w_attr_name, w_attr_value);
}

function go_to_type_time(new_id)
{
	
 	w_attr_name=[];
 	w_attr_value=[];
	
	type_time(new_id, 'Time:', 'left', '24', '0', '1','','','', 'no', '',w_attr_name, w_attr_value);
	
}

function go_to_type_date(new_id)
{
	
 	w_attr_name=[];
 	w_attr_value=[];
	
	type_date(new_id, 'Date:', 'left', '', 'no', '', '%Y-%m-%d', '...',w_attr_name, w_attr_value);
}

function go_to_type_date_fields(new_id)
{
	
 	w_attr_name=[];
 	w_attr_value=[];
	type_date_fields(new_id, 'Date:', 'left', '', '', '', 'SELECT', 'SELECT', 'SELECT', 'day', 'month', 'year', '40', '60', '60', 'no', '', '1901', '2012', '&nbsp;/&nbsp;', w_attr_name, w_attr_value);
}

function go_to_type_button(new_id)
{
 	w_title=[ "Button"];
 	w_func=[""];
	
 	w_attr_name=[];
 	w_attr_value=[];
	
	type_button(new_id, w_title, w_func, '',w_attr_name, w_attr_value);
}

function el_checkbox()
{
if(document.getElementById("editing_id").value)
	new_id=document.getElementById("editing_id").value;
else
	new_id=gen;
	


	var pos=document.getElementsByName("el_pos");
			pos[0].removeAttribute("disabled");
			pos[1].removeAttribute("disabled");
			pos[2].removeAttribute("disabled");

 	w_choices=[ "option 1", "option 2"];
 	w_choices_checked=["0", "0"];
	
 	w_attr_name=[];
 	w_attr_value=[];
	type_checkbox(new_id,'Checkbox:', 'left', 'ver', w_choices, w_choices_checked, 'no', '',w_attr_name, w_attr_value);
}
function el_radio()
{
	
if(document.getElementById("editing_id").value)
	new_id=document.getElementById("editing_id").value;
else
	new_id=gen;
	
	var pos=document.getElementsByName("el_pos");
			pos[0].removeAttribute("disabled");
			pos[1].removeAttribute("disabled");
			pos[2].removeAttribute("disabled");

 	w_choices=[ "option 1", "option 2"];
 	w_choices_checked=["0", "0"];
	
 	w_attr_name=[];
 	w_attr_value=[];
	
	type_radio(new_id,'Radio:', 'left', 'ver', w_choices, w_choices_checked, 'no', '',w_attr_name, w_attr_value);
}

function el_time_and_date()
{
if(document.getElementById("editing_id").value)
	new_id=document.getElementById("editing_id").value;
else
	new_id=gen;
	
	
//edit table

	//type select		
	var el_type_label = document.createElement('label');
                el_type_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
		el_type_label.innerHTML = "<br />&nbsp;&nbsp;Field type";
	
	var el_type_radio_time = document.createElement('input');
                el_type_radio_time.setAttribute("id", "el_type_radio_time");
                el_type_radio_time.setAttribute("type", "radio");
                el_type_radio_time.setAttribute("value", "time");
                el_type_radio_time.style.cssText = "margin-left:15px";
                el_type_radio_time.setAttribute("name", "el_type_radio_time");
                el_type_radio_time.setAttribute("onclick", "go_to_type_time('"+new_id+"')");
		Time_ = document.createTextNode("Time");

	var el_type_radio_date = document.createElement('input');
                el_type_radio_date.setAttribute("id", "el_type_radio_date");
                el_type_radio_date.setAttribute("type", "radio");
                el_type_radio_date.setAttribute("value", "date");
                el_type_radio_date.style.cssText = "margin-left:15px";
                el_type_radio_date.setAttribute("name", "el_type_radio_time");
                el_type_radio_date.setAttribute("onclick", "go_to_type_date('"+new_id+"')");
				el_type_radio_date.setAttribute("checked", "checked");

		Date_ = document.createTextNode("Date (Single fileld with a picker)");
		
	var el_type_radio_date_fields = document.createElement('input');
                el_type_radio_date_fields.setAttribute("id", "el_type_radio_date_fields");
                el_type_radio_date_fields.setAttribute("type", "radio");
                el_type_radio_date_fields.setAttribute("value", "date_fields");
                el_type_radio_date_fields.style.cssText = "margin-left:15px";
                el_type_radio_date_fields.setAttribute("name", "el_type_radio_time");
                el_type_radio_date_fields.setAttribute("onclick", "go_to_type_date_fields('"+new_id+"')");
			
		Date_fields_ = document.createTextNode("Date (3 separate fields)");

	var td  = document.getElementById('edit_table');
	
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	
	td.appendChild(el_type_label);
	td.appendChild(br1);
	td.appendChild(el_type_radio_date);
	td.appendChild(Date_);
	td.appendChild(br2);
	td.appendChild(el_type_radio_date_fields);
	td.appendChild(Date_fields_);
	td.appendChild(br3);
	td.appendChild(el_type_radio_time);
	td.appendChild(Time_);
	var pos=document.getElementsByName("el_pos");
			pos[0].removeAttribute("disabled");
			pos[1].removeAttribute("disabled");
			pos[2].removeAttribute("disabled");
	
	go_to_type_date(new_id);
	
}
function go_to_type_own_select(new_id)
{
 	w_choices=[ "Select value", "option 1", "option 2"];
 	w_choices_checked=["1", "0", "0"];
	w_choices_disabled=[true, false, false];
 	w_attr_name=[];
 	w_attr_value=[];
	type_own_select(new_id, 'Select:', 'left', '200',w_choices, w_choices_checked, 'no','',w_attr_name, w_attr_value, w_choices_disabled);
}
function go_to_type_country(new_id)
{
 	w_attr_name=[];
 	w_attr_value=[];
	type_country(new_id,'Country:', 'left', '200', 'no', '',w_attr_name, w_attr_value);
}


function el_select()
{
//edit table
if(document.getElementById("editing_id").value)
	new_id=document.getElementById("editing_id").value;
else
	new_id=gen;
	

	//type select		
	var el_type_label = document.createElement('label');
                el_type_label.style.cssText ="color:#00aeef; font-weight:bold; font-size: 13px";
		el_type_label.innerHTML = "<br />&nbsp;&nbsp;Field type";
	
	var el_type_radio_own_select = document.createElement('input');
                el_type_radio_own_select.setAttribute("id", "el_type_radio_own_select");
                el_type_radio_own_select.setAttribute("type", "radio");
                el_type_radio_own_select.setAttribute("value", "own_select");
                el_type_radio_own_select.style.cssText = "margin-left:15px";
                el_type_radio_own_select.setAttribute("name", "el_type_radio_select");
                el_type_radio_own_select.setAttribute("onclick", "go_to_type_own_select('"+new_id+"')");
		el_type_radio_own_select.setAttribute("checked", "checked");
		Own_select = document.createTextNode("Custom Select");
		
	var el_type_radio_country = document.createElement('input');
                el_type_radio_country.setAttribute("id", "el_type_radio_country");
                el_type_radio_country.setAttribute("type", "radio");
                el_type_radio_country.setAttribute("value", "country");
                el_type_radio_country.style.cssText = "margin-left:15px";
                el_type_radio_country.setAttribute("name", "el_type_radio_select");
                el_type_radio_country.setAttribute("onclick", "go_to_type_country('"+new_id+"')");
		Country = document.createTextNode("Country List");

	var td  = document.getElementById('edit_table');
	
	var br1 = document.createElement('br');
	var br2 = document.createElement('br');
	var br3 = document.createElement('br');
	
	td.appendChild(el_type_label);
	td.appendChild(br1);
	td.appendChild(el_type_radio_own_select);
	td.appendChild(Own_select);
	td.appendChild(br2);
	td.appendChild(el_type_radio_country);
	td.appendChild(Country);
	var pos=document.getElementsByName("el_pos");
			pos[0].removeAttribute("disabled");
			pos[1].removeAttribute("disabled");
			pos[2].removeAttribute("disabled");
	
	go_to_type_own_select(new_id);
}

function el_file_upload()
{
alert("This field type is disabled in free version. If you need this functionality, you need to buy the commercial version.");
return;
}
function el_captcha()
{
//edit table
if(document.getElementById("editing_id").value)
	new_id=document.getElementById("editing_id").value;
else
	new_id=gen;
	
	if(document.getElementById('wd_captcha'))
	{
		alert("The captcha already has been created.");
		return;
	}
	var pos=document.getElementsByName("el_pos");
		pos[0].removeAttribute("disabled");
		pos[1].removeAttribute("disabled");
		pos[2].removeAttribute("disabled");
	
 	w_attr_name=[];
 	w_attr_value=[];
	type_captcha(new_id,'Word Verification:', 'left', '6','',w_attr_name, w_attr_value);
}

function remove_row(id)
{
	var tr = document.getElementById(id);
	tr.parentNode.removeChild(tr);
	refresh_();
}
function destroyChildren(node)
{
  while (node.firstChild)
      node.removeChild(node.firstChild);
}

function cont_elements()
{
cp2=0;

for(t=1;t<100; t++)
	if(document.getElementById(t))
		cp2++;
return cp2;
}

function add(key)
{
	if(document.getElementById('editing_id').value=="")
	if(key==0)
	{
		kz6=cont_elements();
		if(kz6>4)
		{
		alert("The free version is limited up to 5 fields to add. If you need this functionality, you need to buy the commercial version.");
		return;
		}
	}
	if(document.getElementById('main_editor').style.display=="block")
	{
		if(document.getElementById('editing_id').value)
		{
			i=document.getElementById('editing_id').value;		
			document.getElementById('editing_id').value="";
			tr=document.getElementById(i);
			destroyChildren(tr);
			var img_X = document.createElement("img");
					img_X.setAttribute("src", main_location+"/images/delete_el.png");
//					img_X.setAttribute("height", "17");
					img_X.style.cssText = "cursor:pointer; margin-left:30px";
					img_X.setAttribute("onclick", 'remove_row("'+i+'")');
					
			var td_X = document.createElement("td");
					td_X.setAttribute("id", "X_"+i);
					td_X.setAttribute("valign", "middle");
					td_X.setAttribute("align", "right");
					td_X.appendChild(img_X);
//image pah@
			var img_UP = document.createElement("img");
					img_UP.setAttribute("src", main_location+"/images/up.png");
//					img_UP.setAttribute("height", "17");
					img_UP.style.cssText = "cursor:pointer";
					img_UP.setAttribute("onclick", 'up_row("'+i+'")');
					
			var td_UP = document.createElement("td");
					td_UP.setAttribute("id", "up_"+i);
					td_UP.setAttribute("valign", "middle");
					td_UP.appendChild(img_UP);
					
			var img_DOWN = document.createElement("img");
					img_DOWN.setAttribute("src", main_location+"/images/down.png");
//					img_DOWN.setAttribute("height", "17");
					img_DOWN.style.cssText = "margin:2px;cursor:pointer";
					img_DOWN.setAttribute("onclick", 'down_row("'+i+'")');
					
			var td_DOWN = document.createElement("td");
					td_DOWN.setAttribute("id", "down_"+i);
					td_DOWN.setAttribute("valign", "middle");
					td_DOWN.appendChild(img_DOWN);

					
			var img_RIGHT = document.createElement("img");
					img_RIGHT.setAttribute("src", main_location+"/images/right.png");
	//				img_RIGHT.setAttribute("height", "17");
					img_RIGHT.style.cssText = "cursor:pointer";
					img_RIGHT.setAttribute("onclick", 'right_row("'+i+'")');
					
			var td_RIGHT = document.createElement("td");
					td_RIGHT.setAttribute("id", "right_"+i);
					td_RIGHT.setAttribute("valign", "middle");
					td_RIGHT.appendChild(img_RIGHT);
					
			var img_LEFT = document.createElement("img");
					img_LEFT.setAttribute("src", main_location+"/images/left.png");
//					img_LEFT.setAttribute("height", "17");
					img_LEFT.style.cssText = "margin:2px;cursor:pointer";
					img_LEFT.setAttribute("onclick", 'left_row("'+i+'")');
					
			var td_LEFT = document.createElement("td");
					td_LEFT.setAttribute("id", "left_"+i);
					td_LEFT.setAttribute("valign", "middle");
					td_LEFT.appendChild(img_LEFT);

			var img_EDIT = document.createElement("img");
					img_EDIT.setAttribute("src", main_location+"/images/edit.png");
//					img_EDIT.setAttribute("height", "17");
					img_EDIT.style.cssText = "margin:2px;cursor:pointer";
					img_EDIT.setAttribute("onclick", 'edit("'+i+'")');
					
			var td_EDIT = document.createElement("td");
					td_EDIT.setAttribute("id", "edit_"+i);
					td_EDIT.setAttribute("valign", "middle");
					td_EDIT.appendChild(img_EDIT);
///////////////////////////////////////////////////////////////////////////////////////////////
			var in_editor = document.createElement("td");
					in_editor.setAttribute("id", i+"_element_section");
         			in_editor.setAttribute("align", 'left');
					in_editor.setAttribute("valign", "top");
					in_editor.setAttribute("colspan", "2");
					
	
		if(document.getElementById('editor').style.display=="none")
		{
			ifr_id=document.getElementsByTagName("iframe")[0].id;
				ifr=getIFrameDocument(ifr_id);
				in_editor.innerHTML=ifr.body.innerHTML;
		}
		else
		{
				in_editor.innerHTML=document.getElementById('editor').value;
		}
			
			var label = document.createElement('span');
					label.setAttribute("id", i+"_element_label");
					label.innerHTML = "custom_"+i;
					label.style.cssText = 'display:none';
					
			td_EDIT.appendChild(label);
			tr.appendChild(in_editor);
			tr.appendChild(td_X);
			tr.appendChild(td_LEFT);
			tr.appendChild(td_UP);
			tr.appendChild(td_DOWN);
			tr.appendChild(td_RIGHT);
			tr.appendChild(td_EDIT);
			j=2;
			refresh_();
		}
		else
		{
			i=gen;
			gen++;
			
			var tr = document.createElement('tr');
				tr.setAttribute("id", i);
				tr.setAttribute("type", "type_editor");
				
				
			var select_ = document.getElementById('sel_el_pos');
			var option = document.createElement('option');
				option.setAttribute("id", i+"_sel_el_pos");
				option.setAttribute("value", i);
				option.innerHTML="custom_"+i;
				
			if(document.getElementById('form_view').firstChild.nodeType==3)
				table=document.getElementById('form_view').childNodes[1].childNodes[1].childNodes[1].childNodes[1];
			else
				table=document.getElementById('form_view').firstChild.firstChild.firstChild.firstChild;

			var img_X = document.createElement("img");
					img_X.setAttribute("src", main_location+"/images/delete_el.png");
//					img_X.setAttribute("height", "17");
					img_X.style.cssText = "cursor:pointer; margin-left:30px";
					img_X.setAttribute("onclick", 'remove_row("'+i+'")');
					
			var td_X = document.createElement("td");
					td_X.setAttribute("id", "X_"+i);
					td_X.setAttribute("valign", "middle");
					td_X.setAttribute("align", "right");
					td_X.appendChild(img_X);
//image pah@
			var img_UP = document.createElement("img");
					img_UP.setAttribute("src", main_location+"/images/up.png");
//					img_UP.setAttribute("height", "17");
					img_UP.style.cssText = "cursor:pointer";
					img_UP.setAttribute("onclick", 'up_row("'+i+'")');
					
			var td_UP = document.createElement("td");
					td_UP.setAttribute("id", "up_"+i);
					td_UP.setAttribute("valign", "middle");
					td_UP.appendChild(img_UP);
					
			var img_DOWN = document.createElement("img");
					img_DOWN.setAttribute("src", main_location+"/images/down.png");
//					img_DOWN.setAttribute("height", "17");
					img_DOWN.style.cssText = "margin:2px;cursor:pointer";
					img_DOWN.setAttribute("onclick", 'down_row("'+i+'")');
					
			var td_DOWN = document.createElement("td");
					td_DOWN.setAttribute("id", "down_"+i);

					td_DOWN.setAttribute("valign", "middle");
					td_DOWN.appendChild(img_DOWN);

					
			var img_RIGHT = document.createElement("img");
					img_RIGHT.setAttribute("src", main_location+"/images/right.png");
//					img_RIGHT.setAttribute("height", "17");
					img_RIGHT.style.cssText = "cursor:pointer";
					img_RIGHT.setAttribute("onclick", 'right_row("'+i+'")');
					
			var td_RIGHT = document.createElement("td");
					td_RIGHT.setAttribute("id", "right_"+i);
					td_RIGHT.setAttribute("valign", "middle");
					td_RIGHT.appendChild(img_RIGHT);
					
			var img_LEFT = document.createElement("img");
					img_LEFT.setAttribute("src", main_location+"/images/left.png");
//					img_LEFT.setAttribute("height", "17");
					img_LEFT.style.cssText = "margin:2px;cursor:pointer";
					img_LEFT.setAttribute("onclick", 'left_row("'+i+'")');
					
			var td_LEFT = document.createElement("td");
					td_LEFT.setAttribute("id", "left_"+i);
					td_LEFT.setAttribute("valign", "middle");
					td_LEFT.appendChild(img_LEFT);

			var img_EDIT = document.createElement("img");
					img_EDIT.setAttribute("src", main_location+"/images/edit.png");
//					img_EDIT.setAttribute("height", "17");
					img_EDIT.style.cssText = "margin:2px;cursor:pointer";
					img_EDIT.setAttribute("onclick", 'edit("'+i+'")');
					
			var td_EDIT = document.createElement("td");
					td_EDIT.setAttribute("id", "edit_"+i);
					td_EDIT.setAttribute("valign", "middle");
					td_EDIT.appendChild(img_EDIT);

///////////////////////////////////////////////////////////////////////////////////////////////
			var in_editor = document.createElement("td");
					in_editor.setAttribute("id", i+"_element_section");
         				in_editor.setAttribute("align", 'left');
					in_editor.setAttribute("valign", "top");
					in_editor.setAttribute("colspan", "2");
					if(document.getElementsByTagName("iframe")[0])
					{
ifr_id=document.getElementsByTagName("iframe")[0].id;
ifr=getIFrameDocument(ifr_id)
					}

		if(document.getElementById('editor').style.display=="none")
		{
			if(document.getElementsByTagName("iframe")[0])
				in_editor.innerHTML=ifr.body.innerHTML;
				else
				in_editor.innerHTML=document.getElementById('editor').value;
		}
		else
		{
				in_editor.innerHTML=document.getElementById('editor').value;
		}
			
			
			
			var label = document.createElement('span');
					label.setAttribute("id", i+"_element_label");
					label.innerHTML = "custom_"+i;
					label.style.cssText = 'display:none';
					
			td_EDIT.appendChild(label);
			tr.appendChild(in_editor);

			tr.appendChild(td_X);
			tr.appendChild(td_LEFT);
			tr.appendChild(td_UP);
			tr.appendChild(td_DOWN);
			tr.appendChild(td_RIGHT);
			tr.appendChild(td_EDIT);

			if(document.getElementById('pos_end').checked)
			{
				table.appendChild(tr);
			}
			if(document.getElementById('pos_begin').checked)
			{	
				table.insertBefore(tr, table.firstChild);
			}
			if(document.getElementById('pos_before').checked)
			{
				beforeTr=document.getElementById(document.getElementById('sel_el_pos').value);
				table=beforeTr.parentNode;
				beforeOption=document.getElementById(document.getElementById('sel_el_pos').value+'_sel_el_pos');
				table.insertBefore(tr, beforeTr);
				select_.insertBefore(option, beforeOption);
			}
			j=2;
			refresh_();
		
		}

	close_window();
	}


	else
	if(document.getElementById('show_table').innerHTML)
	{
		
		if(document.getElementById('editing_id').value)
			i=document.getElementById('editing_id').value;		
		else
			i=gen;
			
		type=document.getElementById("element_type").value;
		if(type=="type_hidden")
		{
			if(document.getElementById(i+'_element').name=="")
			{
				alert("The name of the field is required.");
				return;
			}
		}
		
		if(type=="type_map")
		{
			if_gmap_updateMap();
		}
	
		if(document.getElementById(i+'_element_label').innerHTML)
		{

		if(document.getElementById('editing_id').value)
		{
			Disable();
			i=document.getElementById('editing_id').value;		
			in_lab=false;
			var labels_array=new Array();
			for(w=0; w<gen;w++)
			{	
				if(w!=i)
				if(document.getElementById(w+'_element_label'))
					labels_array.push(document.getElementById(w+'_element_label').innerHTML);
			}			
	
			for(t=0; t<labels_array.length;t++)
			{	
			if(document.getElementById(i+'_element_label').innerHTML==labels_array[t])
				{
					in_lab=true;
					break;
				}
			}
			if(in_lab)
			{
				alert('Sorry, the labels must be unique.');
			}
			else
			{
	
	
	
	
				document.getElementById('editing_id').value="";
	
				tr=document.getElementById(i);
	
				destroyChildren(tr);
				tr.setAttribute('type', type);
				var img_X = document.createElement("img");
	
						img_X.setAttribute("src", main_location+"/images/delete_el.png");
	
//						img_X.setAttribute("height", "17");
	
						img_X.style.cssText = "cursor:pointer; margin-left:30px";
	
						img_X.setAttribute("onclick", 'remove_row("'+i+'")');
	
						
	
				var td_X = document.createElement("td");
	
						td_X.setAttribute("id", "X_"+i);
	
						td_X.setAttribute("valign", "middle");
						td_X.setAttribute("align", "right");
						td_X.appendChild(img_X);
	
	//image pah@
	
				var img_UP = document.createElement("img");
	
						img_UP.setAttribute("src", main_location+"/images/up.png");
	
//						img_UP.setAttribute("height", "17");
	
						img_UP.style.cssText = "cursor:pointer";
	
						img_UP.setAttribute("onclick", 'up_row("'+i+'")');
	
						
	
				var td_UP = document.createElement("td");
	
						td_UP.setAttribute("id", "up_"+i);
	
						td_UP.setAttribute("valign", "middle");
	
						td_UP.appendChild(img_UP);
	
						
	
				var img_DOWN = document.createElement("img");
	
						img_DOWN.setAttribute("src", main_location+"/images/down.png");
	
//						img_DOWN.setAttribute("height", "17");
	
						img_DOWN.style.cssText = "margin:2px;cursor:pointer";
	
						img_DOWN.setAttribute("onclick", 'down_row("'+i+'")');
	
						
	
				var td_DOWN = document.createElement("td");
	
						td_DOWN.setAttribute("id", "down_"+i);
	
						td_DOWN.setAttribute("valign", "middle");
	
						td_DOWN.appendChild(img_DOWN);
	
				var img_RIGHT = document.createElement("img");
	
						img_RIGHT.setAttribute("src", main_location+"/images/right.png");
	
//						img_RIGHT.setAttribute("height", "17");
	
						img_RIGHT.style.cssText = "cursor:pointer";
	
						img_RIGHT.setAttribute("onclick", 'right_row("'+i+'")');
	
						
	
				var td_RIGHT = document.createElement("td");
	
						td_RIGHT.setAttribute("id", "right_"+i);
	
						td_RIGHT.setAttribute("valign", "middle");
	
						td_RIGHT.appendChild(img_RIGHT);
	
						
	
				var img_LEFT = document.createElement("img");
	
						img_LEFT.setAttribute("src", main_location+"/images/left.png");
	
//						img_LEFT.setAttribute("height", "17");
	
						img_LEFT.style.cssText = "margin:2px;cursor:pointer";
	
						img_LEFT.setAttribute("onclick", 'left_row("'+i+'")');
	
						
	
				var td_LEFT = document.createElement("td");
	
						td_LEFT.setAttribute("id", "left_"+i);
	
						td_LEFT.setAttribute("valign", "middle");
	
						td_LEFT.appendChild(img_LEFT);
	
	
				var img_EDIT = document.createElement("img");
	
						img_EDIT.setAttribute("src", main_location+"/images/edit.png");
	
//						img_EDIT.setAttribute("height", "17");
	
						img_EDIT.style.cssText = "margin:2px;cursor:pointer";
	
						img_EDIT.setAttribute("onclick", 'edit("'+i+'")');
	
						
	
				var td_EDIT = document.createElement("td");
	
						td_EDIT.setAttribute("id", "edit_"+i);
	
						td_EDIT.setAttribute("valign", "middle");
	
						td_EDIT.appendChild(img_EDIT);
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
				if(document.getElementById('edit_for_label_position_top'))
	
					if(document.getElementById('edit_for_label_position_top').checked)
	
					{
						var add1 = document.getElementById(i+'_label_section');
	
						var add2 = document.getElementById(i+'_element_section');
	
							
	
							tr.appendChild(add1);
	
							tr.appendChild(add2);
	
					}
	
					else	
	
					{
	
						var td1 = document.createElement('td');
	
							td1.setAttribute("colspan", "2");
	
							td1.setAttribute("id", i+'_label_and_element_section');
	
						var add = document.getElementById(i+'_elemet_table');
	
								
	
							td1.appendChild(add);
	
							tr.appendChild(td1);
	
					}
	
				else
	
					{
						var td1 = document.createElement('td');
		
							td1.setAttribute("colspan", "2");
		
							td1.setAttribute("id", i+'_label_and_element_section');
		
						var add = document.getElementById(i+'_elemet_table');
		
								
		
						td1.appendChild(add);
		
						tr.appendChild(td1);
	
					}
	
	
	
				tr.appendChild(td_X);
	
				tr.appendChild(td_LEFT);
				tr.appendChild(td_UP);
	
				tr.appendChild(td_DOWN);
				tr.appendChild(td_RIGHT);
	
				tr.appendChild(td_EDIT);
	
				j=2;
	
				close_window() ;
	
				refresh_();
				
			call(i,key);
			}
		}
		else
		{	
		i=gen;
		in_lab=false;
		var labels_array=new Array();
		for(w=0; w<gen;w++)
		{	
			if(document.getElementById(w+'_element_label'))
				labels_array.push(document.getElementById(w+'_element_label').innerHTML);
		}			
		for(t=0; t<labels_array.length;t++)
		{	
		if(document.getElementById(i+'_element_label').innerHTML==labels_array[t])
			{
				in_lab=true;
				break;
			}
		}
		if(in_lab)
		{
			alert('Sorry, the labels must be unique.');
		}
		else
		{
			
			
			gen++;
			
			if(document.getElementById('form_view').firstChild.nodeType==3)
				table=document.getElementById('form_view').childNodes[1].childNodes[1].childNodes[1].childNodes[1];
			else
				table=document.getElementById('form_view').firstChild.firstChild.firstChild.firstChild;
			
			
			var tr = document.createElement('tr');
				tr.setAttribute("id", i);
				tr.setAttribute("type", type);

		
			var select_ = document.getElementById('sel_el_pos');
			var option = document.createElement('option');
				option.setAttribute("id", i+"_sel_el_pos");
				option.setAttribute("value", i);
				option.innerHTML=document.getElementById(i+'_element_label').innerHTML;

			var img_X = document.createElement("img");
					img_X.setAttribute("src", main_location+"/images/delete_el.png");
//					img_X.setAttribute("height", "17");
					img_X.style.cssText = "cursor:pointer; margin-left:30px";
					img_X.setAttribute("onclick", 'remove_row("'+i+'")');
					
			var td_X = document.createElement("td");
					td_X.setAttribute("id", "X_"+i);
					td_X.setAttribute("valign", "middle");
					td_X.setAttribute("align", "right");
					td_X.appendChild(img_X);
//image pah@
			var img_UP = document.createElement("img");
					img_UP.setAttribute("src", main_location+"/images/up.png");
//					img_UP.setAttribute("height", "17");
					img_UP.style.cssText = "cursor:pointer";
					img_UP.setAttribute("onclick", 'up_row("'+i+'")');
					
			var td_UP = document.createElement("td");
					td_UP.setAttribute("id", "up_"+i);
					td_UP.setAttribute("valign", "middle");
					td_UP.appendChild(img_UP);
					
			var img_DOWN = document.createElement("img");
					img_DOWN.setAttribute("src", main_location+"/images/down.png");
//					img_DOWN.setAttribute("height", "17");
					img_DOWN.style.cssText = "margin:2px;cursor:pointer";
					img_DOWN.setAttribute("onclick", 'down_row("'+i+'")');
					
			var td_DOWN = document.createElement("td");
					td_DOWN.setAttribute("id", "down_"+i);
					td_DOWN.setAttribute("valign", "middle");
					td_DOWN.appendChild(img_DOWN);
					
			var img_RIGHT = document.createElement("img");
					img_RIGHT.setAttribute("src", main_location+"/images/right.png");
//					img_RIGHT.setAttribute("height", "17");
					img_RIGHT.style.cssText = "cursor:pointer";
					img_RIGHT.setAttribute("onclick", 'right_row("'+i+'")');
					
			var td_RIGHT = document.createElement("td");
					td_RIGHT.setAttribute("id", "right_"+i);
					td_RIGHT.setAttribute("valign", "middle");
					td_RIGHT.appendChild(img_RIGHT);
					
			var img_LEFT = document.createElement("img");
					img_LEFT.setAttribute("src", main_location+"/images/left.png");
//					img_LEFT.setAttribute("height", "17");
					img_LEFT.style.cssText = "margin:2px;cursor:pointer";
					img_LEFT.setAttribute("onclick", 'left_row("'+i+'")');
					
			var td_LEFT = document.createElement("td");
					td_LEFT.setAttribute("id", "left_"+i);
					td_LEFT.setAttribute("valign", "middle");
					td_LEFT.appendChild(img_LEFT);
					
			var img_EDIT = document.createElement("img");
					img_EDIT.setAttribute("src", main_location+"/images/edit.png");
//					img_EDIT.setAttribute("height", "17");
					img_EDIT.style.cssText = "margin:2px;cursor:pointer";
					img_EDIT.setAttribute("onclick", 'edit("'+i+'")');
					
			var td_EDIT = document.createElement("td");
					td_EDIT.setAttribute("id", "edit_"+i);
					td_EDIT.setAttribute("valign", "middle");
					td_EDIT.appendChild(img_EDIT);

///////////////////////////////////////////////////////////////////////////////////////////////

			
			if(document.getElementById('edit_for_label_position_top'))
				if(document.getElementById('edit_for_label_position_top').checked)
				{
					var add1 = document.getElementById(i+'_label_section');
					var add2 = document.getElementById(i+'_element_section');
						
						tr.appendChild(add1);
						tr.appendChild(add2);
				}
				else	
				{
					
					var td1 = document.createElement('td');
						td1.setAttribute("colspan", "2");
						td1.setAttribute("id", i+'_label_and_element_section');
					var add = document.getElementById(i+'_elemet_table');
							
						td1.appendChild(add);
						tr.appendChild(td1);
				}
			else	
			{
				var td1 = document.createElement('td');
					td1.setAttribute("colspan", "2");
					td1.setAttribute("id", i+'_label_and_element_section');
				var add = document.getElementById(i+'_elemet_table');
						
					td1.appendChild(add);
					tr.appendChild(td1);
			}
			tr.appendChild(td_X);
			tr.appendChild(td_LEFT);
			tr.appendChild(td_UP);
			tr.appendChild(td_DOWN);
			tr.appendChild(td_RIGHT);
			tr.appendChild(td_EDIT);
			if(document.getElementById('pos_end').checked)
			{
				table.appendChild(tr);
			}
			if(document.getElementById('pos_begin').checked)
			{	
				table.insertBefore(tr, table.firstChild);
			}
			if(document.getElementById('pos_before').checked)
			{
				beforeTr=document.getElementById(document.getElementById('sel_el_pos').value);
				table=beforeTr.parentNode;
				beforeOption=document.getElementById(document.getElementById('sel_el_pos').value+'_sel_el_pos');
				table.insertBefore(tr, beforeTr);
				select_.insertBefore(option, beforeOption);
			}
			j=2;
			close_window() ;
			refresh_();
		call(i,key);
		
		}
	}	
	}
		else
		{
			alert("The field label is required.");
		}
	}			
	else alert("Please select an element to add.");

}

function call(i,key)
{
if(key==0)
{
edit(i);
add('1');
}
}
function edit(id)
{
	enable2();
	document.getElementById('editing_id').value=id;
	type=document.getElementById(id).getAttribute('type');
	//////////////////////////////parameter take
	if(document.getElementById(id+'_element_label').innerHTML)
		w_field_label=document.getElementById(id+'_element_label').innerHTML;
	
	if(document.getElementById(id+'_label_and_element_section'))
		w_field_label_pos="top";
	else
		w_field_label_pos="left";
		
	if(document.getElementById(id+'_hor'))
		w_flow="hor"
		
	else
		w_flow="ver";
	if(document.getElementById(id+"_element"))
	{
		s=document.getElementById(id+"_element").style.width;
		 w_size=s.substring(0,s.length-2);
		s=document.getElementById(id+"_element").style.height;
		 w_size_h=s.substring(0,s.length-2);
	}
	if(document.getElementById(id+"_element"))
	{
		w_first_val=document.getElementById(id+"_element").value;
		w_title=document.getElementById(id+"_element").title;
	}
	
		
	k=0;
	
	w_choices=new Array();	
	w_choices_checked=new Array();
	w_choices_disabled=new Array();
	
	t=0;
	if(type=="type_checkbox" || type=="type_radio" )
	{
		v=0;
		for(k=0;k<100;k++)
			if(document.getElementById(id+"_element"+k))
			{
				w_choices[t]=document.getElementById(id+"_element"+k).value;
				w_choices_checked[t]=document.getElementById(id+"_element"+k).checked;
				t++;
				v=k;
			}
			atrs=return_attributes(id+'_element'+v);
			w_attr_name=atrs[0];
			w_attr_value=atrs[1];
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(type=="type_text" || type=="type_password" || type=="type_textarea"  || type=="type_submitter_mail" || type=="type_file_upload" || type=="type_country"  || type=="type_own_select"  || type=="type_map" || type=="type_hidden" )
	
	{
		atrs=return_attributes(id+'_element');
		w_attr_name=atrs[0];
		w_attr_value=atrs[1];
	}
	
	if(type=="type_name")
	
	{
		s=document.getElementById(id+"_element_first").style.width;
		w_size=s.substring(0,s.length-2);
		atrs=return_attributes(id+'_element_first');
		w_attr_name=atrs[0];
		w_attr_value=atrs[1];
	}
	
	if(type=="type_own_select")
	{
		for(k=0;k<100;k++)
			if(document.getElementById(id+"_option"+k))
			{
				w_choices[t]=document.getElementById(id+"_option"+k).innerHTML;
				w_choices_checked[t]=document.getElementById(id+"_option"+k).selected;
				if(document.getElementById(id+"_option"+k).value=="")
					w_choices_disabled[t]=true;
				else
					w_choices_disabled[t]=false;
				t++;
			}
	}
	
	
	if(document.getElementById(id+"_send"))
	  
	  	w_send=document.getElementById(id+"_send").value;
		
	if(document.getElementById(id+"_required"))
	  
	  	w_required=document.getElementById(id+"_required").value;
		
	if(document.getElementById(id+"_destination"))
	  	{
		// w_destination=str_replace("***destinationverj"+id+"***", "", str_replace("***destinationskizb"+id+"***", "", document.getElementById(id+"_destination").value));
		 w_destination=document.getElementById(id+"_destination").value.replace("***destinationverj"+id+"***", "").replace("***destinationskizb"+id+"***", "");
		 w_extension  =document.getElementById(id+"_extension").value.replace("***extensionverj"+id+"***", "").replace("***extensionskizb"+id+"***", "");
		 w_max_size   =document.getElementById(id+"_max_size").value.replace("***max_sizeverj"+id+"***", "").replace("***max_sizeskizb"+id+"***", "");
		 }

	if(type=="type_captcha")
	{
		w_digit=document.getElementById("wd_captcha").getAttribute("digit");
		atrs=return_attributes('wd_captcha');
		w_attr_name=atrs[0];
		w_attr_value=atrs[1];
	}
	if(document.getElementById(id+'_element_middle'))
		w_name_format="extended";
	else
		w_name_format="normal";
		
	if(type=="type_time")
	{
		atrs=return_attributes(id+'_hh');
		w_attr_name=atrs[0];
		w_attr_value=atrs[1];
		w_hh=document.getElementById(id+'_hh').value;
		w_mm=document.getElementById(id+'_mm').value;
		if(document.getElementById(id+'_ss'))
		{
			w_ss=document.getElementById(id+'_ss').value;
			w_sec="1";
		}
		else
		{
			w_ss="";
			w_sec="0";
		}
		if(document.getElementById(id+'_am_pm_select'))
		{
			w_am_pm=document.getElementById(id+'_am_pm').value;
			w_time_type="12";
		}
		else
		{
			w_am_pm=0;
			w_time_type="24";
		}
	}
	if(document.getElementById(id+'_label_section'))
	{
		w_class=document.getElementById(id+'_label_section').getAttribute("class");
		if(!w_class)
			w_class="";
	}
	if(type=="type_date")
	{	
		atrs=return_attributes(id+'_element');
		w_attr_name=atrs[0];
		w_attr_value=atrs[1];
		w_date=document.getElementById(id+'_element').value;
		w_format=document.getElementById(id+'_button').getAttribute("format");
		w_but_val=document.getElementById(id+'_button').value;
	}
	
	if(type=="type_date_fields")
	{	
		atrs			=return_attributes(id+'_day');
		w_attr_name		=atrs[0];
		w_attr_value	=atrs[1];
		w_day			=document.getElementById(id+'_day').value;
		w_month			=document.getElementById(id+'_month').value;
		w_year			=document.getElementById(id+'_year').value;
		w_day_type		=document.getElementById(id+'_day').tagName;
		w_month_type	=document.getElementById(id+'_month').tagName;
		w_year_type		=document.getElementById(id+'_year').tagName;
		w_day_label		=document.getElementById(id+'_day_label').innerHTML;
		w_month_label	=document.getElementById(id+'_month_label').innerHTML;
		w_year_label	=document.getElementById(id+'_year_label').innerHTML;
		
		s				=document.getElementById(id+'_day').style.width;
		w_day_size		=s.substring(0,s.length-2);
		
		s				=document.getElementById(id+'_month').style.width;
		w_month_size	=s.substring(0,s.length-2);
		
		s				=document.getElementById(id+'_year').style.width;
		w_year_size		=s.substring(0,s.length-2);
		
		if(w_year_type=='SELECT')
		{
			w_from			=document.getElementById(id+'_year').getAttribute('from');
			w_to			=document.getElementById(id+'_year').getAttribute('to');
		}
		else
		{
			w_from			='1901';
			w_to			='2012';
		}
		w_divider		=document.getElementById(id+'_separator1').innerHTML;
		//w_format		=document.getElementById(id+'_button').getAttribute("format");
	}
	
	if(type=="type_editor")
	{
		w_editor=document.getElementById(id+"_element_section").innerHTML;
	}

	if(type=="type_hidden")
	{
		w_value  = document.getElementById(id+"_element").value;
		w_name  = document.getElementById(id+"_element").name;
	}
	
	if(type=="type_map")
	{
		w_info  = document.getElementById(id+"_element").getAttribute("info");
		w_long  = document.getElementById(id+"_element").getAttribute("long");
		w_lat   = document.getElementById(id+"_element").getAttribute("lat");
		w_zoom  = document.getElementById(id+"_element").getAttribute("zoom");
	  	w_width = parseInt(document.getElementById(id+"_element").style.width);
	  	w_height= parseInt(document.getElementById(id+"_element").style.height);
	}
	
	if(type=="type_submit_reset")
	{
		atrs=return_attributes(id+'_element_submit');
		w_act=!(document.getElementById(id+"_element_reset").style.display=="none");
		w_attr_name=atrs[0];
		w_attr_value=atrs[1];
		w_submit_title = document.getElementById(id+"_element_submit").value;
		w_reset_title  = document.getElementById(id+"_element_reset").value;
	}
	if(type=="type_button" )
	{
		w_title	=new Array();	
	
		w_func	=new Array();
		t=0;
		v=0;
		for(k=0;k<100;k++)
			if(document.getElementById(id+"_element"+k))
			{
				w_title[t]=document.getElementById(id+"_element"+k).value;
				w_func[t]=document.getElementById(id+"_element"+k).getAttribute("onclick");
				t++;
				v=k;
			}
		atrs=return_attributes(id+'_element'+v);
		w_attr_name=atrs[0];
		w_attr_value=atrs[1];
	}
	//document.getElementById(id).innerHTML='';
	
	switch(type)
		{
			case 'type_editor':
			{
				type_editor(id, w_editor); break;
			}
			case 'type_text':
			{
				type_text(id, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_class,  w_attr_name, w_attr_value); break;
			}
			case 'type_password':
			{
				type_password(id, w_field_label, w_field_label_pos, w_size, w_required, w_class, w_attr_name, w_attr_value); break;
			}
			case 'type_textarea':
			{
				type_textarea(id, w_field_label, w_field_label_pos, w_size, w_size_h, w_first_val, w_title, w_required, w_class, w_attr_name, w_attr_value); break;
			}
			case 'type_name':
			{
				type_name(id, w_field_label, w_field_label_pos, w_size, w_name_format, w_required, w_class, w_attr_name, w_attr_value); break;
			}
			case 'type_submitter_mail':
			{
				type_submitter_mail(id, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_send, w_required, w_class, w_attr_name, w_attr_value); break;
			}
			case 'type_checkbox':
			{	
				type_checkbox(id, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_class, w_attr_name, w_attr_value); break;
			}
			case 'type_radio':
			{	
				type_radio(id, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_class, w_attr_name, w_attr_value); break;
			}
			case 'type_time':
			{	
				type_time(id, w_field_label, w_field_label_pos, w_time_type, w_am_pm, w_sec, w_hh, w_mm, w_ss, w_required, w_class, w_attr_name, w_attr_value); break;
			}
			case 'type_date':
			{	
				type_date(id, w_field_label, w_field_label_pos, w_date, w_required, w_class, w_format, w_but_val, w_attr_name, w_attr_value); break;
			}
			case 'type_date_fields':
			{	
				type_date_fields(id, w_field_label, w_field_label_pos, w_day, w_month, w_year, w_day_type, w_month_type, w_year_type, w_day_label, w_month_label, w_year_label, w_day_size, w_month_size, w_year_size, w_required, w_class, w_from, w_to, w_divider, w_attr_name, w_attr_value); break;
			}
			case 'type_own_select':
			{	
				type_own_select(id, w_field_label, w_field_label_pos, w_size, w_choices, w_choices_checked, w_required, w_class, w_attr_name, w_attr_value, w_choices_disabled); break;
			}
			case 'type_country':
			{	
				type_country(id, w_field_label, w_field_label_pos, w_size, w_required, w_class,  w_attr_name, w_attr_value); break;
			}
			case 'type_file_upload':
			{
			}
			case 'type_captcha':
			{
				type_captcha(id, w_field_label, w_field_label_pos, w_digit, w_class,  w_attr_name, w_attr_value); break;
			}
			case 'type_map':
			{
				type_map(id, w_long, w_lat, w_zoom, w_width, w_height, w_class, w_info, w_attr_name, w_attr_value); break;
			}
			case 'type_submit_reset':
			{
				type_submit_reset(id, w_submit_title , w_reset_title , w_class, w_act, w_attr_name, w_attr_value); break;
			}

			case 'type_button':
			{
				type_button (id, w_title , w_func , w_class,w_attr_name, w_attr_value); break;
			}
			case 'type_hidden':
			{
				type_hidden (id, w_name, w_value , w_attr_name, w_attr_value); break;
			}

		}

	var pos=document.getElementsByName("el_pos");
	pos[0].setAttribute("disabled", "disabled");
	pos[1].setAttribute("disabled", "disabled");
	pos[2].setAttribute("disabled", "disabled");
}

function refresh_()
{
	document.getElementById('form').value=document.getElementById('take').innerHTML;
	document.getElementById('counter').value=gen;

}
