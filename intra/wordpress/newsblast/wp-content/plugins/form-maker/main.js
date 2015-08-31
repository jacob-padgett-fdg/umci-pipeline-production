F=2;//choices id
var c;
var a=new Array();


function set_select(select_)
{
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

function check_isnum(e)
{
   	var chCode1 = e.which || e.keyCode;
    	if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57))
        return false;
	return true;
}

function captcha_refresh(id)
{
	srcArr=document.getElementById(id).src.split("&r=");
	document.getElementById(id).src=srcArr[0]+'&r='+Math.floor(Math.random()*100);
	document.getElementById(id+"_input").value='';
	document.getElementById(id).style.display="block";
}

function set_checked(id){}

function set_default(id, j){}

function add_0(id)
{
	input=document.getElementById(id);
	if(input.value.length==1)
	{
		input.value='0'+input.value;
		input.setAttribute("value", input.value);
	}
}

function change_hour(ev, id,hour_interval)
{
	if(check_hour(ev, id,hour_interval))
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

		input.setAttribute("value", input.title);
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



function change_file_value(destination, id)
{	
	input=document.getElementById(id);
	input.setAttribute("value", destination);
}

function change_label(id, label)
{
	document.getElementById(id).innerHTML=label;
	document.getElementById(id).value=label;
}

function change_in_value(id, label)
{
	document.getElementById(id).setAttribute("value", label);
}

function destroyChildren(node)
{
  while (node.firstChild)
      node.removeChild(node.firstChild);
}