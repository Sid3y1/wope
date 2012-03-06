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
		case 'display_result':
			$admin_panel->adminUserContent((isset($_GET['q'])?$_GET['q']:null),(isset($_GET['option'])?$_GET['option']:null),(isset($_GET['cur_page'])?$_GET['cur_page']:null),(isset($_GET['nb_per_page'])?$_GET['nb_per_page']:null));
			break;
		case 'display_menu':
			$admin_panel->adminUserMenu((isset($_GET['q'])?$_GET['q']:''),(isset($_GET['option'])?$_GET['option']:null));
			break;
		case 'display_menu_user':
			$admin_panel->adminUserMenuUser($_GET['user']);
			break;
		case 'display_user':
			$admin_panel->displayUser($_GET['user']);
			break;
		case 'display_update_trombi_card':
			$admin_panel->displayUpdateTrombiCard($_GET['user']);
			break;
		case 'display_trombi_card':
			$admin_panel->displayTrombiCard($_GET['user']);
			break;
		case 'display_user_prefs':
			$admin_panel->displayUserPrefs($_GET['user']);
			break;
		case 'active_user':
			$admin_panel->activeUser($_GET['user'],$_GET['active']);
			//this is quite ugly, I wasn't gifted --> see my trade application to do better
			$admin_panel->adminUserContent($_GET['q'],(isset($_GET['option'])?$_GET['option']:null),(isset($_GET['cur_page'])?$_GET['cur_page']:null),(isset($_GET['nb_per_page'])?$_GET['nb_per_page']:null));
			break;
		case 'delete_user':
			$admin_panel->deleteUser($_GET['user']);
			//this is quite ugly, I wasn't gifted --> see my trade application to do better
			$admin_panel->adminUserContent($_GET['q'],(isset($_GET['option'])?$_GET['option']:null),(isset($_GET['cur_page'])?$_GET['cur_page']:null),(isset($_GET['nb_per_page'])?$_GET['nb_per_page']:null));
			break;
		case 'display_add_user':
			$admin_panel->displayAddUser();
			break;
		case 'display_update_user':
			$admin_panel->displayUpdateUser($_GET['user']);
			break;
		case 'save_user':
			$admin_panel->saveUser($_POST);
			break;
		case 'change_password':
			$admin_panel->lostPassword($_GET['user']);
			break;
		default:
			//void
			break;
	}
?>
