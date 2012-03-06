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


$SECU = 'webmaster admin';
$relative_path = '../../../..';
include($relative_path.'/include/main.inc');

$core->loadClass('bug_report_admin',dirname(__FILE__).'/..');
$admin_bug_report = unserialize($_SESSION['bug_report_admin_'.$_REQUEST['id_admin']]);

switch($_REQUEST['wope_action']){

	case 'display_severity_list':
		$admin_bug_report->displaySeverityLevels();
		break;
	case 'add_severity_level':
		$admin_bug_report->displayAddSeverityLevel();
		break;
	case 'delete_severity_level':
		$admin_bug_report->deleteSeverityLevel($_GET['severity_name']);
		$admin_bug_report->displaySeverityLevels();
		break;
	case 'update_severity_level':
		$admin_bug_report->displayUpdateSeverityLevel($_GET['severity_name']);
		break;


	case 'display_bug_types_list':
		$admin_bug_report->displayBugTypes();
		break;
	case 'add_bug_type':
		$admin_bug_report->displayAddBugType();
		break;
	case 'delete_bug_type':
		$admin_bug_report->deleteBugType($_GET['type_name']);
		$admin_bug_report->displayBugTypes();
		break;
	case 'update_bug_type':
		$admin_bug_report->displayUpdateBugType($_GET['type_name']);
		break;


	case 'display_message':
		echo 'Veuillez modifier la variable <strong>$config["mailBug"]</strong> dans le fichier <strong>include/config_secure.inc</strong>';
		break;

	default:
		//void
		break;
}
?>
