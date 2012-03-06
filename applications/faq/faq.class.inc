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


class Faq{
	
	private $id;
	
	public final function __construct(){
		$this->id = rand(1000000,9999999);
		$_SESSION['faq_'.$this->id] = serialize($this);
	}

	public function display(){
		$this->displayAllQuestions();
	}

	private function displayAllQuestions(){
		global $db;

		$result = $db->query("SELECT id, question, answer FROM faq_content WHERE active='Y' ORDER BY question ASC");

		while($row = $db->fetchArray($result)){
			$this->displayQuestion($row['id'],stripslashes($row['question']),stripslashes($row['answer']));
		}
	}

	private function displayQuestion($id, $question, $answer){
		global $core;
		
		$question_first_letter = substr($question,0,1);
		$question_end = substr($question,1,300);

		echo '
			<div class="faq_element">
				<div class="faq_question"><a '.
					'onclick="if(window.getComputedStyle(gE(\'faq_answer_'.$id.'\'), \'\').display != \'none\') {'.
							'hide(gE(\'faq_answer_'.$id.'\'));'.
							'hide(gE(\'faq_comment_'.$id.'\'));'.
						'}else{'.
							'display(gE(\'faq_answer_'.$id.'\'));'.
					'}"><span>'.$question_first_letter.'</span>'.$question_end.'</a></div>
				<div class="faq_answer" id="faq_answer_'.$id.'" style="display: none;" >'.
				$answer;
		/*echo '<div><a class="link_button left" onclick="display(gE(\'faq_comment_'.$id.'\'));display(gE(\'show\'));display(gE(\'hide\'))"><span id="show">Laisser un commentaire</span><span id="hide" style="display: none;">Masquer l\'ajout de commentaire</span></a></div>
				<div class="clear"></div>*/
		echo '</div>';
		/*echo '<div class="faq_comment_form" id="faq_comment_'.$id.'" style="display: none;" >
					<form action="ajax/faq.php"
						onsubmit="AJAX.submitAndCall(this,false,function(){display(gE(\'faq_comment_'.$id.'\'));});return false" >
						<div>
							<input type="hidden" name="faq_comment_login" value="'.$core->getUserId().'" />
							<input type="hidden" name="wope_action" value="save_comment" />
							<input type="hidden" name="id_faq" value="'.$this->id.'" />
						</div>
						<p class="form-textarea">
							<label></label>
							<textarea name="faq_comment" cols="60" rows="6" ></textarea>
						</p>
						<p class="form-submit">
							<input type="submit" value="Envoyer" />
						</p>
					</form>
				</div>';*/
		echo '</div>
		';
	}

	public function saveComment($infos){
		global $db;
	}
}
?>
