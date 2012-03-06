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


	$SECU = 'admin';
	include_once( dirname(__FILE__).'/../../../include/main.inc' );
	include_once( dirname(__FILE__).'/../admin.class.inc' );

	$action = isset($_POST['ga']) ? $_POST['ga'] : ( isset($_GET['ga']) ? $_GET['ga'] : '' );
	$idAdmin = isset($_POST['ida']) ? $_POST['ida'] : ( isset($_GET['ida']) ? $_GET['ida'] : '' );
	$adminPanel = unserialize( $_SESSION['admin_panel_'.$idAdmin] );
				
	switch( $action ){
		case 'dam':
			$adminPanel->displayEditModule('add');
			break;
		case 'dum':
			$adminPanel->displayEditModule('update', $_GET['m']);
			break;
		case 'dml':
			$adminPanel->displayModulesList();
			break;
		case 'dtl':
			//$adminPanel->displayTabsList();
			  debug("Not yet implemented");
			break;
		case 'sm':
			$adminPanel->saveModule($_POST);
			break;
		case 'dm':
			$adminPanel->deleteModule($_GET['m']);
			break;
		case 'st':
			$adminPanel->saveTab($_POST);
			break;
		case 'dt':
			$adminPanel->deleteTab($_GET['t']);
			break;
		case 'sc':
			$adminPanel->saveColumn($_POST);
			break;
		case 'dc':
			$adminPanel->deleteColumn($_GET['c']);
			break;
		default:
			//void
			break;
	}
?>
