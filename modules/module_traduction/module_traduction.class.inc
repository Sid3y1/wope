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



class Module_traduction extends Module {

	public final function __construct($informations = Array()) {
		parent::__construct($informations);
	}

  public function display() {
    global $page;

	// Modifier l'adresse du site de traductiuon
	?>
	<form action="http://translate.google.com/translate_t" target="_blank" method="post">
		<div>
  		<input name="hl" value="fr" type="hidden" />
  		<input name="ie" value="UTF-8" type="hidden" />
  		<input name="oe" value="UTF-8" type="hidden" />
	  	<input type="text" size="33" name="text" />
	  	<select name="langpair">
 		 	  <option value="de|fr">Allemand -> Français</option>
 		 	  <option value="de|en">Allemand -> Anglais</option>
 	 		  <option value="en|fr">Anglais -> Français</option>
  		  <option value="en|de">Anglais -> Allemand</option>
  		  <option value="en|es">Anglais -> Espagnol</option>
  		  <option value="en|it">Anglais -> Italien</option>
  		  <option value="en|pt">Anglais -> Portugais</option>
  		  <option value="en|ru">Anglais -> Russe</option>
  		  <option value="es|en">Espagnol -> Anglais</option>
  		  <option selected="selected" value="fr|en">Français -> Anglais</option>
  		  <option value="fr|de">Français -> Allemand</option>
  		  <option value="it|en">Italien -> Anglais</option>
  		  <option value="pt|en">Portugais -> Anglais</option>	
 	  	</select>
 	  	<input type="submit" value="Traduire" />
		</div>
	</form>
  <?
  }
}
?>
