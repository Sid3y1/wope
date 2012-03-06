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


class Module_cinema extends Module {
	
	private $cache;

	public final function __construct($infos = Array() ){
		parent::__construct($infos);
	}
	
/**
	*
	* \brief Affichage du module
	*
	*/
	public function display() {
		global $core;
		$this->cache = new Html_cache('cinema.html','24');

		echo'<div class="module_title_block"><span class="titre_menu"><span>A</span> l\'affiche cette semaine :</span></div>';

		// We check if the cache is usable
		if ($this->cache->isUsable()) {
			debug("On utilise le cache");
			$this->cache->readFile();
		} else {
				// If not we create it !
				debug("On recréé le cache");
				$this->generateCacheFile();
				debug("On affiche le cache que l'on vient de créer");
				$this->cache->readFile();
		}

		$this->filmShowForm();

	}



	private function generateCacheFile() {
		global $core;
		
		$core->loadClass('rss');
		try{
			$rss = new ParseRss('http://www.allocine.fr/xml/rss/cettesemaine.xml',24);
		}catch(FileException $e){
			echo $e->getUserMessage();
			echo "<h4>Service indisponible pour l'instant veuillez réessayer ultérieurement</h4>";
			return false;
		}
		
		// We create the html content
		$content ="";
		$content .= "<ul>\n";
		
		$line_style = '';
		$j = 0;
		foreach ($rss->getItems() as $item) {
			
			if ($j%2) {
				$line_style = 'black';
			} else {
					$line_style = 'grey';
				}
		
			$content .= '<li class="'.$line_style.'">'
										.'<a href="'.$item->link.'"'. 
												 'onmouseover="activateToolTips(this,\''.addslashes(htmlentities($item->description, ENT_COMPAT, 'UTF-8')).'\')">'.$item->title.''.
										 '</a>'
								 .'</li>';
								 $j++;
		}
		
		$content .="</ul>";


		// We create the cache file
		debug("On créé le fichier de cache");
		$this->cache->createFile($content);		
	}

/**
*
*	We send the user on the yahoo cinema web page
*
*
*/


	private function filmShowForm(){
		echo '<a class="link_button" onclick="display(gE(\'filmShowForm\'))">Horaires par salles</a>'
			.'<div class="clear"></div>'
			.'<form id="filmShowForm" action="http://fr.movies.yahoo.com/cinemas/index.html" method="get" style="display: none" target="_blank">'
				.'<input type="hidden" name="mid" value="" />'
				.'<input type="hidden" name="cid" value="" />'
				.'<input type="hidden" name="sp" value="mov" />'
				.'<div class="div_input">'
						.'<label class="left"> Code postal :</label>'
						.'<input class="left" type="text" name="zip" />'
						.'<input class="left" type="submit" value="Ok" />'
				.'</div>'
				.'<div class="clear"></div>'
			.'</form>';
	}
	
}//End of Module_cinema class
								
?>
