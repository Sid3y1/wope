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
 * This class is used to generate the main menu of the site. This menu is a basic version.
 * If you want to hack it, please do it in the indexMenu class (located in the skin directory) by redefinning functions of the this class.
 * We ask you to do it that way in order to avoid any changes directly on the site core.
 */
class IndexMenuBasic {

  /**
   * This function display the menu on every pages of the site
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function display(){
		global $core;
	
		try{
		  // We use a user cache and not a general one so that there will only the displayMenu function to override to change the behaviour of the menu.
			$cache = new User_cache('menu','1',$this, 'displayMenu');
			$cache->displayCache();
		}
		catch (FileException $e){

			echo $e->getFullDebugMessage();
			return false;
		}
	}

  /**
   * This function display the menu without any cache layer.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return String
   */
  public function displayMenu(){
    global $core, $db;
    
    $res = $db->query("SELECT ksm.href, ksm.name, ksm.image, ksm.permission, kr.name as permission_name
						FROM kernel_skin_menu ksm, kernel_right kr 
						WHERE ksm.permission=kr.id AND valid='Y' ORDER by rank");
    if ($db->numRows($res) > 0){
      echo '<div class="wope_menu_block">'
   	  	.'<div class="wope_menu_bar">'
	    	.'<ul>';
      while($don = $db->fetchArray($res))
      {
        if ($core->verifDroits($don['permission_name'])){
          echo '<li>
					<a href="'. $core->makeUrl(stripslashes($don['href'])) .'">'.stripslashes($don['name']).'</a>
				</li>';
        }
      }
    
      echo '</ul>'
		.'</div>'
             .'</div>';
    }
  }
}
