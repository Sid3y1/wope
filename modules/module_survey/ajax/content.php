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


  $SECU='user';
  include_once('../../../include/main.inc');
  include_once('../module_survey.class.inc');
  $survey = unserialize($_SESSION['module_survey_'.$_REQUEST['id_survey']]);

  switch($_REQUEST['wope_action']) {
	
	  case 'submitVote':
			$survey->submitVote($_GET['target_survey'],$_GET['answer']);		
			echo $survey->displayTargetSurvey($_GET['target_survey'], 'module');
		break;

		case 'displaySurveys':
			$survey->displaySurveysByType($_GET['type'], $_GET['cur_page']);
		break;

		case 'saveSurvey':
			$survey->saveSurvey($_POST);
			echo '<div class="msg_info">Votre proposition a bien été enregistrée ! L\'administrateur l\'examinera tout prochainement. Merci de votre participation.</div>';
			$survey->displaySurveysByType(1);
		break;

		default:
			//void
		break;
																																																																															  }
																																																																															

// End of this file.

?>
