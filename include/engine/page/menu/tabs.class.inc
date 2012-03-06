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


class Tabs {
 private $defaultTab;
 private $rand_id;

 private $tabs;
 private $default;

	public final function __construct($tabs, $default='') {
	 global $core;
		$this->rand_id=rand();
		$this->tabs = $tabs;
		$this->default = $default;
	}

	/**
	 * tabsPrint Affiche les onglets !
		* 
	 * @param mixed $tabs 
	 * @param $default //default tab to display
	 * @access public
	 * @return void
	 */
	public function display(){
		echo '<ul class="tabs" id="tabs_list_'.$this->rand_id.'" >';

		$keys = array_keys($this->tabs);
			
		if($this->default=='' || ($this->default!='' && !in_array($this->default, $keys)) ){
			$this->defaultTab = $keys[0];
		}else{
			$this->defaultTab = $this->default;
		}
		
		foreach($this->tabs as $key => $tab){
			$class='';
			if($this->defaultTab == $key){
				$class = ' class="selectedTab" ';
			}
			echo '<li id="tab_'.$this->rand_id.'_'.$key.'" '.$class.' ><a onclick="TABS.displayTab(\'tabs_list_'.$this->rand_id.'\',\''.$this->rand_id.'_'.$key.'\')" >'.$tab.'</a></li>';
		}	
		echo '</ul>';
	}

	public function onclickGotoTab($tab){
		return 'TABS.displayTab(\'tabs_list_'.$this->rand_id.'\',\''.$this->rand_id.'_'.$tab.'\');';
	}

	/**
	 * separatorStart Declare le debut du contenu d'un onglet
		* 
	 * @param mixed $key 
	 * @access public
	 * @return void
	 */
	public function separatorStart($key){
	 	if($key == $this->defaultTab){
   		echo '<div id="'.$this->rand_id.'_'.$key.'" style="display:block" class="tabsContent">';
		}else{
   		echo '<div id="'.$this->rand_id.'_'.$key.'" style="display:none" class="tabsContent">';
		}
	}
	
	/**
	 * separatorStop Declare la fin du contenu d'un onglet 
	 * 
	 * @param string $key 
	 * @access public
	 * @return void
	 */
	public function separatorStop($key = ''){
	 	//Whaw ca c de la fonction php !
  	echo '</div>';
 	}

	public function getRandId(){
		return $this->rand_id;
	}
 
}
