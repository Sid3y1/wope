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

class GroupField implements Generable{
	private $text="";
	private $childs=array();
	
	public function __construct($text) {
		$this->text=$text;
		$this->childs=array();
	}
    public function sql(&$extraSql) {
    	$return="";
    	foreach ($this->childs as $child) {
    		$return.=$child->sql($extraSql);
    	}
    	return $return;
    }
    
    public function updateSql(&$extraSql) {
		$return="";
    	foreach ($this->childs as $child) {
    		$return.=$child->updateSql($extraSql);
    	}
    	return $return;
	}
	
    public function add($field) {
    	$this->childs[]=$field;
    }
    
    public function createModifyForm(){
		$html=file_get_contents(dirname(__FILE__)."/../templates/createModify_groupField.inc");
    	
    	$form="";
    	foreach ($this->childs as $child) {
    		$form.=$child->createModifyForm();
    	}
    	$replacement=array("/*chieldform*/"=>$form, "/*nom*/"=>$this->text);
    	$html=strtr($html,$replacement);
		
    	return $html;
	}
	
    public function detail(){
	    $locale = Locale::getLocale();
		$html = file_get_contents(dirname(__FILE__)."/../templates/detail_groupField.inc");
    	
    $form = '';
    foreach( $this->childs as $child ){
    	$form .= $child->detail();
    }
		$form = $form != '' ? $form : $locale->display('nc', 'trombi');
    $replacement = Array("/*chieldform*/" => $form, "/*nom*/" => $this->text);
    $html = strtr($html, $replacement);
		
    return $html;
	}
	
	public function card(){
	    $locale = Locale::getLocale();
		$html = file_get_contents(dirname(__FILE__).'/../templates/card_groupField.inc');
    	
   	$block = '';
   	foreach ($this->childs as $child) {
   		$block .= $child->card();
   	}
		
		$block = $block != '' ? $block : $locale->display('nc', 'trombi');
   	$replacement = Array('/*chieldform*/' => $block, '/*title*/' => $this->text);
   	$html = strtr($html, $replacement);
		
   	return $html;
	}

	public function createModifyValidate(){
		$validate="";
		foreach ($this->childs as $child) {
    		$validate.=$child->createModifyValidate();
    	}
    	return $validate;
	}
	
	public function createModifyError(){
		$err = '';
		foreach ($this->childs as $child) {
    		$err .= $child->createModifyError();
    }
   	return $err;
	}

	public function createModifySql(){
		$sql="";
    	foreach ($this->childs as $child) {
    		$sql.=$child->createModifySql();
    	}
    	return $sql;
	} 
    
  public function createModifyPreForm() {
   	return "\$tabs_array['".$this->text."']='".$this->text."';\n";
  }
	
  public function preDetail() {
   	return "\$tabs_array['".$this->text."']='".$this->text."';\n";
  }
	
  public function searchFastSql() {
   	$sql="";
   	foreach ($this->childs as $child) {
   		$sql.=$child->searchFastSql();
   	}
   	return $sql;
  }
	
	public function searchSql(){
		$sql="";
    	foreach ($this->childs as $child) {
    		$sql.=$child->searchSql();
    	}
    	return $sql;
	}
	
	public function searchForm(){
		$html=file_get_contents(dirname(__FILE__)."/../templates/search_groupField.inc");
  
		$form="";
    	foreach ($this->childs as $child) {
    		$form.=$child->searchForm();
    	}
    	if ($form=="") {
    		$html="";
    	}
    	$replacement=array("/*chieldform*/"=>$form, "/*nom*/"=>$this->text, "/*submitbutton*/"=>'<input type="submit" name="search" value="<?=$locale->display("seek","kernel")?>">');
    	$html=strtr($html,$replacement);
		
    	return $html;
	}

	public function toString() {
   	$return="--- ".$this->text."\n";
   	foreach ($this->childs as $child) {
   		$return.=$child->toString();
   	}
   	$return.="--- /".$this->text."\n";
   	return $return;	
   }
    
}
?>
