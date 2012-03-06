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


Class Module_survey extends Module {

	public final function __construct($informations = Array() ) {
		global $core;

		parent::__construct($informations);
		
    $this->path = 'modules/module_survey';
    include($core->getConfig('baseServer').'/'.$this->path.'/config_module_survey.inc');
    $this->config = array_merge($config_module_survey);
		do {
			$this->id = rand(1000000, 9999999);
		} 
		while( isset($_SESSION['module_survey_'.$this->id]) );
	    $_SESSION['module_survey_'.$this->id] = serialize($this);
	}



	public function display() {
		global $core;

		$core->loadClass('Page');
		$page = new Page();

		$this->displayCurrentSurveys();
		echo '<div class="clear sep"></div>'
				.'<a class="see_all_surveys link_button" href="'.$core->makeUrl("/modules/module_survey/index.php").'?display=all">Afficher tous les sondages...</a>';
		$page->adminLink('module','module_survey','admin');
	}



	//Function to display some active surveys.
	//The $type argument is used to specify the type of the display : for the module or for the full php page
	public function displayCurrentSurveys() {
	
		$surveys_id = $this->getCurrentSurveys();

		for ($i = 0; $i <= $surveys_id['nb']; $i++) {
			$survey = $this->gatherSurveyInfos($surveys_id[$i]);
			echo $this->displayTargetSurvey($surveys_id[$i], 'module');
		}
	
	}



	public function displayTargetSurvey($id_survey, $place = 'page') {
		global $core, $db;

		$survey = $this->gatherSurveyInfos($id_survey);

		//WE CHECK IF THE USER HAS ALREADY ANSWERED OR NOT
		if ($db->numRows($db->query("SELECT id FROM module_survey_answers WHERE id_survey = '".$id_survey."' AND login = '".$core->getUserId()."'")) > 0 || $survey['ended'] == 'Y' || $survey['archive'] == 'Y') {
		
			//CASE OF AN EXISTING ANSWER OR OF A CLOSED SURVEY : HE CAN SEE THE RESULTS BUT CANNOT ANSWER AGAIN
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

					//THE WIDTH OF THE RESULT BAR DEPENDS ON THE PLACE IT'S DISPLAYED
					if ($place == 'page') {
						$bar_width = 300;
					} elseif ($place == 'module') {
							$bar_width = 190;
						}
					$margin = round($bar_width*$percentage)/100 + 4;
					$infos .= '<div class="survey_answer_block">'
												.$survey['q'.$i]['title'].''
									 .'</div>'
									 .'<div class="results_block">'
									 		.'<div class="results" style="width: '.$percentage.'%;">&nbsp;</div><span style="margin: -15px 0px 0px '.$margin.'px">'.round($percentage).'%</span>'
									 .'</div>';
				}
			}

		} else {
		
				//CASE OF NO EXISTING ANSWER : HE CANNOT SEE THE RESULTS BUT CAN ANSWER
				$s_id = rand(1000000,9999999);
				$infos = '<div id="answers_block_'.$s_id.'"><div class="survey_title_block">'.$survey['title'].'</div><div class="answers_block">';
				for ($j = 1; $j <= 10; $j++) {
					if ($survey['q'.$j]['title'] != '') {
						$infos .= '<div class="survey_answer_block">'
												.'<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_survey/ajax/content.php').'?id_survey='.$this->id.''
															.'&target_survey='.$id_survey.''
															.'&answer=q'.$j.''
															.'&wope_action=submitVote\','
															.'\'answers_block_'.$s_id.'\')'
												.'">'.$survey['q'.$j]['title'].'</a>'
										 .'</div>';
					}
				}
				$infos .= '</div></div>';
			}
		return $infos;
	}



	public function getCurrentSurveys($id_type= 'all') {
		global $db;

		$surveys_query = $db->query("SELECT id FROM module_survey WHERE active = 'Y'");
		$current_surveys = Array();
		
		$i = 0;
		while ($result = $db->fetchArray($surveys_query)) {
			$current_surveys[$i] = $result['id']; 
			$i++;
		}

		$current_surveys['nb'] = $i - 1;
		return $current_surveys;
	}


	
	public function getClosedSurveys() {
		global $db;

		$surveys_query = $db->query("SELECT id FROM module_survey WHERE ended = 'Y'");
		$closed_surveys = Array();
		
		$i = 0;
		while ($result = $db->fetchArray($surveys_query)) {
			$closed_surveys[$i] = $result['id']; 
			$i++;
		}
		
		return $closed_surveys;
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

  public function displayTypes() {
		global $db, $page, $core;
		
			if (!isset ($page))
			$page = new Page();
			echo '<div class="links_classes">';
			$result = $db->query("SELECT idCategory as id,name,image
														FROM kernel_tag_category_user categ
														WHERE categ.login='".(int)$core->getUserId()."'
														AND categ.isShow='true'
													");
													
			echo '<ul class="panel">'
							.'<li class="link_menu">'
									.'<a href="index.php?display=all" >Tous types</a>'
							.'</li>';
			while ($row = $db->fetchArray($result)) {
				echo '<li class="link_menu"><a href="' . $_SERVER['PHP_SELF'] . '?display=' . $row['id'] . '" title="' . stripslashes($row['name']) . '">';
				echo ' ' . stripslashes($row['name']) . '</a></li>';
			}
			
			echo '</ul></div>';
	}



	//Function to display a list of all the surveys, by category.
	public function displaySurveysByType($id_type = 'all', $cur_page = 1) {
		global $core, $db;
		
		$core->loadClass('Date');
		$date = new Date();
	  $core->loadClass('box');
    $box = new Box();
		$core->loadClass('Page');
		$page = new Page();
				
		if (!empty ($id_type) && $id_type != 'all') {
			$id_type=(int)$id_type;
			$sqlType = " AND categ.idCategory = $id_type ";
		} else {
			$sqlType = '';
		}

		//WE GATHER ALL THE SURVEYS WHICH HAS ENDED OR ARE CURRENTLY USED
		$start_row = ($cur_page - 1) * $this->config['items_per_page'];
		$result = "SELECT 
												categ.name AS type,
												module_survey.id,
												module_survey.title, 
												module_survey.date,
												module_survey.active
																	
									FROM kernel_tag_category_user categ, 
											 kernel_tag_information info,
											 kernel_tag_information_category info_categ,
											 module_survey
											 
									WHERE (module_survey.active = 'Y'
										OR module_survey.ended = 'Y')
										AND info.idOrig = module_survey.id
										AND info.tableOrig = 'module_survey'
										AND info_categ.idInformation = info.id
										AND info_categ.idCategory = categ.idCategory
										AND categ.isShow = 'true'
										AND categ.login = '".(int)$core->getUserId()."'
										".$sqlType."

										
									GROUP BY module_survey.id
									ORDER BY module_survey.active ASC, module_survey.date DESC";

				$surveys_taken = $db->query($result." LIMIT ".$start_row.",".$this->config['items_per_page']);
				if ($db->numRows($surveys_taken) > 0) {
					$current_surveys = '<div class="current_surveys">';
					$ended_surveys = '<div class="ended_surveys">';
					$i = 0;
					$j = 0;
					while ($surveys_to_display = $db->fetchArray($surveys_taken)) {
						if ($i%2) {
        			$sec_class = 'a1';
				    } else { $sec_class = 'b1'; }
						if ($surveys_to_display['active'] == 'Y') {
							$current_surveys .= $this->displayTargetSurvey($surveys_to_display['id']);
							$j++;
						} else {
								$dialbox_content = $this->displayTargetSurvey($surveys_to_display['id']);
    	          if (strlen(stripslashes($surveys_to_display['title'])) > 82) {
				          $survey_title = substr(stripslashes($surveys_to_display['title']),0,80).'...';
				        } else {
					          $survey_title = stripslashes($surveys_to_display['title']);
					        }																																		
								$ended_surveys .= '<div class="ended_survey_block '.$sec_class.'">'
																		.'<div class="ended_survey_title">'
																				.'<a onclick="'.$box->info($dialbox_content).';" onmouseover="activateToolTips(this,\''.$page->htmlBlock->escapeTipContent(stripslashes($surveys_to_display['title'])).'\');">'.$survey_title.'</a>'
																		.'</div>'
																		.'<div class="ended_survey_date">lancé le '.$date->reverseDate($surveys_to_display['date']).'</div>'
																		.'<div class="clear"></div>'
															 	 .'</div>';
								$i++;
							}
					}
					if ($j > 0) {
						$current_surveys .= '</div>';
					} else {
						$current_surveys .= '<p>Aucun sondage en cours.</p></div>';
						}
					if ($i > 0) {
						$ended_surveys .= '</div>';
					} else {
						$ended_surveys .= '<p>Aucun sondage clos dans cette catégorie.</p></div>';
						}

					//THE RESULTS ARE DISPLAYED
					echo '<p>'
									.'<span class="titre_menu">'
											.'<span>S</span>ondages en cours :'
									.'</span>'
							.'</p>'
							.'<em>(Il est encore temps de répondre si ce n\'est pas déjà fait)</em>';
					echo $current_surveys;
					echo '<p>'
									.'<span class="titre_menu">'
											.'<span>S</span>ondages terminés :'
									.'</span>'
							.'</p>'
							.'<em>(Il ne vous est plus possible d\'y répondre mais leurs résultats sont consultables)</em>';
					echo $ended_surveys;

				} else {
						echo '<div class="clear"></div>';
						echo '<p>Aucun sondage dans cette catégorie</p>';
					}

				$page->htmlBlock->pagesList($result, $this->config['items_per_page'], $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/content.php').'?id_survey='.$this->id.'&amp;wope_action=displaySurveys&amp;cur_page=[#]&amp;type='.$id_type.'\',\'SurveysList\')');
	}



	//Function to submit a vote.
	public function submitVote($survey_id, $answer) {
		global $core, $db;
		
		$db->query("INSERT INTO module_survey_answers (answer,id_survey,login) VALUES (
									'".$answer."',
									'".$survey_id."',
									'".$core->getUserId()."')
							");
	}



	//Function to propose a survey.
	public function proposeSurvey() {
		global $core,$db,$page;
	
			$select_content = $db->query("SELECT id, name FROM kernel_tag_category");

			echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_survey/ajax/content.php').'" onsubmit="AJAX.submitAndUpdate(this,true,\'SurveysList\');return false">';
			
			echo '<input type="hidden" name="id_survey" value="'.$this->id.'" />'
					.'<input type="hidden" name="wope_action" value="saveSurvey" />'
					
					.'<p class="form-input">'
						.'<label for="category">Type de sondage* :</label>'
						.'<select multiple name="category[]" size="3">';
							
			while ($row_select = $db->fetchArray($select_content)) {
				echo '<option value="'.$row_select['id'].'">'.stripslashes($row_select['name']).'</option>';
			}
				
			echo '</select></p>';

			echo '<p class="form-input">'
						 .'<label for="survey_title">Question de votre sondage : </label>'
					   .'<input id="survey_title" type="text" name="title" size="80" />'
					.'</p>'

					.'<p class="form-submit">'
						.'<input type="submit" value="Proposer" />'
					.'</p>'


			.'</form>';

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

			$db->query("INSERT INTO module_survey (title,date, active, archive, proposal) VALUES (
				'".$infos['title']."',
				'".$creation_date."',
				'N',
				'N',
				'Y')
			");
		
			$id = $db->insertId();
			$idInformation = $tag->getIdInformation('module_survey',$id);
			$tag->setInformationCategory($idInformation, (isset($infos['category']) ? $infos['category'] : null));
	}



}
// End of Survey Class.

?>
