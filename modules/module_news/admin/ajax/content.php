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
	include_once('../module_news_admin.class.inc');
	$admin_news = unserialize($_SESSION['admin_news_'.$_REQUEST['id_admin']]);
	
	switch($_REQUEST['content']){
		case 'display_news':
			$admin_news->displayNews(isset($_GET['page']) ? $_GET['page'] : 1);
			break;
		case 'display_types':
			$admin_news->displayTypes();
			break;
		case 'add_news':
			$admin_news->displayAddNews();
			break;
		case 'add_type':
			$admin_news->displayAddType();
			break;
		case 'update_news':
			$admin_news->displayUpdateNews($_GET['id_news']);
			break;
		case 'update_type':
			$admin_news->displayUpdateType($_GET['id_type']);
			break;
		case 'del_news':
			$admin_news->deleteNews($_GET['id_news']);
			$admin_news->displayNews();
			break;
		case 'del_type':
			$admin_news->deleteType($_GET['id_type']);
			$admin_news->displayTypes();
			break;
		case 'validate_news':
			$admin_news->validateNews($_GET['id_news'],'Y');
			$admin_news->displayNews();
			break;
		case 'cancel_news':
			$admin_news->validateNews($_GET['id_news'],'N');
			$admin_news->displayNews();
			break;
		case 'save_news':
			$admin_news->saveNews($_POST);
			break;
		case 'save_type':
			$admin_news->saveType($_REQUEST, (isset($_FILES['news_type_image']))?$_FILES['news_type_image']:'');
			break;
		default:
			//void
			break;
	}
?>
