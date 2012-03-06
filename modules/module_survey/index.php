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


// DO NOT DELETE : SECURITY LEVELS
$SECU = 'user';

$ARIANEWIRE = Array (
	"Accueil" => "../../index.php",
	"Sondages" => "index.php"
);
		
include ('../../include/main.inc');

$core->loadClass('module_survey');
$survey = new Module_Survey();

$page = new Page();
$page->header('Sondages','surveys');

echo '<div class="menu_block left">';
$page->ModuleHeader('Menu','');
	$survey->displayTypes();
	echo '<div class="clear sep"></div>'
			.'<a class="link_button" href="'.$core->makeUrl("/modules/module_survey/index.php").'?display=proposeSurvey&module_title=Proposition de sondage" >Proposer un sondage</a>'
			.'<div class="clear sep"></div>';
	$page->adminLink('module','module_survey','admin');
$page->ModuleFooter();
echo '</div>';

echo '<div class="list_block left">';

$module_title = 'Liste des sondages';
if (isset($_GET['module_title'])) {
	$module_title = $_GET['module_title'];
}

$page->ModuleHeader($module_title,'');
	echo '<div id="SurveysList">';

	switch ($_GET['display']) {

	case 'all' :
	$survey->displaySurveysByType();
	break;
	
	case 'proposeSurvey' :
	$survey->proposeSurvey();
	break;

	default :
	$survey->displaySurveysByType((int)$_GET['display']);
	break;
	
	}
	echo '</div>';
	
$page->ModuleFooter();
echo '</div>';

echo '<div class="clear"></div>';

$page->footer();
//End of this file.

?>
