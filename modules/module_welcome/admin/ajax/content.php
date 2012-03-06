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
	include_once('../../../../include/main.inc');
	include_once('../module_welcome_admin.class.inc');
	$admin_welcome = unserialize($_SESSION['admin_welcome_'.$_REQUEST['id_admin']]);
	
	switch($_REQUEST['content']){
		case 'display_messages':
			$admin_welcome->displayMessages();
		break;
		
		case 'add_message':
			$admin_welcome->displayAddMessage();
		break;
		
		case 'update_message':
			$admin_welcome->displayUpdateMessage($_GET['id_message']);
		break;
		
		case 'del_message':
			$admin_welcome->deleteMessage($_GET['id_message']);
			$admin_welcome->displayMessages();
		break;
		
		case 'save_message':
			$admin_welcome->saveMessage($_REQUEST, (isset($_FILES['message_image']))?$_FILES['message_image']:'');
		break;
		
		case 'validate_message':
			$admin_welcome->validateMessage($_GET['id_message']);
		break;
		
		default:
			//void
		break;
	}
?>
