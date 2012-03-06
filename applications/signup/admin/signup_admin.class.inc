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


class Signup_Admin {
	
	private $id;
	
	public final function __construct(){
		$this->id = rand(1000000,9999999);
		$_SESSION['signup_admin_'.$this->id] = serialize($this);
	}

	public function displayContent(){
		global $page;
		if(!isset($page))$page = new Page();

		$page->moduleHeader('Inscriptions','');
		echo '<div id="signup_admin_content">';
		echo '</div>';
		$page->moduleFooter();
	}
	
	public function displayMenu(){
		global $page,$core;
		if(!isset($page))$page = new Page();

		$page->moduleHeader('Inscription','');
		echo '<ul class="panel">
			<li></li>
		</ul>';
		$page->moduleFooter();
	}

}
?>
