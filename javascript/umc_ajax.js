function ajaxProc(x,elname,mode) {
    if (elname!='verynull') {
        if (x.readyState == 4 && x.status == 200) {
            if (mode == 'load_js') {
                var getheadTag = document.getElementsByTagName('head')[0];
                setjs = document.createElement('script');
                setjs.setAttribute('type', 'text/javascript');
                getheadTag.appendChild(setjs);
                setjs.text = x.responseText;
            } else {
                if (elname != 'null') {
                    el = document.getElementById(elname);
                    try {
                        el.innerHTML = x.responseText;
                    } catch (e) {
                    }
                    if (mode == 'load_form') {
                        document.getElementById('ajax_form_data_field').focus()
                    }
                }
            }
            if (typeof el != 'undefined' && el != null) {
                for (i = 0; i < el.childNodes.length; i++) {
                    anyobj = el.childNodes[i];
                    if (typeof(anyobj.onload) == 'function') {
                        anyobj.onload();
                    }
                }
                if (typeof(ajax_trigger) == 'function')
                    if (el.ajax_trigger != 'never')
                        ajax_trigger(elname, mode);
                if (el.ajax_trigger_function != '') {
                    eval(el.ajax_trigger_function);
                    el.ajax_trigger_function = '';
                }
            }
        }
    }
}

function ajaxManager() {
var args=ajaxManager.arguments;
if (document.getElementById) {	var x=(window.ActiveXObject)?new ActiveXObject("Microsoft.XMLHTTP"):new XMLHttpRequest(); }
if (x) {
switch (args[0]) {
	case "load_js":
	case "load_page":
	case "load_form":
		x.onreadystatechange=function() { ajaxProc(x,args[2],args[0]); }
		x.open("GET",args[1],true);x.send(null);
		break;
	case "display_info":
		document.getElementById(args[2]).innerHTML=args[1];
		break;
	case "start_up":
		defaultval='load_js';
		filetype=new Array();
		filetype['js']='load_js';filetype['html']='load_page';filetype['css']='load_css';
		count=0;
		while(count < umc_javascript_loadlist.length) {
			t=filetype[umc_javascript_loadlist[count]];
			if (t) defaultval=t;
			else ajaxManager(defaultval,umc_javascript_loadlist[count]);
			count=count+1;
			}
		break;
		}
	}
}
ajaxManager('start_up');
