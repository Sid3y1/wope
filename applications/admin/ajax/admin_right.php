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

	$SECU='admin';
	include_once('../../../include/main.inc');
	include_once('../admin.class.inc');
	$admin_panel = unserialize($_SESSION['admin_panel_'.$_REQUEST['id_admin']]);
	$page = new Page();
				
	switch($_REQUEST['wope_action']){
		case 'display_right':
			$admin_panel->displayRightContent($_GET['right']);
			break;
		case 'display_update_right':
			$admin_panel->displayUpdateRight($_GET['right']);
			break;
		case 'display_add_right':
			$admin_panel->displayAddRight();
			break;
		case 'delete_right':
			$admin_panel->deleteRight($_GET['right']);
			break;
		case 'display_rights_tree':
			$admin_panel->displayRightsTree();
			break;
		case 'display_right_desc';
			$admin_panel->displayRightDescription($_GET['right']);
			break;
		case 'save_right';
			$admin_panel->saveRight($_POST);
			break;
		case 'update_father_right';
			$admin_panel->updateFatherRight($_GET['right'],$_GET['father']);
			break;
		case 'display_father_right':
			$admin_panel->displayFatherRight($_GET['right']);
			break;
		case 'add_right_to_user';
			$admin_panel->addRightToUser($_GET['right'],$_GET['user']);
			break;
		case 'add_admin_to_right';
			$admin_panel->addAdminToRight($_GET['right'],$_GET['user']);
			break;
		case 'remove_right_from_user';
			$admin_panel->removeRightFromUser($_GET['right'],$_GET['user']);
			break;
		case 'remove_admin_from_right';
			$admin_panel->removeAdminFromRight($_GET['right'],$_GET['user']);
			break;
		case 'display_right_added':
			$page->msgInfo('Droit '.$_GET['right'].' ajouté avec succès');
			$admin_panel->adminRightMenuDefault();
			break;
		case 'display_right_deleted':
			$page->msgInfo('Droit supprimé avec succès');
			$admin_panel->adminRightMenuDefault();
			break;
		case 'display_right_default':
			$admin_panel->adminRightMenuDefault();
			break;
		default:
			//void
			break;
	}
?>
