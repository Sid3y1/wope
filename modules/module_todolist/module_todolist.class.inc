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


class Module_todolist extends Module {

	private $id;
	
	public final function __construct ( $informations = Array() ) {
		parent::__construct($informations);
		
		do{
			$this->id = rand(1000000, 9999999);
		}while( isset($_SESSION['module_todolist_'.$this->id]) );
		
		$_SESSION['module_todolist_'.$this->id] = serialize($this);
	}


	/* Function cutTitle($title,$max_length)
	 *	-> Allow to cut a title to a smaller title with "..."
	 * 
	 * @param  : $title = title that you want to cut
	 * @param  : $max_length = max length of the text that you want to be displayed
	 * @return : $title (the cuted title)
	 *
	 ********/
  private function cutTitle ($title,$max_length) {
	  return htmlentities(preg_replace("/(.{".($max_length-3)."}).+/","\\1...",html_entity_decode($title, ENT_QUOTES, 'UTF-8')), ENT_QUOTES, 'UTF-8');
  }


	/* Function display()
	 *	-> Display the content of the module
	 * 
	 * @param  : void
	 * @return : void
	 *
	 ********/
  public function display(){
  	global $core,$db;
   
 		echo '<div class="title_block">
						<span class="titre_menu"><span>M</span>es prochaines tâches :</span>
					</div>
	
					<div id="todo_block" class="left">';
						// Everytime an action is done, this div is refreshed to display the changes
						$this->displayTasks();
		echo '</div>';
		
		// The "calContainer" div contains the YAHOO calendar, which is displayed when the user clicks on the "create_cal_buttons" links
		echo '<span id="calContainer"></span>';

		echo '<div class="clear" style="height: 20px;"></div>';

  }



	/* Function displayTasks()
	 *	-> Display the content of the "todo_block", which contains the list of the tasks to do
	 * 
	 * @param  : void
	 * @return : void
	 *
	 ********/
	private function displayTasks() {
		global $db, $core;

		$core->loadClass('date');
		$date = new Date();
		$box = new Box();
		
		// Selection of the user's tasks only
		$res = $db->query("SELECT id, title, expiry, done FROM module_todolist WHERE login = '".$core->getUserId()."' ORDER BY expiry");
						
		// With this loop, the table of the user's tasks is read and his tasks are displayed
		$i = 1;
		$done_class = '';
		while ($row = $db->fetchArray($res)) {
						if ($i%2) {
							$line_style = '_a';
						} else { 
								$line_style = '_b';
							}
			echo '<div class="left task task'.$line_style.'">
							<input type="checkbox" value="'.( ($row['done'] == 'Y') ? 'N' : 'Y').'" '; 
							if ($row['done'] == 'Y') {
								echo ' checked="ckecked" ';
								$done_class = 'done';
							}else{
								$done_class = 'not_done';
							}
							
							echo '" onclick="'.
								'AJAX.getAndCall(\''.$core->makeUrl('modules/module_todolist/ajax/ajax_todolist.php').'?id_todo='.$this->id.'&amp;wope_action=valid_task&amp;id_task='.$row['id'].'&amp;done=\' + ( (this.checked) ? \'Y\' : \'N\' ) , function(){'.
									'switchClassName(gE(\'title_'.$i.'\'), \'done\', \'not_done\');'.
								'} )" />';
		
							if (date('Y-m-d') >= $row['expiry']) {
								$warning = 'be_careful';
							} else {
									$warning = 'ok';
								}  

			echo '	<div class="task_title_block">
								<span id="title_'.$i.'" class="'.$done_class.' '.$warning.'" onclick="editAndSaveTarget(gE(\'source_'.$i.'\'),\'text\',this,\''.$core->makeUrl('modules/module_todolist/ajax/ajax_todolist.php').'?id_todo='.$this->id.'&amp;wope_action=update_title&amp;id_task='.$row['id'].'\',\'update_title\',1);">'.$this->cutTitle(stripslashes($row['title']),50).'</span>
								<span id="source_'.$i.'" style="display: none;">'.stripslashes($row['title']).'</span>
							</div>							

							<div class="task_expiry_block">
								<span id="expiry_'.$row['id'].'"'.
									'onclick="editAndSaveTarget(gE(\'expiry_'.$row['id'].'\'), \'date\', this, '.
										'\''.$core->makeUrl('modules/module_todolist/ajax/ajax_todolist.php').'?id_todo='.$this->id.'&amp;wope_action=update_date&amp;id_task='.$row['id'].'\', '.
										'\'update_date\', '.
										'1, '.
										'function(){gE(\'todo_block\').innerHTML = this.responseText;} '.
									');">'.$date->reverseDate($row['expiry']).'</span>
							</div>
							
							<a class="calendar_button" onclick="selectDate(\'calContainer\', '.
								'function(date){'.
									' gE(\'expiry_'.$row['id'].'\').innerHTML = date; '.
									' AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_todolist/ajax/ajax_todolist.php').'?id_todo='.$this->id.'&amp;wope_action=update_date&amp;id_task='.$row['id'].'&amp;update_date=\'+date, \'todo_block\'); '.
								'}'.
							', true, \'FR\');">&nbsp;</a>';

			echo '	<a class="del_link left" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_todolist/ajax/ajax_todolist.php').'?id_todo='.$this->id.'&amp;wope_action=del_task&id_task='.$row['id'].'\',\'todo_block\')','Êtes-vous sûr de vouloir supprimer cette tâche ?').'"></a> 

						</div><div class="clear"></div>';
						$i++;

		} // End of the loop which displays the tasks

		
		// Next div (id : "ADDBLOCK") content allow user to add some new tasks
		
		// Title of the adding menu
		echo '<div class="add_block">
						<a onclick="display(gE(\'td_masquer\')); display(gE(\'td_ajouter\')); display(gE(\'add_block\'));">
							<span id="td_ajouter">Ajouter ...</span>
							<span id="td_masquer" style="display: none;">Masquer ...</span>
						</a>
					</div>';

		// The values of the checkbox and the input content are read with some javascript code and transmitted to the AJAX file to add the entry in the Base
		echo '<div id="add_block" class="task" style="display: none">
		
						<input id="new_task_status" type="checkbox" value="N" onclick="'.
							'if (this.value == \'Y\') {'.
								'this.value = \'N\''.
							'} else { '.
								'this.value = \'Y\'; '.
							'}" />
							
						<input type="text" id="new_task_title" class="'.$done_class.'"'. 
									 'onfocus="if (this.value==\'Titre de la tâche\') {'.
									 						'this.value = \'\';'.
														'}"'.
														'name="new_task_title" value="Titre de la tâche" />
						<input type="text" id="new_task_expiry" class="new_task_date"'.
										'onfocus="if (this.value==\'jj/mm/aaaa\') {'.
																'this.value = \'\';'.
															'}" '.
															' maxlength="10" name="new_task_expiry" value="jj/mm/aaaa" />'.
					
						// The button to create a calendar which allow to select a date of expiry directly in it for the current task
						'<a class="calendar_button" onclick="selectDate(\'calContainer\', '.
							'function(date){'.
								' gE(\'new_task_expiry\').value = date; '.
							'}, true, \'FR\');">&nbsp;</a>'.

						// The button to add the task in the user's list and the MYSQL base : the update uses the values contained in the hidden inputs
						'<a class="link_button" onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_todolist/ajax/ajax_todolist.php').'?id_todo='.$this->id.'&amp;wope_action=add_task&amp;done=\'+gE(\'new_task_status\').value+\'&amp;task_title=\'+gE(\'new_task_title\').value+\'&amp;date=\'+gE(\'new_task_expiry\').value,\'todo_block\')" >Ajouter la tâche</a>
					</div>';

	}



	public function addTask($title,$status,$expiry) {
		global $db, $core;
		$core->loadClass('date');
		$date = new Date();

		$db->query("INSERT INTO module_todolist VALUES ('','".$core->getUserId()."','".htmlentities(stripslashes($title), ENT_QUOTES, 'UTF-8')."','".$date->reverseDate($expiry)."','".$status."')");
		$this->displayTasks();
	}


	public function validTask($id_task,$done) {
		global $db, $core;

		$db->query("UPDATE module_todolist SET done = '".$done."' WHERE id = ".$id_task." AND login = '".$core->getUserId()."' ");
		
	}


	public function removeTask($id_task) {
		global $core, $db;

		$db->query("DELETE FROM module_todolist WHERE id='".$id_task."' AND login = '".$core->getUserId()."' ");
		$this->displayTasks();
	}

	
	public function updateTitle($id_task, $title){
		global $core, $db;

		//login is checked for security reasons
		$db->query("UPDATE module_todolist SET title = '".$db->escapeString(htmlentities($title, ENT_QUOTES, 'UTF-8'))."' WHERE id = '".$id_task."' AND login = '".$core->getUserId()."' ");
	}
	
	public function updateDate($id_task, $expiry){
		global $core, $db;
		$core->loadClass('date');
		$date = new Date();
		
		if(!$date->isDate($expiry, 'FR')){
			$expiry = $date->returnDate();
		}else{
			$expiry = $date->reverseDate($expiry);
		}
		
		//login is checked for security reasons
	  $db->query("UPDATE module_todolist SET expiry = '".$expiry."' WHERE id = '".$id_task."' AND login = '".$core->getUserId()."' ");
		$this->displayTasks();
	}

}



?>
