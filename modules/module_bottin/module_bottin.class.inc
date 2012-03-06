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


class Module_bottin extends Module {

	public final function __construct($infos = Array() ){
		parent::__construct($infos);
	}
				
  public function display() {
	global $page, $core;

	$core->loadClass('tabs');

	$tabs = new Tabs(Array("pages_jaunes"=>"Pages Jaunes","pages_blanches"=>"Pages Blanches","annuaire_inverse"=>"Annuaire inversé"),'');
	
	$tabs->display();
	$tabs->separatorStart("pages_jaunes");
  echo '
    <form id="pages_j" method="get" action="http://www.pagesjaunes.fr/pj.cgi?" target="_blank">
		
		  <p class="form-input">
			<label>Activité :</label>
      <input type="text" id="activite" name="FRM_ACTIVITE" />
			</p>
			<p class="form-input">
			<label>Nom :</label>
      <input type="text" id="nom_soc" name="FRM_NOM" />
			</p>
			<p class="form-input">
			<label>Adresse :</label>
			<input type="text" id="adresse_soc" name="FRM_ADRESSE" />
			</p>
			<p class="form-input">
			<label>Ville :</label>
      <input type="text" id="localite_soc" name="FRM_LOCALITE" />
			</p>
			<p class="form-input">
			<label>Département :</label>
      <input type="text" id="departement_soc" name="FRM_DEPARTEMENT" />
			</p>
			<p class="form-submit">
      <input type="submit" value="Chercher">
      </p>
    </form>';
	$tabs->separatorStop("pages_jaunes");
	$tabs->separatorStart("pages_blanches");
  echo '
    <form id="pages_b" method="get" action="http://www.pagesjaunes.fr/pb.cgi?" target="_blank">
		
		  <p class="form-input">
			<label>Nom :</label>
      <input type="text" id="nom" name="FRM_NOM" />
			</p>
			<p class="form-input">
			<label>Prénom :</label>
      <input type="text" id="prenom" name="FRM_PRENOM" />
			</p>
			<p class="form-input">
			<label>Adresse :</label>
			<input type="text" id="adresse" name="FRM_ADRESSE" />
			</p>
			<p class="form-input">
			<label>Localité :</label>
      <input type="text" id="localite" name="FRM_LOCALITE" />
			</p>
			<p class="form-input">
			<label>Département :</label>
      <input type="text" id="departement" name="FRM_DEPARTEMENT" />
			</p>
			<p class="form-submit">
      <input type="submit" value="Chercher">
      </p>
    </form>';
	$tabs->separatorStop("pages_blanches");
	$tabs->separatorStart("annuaire_inverse");

  echo'
    <form id="annu_inv" method="get" action="http://www.annuaireinverse.com/G118012/home.asp" target="_blank">
		  <p class="form-input">
			<label>Numéro :</label>
      <input type="text" id="tel" name="RN" size="12">
	 		</p>
			<p class="form-submit">
      <input type="submit" value="Chercher">
			</p>
    </form>';
	$tabs->separatorStop("annuaire_inverse");
	
	}
}
?>
