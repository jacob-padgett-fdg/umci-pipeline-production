function jc(formname,fieldname,query,onchange,settings) {
	var default_id;
	field=formname + '.' + fieldname;
	eval('default_id=document.' + field + '.value');
	field=urlencode(field);
	query=urlencode(query);
	onchange=urlencode(onchange);
	var winrand=Math.round(Math.random() * 100000);
	open("/global/contacts/contact_selection.phtml?query=" + query + "&field=" + field + "&contacts_id=" + default_id + "&onchange=" + onchange + "&settings=" + settings,"contactsbox_window" + winrand,"width=550,height=550,scrollbars=yes");
	}
function jcro(formname,fieldname) {
	var default_id;
	field=formname + '.' + fieldname;
	eval('default_id=document.' + field + '.value');
	open('/global/contacts/contactview.phtml?read_only_warning=yes&contacts_id=' + default_id,'contactsbox_window_info_win' + default_id,'width=550,height=550,scrollbars=yes,resizable=yes');
	}
