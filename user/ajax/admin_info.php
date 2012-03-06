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

	$SECU='log';
	include_once('../../include/main.inc');
	include_once('../user_admin.class.inc');
	$user_panel = unserialize($_SESSION['user_panel_'.$_REQUEST['id_admin']]);
	$page = new Page();

	switch($_REQUEST['wope_action']){
		case 'display_change_foto':
			$user_panel->displayChangeInfoFoto();
			break;
		case 'display_change_personal':
			$user_panel->displayChangeInfoPersonal();
			break;
		case 'display_change_contact':
			$user_panel->displayChangeInfoContact();
			break;
		case 'display_change_address':
			$user_panel->displayChangeInfoAddress();
			break;
		case 'delete_user_foto':
			$user_panel->deleteUserFoto($_GET['foto']);
			$user_panel->displayChangeInfoFoto();
			break;
		case 'update_user_foto':
			$user_panel->updateUserFoto($_POST['old_user_foto'],((isset($_FILES['new_user_foto']))?$_FILES['new_user_foto']:''));
			break;
		case 'save_user_prefs':
			$user_panel->saveUserPrefs($_POST['pref']);
			$break;
		case 'user_infos_saved':
			$page->msgInfo('Informations sauvegardées.');
			break;								
		default:
			//void
			break;
	}
?>
