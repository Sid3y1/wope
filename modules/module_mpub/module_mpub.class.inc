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


class Module_mpub extends Module {

	private $ads = Array();

	public final function __construct($informations = Array()) {
		global $db;

		parent::__construct($informations);
		
		if($db->numRows($db->query("SELECT * FROM module_mpub WHERE valide = 'Y' AND place = 'top' ")) == 0){
			$this->setContentInModule(false);
		}
	}

		
	public function display() {

		$this->displayAd($place);
  }


  public function displayAd($place) {
		global $core, $db;

		if ($place != 'bottom') {
			$place = 'top';
		}
		$ads_result = $db->query("SELECT * FROM module_mpub WHERE valide = 'Y' AND place = '".$place."'");
		$nbimages = $db->query("SELECT COUNT(id) as nbi FROM module_mpub WHERE valide = 'Y' AND place = '".$place."'");
		$nb = $db->fetchArray($nbimages);

		$total = $nb['nbi'];

		if($total) {
    	$i = 1;
    	while($row = $db->fetchArray($ads_result)) {
      	$adresse_image[$i] = $row['image'];
				$lien_image[$i] = $row['link'];
      	$i++;
    	}

    	srand((double)microtime()*1000000);
    	$image_affichee = rand(1,$total);

		}
		if ($total > 0) {

			if ($place == 'bottom') {
				echo '<div id="ad_space_2" style="height: 80px">';
			}	
			
			echo'
				<div class="pub">
					<a href="'.$lien_image[$image_affichee].'">
						<img src="'.$core->makeUrl('modules/module_mpub/images/').$adresse_image[$image_affichee].'" />
					</a>
				</div>
				 ';

			if ($place == 'bottom') {
				echo '</div>';
			}
		
		}
 	}

	}
?>
