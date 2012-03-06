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
include($relative_path.'/include/main.inc');
$core->loadClass('trombi', dirname(__FILE__).'/..');

$action = isset($_GET['wope_action']) ? $_GET['wope_action'] : ( isset($_POST['wope_action']) ? $_POST['wope_action'] : '' ) ;

$id_trombi = isset($_GET['id_trombi']) ? $_GET['id_trombi'] : ( isset($_POST['id_trombi']) ? $_POST['id_trombi'] : '' );

$trombi = unserialize($_SESSION['trombi_'.$id_trombi]);

switch($action){

	case 'save_status':
		$trombi->saveStatus($_POST['social_status']);
		break;
		
	default:
		break;
		
}

?>