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


  $SECU='log';
	include_once('../../../include/main.inc');
	include_once('../module_links.class.inc');
	$links = new Module_links();
	
	switch($_REQUEST['wope_action']) {
	
		case 'display_result':
			$links->displayLinksList((isset($_GET['cur_page'])?$_GET['cur_page']:null),(isset($_GET['nb_per_page'])?$_GET['nb_per_page']:null));
			break;

		default:
			//void
			break;
	}
	
?>
