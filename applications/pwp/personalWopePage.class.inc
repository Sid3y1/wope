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

/**
	* This class is used to display the personal start of the users
	*/
class PersonalWopePage
{
  /**
   * Constructor
   * 
   * @author Nicolas Demengel & Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
	public final function __construct(){
	  //void
	}

	/**
	 * Add a module on the page and store it in session.
	 *
	 * @author Nicolas Demengel & Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @param int $mId Module Id
	 * @param int $cId Column Id
	 * @return Module
	 */
	public function addModule($mId, $cId){
		global $db, $core;

		$res = false;

		if ( $core->session->isAuthOk() ) {
			if( $result = $db->query("SELECT addModuleToUser('".$mId."', '".$core->getUserId()."', '".$cId."') ") ){

				$id = $db->result($result, 0);

				$result = $db->query("SELECT * FROM kernel_module_user_module_final WHERE id = '".$id."' ");
				$row = $db->fetchArray($result);

				$class = stripslashes($row['class']);
				$core->loadClass($class);
				$module = new $class($row);

				$_SESSION['userModules'][ $core->getUserId() ]['modules'][ $id ]['class'] = $class;
				$_SESSION['userModules'][ $core->getUserId() ]['modules'][ $id ]['object'] = serialize($module);

				$res = $module;
			}
		}

		return $res;

	}

	/**
	 * This function just include the module class and load the css of the module.
	 *
	 * @author Nicolas Demengel & Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @param String $class The name of the module (his class).
	 * @return void
	 */
	public function loadModule($class){
		global $core;

		$core->loadClass($class);
		$this->loadModuleCss($class);

	}

	/**
	 * loadModuleCss Charge la css du module !
	 * 
	 * @author Nicolas Demengel & Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @param String $name The name of style sheet 
	 * @return void
	 */
	private function loadModuleCss($name){
		global $core;
		if(is_file(dirname(__FILE__) . '/../../../skins/'.$core->user->getSkin().'/css/modules/'.$name.'.style.css')){
  			echo '<style>
		 	@import url('.$core->makeUrl('skins/'.$core->user->getSkin().'/css/modules/'.$name.'.style.css').');
			</style>';
		}else{
 			echo '<style>	
			@import url('.$core->makeUrl('modules/'.$name.'/'.$name.'.style.css').');
			</style>';
		}
	}

	/**
	 * This function display everything of the start page
	 * @author Nicolas Demengel & Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return void
	 */
	public function displayModules(){
		global $core, $db;
		$core->loadClass('tabs');
		
		//to load script of user's modules
		$core->javascriptCore->loadContextScripts('index','.');
					
		$curTab = $curCol = $tabPos = $colPos = 0;
		$arTabs = Array();


		$userModulesCount = " SELECT COUNT(*) ";
		$userModulesMiddle = " FROM kernel_module_user_module_final ";
		$userModulesCountCondition = " WHERE idUser = '".$core->getUserId()."' AND idModule != 0 ";
		
		//always test this before calling `checkMandatoryModulesForUser`
		//if user has no module, adds him default modules
		$userModulesTotal = $db->result($db->query($userModulesCount.$userModulesMiddle.$userModulesCountCondition), 0);
		if($userModulesTotal == 0){
			$db->query(" CALL addDefaultModulesToUser('".$core->getUserId()."') ");
		
		}
			//checks if user has all mandatory modules, with good parameters
			$db->query(" CALL checkMandatoryColumnsForUser('".$core->getUserId()."') ");
			$db->query(" CALL checkMandatoryModulesForUser('".$core->getUserId()."') ");
		
		//checks module positions, reorders if necessary
		$db->query(" CALL checkModulesOrderForUser('".$core->getUserId()."') ");


		$userTabsCount = " SELECT COUNT(*) ";
		$userTabsSelect = " SELECT mut.id_tab, mut.name, mut.columns ";
		$userTabsMiddle = " FROM kernel_module_user_tab mut INNER JOIN kernel_module_tab mt ON mut.id_tab = mt.id ";
		$userTabsCondition = " WHERE mt.available = 'Y' AND id_user = '".$core->getUserId()."' ";
		$userTabsEnd = " ORDER BY position ";

		//are there several tabs or just one ? (or none)
		$sevTabs = $db->result($db->query($userTabsCount.$userTabsMiddle.$userTabsCondition), 0) > 1 ? true : false;

		//selects user tabs
		$userTabsResult = $db->query($userTabsSelect.$userTabsMiddle.$userTabsCondition.$userTabsEnd);
		//creates tabs
		while( $rowTab = $db->fetchArray($userTabsResult) ){
			echo $rowTab['name'];
			$this->tabs[ 'tab_'.$rowTab['id_tab'] ] = $rowTab['name'];
			$arTabs[] = $rowTab;
		}

		if($sevTabs){
			$this->tabBar = new Tabs($this->tabs, null);
		}
	
		//display tabs
		foreach($arTabs as $rowTab){
		


			$userModulesSelect = " SELECT * ";
			$userModulesMiddle = " FROM kernel_module_user_module_final ";
			$userModulesCondition = " WHERE idUser = '".$core->getUserId()."' OR idUser = '0' ";
		
			//selects user modules
			$userModulesResult = $db->query($userModulesSelect.$userModulesMiddle.$userModulesCondition);

			$this->openTab($rowTab['id_tab'], $sevTabs);

			//creates modules (and columns)
			while( $row = $db->fetchArray($userModulesResult) ){
				$class = stripslashes($row['class']);
			
				//column opening/closing management
				if( $row['column'] != $curCol ){
					$colPos++;
				
					if( $curCol != 0 ){
						$this->closeColumn($curCol);
					}
					
					$curCol = $row['column'];

					$this->openColumn($curCol, $colPos, $row['columnWidth']);

				}
		
				// case 0 is to note there is a column, possibly empty
				if( $row['idModule'] != 0 ){
					
					//module not in $_SESSION => creates it
					if( ! isset( $_SESSION['userModules'][ $core->getUserId() ]['modules'][ $row['id'] ] ) ){
						$core->loadClass($class);
						$module = new $class($row);
							
						$_SESSION['userModules'][ $core->getUserId() ]['modules'][ $row['id'] ] = 
							Array( 
								'class' => $class, 
								'object' => serialize($module)
							);

					//recovers the module from $_SESSION
					}else{
						$core->loadClass($class);
						$module = unserialize( $_SESSION['userModules'][ $core->getUserId() ]['modules'][ $row['id'] ]['object'] );
					}
					
					if( $module->isContentInModule() ){
						$module->displayModule();
					}
					
				}
					
			}//end while

			//column opening/closing management
			if( $curCol != 0){
				$this->closeColumn($curCol);
			}
		
			$this->closeTab($rowTab['id_tab'], $sevTabs);

		}//end while
		
	}

	/**
	 * Open a new tab in the HTML structure.
	 *
	 * @author Nicolas Demengel & Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @param int $id The id identifiying the tab
	 * @param  boolean $sevTabs Is there sevral tabs or not
	 * @return void
	 */
	private function openTab($id, $sevTabs){
		if($sevTabs){

			$this->tabBar->separatorStart('tab_'.$id);
		}else{
			echo '<div id="tab_'.$id.'" >';
		}
	}

	/**
	 * Close a tab in the HTML structure.
	 *
	 * @author Nicolas Demengel & Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @param int $id The id identifiying the tab
	 * @param  boolean $sevTabs Is there sevral tabs or not
	 * @return void
	 */
	private function closeTab($id, $sevTabs){
		if($sevTabs){

			$this->tabBar->separatorStop('tab_'.$id);
		}else{
			echo '</div>';
		}
	}

	/**
	 * Open a new column in the HTML structure.
	 * 
	 * @author Nicolas Demengel & Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @param int $id The Id of the column
	 * @param int $pos The position of the column
	 * @param int $width The width of the column
	 * @return void
	 */
	private function openColumn($id, $pos, $width = 0){
		$w = $width != 0 ? ' style="width: '.$width.'px; overflow: hidden;" ' : '';
		echo '<div id="column_'.$id.'" class="column'. --$pos .'" '.$w.' >';
	}

	/**
	 * Close a column in the HTML structure.
	 * 
	 * @author Nicolas Demengel & Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @return void
	 */
	private function closeColumn(){
		echo '</div>';
	}

}

?>
