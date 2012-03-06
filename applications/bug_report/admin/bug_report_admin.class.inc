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


class Bug_Report_Admin {
	
	private $id;
	
	public final function __construct(){
		$this->id = rand(1000000,9999999);
		$_SESSION['bug_report_admin_'.$this->id] = serialize($this);
	}

	public function displayContent(){
		global $page;
		if(!isset($page))$page = new Page();

		$page->moduleHeader('Report de bugs','');
		echo '<div id="bug_report_admin_content">';
		
		echo '</div>';
		$page->moduleFooter();
	}
	
	public function displayMenu(){
		global $pag,$core;
		if(!isset($page))$page = new Page();

		$page->moduleHeader('Menu du report de bugs','');
		echo '<ul class="panel">
		
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=display_severity_list\',\'bug_report_admin_content\')">Degrés de gravité disponibles</a></li>
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=add_severity_level\',\'bug_report_admin_content\')">Ajouter un degré de gravité</a></li>
			
			</ul>


		<ul class="panel" style="margin-top: 8px">
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=display_bug_types_list\',\'bug_report_admin_content\')">Types de bugs</a></li>
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=add_bug_type\',\'bug_report_admin_content\')">Ajouter un type de bug</a></li>

		</ul>
		
		<ul class="panel" style="margin-top: 8px">
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=display_message\',\'bug_report_admin_content\')">Modifier l\'adresse de destination des rapports</a></li>
		</ul>';
		$page->moduleFooter();
	}


	/**** DEBUT DES FONCTIONS D'AFFICHAGE ****/


	public function displaySeverityLevels(){
		global $db, $core,$page;
		if(!isset($page))$page = new Page();
		$box = new Box();
		$result = $db->query("SELECT severity FROM bug_report_severity ORDER BY severity ASC");

		echo '<table class="data_table" width="95%">
			<tr>
				<th width="80%" align="center">Degré de sévérité</th>
				<th width="10%" align="center">Suppression</th>
			</tr>';

		while($row = $db->fetchArray($result)){
			
			echo '<tr>
				<td><a '.
					'onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=update_severity_level&severity_name='.stripslashes($row['severity']).'\',\'bug_report_admin_content\')">'.stripslashes($row['severity']).'</a></td>
				<td align="center"><a class="trash" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=delete_severity_level&severity_name='.stripslashes($row['severity']).'\',\'bug_report_admin_content\')','Etes-vous sûr de vouloir supprimer ce degré de sévérité ?').'"></a></td>
			</th>';
		}
		if($db->numRows($result)==0){
			echo '<tr align="center" colspan="3">Aucun type de gravité de bug dans la liste</tr>';
		}
		echo '</table>';
	}


	public function displayBugTypes(){
		global $db, $core,$page;
		if(!isset($page))$page = new Page();
		$box = new Box();
		$result = $db->query("SELECT type FROM bug_report_type ORDER BY type ASC");

		echo '<table class="data_table" width="95%">
			<tr>
				<th width="80%">Type de bug</th>
				<th width="10%" align="center">Suppression</th>
			</tr>';

		while($row = $db->fetchArray($result)){
			
			echo '<tr>
				<td><a '.
					'onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=update_bug_type&type_name='.stripslashes($row['type']).'\',\'bug_report_admin_content\')">'.stripslashes($row['type']).'</a></td>
				<td align="center"><a class="trash" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=delete_bug_type&type_name='.stripslashes($row['type']).'\',\'bug_report_admin_content\')','Etes-vous sûr de vouloir supprimer ce type de bug ?').'"></a></td>
			</th>';
		}
		if($db->numRows($result)==0){
			echo '<tr align="center" colspan="3">Aucun type de bug dans la liste</tr>';
		}
		echo '</table>';
	}



	/**** FIN DES FONCTIONS D'AFFICHAGE ****/



	/**** DEBUT DES FONCTIONS DE SUPPRESSION ****/


	public function deleteSeverityLevel($severity_name){
		global $db;
		$db->query("DELETE FROM bug_report_severity WHERE severity='".$severity_name."'");
	}

	public function deleteBugType($bug_type_name){
		global $db;
		$db->query("DELETE FROM bug_report_type WHERE type='".$bug_type_name."'");
	}


	/**** FIN DES FONCTIONS DE SUPPRESSION ****/



	/**** DEBUT DES FONCTIONS D'AJOUT ****/


	public function displayAddSeverityLevel() {
		global $core;
		$core->loadClass('magicform');

		$bug_options = Array(
			'update'=>false,
			'legend'=>Array('severity'=>'Gravité du bug : '),
			'button'=>'Ajouter',
			'name'=>'<h4>Nouveau degré de gravité</h4>',
			'message'=>'Envoi en cours...',
			'feedback'=>'info',
			'once'=>true,
			'onsubmit'=>'AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=display_severity_list\',\'bug_report_admin_content\');',
			'describe' => Array(
				'severity' =>  Array("severity"=>"varchar(50)"),
			),
			'analyse'=>false
		);
		
		$bug_o = new MagicForm('bug_report_severity',$bug_options);
		$bug_o->display();
	}

	public function displayAddBugType() {
		global $core;
		$core->loadClass('magicform');

		$bug_options = Array(
			'update'=>false,
			'legend'=>Array('type'=>'Type du bug : '),
			'button'=>'Ajouter',
			'name'=>'<h4>Nouveau type de bug</h4>',
			'message'=>'Envoi en cours...',
			'feedback'=>'info',
			'once'=>true,
			'onsubmit'=>'AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=display_bug_types_list\',\'bug_report_admin_content\');',
			'describe' => Array(
				'type' =>  Array("type"=>"varchar(50)"),
			),
			'analyse'=>false
		);
		
		$bug_o = new MagicForm('bug_report_type',$bug_options);
		$bug_o->display();
	}


	/**** FIN DES FONCTIONS D'AJOUT ****/



	/**** DEBUT DES FONCTIONS DE MODIFICATION ****/


	public function displayUpdateSeverityLevel($name) {
		global $core;
		$core->loadClass('magicform');

		$bug_edit = Array(
			'update'=>true,
			'rowToUpdate'=>Array("field"=>"severity", "pattern"=>"".$name.""),
			'legend'=>Array('severity'=>'Nouvelle gravité : '),
			'button'=>'Modifier',
			'name'=>'<h4>Modification du degré de gravité : </h4>',
			'message'=>'Envoi en cours...',
			'feedback'=>'info',
			'once'=>true,
			'onsubmit'=>'AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=display_severity_list\',\'bug_report_admin_content\');',
			'describe' => Array(
				'severity' =>  Array("type"=>"varchar(50)"),
			),
			'analyse'=>false
		);
		
		$bug_ed = new MagicForm('bug_report_severity',$bug_edit);
		$bug_ed->display();
	}

	public function displayUpdateBugType($name) {
		global $core;
		$core->loadClass('magicform');

		$bug_edit = Array(
			'update'=>true,
			'rowToUpdate'=>Array("field"=>"type", "pattern"=>"".$name.""),
			'legend'=>Array('type'=>'Nouveau type du bug : '),
			'button'=>'Modifier',
			'name'=>'<h4>Modification du type de bug : </h4>',
			'message'=>'Envoi en cours...',
			'feedback'=>'info',
			'once'=>true,
			'onsubmit'=>'AJAX.getAndUpdate(\''.$core->makeUrl('applications/bug_report/admin/ajax/bug_report_admin.php').'?id_admin='.$this->id.'&wope_action=display_bug_types_list\',\'bug_report_admin_content\');',
			'describe' => Array(
				'type' =>  Array("type"=>"varchar(50)"),
			),
			'analyse'=>false
		);
		
		$bug_ed = new MagicForm('bug_report_type',$bug_edit);
		$bug_ed->display();
	}

	/**** FIN DES FONCTIONS DE MODIFICATION ****/


	
}

?>
