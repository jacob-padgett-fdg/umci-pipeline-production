document.fd_menus_positioned_already=0;

function fix_fd_height()
{
    var x,y;
	if (self.innerHeight)
    {
        // Most Browsers
		x = self.innerWidth;
		y = self.innerHeight;
		document.getElementById('fd_main_application_window').style.height=(y - 5) + 'px';
	}
    else if (document.documentElement && document.documentElement.clientHeight)
    {
		// IE6
		x = document.documentElement.clientWidth;
		y = document.documentElement.clientHeight;
	}
	
    apptable=document.getElementById('sub_application_main_table');
}

//////////////////////////////////
// This is the current name for
// the cookie function, but it
// should be renamed to have batch
// at the begining.. so it's duped
// below with the new correct name
//////////////////////////////////
function add_doc_to_batch(doc_id,obj) {
    if (obj.style.background!='')
        obj.style.background='';
    else
        obj.style.background='black';
    
    ajaxManager('load_page','/global/front_desk/?mode=fd_doc_queue_main&add_doc_id=' + doc_id,'fd_doc_queue');
}
	
function batch_add_doc(doc_id,obj)
{
	if (obj.style.background!='')
        obj.style.background='';
	else
        obj.style.background='black';
		
    ajaxManager('load_page','/global/front_desk/?mode=fd_doc_queue_main&add_doc_id=' + doc_id,'fd_doc_queue');
}

function batch_move_doc_up(doc_id)
{
	ajaxManager('load_page','/global/front_desk/?mode=fd_doc_queue_main&move_up_doc_id=' + doc_id,'fd_doc_queue');
}

function batch_move_doc_down(doc_id)
{
	ajaxManager('load_page','/global/front_desk/?mode=fd_doc_queue_main&move_down_doc_id=' + doc_id,'fd_doc_queue');
}

function open_job_search()
{
	open('/global/front_desk/?mode=job_search','job_search','width=400,height=600,resizable=1,scrollbars=1,top=0,left=300');
}

function show_document_attachments(doc_id)
{
	open('/global/front_desk/?mode=fd_doc_attachments_show&doc_id=' + doc_id,'doc_attachments' + doc_id,'width=800,height=700,scrollbars=1,resizable=1');
}

function show_document_history(doc_id)
{
	open('/global/front_desk/?mode=fd_doc_history_show&doc_id=' + doc_id,'doc_history' + doc_id,'width=500,height=500,scrollbars=1,resizable=1');
}

function show_my_jobs()
{
	position_menus();
	document.getElementById('my_jobs_drop').style.display='block';
}

function hide_my_jobs()
{
	document.getElementById('my_jobs_drop').style.display='none';
}

function show_recent_jobs()
{
	position_menus();
	document.getElementById('recent_jobs_drop').style.display='block';
}

function hide_recent_jobs()
{
	document.getElementById('recent_jobs_drop').style.display='none';
}

function load_job(jid)
{
	document.location="/global/front_desk/?mode=main&global_jobinfo_id_set=" + jid;
}

function position_menus()
{
	if (document.fd_menus_positioned_already == 1)
        return 0;
	
    document.fd_menus_positioned_already=1;
	////////////////////////////////////////////
	// Set position for 'my_jobs'
	////////////////////////////////////////////
	var x=0;
	var y=0;
	myjobs_button=document.getElementById('my_jobs_button');
	
    while ( myjobs_button != null)
    {
		//alert(myjobs_button.style.height);
		x+= myjobs_button.offsetLeft;
		myjobs_button = myjobs_button.offsetParent;
	}
	
    myjobs_button=document.getElementById('my_jobs_button');
	y=myjobs_button.offsetHeight;
	while ( myjobs_button != null)
    {
		y+= myjobs_button.offsetTop;
		myjobs_button = myjobs_button.offsetParent;
	}
    
	myjobs_div=document.getElementById('my_jobs_drop');
	myjobs_div.style.left = x+"px";
	myjobs_div.style.top = y+"px";

	////////////////////////////////////////////
	// Set position for 'recent_jobs'
	////////////////////////////////////////////
	var x=0;
	var y=0;
	recentjobs_button=document.getElementById('recent_jobs_button');
	while ( recentjobs_button != null)
    {
		x+= recentjobs_button.offsetLeft;
		recentjobs_button = recentjobs_button.offsetParent;
	}
	recentjobs_button=document.getElementById('recent_jobs_button');
	y=recentjobs_button.offsetHeight;
	
    while ( recentjobs_button != null)
    {
		y+= recentjobs_button.offsetTop;
		recentjobs_button = recentjobs_button.offsetParent;
	}
	
    recentjobs_div=document.getElementById('recent_jobs_drop');
	recentjobs_div.style.left = x+"px";
	recentjobs_div.style.top = y+"px";
}

function findPos(obj)
{
    var curleft = curtop = 0;
    if (obj.offsetParent)
    {
        curleft = obj.offsetLeft;
        curtop = obj.offsetTop;
        while (obj = obj.offsetParent)
        {
            curleft += obj.offsetLeft
            curtop += obj.offsetTop
        }
    }
    return [curleft,curtop];
}

var my_tasks_menu_status=0;
function my_tasks_menu_hold()
{
    my_tasks_menu_status = my_tasks_menu_status + 1;
    //my_tasks_menu_status = 1;
}

function my_tasks_menu_release()
{
    my_tasks_menu_status = my_tasks_menu_status - 1;
    setTimeout("my_tasks_menu_close_test()",100)
}

function my_tasks_menu_close_test()
{
    if (my_tasks_menu_status < 1)
    {
        document.getElementById('my_pal_details_div').style.display='none';
        document.getElementById('my_task_items_div').style.display='none';
        document.getElementById('my_tasks').style.display='none';
        //rootwin.style.display='none';
    }
}

function toggle_my_tasks_div(jobinfo_id)
{
    rootwin=document.getElementById('my_tasks');
    if (rootwin.style.display!='none')
    {
        document.getElementById('my_pal_details_div').style.display='none';
        document.getElementById('my_task_items_div').style.display='none';
        rootwin.style.display='none';
        return 0;
    }
    
    button=document.getElementById('my_tasks_nav_button');
    buttonpos=findPos(button);
    rootwin.ajax_trigger='never';
    button.ajax_trigger='never';

    rootwin.style.left=buttonpos[0] + button.offsetWidth + 'px';
    rootwin.style.top=buttonpos[1] + (button.offsetHeight / $height_drop_factor) + 'px';
    rootwin.innerHtml='';
    ajaxManager('load_page','/global/front_desk/pal/?mode=my_task_apps&jobinfo_id=' + jobinfo_id,'my_tasks');
    rootwin.style.display='block';
}
    
function toggle_my_task_items(jobinfo_id,doc_type,parentwin,mode)
{
    abort=0;
    rootwin=document.getElementById('my_task_items_div');
    rootwin.ajax_trigger='never';
    if (mode=='on')
    {
        if (rootwin.style.display!='none')
        {
            if (document.getElementById('my_task_items_div').my_data!=doc_type)
            {
                toggle_my_task_items(jobinfo_id,doc_type,parentwin);
            }
            else
            {
                abort=1;
            }
        }
    }

    if (abort<1)
    {
        parentpos=findPos(parentwin);
        if (rootwin.style.display!='none')
        {
            document.getElementById('my_pal_details_div').style.display='none';
            rootwin.style.display='none';
            return 0;
        }
        
        rootwin.style.left=parentpos[0] + parentwin.offsetWidth + 'px';
        rootwin.style.top=parentpos[1] + (parentwin.offsetHeight / $height_drop_factor) + 'px';
        rootwin.innerHTML='';
        document.getElementById('my_task_items_div').my_data=doc_type;
        ajaxManager('load_page','/global/front_desk/pal/?mode=my_task_items&jobinfo_id=' + jobinfo_id + '&doc_type=' + doc_type,'my_task_items_div');
        rootwin.style.display='block';
    }
}


function toggle_my_pal_details(pal_id,parentwin,mode)
{
    rootwin=document.getElementById('my_pal_details_div');
    rootwin.ajax_trigger='never';
    if (mode=='on')
    {
        abort=0;
    
        if (rootwin.style.display!='none')
        {
            if (document.getElementById('my_pal_details_div').my_data!=pal_id)
            {
                //toggle_my_task_items(jobinfo_id,doc_type,parentwin);
                toggle_my_pal_details(pal_id,parentwin);
            }
            else
            {
                abort=1;
            }
        }

    }
    
    if (abort<1)
    {
        parentpos=findPos(parentwin);
        
        if (rootwin.style.display!='none')
        {
            rootwin.style.display='none';
            return 0;
        }
        
        rootwin.style.left=parentpos[0] + parentwin.offsetWidth + 'px';
        rootwin.style.top=parentpos[1] + (parentwin.offsetHeight / $height_drop_factor) + 'px';
        rootwin.innerHTML='';
        document.getElementById('my_pal_details_div').my_data=pal_id;
        ajaxManager('load_page','/global/front_desk/pal/?mode=my_pal_details&pal_id=' + pal_id,'my_pal_details_div');
        rootwin.style.display='block';
    }
}
	
function toggle_project_team(jobinfo_id) {
    pbdiv=document.getElementById('phone_book');
    pbdiv.ajax_trigger='never';
    if (pbdiv.style.display=='none') {
        ajaxManager('load_page','/global/front_desk/?mode=show_team&jobinfo_id=' + jobinfo_id,'phone_book');
        close_menu_divs();
        pbbdiv=document.getElementById('phone_book_button');
        parentpos=findPos(pbbdiv);
        pbdiv.style.top=parentpos[1] + pbbdiv.offsetHeight + 'px';
        pbdiv.style.display='block';
        } else {
        pbdiv.style.display='none';
        pbdiv.innerHTML='';
        }
    }
        
function show_material_phases(jobinfo_id,sub_job,no_close) {
    if(typeof sub_job == 'undefined') {
        sub_job="";
        }
    if(typeof no_close == 'undefined') {
        no_close="";
        }
    outdiv=document.getElementById('material_phases');
    outdiv.ajax_trigger='never';
    if ((outdiv.style.display=='none')||(no_close!='')) {
        ajaxManager('load_page','/global/front_desk/?mode=material_phases&jobinfo_id=' + jobinfo_id + '&sub_job=' + sub_job, 'material_phases');
        close_menu_divs();
        butdiv=document.getElementById('material_phases_button');
        parentpos=findPos(butdiv);
        outdiv.style.top=parentpos[1] + butdiv.offsetHeight + 'px';
        outdiv.style.left=parentpos[0] + 'px';
        outdiv.style.display='block';
        } else {
        outdiv.style.display='none';
        outdiv.innerHTML='';
        }
    }
        
function show_labor_phases(jobinfo_id,sub_job,no_close) {
    if(typeof sub_job == 'undefined') {
        sub_job="";
        }
    if(typeof no_close == 'undefined') {
        no_close="";
        }
    outdiv=document.getElementById('labor_phases');
    outdiv.ajax_trigger='never';
    if ((outdiv.style.display=='none')||(no_close!='')) {
        ajaxManager('load_page','/global/front_desk/?mode=labor_phases&jobinfo_id=' + jobinfo_id + '&sub_job=' + sub_job, 'labor_phases');
        close_menu_divs();
        butdiv=document.getElementById('labor_phases_button');
        parentpos=findPos(butdiv);
        outdiv.style.top=parentpos[1] + butdiv.offsetHeight + 'px';
        outdiv.style.left=parentpos[0] + 'px';
        outdiv.style.display='block';
        } else {
        outdiv.style.display='none';
        outdiv.innerHTML='';
        }
    }
        
function close_menu_divs() {
    document.getElementById('sub_phases').style.display='none';
    document.getElementById('material_phases').style.display='none';
    document.getElementById('labor_phases').style.display='none';
    document.getElementById('phone_book').style.display='none';
    }
        
function show_sub_phases(jobinfo_id) {
    outdiv=document.getElementById('sub_phases');
    outdiv.ajax_trigger='never';
    if (outdiv.style.display=='none') {
        ajaxManager('load_page','/global/front_desk/?mode=sub_phases&jobinfo_id=' + jobinfo_id,'sub_phases');
        close_menu_divs();
        butdiv=document.getElementById('sub_phases_button');
        parentpos=findPos(butdiv);
        outdiv.style.top=parentpos[1] + butdiv.offsetHeight + 'px';
        outdiv.style.left=parentpos[0] + 'px';
        outdiv.style.display='block';
        } else {
        outdiv.style.display='none';
        outdiv.innerHTML='';
        }
}

function create_new_dailylog(pageName)
{
	if (confirm('Create new log now?'))
    {
		document.location= pageName + "?mode=create_log";
	}
}

function contact_view(contact)
{
    open('/global/contacts/contactview.phtml?contacts_id=' + contact,'contact_info_win' + contact,'width=500,height=640,scrollbars=yes,resizable=yes');
}

function cancel_entry(log_id) {
    if (confirm("Cancel Entry?")){
        document.location.href='/global/front_desk/dailylog/dailylog_cancel.php?dailylog_id=' + log_id;
    }
}
    