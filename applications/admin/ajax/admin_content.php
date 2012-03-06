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
	$core->loadClass('admin',dirname(__FILE__).'/..');
	$admin = new Admin();
	
	switch($_GET['part']){
		case 'module':
			$admin->adminModuleContent($_GET['content']);
			break;
		case 'app':
			$admin->adminAppContent($_GET['content']);
			break;
		default:
			//void
			break;
	}
?>
