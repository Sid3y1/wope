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


class Module_event extends Module {

	public final function __construct($infos = Array() ){
		parent::__construct($infos);
	}
  
  public function display()
  {
    global $db,$core,$page;
    $aujourdhui = date("Y-m-d");
    $req = $db->query("SELECT titre,texte,image FROM module_event where (jr_debut<='$aujourdhui' and jr_fin>='$aujourdhui') order by jr_debut asc");
    $url_image='';
    if($db->numRows($req)>0){
	    while ($resultat = $db->fetchArray($req))
	    {
		    $url_image = $core->getConfig('baseServer').'/modules/module_event/images/'.$resultat['image'];
				
		    echo '<div class="event_block">';
				
				$title = stripslashes($resultat['titre']);
		    if(is_file($url_image))
		    { 
				  echo '
					<div class="photo">
						<img src="'.$core->makeUrl('modules/module_event/images/'.$resultat['image']).'" alt="'.$title.'" />
					</div>';
				}

		    echo '
					<div class="event_title"><span class="titre_menu"><span>'.substr($title, 0, 1).'</span>' .substr($title, 1).'</span></div>
					<div class="text">'. $resultat['texte'] .'</div>
				</div>';
	    }
    }else{
    	echo '<p>Pas d\'information importante en ce moment</p>';
    }

		$page->adminLink('module','module_event','admin');
	}
}
?>
