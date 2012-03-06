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

	class Summary extends Step {

		public final function __construct() {
		
		}

		public function display() {
			global $core, $db;
		
			include_once('config_signup.inc');

			echo '
			<p class="step_title"><span>'._('Summary').' :</span></p>
			<div class="summary_text left">';
			
			for ($i = 0; $i < $config['stepsNumber'] - 1; $i++) {
				$class_name = substr($config['steps'][$i], 5, 100);
				include_once('step_'.$class_name.'.class.inc');
				$temp_object = unserialize($_SESSION[$class_name.'_'.$_SESSION['steps_id']]); 

				if ($class_name != 'avatarInfos' && $class_name != 'termsOfService') {
					$temp_object->displayInfos();
				}
				
				$is_uploaded = false;
				if ($class_name == 'avatarInfos') {
					$is_uploaded = $temp_object->dataform['is_uploaded'];
				}
			
				$_SESSION[$class_name.'_'.$_SESSION['steps_id']] = serialize($temp_object);
			}
							
			echo '</div>';


			if( $is_uploaded == 'yes' ){
				echo '
				<div class="summary_avatar right">
					<p><span>'._('Your avatar').' :</span></p>
					<img src="'.$_SESSION['avatar_url'].'" />
				</div>
				<div class="clear"></div>';
			}
				
			echo'
			<form action="'.$core->makeUrl('applications/signup/ajax/signup.php').'" method="post" id="signup_step" onsubmit="return false;" >
				<div>
					<input type="hidden" id="step_name" name="step_name" value="summary" />
					<input type="hidden" id="wope_action" name="wope_action" />
					<input type="hidden" id="control" name="control" />
				</div>
			</form>';
		
			$this->displayNavBlock('false','false');

		}

	}//End of Summary class.

?>
