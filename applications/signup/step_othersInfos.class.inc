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

	class OthersInfos extends Step {


	public final function __construct() {
		global $db;
		
	}


	public function display() {
		global $core, $db;
	
		echo '
		<form id="signup_step" action="'.$core->makeUrl('applications/signup/ajax/signup.php').'" method="post" onsubmit="return false;">
			<p class="step_title">
				<span>'._('Personal informations').' : </span>
			</p>
										
			<p class="sub_category_title">
				<span>'.('Me').' : </span>
			</p>

			<div class="div_input">
				<div class="div_label left">
					<label>'._('Birthday').' : </label>
				</div>
				<select name="signup_birthday" onchange="this.form.signup_isBirthdate.value=\'true\';">';

		for($i = 1; $i <= 31; $i++){
			$d = $i < 10 ? '0'.$i : $i;
	
			$sel = '';
			if( $this->dataform['signup_birthday'] == $d ){
				$sel = ' selected="selected" ';
			}
			echo '<option value="'.$d.'" '.$sel.' >'.$d.'</option>';
		}
				
		echo '</select>
				<select name="signup_birthmonth" onchange="this.form.signup_isBirthdate.value=\'true\';">';

		for($i = 1; $i <= 12; $i++){
			$m = $i < 10 ? '0'.$i : $i;
	
			$sel = '';
			if( $this->dataform['signup_birthmonth'] == $m ){
				$sel = ' selected="selected" ';
			}
			echo '<option value="'.$m.'" '.$sel.' >'.$m.'</option>';
		}
				
		echo '</select>
				<select name="signup_birthyear" onchange="this.form.signup_isBirthdate.value=\'true\';">';

		$now = getdate();
		$cur_year = $now['year'];
		$start_year = $cur_year - 100;
		$end_year = $cur_year - 5;
		for($y = $start_year; $y <= $end_year; $y++){
	
			$sel = '';
			if( $this->dataform['signup_birthyear'] == $y ){
				$sel = ' selected="selected" ';
			}elseif($this->dataform['signup_birthyear'] == ''){
				if( $y == $cur_year - 30){
					$sel = ' selected="selected" ';
				}
			}
			echo '<option value="'.$y.'" '.$sel.' >'.$y.'</option>';
		}
				
		echo '</select>
				<div class="clear"></div>
			</div>';
					
/*		echo '<div class="div_input">
						<div class="div_label left">
							<label>Sexe : </label>
						</div>
						<select class="left" onchange="if (this.value != \'none\') {'.
																					'show(gE(\'check_2\'));'.
																				'}else{'.
																					 'hide(gE(\'check_2\'));'.
																				'}'.
						'" name="signup_gender">
							<option value="none"';
							if (isset($this->dataform['signup_gender']) && $this->dataform['signup_gender'] == 'none') {
								echo 'selected ';
							}
			echo '>-</option>
							<option value="F"';
							if (isset($this->dataform['signup_gender']) && $this->dataform['signup_gender'] == 'F') {
								echo 'selected ';
							}
			echo '>Féminin</option>
							<option value="H"';
							if (isset($this->dataform['signup_gender']) && $this->dataform['signup_gender'] == 'H') {
								echo 'selected ';
							}
			echo '>Masculin</option>
						</select>
						<div id="check_2" class="check_input left" style="display: none;"></div>
						<div class="clear"></div>
					</div>';*/

/*					<p class="input_description">Quelques lignes pour parler de vos passions ou écrire ce qui vous passe par la tête.</p>';
		echo '<div class="div_input">
						<div class="div_label left">
							<label>Hobbies : </label>
						</div>
						<textarea class="left" onkeyup="if (validateInput(this, \'SOMETHING\')) {'.
																					'show(gE(\'check_3\'));'.
																				'}else{'.
																					 'hide(gE(\'check_3\'));'.
																				'}'.
						'" name="signup_hobbies" value="'.$this->dataform['signup_hobbies'].'" ></textarea>
						<div id="check_3" class="check_input left" style="display: none;"></div>
						<div class="clear"></div>
					</div>
					
					<p class="sub_category_title"><span><span class="first_letter">M</span>on activité : </span></p>';*/

		echo '
			<div class="div_input">
				<div class="div_label left">
					<label>'._('Group').' :</label>
				</div>
				<input class="left" type="text" maxlength="30" name="signup_company_name" value="'.$this->dataform['signup_company_name'].'" />
				<div class="clear"></div>
			</div>';

		echo '
			<div class="div_input">
				<div class="div_label left">
					<label>'._('Position').' :</label>
				</div>
				<input class="left" type="text" maxlength="30" name="signup_position" value="'.$this->dataform['signup_position'].'" />
				<div class="clear"></div>
			</div>
					
			<p class="sub_category_title"><span>'._('Contact').'</span></p>';

		echo '
			<div class="div_input">
				<div class="div_label left">
					<label>'._('Phone').' :</label>
				</div>
				<input class="left" onblur="validateInput(this, \'PHONE_FR\', true);" type="text" maxlength="15" name="signup_phone" value="'.$this->dataform['signup_phone'].'" />
				<div class="clear"></div>
			</div>';

		echo '
			<div class="div_input">
				<div class="div_label left">
					<label>'._('Address line 1').' :</label>
				</div>
				<input class="left" type="text" name="signup_address" maxlength="60" value="'.$this->dataform['signup_address'].'" />
				<div class="clear"></div>
			</div>';
					
		echo '
			<div class="div_input">
				<div class="div_label left">
					<label>'._('Address line 2').' :</label>
				</div>
				<input class="left" type="text" name="signup_address2" maxlength="60" value="'.$this->dataform['signup_address2'].'" />
				<div class="clear"></div>
			</div>';

		echo '
			<div class="div_input">
				<div class="div_label left">
					<label>'.('City').' :</label>
				</div>
				<input class="left" type="text" name="signup_city" maxlength="60" value="'.$this->dataform['signup_city'].'" />
				<div class="clear"></div>
			</div>';

		echo '
			<div class="div_input">
				<div class="div_label left">
					<label>'._('Zip code').'</label>
				</div>
				<input class="left" onblur="validateInput(this, \'INT\', true);" type="text" maxlength="6" size="6" name="signup_postal_code" value="'.$this->dataform['signup_postal_code'].'" />
				<div class="clear"></div>
			</div>';
/*
		echo '<div class="div_input">
						<div class="div_label left">
							<label>Pays : </label>
						</div>
						<input class="left" onkeyup="if (validateInput(this, \'SOMETHING\')) {'.
																					'show(gE(\'check_9\'));'.
																				'}else{'.
																					 'hide(gE(\'check_9\'));'.
																				'}'.
						'" type="text" name="signup_country" value="'.$this->dataform['signup_country'].'" />
						<div id="check_9" class="check_input left" style="display: none;"></div>
						<div class="clear"></div>
					</div>'; */

		echo'
			<div>
				<input type="hidden" id="step_name" name="step_name" value="othersInfos" />
				<input type="hidden" id="wope_action" name="wope_action" />
				<input type="hidden" id="control" name="control" />
				<input type="hidden" name="signup_isBirthdate" value="'.$this->dataform['signup_isBirthdate'].'" />
			</div>
		
		</form>';

		$this->displayNavBlock('false','false');

	}


	public function displayInfos() {

			echo '<p><span>'._('Birth date').' :</span>'.($this->dataform['signup_isBirthdate'] == 'true' ? $this->dataform['signup_birthday'].'/'.$this->dataform['signup_birthmonth'].'/'.$this->dataform['signup_birthyear'] : '').'</p>';
			/*echo '<p><span>Sexe : </span>'.$this->dataform['signup_gender'].'</p>';*/
			/*echo '<p><span>Hobbies : </span>'.$this->dataform['signup_hobbies'].'</p>';*/
			echo '<p><span>'._('Group').' :</span>'.stripslashes($this->dataform['signup_company_name']).'</p>';
			echo '<p><span>'._('Position').' :</span>'.stripslashes($this->dataform['signup_position']).'</p>';
			echo '<p><span>'._('Phone').'</span>'.stripslashes($this->dataform['signup_phone']).'</p>';
			echo '<p><span>'._('Address line 1').' :</span>'.stripslashes($this->dataform['signup_address']).'</p>';
			echo '<p><span>'._('Address line 2').' :</span>'.stripslashes($this->dataform['signup_address2']).'</p>';
			echo '<p><span>'._('City').' :</span>'.stripslashes($this->dataform['signup_city']).'</p>';
			echo '<p><span>'._('Zip code').' :</span>'.stripslashes($this->dataform['signup_postal_code']).'</p>';
			/*echo '<p><span>Pays : </span>'.$this->dataform['signup_country'].'</p>';*/
		
	}


	public function sendInfosToBase() {
    global $db;

		$db->query("UPDATE trombi_trombi 
		SET
			birthdate = '".($this->dataform['signup_isBirthdate'] == 'true' ? $db->escapeString($this->dataform['signup_birthyear'].'-'.$this->dataform['signup_birthmonth'].'-'.$this->dataform['signup_birthday']) : '0000-00-00')."',
			address = '".$db->escapeString($this->dataform['signup_address'])."',
			address2 = '".$db->escapeString($this->dataform['signup_address2'])."',
			postal_code = '".$db->escapeString($this->dataform['signup_postal_code'])."',
			city = '".$db->escapeString($this->dataform['signup_city'])."',
			phone = '".$db->escapeString($this->dataform['signup_phone'])."',
			company_name = '".$db->escapeString($this->dataform['signup_company_name'])."',
			position = '".$db->escapeString($this->dataform['signup_position'])."'
		
		WHERE id_user = '".$_SESSION['new_user']."'
		");
		
	}
		



}
// End of othersInfos class.

?>
