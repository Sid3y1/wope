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
	include_once('../module_agenda_admin.class.inc');
	$admin_agenda = unserialize($_SESSION['admin_agenda_'.$_REQUEST['id_admin']]);
	
	switch($_REQUEST['content']){
		case 'display_events':
			$admin_agenda->displayEvents(isset($_GET['page']) ? $_GET['page'] : 1);
			break;
		case 'display_types':
			$admin_agenda->displayTypes();
			break;
		case 'add_event':
			$admin_agenda->displayAddEvent();
			break;
		case 'add_type':
			$admin_agenda->displayAddType();
			break;
		case 'update_event':
			$admin_agenda->displayUpdateEvent($_GET['id_event']);
			break;
		case 'update_type':
			$admin_agenda->displayUpdateType($_GET['id_type']);
			break;
		case 'del_event':
			$admin_agenda->deleteEvent($_GET['id_event']);
			$admin_agenda->displayEvents();
			break;
		case 'del_type':
			$admin_agenda->deleteType($_GET['id_type']);
			$admin_agenda->displayTypes();
			break;
		case 'validate_event':
			$admin_agenda->validateEvent($_GET['id_event'],'Y');
			$admin_agenda->displayEvents();
			break;
		case 'cancel_event':
			$admin_agenda->validateEvent($_GET['id_event'],'N');
			$admin_agenda->displayEvents();
			break;
		case 'save_type':
			$admin_agenda->saveType($_REQUEST, (isset($_FILES['event_type_image']))?$_FILES['event_type_image']:'');
			break;
		case 'save_event':
			$admin_agenda->saveEvent($_REQUEST, (isset($_FILES['event_image']))?$_FILES['event_image']:'');
			$admin_agenda->displayEvents();
			break;
		default:
			//void
			break;
	}
?>
