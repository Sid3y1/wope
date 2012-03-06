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
	include_once('../module_links_admin.class.inc');
	$admin_links = unserialize($_SESSION['admin_links_'.$_REQUEST['id_admin']]);
	
	switch($_REQUEST['content']){
		case 'display_links':
			$admin_links->displayLinks();
			break;
		case 'add_link':
			$admin_links->displayAddLink();
			break;
		case 'update_link':
			$admin_links->displayUpdateLink($_GET['id_link']);
			break;
		case 'del_link':
			$admin_links->deleteLink($_GET['id_link']);
			$admin_links->displayLinks();
			break;
		case 'save_link':
			$admin_links->saveLink($_REQUEST);
			break;
		case 'validate_link';
			$admin_links->validateLink($_GET['id_link'],'Y');
			$admin_links->displayLinks();
			break;
		case 'cancel_link';
			$admin_links->validateLink($_GET['id_link'],'N');
			$admin_links->displayLinks();
			break;

		default:
			//void
			break;
	}
?>
