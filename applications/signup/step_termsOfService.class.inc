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

	class TermsOfService extends Step {


	public final function __construct() {
		global $db;
		
	}


	public function display() {
		global $core, $db;
		include(dirname(__FILE__).'/config_signup.inc');
	
		echo '<p class="step_title">
								<span>
										<span class="first_letter">'._('Term of service').' : 
								</span>
					</p>

			<form action="'.$core->makeUrl('applications/signup/ajax/signup.php').'" method="post" id="signup_step" onsubmit="return false;">
			
				<div>
					<textarea id="terms_of_service" name="terms_of_service" readonly="readonly">'.$config['tos'].'</textarea>
				</div>
				<div class="div_input">
					<div class="terms_of_service_label left">
						<label>'._('I accept the terms of service').'</label>
					</div>
					<input class="checkbox_input left" type="checkbox" name="accept_terms" ';
								
		if ($this->dataform['accept_terms'] == 'terms_accepted') {
			echo ' checked="checked" ';
		}
		
		echo ' value="terms_accepted" />
					<div class="clear"></div>
				</div>
				<div>					
					<input type="hidden" id="step_name" name="step_name" value="termsOfService" />
					<input type="hidden" id="wope_action" name="wope_action" />
					<input type="hidden" id="control" name="control" />
				</div>
				
			</form>';
	
		$this->displayNavBlock('false','true');


	}

	public function saveInfos($infos) {

		foreach ($infos as $name => $value) {
			if($name != 'action'){
				$this->dataform[$name] = $value;
			}
		}
		
	}



  public function returnErrors($infos) {
		global $core;

		$core->loadClass('form.class.inc');
		$form = new Form();
		$flag = false;
		$errors = Array();

		if ($infos['accept_terms'] != 'terms_accepted') {
			$flag = true;
			$errors[] = _('You must accept the terms of service');
		}
	
		return !$flag ? $flag : $errors;
  }
									

}
// End of TermsOfService class.

?>
