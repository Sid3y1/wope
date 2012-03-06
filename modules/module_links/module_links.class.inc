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


class Module_links extends Module {

	public function display() {
		global $page, $core, $db;
		if(!isset($page))$page = new Page();
		$core->loadClass('date');
		$date = new Date();
		
		$res = $db->query("SELECT 
												module_links.id,
												module_links.name, 
												module_links.url, 
											  module_links.description,
												module_links.valid,
												module_links.date
													
											FROM 
												( module_links LEFT JOIN
													( kernel_tag_information info LEFT JOIN
														( kernel_tag_information_category info_categ INNER JOIN kernel_tag_category_user categ ON info_categ.idCategory = categ.idCategory )
														ON info.id = info_categ.idInformation )
													ON module_links.id = info.idOrig )
													
											WHERE module_links.valid = 'Y'
											 	AND info.tableOrig = 'module_links'
											 	AND categ.isShow = 'true'
											 	AND categ.login = '".(int)$core->getUserId()."'
											GROUP BY module_links.id
											ORDER BY module_links.id DESC
											LIMIT 6");
	
		echo '<div class="block_module_title"><span class="titre_menu"><span>L</span>es derniers liens utiles :</span></div>';
		echo '<div class="links_block">';
		
		$style = false;
		
		$i = 0;
		while ($links = $db->fetchArray($res)) {
		
			if ($i%2) {
				$sec_class = 'a1';
			} else { $sec_class = 'b1'; }
		
			echo '<div class="link_line '.$sec_class.'">';
			
							$tooltip_content = $page->htmlBlock->escapeTipContent(stripslashes('<p class="tt_link_title">'.$links['name'].'</p>'));
							
							if (isset($links['description']) && $links['description'] != '') {
								$tooltip_content .= $page->htmlBlock->escapeTipContent(stripslashes('<p class="tt_link_description">-> '.$links['description'].'</p>'));
							}

							if (strlen(stripslashes($links['name'])) > 55) {
								$link_title = substr(stripslashes($links['name']),0,55).'...';
							} else {
									$link_title = stripslashes($links['name']);
							  }
							
			echo'		<div class="block_link_title left">
								<a class="useful_link" onmouseover="activateToolTips(this,\''.$tooltip_content.'\')" href="'.stripslashes($links['url']).'">'. $link_title .'</a> 
							</div>
							<span left>ajouté le '.$date->reverseDate($links['date']).'</span>
						</div>';
			$i++;
		}
		
		echo '</div>
		
		<a class="see_all_links left" href="'.$core->makeUrl("/modules/module_links/index.php").'?display=all">Afficher tous les liens...</a>
		<div class="clear"></div>';
		$page->adminLink('module','module_links','admin');




		echo'<div class="clear"></div>';
		
	}


	public function displayLinksList($cur_page, $nb_per_page, $request='') {
		global $core,$db,$page;
		$core->loadClass('date');
		$date = new Date();
		if(!isset($page))$page = new Page();
		$box = new Box();
		if(!isset($cur_page) || $cur_page=='')$cur_page=1;
		if(!isset($nb_per_page) || $nb_per_page=='')$nb_per_page = 20;
		$start_row = ($cur_page - 1) * ($nb_per_page);

		//request construction
		if ($request == '') {
		$request = "SELECT ml.id, ml.name, ml.url, ml.description, ml.date
			FROM 
			( module_links ml LEFT JOIN
				( kernel_tag_information info LEFT JOIN
					( kernel_tag_information_category info_categ INNER JOIN kernel_tag_category_user categ ON info_categ.idCategory = categ.idCategory )
					ON info.id = info_categ.idInformation )
				ON ml.id = info.idOrig )

			WHERE ml.valid = 'Y'
				AND info.tableOrig = 'module_links'
				AND categ.isShow = 'true'
				AND categ.login = '".$core->getUserId()."'
			GROUP BY ml.id
			ORDER BY date DESC";
		}
		
		$display_request = $request.' LIMIT '.$start_row.','.$nb_per_page.' ';
		
		//query
		$result = $db->query($display_request);

		if($db->numRows($result)==0){
			echo 'Aucun résultat';
		} else {
		
			//display
		echo '<div class="links_block">';
		
		$i = 0;
		while ($links = $db->fetchArray($result)) {

			if ($i%2) {
				$sec_class = 'a1';
			} else { $sec_class = 'b1'; }
	
			if (strlen(stripslashes($links['name'])) > 65) {
				$link_title = substr(stripslashes($links['name']),0,65).'...';
			} else {
					$link_title = stripslashes($links['name']);
			  }
	
			echo '<div class="link_line '.$sec_class.'">';
			
							$tooltip_content = $page->htmlBlock->escapeTipContent(stripslashes('<p class="tt_link_title">'.$links['name'].'</p>'));
							
							if (isset($links['description']) && $links['description'] != '') {
								$tooltip_content .= $page->htmlBlock->escapeTipContent(stripslashes('<p class="tt_link_description">-> '.$links['description'].'</p>'));
							}
							
			echo'		<div class="block_link_title left">
								<a class="useful_link" onmouseover="activateToolTips(this,\''.$tooltip_content.'\')" href="'.stripslashes($links['url']).'">'. $link_title .'</a> 
							</div>
							<span left>ajouté le '.$date->reverseDate($links['date']).'</span>
						</div>';
			$i++;
		}
		
		echo '</div>';

		$page->htmlBlock->pagesList($request,$nb_per_page,$cur_page,'onclick','AJAX.getAndUpdate(\''.$core->makeUrl("modules/module_links/ajax/links_ajax.php").'?wope_action=display_result&nb_per_page='.$nb_per_page.'&cur_page=[#]\',\'LinksList\')');
		}

	}


	public function displayTypes() {
		global $db, $page, $core;
		if (!isset($page))
			$page = new Page();
		echo '<div class="links_classes">';
		$result = $db->query("SELECT idCategory as id,name,image 
							FROM kernel_tag_category_user categ 
							WHERE categ.login='".(int)$core->getUserId()."'
								AND categ.isShow='true'");
		echo '<ul class="panel"><li class="link_menu"><a href="index.php?display=all" >Tous types</a></li>';
		while ($row = $db->fetchArray($result)) {
			echo '<li class="link_menu"><a href="' . $_SERVER['PHP_SELF'] . '?display=' . $row['id'] . '" title="' . stripslashes($row['name']) . '">'; 
			echo ' ' . stripslashes($row['name']) . '</a></li>';
		}
		echo '</ul></div>';
	}


	public function displayLinksType($id_type = 'all') {
		global $core, $db, $page;

		if (!empty ($id_type) && $id_type != 'all') {
			$id_type=(int)$id_type;
			$sqlType = " AND categ.idCategory = $id_type ";
		} else {
			$sqlType = '';
		}
		$result = "SELECT 
												categ.image AS image,
												categ.name AS type,
												module_links.id,
												module_links.name, 
												module_links.url, 
												module_links.description,
												module_links.valid,
												module_links.date
																	
									FROM kernel_tag_category_user categ, 
											 kernel_tag_information info,
											 kernel_tag_information_category info_categ,
											 module_links 
											 
									WHERE module_links.valid = 'Y'
										AND info.idOrig = module_links.id
										AND info.tableOrig = 'module_links'
										AND info_categ.idInformation = info.id
										AND info_categ.idCategory = categ.idCategory
										AND categ.isShow = 'true'
										AND categ.login = '".(int)$core->getUserId()."'
										".$sqlType."
										
									GROUP BY module_links.id
									ORDER BY name DESC";
				if ($db->numRows($db->query($result)) > 0) {
					echo '<div class="clear"></div>';
					echo '<div class=""></div>';
					$this->displayLinksList('','',$result);
				} else {
						echo '<div class="clear"></div>';
						echo '<p>Aucun lien dans cette catégorie</p>';
					}
	}

}

?>
