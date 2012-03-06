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


class Form {

	private $session;
	
	public final function __construct(  ) {
		global $session;
		$this->session = $session;
	}
	
	
	/**
	 * mailto  cree completement le lien d'envoi d'un email, avec formulaire ajax pour envoyer directement le mail
	 * 
	 * @param string $email 
	 * @param string $cc 
	 * @param string $cci 
	 * @param string $subject 
	 * @param string $body 
	 * @access public
	 * @return void
	 */
	public function mailto ($email='',$cc='',$cci='',$subject='',$body='') {
		global $core;
		$form = '<form action="'.$core->makeUrl('include/ajax/framework.php?action=email&mail='.$email.'').'" onsubmit="this.makeAndCall(function(){ dialBox.erase();dialBox.draw(this.responseText)}); this.onsubmit = function(){return false}; return false;" method="post">
			<p class="form-input"><label for="subject">to:'.$email.'</label>
      <input type="text" id="subject" name="subject" value="'.$subject.'" /></p>
      <p class="form-textarea"><textarea name="body">'.$body.'</textarea></p>
      <p class="form-submit"><input type="submit" name="Send" /></p>
      </form>';	
		echo '<span class="mailTo"><a class="popup" onclick="dialBox.draw(\''.htmlentities(str_replace("\n", "<br />", $form), ENT_QUOTES).'\');">[M]</a><a href="'.$this->mail($email,$cc,$cci,$subject,$body).'" > '.$email.' </a></span>';	
	}
	
	
	/**
		* Fonction verifForm
		* Verifie la validite d'une donnee
		*
		* @param $value Valeur a tester
		* @param $fieldType Type du test a? effectuer sur la valeur
		* @return Boolean , true = ok
		*/
	public function verifForm ($value, $fieldType) {
		switch ($fieldType) {

			//Email
			case 'email':
				$result = eregi("^[a-z0-9._-]+@[a-z0-9._-]{2,}\.[a-z]{2,4}$",$value);
				return $result;

			//Adresse http/ftp
			case 'http':
				$result = eregi("^(https?|ftp)://(w{3}\.)?(.+)(\.[a-z0-9_-]+)+$",$value);
				$result=1;
				return $result;

			//tel
			case 'tel':
				$result = ereg("^[0-9]+$",$value);
				return $result;

			//Texte restreint
			case 'login':
				$result = ereg("^[_\.[:alnum:]]{4,50}$",$value);
				return $result;

			//Password
			case 'password':
			$result = ereg("^[a-zA-Z0-9_\*\$@&%\.,;:+!?=#-]{6}[a-zA-Z0-9_\*\$@&%\.,;:+!?=#-]*$", $value);
				return $result;
			
			//Texte mais sans balise
			case 'txtBrut':

				break;

			//Avec plein de trucs
			case 'txtRich':

				break;
				
			case 'money':
				$result = ereg('^[[:digit:]]+([,\.][[:digit:]]{1,2})?$',$value);
				return $result;
			case 'int':
				$result = ereg('^[[:digit:]]+$',$value);
				return $result;
		}
	}



	/**
	 * selectFromDb displays options of a select input. Informations are loaded from DB
	 *
	 * @param $table      Table in which informations are
	 * @param $field      Field of $table in which informations are (what we see in select)
	 * @param $condition  "WHERE" condition of sql request (* JOIN is also allowed)
	 * @param $prompt     Message to display as a prompt -> selected and disabled option
	 * @param $selected   Selected option(s) (in that case, $prompt is not selected)
	 * @param $valueAtt		Options "value" attribute is set to $valueAtt (a field of $table)
	 *                     instead of $field (but text of options is still $field value)
	 * @return void
	 */
  public function selectFromDb($table,$field,$condition,$prompt,$selected,$valueAtt){
		global $db;
		if(isset($valueAtt) && ($valueAtt != '')){
			$sql = 'SELECT '.$table.'.'.$field.', '.$table.'.'.$valueAtt.' FROM '.$table.' '.$condition.' ORDER BY '.$field.' ASC ;';
		}else{
		  $valueAtt = $field;
			$sql = 'SELECT '.$table.'.'.$field.' FROM '.$table.' '.$condition.' ORDER BY '.$field.' ASC ;';
		}
		$sel='';
		if( isset($prompt)&& ($prompt != '') ){
		  if(!isset($selected) || ($selected == '')){
		    $sel=' selected="selected" ';
			}
			echo '<option value="NULL" disabled="disabled" '.$sel.' >'.$prompt.'</option>';
		}
		$data = $db->query($sql);
		while( $row = $db->fetchArray($data) ){
			$sel = '';
			//multiple select : $selected is an array
			if( isset($selected) && is_array($selected) ){
				if( in_array( stripslashes($row[$valueAtt]), $selected )){
					$sel = ' selected="selected" ';
				}
			//simple select
		  }elseif( isset($selected) && ($selected != '') ){
			  if( stripslashes($row[$valueAtt]) == $selected ){
			    $sel = ' selected="selected" ';
			  }
			}
			echo '<option value="'.stripslashes($row[$valueAtt]).'" '.$sel.' >'.stripslashes($row[$field]).'</option>';
		}
	}


	/**
	 * This method links a calendar to an input of your form,
	 * i.e. it creates a button on which you can click to make
	 * a calendar appear and let you select a date to fill the input.
	 *
	 * @param  input_id  the id of the input to fill
	 * @param  locale the locale the calendar has to use
	 * @return      the HTML code of the button and the calendar container
	 */
	public function linkCalendarTo($input_id, $locale = 'FR'){
		return '<a title="'.locale::display('calendar', 'kernel').'" class="calendar_button" onclick="selectDate(\'cal_container\', function(date){gE(\''.$input_id.'\').value = date;}, true, \''.$locale.'\');">&nbsp;</a><span id="cal_container"></span>';
	}
	

}

?>
