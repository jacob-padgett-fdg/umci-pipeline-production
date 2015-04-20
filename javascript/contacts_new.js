
function watch_contact(id) {
	obj1=document.getElementById(id);
	obj2=document.getElementById(id + 'orig_value');
	if (obj1.value!=obj2.value) obj2.onchange();
	else setTimeout("watch_contact('" + id + "')",100);
	}

function jcnew(id,query,onchange,options) {
	default_contact=document.getElementById(id).value;
	query=urlencode(query);
	if (onchange!="") watch_contact(id);
	onchange=urlencode(onchange);
	open("/global/contacts/contact_selector.phtml?query=" + query + "&id=" + id + "&contacts_id=" + default_contact + "&onchange=" + onchange + "&options=" + options + '&teamfirst=Y',"contactsbox_window","width=550,height=550,scrollbars=yes");
	}

function jcronew(id) {
	var default_contact;
	default_contact=document.getElementById(id).value;
	open('/global/contacts/contactview.phtml?read_only_warning=yes&contacts_id=' + default_id,'contactsbox_window_info_win' + default_id,'width=550,height=550,scrollbars=yes,resizable=yes');
	}
