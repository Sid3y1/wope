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

class Signup {

	/*
	 * id of the object
	 */
	private $id;

	/*
	 * path to access the object
	 */
	private $path;

	/*
	 * array of configuration values
	 */
	public $config;

 /*
	* The array containing all the errors
	*
	*/
	private $error;

	public final function __construct($path = 'signup') {

		include_once(dirname(__FILE__).'/config_signup.inc');
		$this->config = $config;
		$this->path = $path;
		
		do{
			$this->id = rand(1000000,9999999);
		}while( isset($_SESSION['signup_'.$this->id]) );

		$_SESSION['signup_'.$this->id] = serialize($this);
	}
	
	public function display() {
		global $core, $page;
		
		// We store the value saying if a mail address is required or not
		$_SESSION['isMailCreated'] = $this->config['isMailCreated'];
		
		// We store the value saying if subscribe must be validated by admin
		$_SESSION['activeUser'] = $this->config['activeUser'];

		// right to give to new user
		$_SESSION['newAccountRight'] = $this->config['newAccountRight'];

		// Id's of the steps
		$_SESSION['steps_id'] = $this->id;
		
		$cur_step = 0;
		// Position of the step in the config Array
		$_SESSION['array_cur_pos'] = $cur_step;
	
		$cur_step++;
		// Number of the step to display to the user in the module header
		$_SESSION['cur_step'] = $cur_step;

		// Total number of steps
		$steps_number = $this->config['stepsNumber'];
		$_SESSION['steps_number'] = $steps_number;
			
	
		$page->moduleheader(_('Sign up : step #  ').'<span id="current_step">'.$_SESSION['cur_step'].'</span> / '.$steps_number,'');
		echo '<div id="custom_form">';
		/*********************************************************/
		
		
		$this->displayStep($this->config['steps']['0'],'');
		
		
		/*********************************************************/
		echo '</div>';
		$page->modulefooter();
	}


	public function displayStep($target_step,$class_path) {
		global $core;
		
		// We include the step's class we want to display AND cut the entire name of the step to extract the class name from it
		include_once($class_path.$target_step.'.class.inc');
		$class_name = substr($target_step,5,100);
		
		// If the step has never been displayed (and so never stored in session)
		if ( !isset($_SESSION[$class_name.'_'.$this->id]) ) {
			$step_to_display = new $class_name();
			$_SESSION[$class_name.'_'.$this->id] = serialize($step_to_display);
			$step_to_display->display();
		}
		// If the step is already stored in session
		else {
			$step_to_display = unserialize($_SESSION[$class_name.'_'.$this->id]);
			$step_to_display->display();
		}


	}

	
	public function storeInfos() {
		global $db, $core;

		// We execute all pre-creation hooks
		$this->executePostCreationHooks();
	
		// For each step that have so store infos, we launch the sendInfosToBase method.
		for ($i = 0; $i < ($this->config['stepsNumber'] - 2); $i++) {
		
			include_once($this->config['steps'][$i].'.class.inc');
			$step_name = substr($this->config['steps'][$i],5,100);
			$step_to_store = unserialize($_SESSION[$step_name.'_'.$this->id]);

			$step_to_store->sendInfosToBase();

		}
		if (!isset($_SESSION['avatar_original_name'])) {
			$core->loadClass('filemanager');
			$file_m = new FileManager();
			$avatar_id = $file_m->moveFileToFm($core->getConfig('baseServer').'/signup/images/nophoto.png', 'nophoto.png', true, false);

			$db->query("UPDATE trombi_trombi SET photo ='".$avatar_id."' WHERE id_user='".$_SESSION['new_user']."'");
		}

		// We execute all post-creation hooks
		$this->executePreCreationHooks();
	}

	private function executePostCreationHooks() {
		foreach ($this->config['hooks']['post_creation'] as $file) {
			include(dirname(__FILE__)."/hooks/$file");
		}
	}

	private function executePreCreationHooks() {
		foreach ($this->config['hooks']['pre_creation'] as $file) {
			include(dirname(__FILE__)."/hooks/$file");
		}
	}




}
//End of Signup class.


?>
