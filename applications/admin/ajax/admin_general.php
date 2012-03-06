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
				
	switch($_REQUEST['wope_action']){
		case 'update_skin';
			$admin_panel->updateSkin($_GET['new_skin']);
			break;
		case 'del_default_module':
			$admin_panel->removeModule('default',$_GET['module']);
			break;
		case 'add_default_module':
			$admin_panel->insertModule('default',$_GET['module']);
			break;
		case 'add_logout_module':
			$admin_panel->insertModule('',$_GET['module']);
			break;
		case 'del_logout_module':
			$admin_panel->removeModule('',$_GET['module']);
			break;
		case 'up_logout_module':
			$admin_panel->moveUpLogoutModule($_GET['module']);
			break;
		case 'down_logout_module':
			$admin_panel->moveDownLogoutModule($_GET['module']);
			break;
		case 'display_menu_list':
			$admin_panel->displayMenuList();
			break;
		case 'display_add_menu_entry':
			$admin_panel->displayAddMenuEntry();
			break;
		case 'display_update_menu_entry':
			$admin_panel->displayUpdateMenuEntry($_GET['id_entry']);
			break;
		case 'del_menu_entry':
			$admin_panel->deleteMenuEntry($_GET['id_entry']);
			break;
		case 'validate_menu_entry':
			$admin_panel->validateMenuEntry($_GET['id_entry'],$_GET['valid']);
			break;
		case 'save_menu_entry':
			$admin_panel->saveMenuEntry($_POST);
			break;
		case 'empty_menu_cache':
			$admin_panel->emptyMenuCache();
			break;
		default:
			//void
			break;
	}
?>
