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



class Module_welcome extends Module {

	public final function __construct($informations = Array()) {
		parent::__construct($informations);
	}
												
  public function display()
  {
    global $db,$core,$page;
    $req = $db->query("SELECT title,text,image FROM module_welcome WHERE valid='Y' LIMIT 0,1");
    $url_image='';
    if($db->numRows($req)>0){
	    while ($resultat = $db->fetchArray($req))
	    {
		    $url_image = $core->getConfig('baseServer').'/modules/module_welcome/images/'.$resultat['image'];
				
		    echo '<div class="welcome_block">';
				
				$title = stripslashes($resultat['title']);
		    if(is_file($url_image))
		    { 
				  echo '
					<div class="photo">
						<img src="'.$core->makeUrl('modules/module_welcome/images/'.$resultat['image']).'" alt="'.$title.'" />
					</div>';
				}

		    echo '
					<div class="welcome_title"><span class="titre_menu"><span>'.substr($title, 0, 1).'</span>' .substr($title, 1).'</span></div>
					<div class="text">'. $resultat['text'] .'</div>
				</div>
				<div class="clear"></div>';
	    }
		}
		$page->adminLink('module','module_welcome','admin');
	}
}
?>
