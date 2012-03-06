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
  $SECU = 'log';       //Niveaux de sécurité (voir $session->authPage() )
  include ('../../include/main.inc');
	include ('config_module_agenda.inc');
 		
	$ARIANEWIRE= Array (
  	"Accueil" => $core->makeUrl('index.php'),
		"Agenda" => $core->makeUrl('modules/module_agenda')
	);

	$core->localLoadClass('module_agenda');
	$agenda = new Module_agenda();
	
  $page = new Page();
  $page->header(locale::display('event','kernel'),'agenda');

	$action = isset($_POST['ga']) ? $_POST['ga'] : ( isset($_GET['ga']) ? $_GET['ga'] : '');
	
	switch( $action ){
	
		case 's';
			$agenda->saveEvent($_POST, $_FILES);
			break;
			
		case 'a':
			$agenda->displayAppHeader();
			$agenda->displayAddForm();
			break;
			
		case 'd':
		default:
			$agenda->displayAppHeader();
			$agenda->displayTypes();
			
			$cur_page = isset($_GET['page']) ? $_GET['page'] : 1 ;
			//on affiche les evenements qui doivent l'etre
			if (isset($_GET['date_af'])) {
				$agenda->displayEventDate($_GET['date_af'], $cur_page);
			} elseif (isset($_GET['id_event'])) {
				$agenda->displayEventId($_GET['id_event'], $cur_page);
			} elseif (isset($_GET['id_type'])) {
				$agenda->displayEventsType($_GET['id_type'], $cur_page);
			} else {
				$agenda->displayFutureEvents($cur_page);
			}
			break;
	}

	$page->footer();
	
?>
