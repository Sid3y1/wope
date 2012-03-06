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

class Text implements Field, Generable {
	private $id="";
	private $text="";
	private $extraText="";
	private $default="";
	private $maxSize="";
	private $searchable=false;
	private $inFastSearch=false;
	private $nbLigne="";
	
	private $regExpTest="";
	private $regExpError="";
	
	public function __construct($id, $text) {
		$this->id=$id;
		$this->text=$text;
		$this->extraText="";
		$this->default="";
		$this->maxSize="";
		$this->searchable=false;
		$this->inFastSearch=false;
		$this->nbLigne="";
		$this->regExpTest="";
		$this->regExpError="";	
	}
    
  public function setDefault ($default) {
 		$this->default=$default;
  }
    
  public function setMaxSize($maxSize) {
		$this->maxSize=$maxSize;
	}
	
	public function setSearchable($searchable) {
		$this->searchable=($searchable=="true");
	}
	
	public function setInFastSearch($inFastSearch) {
		$this->inFastSearch=($inFastSearch=="true");
	}
	
	public function setNbLigne($nbLigne) {
		$this->nbLigne=$nbLigne;
	}
	
	public function setRegExpTest($regExpTest) {
		$regExpTest=str_replace("/",'\/',$regExpTest);
		$this->regExpTest=$regExpTest;
	}
	
	public function setRegExpError($regExpError) {
		$this->regExpError=$regExpError;
	}
	
	public function setExtraText($extraText) {
		$this->extraText=$extraText;
	}
	
	public function sql(&$extraSql) {
    	$ret="";
    	if ($this->maxSize<255) {
			$ret.= "  `".$this->id."` VARCHAR(".$this->maxSize.") ";
		} else {
			$ret.= "  `".$this->id."` TEXT ";	
		}
		$ret.= " NOT NULL default '', \n";
		if ($this->inFastSearch) {
			$extraSql.="ALTER TABLE `trombi_trombi` ADD INDEX (`".$this->id."`); \n\n";	
		}
		return $ret;
    }
    
	public function updateSql(&$extraSql) {
		return " MODIFY COLUMN ".$this->sql($extraSql);
	}
	
	public function createModifySql(){
		return "$this->id='\".mysql_real_escape_string(str_replace(Array(\"\\n\", \"  \"), Array(\"<br />\", \"&nbsp;&nbsp;\"), htmlentities(stripslashes(\$_POST['$this->id']), ENT_QUOTES, \"UTF-8\"))).\"', ";
	}
	
	public function createModifyValidate(){
		$validate="";
		if ($this->regExpTest!='') {
			$validate.='if (!preg_match("/^'.$this->regExpTest.'$/",$_POST["'.$this->id.'"])) {'."\n";
			$validate.='$valid=false;'."\n";
			$validate.='$error["'.$this->id.'"]="'.$this->regExpError.'";'."\n";
			$validate.='};'."\n";
		}
		return $validate;
	}

	public function createModifyError(){
		$err = 'if( isset($error["'.$this->id.'"]) ){'."\n".
		'$err_msg .= "<dt>'.$this->text.'</dt><dd>".$error["'.$this->id.'"]."</dd>";'."\n".
		'}'."\n";
		return $err;
	}

	public function createModifyForm(){
		if ($this->nbLigne>1) {
			$html=file_get_contents(dirname(__FILE__)."/../templates/createModify_text_big.inc");
		} else {
			$html=file_get_contents(dirname(__FILE__)."/../templates/createModify_text_small.inc");
		}
		$cols=intVal($this->maxSize/$this->nbLigne)+1;
    	$replacement=array(
							"/*id*/"=>$this->id,
    						"/*text*/"=>$this->text,
    						"/*maxSize*/"=>$this->maxSize,
    						"/*cols*/"=>$cols,
    						"/*rows*/"=>$this->nbLigne,
								"/*extraText*/"=>$this->extraText);
    	$html=strtr($html,$replacement);
    	return $html;
	}
	
	public function detail(){
		if ($this->nbLigne>1) {
			$html=file_get_contents(dirname(__FILE__)."/../templates/detail_text_big.inc");
		} else {
			$html=file_get_contents(dirname(__FILE__)."/../templates/detail_text_small.inc");
		}
		
		$replacement=array("/*id*/"=>$this->id,
    						"/*text*/"=>stripslashes($this->text));
    $html=strtr($html,$replacement);
    return $html;
	}
	
	public function card(){
		if ($this->nbLigne>1) {
			$html=file_get_contents(dirname(__FILE__)."/../templates/card_text_big.inc");
		} else {
			$html=file_get_contents(dirname(__FILE__)."/../templates/card_text_small.inc");
		}
		
		$replacement = Array(
			'/*id*/' => $this->id,
    	'/*text*/' => $this->text
		);
    $html = strtr($html, $replacement);
		return $html;
	}
    
  public function toString() {
   	$return="Text (".$this->id.") : \n"
			."  Text = ".$this->text."\n"
			."  Default = ".$this->default."\n"
			."  MaxSize = ".$this->maxSize."\n"
			."  Searchable = ".$this->searchable."\n"
			."  InFastSearch = ".$this->inFastSearch."\n"
			."  NbLigne = ".$this->nbLigne."\n"
			."  RegExpTest = ".$this->regExpTest."\n"
			."  RegExpError = ".$this->regExpError."\n";
		return $return;
  }
    
  public function searchFastSql() {
   	if ($this->inFastSearch) {
   		return " OR LOWER(".$this->id.") LIKE CONCAT('%',LOWER('\".$db->escapeString(getValue('fastsearch')).\"'),'%') ";
   	} else {
   		return '';
   	}
  }
	
	public function searchSql(){
		if ($this->searchable) {
			return "\".(getValue('".$this->id."')==null?\"\":\" AND LOWER(".$this->id.") LIKE CONCAT('%',LOWER('\".$db->escapeString(getValue('".$this->id."')).\"'),'%')\").\"";	
		} else {
			return "";
		}
	}
	
	public function searchForm(){
		if ($this->searchable) {
			$html=file_get_contents(dirname(__FILE__)."/../templates/search_text.inc");
			
			$replacement=array(
							"/*id*/"=>$this->id,
    						"/*text*/"=>$this->text,
    						"/*maxSize*/"=>$this->maxSize,
    						"/*rows*/"=>$this->nbLigne,
								"/*extraText*/"=>$this->extraText);
    		$html=strtr($html,$replacement);
    		return $html;
		} else {
			return "";
		}
	}
}
?>
