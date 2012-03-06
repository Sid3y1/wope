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


class Faq_Admin {
	
	private $id;
	
	public final function __construct(){
		$this->id = rand(1000000,9999999);
		$_SESSION['faq_admin_'.$this->id] = serialize($this);
	}

	public function displayContent(){
		global $page;
		if(!isset($page))$page = new Page();

		$page->moduleHeader('FAQ','');
		echo '<div id="faq_admin_content">';
		$this->displayQuestions();
		echo '</div>';
		$page->moduleFooter();
	}
	
	public function displayMenu(){
		global $page,$core;
		if(!isset($page))$page = new Page();

		$page->moduleHeader('FAQ','');
		echo '<ul class="panel">
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/faq/admin/ajax/faq_admin.php').'?id_admin='.$this->id.'&wope_action=display_questions\',\'faq_admin_content\')">Liste des questions</a></li>
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/faq/admin/ajax/faq_admin.php').'?id_admin='.$this->id.'&wope_action=add_question\',\'faq_admin_content\')">Ajouter une question</a></li>
		</ul>';
		$page->moduleFooter();
	}

	public function displayQuestions(){
		global $db, $core,$page;
		if(!isset($page))$page = new Page();
		$box = new Box();
		$result = $db->query("SELECT id, question, answer, active FROM faq_content WHERE 1 ORDER BY question ASC");

		echo '<table class="data_table" width="95%">
			<tr>
				<th>Question</th>
				<th>Actif</th>
				<th></th>
			</tr>';

		while($row = $db->fetchArray($result)){
			$active_class = ($row['active']=='N')?' class="no_active" ':'';
			$tooltip_content = $page->htmlBlock->escapeTipContent('<h4>Réponse :</h4><p>'.$row['answer'].'</p>');
			
			echo '<tr '.$active_class.' >
				<td><a '.
					'onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/faq/admin/ajax/faq_admin.php').'?id_admin='.$this->id.'&wope_action=update_question&question='.$row['id'].'\',\'faq_admin_content\');hide(toolTips)" '.
					'onmouseover="activateToolTips(this,\''.$tooltip_content.'\')" '.
					'>'.stripslashes($row['question']).'</a></td>
				<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/faq/admin/ajax/faq_admin.php').'?id_admin='.$this->id.'&wope_action=active_question&question='.$row['id'].'&active='.(($row['active']=='Y')?'N':'Y').'\',\'faq_admin_content\')">'.$row['active'].'</a></td>
				<td align="center"><a onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('applications/faq/admin/ajax/faq_admin.php').'?id_admin='.$this->id.'&wope_action=delete_question&question='.$row['id'].'\',\'faq_admin_content\')','Etes-vous sûr de vouloir supprimer cette question ?').'">Supprimer</a></td>
			</th>';
		}
		if($db->numRows($result)==0){
			echo '<tr align="center" colspan="3">Aucune question dans la FAQ</tr>';
		}
		echo '</table>';
	}

	public function deleteQuestion($id_question){
		global $db;
		$db->query("DELETE FROM faq_content WHERE id='".$id_question."'");
	}

	public function activeQuestion($id_question, $active){
		global $db;
		$db->query("UPDATE faq_content SET active='".$active."' WHERE id='".$id_question."'");
	}

	public function displayAddQuestion(){
		global $core;
		$core->loadClass('magicform');
		$date = date('Y-m-d');

		$mf_options = Array(
			'update'=>false,
			'only'=>Array('question','answer','active','date'),
			'legend'=>Array('question'=>'Question','answer'=>'Réponse','active'=>'Activer la question'),
			'button'=>'Ajouter',
			'name'=>'<h4>Nouvelle question</h4>',
			'message'=>'Envoi en cours...',
			'feedback'=>'info',
			'once'=>true,
			'onsubmit'=>'AJAX.getAndUpdate(\''.$core->makeUrl('applications/faq/admin/ajax/faq_admin.php').'?id_admin='.$this->id.'&wope_action=display_questions\',\'faq_admin_content\');',
			'forceType'=>Array(
				'active'=>Array('type'=>'radio','options'=>Array('Y'=>'Oui','N'=>'Non'),'default'=>'Y'),
				'date'=>Array('type'=>'hidden','value'=>$date)
			),
			'describe' => Array(
				'id' =>  Array("type"=>"int(10) unsigned","vide"=>"NO","key"=>"1","default"=>"","extra"=>"auto_increment"),
				'question' =>  Array("type"=>"varchar(255)","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'answer' =>  Array("type"=>"text","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'date' =>  Array("type"=>"date","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'active' =>  Array("type"=>"enum('Y','N')","vide"=>"NO","key"=>"","default"=>"Y","extra"=>"")
			),
			'analyse'=>false
		);
		
		$mf = new MagicForm('faq_content',$mf_options);
		$mf->display();
	}

	public function displayUpdateQuestion($id_question){
		global $core;
		$core->loadClass('magicform');
		$date = date('Y-m-d');

		$mf_options = Array(
			'update'=>true,
			'rowToUpdate'=>Array('field'=>'id','pattern'=>$id_question),
			'only'=>Array('question','answer','active','date'),
			'legend'=>Array('question'=>'Question','answer'=>'Réponse','active'=>'Activer la question'),
			'button'=>'Modifier',
			'name'=>'<h4>Modification de la question</h4>',
			'message'=>'Envoi en cours...',
			'feedback'=>'info',
			'once'=>true,
			'onsubmit'=>'AJAX.getAndUpdate(\''.$core->makeUrl('applications/faq/admin/ajax/faq_admin.php').'?id_admin='.$this->id.'&wope_action=display_questions\',\'faq_admin_content\');',
			'forceType'=>Array(
				'active'=>Array('type'=>'radio','options'=>Array('Y'=>'Oui','N'=>'Non'),'default'=>'Y'),
				'date'=>Array('type'=>'hidden','value'=>$date)
			),
			'describe' => Array(
				'id' =>  Array("type"=>"int(10) unsigned","vide"=>"NO","key"=>"1","default"=>"","extra"=>"auto_increment"),
				'question' =>  Array("type"=>"varchar(255)","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'answer' =>  Array("type"=>"text","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'date' =>  Array("type"=>"date","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'active' =>  Array("type"=>"enum('Y','N')","vide"=>"NO","key"=>"","default"=>"Y","extra"=>"")
			),
			'analyse'=>false
		);
		
		$mf = new MagicForm('faq_content',$mf_options);
		$mf->display();
	}
}
?>
