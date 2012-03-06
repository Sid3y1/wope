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


/******************* module_agenda_admin.class **********************************
Definition pour l'admin de l'agenda
**********************************************************************/
class module_agenda_admin {
	
	private $id;

	private $config;

	private $path;

	public final function __construct()
	{
		global $core;

		$this->path = 'modules/module_agenda/admin';
		include($core->getConfig('baseServer').'/'.$this->path.'/config_admin_module_agenda.inc');
		include($core->getConfig('baseServer').'/'.$this->path.'/../config_module_agenda.inc');
		
		//pay attention to the order, possible identical keys are set to the second value
		$this->config = array_merge($config_module_agenda, $config_admin_module_agenda);
		$this->id = rand(1000000,9999999);
    $_SESSION['admin_agenda_'.$this->id] = serialize($this);
	}
						
	public function displayContent($event = null) {
		global $core,$page;
		if(!isset($page))$page= new Page();
		$page->moduleHeader('Module Agenda','');
		echo '<div id="module_agenda_admin_content">';
		if( $event != null ){
			$this->displayUpdateEvent($event);
		}else{
			$this->displayEvents();
		}
		echo '</div>';
		$page->moduleFooter();
	}

	public function displayMenu() {
		global $core,$page;
		if(!isset($page))$page= new Page();
		$page->moduleHeader('Module Agenda','');
		echo '<ul class="panel">
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'?id_admin='.$this->id.'&content=display_events\',\'module_agenda_admin_content\')">Evénements</a></li>
						<li class="link_menu"><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'?id_admin='.$this->id.'&content=add_event\',\'module_agenda_admin_content\')">Ajouter un événement</a></li>
					</ul>';
		$page->moduleFooter();
	}

	public function displaySummary($events_nb, $period){
		global $page, $core, $db;
		if(!isset($page))$page = new Page();

		$result = $db->query("SELECT id, titre FROM eventsListe WHERE actif='N' AND date > NOW() ORDER BY date ASC LIMIT ".$events_nb." ");
		$nb = $db->numRows($result);

		$page->moduleHeader('Evénements à modérer ( '.$nb.' )', '');
		echo '<h4>Derniers événements à modérer :</h4>';
	
		if($db->numRows($result) != 0 ){
			$i = 0;
			$line_style = '';
			while($row = $db->fetchArray($result)) {
				if ($i%2) {
					$line_style = 'line_1';
				} else {
						$line_style = 'line_2';
					}
				echo '<div class="admin_home_item_list '.$line_style.'"><a href="'.$core->makeUrl('applications/admin/index.php').'?part=module&amp;content=module_agenda&amp;option='.$row['id'].'">'.$row['titre'].'</a></div>';
				$i++;
			}
		}else{
			echo '<div><i>Aucun événement futur à modérer.</i></div>';
		}

		echo '<div class="end_list_clear"></div><div>';
		$page->adminLink('module', 'module_agenda', $this->config['admin_rights']);
		echo '</div>';
		
		$page->moduleFooter();
	}

	public function displayEvents($cur_page = 1){
		global $core,$db, $page;
		if(!isset($page))$page = new Page();
		$core->loadClass('date');
		$core->loadClass('box');
		$box = new Box();
		$date = new Date();

		$start_row = ($cur_page - 1) * $this->config['items_per_page'];
		
	
		$request = "SELECT eventsListe.id, 
					 eventsListe.titre, 
					 eventsListe.heure, 
					 eventsListe.date, 
					 categ.image AS imageMini, 
					 categ.name AS type, 
					 eventsListe.actif 
					 
		FROM 	 eventsListe, kernel_tag_information info 
		
		LEFT JOIN ( kernel_tag_information_category info_categ
								JOIN
								kernel_tag_category_user categ)
		ON (info.id = info_categ.idInformation
				AND
				categ.idCategory = info_categ.idCategory)

		WHERE info.tableOrig = 'eventsListe'
		AND info.idOrig = eventsListe.id
		GROUP BY eventsListe.id
		ORDER BY date DESC";
		
		$result = $db->query($request." LIMIT ".$start_row.", ".$this->config['items_per_page']." ");
		
		$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/content.php').'?id_admin='.$this->id.'&amp;content=display_events&amp;page=[#]\',\'module_agenda_admin_content\')');
		
		echo '
		<table align="center" class="data_table" width="95%">
		<tr>
	 		<th width="10%">Date</th>
	 		<th width="10%">Heure</th>
	 		<th width="30%">Titre</th>
	 		<th width="20%">Type</th>
	 		<th width="15%">Validation</th>
	 		<th width="15%">Suppression</th>
		</tr>
		';
		
		while ($row = $db->fetchArray($result)) {
		$class='';
	   if ($row['actif']=='N') {
	      $class=' class="no_active" ';
	   }
	   echo '<tr '.$class.' >
	    <td align="center"><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_event&id_event='.$row['id'].'\',\'module_agenda_admin_content\')">'.$date->reverseDate($row['date']).'</a></td>
	    <td align="center"><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_event&id_event='.$row['id'].'\',\'module_agenda_admin_content\')">'.substr($row['heure'], 0, 5).'</a></td>
	    <td><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_event&id_event='.$row['id'].'\',\'module_agenda_admin_content\')">'.stripslashes($row['titre']).'</a></td>
	    <td align="center"><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_event&id_event='.$row['id'].'\',\'module_agenda_admin_content\')">';
			if($row['type'] != ''){
				echo '<img alt="Icône '.stripslashes($row['type']).'" src="'.$core->makeUrl('data/files/categ_img/'.$row['imageMini']).'" width="30px" height="30px">';
			}
			echo '</a></td>
	    <td align="center">';

	   if ($row['actif']=='N') {
	      echo '<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'?id_admin='.$this->id.'&content=validate_event&id_event='.$row['id'].'\',\'module_agenda_admin_content\')">Valider</a>';
	   } else {
              echo '<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'?id_admin='.$this->id.'&content=cancel_event&id_event='.$row['id'].'\',\'module_agenda_admin_content\')">Retirer</a>';
	   }
	        	    
	    echo '</td>
	    <td align="center"><a class="trash" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'?id_admin='.$this->id.'&content=del_event&id_event='.$row['id'].'\',\'module_agenda_admin_content\');', 'Etes-vous sûr de vouloir supprimer cet événement ?').'"></a></td>

  </tr>';
	}
	echo '
	</table>';
		
		$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/content.php').'?id_admin='.$this->id.'&amp;content=display_events&amp;page=[#]\',\'module_agenda_admin_content\')');

}

	public function displayUpdateEvent($id_event){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		$core->loadClass('texteditor');
		$core->loadClass('date');
		$date = new Date();

		echo '<div id="update_event_feedback" style="display:none" >';
		$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		
		$res = $db->query("SELECT login, id_events_type, date, heure, titre, description, auteur, lieu, image FROM eventsListe WHERE id='".$id_event."'");
		
		$result = $db->query("SELECT
										               categ.id,
														       categ.name,
																	 
												  IF(info.id IS NULL, 'false', 'true') as selected																																																	            
													FROM kernel_tag_category categ
													
													
													LEFT JOIN (kernel_tag_information info
													
													INNER JOIN kernel_tag_information_category info_categ) ON (categ.id = info_categ.idCategory
																																								 AND info.tableOrig='eventsListe'
																																								 AND info.idOrig='".$id_event."'
																																								 AND info.id=info_categ.idInformation)
																																																																																											" );
																																	 
		$row = $db->fetchArray($res);
		
		echo '<div id="cal_container"></div>';
		echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'" onsubmit="AJAX.submitAndUpdate(this,true,\'module_agenda_admin_content\');display(gE(\'update_event_feedback\'));return false">';
			echo '<input type="hidden" name="id_admin" value="'.$this->id.'" />';
			echo '<input type="hidden" name="content" value="save_event" />';
			echo '<input type="hidden" name="event_save_case" value="update" />';
			echo '<input type="hidden" name="id_event" value="'.$id_event.'" />';
			echo '<input type="hidden" name="event_login" value="'.$row['login'].'" />';
			
			echo '<p class="form-input"><label>Type d\'événement* :</label>';
			echo '<select multiple name="category[]" size="3">';
			while ($row_select = $db->fetchArray($result)) {
      	if($row_select['selected'] == 'true' ) {
			   	 $sel = 'selected="selected"';
				} else {
				   $sel ='';
				}
				echo '<option '.$sel.' value="'.$row_select['id'].'">'.stripslashes($row_select['name']).'</option>';
			}
			echo '</select></p>';
			
			echo '<p class="form-date">
				<label>Date *:</label>
				<input type="text" id="event_date" name="event_date" size="10" value="'.$date->reverseDate($row['date']).'" />
				<a class="calendar_button" onclick="selectDate(\'cal_container\', '.
					'function(date){'.
						' gE(\'event_date\').value = date; '.
					'}, true, \'FR\');">&nbsp;
				</a>
			</p>';
		
			echo '<p class="form-input">
				<label>'.locale::display('hour','kernel').' :</label>
				<select name="event_hours" >';
        for($i = 0; $i < 24; $i++){
				  $h = $i < 10 ? '0'.$i : $i;
					$sel = substr($row['heure'], 0, 2)  == $h ? ' selected="selected" ' : '';
				  echo '<option value="'.$h.'" '.$sel.' >'.$h.'</option>
				  ';
				}
			echo '</select> h '.
				'<select name="event_minutes" >';
        for($i = 0; $i < 60; $i+=5){
				  $m = $i < 10 ? '0'.$i : $i;
					$sel = substr($row['heure'], 3, 2) == $m ? ' selected="selected" ' : '';
				  echo '<option value="'.$m.'" '.$sel.' >'.$m.'</option>
				  ';
				}
			echo '</select>
			</p>';
			echo '<p class="form-input"><label>Lieu :</label><input type="text" name="event_place" size="20" value="'.html_entity_decode(stripslashes($row['lieu']), ENT_QUOTES, 'UTF-8').'" /></p>';
			echo '<p class="form-input"><label>Intitulé * :</label><input type="text" name="event_title" value="'.html_entity_decode(stripslashes($row['titre']), ENT_QUOTES, 'UTF-8').'" size="40" /></p>';
			echo '<p class="form-textarea">
				<label>Description de l\'événement * :</label>
			';
			$te = new TextEditor(
				'event_description',
				Array(
					'rows'=>12,
					'cols'=>50,
					'value'=>stripslashes($row['description'])
				)
			);
			$te->display();
			echo '
			</p>';
			echo '<p class="form-input"><label>Auteur :</label><input type="text" name="event_author" value="'.html_entity_decode(stripslashes($row['auteur']), ENT_QUOTES, 'UTF-8').'" /></p>';
			echo '<p class="form-input"><label>Image associée :</label>';
			if(isset($row['image']) && !empty($row['image']) ){
				echo '<img alt="Image de '.stripslashes($row['titre']).'" src="'.$core->makeUrl('modules/module_agenda/affiches').'/'.$row['image'].'" /></p>
				<p class="form-input"><label></label><input type="checkbox" name="del_event_img" value="yes" /><span> Pas d\'image</span></p><p class="form-input"><label>Nouvelle image :</label>';
			}
			echo '<input type="file" name="event_image" size="19" /></p>';
			echo '<p class="form-submit">
							<input type="submit" value="Modifier" /></p>
		</form>'; 
	
	}

	public function displayAddEvent(){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		$core->loadClass('texteditor');

		$nom_prenom = $db->query("SELECT firstname, lastname FROM trombi_trombi WHERE id_user = '" .$core->getUserId(). "'");
    while ($row = $db->fetchArray($nom_prenom)) {
			$author = stripslashes($row['firstname'].' '.$row['lastname']);
		}
		
		$result = $db->query("SELECT id, name
													FROM kernel_tag_category");
	
			echo '<div id="cal_container"></div>';
			echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_agenda/admin/ajax/content.php').'" onsubmit="AJAX.submitAndUpdate(this,true,\'module_agenda_admin_content\');display(gE(\'update_event_feedback\'));return false">';
			echo '<input type="hidden" name="id_admin" value="'.$this->id.'" />';
			echo '<input type="hidden" name="content" value="save_event" />';
			echo '<input type="hidden" name="event_save_case" value="add" />';
			echo '<input type="hidden" name="event_login" value="'.$core->getUserId().'" />';
			
			echo '<p class="form-input"><label for="category">Type d\'événement* :</label>';
			echo '<select multiple name="category[]" size="3">';
			while ($row_select = $db->fetchArray($result)) {
				echo '<option value="'.$row_select['id'].'">'.stripslashes($row_select['name']).'</option>';
			}
			echo '</select></p>';
			
			echo '<p class="form-date">
				<label for="event_date">Date * :</label>
				<input type="text" id="event_date" name="event_date" size="10" />
				<a class="calendar_button" onclick="selectDate(\'cal_container\', '.
					'function(date){'.
						' gE(\'event_date\').value = date; '.
					'}, true, \'FR\');">&nbsp;
				</a>
			</p>';
			echo '<p class="form-input">
				<label for="event_hours">'.locale::display('hour','kernel').' :</label>
				<select name="event_hours" >';
        for($i = 0; $i < 24; $i++){
				  $h = $i < 10 ? '0'.$i : $i;
					$sel = $h == '08' ? ' selected="selected" ' : '';
				  echo '<option value="'.$h.'" '.$sel.' >'.$h.'</option>
				  ';
				}
			echo '</select> h '.
				'<select name="event_minutes" >';
        for($i = 0; $i < 60; $i+=5){
				  $m = $i < 10 ? '0'.$i : $i;
				  echo '<option value="'.$m.'" >'.$m.'</option>
				  ';
				}
			echo '</select>
			</p>';
			echo '<p class="form-input"><label for="event_place">Lieu :</label><input type="text" name="event_place" size="20" /></p>';
			echo '<p class="form-input"><label for="event_title" >Intitulé * :</label><input type="text" name="event_title" size="40" /></p>';
			echo '<p class="form-textarea">
				<label for="event_description">Description de l\'événement * :</label>
			';
			$te = new TextEditor(
				'event_description',
				Array(
					'rows'=>12,
					'cols'=>50,
				)
			);
			$te->display();
			echo '
			</p>';
			echo '<p class="form-input"><label for="event_author">Auteur :</label><input type="text" name="event_author" value="'.$author.'" /></p>';
			echo '<p class="form-input"><label for="event_image">Image associée :</label>';
			echo '<input type="file" name="event_image" size="19" /></p>';
			echo '<p class="form-submit">
							<input type="submit" value="Ajouter" /></p>
			</form>';

	}

	public function deleteEvent($id_event){
		global $db;
		$res = $db->query("SELECT image FROM eventsListe WHERE id='".$id_event."'");
		while($img = $db->fetchArray($res)){
			$f = '../affiches/'.$img['image'];
			if(is_file($f))	unlink($f);
		}
		$db->query("DELETE FROM eventsListe WHERE id='".$id_event."'");
	}

	public function validateEvent($id_event,$actif){
		global $db;
		$db->query("UPDATE eventsListe SET actif='".$actif."' WHERE id='".$id_event."'");
	}


	public function saveEvent($infos, $image=''){
		global $core, $db;
		$core->loadClass('date');
    $core->loadClass('file');
    $core->loadClass('tag',dirname(__FILE__).'/../include/engine/tag');
	  $core->loadClass('tagInterface',dirname(__FILE__).'/../include/engine/tag');
		$date = new Date();
    $file = new file();
    $tag = new Tag(array('eventsListe' => 'id'),$core,$db);
		
	 	$img_id = rand();
		
		$time = ( isset($infos['event_hours']) && $infos['event_hours'] != '' && isset($infos['event_minutes']) && $infos['event_minutes'] != '') ? $infos['event_hours'].':'.$infos['event_minutes'] : '';
		
		switch($infos['event_save_case']){
		case 'add':
			$infos['event_date'] = $date->reverseDate($infos['event_date']);
			$db->query("INSERT INTO eventsListe (login, date, heure, titre, description, auteur, actif, lieu, image) VALUES (
				'".$infos['event_login']."', 
				'".$infos['event_date']."',
				'".$time."',
				'".$db->escapeString(htmlentities($infos['event_title'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString($infos['event_description'])."',
				'".$db->escapeString(htmlentities($infos['event_author'], ENT_QUOTES, 'UTF-8'))."',
				'Y',
				'".$db->escapeString(htmlentities($infos['event_place'], ENT_QUOTES, 'UTF-8'))."',
				'".((isset($image['name']))?$img_id.'.jpg':'')."'
			)");
			
			$id = $db->insertId();
			$idInformation = $tag->getIdInformation('eventsListe',$id);
			$tag->setInformationCategory($idInformation, (isset($infos['category']) ? $infos['category'] : null));
			break;
			
		case 'update':
		
    $id = $infos['id_event'];
    $idInformation = $tag->getIdInformation('eventsListe', $id);
		$tag->setInformationCategory($idInformation, (isset($infos['category']) ? $infos['category'] : null));
		
			$infos['event_date']=$date->reverseDate($infos['event_date']);
			if((isset($image['name']) && $image['name']!='') || (isset($infos['del_event_img']) && $infos['del_event_img']=='yes')) {
				$res = $db->query("SELECT image FROM eventsListe WHERE id='".$infos['id_event']."'");
				while($img = $db->fetchArray($res)) {
					$f = '../affiches/'.$img['image'];
					if(is_file($f))	unlink($f);
				}
			}


			
			if(isset($image['name']) && $image['name']!=''){
    		$db->query("UPDATE eventsListe SET login='".$infos['event_login']."', date='".$infos['event_date']."', ".
				" heure='".$time."', titre='".$db->escapeString(htmlentities($infos['event_title'], ENT_QUOTES, 'UTF-8'))."', description='".$db->escapeString($infos['event_description'])."', ".
				" auteur='".$db->escapeString(htmlentities($infos['event_author'], ENT_QUOTES, 'UTF-8'))."', lieu='".$db->escapeString(htmlentities($infos['event_place'], ENT_QUOTES, 'UTF-8'))."', image='".$img_id.'.jpg'."' ".
				" WHERE id='".$infos['id_event']."'");
			} else {
    		$db->query("UPDATE eventsListe SET login='".$infos['event_login']."', date='".$infos['event_date']."', heure='".$time."', ".
				" titre='".$db->escapeString(htmlentities($infos['event_title'], ENT_QUOTES, 'UTF-8'))."', description='".$db->escapeString($infos['event_description'])."', ".
				" auteur='".$db->escapeString(htmlentities($infos['event_author'], ENT_QUOTES, 'UTF-8'))."', lieu='".$db->escapeString(htmlentities($infos['event_place'], ENT_QUOTES, 'UTF-8'))."', image=".((isset($infos['del_event_img']) && $infos['del_event_img']=='yes')?"NULL":"image").
				" WHERE id='".$infos['id_event']."'");
			}
		break;
		default:
			//void
			break;
		}

		if(isset($image) && $image['name']!='') {
	 		$url = $core->getConfig('baseServer').'/modules/module_agenda/affiches/'.$img_id.'.jpg';
	 		switch($image['error']) {
	    	case 0:
	       	$size = getimagesize($image['tmp_name']);
					//type d'image : gif, jpeg, png acceptes
	       	if ($size[2] >= 1 && $size[2] <= 3) {
		  			//On enregistre l'image
		  			if(move_uploaded_file($image['tmp_name'], $url)){
             
            	 //On redimensionne l'image grace à la classe file
            	if($file->image_max_size($url,150,150) == false){
								$page->msgError(locale::display('img_error','event'));
							}
		  			}
	     		}
			}

		}
	}

}
?>
