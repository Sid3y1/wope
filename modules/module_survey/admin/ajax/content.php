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
	include_once('../../../../include/main.inc');
	include_once('../module_survey_admin.class.inc');
	$admin_survey = unserialize($_SESSION['admin_survey_'.$_REQUEST['id_admin']]);


	switch($_REQUEST['content']) {

		case 'displayList' :
			if (!isset($_REQUEST['cur_page'])) {
				$cur_page = 1;
			} else {
					$cur_page = $_REQUEST['cur_page'];
				}
			$admin_survey->displaySurveysList($cur_page);
		break;
		
		case 'addSurvey' :
			$admin_survey->displayAddSurvey();
		break;

		case 'updateSurvey' :
			$admin_survey->displayUpdateSurvey($_REQUEST['id_survey'], $_REQUEST['cur_page']);
		break;
		
		case 'saveSurvey' :
			$admin_survey->saveSurvey($_REQUEST);
			$admin_survey->displaySurveysList($_REQUEST['cur_page']);
		break;

		case 'closeSurvey' :
			$admin_survey->closeSurvey($_REQUEST['id_survey'], $_REQUEST['ended_status'], $_REQUEST['active_status']);
			$admin_survey->displaySurveysList($_REQUEST['cur_page']);
		break;
		
		case 'activateSurvey' :
			$admin_survey->activateSurvey($_REQUEST['id_survey'], $_REQUEST['ended_status'], $_REQUEST['active_status']);
			$admin_survey->displaySurveysList($_REQUEST['cur_page']);
		break;

		case 'archiveSurvey' :
			$admin_survey->archiveSurvey($_REQUEST['id_survey']);
			$admin_survey->displaySurveysList($_REQUEST['cur_page']);
		break;

		case 'deleteSurvey' :
			$admin_survey->deleteSurvey($_REQUEST['id_survey']);
			$admin_survey->displaySurveysList($_REQUEST['cur_page']);
		break;

		case 'displayLegend':
			$admin_survey->displayLegend();
		break;

		default :
			//void.
		break;

	}
					


// End of this file.

?>
