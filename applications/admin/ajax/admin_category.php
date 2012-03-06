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
	$relative_path = '../../..';
	include_once($relative_path.'/include/main.inc');
	$admin_panel = unserialize($_SESSION['admin_panel_'.$_REQUEST['id_admin']]);
				
	switch($_REQUEST['wope_action']){
		case 'addCategory':
			$admin_panel->adminCategoryAddCategory($name, $rightGet, $rightSet, $image, $defaultShow);
			break;
		case 'delCategory':
			$admin_panel->adminCategoryDelCategory($id);
			break;
		case 'updateCategory':
			$admin_panel->adminCategoryUpdateCategory($id, $name, $rightGet, $rightSet, $image, $defaultShow);
			break;
		case 'listCategory':
			$admin_panel->adminCategoryListCategory();
			break;
		default:
			//void
			break;
	}
?>
