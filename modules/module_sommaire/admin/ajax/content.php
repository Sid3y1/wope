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
	include_once('../module_sommaire_admin.class.inc');
	$admin_sommaire = unserialize($_SESSION['admin_sommaire_'.$_GET['id_admin']]);
	
	switch($_GET['content']){
		case 'add_title':
			$admin_sommaire->displayForm('create');
			break;
		case 'update':
			$admin_sommaire->displayForm('update',$_GET['title']);
			break;
		case 'category':
			$admin_sommaire->fatherSelect($_GET['cat'],((isset($_GET['id']))?$_GET['id']:''),'');
			break;
		case 'save':
			$admin_sommaire->saveForm($_GET);
			$admin_sommaire->displayTitles();
			break;
		case 'display_titles':
			$admin_sommaire->displayTitles();
			break;
		case 'hide_title':
			$admin_sommaire->hideTitle($_GET['title'],$_GET['hidden']);
			$admin_sommaire->displayTitles();
			break;
		default:
			//void
			break;
	}
?>
