<?php
/**
*		Copyright	: (C) 2006 Geniutt SARL
*		Licence		: GNU GPL
*		Email     : contact@geniutt.fr
*		Version		: 8.0
*
*		This program is free software; you can redistribute it and/or modify
*		it under the terms of the GNU General Public License as published by
*		the Free Software Foundation; either version 2 of the License, or   
*		(at your option) any later version.                                 
*			                                                           
*		This program is distributed in the hope that it will be useful,     
*		but WITHOUT ANY WARRANTY; without even the implied warranty of      
*		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       
*		GNU General Public License for more details.                        
*				                                                           
*		You should have received a copy of the GNU General Public License   
*		along with this program; if not, write to the Free Software         
*		Foundation, Inc., 59 Temple Place, Suite 330, Boston,               
*		MA  02111-1307  USA
*	
*		Created on 6 sept. 2006 by Vincent Keravec                                                 
*
**/

// Include all the class helping the generation process
include (dirname(__FILE__)."/generateClass/generable.interface.php");
include (dirname(__FILE__)."/generateClass/field.interface.php");
include (dirname(__FILE__)."/generateClass/groupField.class.php");
include (dirname(__FILE__)."/generateClass/option.class.php");
include (dirname(__FILE__)."/generateClass/choice.class.php");
include (dirname(__FILE__)."/generateClass/text.class.php");
include (dirname(__FILE__)."/generateClass/date.class.php");
include (dirname(__FILE__)."/generateClass/photo.class.php");

class Generate {
	
	private $childs=array();
	private $secret="";
	//private $rep="";
	//private $relRep="";
	
	public function __construct() {
		$this->secret=sha1(microtime(true));
		$this->childs=array();
		//Load the xml document
		$doc = new DOMDocument();
		if (!$doc->load('trombi.xml') or !$doc->validate()) {
			// return if the document is invalid
			exit ("trombi.xml invalid");
		}
		
		
		// Start the document
		$child=$doc->firstChild;
		while ($child!=null and $child->nodeType!=XML_ELEMENT_NODE) {
			$child=$child->nextSibling;
		}
		
		//Get the main option
		/*
		$this->relRep=$child->getAttribute("outputDir");
		$this->rep=dirname(__FILE__)."/../".$this->relRep;
		
		if (file_exists($this->rep)) {
			if (!is_dir($this->rep)) {
				exit ($this->rep." exists and is not a directory");	
			}
		} else {
			if (!mkdir($this->rep)) {
				exit ("Can't create ".$this->rep);	
			}	
		}
		*/
		
		$child=$child->firstChild;
		
		do {
			//Check the type of node we got
			if ($child->nodeType==XML_ELEMENT_NODE) {
				switch ($child->nodeName) {
					case "GROUPFIELD" :
						$this->childs[]=$this->parseGroupField($child);
						break;
				}
			}
		} while ($child=$child->nextSibling);
	}
	
	private function parseGroupField($child) {
		$groupField = new GroupField (
							$child->getAttribute("text"));
		$lChild=$child->firstChild;
		do {
			//Check the type of node we got
			if ($lChild->nodeType==XML_ELEMENT_NODE) {
				switch ($lChild->nodeName) {
					case "TEXT" :
						$groupField->add($this->parseText($lChild));
						break;
					case "PHOTO" :
						$groupField->add($this->parsePhoto($lChild));
						break;
					case "CHOICE" :
						$groupField->add($this->parseChoice($lChild));
						break;
					case "DATE" :
						$groupField->add($this->parseDate($lChild));
						break;
				}
			}
		} while ($lChild=$lChild->nextSibling);
		return $groupField;
	}
	
	private function parseText($child) {
		$text=new Text (
						$child->getAttribute("id"),
						$child->getAttribute("text"));
		if ($child->hasAttribute("default")) {
			$text->setDefault($child->getAttribute("default"));	
		}
		if ($child->hasAttribute("maxSize")) {
			$text->setMaxSize($child->getAttribute("maxSize"));	
		}
		if ($child->hasAttribute("searchable")) {
			$text->setSearchable($child->getAttribute("searchable"));	
		}
		if ($child->hasAttribute("inFastSearch")) {
			$text->setInFastSearch($child->getAttribute("inFastSearch"));	
		}
		if ($child->hasAttribute("nbLigne")) {
			$text->setNbLigne($child->getAttribute("nbLigne"));	
		}
		if ($child->hasAttribute("extraText")) {
			$text->setExtraText($child->getAttribute("extraText"));
		}
		
		if ($child->firstChild!=null) {
			$lChild=$child->firstChild;
			do {
				//Check the type of node we got
				if ($lChild->nodeType==XML_ELEMENT_NODE) {
					switch ($lChild->nodeName) {
						case "REGEXPTEST" :
							$text->setRegExpTest($lChild->getAttribute("test"));
							$text->setRegExpError($lChild->getAttribute("error"));
							break;
					}
				}
			} while ($lChild=$lChild->nextSibling);
		}
		
		return $text;
	}
	
	private function parseDate($child) {
		
		$date=new TrombiGenDate (
						$child->getAttribute("id"),
						$child->getAttribute("text"));
		if ($child->hasAttribute("searchable")) {
			$date->setSearchable($child->getAttribute("searchable"));	
		}
		if ($child->hasAttribute("inFastSearch")) {
			$date->setInFastSearch($child->getAttribute("inFastSearch"));	
		}
		if ($child->hasAttribute("selectWith")) {
			$date->setSelectWith($child->getAttribute("selectWith"));	
		}
		if ($child->hasAttribute("extraText")) {
			$date->setExtraText($child->getAttribute("extraText"));
		}
		echo $date->toString();
		return $date;
	}
	
	private function parsePhoto($child) {
		$photo=new Photo (
						$child->getAttribute("id"),
						$child->getAttribute("text"));
		if ($child->hasAttribute("path")) {
			$photo->setPath($child->getAttribute("path"));
		}
		if ($child->hasAttribute("maxSize")) {
			$photo->setMaxSize($child->getAttribute("maxSize"));
		}
		if ($child->hasAttribute("imageSize")) {
			$photo->setImageSize($child->getAttribute("imageSize"));
		}
		return $photo;
	}
	
	private function parseChoice($child) {
		$choice= new Choice (
						$child->getAttribute("id"),
						$child->getAttribute("text"));
		if ($child->hasAttribute("multipleChoice")) {
			$choice->setMultipleChoice($child->getAttribute("multipleChoice"));
		}
		
		if ($child->firstChild!=null) {
			$lChild=$child->firstChild;
			do {
				//Check the type of node we got
				if ($lChild->nodeType==XML_ELEMENT_NODE) {
					switch ($lChild->nodeName) {
						case "OPTION" :
							$choice->addOption(
									$lChild->getAttribute("id"),
									$lChild->getAttribute("value"));
							break;
						case "OPTIONDB" :
							$choice->setOptionDb(
									$lChild->getAttribute("query"),
									$lChild->getAttribute("maxSizeOptionId"));
							break;
					}
				}
			} while ($lChild=$lChild->nextSibling);
		}
		return $choice;
	}

	public function sql() {
		if (file_exists(dirname(__FILE__)."/sql")) {
			if (!is_dir(dirname(__FILE__)."/sql")) {
				exit (dirname(__FILE__)."/sql exist and is not a directory");
			}
		}else {
			if (!mkdir(dirname(__FILE__)."/sql")) {
				exit ("Can't create ".dirname(__FILE__)."/sql");	
			}	
		}
		$sql="DROP TABLE IF EXISTS `trombi_trombi`;\n";
		$sql.="CREATE TABLE `trombi_trombi` (\n";
		$sql.="  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT, \n";
		$sql.="  `id_user` INT UNSIGNED NOT NULL, \n";
		$extraSql="";
		foreach ($this->childs as $child) {
			$extraSqlChild="";
			$sql.=$child->sql($extraSqlChild);
			$extraSql.=$extraSqlChild;
		}
		
		$sql.="  PRIMARY KEY  (`id`)\n,";
		$sql.="  INDEX  (`id_user`)\n,";
		$sql.="	 FOREIGN KEY (`id_user`) REFERENCES `usersInfo` (`id`) \n";
		$sql.="	 ON DELETE CASCADE \n";
		$sql.="	 ON UPDATE CASCADE \n";
		$sql.=") ENGINE = InnoDB CHARACTER SET utf8 COLLATE utf8_general_ci;\n";
		$sql.="\n".$extraSql;
		echo "<h2>Trombi.sql</h2>";
		echo nl2br($sql);
		file_put_contents(dirname(__FILE__)."/sql/trombi.sql",$sql);
		$cleanDb="";
		if (preg_match_all("/^(DROP.*)/m", $sql, $temp)) {
			foreach($temp[1]as $dropInstruction) {
				$cleanDb.=$dropInstruction."\n";
			}	
		}
		echo "<h2>Trombi.clean.sql</h2>";
		echo nl2br($cleanDb);
		file_put_contents(dirname(__FILE__)."/sql/trombi.clean.sql",$cleanDb);
		$sql="-- Ce fichier doit être modifié pour correspondre aux demandes client.\n";
		$sql.="-- Changer les modify en add si le champ n'existe pas déjà chez le client\n";
		$sql.="-- Effacer aussi ces lignes avant d'envoyer le fichier ;)\n";
		$sql.="SELECT 'A modifier';\n";
		$sql.="quit;\n";
		$sql.="-- START \n";
		$sql.="ALTER TABLE `trombi_trombi` \n";
		$extraSql="";
		
		foreach ($this->childs as $child) {
			$extraSqlChild = '';
			$sql .= $child->updateSql($extraSqlChild);
			$extraSql .= $extraSqlChild;
		}
		
		$sql = substr($sql,0,-3);
		$sql .= " ; \n".$extraSql;
		echo '<h2>Trombi_update.sql</h2>';
		echo nl2br($sql);
		file_put_contents(dirname(__FILE__).'/sql/update.sql',$sql);
		
	}
	
	public function createModify() {
		$createModify = file_get_contents(dirname(__FILE__).'/templates/createModify.inc');
		/*
		$temps=explode("/",$this->relRep);
		$include_path="";
		foreach($temps as $temp) {
			if ($temp!="") {
				$include_path.="../";	
			}	
		}
		*/
		
		//Validation test
		$validate = '';
		foreach( $this->childs as $child ){
			$validate .= $child->createModifyValidate();
		}
		
		//Error posting
		$er = '';
		foreach( $this->childs as $child ){
			$er .= $child->createModifyError();
		}

		//Form creation
		$form = '';
		foreach( $this->childs as $child ){
			$form .= $child->createModifyForm();
		}
		
		//Sql update
		$sql = '';
		foreach($this->childs as $child) {
			$sql .= $child->createModifySql();
		}
		
		if(preg_match("/^(.*), ?/",$sql,$temp)) {
			$sql = $temp[1].' ';	
		}
		
		//pre form
		$preForm="";
		foreach ($this->childs as $child) {
			$preForm.=$child->createModifyPreForm();
		}
		
		$replacement = Array(
			'/*validate*/' => $validate,
    	'/*preform*/' => $preForm,
			'/*displayErrors*/' => $er,
    	'/*form*/' => $form,
    	'/*sql*/' => $sql
		);
    
		$createModify = strtr($createModify,$replacement);
    	
    $replacement = Array( //"/*includePath*/"=>$include_path,
    						//"/*relRep*/"=>$this->relRep,
    						"/*idName*/"=>"id");
								
    $createModify=strtr($createModify,$replacement);
		
		echo "----------------\n\n";
		$string=highlight_string($createModify,true);
		$lines=explode("<br />",$string);
		echo "<table>";
		$i=1;
		foreach ($lines as $line) {
			echo "<tr>";
			echo "<td width='30px' style='background-color:#EEEEEE'>".($i++)."</td>";
			echo "<td>".$line."</td>";
			echo "</tr>";	
		}
		echo "</table>";
		echo "----------------\n\n";
		file_put_contents(dirname(__FILE__)."/../user/trombi.inc",$createModify);
	}
	
	public function detail() {
		$detail=file_get_contents(dirname(__FILE__)."/templates/detail.inc");
		/*
		$temps=explode("/",$this->relRep);
		$include_path="";
		foreach($temps as $temp) {
			if ($temp!="") {
				$include_path.="../";	
			}	
		}
		*/
		//Detail creation
		$detailChild="";
		foreach ($this->childs as $child) {
			$detailChild.=$child->detail();
		}
		
		//Detail creation
		$preDetailChild="";
		foreach ($this->childs as $child) {
			$preDetailChild.=$child->preDetail();
		}
				
		$replacement=array( "/*detail*/"=>$detailChild,
							"/*preDetail*/"=>$preDetailChild);
    	$detail=strtr($detail,$replacement);
    	
    	$replacement=array( //"/*includePath*/"=>$include_path,
    						//"/*relRep*/"=>$this->relRep,
    						"/*idName*/"=>"id");
    	$detail=strtr($detail,$replacement);
		echo "----------------\n\n";
		$string=highlight_string($detail,true);
		$lines=explode("<br />",$string);
		echo "<table>";
		$i=1;
		foreach ($lines as $line) {
			echo "<tr>";
			echo "<td width='30px' style='background-color:#EEEEEE'>".($i++)."</td>";
			echo "<td>".$line."</td>";
			echo "</tr>";	
		}
		echo "</table>";
		echo "----------------\n\n";
		file_put_contents(dirname(__FILE__)."/../trombi/detail.inc",$detail);
	}
	
	public function search() {
		$search=file_get_contents(dirname(__FILE__)."/templates/search.inc");
		
		/*$temps=explode("/",$this->relRep);
		$include_path="";
		foreach($temps as $temp) {
			if ($temp!="") {
				$include_path.="../";	
			}	
		}*/
		
		//FastSearchSql creation
		$fastsearchsqlChild = '';
		foreach ($this->childs as $child) {
			$fastsearchsqlChild.=$child->searchFastSql();
		}
		
		//SearchSql creation
		$searchsqlChild = '';
		foreach ($this->childs as $child) {
			$searchsqlChild.=$child->searchSql();
		}
		
		//SearchSql creation
		$searchformChild = '';
		foreach ($this->childs as $child) {
			$searchformChild.=$child->searchForm();
		}

		$cardChild = '';
		foreach( $this->childs as $child ){
			$cardChild .= $child->card();
		}
		
		$showresult=file_get_contents(dirname(__FILE__)."/showResult.inc");
		
		$replacement=array( "/*fastsearchsql*/"=>$fastsearchsqlChild,
							"/*searchsql*/"=>$searchsqlChild,
							"/*searchform*/"=>$searchformChild,
							"/*showresult*/"=>$showresult,
							"/*showcard*/"=>$cardChild);
    	$search=strtr($search,$replacement);
    	
    	$replacement=array( //"/*includePath*/"=>$include_path,
    						//"/*relRep*/"=>$this->relRep,
    						"/*idName*/"=>"id",
    						"/*secret*/"=>$this->secret);
    	$search=strtr($search,$replacement);
		
		file_put_contents(dirname(__FILE__)."/../trombi/index.php",$search);
		
		echo "----------------\n\n";
		$string=highlight_string($search,true);
		$lines=explode("<br />",$string);
		echo "<table>";
		$i=1;
		foreach ($lines as $line) {
			echo "<tr>";
			echo "<td width='30px' style='background-color:#EEEEEE'>".($i++)."</td>";
			echo "<td>".$line."</td>";
			echo "</tr>";	
		}
		echo "</table>";
		echo "----------------\n\n";
		
	}
	
	public function fastSearch() {
		$search=file_get_contents(dirname(__FILE__)."/templates/fastsearch.inc");
		
		if (!file_exists(dirname(__FILE__)."/../trombi/ajax/")) {
			mkdir(dirname(__FILE__)."/../trombi/ajax/");
		}
		
		//FastSearchSql creation
		$fastsearchsqlChild="";
		foreach ($this->childs as $child) {
			$fastsearchsqlChild.=$child->searchFastSql();
		}
		
		$replacement=array( "/*fastsearchsql*/"=>$fastsearchsqlChild);
    	$search=strtr($search,$replacement);
    	
    	$replacement=array( //"/*includePath*/"=>$include_path,
    						//"/*relRep*/"=>$this->relRep,
    						"/*idName*/"=>"id");
    	$search=strtr($search,$replacement);
		
		file_put_contents(dirname(__FILE__)."/../trombi/ajax/fastsearch.php",$search);
		
		echo "----------------\n\n";
		$string=highlight_string($search,true);
		$lines=explode("<br />",$string);
		echo "<table>";
		$i=1;
		foreach ($lines as $line) {
			echo "<tr>";
			echo "<td width='30px' style='background-color:#EEEEEE'>".($i++)."</td>";
			echo "<td>".$line."</td>";
			echo "</tr>";	
		}
		echo "</table>";
		echo "----------------\n\n";
		
	}
	
	public function otherFiles() {
		
		if (!file_exists(dirname(__FILE__)."/../trombi/ajax/")) {
			mkdir(dirname(__FILE__)."/../trombi/ajax/");
		}

		copy(dirname(__FILE__)."/templates/socialstatus.inc", dirname(__FILE__)."/../trombi/ajax/social_status.php");
		copy(dirname(__FILE__)."/templates/trombi.inc", dirname(__FILE__)."/../trombi/trombi.class.inc");
		
		echo "----------------\n\n";
		echo 'copy('.dirname(__FILE__)."/templates/socialstatus.inc".', '.dirname(__FILE__)."/../trombi/ajax/social_status.php".')';
		echo 'copy('.dirname(__FILE__)."/templates/trombi.inc".', '.dirname(__FILE__)."/../trombi/trombi.class.inc".')';
		echo "----------------\n\n";
		
	}
	

	private function colorSyntaxSql ($aff) {
		$pattern = '/\'([^\']*)\'/i';
		$replacement = '<span style="color:green;">\'$1\'</span>';
		$aff=preg_replace($pattern, $replacement, $aff);
		$pattern = '/`([^`]*)`/i';
		$replacement = '<span style="color:red;">`$1`</span>';
		$aff=preg_replace($pattern, $replacement, $aff);
		
		$format_keywords=array("DESCRIBE ",
								"INSERT ",
								"SET ",
								"FROM ",
								"WHERE ",
								"UPDATE ",
								"DELETE ",
								"DROP ",
								"SELECT ",
								"GROUP BY ",
								"ORDER BY ",
								"LIMIT ",
								"LEFT JOIN",
								"INNER JOIN",
								"JOIN ",
								"ADD ",
								"CREATE",
								"ENGINE",
								"DEFAULT CHARSET",
								"ALTER");
		foreach ($format_keywords as $keyword) {
			$color_version[]="<br><span style='color:blue;font-weight:bold;'>".$keyword."</span>";
		}
		$keywords=$format_keywords;
		$color_keywords = array("AND ","OR ", " ON ");
		foreach ($color_keywords as $keyword) {
			$color_version[]="<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:blue;'>".$keyword."</span>";
			$keywords[]=$keyword;
		}
		$color_keywords = array(" AS ","LIKE","VALUES", "COUNT","MAX","MIN","PASSWORD","*","TABLE ", "MODIFY COLUMN ","IF EXISTS","INDEX ","PRIMARY KEY ");
		foreach ($color_keywords as $keyword) {
			$color_version[]=" <span style='color:blue;'>".$keyword."</span> ";
			$keywords[]=$keyword;
		}
		
		$keywords[]=";";
		$color_version[]=";<br>";
		$keywords[]=",";
		$color_version[]=",<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		$aff=str_ireplace($keywords,$color_version,$aff);
		
		return $aff;
	}
}


?>
