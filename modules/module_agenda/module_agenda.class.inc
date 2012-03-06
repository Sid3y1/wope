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


/******************* agenda.class ***************************************
		Définition pour le agenda

	**********************************************************************/
class Module_agenda extends Module {
	
	private $login;

	private $config;

	public final function __construct($informations = Array()){
		global $core;

		parent::__construct($informations);
		
		include('config_module_agenda.inc');
		$this->login = (int)$core->getUserId();
		$this->config = $config_module_agenda;
	}

	public function display() {
		global $db, $core, $page;

		if (($core->session->getAuthOk() == false) || ($core->user->getPref($core->getUserId(),'event_type') == '') )
		{
			$type = 'week';
		}
		else
		{
			$preference = $core->user->getPref($core->getUserId(), 'event_type');
			if ($preference == 'week')
			{
				$type = 'week';
			}
			else
			{
				$type = 'month';
			}
		}
		echo'			<div class="centrage">
								<span class="panel_h">
									<a href="'.$core->makeUrl('modules/module_agenda/index.php?ga=a').'">Ajouter un événement</a> | 
									<a href="'.$core->makeUrl('modules/module_agenda/index.php?ga=d').'">Afficher tous les événements</a>';
		echo '			</span>
							</div>';

		echo '<div id="agenda_content">';
	
		switch ($type) {
			case 'week':
				$this->displayWeek();
				break;
			case 'month':
				$month = date("m");
				$year = date("Y");
				$this->displayMonth($month, $year);
				break;
		}

		echo '</div>';
		
		echo '<div class="clear"></div>';
		echo $page->adminLink('module','module_agenda',$this->config['admin_rights']);
		
	}

	public function displayMonth($mois, $annee) {
		global $db, $core, $page;
		if(!isset($page))$page = new Page();
		$core->loadClass("date");

		$mydate = new Date(); 
		//$aryDateMaintenant = explode("-",date("d-m-Y"));
		$today_stmp = mktime(0,0,0,date("m"),date("d"),date("Y"));
		$today = date("Y-m-d");
		//$str = '';

		if(empty($annee)) $annee = date("Y");
		if(empty($mois)) $mois = date("m");
		if(empty($jour)) $jour = date("d");

		// INITIALISATIONS
		// premier jour du mois (lundi, mardi, mercredi etc..)
		$intPremierJour = date("w",mktime(0,0,0,$mois,1,$annee));
		if ($intPremierJour == 0) $intPremierJour = 7; // si c'est un dimanche

		//Pour changer de mois
		$monthPrevious = mktime(0,0,0,$mois - 1,1,$annee);
		$monthNext = mktime(0,0,0,$mois + 1,1,$annee);

		// nombre de jours dans le mois (28,...,31)
		$intNbJoursDansMois = date ("t", mktime(0, 0, 0, $mois, 1, $annee));

		echo '<div class="col_left">';
		
		// AFFICHAGE DE LA TABLE
		// affichage de la barre de navigation des mois (ex: "< septembre >")
		//voir avec Sid pour se débarrasser du http://blabla
		echo '<div class="nav"><a  onclick="AJAX.getAndUpdate(\'modules/module_agenda/ajax/agenda_display.php?display=month&month='.date('m', $monthPrevious).'&year='.date('Y',$monthPrevious).'\',\'agenda_content\')" class="previous">&laquo;</a>
			<h3>'.$mydate->arrayMonth[intval($mois)] . ' ' . $annee .'</h3>
			<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('./modules/module_agenda/ajax/agenda_display.php').'?display=month&month='.date('m', $monthNext).'&year='.date('Y',$monthNext).'\',\'agenda_content\')" class="next">&raquo;</a></div>';
		echo '<table class="month">';
		// affichage de la ligne des jours
		echo '<thead>
			<tr>
			<th>L</th>
			<th>M</th>
			<th>M</th>
			<th>J</th>
			<th>V</th>
			<th>S</th>
			<th>D</th>
			</tr>
			</thead>';
		// affichage du reste du calendrier
		echo '<tbody>
			<tr>';
		// on affiche les premiers jours vides ...
		for($i = 1; $i < $intPremierJour ; $i ++)
			echo '<td class="noday">&nbsp;</td>';
		// ... pour commencer a partir de notre premier jour calcule plus haut
		$j = $intPremierJour;
		for( $i=1 ; $i <= $intNbJoursDansMois ; $i ++ )
		{
			$day_stmp=mktime(0,0,0,$mois,$i,$annee);
			if ($day_stmp == $today_stmp) {
				$style = ' class="today"';
			} else {
				$style = '';
			}

			$day = date("Y-m-d", $day_stmp);
			
			$result = $db->query(" 
			
			SELECT eventsListe.id AS id,
						 eventsListe.titre AS event_title,
						 eventsListe.description AS event_description,
						 eventsListe.lieu AS event_place,
						 eventsListe.heure AS event_hour,
						 categ.image AS image,
						 categ.name AS event_type
			
			FROM   eventsListe, kernel_tag_information info
		  LEFT JOIN ( kernel_tag_information_category info_categ
								  JOIN
									kernel_tag_category_user categ)

			ON (info.id = info_categ.idInformation
			    AND
				  categ.idCategory = info_categ.idCategory)
									
			WHERE    date = '$day' 
			AND      actif='Y'
			AND			 info.idOrig = eventsListe.id
			AND			 info.tableOrig = 'eventsListe'
			AND			 categ.isShow = 'true'
			AND			 categ.login = '".$this->login."'

			GROUP BY eventsListe.id");

			if ($db->numRows($result) > 0)
			{
				if ($day_stmp == $today_stmp)
				{
					$style = ' class="event today"';
				}
				else
				{
					$style = ' class="event"';
				}
				
				echo '<td'.$style.' onmouseover="activateToolTips(this,\'';
				while ($row=$db->fetchArray($result))
				{
					$event_hour_h   = substr($row['event_hour'],0,2);
					$event_hour_min = substr($row['event_hour'],3,2);
					$first_letter_title = substr($row['event_title'],0,1);
					$rest_title = substr($row['event_title'],1,200);
					echo $page->htmlBlock->escapeTipContent('<p class=tt_infos><span class=tt_title><span>'.stripslashes($first_letter_title).'</span>'.stripslashes($rest_title).'</span> à <span class=tt_hour>'.$event_hour_h.'h'.$event_hour_min.'</span></p><p class=tt_place>Lieu : <span>' . stripslashes($row['event_place']) . '</span></p>');
				}
				echo '\');"><a href="modules/module_agenda/index.php?ga=d&date_af='.$day.'">'.$i.'</a></td>';
			}
			else
			{
				echo '<td'.$style.'>'.$i.'</td>';
			}
			// dernier jour de la semaine
			if ($j == 7)
			{
				// on change de ligne
				if ($i < $intNbJoursDansMois)
				{
					echo "</tr>";
					echo "<tr>";
					$j = 1;
				}
				// sinon on avance d'un jour dans la semaine...
			}
			else
			{
				$j ++;
			}
		}
		// il reste a affiche les jours vides restant
		for ( $i = $j ; $i <= 7 ; $i ++)
			echo '<td class="noday">&nbsp;</td>';
		// fin de la table
		echo '</tr>';
		echo '</table>';
		echo '</div>';
		echo '<p><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('./modules/module_agenda/ajax/agenda_display.php').'?display=week\',\'agenda_content\')" class="link_button">Mode hebdomadaire</a></p>';

		echo '<div class="col_right">';
		echo '<dl class="nextEvents"><dt id="nextitre"><span>L</span>iste des prochains événements :</dt>';
		$today = date("Y-m-d");
		
		$result = $db->query("
		
		SELECT eventsListe.id,
					 eventsListe.date AS event_date,
					 eventsListe.heure AS event_hour,
					 eventsListe.titre AS event_title,
					 eventsListe.description AS event_description,
					 categ.name AS event_type,
					 categ.image AS type_image
		
		FROM   eventsListe, kernel_tag_information info
		LEFT JOIN ( kernel_tag_information_category info_categ
		            JOIN
								kernel_tag_category_user categ)
		ON (info.id = info_categ.idInformation
			  AND
			  categ.idCategory = info_categ.idCategory)
		
		WHERE 	date >= CURDATE() 
		AND 		actif='Y' 
		AND			info.idOrig = eventsListe.id
		AND			info.tableOrig = 'eventsListe'
		AND			categ.isShow = 'true'
		AND			categ.login = '".$this->login."'
		
		GROUP BY eventsListe.id
		ORDER BY date ASC 
		
		LIMIT 6
												");
		while ($row = $db->fetchArray($result))
		{
			$style = ($row['event_date'] == $today) ? ' class="today" ' : '' ;
			
			$f_letter_type = substr($row['event_type'],0,1);
			$rest_type = substr($row['event_type'],1,200);
			$next_events_tooltip = $page->htmlBlock->escapeTipContent(
				'<div class="tooltip">
					<div class="title">
						<div class="img"><img src="'.$core->makeUrl('data/files/categ_img/'.$row['type_image']).'" alt="'.str_replace(Array(' ', "'"), Array('_', ''), strtolower(stripslashes($row['event_type']))).'_icon" /></div>'.
						'<div class="txt"><span><span>'.stripslashes($f_letter_type).'</span>'.stripslashes($rest_type).' : '.stripslashes($row['event_title']).'</span></div>
						<div class="date">le '.$mydate->reverseDate($row['event_date']). ( ($row['event_hour'] != '00:00:00') ? ' à '.substr(stripslashes($row['event_hour']), 0, 5) : '' ) .'</div>
					</div>
					<div class="content">'.
						stripslashes($row['event_description']).
					'</div>
				</div>'
			);
			
			echo '<dd'.$style.' onmouseover="activateToolTips(this, \''.$next_events_tooltip.'\');" >' . $mydate->formatDate($row['event_date'],"d/m") . ' : '.
				'<a href="'.$core->makeUrl('modules/module_agenda/index.php?ga=d&id_event='.$row['id']).'" >'.
					stripslashes($row['event_title']).
				'</a>
			</dd>';
		}
		echo '</dl>';
		echo '</div>';
		if ($core->session->getAuthOk())
		{
			echo '<div class="clear"></div>';
			}
	}
	
	public function displayWeek() {
		global $db, $core, $page;
		if(!isset($page)) $page = new Page();
											
    echo '<div><h3 class="semainier_bigtitredelamort"><span>E</span>vénements de la semaine : </h3></div>';
		$core->loadClass("date");
		$mydate = new Date();
		echo '<table align="center" width="480" id="calendrier_sem">';
		echo '<tr>';
		for ($i = 0; $i < 7 ; $i++)
		{
			$date = mktime(0,0,0,date("m"),date("d") + $i, date("Y"));

			if (date("w", $date) == 0 OR date("w", $date) == 6)
			{
				$style = ' class="weekend"';
			}
			else
			{
				$style = '';
			}
			/* LIGNE DES JOURS */
			echo '<th align="center" height="45" nowrap="nowrap" valign="top" class="colonne_cal">';
			
			$result = $db->query(" 
			
			SELECT			 categ.image AS image,
									 categ.name AS event_type,
									 eventsListe.id AS id,
									 eventsListe.titre AS event_title,
									 eventsListe.heure AS hour, 
									 eventsListe.description AS description 
							 
			FROM				 kernel_tag_category_user categ,
									 kernel_tag_information info,
									 kernel_tag_information_category info_categ,
									 eventsListe
			
			WHERE				 info.idOrig = eventsListe.id
			AND					 info.tableOrig = 'eventsListe'
			AND					 info_categ.idInformation = info.id
			AND					 info_categ.idCategory = categ.idCategory
			AND					 categ.isShow = 'true'
			AND					 categ.login = '".$this->login."'
			AND					 eventsListe.date = '".date("Y-m-d",$date)."' 
			AND					 eventsListe.actif = 'Y'");
			
			echo '<a href="'.$core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;date_af='.date("Y-m-d",$date).'" class="semainier_jour"'.$style.'>' .substr($mydate->arrayDow[date("w",$date)],0,3) . ' ' . date("d",$date) . '</a>';
			if ($db->numRows($result) > 0)
			{
				$compteur_e=0;
				while ($row = $db->fetchArray($result))
				{
					$compteur_e++;
					if($compteur_e < 3)
					{
						$tooltip = $page->htmlBlock->escapeTipContent(
				'<div class="tooltip">
					<div class="title">
						<div class="img"><img src="'.$core->makeUrl('data/files/categ_img/'.$row['image']).'" alt="'.str_replace(Array(' ', "'"), Array('_', ''), strtolower(stripslashes($row['event_type']))).'_icon" /></div>'.
						'<div class="txt">'.stripslashes($row['event_type']).' : '.stripslashes($row['event_title']).'</div>
						<div class="date">'. ( ($row['hour'] != '00:00:00') ? ' à '.substr(stripslashes($row['hour']), 0, 5) : '' ) .'</div>
					</div>
					<div class="content">'.
						stripslashes($row['description']).
					'</div>
				</div>'

//							stripslashes($row['event_type']).' : ' .stripslashes($row['titreEvent'])
						);
						
						echo '<div class="event_sem"></br>
							<a href="'.$core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;id_event='.$row['id'].'" onmouseover="activateToolTips(this, \''.$tooltip.'\')">
								<img src="'.$core->makeUrl('data/files/categ_img/'.$row['image']).'" alt="'.stripslashes($row['event_type']).'" height="22px" width="22px"></img>
							</a></div>';
					}
					elseif($compteur_e==3)
					{
						echo '..';
					}
				}
			}
			echo '</th>';

			if ($i !=6 )
			{
				echo '<td class="separation"></td>';
			}

		}
	  	echo '</tr>';
		echo '</table>';

		if ($core->session->getAuthOk())
		{
		echo '<p><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('./modules/module_agenda/ajax/agenda_display.php').'?display=month&month='.date('m').'&year='.date('Y').'\',\'agenda_content\')" class="link_button mensuel">Mode mensuel</a></p>';
		}
	}



/*****      FUNCTIONS USED IN THE AGENDA PART     *****/

	public function displayAppHeader(){
	 	global $core,$page;
		if(!isset($page))$page = new Page();

		echo '<div class="col0 left">';
		$page->moduleHeader(locale::display('header_title','events'),'');
  	echo '<ul class="panel left">';
		echo '<li class="link_menu">
						<a class="next_events" href="index.php?ga=d" title="'.locale::display('next_events','events').'">'.locale::display('next_events','events').'</a>
				  </li>';
				 
  	echo '<li class="link_menu">
						<a class="add_event" href="index.php?ga=a" title="'.locale::display('add_event','events').'">Ajouter un événement</a>
					</li>';
					
  	echo '</ul>
		
					<div class="right">';
    $page->adminLink('module','module_agenda',$this->config['admin_rights']);
		echo '</div>';
					
		$page->moduleFooter();
		echo '</div>';
		
	}


	public function displayTypes() {
		global $db, $page, $core;
		if (!isset ($page))
			$page = new Page();
		echo '<div class="col1 right">';
		$page->moduleHeader('Types d\'événements', '');
		$result = $db->query("SELECT idCategory as id,name,image 
							FROM kernel_tag_category_user categ 
							WHERE categ.login='".$this->login."'
								AND categ.isShow='true'");
		echo '<ul class="panel">
						<li class="link_menu"><a href="'.$core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;id_type=all" >Tous types</a></li>';
		while ($row = $db->fetchArray($result)) {
			echo '<li class="link_menu"><a href="'.$core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;id_type='.$row['id'].'">'; 
			echo ' ' . stripslashes($row['name']) . '</a></li>';
		}
		echo '</ul>';
		$page->moduleFooter();
		echo '</div>';
	}


	public function displayEventsType($id_type = 'all', $cur_page = 1) {
		global $core, $db, $page;
		if (!isset ($page))
			$page = new Page();

		if (!empty ($id_type) && $id_type != 'all') {
			$id_type=(int)$id_type;
			$sqlType = " AND categ.idCategory = $id_type ";
		} else {
			$sqlType = '';
		}
		
		//pages management
		$start_row = ($cur_page - 1) * ($this->config['items_per_page']);
		
		echo '<div class="clear"></div>';

		$request = "SELECT 
								categ.image AS image,
								categ.name AS event_type,
								eventsListe.lieu AS event_place,
							  eventsListe.actif AS event_active,
							  eventsListe.date AS event_date,
							  eventsListe.auteur AS event_author,
							  eventsListe.description AS event_description,
							  eventsListe.titre AS event_title,
							  eventsListe.id AS event_id,
							  eventsListe.heure AS event_hour,
							  eventsListe.image AS event_image

							FROM 	kernel_tag_category_user categ, 
										kernel_tag_information info,
										kernel_tag_information_category info_categ,
										eventsListe

							WHERE eventsListe.actif = 'Y'
								AND info.idOrig=eventsListe.id
								AND info.tableOrig='eventsListe'
								AND info_categ.idInformation=info.id
								AND info_categ.idCategory=categ.idCategory
								AND categ.isShow='true'
								AND categ.login='".$this->login."'
								".$sqlType."
							GROUP BY eventsListe.id
							ORDER BY date ASC ";
							
		$result = $db->query($request." LIMIT ".$start_row.", ".$this->config['items_per_page']." ");
	
		if ($db->numRows($result) > 0) {
			echo '<div class="clear"></div>';
			$page->moduleHeader('Evénements à venir', '');

			$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'href', $core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;id_type='.$id_type.'&amp;page=[#]', 'Aller à la page : ');
			
			$style = true;
			while ($row = $db->fetchArray($result)) {
				$this->displayEvent($row, $style);
				$style = !$style;
			}
			
			$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'href', $core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;id_type='.$id_type.'&amp;page=[#]', 'Aller à la page : ');
			
			$page->moduleFooter();
		} else {
?> <div class="clear"></div> <?php

			$page->msgInfo('Aucun événement');
		}
	}


	/**
	 * displayEvent display an event
	 *
	 * @param   $info   Array :    Informations about the event
	 * @param   $style  boolean :  Style of the event
	 * @return  void
	*/
	public function displayEvent($info,$style = true){
	 	global $core,$db,$page;
  	$core->loadClass('date');
  	$date = new Date();	
		if(!isset($page))$page = new Page();

		$style = ($style) ? 1 : 2;
		$first_letter = substr(stripslashes($info['event_type']),0,1);
		$other_letters = substr(stripslashes($info['event_type']),1,300);
		
   	echo '
			<div class="row row'.$style.'">
			
        <div class="titre_news">
					<span class="titre_menu titre'.$style.'"><span>'.$first_letter.'</span>'.$other_letters.' : '.stripslashes($info['event_title']).'</span>
				</div>
				
				<div class="infos_creation_news">
					le '.$date->formatDate($info['event_date']);
					if(!empty($info['event_place']) && !empty($info['event_hour']) )' à ';
   				if (!empty($info['event_place'])) echo '<span class="event_info_title"> '.stripslashes($info['event_place']).',';
					if (!empty($info['event_hour']) && $info['event_hour'] != '00:00:00') echo '<span class="event_info_title"> '.substr($info['event_hour'], 0, 2).'h'.substr($info['event_hour'], 3, 2);
    			echo '<span class="infos_complementaires">
									(par : ' . stripslashes($info['event_author']).')
								</span>
				</div>
				
				<div class="right">';    
					$image = 'affiches/'.$info['event_image'];
    			if (is_file($image)) {
    				echo '<img src="'.$image.'" alt="" hspace="10" vspace="5" />';
    			}
					echo'
				</div>';
				
				
        echo'<div class="text_block">
        			 <div class="item_picture">
							 		<img src="'.$core->makeUrl('data/files/categ_img/'.$info['image']).'"'.
											'alt="'.stripslashes($info['event_type']).'"'. 
											'hspace="0" vspace="0" width="50" height="50" />'.
							'</div>
							
							 <span class="text-indent"> &nbsp; &nbsp; </span>
							 <span class="text">';
							 		echo nl2br(stripslashes($info['event_description']));
		 echo     '</span>'.
		 				'</div>';
						
     echo '<div class="clear"></div></div>';
	}


	public function displayFutureEvents($cur_page = 1){
  	global $core,$db,$page;
		if(!isset($page))$page = new Page();
		
		//pages management
		$start_row = ($cur_page - 1) * ($this->config['items_per_page']);
		
		echo '<div class="clear"></div>';
	
 		$request = "SELECT 				categ.image AS image,
																				categ.name AS event_type,
																				eventsListe.lieu AS event_place,
							    										  eventsListe.actif AS event_active,
											  						    eventsListe.date AS event_date,
																		    eventsListe.auteur AS event_author,
																		    eventsListe.description AS event_description,
																			  eventsListe.titre AS event_title,
																			  eventsListe.id AS event_id,
																			  eventsListe.heure AS event_hour,
																 				eventsListe.image AS event_image
																 
				  			          FROM          kernel_tag_category_user categ,
																				kernel_tag_information info,
																				kernel_tag_information_category info_categ,
																				eventsListe
																				
								 				  WHERE         eventsListe.actif = 'Y'
													AND						info.idOrig = eventsListe.id
													AND						info.tableOrig = 'eventsListe'
													AND						info_categ.idInformation = info.id
													AND						info_categ.idCategory = categ.idCategory
													AND						categ.isShow = 'true'
													AND						categ.login = '".$this->login."'
        					        AND           date >= CURDATE()
													GROUP BY			eventsListe.id
				                  ORDER BY      date ASC";

		$result = $db->query($request." LIMIT ".$start_row.", ".$this->config['items_per_page']." ");

		if ($db->numRows($result) > 0) {
			$page->moduleHeader(locale::display('next_events','events'),'');
			$style = true;

			$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'href', $core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;page=[#]', 'Aller à la page : ');
			
   		while ($row = $db->fetchArray($result)){
				$this->displayEvent($row,$style);
				$style = !$style;
   		}
			
			$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'href', $core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;page=[#]', 'Aller à la page : ');
			
			$page->moduleFooter();
		} else {
 	  	$page->msgInfo(locale::display('no_event','events'));
		}
	
	}//end displayFutureEvents()

	public function displayEventDate($date_af, $cur_page = 1){
  	global $core,$db,$page;
		$core->loadClass('date');
		$date = new Date();
		if(!isset($page))$page = new Page();

		//pages management
		$start_row = ($cur_page - 1) * ($this->config['items_per_page']);
		
		echo '<div class="clear"></div>';
		
  	$request ="SELECT
							categ.image AS image,
							categ.name AS event_type,
							eventsListe.lieu AS event_place,
					    eventsListe.actif AS event_active,
					    eventsListe.date AS event_date,
					    eventsListe.auteur AS event_author,
					    eventsListe.description AS event_description,
					    eventsListe.titre AS event_title,
					    eventsListe.id AS event_id,
					    eventsListe.heure AS event_hour,
							eventsListe.image AS event_image
							 
		FROM eventsListe,
				 kernel_tag_category_user categ,
				 kernel_tag_information info,
				 kernel_tag_information_category info_categ
		
		WHERE eventsListe.actif = 'Y'
		AND   info.idOrig = eventsListe.id
		AND   info.tableOrig = 'eventsListe'
		AND	  info_categ.idInformation = info.id
		AND   info_categ.idCategory = categ.idCategory
		AND   categ.isShow = 'true'
		AND   categ.login = '".$this->login."'
		AND   eventsListe.date = '".$date_af."' 
		
		ORDER BY event_hour ASC";
		
		$result = $db->query($request." LIMIT ".$start_row.", ".$this->config['items_per_page']." ");
	
		if ($db->numRows($result) > 0) {
			$page->moduleHeader(locale::display('events_this_day','events').$date->reverseDate($date_af),'');
			//$style alterns events display style
			$style=true;
			
			$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'href', $core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;date_af='.$date_af.'&amp;page=[#]', 'Aller à la page : ');
			
   		while ($row = $db->fetchArray($result)){
				$this->displayEvent($row,$style);
				$style=!$style;
   		}
			
			$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'href', $core->makeUrl('modules/module_agenda/index.php').'?ga=d&amp;date_af='.$date_af.'&amp;page=[#]', 'Aller à la page : ');
			
			$page->moduleFooter();
		} else {
 	  	$page->msgInfo(locale::display('no_event','events').' le '.$date->reverseDate($date_af));
		}
	}
	
	public function displayEventId($id){
  	global $core,$db,$page;
		if(!isset($page))$page = new Page();
		
		echo '<div class="clear"></div>';

		//OK TAGGING
		$result = $db->query("
		SELECT 			eventsListe.lieu AS event_place,
					      eventsListe.actif AS event_active,
					      eventsListe.date AS event_date,
					      eventsListe.auteur AS event_author,
					      eventsListe.description AS event_description,
					      eventsListe.titre AS event_title,
					      eventsListe.id AS event_id,
					      eventsListe.heure AS event_hour,
								eventsListe.image AS event_image,
								categ.image AS image,
								categ.name AS event_type
								
		FROM 				eventsListe,
								kernel_tag_category_user categ,
								kernel_tag_information info,
								kernel_tag_information_category info_categ
		
		WHERE 			eventsListe.actif = 'Y'
		AND					info.idOrig = eventsListe.id
		AND					info.tableOrig = 'eventsListe'
		AND					info_categ.idInformation = info.id
		AND					info_categ.idCategory = categ.idCategory
		AND					categ.isShow = 'true'
		AND 				categ.login = '".$this->login."'
		AND 				eventsListe.id = '".$id."'
		");
		
		if ($db->numRows($result) > 0) {
			$page->moduleHeader('','');
   		while ($row = $db->fetchArray($result)){
				$this->displayEvent($row);
   		}
			$page->moduleFooter();
		} else {
 	  	$page->msgInfo(locale::display('no_event','events'));
		}

	}


	public function saveEvent($frm, $files = Array()){
		global $core, $db, $page;
  	$core->loadClass('date');
		$core->loadClass('tag',dirname(__FILE__).'/../include/engine/tag');
		$core->loadClass('tagInterface',dirname(__FILE__).'/../include/engine/tag');
		$core->loadClass('file');
  	$date = new Date();
		$tag = new Tag(Array('eventsListe'=>'id'), $core, $db);
		$file = new file();

		$errors = Array();

		//On insere un éventuel nouvel évenement
		if (!isset($frm['date']) || $frm['date'] == ''){
			$errors[] = 'Aucune date n\'a été fournie.';
		}else{
			if(!$date->isDate($frm['date'], 'FR')){
				$errors[] = 'La date fournie n\'est pas valide. Veuillez entrer une date au format : jj/mm/aaaa. Vous pouvez vous serivr du calendrier..."';
			}
		}
		
		if (23 < (int)$frm['hours'] || (int)$frm['hours'] < 0 || 55 < (int)$frm['minutes'] || (int)$frm['minutes'] < 0 ){
			$errors[] = 'L\'heure fournie n\'est pas valide. Veuillez entrer une heure au format : hh/mm. Utilisez les listes déroulantes...';
		}else{
			$hour = $frm['hours'].':'.$frm['minutes'];
		}
		
		if (!isset($frm['titre']) || $frm['titre'] == ''){
			$errors[] = 'Aucun titre n\'a été fourni. Veuilez entrer un intitulé pour votre événement.';
		}
		if (!isset($frm['description']) || $frm['description'] == '' ){
			$errors[] = 'Vous n\'avez pas fourni de description pour votre événement, veuillez en entrer une.';
		}
		
		$image = '';	
		if( isset($files['image']) || $files['image'] == ''){
			do{
				$img_id=rand();
			}while(is_file('affiches/'.$img_id.'.jpg'));
			$image = $img_id.'.jpg';
		}

		if(!isset($frm['category']) || $frm['category'] == '' || $frm['category'] == 0){
			$errors[] = 'Vous n\'avez pas renseigné la catégorie de l\'événement, veuillez en sélectionner une.';
		}
			
		if(count($errors) == 0){
	    	//on insère dans la base
  	  	$res = $db->query("INSERT INTO eventsListe (login,date,heure,titre,description,auteur,lieu,image,actif) ".
					" VALUES( ".
						" '".$core->getUserId()."' , ".
						" '".$date->reverseDate($frm['date'])."' , ".
						" '".$db->escapeString($hour)."' , ".
						" '".$db->escapeString(htmlentities($frm['titre'], ENT_QUOTES, 'UTF-8'))."' , ".
						" '".$db->escapeString(htmlentities($frm['description'], ENT_QUOTES, 'UTF-8'))."' , ".
						" '".$db->escapeString(htmlentities($frm['auteur'], ENT_QUOTES, 'UTF-8'))."' , ".
						" '".$db->escapeString(htmlentities($frm['lieu'], ENT_QUOTES, 'UTF-8'))."' , ".
						" '".$image."' , ".
						" '". (($core->verifDroits($this->config['admin_rights']))?'Y':$this->config['auto_valid'])."' ".
					" )"
				);
			
				$last_insert_id = $db->insertId();
				$idInformation = $tag->getIdInformation('eventsListe',$last_insert_id);
				$tag->setInformationCategory($idInformation, $frm['category']);
	
	    	if(isset($files['image'])){
  	    	$image_url = 'affiches/'.$image;
    	  	switch($files['image']['error']) {
      	  	case 0:
    			  	//On ne veut que des images jpeg
          		$size = getimagesize($files['image']['tmp_name']);
          		if ($size[2] >= 1 && $size[2] <= 3) {
            		//On enregistre l'image
   	        		if(move_uploaded_file($files['image']['tmp_name'], $image_url)){
			          	//On redimensionne l'image grace à la classe file
		  	        	if($file->image_max_size($image_url,$this->config['image_height'],$this->config['image_width']) == false){
										$page->msgError(locale::display('img_error','event'));
									}
				  			} 
      	    	}
          	break;
      			}
    			}
					
    			//on affiche le message de confirmation
					if($core->verifDroits($this->config['admin_rights']) || $this->config['auto_valid'] == 'Y' ){
	  	 	 		$page->msgInfo(locale::display('event_recorded_webmaster','events'));  
					}else{
						$page->msgInfo(locale::display('event_recorded','events'));
					}

					$this->displayHeader();
					$this->displayTypes();
					$this->displayFutureEvents(1);
					
			}else{
				$error_msg = '<p>Des erreurs ont été commises :</p><ul>';
				foreach($errors as $er){
					$error_msg .= '<li>'.$er.'</li>';
				}
				$error_msg .= '</ul>';
				
				$page->msgError($error_msg);
				$this->displayHeader();
				$this->displayTypes();
				$this->displayAddForm($frm);
			}
			
	}


	public function displayAddForm($frm = '') {
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		$form = new Form();
		
		$val = (is_array($frm));
		
		echo '<div class="clear"></div>';
		$page->moduleHeader('Ajouter un événement');
  	
		echo '
			<form action="index.php" method="post" name="frmajout" enctype="multipart/form-data">
				<div class="input-hidden">
					<input type="hidden" name="ga" value="s" />
				</div>
        <p class="form-input"><label>Type d\'événement *° :</label>
        <select multiple="multiple" name="category[]" size="3">';
				
    $result = $db->query("SELECT categ.idCategory as id, categ.name as name FROM kernel_tag_category_user_set categ WHERE categ.login = '".(int)$core->getUserId()."'");
		
    while ($row=$db->fetchArray($result)) {
      echo '<option value="'.$row['id'].'" >'.stripslashes($row['name']).'</option>';
    }
	  
    echo '
        </select></p>
        <p class="form-date"><label>'.locale::display('date','kernel').' * :</label>
        	<input type="text" name="date" size="10" maxlength="10" id="date" value="'.( ($val && isset($frm['date'])) ? $frm['date'] : '' ).'" onblur="validateInput(this, \'DATE_FR\');" />'.$form->linkCalendarTo('date').'
				</p>
        <p class="form-input"><label>'.locale::display('hour','kernel').' :</label>
					<select name="hours" >';
					for($i = 0; $i < 24; $i++){
						$h = $i < 10 ? '0'.$i : $i;
						$sel = ($val && $frm['hours'] == $h) ? ' selected="selected" ' : ($i == 8 ? ' selected="selected" ' : '');
						echo '<option value="'.$h.'" '.$sel.' >'.$h.'</option>
						';
					}
		echo '</select> h '.
					'<select name="minutes" >';
					for($i = 0; $i < 60; $i+=5){
						$m = $i < 10 ? '0'.$i : $i;
						$sel = ($val && $frm['minutes'] == $m) ? ' selected="selected" ' : '';
						echo '<option value="'.$m.'" '.$sel.' >'.$m.'</option>
						';
					}
		echo '</select>
				</p>
        <p class="form-input"><label>'.locale::display('place','kernel').' :</label><input type=text name="lieu" size="30"  value="'.( ($val && isset($frm['lieu'])) ? $frm['lieu'] : '' ).'" /></p>
        <p class="form-input"><label>'.locale::display('title','kernel').' * :</label><input type=text name="titre" size="30"  value="'.( ($val && isset($frm['titre'])) ? $frm['titre'] : '' ).'" onblur="validateInput(this, \'SOMETHING\');" /></p>
        <p class="form-input">
					<label>'.locale::display('description','kernel').' * :</label>
					<textarea name="description" rows="10" cols="50">'.($val && isset($frm['description']) ? $frm['description'] : '').'</textarea>
				</p>';

		$res = $db->query("SELECT firstname, lastname FROM trombi_trombi where id_user = '".$core->getUserId()."'");
		$row = $db->fetchArray($res);

		echo '
        <p class="form-input"><label>'.locale::display('author','kernel').' :</label><input type="text" name="auteur" size="30" value="'.( ($val && isset($frm['auteur'])) ? $frm['auteur'] : stripslashes($row['firstname']).' '.stripslashes($row['lastname']) ).'" /></p>
        <p class="form-input"><label>'.locale::display('image_jpg','events').' :</label>
        <input type="file" name="image" size="28" /></p>
        <p class="form-submit"><input type="submit" value="Ajouter" /></p>
	 			<p>° Si vous ne trouvez pas de catégorie adéquate, contactez le <a href="mailto:'.$core->getConfig('mailWebmaster').'">webmaster</a> pour la rajouter.</p>
		</form>';

	$page->moduleFooter();
	}



	
}
?>
