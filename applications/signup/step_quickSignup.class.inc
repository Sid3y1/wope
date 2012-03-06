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

include_once('step.class.inc');


/**************************
*
*	SIGNUP -> STEP_QUICKSIGNUP.CLASS.INC
*		First step of the signup, always used.	
*
*	@author : Sylvain Ramboz
* @date : 2006
*
***************************/

class QuickSignup extends Step {
	

	public final function __construct() {
		global $db;

	}

/**
  * \brief Minimal necessary informations to create a user (login, first name, last name, email, and password)
  *
  */
	
	public function display() {
		global $core, $db, $page;
		if(!isset($page)) $page = new Page();
		$core->loadClass('htmlblock');
		$hb = new HtmlBlock();
		$logins = $this->getExistingLogins();
		
		$login_used =	$hb->escapeTipContent('<div id="check_1" class="check_input_false left"></div><span class="error_message left">'._('This login is not available').'</span>');
		$login_not_ok =	$hb->escapeTipContent('<div id="check_1" class="check_input_false left"></div><span class="error_message left">'._('This login is not valid').'</span>');
		$login_ok = $hb->escapeTipContent('<div id="check_1" class="check_input left"></div><span class="confirmation_message left">'._('This is login available').'</span>');

		$page->msgInfo(_('Your web browser must accept cookies and Javascript if you want to use Wope.'));
	
		echo '
			<form action="'.$core->makeUrl('applications/signup/ajax/signup.php').'" method="post" id="signup_step" onsubmit="return false;">
			
				<p class="step_title">
							<span>
								'._('Basic informations').' : 
							</span>
				</p>
		';

		if($_SESSION['isMailCreated']){
			echo '<p class="input_description"><span>Attention : votre login composera également votre adresse email (identifiant@'.$core->getConfig('local').').</span></p>';
		}
		
		echo '<p class="input_description">'._('Your login must contain between 4 and 50 chars.').'</p>';

		echo '<div class="div_input">
					<div class="div_label left">
						<label>'._('Login').' :</label>
					</div>
					<input class="left" type="text" name="signup_login" id="basic_infos_login" maxlength="50" onblur="'.
						'var flag = undefined;'.
						'var logins = \''.$logins.'\';'.
						'var logins_tab = logins.split(\'|\');'.
						'var logins_nb = logins_tab.length;'.
						'for (var i = 0; i < logins_nb; i++) {'.
						'		if ( this.value.toLowerCase() == logins_tab[i].toLowerCase() ) {'.
						'			flag = true;'.
						'		}'.
						'}'.
						'if ( !validateInput(this, \'LOGIN\') ){'.
						'	gE(\'login_check_result\').innerHTML = \''.$login_not_ok.'\';'.
						'}else if(flag){'.
						' gE(\'login_check_result\').innerHTML = \''.$login_used.'\';'.
						'}else{'.
						' gE(\'login_check_result\').innerHTML = \''.$login_ok.'\';'.
						'}'.
						
						'this.onkeyup = function(){'.
							'var flag = undefined;'.
							'var logins = \''.$logins.'\';'.
							'var logins_tab = logins.split(\'|\');'.
							'var logins_nb = logins_tab.length;'.
							'for (var i = 0; i < logins_nb; i++) {'.
							'		if ( this.value.toLowerCase() == logins_tab[i].toLowerCase() ) {'.
							'			flag = true;'.
							'		}'.
							'}'.
							'if ( !validateInput(this, \'LOGIN\') ){'.
							'	gE(\'login_check_result\').innerHTML = \''.$login_not_ok.'\';'.
							'}else if(flag){'.
							' gE(\'login_check_result\').innerHTML = \''.$login_used.'\';'.
							'}else{'.
							' gE(\'login_check_result\').innerHTML = \''.$login_ok.'\';'.
							'}'.
						'}'.
				
					'" value="'.$this->dataform['signup_login'].'" />
					
				<div class="left" id="login_check_result"> </div>
			<div class="clear"></div>
				</div>
				
  		 <div class="div_input">
			
						<div class="div_label left">
							<label>'._('Password').' :</label>
						</div>

						<input class="left" onblur="'.
							'if( validateInput(this, \'PASSWORD\') ){'.
								'show(gE(\'check_2\'));'.
								'hide(gE(\'invalid_pass\'));'.
								'if( gE(\'signup_pass_conf\').value != \'\' && this.value != gE(\'signup_pass_conf\').value ){'.
									'show(gE(\'check_3_alt\'));'.
									'hide(gE(\'check_3\'));'.
								'}else{'.
									'hide(gE(\'check_3_alt\'));'.
								'}'.
							'} else {'.
							  'hide(gE(\'check_2\'));'.
								'show(gE(\'invalid_pass\'));'.
							'}'.

							'this.onkeyup = function(){'.
								'if( validateInput(this, \'PASSWORD\') ){'.
									'show(gE(\'check_2\'));'.
									'hide(gE(\'invalid_pass\'));'.
									'if( gE(\'signup_pass_conf\').value != \'\' && this.value != gE(\'signup_pass_conf\').value ){'.
										'show(gE(\'check_3_alt\'));'.
										'hide(gE(\'check_3\'));'.
									'}else{'.
										'hide(gE(\'check_3_alt\'));'.
									'}'.
								'} else {'.
								  'hide(gE(\'check_2\'));'.
									'show(gE(\'invalid_pass\'));'.
								'}'.
							'}'.
							
						'" type="password" name="signup_pass" id="signup_pass" value="'.$this->dataform['signup_pass'].'" />
						<div id="check_2" class="check_input left" style="display: none;"></div>
						<div id="invalid_pass" style="display: none;"><div class="check_input_false left"></div><span class="error_message left" >'._('The password you entered is not valid.').'</span></div>
						<div class="clear"></div>
				</div>
				
			  <div class="div_input">
			
						<div class="div_label left">
							<label>'._('Password').' :</label>
						</div>
						<input type="password" class="left" onblur="'.
							'if( this.value == gE(\'signup_pass\').value && this.value != \'\' ){'.
								'removeClassName(this.parentNode, \'error\');'.
								'show(gE(\'check_3\'));'.
								'hide(gE(\'check_3_alt\'));'.
							'}else{'.
								'addClassName(this.parentNode, \'error\');'.
								'show(gE(\'check_3_alt\'));'.
								'hide(gE(\'check_3\'));'.
							'}'.
							'this.onkeyup = function(){'.
								'if( gE(\'signup_pass_conf\').value == gE(\'signup_pass\').value &&  gE(\'signup_pass_conf\').value != \'\' ){'.
									'removeClassName(gE(\'signup_pass_conf\').parentNode, \'error\');'.
									'show(gE(\'check_3\'));'.
									'hide(gE(\'check_3_alt\'));'.
								'}else{'.
									'addClassName(gE(\'signup_pass_conf\').parentNode, \'error\');'.
									'show(gE(\'check_3_alt\'));'.
									'hide(gE(\'check_3\'));'.
								'}'.
							'}'.
																				
						'" name="signup_pass_confirmation" id="signup_pass_conf" value="'.$this->dataform['signup_pass_confirmation'].'" />
						<div id="check_3" class="check_input left" style="display: none;"></div>
						<div id="check_3_alt" style="display: none;">
							<div class="check_input_false left"></div>
								<span>'._('The passwords you entered do not match.').'</span>
						</div>
						<div class="clear"></div>
				</div>'
				
			.'<div class="div_input">
			
						<div class="div_label left">
							<label>'._('Last name').' :</label>
						</div>
						<input class="left" maxlength="30" onblur="'.
							'if (validateInput(this, \'SOMETHING\')) {'.
								'hide(gE(\'no_lastname\'));'.
								'show(gE(\'check_4\'));'.
							'}else{'.
								'show(gE(\'no_lastname\'));'.
								'hide(gE(\'check_4\'));'.
							'}'.
							'this.onkeyup = function(){'.
								'if (validateInput(this, \'SOMETHING\')) {'.
									'hide(gE(\'no_lastname\'));'.
									'show(gE(\'check_4\'));'.
								'}else{'.
									'show(gE(\'no_lastname\'));'.
									'hide(gE(\'check_4\'));'.
								'}'.
							'}'.
						'" type="text" name="signup_lastname" value="'.$this->dataform['signup_lastname'].'" />
						<div id="check_4" class="check_input left" style="display: none;"></div>
						<div id="no_lastname" style="display: none;"><div class="check_input_false left"></div><span class="error_message left" >'._('This field is mandatory').'</span></div>
						<div class="clear"></div>
				</div>'
				
			.'<div class="div_input">
			
						<div class="div_label left">
							<label>'._('First name').' :</label>
						</div>
						<input class="left" maxlength="30" onblur="'.
							'if (validateInput(this, \'SOMETHING\')) {'.
								'hide(gE(\'no_firstname\'));'.
								'show(gE(\'check_5\'));'.
							'}else{'.
								'show(gE(\'no_firstname\'));'.
								'hide(gE(\'check_5\'));'.
							'}'.
							'this.onkeyup = function(){'.
								'if (validateInput(this, \'SOMETHING\')) {'.
									'hide(gE(\'no_firstname\'));'.
									'show(gE(\'check_5\'));'.
								'}else{'.
									'show(gE(\'no_firstname\'));'.
									'hide(gE(\'check_5\'));'.
								'}'.
							'}'.
						'" type="text" name="signup_firstname" value="'.$this->dataform['signup_firstname'].'" />
						<div id="check_5" class="check_input left" style="display: none;"></div>
						<div id="no_firstname" style="display: none;"><div class="check_input_false left"></div><span class="error_message left" >'._('This field is mandatory').'</span></div>
						<div class="clear"></div>
				</div>';
				
			
			echo '<div class="div_input">

						<div class="div_label left">
							<label>'._('E-mail').' :</label>
						</div>
						<input class="left" type="text" maxlength="150" name="signup_email" value="'.$this->dataform['signup_email'].'" onblur="';

		if( !$_SESSION['isMailCreated'] ){
		
			echo 'if (validateInput(this, \'EMAIL\')) {'.
				'hide(gE(\'no_email\'));'.
				'show(gE(\'check_6\'));'.
			'}else{'.
				'show(gE(\'no_email\'));'.
				'hide(gE(\'check_6\'));'.
			'}'.
			'this.onkeyup = function(){'.
				'if (validateInput(this, \'EMAIL\')) {'.
					'hide(gE(\'no_email\'));'.
					'show(gE(\'check_6\'));'.
				'}else{'.
					'show(gE(\'no_email\'));'.
					'hide(gE(\'check_6\'));'.
				'}'.
			'}';
		
		}else{
			echo 'validateInput(this, \'EMAIL\', true);';
		}
		
		echo '" />
						<div id="check_6" class="check_input left" style="display: none;"></div>
						<div id="no_email" style="display: none;"><div class="check_input_false left"></div><span class="error_message left" >'._('This field is mandatory').'</span></div>
						<div class="clear"></div>
				</div>

			<div>	
				<input type="hidden" id="step_name" name="step_name" value="quickSignup" />
				<input type="hidden" id="wope_action" name="wope_action" />
				<input type="hidden" id="control" name="control" />
			</div>
			
		</form>';

		$this->displayNavBlock('false','true');

		
	}

	
	public function returnErrors($infos) {
		global $core;
	
		$core->loadClass('form.class.inc');
		$form = new Form();
		$flag = false;
		$errors = Array();
		
		foreach ($infos as $name => $value) {
		
			switch ($name) {
			
				case 'signup_login':
					if( $form->verifForm($this->dataform[$name],'login') != true ){
						$flag = true;
						$errors[] = _('The login you entered is not valid');
					}
					$logins = $this->getExistingLogins(true);
					if( in_array($this->dataform[$name], $logins) ){
						$flag = true;
						$errors[] = _('This login is not available');
					}
					break;
					
				case 'signup_pass':
					if ($form->verifForm($this->dataform[$name],'password') != true) {
		    		$flag = true;
						$errors[] = _('The password you enterd is not valid');
					}
					break;
					
				case 'signup_pass_confirmation':
					if ($this->dataform['signup_pass'] != $this->dataform['signup_pass_confirmation']) {
					 	$flag = true;
						$errors[] = _('The passwords you entered do not match');
					}
					break;
					
				case 'signup_lastname':
				  if ($this->dataform[$name] == '') {
					  $flag = true;
						$errors[] = _('You must fill in the last name field');
					}
					break;
					
				case 'signup_firstname':
				  if ($this->dataform[$name] == '') {
					  $flag = true;
						$errors[] = _('You must fill in the first name field');
					}
					break;

				case 'signup_email':
					if ( !$_SESSION['isMailCreated'] && $this->dataform[$name] == '' ) {
		        $flag = true;
		        $errors[] = _('You must fill in the E-mail field');
		      }
				  if($this->dataform[$name] != '' && $form->verifForm($this->dataform[$name],'email') != true ){
					 	$flag = true;
						$errors[] = _('The e-mail you entered is not valid');
					}
					break;

				default:
					//empty
					break;

			}
		
		}
		return !$flag ? $flag : $errors;
	}

	
	public function displayInfos() {
		global $core;

		echo '<p><span>'._('Login').' :</span>'.stripslashes($this->dataform['signup_login']).'</p>';
		echo '<p><span>'._('Last name').' :</span>'.stripslashes($this->dataform['signup_lastname']).'</p>';
		echo '<p><span>'._('First name').' :</span>'.stripslashes($this->dataform['signup_firstname']).'</p>';
		echo '<p><span>'._('E-mail').' :</span>'.stripslashes($this->dataform['signup_email']).'</p>';
		if($_SESSION['isMailCreated']){
			echo '<p><span>E-mail créé : </span>'.$this->dataform['signup_login'].'@'.$core->getConfig('local').'</p>';
		}
		
	}


	public function sendInfosToBase() {
		global $core, $db;
		
		if ($_SESSION['activeUser'] == true) {
			$conf = 'Y';
		} else {
				$conf = 'N';
			}
		
		$_SESSION['new_user'] = $this->dataform['signup_login'];
		$db->query("INSERT INTO usersInfo (login, pass, actif, dateCreation) 
		VALUES (
			'".$this->dataform['signup_login']."',
			PASSWORD('".$this->dataform['signup_pass']."'),
			'".$conf."',
			NOW()
		)");

		// We store the id of the new user to update the avatar of the good person after
		$user_id = $db->result($db->query("SELECT id FROM usersInfo WHERE login = '".$_SESSION['new_user']."'"), 0);
		$_SESSION['new_user'] = (int)$user_id;

		$right = $_SESSION['newAccountRight'];

		$db->query("INSERT INTO kernel_right_user_get (login, idRight) VALUES ('".$user_id."', ( SELECT id FROM kernel_right WHERE name='".$right."' ) ) ");

		// We store the others informations in the trombi_trombi table
		$request = "INSERT INTO trombi_trombi (id_user,lastname,firstname,nickname,email,email2) 
		VALUES (
			'".$_SESSION['new_user']."',
			'".$db->escapeString($this->dataform['signup_lastname'])."',
			'".$db->escapeString($this->dataform['signup_firstname'])."',
			'".$db->escapeString($this->dataform['signup_firstname'].' '.$this->dataform['signup_lastname'])."', ";
			
			if($_SESSION['isMailCreated']){
				$request .= "'".$db->escapeString($this->dataform['signup_login'].'@'.$core->getConfig('local'))."',
				'".$db->escapeString($this->dataform['signup_email'])."'";
			}else{
				$request .= "'".$db->escapeString($this->dataform['signup_email'])."',
				''";
			}
		$request .= ")";
				
		$db->query($request);
	}


	public function getExistingLogins($as_array = false) {
		global $core, $db;

		$res = $db->query("SELECT LOWER(login) AS login FROM usersInfo");
	
		if($as_array){
			$logins = Array();
		}else{
			$logins = '';
		}
		
		while ($row = $db->fetchArray($res)) {
			if($as_array){
				$logins[] = $row['login'];
			}else{
				$logins .= $row['login'].'|';
			}
		}
		return $logins;
	
	}


}
//End of class QuickSignup.

?>
