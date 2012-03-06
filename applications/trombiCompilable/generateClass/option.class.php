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

class Option {
	private $query="";
	private $options=null;
	private $modeDb=true;
	private $maxSizeOptionId=0;
	private $maxSizeOptionValue=0;
	
	public function __construct() {
		$this->options=null;
		$this->maxSizeOptionId=0;
		$this->modeDb=true;
		$this->maxSizeOptionValue=0;
	}
    
    public function setOptionDb($maxSizeOptionId) {
		$this->modeDb=true;
		$this->maxSizeOptionId=$maxSizeOptionId;
	}
	
	public function addOption($id,$value) {
		$this->modeDb=false;
		$tab=array();
		$tab["id"]=$id;
		$tab["value"]=$value;
		if ($this->options==null) {
			$this->options=array();
		}
		$this->options[]=$tab;
	}
	
	public function isModeDb() {
		return $this->modeDb;
	}
	
	public function getMaxSizeOptionId() {
		if ($this->maxSizeOptionId==0) {
			$max=0;
			foreach ($this->options as $option) {
				if (strlen($option['id'])>$max) {
					$max=strlen($option['id']);	
				}	
			}
			$this->maxSizeOptionId=$max;
		}
		return $this->maxSizeOptionId;
	}
	
	public function getMaxSizeOptionValue() {
		if ($this->maxSizeOptionValue==0) {
			foreach ($this->options as $option) {
				if (strlen($option['value'])>$this->maxSizeOptionValue) {
					$this->maxSizeOptionValue=strlen($option['value']);	
				}
			}
		}
		return $this->maxSizeOptionValue;
	}
	
	public function sql() {
		$ret="";
		if (!$this->modeDb) {
			foreach ($this->options as $option) {
				$ret.="INSERT INTO `/*id*/` VALUES ('".$option["id"]."','".$option["value"]."');\n";	
			}
		}
		return $ret;	
	}
        
    public function toString() {
    	$return = "Option :\n"
    		."  Query = ".$this->query."\n"
    		."  MaxSizeOptionId = ".$this->getMaxSizeOptionId()."\n";
    	if (!$this->modeDb) {
    		$return .="  Options = \n";
    		foreach ($this->options as $option) {
    			$return.="\t[".$option['id'].",".$option['value']."]\n";
    		}
    	}
    	return $return;
    }
}
?>