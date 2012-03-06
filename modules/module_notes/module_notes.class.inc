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


class Module_notes extends Module {

	/* login
	 * string  user's login (is it a standard module or does it belong to admin panel)
	 */
	private $login;

	/* max_lines
	 * unsigned int  max number of lines allowed without scrollbar
	 */
	private $max_lines = 15;

	private $id;

	public final function __construct($informations = false){
		global $core, $db;
		
		if( is_array($informations) ){
			parent::__construct($informations);
		}
		
		if( $informations === true ){
			$this->login = 0;
		}else{
			$this->login = $core->getUserId();
		}
		
		do{
			$this->id = rand(1000000, 9999999);
		}while(isset($_SESSION['module_notes_'.$this->id]));
		
		$_SESSION['module_notes_'.$this->id] = serialize($this);
	}

	public function display() {
		global $core;
	
		echo '<p onclick="editAndSaveNotes(this,\''.$core->makeUrl('modules/module_notes/ajax/module_notes.php').'?id_notes='.$this->id.'\', \'notes_content\', 15);">';
		$this->displayContent();
		echo '</p>';
	}

	private function displayContent(){
		global $db,$core;
	
		$result = $db->query("SELECT content FROM module_notes WHERE login = '".$this->login."'");
		if( isset($result) ){
		
	    $text = $db->result($result, 0);
			if(isset($text) && ($text != '')){
				$text = $this->escapeNotesContent($text);
				echo $text;
			}else{
		    $db->query("DELETE FROM module_notes WHERE login='".$this->login."'");
				$db->query("INSERT INTO module_notes (login, content, date) VALUES ('".$this->login."', 'Cliquez sur ce texte pour le modifier', NOW())");
				echo 'Cliquez sur ce texte pour le modifier';
			}
			
		}else{
			echo 'Cliquez sur ce texte pour le modifier';
		}
																			
	}

	public function saveContent($content){
		global $db;
		
		$db->query("UPDATE module_notes SET content = '".$db->escapeString($content)."' , date=NOW() WHERE login = '".$this->login."' ");
	}

	public function escapeNotesContent($text = ''){
	  $text=stripslashes($text);
		$text=htmlentities($text,ENT_QUOTES,'UTF-8');
		$text=preg_replace("/\\r\\n|\\n|\\r/","<br />",$text); //better than nl2br() -> adds "<br />" AND removes line breaks
		return $text;
	}
											
	
}

?>
