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


//Obligatoire
$relativePath = '../../..';       //Chemin relatif mpour accéder à la racine de l'arborescence
$SECU = 'log';
include_once ($relativePath . '/include/main.inc');

$core->loadClass('module_todolist',dirname('__FILE__').'/..');


$id_todolist = ( ( isset($_GET['id_todo']) ) ? $_GET['id_todo'] : ( ( isset($_POST['id_todo']) ) ? $_POST['id_todo'] : '') );

$todolist = unserialize($_SESSION['module_todolist_'.$id_todolist]);

$action = isset($_GET['wope_action']) ? $_GET['wope_action'] : ( isset($_POST['wope_action']) ? $_POST['wope_action'] :'');

switch ($action) {

	case 'update_title':
		$todolist->updateTitle($_GET['id_task'], $_POST['update_title']);
		break;
		
	case 'update_date':
		$todolist->updateDate($_REQUEST['id_task'], $_REQUEST['update_date']);
		break;
		
	case 'valid_task':
		$todolist->validTask($_GET['id_task'],$_GET['done']);
		break;

	case 'add_task':
		$todolist->addTask($_GET['task_title'],$_GET['done'],$_GET['date']);
		break;

	case 'del_task':
		$todolist->removeTask($_GET['id_task']);
		break;
	
	default:
		//empty
		break;
	
}


?>
