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


class Menu {
	public $indexMenu;

	public final function __construct() {
		global $core;
		$core->loadClass('indexMenu');
		$this->indexMenu = new IndexMenu();
	}

	/**
	 * makeMenu Affiche un menu avec le contenu du tableau
	 * 
	 * @param mixed $data 
	 * @param mixed $type 
	 * @access public
	 * @return void
	 */
	public function makeMenu($data,$type){
  echo '<ul class="'.$type.'">';
		$this->makeMenuLinks($data);
  echo '</ul>';
	}


 /**
  * makeMenuLinks
		* Fonction de makeMenu qui est récursive
  * 
  * @param mixed $data 
  * @access private
  * @return void
  */
	private function makeMenuLinks($data){
		foreach ($data as $name => $href){
			if(is_array($href)){
    		echo '
				<li><a onclick="display(this.parentNode.lastChild);">'.$name.'</a>
					<ul style="display:none">
				';
				$this->makeMenuLinks($href);
				echo '
					</ul>
				</li>';
			}else{
   			echo '
	 			<li><a href="'.$href.'">'.$name.'</a></li>';
			}
		}
  
	}

}
