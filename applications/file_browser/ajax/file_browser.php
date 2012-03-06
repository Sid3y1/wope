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


	$SECU = 'log';
	$relative_path = '../../..';
	include_once($relative_path.'/include/main.inc');
	$core->loadClass('filemanager');
	$core->loadClass('file_browser', dirname(__FILE__).'/..');

	$fb = unserialize($_SESSION['file_browser_'.$_REQUEST['id_fb']]);

	switch($_REQUEST['wope_action']){
		case 'remove_f':
			$fb->removeFile($_GET['f']);
			$fm = new FileManager();
			$fm->remove($_GET['f']);
			break;
		case 'display_ft':
			$fb->displayFilesTable();
			break;
		case 'display_quotas':
			$fm = new FileManager();
			echo $fm->displayQuotas();
			break;
		default:
			//empty
			break;
	}
	
?>
