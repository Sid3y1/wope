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

class TrombiGenDate implements Field, Generable {
	private $id="";
	private $text="";
	private $extraText="";
	private $selectWith='text';
	private $searchable=false;
	private $inFastSearch=false;
	
	
	public function __construct($id, $text) {
		$this->id=$id;
		$this->text=$text;
		$this->extraText='';
		$this->searchable=false;
		$this->inFastSearch=false;
		$this->selectWith='text';
	}
        
	public function setSelectWith($selectWith) {
		$this->selectWith=$selectWith;
	}
	
	public function setSearchable($searchable) {
		$this->searchable=($searchable=="true");
	}
	
	public function setInFastSearch($inFastSearch) {
		$this->inFastSearch=($inFastSearch=="true");
	}
	
	public function setExtraText($extraText) {
		$this->extraText=$extraText;
	}
	
	
	public function sql(&$extraSql) {
    	$ret= "  `".$this->id."` DATE ";
		$ret.= " NOT NULL default '0000-00-00', \n";
		if ($this->inFastSearch) {
			$extraSql.="ALTER TABLE `trombi_trombi` ADD INDEX (`".$this->id."`); \n\n";	
		}
		return $ret;
    }
    
	public function updateSql(&$extraSql) {
		return " MODIFY COLUMN ".$this->sql($extraSql);
	}
	
	public function createModifySql(){
		return $this->id."='\$post_".$this->id."', ";
	}
	
	public function createModifyValidate(){
		$validate="";

		switch($this->selectWith) {
			case 'selects':
				$validate='if( $_POST["day_'.$this->id.'"]!="" && $_POST["month_'.$this->id.'"]!="" && $_POST["year_'.$this->id.'"]!="" ){'."\n";
				$validate.='$post_'.$this->id.'= $_POST["'.$this->id.'"] = $_POST["year_'.$this->id.'"]."-".$_POST["month_'.$this->id.'"]."-".$_POST["day_'.$this->id.'"];'."\n";
				$validate.='} else { '."\n";
				$validate.='$post_["'.$this->id.'"]="0000-00-00";'."\n";
				$validate.='} '."\n";
				break;
				
			default:
				$validate='if ($_POST["'.$this->id.'"]!="" and !preg_match("/^([0-1]?[0-9])\/([0-3]?[0-9])\/([2,1][0,9][0-9][0-9])$/",$_POST["'.$this->id.'"], $temp)) {'."\n";
				$validate.='$valid=false;'."\n";
				$validate.='$error["'.$this->id.'"]="Date invalide";'."\n";
				$validate.='} elseif ($_POST["'.$this->id.'"]!="") {'."\n";
				$validate.='$post_'.$this->id.'=$temp[3]."-".$temp[2]."-".$temp[1];'."\n";
				$validate.='} else { '."\n";
				$validate.='$post_["'.$this->id.'"]="0000-00-00";'."\n";
				$validate.='} '."\n";
				break;
		}
		
		return $validate;
	}
	
	public function createModifyError(){
		$err = 'if( isset($error["'.$this->id.'"]) ){'."\n".
		'$err_msg .= "<dt>'.$this->text.' </dt><dd>".$error["'.$this->id.'"]."</dd>";'."\n".
		'}'."\n";
		return $err;
	}

	public function createModifyForm(){
		$html=file_get_contents(dirname(__FILE__)."/../templates/createModify_date.inc");
		
		$pattern=array("/\/\*id\*\//", "/\/\*text\*\//", "/\/\*extraText\*\//");
		$replacement=array($this->id, $this->text, $this->extraText);
		
		switch($this->selectWith) {
			case 'calendar':
				//TODO
				$pattern[]="/\/\*calendar\*\//";
				$pattern[]="/\/\*input-selects\*\/.*\/\*input-selects\*\//s";
				$pattern[]="/\/\*input-text\*\//";
				$replacement[]="";
				$replacement[]="";
				$replacement[]="";
				break;
			case 'selects':
				//TODO
				$pattern[]="/\/\*input-selects\*\//";
				$pattern[]="/\/\*calendar\*\/.*\/\*calendar\*\//s";
				$pattern[]="/\/\*input-text\*\/.*\/\*input-text\*\//s";
				$replacement[]="";
				$replacement[]="";
				$replacement[]="";
				break;
			case 'text':
				$pattern[]="/\/\*input-text\*\//";
				$pattern[]="/\/\*calendar\*\/.*\/\*calendar\*\//s";
				$pattern[]="/\/\*input-selects\*\/.*\/\*input-selects\*\//s";
				$replacement[]="";
				$replacement[]="";
				$replacement[]="";
				break;
		}
		
		$html=preg_replace($pattern, $replacement, $html);
    return $html;
	}
	

	public function detail(){
		$html=file_get_contents(dirname(__FILE__)."/../templates/detail_date.inc");
		
		$replacement=array("/*id*/"=>$this->id,
    						"/*text*/"=>$this->text);
   	$html=strtr($html,$replacement);
   	return $html;
	}
	
	public function card(){
		$html=file_get_contents(dirname(__FILE__)."/../templates/card_date.inc");
		
		$replacement=array("/*id*/"=>$this->id,
    						"/*text*/"=>$this->text);
   	$html=strtr($html,$replacement);
   	return $html;
	}
    
    public function toString() {
    	$return="Date (".$this->id.") : \n"
			."  Date = ".$this->text."\n"
			."  Searchable = ".$this->searchable."\n"
			."  InFastSearch = ".$this->inFastSearch."\n";
			
		return $return;
    }
    
    public function searchFastSql() {
    	if ($this->inFastSearch) {
    		return '".(
    			preg_match("/^([0-1]?[0-9])\/([0-3]?[0-9])\/([2,1][0,9][0-9][0-9])/",getValue("fastsearch"), $temp)?
					" OR '.$this->id.'=\'".$temp[3]."-".$temp[2]."-".$temp[1]."\'"
					:"")."';
    	} else {
    		return "";
    	}
    }
	
	public function searchSql(){
		if ($this->searchable) {
			return '".((getValue("'.$this->id.'"))==null?"":(
    			preg_match("/^([0-1]?[0-9])\/([0-3]?[0-9])\/([2,1][0,9][0-9][0-9])/",getValue("'.$this->id.'"), $temp)?
					" AND '.$this->id.'=\'".$temp[3]."-".$temp[2]."-".$temp[1]."\'"
					:""))."';	
		} else {
			return "";
		}
	}
	
	public function searchForm(){
		
		if ($this->searchable) {
			$html=file_get_contents(dirname(__FILE__)."/../templates/search_date.inc");
			
			$pattern=array("/\/\*id\*\//", "/\/\*text\*\//", "/\/\*extraText\*\//");
			$replacement=array($this->id, $this->text, $this->extraText);
		
			switch($this->selectWith) {
				case 'calendar':
					//TODO
					$pattern[]="/\/\*calendar\*\//";
					$pattern[]="/\/\*input-selects\*\/.*\/\*input-selects\*\//s";
					$pattern[]="/\/\*input-text\*\//";
					$replacement[]="";
					$replacement[]="";
					$replacement[]="";
					break;
				case 'selects':
					//TODO
					$pattern[]="/\/\*input-selects\*\//";
					$pattern[]="/\/\*calendar\*\/.*\/\*calendar\*\//s";
					$pattern[]="/\/\*input-text\*\/.*\/\*input-text\*\//s";
					$replacement[]="";
					$replacement[]="";
					$replacement[]="";
					break;
				case 'text':
					$pattern[]="/\/\*input-text\*\//";
					$pattern[]="/\/\*calendar\*\/.*\/\*calendar\*\//s";
					$pattern[]="/\/\*input-selects\*\/.*\/\*input-selects\*\//s";
					$replacement[]="";
					$replacement[]="";
					$replacement[]="";
					break;
			}
		
			$html=preg_replace($pattern, $replacement, $html);
	    return $html;
		} else {
			return "";
		}
	}
}
?>
