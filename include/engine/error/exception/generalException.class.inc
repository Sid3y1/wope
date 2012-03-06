<?php
/**
*   Copyright : (C) 2007 Wope
*   License : GNU GPL
*   Contact : http://www.wope-project.org
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
*   MA  02111-1307  USA
*
**/


class GException extends Exception {

	protected $exceptionLevel;	// With this attribut we can choose to display the message at a particular debug level
	protected $siteLevel;				// The debug level of the site (in config_secure.inc)
	public $userMessage; 		// This message is used to informe the end-user of an error whereas the classic exception message is used for debugging purpose and is only displayed at a particular debug level
	private	$type; // The type of the exception (fileException, dbException, ...)

	public function __construct($debugMessage, $code = 0, $exceptionLevel = 1, $userMessage = ''){
		global $core;
		parent::__construct($debugMessage,$code);
		$this->exceptionLevel = $exceptionLevel;
		$this->siteLevel = $core->getConfig('debugLevel');
		$this->userMessage = $userMessage;
		$this->type = "general";
	}

	public function setType($type){
		$this->type=$type;
	}

	public function getIfLevel($data){
		$result = false;
		if($this->exceptionLevel < $this->siteLevel){
			$result = $data;
		}
		
		return $result;
	}	

	public function formatDebugDisplay($data){
		$message= '<div class="debug">'
						.'<h4>'.$this->type.' error #'.$this->getCode().'</h4>'
						.'<p>'
							.$data
						.'</p>'
					.'</div>';
		return $this->getIfLevel($message);

	}

	public function getUserMessage(){
		return $this->userMessage;
	}

	public function getFullDebugMessage(){
		$message= '<strong>'.$this->getMessage().'</strong><br />'
						.'In the file : '.$this->getFile().'<br />'
						.'At line : '.$this->getLine().'<br />'
						.'The full trace is : <br />'
						.'<pre>'.$this->getTraceAsString().'</pre>';
		return $this->formatDebugDisplay($message);
	}
	
}// End class


?>
