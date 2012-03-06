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


class Module_news extends Module {

	private $login;
	
	private $config;

	public final function __construct($informations = Array()) {
		global $core;

		parent::__construct($informations);
		
		$this->login = $core->getUserId();
		include('config_module_news.inc');
		$this->config = $config_module_news;
		
	}

	public function display() {
		global $core, $db, $page;
		$core->loadClass("date");
		
		$result = $db->query("SELECT 
								categ.image AS image,
								categ.name AS type,
								news_liste.id_news AS id_news,
								news_liste.date_creation AS date_creation,
								news_liste.titre AS titre,
								news_liste.information AS content,
								news_liste.origine AS source 
							FROM news_liste, 
								kernel_tag_category_user categ, 
								kernel_tag_information info,
								kernel_tag_information_category info_categ
							WHERE news_liste.valide = 'Y'
								AND info.idOrig=news_liste.id_news
								AND info.tableOrig='news_liste'
								AND info_categ.idInformation=info.id
								AND info_categ.idCategory=categ.idCategory
								AND categ.isShow='true'
								AND categ.login='".$this->login."'
							GROUP BY news_liste.id_news
							ORDER BY date_creation DESC
							LIMIT 0,6");
							
							
		echo '<div class="centrage">
      	<span class="panel_h">
      		<a href="'.$core->makeUrl('modules/module_news/index.php?action=add').'" title="'._('Submit a news').'">'._('Submit a news').'</a> | 
      		<a href="'.$core->makeUrl('modules/module_news/index.php?action=display&id_type=all').'" title="'._('Display all').'">'._('Display all').'</a>';
      	echo '</span>
			</div>';



		echo '<div class="news_list">';
		if ($db->numRows($result) > 0) {
			$i = 0;
			while ($row = $db->fetchArray($result)) {
				$i++;
				/*  if($core->getLogin()!='')
				  {
				
							Edit Sylv : intéressant mais mal conçu : à revoir 
				  if($db->result($db->query("SELECT COUNT(*) FROM news_read WHERE login='".$core->getLogin()."' AND id_news=$row[id_news]"),0) == 0)
				    $newStat = ' class="new"';
				  else
				    $newStat = '';
				}
				
				else
				  $newStat = '';
				  echo '<li'. $newStat .'>';
				*/
				if ($i % 2) {
					$classe = 1;
				} else {
					$classe = 2;
				}

				echo '<div class="liste_news l' . $classe . '">';

				$row['titre'] = stripslashes($row['titre']);
				$first_letter = substr($row['titre'], 0, 1);
				$end_title = substr($row['titre'], 1, 200);
				$date = new Date($row['date_creation']);

				$toolTips_content = '
					<div class="img_block">
						<img src=data/files/categ_img/' . $row['image'] . ' alt="'.stripslashes($row['type']).'" />
					</div>					
				<div class="details_block">	
					<div class="sujet_block"><span class="sujet_news"><span>' . $first_letter . '</span>' . $end_title. '</span></div>
					<div class="date_publication">' . $date->display('/') . '</div>
					<div class="source"><span>source : </span>' . stripslashes($row['source']). '</div>
				</div>
					
					<div class="clear"></div>
					<div class="le_texte">' . stripslashes($row['content']) . '</div>';
				
				echo '<a onmouseover="'.$page->htmlBlock->tooltip($toolTips_content).'" href="' . $core->makeUrl('modules/module_news/index.php?id_news=' . $row['id_news'] . '') . '" >
					<span class="date_publication">' . $date->formatDate($row['date_creation'], "d/m à H:i") . '</span>';
				echo '<span class="titre_news">' . stripslashes($row['titre']) . '</span></a></div>';

			}
		}
		echo "</div>";
		$page->adminLink('module', 'module_news', $this->config['admin_rights']);
	}

	public function displayAppHeader() {
		global $core, $page;
		if (!isset ($page))
			$page = new Page();
		echo '<div class="types_news modl_news">';
		$page->moduleHeader('News', '');
		echo '<ul class="panel">';
		echo '<li class="link_menu"><a href="index.php?action=display&id_type=all">'._('Display all').'</a></li>';
		echo '<li class="link_menu"><a href="index.php?action=add" >'._('Submit a news').'</a></li>';
		echo '</ul>';
		$page->adminLink('module', 'module_news', $this->config['admin_rights']);
		$page->moduleFooter();
		echo '</div>';
	}

	public function displayTypes() {
		global $db, $page;
		if (!isset ($page))
			$page = new Page();
		echo '<div class="types_news modr_news">';
		$page->moduleHeader(_('Categories'), '');
		$result = $db->query("SELECT idCategory as id,name,image 
							FROM kernel_tag_category_user categ 
							WHERE categ.login='".$this->login."'
								AND categ.isShow='true'");
		echo '<ul class="panel"><li class="link_menu"><a href="index.php?action=display&id_type=all" >'._('Display all categories').'</a></li>';
		while ($row = $db->fetchArray($result)) {
			echo '<li class="link_menu"><a href="' . $_SERVER['PHP_SELF'] . '?action=display&id_type=' . $row['id'] . '" title="' . $row['name'] . '">'; 
			echo ' ' . stripslashes($row['name']) . '</a></li>';
		}
		$page->moduleFooter();
		echo '</div>';
	}

	public function displayNews($info, $classe) {
		global $core, $db;
		$core->loadClass('date');
		$date = new Date();
		$info['id_news']=(int)$info['id_news'];
		$nb = $db->result($db->query("SELECT COUNT(DISTINCT(login)) FROM news_read WHERE id_news='" . $info['id_news'] . "'"), 0);
		
		$info['type'] = stripslashes($info['type']);
		$first_letter = substr($info['type'], 0, 1);
		$end_title = substr($info['type'], 1, 120);
		echo '<div class="row row' . $classe . '">
						
						<div class="titre' . $classe . '" name="event' . $info['id_news'] . '">
							<span class="titre_menu">
								<span>'.$first_letter.'</span>'.$end_title.' : '.stripslashes($info['titre']).'
							</span>
						</div>
						
			   		<div class="infos_creation_news infos' . $classe . '">
							<span>Posté par : ' . stripslashes($info['auteur']) . ' le ' . $date->formatDate($info['date_creation'], "d/m/Y") . ' - vu par <b>' . $nb . '</b> personne'.($nb > 1 ? 's' : '').'</span>
						</div>
						
			 			<div class="text_block">
							<div class="item_picture">
								<img src="'.$core->makeUrl('data/files/categ_img/'). $info['image'] . '" alt="' . stripslashes($info['type']) . '" hspace="0" vspace="0" />
							</div>
							<span class="text-indent"> &nbsp; &nbsp; </span>
							<span class="text">' . nl2br(stripslashes($info['information'])) . '</span>
							<div class="text_source">Source : <span>'.stripslashes($info['source']).'</span></div>
			 			</div>
						
						<div class="clear"></div>
						
					</div>';

	}

	public function displayNewsType($id_type = 'all', $cur_page = 1) {
		global $core, $db, $page;
		if (!isset ($page))
			$page = new Page();

		if (!empty ($id_type) && $id_type != 'all') {
			$id_type=(int)$id_type;
			$sqlType = " AND categ.idCategory = $id_type ";
		} else {
			$sqlType = '';
		}

		$start_row = ($cur_page - 1) * $this->config['items_per_page'];
		
		$request_start = "SELECT 
								categ.image AS image,
								categ.name AS type,
								news_liste.id_news AS id_news,
								news_liste.date_creation AS date_creation,
								news_liste.titre AS titre,
								news_liste.information AS information,
								news_liste.auteur AS auteur,
								news_liste.origine AS source,
								IF(news_read.id_news IS NULL,'0','1') AS isRead ";
		
		$request_middle = " FROM kernel_tag_category_user categ, 
								kernel_tag_information info,
								kernel_tag_information_category info_categ,
								news_liste 
									LEFT JOIN news_read
									ON news_read.login='".$this->login."'
										AND news_read.id_news=news_liste.id_news
							WHERE news_liste.valide = 'Y'
								AND info.idOrig=news_liste.id_news
								AND info.tableOrig='news_liste'
								AND info_categ.idInformation=info.id
								AND info_categ.idCategory=categ.idCategory
								AND categ.isShow='true'
								AND categ.login='".$this->login."'
								".$sqlType;
		
		$request_end = " GROUP BY news_liste.id_news
							ORDER BY date_creation DESC ";
							
		$result = $db->query($request_start.$request_middle.$request_end." LIMIT ".$start_row.", ".$this->config['items_per_page']." ");
		$count = $db->result($db->query(" SELECT COUNT(*) ".$request_middle.$request_end), 0);
		
		if ($db->numRows($result) > 0) {
			echo '<div class="clear"></div>';
			echo '<div class=""></div>';
			$page->moduleHeader('News', '');
			$i = 1;
			
			$page->htmlBlock->pagesList($request_start.$request_middle.$request_end, $this->config['items_per_page'], $cur_page, 'href', $core->makeUrl('modules/module_news/index.php').'?action=display&amp;id_type='.$id_type.'&amp;page=[#]', 'Aller à la page : ');
				
			while ($row = $db->fetchArray($result)) {
				$i++;

				if ($core->getLogin() != '') {
					if ($row['isRead'] == 0) {
						$db->query("INSERT INTO news_read (login, id_news, date_lecture) VALUES ('" . $this->login . "', " . $row['id_news'] . ", NOW())");
					}
				}
				if ($i % 2) {
					$classe = 1;
				} else {
					$classe = 2;
				}
				$this->displayNews($row, $classe);
			}

			$page->htmlBlock->pagesList($request_start.$request_middle.$request_end, $this->config['items_per_page'], $cur_page, 'href', $core->makeUrl('modules/module_news/index.php').'?action=display&amp;id_type='.$id_type.'&amp;page=[#]', 'Aller à la page : ');
				
			$page->moduleFooter();
		} else {
?> <div class="clear"></div> <?php

			$page->msgInfo(_('Nothing to display'));
		}
	}

	public function displayNewsId($id_news = '') {
		global $core, $db, $page;
		if (!isset ($page))
			$page = new Page();

		if (!empty ($id_news)) {
			$id_news=(int)$id_news;
			$sqlId = " AND news_liste.id_news = $id_news ";
		} else {
			$sqlId = '';
		}

		$result = $db->query("SELECT 
								categ.image AS image,
								categ.name AS type,
								news_liste.id_news AS id_news,
								news_liste.date_creation AS date_creation,
								news_liste.titre AS titre,
								news_liste.auteur AS auteur,
								news_liste.information AS information,
								news_liste.origine AS source,
								IF(news_read.id_news IS NULL,'0','1') As isRead
							FROM kernel_tag_category_user categ, 
								kernel_tag_information info,
								kernel_tag_information_category info_categ,
								news_liste 
									LEFT JOIN news_read
									ON news_read.login='".$this->login."'
										AND news_read.id_news=news_liste.id_news
							WHERE news_liste.valide = 'Y'
								AND info.idOrig=news_liste.id_news
								AND info.tableOrig='news_liste'
								AND info_categ.idInformation=info.id
								AND info_categ.idCategory=categ.idCategory
								AND categ.isShow='true'
								AND categ.login='".$this->login."'
								".$sqlId."
							GROUP BY news_liste.id_news
							ORDER BY date_creation DESC 
							LIMIT 0,10");
							
		if ($db->numRows($result) > 0) {
			echo '<div class="clear"></div>';
			echo '<div class=""></div>';
			$page->moduleHeader(_('News'), '');
			$i = 1;
			while ($row = $db->fetchArray($result)) {
				$i++;
				if ($core->getLogin() != '') {
					if ($row['isRead'] == 0) {
						$db->query("INSERT INTO news_read (login, id_news, date_lecture) VALUES ('" . $this->login . "', " . $row['id_news'] . ", NOW())");
					}
				}
				if ($i % 2) {
					$classe = 1;
				} else {
					$classe = 2;
				}
				$this->displayNews($row, $classe);
			}
			$page->moduleFooter();
		} else {
?> <div class="clear"></div> <?php

			$page->msgInfo(_('Nothing to display'));
		}

	}


	public function saveNews($frm){
		global $core, $db, $page;
  	$core->loadClass('date');
		$core->loadClass('tag',dirname(__FILE__).'/../include/engine/tag');
		$core->loadClass('tagInterface',dirname(__FILE__).'/../include/engine/tag');
  	$date = new Date();
		$tag = new Tag(Array('news_liste'=>'id'), $core, $db);

		$errors = Array();

		//On insere un éventuel nouvel évenement
		
		if (!isset($frm['titre']) || $frm['titre'] == ''){
			$errors[] = _('Please fill in the tittle field.');
		}
		if (!isset($frm['information']) || $frm['information'] == '' ){
			$errors[] = _('Please fill in the content field for this news.');
		}
		
		if(!isset($frm['category']) || $frm['category'] == '' || $frm['category'] == 0){
			$errors[] = _('Please fill in the category field');
		}
			
		if(count($errors) == 0){
	    	//on insère dans la base

  	  	$res = $db->query("INSERT INTO news_liste (valide, auteur_login, date_creation, auteur, origine, titre, information) ".
					" VALUES( ".
						" '". (($core->verifDroits($this->config['admin_rights']))?'Y':$this->config['auto_valid'])."', ".
						" '".$core->getUserId()."' , ".
						" NOW(), ".
						" '".$db->escapeString(htmlentities($frm['auteur'], ENT_QUOTES, 'UTF-8'))."' , ".
						" '".$db->escapeString(htmlentities($frm['origine'], ENT_QUOTES, 'UTF-8'))."' , ".
						" '".$db->escapeString(htmlentities($frm['titre'], ENT_QUOTES, 'UTF-8'))."' , ".
						" '".$db->escapeString(htmlentities($frm['information'], ENT_QUOTES, 'UTF-8'))."' ".
					" )"
				);
			
				$last_insert_id = $db->insertId();
				$idInformation = $tag->getIdInformation('news_liste',$last_insert_id);
				$tag->setInformationCategory($idInformation, $frm['category']);
	
    			//on affiche le message de confirmation
					if($core->verifDroits($this->config['admin_rights']) || $this->config['auto_valid'] == 'Y'){
	  	 	 		$page->msgInfo(locale::display('news_recorded_webmaster','events'));  
					}else{
						$page->msgInfo(locale::display('news_recorded','events'));
					}

					$this->displayHeader();
					$this->displayTypes();
					$this->displayNewsType('all', 1);
					
			}else{
				$error_msg = '<p>'._('error').'</p><ul>';
				foreach($errors as $er){
					$error_msg .= '<li>'.$er.'</li>';
				}
				$error_msg .= '</ul>';
				
				$page->msgError($error_msg);
				$this->displayHeader();
				$this->displayTypes();
				$this->displayAddForm($frm);
			}
			
	}


	public function displayAddForm($frm = '') {
		global $core,$db,$page;
		if(!isset($page))$page = new Page();

		$val = (is_array($frm));
		
		echo '<div class="clear"></div>';
		$page->moduleHeader(_('Add a news'));
		
		echo '
		<form action="index.php?action=save" method="post">
        <p class="form-input"><label>'._('Categories').' *° :</label>
        <select multiple="multiple" name="category[]" size="3">';
				
    $result = $db->query("SELECT categ.idCategory as id, categ.name as name FROM kernel_tag_category_user_set categ WHERE categ.login = '".(int)$core->getUserId()."'");
		
    while ($row=$db->fetchArray($result)) {
			$sel = '';
			if(isset($frm['category']) && in_array($row['id'], $frm['category']) ){
				$sel = ' selected="selected" ';
			}
      echo '<option value="'.$row['id'].'" '.$sel.' >'.stripslashes($row['name']).'</option>';
    }
	  
    echo '
        </select></p>
        <p class="form-input"><label>'._('Title').' * :</label><input type=text name="titre" size="30"  value="'.( ($val && isset($frm['titre'])) ? $frm['titre'] : '' ).'" onblur="validateInput(this, \'SOMETHING\');" /></p>
        <p class="form-input"><label>'._('Content').' * :</label><textarea cols="27" rows="7" name="information" onblur="validateInput(this, \'SOMETHING\');">'.( ($val && isset($frm['information'])) ? $frm['information'] : '' ).'</textarea></p>';

		$res=$db->query("SELECT firstname, lastname FROM trombi_trombi WHERE id_user = '".$core->getUserId()."' LIMIT 1");
		$row=$db->fetchArray($res);

		echo '
        <p class="form-input"><label>'._('Author').' :</label><input type="text" name="auteur" size="30" value="'.( ($val && isset($frm['auteur'])) ? $frm['auteur'] : stripslashes($row['firstname']).' '.stripslashes($row['lastname']) ).'" /></p>
        <p class="form-input"><label>'._('Source').' :</label>
        	<input type="text" name="origine" size="30" value="'.( ($val && isset($frm['origine'])) ? $frm['origine'] : '' ).'" />
				</p>
        <p class="form-submit"><input type="submit" value="'._('Create').'" /></p>
	 			<p>° If no categories fit your needs, contact the <a href="mailto:'.$core->getConfig('mailWebmaster').'">webmaster</a></p>
		</form>';

	$page->moduleFooter();
	}


}
?>
