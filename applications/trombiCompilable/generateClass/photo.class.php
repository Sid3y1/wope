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

class Photo implements Field, Generable {
	private $id="";
	private $text="";
	private $path="";
	private $maxSize=0;
	private $imageSize="";
	
	
	public function __construct($id, $text) {
		$this->id=$id;
		$this->text=$text;
		$this->path="";
		$this->maxSize=0;
		$this->imageSize="";
	}

	public function setMaxSize($maxSize) {
		$this->maxSize=$maxSize;
	}

	public function setPath($path) {
		$this->path=$path;	
	}
	
	public function setImageSize($imageSize) {
		$this->imageSize=$imageSize;
	}
	
	
  public function sql(&$extraSql) {
   	return "  `".$this->id."` VARCHAR(255) NOT NULL default '', \n";
  }
    
  public function updateSql(&$extraSql) {
		return " MODIFY COLUMN ".$this->sql($extraSql);
	}
    
    public function toString() {
    	$return="Photo (".$this->id.") :\n"
    		."  text = ".$this->text."\n"
    		."  path = ".$this->path."\n";
    	return $return;
    }
	
/*	public function createModifyValidate(){
		$validate='
			if(isset($_FILES[\''.$this->id.'\']) and $_FILES[\''.$this->id.'\'][\'tmp_name\']!="") {
			$'.$this->id.'_photoDir=dirname(__FILE__)."/photo/'.$this->id.'_$idTrombi.jpg";
			$'.$this->id.'_photoUrl="/user/photo/'.$this->id.'_$idTrombi.jpg";
			$size = @getimagesize($_FILES[\''.$this->id.'\'][\'tmp_name\']);
            //Accept GIF JPEG PNG images
            if ($size!=null && $size[2] >= 1 && $size[2] <= 3) {
            	move_uploaded_file ($_FILES[\''.$this->id.'\'][\'tmp_name\'], $'.$this->id.'_photoDir);
            	$core->loadClass(\'file\');
                $file = new file();'."\n";
                
		if ($this->imageSize!="") {
         	list($largeur, $hauteur) = explode("x", $this->imageSize);
         	
	    	$validate.='if($file->image_resize($'.$this->id.'_photoDir,'.$largeur.','.$hauteur.') == false) {
	                	$error["'.$this->id.'"]=$locale->display(\'img_error\',\'trombi\');
	                    $valid=false;
	                } else {'."\n";
	        $validate.='
						$values["'.$this->id.'"]=$'.$this->id.'_photoUrl;
					}'."\n";
        }
	           
        $validate.='
            } else {
				$valid=false;
            	$error["'.$this->id.'"]=$locale->display(\'img_error\',\'trombi\');
            }
		}'."\n";
		$validate.='
				if (isset($_POST["'.$this->id.'"])) {
					if ($_POST["'.$this->id.'"]=="delete") {
						$'.$this->id.'_photoDir=dirname(__FILE__)."/photo/'.$this->id.'_$idTrombi.jpg";
						if (file_exists($'.$this->id.'_photoDir)) {
							unlink($'.$this->id.'_photoDir);
							
						}
						$'.$this->id.'_photoUrl="";
						$falsevalid = true;
					}
				} elseif (!isset($'.$this->id.'_photoUrl)) {
					$'.$this->id.'_photoUrl=$values["'.$this->id.'"];
				}
				';
		return $validate;
	}*/


	public function createModifyValidate(){
		$validate='';
		$validate.='
			if( isset($photo_ids) && isset($photo_ids[\''.$this->id.'\']) && $photo_ids[\''.$this->id.'\'] != \'\'){
				$values["'.$this->id.'"] = $_POST["'.$this->id.'"] = $photo_ids[\''.$this->id.'\'];
			}elseif( isset($_POST["delete_'.$this->id.'"]) && $_POST["delete_'.$this->id.'"] == "delete" ){
				$_POST["'.$this->id.'"] = $values["'.$this->id.'"] = "";
			}
		';
		
		return $validate;
	}
	
	public function createModifyError(){
		$err = 'if( isset($error["'.$this->id.'"]) ){'."\n".
		'$err_msg .= "<dt>'.$this->text.' </dt><dd>".$error["'.$this->id.'"]."</dd>";'."\n".
		'}'."\n";
		return $err;
	}

	public function createModifyForm() {
    	
    	$html=file_get_contents(dirname(__FILE__)."/../templates/createModify_photo.inc");
    	$replacement=array(
							"/*id*/"=>$this->id,
    						"/*text*/"=>$this->text,
    						"/*maxSize*/"=>$this->maxSize);
    	$html=strtr($html,$replacement);
    	return $html;
    }
    
  public function detail() {
    	
    $html=file_get_contents(dirname(__FILE__)."/../templates/detail_photo.inc");
    $replacement=array(
						"/*id*/"=>$this->id,
    				"/*text*/"=>$this->text);
    $html=strtr($html,$replacement);
  	return $html;
  }
	
	public function card(){
  	$html=file_get_contents(dirname(__FILE__)."/../templates/card_photo.inc");
    $replacement=array(
						"/*id*/"=>$this->id,
    				"/*text*/"=>$this->text);
    $html=strtr($html,$replacement);
  	return $html;

	}
    
	public function createModifySql(){
		//return $this->id."='\".mysql_real_escape_string(\"\$".$this->id."_photoUrl\").\"', ";
		return $this->id."='\".mysql_real_escape_string(stripslashes(\$_POST['$this->id'])).\"', ";
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
