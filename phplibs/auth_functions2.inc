<?
function is_from_intranet() {
	$local_net="10.";
	$address=$GLOBALS['REMOTE_ADDR'];
	return(!(strncmp($address,$local_net,3))); 
	}

function webfile_permissions_check_user($global_user_id) {
	$readable=FALSE;
	$writeable=FALSE;
	
	$global_user_id=addslashes($global_user_id);
	$global_user_info=getoneb("select * from contacts where contacts_id = '$global_user_id'");
	$file_group_id=$GLOBALS['file_group_id'];
	$files_id=$GLOBALS['files_id'];
	
	//////////////////////////////////////////////////
	// If you're here, then you have an account, so 
	// lets see if you get default permissions...
	//////////////////////////////////////////////////
	
	if ($file_group_id!="") {
		$file_group_id=addslashes($file_group_id);
		$object_info=getoneb("select * from webfile_groups where file_group_id = '$file_group_id'");
		$GLOBALS['files_id']="";
		$files_id="";
		} else {
		$files_id=addslashes($files_id);
		$object_info=getoneb("select * from webfile_files where files_id = '$files_id'");
		$GLOBALS['file_group_id']="";
		$file_group_id="";
		}
	if ($object_info) {
		if ($object_info->creator!="") $owner=$object_info->creator;
		if ($object_info->owner!="") $owner=$object_info->owner;
		if ($owner=="") $owner='xxxx';
		if ($owner==0) $owner='xxxx';
		///////////////////////////////////////////////
		///////////////////////////////////////////////
		// OK, we know we're working with a file or a
		// file group.. lets check things out.. 
		///////////////////////////////////////////////
		///////////////////////////////////////////////
		
		
		///////////////////////////////////////////////
		// Is this object open to the public?
		///////////////////////////////////////////////
		if ($object_info->access_public=='Y') $readable=TRUE;
		if ($object_info->write_public=='Y') $writeable=TRUE;

		if ($owner==$global_user_id) {
			$readable=TRUE;
			$writeable=TRUE;
			$owner=TRUE;
			} else {
			$owner=FALSE;
			}
		
		///////////////////////////////////////////////
		// Is this object open to the intranet?
		///////////////////////////////////////////////
		if (!(($readable)&&($writeable))) { 
			$intranet=is_from_intranet(); 
			if ($intranet) {
				if (!($readable)) if ($object_info->access_intranet=='Y') $readable=TRUE;
				if (!($writeable)) if ($object_info->write_intranet=='Y') $writeable=TRUE;
				}
			}
		///////////////////////////////////////////////
		// Is this object open to all "users"?
		// If so, we're in the "users" section, so just
		// give them access if it's specified!
		///////////////////////////////////////////////
		if (!(($readable)&&($writeable))) { 
			if (!($readable)) if ($object_info->access_any_user=='Y') $readable=TRUE;
			if (!($writeable)) if ($object_info->write_any_user=='Y') $writeable=TRUE;
			}

		///////////////////////////////////////////////
		// Is this object open to all "employees"?
		///////////////////////////////////////////////
		if (!(($readable)&&($writeable))) { 
			if (!($readable)) if (($object_info->access_employees=='Y')&&($global_user_info->umc_emp=='Y')) $readable=TRUE;
				if (!($writeable)) if (($object_info->write_employees=='Y')&&($global_user_info->umc_emp=='Y')) $writeable=TRUE;
			}
		}
	
	
	//////////////////////////////////////////////////////////////////////////
	// OK, now default permissions have been determined based on the 
	// general class of the user.. Now we should look up any permits or 
	// denies based on the user or the user's groups (once implimented), 
	// and use them to either escalate, or quash the rights already given.
	//
	// This should be added soon, once the supporting tables are added..
	//////////////////////////////////////////////////////////////////////////
	if ($owner) return 3;
	if ($writeable) return 2; // Should never hit.. should probably be removed now.
	if ($readable) return 1;
	return FALSE;
	}

function webfile_permissions_check_anonymous() {
	$readable=FALSE;
	$writeable=FALSE;
	$file_group_id=$GLOBALS['file_group_id'];
	$files_id=$GLOBALS['files_id'];
	
	//////////////////////////////////////////////////////
	// If they're not trying to look up something specific
	// then just give them a login screen.
	//////////////////////////////////////////////////////
	if (($files_id=="")&&($file_group_id=="")) return (0);

	//////////////////////////////////////////////////////
	// You're definitely trying to look something up...
	//////////////////////////////////////////////////////

	if ($file_group_id!="") {
		$file_group_id=addslashes($file_group_id);
		$object_info=getoneb("select * from webfile_groups where file_group_id = '$file_group_id'");
		} else {
		$files_id=addslashes($files_id);
		$object_info=getoneb("select * from webfile_files where files_id = '$files_id'");
		}
	if (!($object_info)) return FALSE;
		
	///////////////////////////////////////////////
	// Is this object open to the public?
	///////////////////////////////////////////////
	if ($object_info->access_public=='Y') $readable=TRUE;
	if ($object_info->write_public=='Y') $writeable=TRUE;

	///////////////////////////////////////////////
	// Is this object open to the intranet?
	///////////////////////////////////////////////
	if (!(($readable)&&($writeable))) { 
		$intranet=is_from_intranet(); 
		if ($intranet) {
			if (!($readable)) if ($object_info->access_intranet=='Y') $readable=TRUE;
			if (!($writeable)) if ($object_info->write_intranet=='Y') $writeable=TRUE;
			}
		}
	if ($writeable) return 2;
	if ($readable) return 1;
	return FALSE;
	}
?>
