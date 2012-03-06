-<?php
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

	class AvatarInfos extends Step {


	public final function __construct() {
		global $db;
	}


	/**
	* \brief AvatarInfos
	*
	*/

	public function display() {
		global $core, $db;

		echo '
		<form action="'.$core->makeUrl('applications/signup/ajax/signup.php').'" method="post" id="signup_step" onsubmit="return false;" >		
			<p class="step_title">
				<span>'._('Upload your avatar : ').'</span>
			</p>
			<p class="input_description">'._('Keep in mind that your avatar will be resize to be 75 pixel width.').'</span>.</p>';

		echo '
			<div class="div_input avatar_preview_block">
				<div class="div_label avatar_label_preview left">
					<label>'._('Preview').' : </label>
				</div>
				<div class="avatar_preview left">';
							
		if (isset($this->dataform['is_uploaded']) && $this->dataform['is_uploaded'] == 'yes') {
			echo '<img src="'.$_SESSION['avatar_url'].'" />';
		}else{
			echo '<div class="default_avatar">&nbsp;</div>';
		}
							
		echo '</div>
				<div class="clear"></div>
			</div>';

					
		echo '
			<div class="div_input">
				<div class="div_label left">
					<label>'._('Upload an image').' : </label>
				</div>

				<input class="left" type="file" name="signup_avatar" />
				<div class="clear"></div>
			</div>
			<div class="clear"></div>
				
			<div>
				<input type="hidden" id="step_name" name="step_name" value="avatarInfos" />
				<input type="hidden" id="wope_action" name="wope_action" />
				<input type="hidden" id="control" name="control" />
			</div>
			
		</form>';
		
		echo '<p>
			<a class="link_button" onclick="'.
				'gE(\'wope_action\').value = \'preview_avatar\';'.
				'gE(\'control\').value = \'false\';'.
				'var form = gE(\'signup_step\');'.
				'AJAX.submitAndUpdate(form, true, \'custom_form\');'.
			'">'._('Preview').'...</a>
		</p>';


		$this->displayNavBlock('true','false');

	}


	public function previewAvatar() {
		global $core;

		if (isset($_FILES['signup_avatar']['tmp_name']) && $_FILES['signup_avatar']['tmp_name'] != '') {
		
			if (isset($this->dataform['is_uploaded']) && $this->dataform['is_uploaded'] == 'yes') {
				unlink($core->getConfig('baseServer').'/tmp/'.$_SESSION['steps_id'].'.jpg');
			}

    	$avatar_url = $core->getConfig('baseServer').'/tmp/'.$_SESSION['steps_id'].'.jpg';
			
			// We store the real name of the file for the last operation done on the avatar
			if (isset($_SESSION['avatar_original_name']) && $_SESSION['avatar_original_name'] != '') {
				$_SESSION['avatar_original_name'] = $_FILES['signup_avatar']['name'];
			}
			
		  $_SESSION['avatar_url'] = $core->makeUrl('tmp/').$_SESSION['steps_id'].'.jpg';			 

			move_uploaded_file($_FILES['signup_avatar']['tmp_name'], $avatar_url);
	 		$core->loadClass('file');
	 		$file = new file();
	 		$file->image_max_size($avatar_url,75,75);
			chmod ($avatar_url, 0774);
			$this->dataform['is_uploaded'] = 'yes';

			//The line below is used to debug
			//print_r($_FILES);
		}
		
	}

	
	public function sendInfosToBase() {
		global $core;
	
		$core->loadClass('filemanager');
		$file_m = new FileManager();
		
		if( isset($this->dataform['is_uploaded']) && $this->dataform['is_uploaded'] == 'yes' ){
			$file_m->moveFileToFm($core->getConfig('tmpDir').'/'.$_SESSION['steps_id'].'.jpg', $_SESSION['avatar_original_name'], true);
		}

	}




}
// End of AvatarInfos class.

?>
