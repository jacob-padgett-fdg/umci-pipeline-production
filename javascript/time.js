// B=Border
// BG=Background
// C=Cell
//var _monthNames=new Array("Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec");
//var _calDOW=new Array("S","M","T","W","T","F","S");
//var _calBG="#009ad8";
//var _calBS=3;
//var _calBtnClr="#dddddd";
//var _calBClr="darkblue";
//var _calCC="#88ccff";
//var _calCCLt="#ddffff";
//var _calCCWeekend="#55aacc";
//var _calCCToday='#55ff55';
//var _calCCSelected='#1155ff';
//var _calCellFont="Tahoma";
//var _calCFontSize="11px";
//var _calTxtFont="Tahoma";
//var _calTxtFntSz="11px";
//var _calTxtClr="black";
var _clockwidth=150;
var _clockheight=20;
//var _calDOWBG="#dddddd";
// Don't edit below

function BrowseTime(input_id) {
	//today=new Date();
	//now=new Time();
	hour=getHours();
	minutes=getMinutes();
	
	//var arrParts=strDate=(document.getElementById(input_id).value.replace('-','/')).replace('-','/').split('/');
	
	input=document.getElementById(input_id).value;
	input=input.value.replace("  "," ");
	timesections=input.split(" ");
	ampm=timesections[1];
	ampm=ampm.toLowerCase();
	input_time=timesections[0];
	timeparts=input_time.split(":");
	hour=timeparts[0];
	minutes=timeparts[1];
	
	//OpenCalendar(arrParts[2], arrParts[0], arrParts[1], input_id);
	}


function OpenClock(Year, defMonth, defDay, input_id) {
	if ((typeof Year == "undefined")||(isNaN(Year))) Year = _cal_today_year;
	if ((typeof defMonth == "undefined")||(isNaN(defMonth))||(defMonth=="")) defMonth = _cal_today_month;
		else defMonth--;
	if ((typeof defDay == "undefined")||(isNaN(defDay))||(defDay=="")) defDay = _cal_today_day;
	var inputobj=document.getElementById(input_id);
	var inputimg=document.getElementById(input_id + '_img');
	special_position=inputobj.special_position;
	inputobj._cal_year=Year;
	inputobj._cal_month=defMonth+1;
	inputobj._cal_day=defDay;
	FillCalendar(Year, defMonth+1, defDay, input_id);
	var x=-35;
	x+=inputimg.clientWidth;
	while ( inputimg != null) {
		x+= inputimg.offsetLeft;
		x+=2;
		inputimg = inputimg.offsetParent;
		}
	var y=0;
	var inputobj=document.getElementById(input_id);
	while ( inputobj != null) {
		y+= inputobj.offsetTop;
		inputobj = inputobj.offsetParent;
		}
	if (special_position=="U") y=y - 117;
	var objPanel = document.getElementById('datepanel' + input_id);
	objPanel.style.display = "block";
	document.getElementById(input_id).position="absolute";
	objPanel.style.left = x+"px";
	objPanel.style.top = y+"px";
}

/*
function stdBtn(objRow,func,txt) {
	var cel=stdCell(objRow);
	cel.style.backgroundColor = _calBtnClr;
	cel.onmousedown=eval(func);
	cel.innerHTML=txt;
	return (cel);
	}

function stdCell(objRow) {
	var objCell=objRow.insertCell(objRow.cells.length);
	objCell.setAttribute("width", _calCWidth+"");
	objCell.setAttribute("height", _calCHeight+"");
	objCell.style.border = "1px solid black";
	objCell.style.cursor = "pointer";
	objCell.style.textAlign = "center";
	objCell.style.fontFamily = _calCellFont;
	objCell.style.fontSize = _calCFontSize;
	return (objCell);
	}

function FillCalendar(year, month, day, input_id) {
	var daysCount=DaysInMonth(year, month);
	var objPanel = document.getElementById('datepanel' + input_id);
	var inputobj=document.getElementById(input_id);

	if (!objPanel) {
		objPanel = document.createElement("div");
		objPanel.id = 'datepanel' + input_id;
		objPanel.style.position = "absolute";
		objPanel.style.display = "none";
		objPanel.style.border = _calBS+"px solid "+_calBClr;
		objPanel.style.width = "150px";
		objPanel.style.backgroundColor = _calBG;
		objPanel.input_id=input_id;
		document.body.appendChild(objPanel);
		}
	objPanel._currentYear=year;
	objPanel._currentMonth=month-1;

	while (objPanel.childNodes.length > 0) objPanel.removeChild(objPanel.childNodes[0]);
	
	var currentDay=1;
	var objTable=document.createElement("table");
	objTable.cellPadding='0';
	objTable.cellSpacing='0';
	objTable.border='0';
	objTable.input_id=input_id;

	objRow=objTable.insertRow(objTable.rows.length);

	var objCell=stdBtn(objRow,'ShowToday','T');
	objCell.title='Today';
	var objCell=stdBtn(objRow,'PreviousMonthClick','&lt;');
	var objCell=stdCell(objRow);
	objCell.style.cursor="default";
	objCell.colSpan="3";
	objCell.style.fontWeight='bold';
	objCell.innerHTML = _monthNames[month-1] + " " + year;
	var objCell=stdBtn(objRow,'NextMonthClick','&gt;');
	var objCell=stdBtn(objRow,'HideCalendar','X');
	objCell.title='Close';

	var x=0;
	objRow=objTable.insertRow(objTable.rows.length);
	objRow.style.backgroundColor=_calDOWBG;
	for (x=0; x<7; x++) {
		var objCell=objRow.insertCell(objRow.cells.length);
		objCell.style.textAlign = "center";
		objCell.style.fontFamily = _calCellFont;
		objCell.style.fontSize = _calCFontSize;
		objCell.style.cursor="default";
		objCell.innerHTML = _calDOW[x];
		}
	
	var objRow=0;
	var foundMonthStart=0;
	month_text=_monthfullNames[month];
	f_dom=month_text + " 01, " + year;
	firstDay=new Date(eval('"'+f_dom+'"'));
	month_start_dow=firstDay.getDay();
	current_dow=0;
	while (currentDay <= daysCount) {
		if ((current_dow%7) == 0) {
			objRow=objTable.insertRow(objTable.rows.length);
			current_dow=0;
			}
		for (var i=1; i<=7; i++) {
			if (current_dow==month_start_dow) foundMonthStart=1;
			
			if (foundMonthStart==0) {
				var objCell=stdCell(objRow);
				objCell.style.cursor="default";
				objCell.style.border="0px solid black";
				} else {
				if (currentDay > daysCount) break;
				var objCell=stdCell(objRow);
				if ((current_dow==0)||(current_dow==6))
					objCell.style.backgroundColor=_calCCWeekend;
				else 
					objCell.style.backgroundColor=_calCC;
				if (currentDay==_cal_today_day) 
					if ((month-1)==_cal_today_month) 
						if (year==_cal_today_year) 
							objCell.style.backgroundColor=_calCCToday;
				if (currentDay==inputobj._cal_day)
					if ((month)==inputobj._cal_month) 
						if (year==inputobj._cal_year) 
							objCell.style.backgroundColor=_calCCSelected;
				objCell.style.fontFamily = _calCellFont;
				objCell.style.fontSize = _calCFontSize;
				objCell.style.cursor = "pointer";
				objCell.onmouseover = new Function("HL(this)");
				objCell.onmouseout = new Function("unHL(this)");
				objCell.onclick = new Function("CalendarCellClick(this);");
				objCell.innerHTML = currentDay;
				currentDay++;
				}
			current_dow++;
			}
	}		
	objPanel.appendChild(objTable);
}

function ShowToday() {
	FillCalendar(_cal_today_year, _cal_today_month + 1, '', this.parentNode.parentNode.parentNode.input_id);
	}

function CalendarCellClick(objCell) {
	var id=objCell.parentNode.parentNode.parentNode.input_id;
	HideCalendar(id);
	var date=new Date();
	panelobject=document.getElementById('datepanel' + id);
	date.setFullYear(panelobject._currentYear, panelobject._currentMonth, parseInt(objCell.innerHTML));
	CalendarCallback(date,id);
	}

function HideCalendar(id) {
	if (isNaN(id)) var id=this.parentNode.parentNode.parentNode.input_id;
	var objPanel = document.getElementById('datepanel' + id);
	objPanel.style.display = "none";
	}

function PreviousMonthClick() {
	id=this.parentNode.parentNode.parentNode.input_id;
	panelobject=document.getElementById('datepanel' + id);
	panelobject._currentMonth--;
	if (panelobject._currentMonth < 0) {
		panelobject._currentMonth = 11;
		panelobject._currentYear--;
		}
	FillCalendar(panelobject._currentYear, panelobject._currentMonth+1, 1, id);
	}

function NextMonthClick() {
	id=this.parentNode.parentNode.parentNode.input_id;
	panelobject=document.getElementById('datepanel' + id);
	panelobject._currentMonth++;
	if (panelobject._currentMonth > 11) {
		panelobject._currentMonth = 0;
		panelobject._currentYear++;
		}
	FillCalendar(panelobject._currentYear, panelobject._currentMonth+1, 1,id);
	}

function DaysInMonth(year, month) {
	var date=new Date();
	var result=27;
	date.setFullYear(year, month-1, 28);
	while ((date.getFullYear() <= year)&&(date.getMonth() <= (month-1))) {
		result++;
		if (result > 31) {
			alert("error getting days in month!\nyear: "+year+", month: "+month);
			return 0;
			}
		date.setFullYear(year, month-1, date.getDate()+1);
		}
	return result;
	}

function HL(objControl) {
	objControl.orig_cell_color=objControl.style.backgroundColor;
	objControl.style.backgroundColor = _calCCLt;
	}

function unHL(objControl) {
	objControl.style.backgroundColor=objControl.orig_cell_color;
	}


var _monthfullNames=new Array("NA","January","February","March","April","May","June","July","August","September","October","November","December");
var _cal_today_day=0;
var _cal_today_month=0;
var _cal_today_year=0;
*/



