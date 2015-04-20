<?

///////////////////////////////////////////////////////////////////
/////////////// UMC OPTIONS ADDED TO Cezpdf CLASS /////////////////
///////////////////////////////////////////////////////////////////


function pdf_simple_cover_page($text,$textsize="25") {
	if ($this->this_is_first_page!="no") $this->this_is_first_page="no";
	else $this->ezNewPage();
	$this->ezSetY($this->ez['pageHeight'] / 2);
	$this->ezText("$text",$textsize,array('justification'=>'center'));
	}

function ez_umc_header_start($text="",$fontsize="",$textoptions="",$redo_header=0) {
	if ($fontsize=="") $fontsize=15;
	if ($redo_header!=0) {
		if ($this->ez_umc_letterhead_definde=1) {
			$this->ez_umc_letterhead_defined=0;
			$this->ez_umc_header_stop();
			}
		}
	if ($this->ez_umc_letterhead_defined!="1") {
		
		$header_height=($this->ez['pageHeight'] - ($height=40) - 36);
		$this->ez_umc_letterhead_object_id=$this->openObject();
		$this->addPngFromFile("/home/web/umci.com/php/global/timesheet/umclogo_wbv2.png",36,$header_height,120,$height);
		$this->setLineStyle(1,'butt');
		$this->line($this->ez['leftMargin'],$header_height - 5,$this->ez['pageWidth'] - $this->ez['rightMargin'],$header_height - 5);
		$this->ezSetY($header_height + ($fontsize * 1.1));
		if ($textoptions=="") $textoptions=array('justification'=>'right');
		if ($text!="") $this->ezText($text,$fontsize,$textoptions);
		$this->closeObject();
		$this->ez_umc_letterhead_defined="1";
		}
	////////////////// Puts the object on all pages until the object stop call
	$this->addObject($this->ez_umc_letterhead_object_id,"all");
	}

function ez_umc_header_stop () {
	$this->stopObject($this->ez_umc_letterhead_object_id);
	}

function ezTableDumpStart() {
	$this->tabledata=array();
	}
function ezTableDumpAdd() {
	$to_add=func_get_args();
	array_push($this->tabledata,$to_add);
	}

function ezTableDumpPrint($firstrowheaders=0,$array_options="") {
	$maxwidth=$this->ez['pageWidth'] - $this->ez['leftMargin'] - $this->ez['rightMargin'];
	if ($firstrowheaders==1) $headings=array_shift($this->tabledata);
	$base_array=array('maxWidth'=>"$maxwidth",'showHeadings'=>"$firstrowheaders");
	if ($array_options!="") {
		$array_options=array_merge($base_array,$array_options);
		$this->ezTable($this->tabledata,$headings,"",$array_options);
		} else {
		$this->ezTable($this->tabledata,$headings,"",$base_array);
		}
	if ($firstrowheaders==1) array_unshift($this->tabledata,$headings);
	}

function ezHR($thickness=1) {
	$downbump=round($this->ez['fontSize']/2);
	$rightside=$this->ez['pageWidth'] - $this->ez['rightMargin'];
	
	$this->setlinestyle($thickness);
	$this->ezSetDY("-$downbump");
	$this->line($this->ez['leftMargin'],$this->y,$rightside,$this->y);
	$this->ezSetDY("-$downbump");
	}

function mysql_to_array($res) {
	$data=array();
	while ($newrow=@mysql_fetch_assoc($res)) {
		array_push($data,$newrow);
		}
	return ($data);
	}
                                

///////////////////////////////////////////////////
//	Note, you must be connected to mysql already //
///////////////////////////////////////////////////
function ezTableDump($query,$title="") {
	$res=@mysql_query($query);
	$data=$this->mysql_to_array($res);
	$maxwidth=$this->ez['pageWidth'] - $this->ez['leftMargin'] - $this->ez['rightMargin'];
	$this->ezTable($data,"","$title",array('maxWidth'=>"$maxwidth"));
	}
?>
