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


class ModuleBasic {

	private $id;

	private $class = '';

	private $idModule;

	private $tab;

	private $title = '';

	private $minimized = 'N';

	private $mandatory = 'N';

	private $instantiable = 'N';

	private $drag = 'N';

	protected $name = '';

	protected $theme;

	protected $dynamicTitle = '';

	private $contentInModule = true;

	public function __construct($informations){
		if( is_array($informations) ){
			foreach( $informations as $name => $value ){
				if( property_exists($this, $name) ){
					$this->$name = stripslashes($value);
				}
			}
			
		}else{
			throw new GException('ModuleBasic::__construct() needs an array of arguments', 4000, 1, 'Creation of a module without informations');
		}
		
	}
	
	public function getId(){
	  return $this->id;
	}
	
	public function getTab(){
		return $this->tab;
	}
	
	public function moveUp(){
		global $db, $core;
		
		if( ! $db->query("CALL moveModuleUp('".$this->id."') ") ){
			throw new GException('"CALL moveModuleUp(\''.$this->id.'\') " failed.', 4000, 1, 'A problem occured when moving up the module');
		}
		
	}

	public function moveDown(){
		global $db, $core;
		
		if( ! $db->query("CALL moveModuleDown('".$this->id."') ") ){
			throw new GException('"CALL moveModuleDown(\''.$this->id.'\') " failed.', 4000, 1, 'A problem occured when moving down the module');
		}

	}

	public function miniMaximize(){
		global $db, $core;
		
		if( ! $db->query("UPDATE kernel_module_user_module SET minimized = '".( $this->minimized == 'Y' ? 'N' : 'Y')."' WHERE id = '".$this->id."' ") ){
			throw new GException('MiniMaximize request on module : '.$this->class.' - '.$this->id.' failed.', 4000, 1, 'A problem occured when managing the module');
		}
		
		$this->minimized = ( $this->minimized == 'Y' ) ? 'N' : 'Y' ;
		
		if( ! $_SESSION['userModules'][ $core->getUserId() ]['modules'][ $this->id ]['object'] = serialize($this) ){
			throw new GException('Serialization of module : '.$this->class.' - '.$this->id.' failed.', 4000, 1, 'A problem occured when managing the module');
		}

	}

	public function remove(){
		global $db, $core;
		
		if( ! $db->query("CALL removeModuleFromUser('".$this->id."', '".$core->getUserId()."')") ){
			throw new GException('"CALL removeModuleFromUser(\''.$this->id.'\', \''.$core->getUserId().'\') " failed.', 4000, 1, 'A problem occured when removing the module');
		}
		
	}

	public function displayHeader($new = false){
		global $core, $page;
		
		echo '<div class="module ' . substr($this->class, 7) . '" id="M' . $this->id . '" >'
			.'<div class="module_border">';
		echo '
			<div class="title">
			<div class="extraDiv1"><span>&nbsp;</span></div>
			<div class="extraDiv2"><span>&nbsp;</span></div>
			<h2>'	. html_entity_decode($this->title, ENT_QUOTES, 'UTF-8') . ' ' . $this->name . ' ' . $this->dynamicTitle . '</h2>';
		
		if ( $core->session->isAuthOK() ){
			$moduleInNewDiv = $new ? 'true' : 'false';
		
			echo '<ul>';
			
			if( $this->mandatory == 'N'){
				echo '<li><a onclick="module.minimize(\''.$this->id.'\', true)" title="'.locale::display('minimize','kernel').'" class="minimize">'.locale::display('minimize','kernel').'</a></li>';
			}
		
			if( $this->drag == 'Y'){
				echo '<li><a onclick="module.action(\'up\',\''.$this->id.'\',\'\',\'\','.$moduleInNewDiv.', true);" title="'.locale::display('top','kernel').'" class="up">'.locale::display('top','kernel').'</a></li>';
				echo '<li><a onclick="module.action(\'down\',\''.$this->id.'\',\'\',\'\','.$moduleInNewDiv.', true);" title="'.locale::display('bottom','kernel').'" class="down">'.locale::display('bottom','kernel').'</a></li>';
			}
			
			if( $this->mandatory == 'N'){
				echo '<li><a onclick="module.action(\'del\','.$this->id.','.$this->idModule.',\''.$page->htmlBlock->escapeTipContent($this->title).'\','.$moduleInNewDiv.', true, '.( $this->instantiable == 'Y' ? 'true' : 'false' ).');" title="'.locale::display('close','kernel').'" class="close">'.locale::display('close','kernel').'</a></li>';
			}
			
			echo '</ul>';
			
		}
	
	echo '<div class="clear"></div>
		</div>';
		
		$minimizedStyle = $this->minimized == 'Y' ? ' style="display: none;" ' : '';

		echo '<div class="content" id="mod' . $this->id . '" ' . $minimizedStyle . ' >';

	}

	public function displayFooter(){
		echo '<div class="clear"></div></div>'; //close "content"
		echo '<div class="clear" id="lineM'. $this->id .'"></div>';
		echo '</div></div>'; //close "module"
	}

	public function displayModule($new = false){
		$this->displayHeader($new);
		$this->display();
		$this->displayFooter();
	}
	
	public function isContentInModule(){
		return (bool)($this->contentInModule);
	}

	public function setContentInModule($bool){
		$this->contentInModule = $bool;
	}

	public function saveInSession(){
	  global $core;

	  //pre($_SESSION['userModules'][ $core->getUserId() ]['modules'][ $this->id ]['object']);
	  $_SESSION['userModules'][ $core->getUserId() ]['modules'][ $this->id ]['object'] = serialize($this);
	}

}

?>
