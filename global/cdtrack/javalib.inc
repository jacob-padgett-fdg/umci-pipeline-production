<?$javalib="1";?>

<script>

function lookupdelconf (desturl) {
	if (confirm ("Are you sure, Records using this entry may give false information!")) {
		document.location=desturl;
		}

	}

function delbuttonconf (desturl) {
	if (confirm ("Are you sure, Records using this entry may give false information!")) {
		document.deletebutton.submit();
		}

	}

function editwin() {
	open("index.html?mode=blank","contact_edit","height=350,width=300,alwaysRaised,dependent,resizable"); 
	document.contactlist.submit();
	}

function neweditwin() {
	open("index.html?mode=blank","contact_edit","height=350,width=300,alwaysRaised,dependent,resizable"); 
	document.newcontact.submit();
	}

</script>
