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



class module_survey_admin {
	
	private $id;

	public final function __construct()
	{
    global $core;
		$this->path = 'modules/module_survey/admin';
		include($core->getConfig('baseServer').'/'.$this->path.'/config_admin_module_survey.inc');
		include($core->getConfig('baseServer').'/'.$this->path.'/../config_module_survey.inc');

		//pay attention to the order, possible identical keys are set to the second value
		$this->config = array_merge($config_module_survey, $config_admin_module_survey);
		$this->id = rand(1000000,9999999);
    $_SESSION['admin_survey_'.$this->id] = serialize($this);
	}



	public function displayContent() {
		global $core,$db,$page;
		if(!isset($page))$page= new Page();
		
		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class='module_survey' "), 0);
		$page->moduleHeader(ucfirst($title), '');
		echo '<div id="module_survey_admin_content">';
		$this->displaySurveysList();
		echo '</div>';
		$page->moduleFooter();
	}


	public function displayMenu() {
		global $core,$page,$db;
		if(!isset($page))$page= new Page();

		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class='module_survey' "), 0);
		$page->moduleHeader(ucfirst($title), '');
		echo '<ul class="panel">'
						.'<li class="link_menu">'
							.'<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_survey/admin/ajax/content.php').''
									.'?id_admin='.$this->id.''
									.'&content=displayList\',\'module_survey_admin_content\')">Listes des sondages</a>'
						.'</li>'
						.'<li class="link_menu">'
							.'<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_survey/admin/ajax/content.php').''
									.'?id_admin='.$this->id.''
									.'&content=addSurvey\',\'module_survey_admin_content\')">Nouveau sondage'
							.'</a>'
						.'</li>'
						.'<li class="link_menu">'
							.'<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_survey/admin/ajax/content.php').''
									.'?id_admin='.$this->id.''
									.'&content=displayLegend\',\'module_survey_admin_content\')">Légende des couleurs'
							.'</a>'
						.'</li>'
					.'</ul>';
		$page->moduleFooter();
	}


	public function displayLegend() {

		echo '<div class="clear sep"></div>' 
				.'<a class="color_legend proposal">&nbsp;</a><span class="legend_span">Sondage venant d\'être proposé. (en attente de validation)</span>'
				.'<div class="clear sep"></div>'
				.'<a class="color_legend active">&nbsp;</a><span class="legend_span">Sondage actif.</span>'
				.'<div class="clear sep"></div>'
				.'<a class="color_legend">&nbsp;</a><span class="legend_span">Sondage inactif.</span>'
				.'<div class="clear sep"></div>'
				.'<a class="color_legend ended">&nbsp;</a><span class="legend_span">Sondage clos.</span>'
				.'<div class="clear sep"></div>'
				.'<a class="color_legend archived">&nbsp;</a><span class="legend_span">Sondage Archivé.</span>'
				.'<div class="clear sep"></div>';
	}



	public function displaySurveysList($cur_page = 1) {
		global $core,$db, $page;
		
		if(!isset($page))$page = new Page();
		$core->loadClass('date');
		$core->loadClass('box');
		$box = new Box();
		$date = new Date();
	  $start_row = ($cur_page - 1) * $this->config['items_per_page'];
    $request = "SELECT id, title, date, active, ended, archive, proposal FROM module_survey ORDER BY date DESC";
		
		$result = $db->query($request." LIMIT ".$start_row.",".$this->config['items_per_page']);

		echo '<table align="center" class="data_table" width="95%">'
						.'<tr>'
							 .'<th width="55%">Titre</th>'
							 .'<th width="15%">Date de création</th>'
							 .'<th width="5%"></th>'
							 .'<th width="5%">Clos</th>'
							 .'<th width="5%">Actif</th>'
							 .'<th width="5%">Supprimer</th>'
							 .'<th width="5%">Archiver</th>'
						.'</tr>';																				

    while ($row = $db->fetchArray($result)) {
			$class='';
			if ($row['active'] == 'N' && $row['ended'] == 'Y') {
				$class=' class="ended" ';
			}
			if ($row['active'] == 'Y') {
				$class = ' class="active"';
				if ($row['ended']=='Y') {
					$class=' class="ended" ';
				}
			}
			if ($row['proposal'] == 'Y') {
				$class=' class="proposal"';
			}
			if ($row['archive'] == 'Y') {
				$class=' class="archived"';
			}
		
			$box_content = $this->displayTargetSurvey($row['id']);
			$archive_content = 'OK';
			$ended_content = '-';
			$active_content = '-';
			if ($row['archive'] == 'N') {
					$archive_content = '<a class="edit" onclick="'.$box->confirm('AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_survey/admin/ajax/content.php').'?id_admin='.$this->id.'&content=archiveSurvey&id_survey='.$row['id'].'&cur_page='.$cur_page.'\',\'module_survey_admin_content\');', 'Etes-vous sûr de vouloir archiver ce sondage ? Cette opération est définitive, seuls les résultats du sondage seront disponibles.').'">&nbsp;</a>';
					$ended_content = '<a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_survey/admin/ajax/content.php').'?id_admin='.$this->id.'&content=activateSurvey&id_survey='.$row['id'].'&cur_page='.$cur_page.'&ended_status='.$row['ended'].'&active_status='.$row['active'].'\',\'module_survey_admin_content\')">'.$row['active'].'</a>';
					$active_content = '<a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_survey/admin/ajax/content.php').'?id_admin='.$this->id.'&content=closeSurvey&id_survey='.$row['id'].'&cur_page='.$cur_page.'&ended_status='.$row['ended'].'&active_status='.$row['active'].'\',\'module_survey_admin_content\');">'.$row['ended'].'</a>';
					
			}
		
			echo '<tr '.$class.' >'
			
							.'<td align="center">'
									.'<a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_survey/admin/ajax/content.php').'?id_admin='.$this->id.'&content=updateSurvey&id_survey='.$row['id'].'&cur_page='.$cur_page.'\',\'module_survey_admin_content\')">'.stripslashes($row['title']).'</a>'
							.'</td>'
							.'<td align="center">'
									.'<a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_survey/admin/ajax/content.php').'?id_admin='.$this->id.'&content=updateSurvey&id_survey='.$row['id'].'&cur_page='.$cur_page.'\',\'module_survey_admin_content\')">'.$date->reverseDate($row['date']).'</a>'
							.'</td>'
							.'<td align="center">'
								.'<a class="see" onclick="'.$box->info($box_content).';">&nbsp;</a>'
							.'</td>'
							.'<td align="center">'
									.$active_content
							.'</td>'
							.'<td align="center">'
									.$ended_content
							.'</td>'
							.'<td align="center">'
									.'<a class="delete" onclick="'.$box->confirm('AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_survey/admin/ajax/content.php').'?id_admin='.$this->id.'&content=deleteSurvey&id_survey='.$row['id'].'&cur_page='.$cur_page.'\',\'module_survey_admin_content\');', 'Etes-vous sûr de vouloir supprimer ce sondage ?').'">&nbsp;</a>'
							.'</td>'
							.'<td align="center">'
									.$archive_content
							.'</td>';
						
		}
		
		echo '</tr></table>';
		
		$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/content.php').'?id_admin='.$this->id.'&amp;content=displayList&amp;cur_page=[#]\',\'module_survey_admin_content\')');

	}


	public function gatherSurveyInfos($id_survey) {
		global $db;

		$titles_query = "SELECT id, title, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, archive, ended FROM module_survey WHERE id = '".$id_survey."'";
		$scores_query = "SELECT COUNT(*) AS result FROM module_survey_answers WHERE id_survey='".$id_survey."' ";
		$survey_infos = Array();
		
		// WE STORE THE TITLE OF THE SURVEY
		$survey_data = $db->fetchArray($db->query($titles_query));
		$survey_infos['title'] = stripslashes($survey_data['title']);
		$survey_infos['archive'] = $survey_data['archive'];
		$survey_infos['ended'] = $survey_data['ended'];
			
		// FOR EACH ANSWER, WE STORE THE TITLE AND THE RELATED NUMBER OF VOTES
		for ($i = 1; $i<=10; $i++) {
			$survey_infos['q'.$i]['title'] = stripslashes($survey_data['q'.$i]);
		  $survey_infos['q'.$i]['result'] = stripslashes($db->result($db->query($scores_query."AND answer='q".$i."'"), 'result'));
		}

		return $survey_infos;
	}



	public function displayTargetSurvey($id_survey, $place = 'page') {
		global $core, $db;

		$survey = $this->gatherSurveyInfos($id_survey);

			//CASE OF AN EXISTING ANSWER OR OF A CLOSED SURVEY
			$result_count= $db->query("SELECT COUNT(*) AS votes_total FROM module_survey_answers WHERE id_survey='$id_survey'");
			$votes_total= $db->result($result_count, 'votes_total');

			// CASE OF AN ARCHIVED SURVEY
			if ($survey['archive'] == 'Y') {
				$result = $db->query("SELECT * FROM module_survey_archives WHERE id_survey='".$id_survey."'");	
				$archive = $db->fetchArray($result);
				$votes_total = 0;
				
				for ($i=1; $i<=10; $i++) {
					$survey['q'.$i]['result'] = $archive['q'.$i.'_results'];
					$votes_total += $survey['q'.$i]['result'];
				}
			}	
			if ($votes_total == 0) { $votes_total = 1; }

			// DISPLAY OF THE RESULTS
			$infos = '<div class="survey_title_block">'.$survey['title'].'<em> ('.$votes_total.' votes)</em></div>';
			for ($i=1; $i<=10; $i++) {
				if ($survey['q'.$i]['title'] != '') {
					$percentage = ($survey['q'.$i]['result']/$votes_total)*100;
					$bar_width = 300;
					$margin = round($bar_width*$percentage)/100 + 4;
					$infos .= '<div class="survey_answer_block">'
												.$survey['q'.$i]['title'].''
									 .'</div>'
									 .'<div class="results_block">'
									 		.'<div class="results" style="width: '.$percentage.'%;">&nbsp;</div><span style="margin: -15px 0px 0px '.$margin.'px">'.round($percentage).'%</span>'
									 .'</div>';
				}
			}
		return $infos;
	}	



	public function displayAddSurvey() {
		global $core,$db,$page;
	
			$select_content = $db->query("SELECT id, name FROM kernel_tag_category");

			echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_survey/admin/ajax/content.php').'" onsubmit="AJAX.submitAndUpdate(this,true,\'module_survey_admin_content\');return false">';
			
			echo '<input type="hidden" name="id_admin" value="'.$this->id.'" />'
					.'<input type="hidden" name="content" value="saveSurvey" />'
					.'<input type="hidden" name="cur_page" value="1" />'
				  .'<input type="hidden" name="save_case" value="add" />'
					.'<p class="form-input">'
						.'<label for="category">Type de sondage* :</label>'
						.'<select multiple name="category[]" size="3">';
							
			while ($row_select = $db->fetchArray($select_content)) {
				echo '<option value="'.$row_select['id'].'">'.stripslashes($row_select['name']).'</option>';
			}
				
			echo '</select></p>';

			echo '<p class="form-input">'
						 .'<label for="survey_title">Question : </label>'
					   .'<input id="survey_title" type="text" name="title" size="60" />'
					.'</p>'
					
						.'<p class="form-input">'
						 .'<label for="q1">Réponse n°1 : </label>'
					   .'<input id="q1" type="text" name="q1" size="60" />'
					.'</p>'
				 
					.'<p class="form-input">'
						 .'<label for="q2">Réponse n°2 : </label>'
					   .'<input id="q2" type="text" name="q2" size="60" />'
					.'</p>'

					.'<p class="form-input">'
						 .'<label for="q3">Réponse n°3 : </label>'
					   .'<input id="q3" type="text" name="q3" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						 .'<label for="q4">Réponse n°4 : </label>'
					   .'<input id="q4" type="text" name="q4" size="60" />'
					.'</p>'

					.'<p class="form-input">'
						 .'<label for="q5">Réponse n°5 : </label>'
					   .'<input id="q5" type="text" name="q5" size="60" />'
					.'</p>'
					
					.'<p class="form-input">'
						 .'<label for="q6">Réponse n°6 : </label>'
					   .'<input id="q6" type="text" name="q6" size="60" />'
					.'</p>'

					.'<p class="form-input">'
						 .'<label for="q7">Réponse n°7 : </label>'
					   .'<input id="q7" type="text" name="q7" size="60" />'
					.'</p>'

					.'<p class="form-input">'
						 .'<label for="q8">Réponse n°8 : </label>'
					   .'<input id="q8" type="text" name="q8" size="60" />'
					.'</p>'

					.'<p class="form-input">'
						 .'<label for="q9">Réponse n°9 : </label>'
					   .'<input id="q9" type="text" name="q9" size="60" />'
					.'</p>'
					
					.'<p class="form-input">'
						 .'<label for="q10">Réponse n°10 : </label>'
					   .'<input id="q10" type="text" name="q10" size="60" />'
					.'</p>'

					.'<p class="form-submit">'
						.'<input type="submit" value="Ajouter" />'
					.'</p>'
															      
			.'</form>';
																		
			echo '<p>* Si vous ne trouvez pas de catégorie adéquate, contactez le <a href="mailto:' . $core->getConfig('mailWebmaster') . '">webmaster</a> pour la rajouter.</p>';
						 
	}

  public function displayUpdateSurvey($id_survey, $cur_page) {

		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		$core->loadClass('date');
		$date = new Date();

		echo '<div id="update_survey_feedback" style="display:none" >';
		$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';

		$res = $db->query("SELECT title,q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,active FROM module_survey WHERE id='".$id_survey."'");
		$row = $db->fetchArray($res);

		$result = $db->query("SELECT
		                        categ.id,
		                        categ.name,
				                  IF (info.id IS NULL, 'false', 'true') as selected                                                                                                             FROM kernel_tag_category categ
				                  LEFT JOIN (kernel_tag_information info
				                  INNER JOIN kernel_tag_information_category info_categ) ON (categ.id = info_categ.idCategory
                          	AND info.tableOrig='module_survey'
                            AND info.idOrig='".$id_survey."'
                            AND info.id=info_categ.idInformation)
                         ");
												 
	echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_survey/admin/ajax/content.php').'" onsubmit="AJAX.submitAndUpdate(this,true,\'module_survey_admin_content\');display(gE(\'update_survey_feedback\'));return false">'
	
					.'<input type="hidden" name="id_admin" value="'.$this->id.'" />'
					.'<input type="hidden" name="content" value="saveSurvey" />'
					.'<input type="hidden" name="save_case" value="update" />'
					.'<input type="hidden" name="cur_page" value="'.$cur_page.'" />'
					.'<input type="hidden" name="id_survey" value="'.$id_survey.'" />'

					.'<p class="form-input">'
							.'<label>Type de ce sondage * :</label>'
							.'<select multiple name="category[]" size="3">';
							
							while ($row_select = $db->fetchArray($result)) {
								if($row_select['selected'] == 'true' ) {
									$sel = 'selected="selected"';
								} else {
										$sel ='';
									}
								echo '<option '.$sel.' value="'.$row_select['id'].'">'.stripslashes($row_select['name']).'</option>';
							}
	
							echo '</select>'
					.'</p>'

					.'<p class="form-input">'
						.'<label for="title">Titre de ce sondage : </label>'
						.'<input type="text" name="title" value="'.stripslashes($row['title']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q1">Réponse n°1 : </label>'
						.'<input type="text" name="q1" value="'.stripslashes($row['q1']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q2">Réponse n°2 : </label>'
						.'<input type="text" name="q2" value="'.stripslashes($row['q2']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q3">Réponse n°3 : </label>'
						.'<input type="text" name="q3" value="'.stripslashes($row['q3']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q4">Réponse n°4 : </label>'
						.'<input type="text" name="q4" value="'.stripslashes($row['q4']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q5">Réponse n°5 : </label>'
						.'<input type="text" name="q5" value="'.stripslashes($row['q5']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q6">Réponse n°6 : </label>'
						.'<input type="text" name="q6" value="'.stripslashes($row['q6']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q7">Réponse n°7 : </label>'
						.'<input type="text" name="q7" value="'.stripslashes($row['q7']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q8">Réponse n°8 : </label>'
						.'<input type="text" name="q8" value="'.stripslashes($row['q8']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q9">Réponse n°9 : </label>'
						.'<input type="text" name="q9" value="'.stripslashes($row['q9']).'" size="60" />'
					.'</p>'
					.'<p class="form-input">'
						.'<label for="q10">Réponse n°10 : </label>'
						.'<input type="text" name="q10" value="'.stripslashes($row['q10']).'" size="60" />'
					.'</p>'
					
					.'<p class="form-submit">'
						.'<input type="submit" value="Modifier" />'
					.'</p>'


			.'</form>';

	}
																																		


	public function closeSurvey($id_survey, $ended_status, $active_status) {
		global $db;
		
			if ($ended_status == 'Y') {
				$ended_status = 'N';
			}
			elseif ($ended_status == 'N' && $active_status == 'Y') {
				$ended_status = 'Y';
				$active_status = 'N';
			}
			elseif ($ended_status == 'N' && $active_status == 'N') {
				$ended_status = 'Y';
			}

			$db->query("UPDATE module_survey SET active = '".$active_status."', ended = '".$ended_status."' WHERE id = '".$id_survey."'");	
	}



	public function activateSurvey($id_survey, $ended_status, $active_status) {
		global $db;
		
			if ($ended_status == 'Y') {
				$ended_status = 'N';
				$active_status = 'Y';
			}
			elseif ($ended_status == 'N' && $active_status == 'Y') {
				$active_status = 'N';
			}
			elseif ($ended_status == 'N' && $active_status == 'N') {
				$active_status = 'Y';
			}

			$db->query("UPDATE module_survey SET active = '".$active_status."', ended = '".$ended_status."' WHERE id = '".$id_survey."'");	
	}



	public function saveSurvey($infos) {
		global $core, $db;
		
		$core->loadClass('date');
		$core->loadClass('file');
		$core->loadClass('tag',dirname(__FILE__).'/../include/engine/tag');
		$core->loadClass('tagInterface',dirname(__FILE__).'/../include/engine/tag');
		$date = new Date();
		$file = new file();
		$tag = new Tag(array('module_survey' => 'id'),$core,$db);

		$creation_date = date("Y-m-d");

		switch ($infos['save_case']) {
		
		// CASE OF AN INSERTION OF A BRAND NEW (AND SHINY) SURVEY
		case 'add' :
			$db->query("INSERT INTO module_survey (title,q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,date,archive) VALUES (
				'".$infos['title']."',
				'".$db->escapeString(htmlentities($infos['q1'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString(htmlentities($infos['q2'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString(htmlentities($infos['q3'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString(htmlentities($infos['q4'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString(htmlentities($infos['q5'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString(htmlentities($infos['q6'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString(htmlentities($infos['q7'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString(htmlentities($infos['q8'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString(htmlentities($infos['q9'], ENT_QUOTES, 'UTF-8'))."',
				'".$db->escapeString(htmlentities($infos['q10'], ENT_QUOTES, 'UTF-8'))."',
				'".$creation_date."',
				'N')
			");
		
			$id = $db->insertId();
			$idInformation = $tag->getIdInformation('module_survey',$id);
			$tag->setInformationCategory($idInformation, (isset($infos['category']) ? $infos['category'] : null));
		break;
		// END OF THIS CASE.

		// CASE OF AN UPDATE OF AN EXISTING (AN OLD) SURVEY
		case 'update' :
    	$id = $infos['id_survey'];
    	$idInformation = $tag->getIdInformation('module_survey', $id);
    	$tag->setInformationCategory($idInformation, (isset($infos['category']) ? $infos['category'] : null));						
		
			$db->query("UPDATE module_survey
				SET
					title  = '".$db->escapeString(htmlentities($infos['title'], ENT_QUOTES, 'UTF-8'))."',
					q1     = '".$db->escapeString(htmlentities($infos['q1'], ENT_QUOTES, 'UTF-8'))."',
					q2     = '".$db->escapeString(htmlentities($infos['q2'], ENT_QUOTES, 'UTF-8'))."',
					q3     = '".$db->escapeString(htmlentities($infos['q3'], ENT_QUOTES, 'UTF-8'))."',
					q4     = '".$db->escapeString(htmlentities($infos['q4'], ENT_QUOTES, 'UTF-8'))."',
					q5     = '".$db->escapeString(htmlentities($infos['q5'], ENT_QUOTES, 'UTF-8'))."',
					q6     = '".$db->escapeString(htmlentities($infos['q6'], ENT_QUOTES, 'UTF-8'))."',
					q7     = '".$db->escapeString(htmlentities($infos['q7'], ENT_QUOTES, 'UTF-8'))."',
					q8     = '".$db->escapeString(htmlentities($infos['q8'], ENT_QUOTES, 'UTF-8'))."',
					q9     = '".$db->escapeString(htmlentities($infos['q9'], ENT_QUOTES, 'UTF-8'))."',
					q10    = '".$db->escapeString(htmlentities($infos['q10'], ENT_QUOTES, 'UTF-8'))."',
					proposal = 'N'
				WHERE 
					id     = '".$infos['id_survey']."'
			");
		break;
		// END OF THIS CASE.

		default:
		break;
		}
	}						

	public function deleteSurvey($id_survey) {
		
		global $db;
		
		// QUERY DELETING THE SURVEY'S "SKELETON"
		$res = $db->query("DELETE FROM module_survey WHERE id='".$id_survey."'");

		// QUERY DELETING THE SURVEY'S ANSWERS
		$res = $db->query("DELETE FROM module_survey_answers WHERE id='".$id_survey."'");

		// QUERY DELETING THE SURVEY'S ARCHIVE
		$res = $db->query("DELETE FROM module_survey_archives WHERE id='".$id_survey."'");

	}


	public function archiveSurvey($id_survey) {
		
		global $core, $db;
		
		$a1  = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q1'"), 0);
		$a2  = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q2'"), 0);
		$a3  = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q3'"), 0);
		$a4  = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q4'"), 0);
		$a5  = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q5'"), 0);
		$a6  = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q6'"), 0);
		$a7  = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q7'"), 0);
		$a8  = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q8'"), 0);
		$a9  = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q9'"), 0);
		$a10 = $db->result($db->query("SELECT COUNT(*) FROM module_survey_answers WHERE id_survey='".$id_survey."' AND answer='q10'"), 0);
		
		// QUERY STORING THE RESULTS IN THE ARCHIVES TABLE
		$db->query ("INSERT INTO module_survey_archives 
			(id_survey,q1_results,q2_results,q3_results,q4_results,q5_results,q6_results,q7_results,q8_results,q9_results,q10_results)
			VALUES('".$id_survey."','".$a1."','".$a2."','".$a3."','".$a4."','".$a5."','".$a6."','".$a7."','".$a8."','".$a9."','".$a10."')
		");
			
		// QUERY DELETING THE RESULTS IN THE ANSWERS TABLE
		$db->query ("DELETE FROM module_survey_answers WHERE id_survey='".$id_survey."'");
		
		// QUERY UPDATING THE SURVEYS TABLE TO SET THE TARGET SURVEY TO ARCHIVE
		$db->query ("UPDATE module_survey SET archive='Y', active='N', ended='Y' WHERE id='".$id_survey."'");
																											
	}


}
// End of this file.

?>
