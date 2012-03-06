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



$SECU = 'all';
$relative_path = '../../..';
include_once($relative_path.'/include/main.inc');

include_once('../signup.class.inc');
include_once('../step_quickSignup.class.inc');
include_once('../step_avatarInfos.class.inc');
include_once('../step_othersInfos.class.inc');
include_once('../step_termsOfService.class.inc');
include_once('../step_summary.class.inc');


$signup = unserialize($_SESSION['signup_'.$_SESSION['steps_id']]);

$_SESSION['step_name'] = $_POST['step_name'];

switch($_POST['wope_action']) {
	
	case 'preview_avatar':

		$temp_object = unserialize($_SESSION[$_SESSION['step_name'].'_'.$_SESSION['steps_id']]);
		if( isset($_FILES['signup_avatar']['tmp_name']) ){
			$temp_object->previewAvatar();
			$_SESSION[$_SESSION['step_name'].'_'.$_SESSION['steps_id']] = serialize($temp_object); 
		}
		$temp_object->display();
		break;


	case 'next_step':
		
		$temp_object = unserialize($_SESSION[$_SESSION['step_name'].'_'.$_SESSION['steps_id']]); 

		// Exception of the avatar step case
		if ($_SESSION['step_name'] == 'avatarInfos' && isset($_FILES['signup_avatar']['tmp_name'])) {
			$temp_object->previewAvatar();
		} 

		// Normal step case, we save the infos
		if ($_SESSION['step_name'] != 'summary') {
			$temp_object->saveInfos($_POST);
		}
		
		$_SESSION[$_SESSION['step_name'].'_'.$_SESSION['steps_id']] = serialize($temp_object); 
		
		// First, we check if a PHP control of the fields is needed. Then, two cases :
				// -> Case of an error in the fields
		
		if( isset($_POST['control']) && $_POST['control'] == 'true' && $errors = $temp_object->returnErrors($_POST) ){
			
					if( is_array($errors) && count($errors) != 0){
						$msg = '<ul>';
						foreach($errors as $error){
							$msg .= '<li>'.$error.'</li>';
						}
						$msg .= '</ul>';
						global $page;
						$page = new page();
						$page->msgError($msg);
					}else{
						echo '<div class="message_error">Une erreur a été commise ou vous avez oublié un champ obligatoire !</div>';
					}
			
			$temp_object->display();

				// -> No errors have been done
		} else {
				$_SESSION['cur_step']++;
				$_SESSION['array_cur_pos']++;
				$signup->displayStep($signup->config['steps'][$_SESSION['array_cur_pos']],'../');
			}
			
		break;
	
	
	case 'previous_step':
	
		$temp_object = unserialize($_SESSION[$_SESSION['step_name'].'_'.$_SESSION['steps_id']]); 
	
		if ($_SESSION['step_name'] == 'avatarInfos' && isset($_FILES['signup_avatar']['tmp_name'])) {
			$temp_object->previewAvatar();
		} 
		
		if ($_SESSION['step_name'] != 'summary') {
				$temp_object->saveInfos($_POST);
		}

		$_SESSION[$_SESSION['step_name'].'_'.$_SESSION['steps_id']] = serialize($temp_object); 
				
		if (isset($_POST['control']) && $_POST['control'] == 'true' && $errors = $temp_object->returnErrors($_POST) ){
			
					if( is_array($errors) && count($errors) != 0){
						echo '<ul>';
						foreach($errors as $error){
							echo '<li>'.$error.'</li>';
						}
						echo '</ul>';
					}else{
						echo '<div class="message_error">Une erreur a été commise ou vous avez oublié un champ obligatoire !</div>';
					}
			
			$temp_object->display();
			
		} else {
				$_SESSION['cur_step']--;
				$_SESSION['array_cur_pos']--;
				$signup->displayStep($signup->config['steps'][$_SESSION['array_cur_pos']],'../');
			}

		break;


	case 'finish_signup':
		
		$signup->storeInfos();
		// End Message is displayed
		echo $signup->config['endMessage'];
		
		$quick_signup = unserialize($_SESSION['quickSignup_'.$_SESSION['steps_id']]); 
		$_SESSION['connect_user'] = $quick_signup->dataform['signup_login'];
		$_SESSION['connect_pass'] = $quick_signup->dataform['signup_pass'];
		
		echo '<p><a href="'.$core->makeUrl('index.php').'">Me connnecter</a></p>';
		break;


	default:
		//void
		break;
}
?>
