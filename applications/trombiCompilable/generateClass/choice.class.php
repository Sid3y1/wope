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

class Choice implements Field, Generable {
	private $id='';
	private $text='';
	private $option=null;
	private $multipleChoice=false;
	private $query=null;
	
	public function __construct($id,$text) {
		$this->id=$id;
		$this->text=$text;
		$this->multipleChoice=false;
		$this->option=new Option();
		$this->query=null;
	}
	
	public function setOptionDb($query,$maxSizeOptionId) {
		$this->query=$query;
		$this->option->setOptionDb($maxSizeOptionId);
	}
	
	public function setMultipleChoice($multipleChoice) {
		$this->multipleChoice=($multipleChoice=="true");
	}
	
	public function addOption($id,$value) {
		$this->option->addOption($id,$value);
	}
	
	public function sql(&$extraSql) {
		if (!$this->option->isModeDb()) {
			$extraSql.="DROP TABLE IF EXISTS `trombi_".$this->id."`;\n";
			$extraSql.="CREATE TABLE `trombi_".$this->id."` (\n";
			$extraSql.="  `id` VARCHAR(".$this->option->getMaxSizeOptionId().") NOT NULL default '', \n";
			if ($this->option->getMaxSizeOptionValue()<255) {
				$extraSql.="  `value` VARCHAR(".$this->option->getMaxSizeOptionValue().") NOT NULL default '', \n";
			} else {
				$extraSql.="  `value` TEXT NOT NULL default '', \n";	
			}
			
			$extraSql.="  PRIMARY KEY  (`id`)\n";
			$extraSql.=") ENGINE=MyISAM DEFAULT CHARSET=utf8;\n\n";
			$replacement=array("/*id*/"=>"trombi_".$this->id);
			$extraSql.=strtr($this->option->sql(),$replacement);
		}
		if (!$this->multipleChoice) {
			return "  `".$this->id."` VARCHAR(".$this->option->getMaxSizeOptionId().") NOT NULL default '', \n";
		} else {
			$extraSql.="DROP TABLE IF EXISTS `trombi_multi_".$this->id."`;\n";
			$extraSql.="CREATE TABLE `trombi_multi_".$this->id."` (\n";
			$extraSql.="  `trombi_id` INT UNSIGNED NOT NULL, \n";
			$extraSql.="  `".$this->id."` VARCHAR(".$this->option->getMaxSizeOptionId().") NOT NULL default '', \n";
			$extraSql.="   PRIMARY KEY(`trombi_id`,`".$this->id."`) \n";
			$extraSql.=") ENGINE=MyISAM DEFAULT CHARSET=utf8;\n\n";
		}
	}
	
	public function updateSql(&$extraSql) {
		$sql=$this->sql($extraSql);
		if ($sql!="") {
			return " MODIFY COLUMN ".$this->sql($extraSql);
		} else {
			return "";
		}
	}
		
	private function getQuery() {
		if ($this->query==null) {
			$this->query="SELECT id, value FROM `trombi_".$this->id."`";
		}
		return $this->query;
	}
	
	//TODO createModifyForm
	public function createModifyForm(){
		
		if ($this->multipleChoice) {
			$type="CHECKBOX";
			$name=$this->id."[]";
			$extraphp="\$query=\"SELECT `".$this->id."` FROM `trombi_multi_".$this->id."` WHERE trombi_id='\$idTrombi'\";\n";
			$extraphp.="\$result=\$db->query(\$query);\n";
			$extraphp.="while (\$row=\$db->fetchArray(\$result,MYSQL_ASSOC)){\n";
			$extraphp.="\$value['".$this->id."'][]=\$row['".$this->id."'];\n";
			$extraphp.="}\n";
		} else {
			$extraphp="";
			$type="RADIO";
			$name=$this->id;
			$query="";
		}
		$html=file_get_contents(dirname(__FILE__)."/../templates/createModify_choice.inc");
		$replacement=array( "/*extraphp*/"=>$extraphp,
							"/*id*/"=>$this->id,
    						"/*text*/"=>$this->text,
    						"/*name*/"=>$name,
    						"/*type*/"=>$type,
    						"/*query*/"=>$this->query);
    	$html=strtr($html,$replacement);
    	return $html;
	}
	
	public function createModifyValidate(){
		$return="\$query=\"".$this->getQuery()."\";\n";
		$return.="\$result=\$db->query(\$query);\n";
		$return.="\$valid_value=array();\n";
		$return.="while (\$row=\$db->fetchArray(\$result,MYSQL_ASSOC)){\n";
		$return.="\$valid_value[]=\$row['id'];\n";
		$return.="}\n";
			
		if ($this->multipleChoice) {
			//TODO : Bug !
			$return.="\$query=\"DELETE FROM `trombi_multi_".$this->id."` WHERE trombi_id='\$idTrombi'\";\n";
			$return.="\$db->query(\$query);\n";
			$return.="if (is_array(\$_POST['".$this->id."'])) {\n";
			$return.="foreach (\$_POST['".$this->id."'] as \$choice_selected_value) {\n";
			$return.="if (in_array(\$choice_selected_value,\$valid_value)) {\n";
			$return.="\$query=\"INSERT INTO `trombi_multi_".$this->id."` VALUES ('\$idTrombi','\$choice_selected_value')\";\n";
			$return.="\$db->query(\$query);\n";
			$return.="}\n";
			$return.="}\n";
			$return.="}\n";
		} else {
			$return.="if (is_array(\$_POST['".$this->id."']) && (in_array(\$_POST['".$this->id."'],\$valid_value))) {\n";
			$return.="\$".$this->id."=\$_POST['".$this->id."'];\n";
			$return.="} else {\n";
			$return.="\$".$this->id."='';\n";;
			$return.="}\n";
		}
		return $return;
	}
	
	public function createModifyError(){
		$err = 'if( isset($error["'.$this->id.'"]) ){'."\n".
		'$err_msg .= "<dt>'.$this->text.' </dt><dd>".$error["'.$this->id.'"]."</dd>";'."\n".
		'}'."\n";
		return $err;
	}

	public function createModifySql(){
		if (!$this->multipleChoice) {
			return "$this->id='\".mysql_real_escape_string(\$".$this->id.").\"', ";	
		} else {
			return "";
		}
	}
    
    public function detail() {
    	if ($this->multipleChoice) {
			$name=$this->id."[]";
			$extraphp="\$query=\"SELECT `".$this->id."` FROM `trombi_multi_".$this->id."` WHERE trombi_id='\$idValue'\";\n";
			$extraphp.="\$result=\$db->query(\$query);\n";
			$extraphp.="while (\$row=\$db->fetchArray(\$result,MYSQL_ASSOC)){\n";
			$extraphp.="\$value['".$this->id."'][]=\$row['".$this->id."'];\n";
			$extraphp.="}\n";
		} else {
			$extraphp="";
			$name=$this->id;
		}
		$html=file_get_contents(dirname(__FILE__)."/../templates/detail_choice.inc");
		$replacement=array( "/*extraphp*/"=>$extraphp,
							"/*id*/"=>$this->id,
    						"/*text*/"=>$this->text,
    						"/*query*/"=>$this->query);
    	$html=strtr($html,$replacement);
    	return $html;
    }

	public function card(){
   	if ($this->multipleChoice) {
			$name=$this->id."[]";
			$extraphp="\$query=\"SELECT `".$this->id."` FROM `trombi_multi_".$this->id."` WHERE trombi_id='\$idValue'\";\n";
			$extraphp.="\$result=\$db->query(\$query);\n";
			$extraphp.="while (\$row=\$db->fetchArray(\$result,MYSQL_ASSOC)){\n";
			$extraphp.="\$value['".$this->id."'][]=\$row['".$this->id."'];\n";
			$extraphp.="}\n";
		} else {
			$extraphp="";
			$name=$this->id;
		}
		$html=file_get_contents(dirname(__FILE__)."/../templates/card_choice.inc");
		$replacement=array( "/*extraphp*/"=>$extraphp,
							"/*id*/"=>$this->id,
    						"/*text*/"=>$this->text,
    						"/*query*/"=>$this->query);
    	$html=strtr($html,$replacement);
    	return $html;
	}
    
    public function toString() {
    	$return = "Choice (".$this->id.") :\n"
    				."  Text = ".$this->text."\n";
    	$return.=$this->option->toString();
    	return $return;	
    }
    
    public function searchFastSql() {
		return "";
    }
    
	public function searchSql(){
		return "";
	}
	
	public function searchForm(){
		return "";
	}
}
?>
