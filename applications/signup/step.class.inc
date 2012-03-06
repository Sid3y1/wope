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

class Step {

	/*
	 * id of the object
	 */
	private $id;

	/*
	 * path to access the object
	 */
	private $path;

	/*
	 * informations entered by the user during this step
	 */
	public $dataform;


	public function __construct(){
	
	}

	public function display() {
	
	}
		
	public function returnErrors() {
		return false;
	}

	public function displayInfos() {

	}
	
	public function sendInfosToBase() {

	}
	
	public function saveInfos($infos) {

		foreach ($infos as $name => $value) {
			$this->dataform[$name] = $value;
		}
		
	}



	public function displayNavBlock($file_or_not,$control_or_not) {
		global $core;
	
		echo '<div class="clear"></div>
		<div class="nav_block">';
			
		switch($control_or_not) {

			case 'false':
			
				if ($_SESSION['cur_step'] > '1') {
					echo '<a class="admin_link left" onclick="'.
						'gE(\'wope_action\').value = \'previous_step\';'.
						'gE(\'control\').value = \'false\';'.
						'var form = gE(\'signup_step\');'.
						'AJAX.submitAndUpdate(form, '.$file_or_not.', \'custom_form\');'.
						'gE(\'current_step\').innerHTML = '.$_SESSION['cur_step'].' - 1;'.
					'">'._('Previous step').'</a>';
				}

				if ($_SESSION['cur_step'] >= '1' && $_SESSION['cur_step'] < $_SESSION['steps_number']) {
					echo '<a class="admin_link left" onclick="'.
						'gE(\'wope_action\').value = \'next_step\';'.
						'gE(\'control\').value = \'false\';'.
						'var form = gE(\'signup_step\');'.
						'AJAX.submitAndUpdate(form, '.$file_or_not.', \'custom_form\');'.
						'gE(\'current_step\').innerHTML = '.$_SESSION['cur_step'].' + 1;'.
					'">'._('Next step').'</a>';
				}

				if ($_SESSION['cur_step'] == $_SESSION['steps_number']) {
					echo '<a class="admin_link left" onclick="'.
						'gE(\'wope_action\').value = \'finish_signup\';'.
						'gE(\'control\').value = \'false\';'.
						'var form = gE(\'signup_step\');'.
						'AJAX.submitAndUpdate(form, '.$file_or_not.', \'custom_form\');'.
					'">Terminer</a>';
				}

				break;
		
			case 'true':
		
				if ($_SESSION['cur_step'] > '1') {
					echo '<a class="admin_link left" onclick="'.
						'gE(\'wope_action\').value = \'previous_step\';'.
						'gE(\'control\').value = \'true\';'.
						'var form = gE(\'signup_step\');'.
						'AJAX.submitAndUpdate(form, '.$file_or_not.', \'custom_form\');'.
						'gE(\'current_step\').innerHTML = '.$_SESSION['cur_step'].' - 1;'.
					'">'._('Previous step').'</a>';
				}

				if ($_SESSION['cur_step'] >= '1' && $_SESSION['cur_step'] < $_SESSION['steps_number']) {
					echo '<a class="admin_link left" onclick="'.
						'gE(\'wope_action\').value = \'next_step\';'.
						'gE(\'control\').value = \'true\';'.
						'var form = gE(\'signup_step\');'.
						'AJAX.submitAndUpdate(form, '.$file_or_not.', \'custom_form\');'.
						'gE(\'current_step\').innerHTML = '.$_SESSION['cur_step'].' + 1;'.
					'">'._('Next step').'</a>';
				}

				break;

			default:
				//void
				break;
		
		}
		
		echo '<div class="clear"></div>
		</div>';

	}

}//End of class Step.

?>
