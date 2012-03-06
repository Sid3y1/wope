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


/******************* module_news_admin.class **********************************
Definition pour l'admin du module de news
**********************************************************************/
class module_news_admin {
	
	private $id;

	private $path;

	private $config;

	public final function __construct()
	{
		global $core;
		$this->path = 'modules/module_news/admin';
		
		include($core->getConfig('baseServer').'/'.$this->path.'/config_admin_module_news.inc');
		include($core->getConfig('baseServer').'/'.$this->path.'/../config_module_news.inc');
		$this->config = array_merge($config_module_news, $config_admin_module_news);
		
		$this->id = rand(1000000,9999999);
    $_SESSION['admin_news_'.$this->id] = serialize($this);
	}
						
	public function displayContent($news = null) {
		global $core,$db,$page;
		if(!isset($page))$page= new Page();

		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class = 'module_news' "), 0);
		$page->moduleHeader('Module '.$title,'');
		echo '<div id="module_news_admin_content">';
		if( $news != null){
			$this->displayUpdateNews($news);
		}else{
			$this->displayNews();
		}
		echo '</div>';
		$page->moduleFooter();
	}

	public function displayMenu() {
		global $core,$db,$page;
		if(!isset($page))$page= new Page();
		
		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class = 'module_news' "), 0);
		$page->moduleHeader('Module '.$title,'');

		echo '<ul class="panel">
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_news/admin/ajax/content.php').'?id_admin='.$this->id.'&content=display_news\',\'module_news_admin_content\')">News</a></li>
						<li class="link_menu"><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_news/admin/ajax/content.php').'?id_admin='.$this->id.'&content=add_news\',\'module_news_admin_content\')">Ajouter une news</a></li>
					</ul>';
		$page->moduleFooter();
	}

	public function displaySummary($news_nb, $period){
		global $page, $core, $db;
		if(!isset($page))$page = new Page();

		$result = $db->query("SELECT id_news, titre FROM news_liste WHERE valide='N' AND DATEDIFF(NOW(), date_creation ) <= 7 ORDER BY date_creation ASC LIMIT ".$news_nb." ");
		$nb = $db->numRows($result);

		$page->moduleHeader('Actualités à modérer ( '.$nb.' )', '');
		echo '<h4>Dernières actualités à modérer :</h4>';
		
		if($db->numRows($result) != 0){
			$i = 0;
			$line_style = '';
			while($row = $db->fetchArray($result)) {
				if ($i%2) {
					$line_style = 'line_1';
				} else {
						$line_style = 'line_2';
					}
				echo '<div class="admin_home_item_list '.$line_style.'"><a href="'.$core->makeUrl('applications/admin/index.php').'?part=module&amp;content=module_news&amp;option='.$row['id_news'].'">'.$row['titre'].'</div>';
				$i++;
			}
		}else{
			echo '<div><i>Aucune actualité à modérer ces '.$period.' derniers jours.</i></div>';
		}
		
		echo '<div class="end_list_clear"></div><div>';
		$page->adminLink('module', 'module_news', $this->config['admin_rights']);
		echo '</div>';
		
		$page->moduleFooter();
	}


	public function displayNews($cur_page = 1){
		global $core, $db, $page;
		if(!isset($page))$page = new Page();
		$core->loadClass('box');
		$core->loadClass('date');
		$box = new Box();
		$date = new Date();
		
		$start_row = ($cur_page - 1) * $this->config['items_per_page'];
		
	
		$request = "SELECT news_liste.*,
									categ.name AS type,
									categ.image as image,
									categ.name as type
								FROM news_liste,
									kernel_tag_information info
									LEFT JOIN
										(kernel_tag_information_category info_categ
										JOIN kernel_tag_category_user categ)
										ON ( info.id = info_categ.idInformation
											AND categ.idCategory = info_categ.idCategory
										) 
								WHERE
									info.tableOrig='news_liste'
									AND info.idOrig=news_liste.id_news
								GROUP BY news_liste.id_news
								ORDER BY date_creation DESC ";

		$result = $db->query($request." LIMIT ".$start_row.", ".$this->config['items_per_page']." ");

		$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/content.php').'?id_admin='.$this->id.'&amp;content=display_news&amp;page=[#]\',\'module_news_admin_content\')');
								
		echo '
		<table align="center" class="data_table" width="95%">
		<tr>
	 		<th width="25%">Date</th>
	 		<th width="30%">Titre</th>
	 		<th width="15%">Type</th>
	 		<th width="15%">Validation</th>
	 		<th width="15%">Suppression</th>
		</tr>
		';
	
		while ($row = $db->fetchArray($result)) {
			$class='';
	   if ($row['valide']=='N') {
	      $class=' class="no_valid" ';
	   }

     $news_date = $date->reverseDate(substr($row['date_creation'],0,10));
		 $news_hour = substr($row['date_creation'],11,2);
		 $news_min = substr($row['date_creation'],14,2);			
	   echo '<tr '.$class.' >
	    <td align="center"><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_news/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_news&id_news='.$row['id_news'].'\',\'module_news_admin_content\')">'.$news_date.' à '.$news_hour.'h'.$news_min.'</a></td>
	    <td><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_news/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_news&id_news='.$row['id_news'].'\',\'module_news_admin_content\')">'.stripslashes($row['titre']).'</a></td>
	    <td align="center" ><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('modules/module_news/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_news&id_news='.$row['id_news'].'\',\'module_news_admin_content\')">';
			if($row['type'] != ''){
				echo '<img alt="Icône '.stripslashes($row['type']).'" src="'.$core->makeUrl('data/files/categ_img/'.$row['image']).'">';
			}
			echo '</a></td>
	    <td align="center">';

	   if ($row['valide']=='N') {
	      echo '<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_news/admin/ajax/content.php').'?id_admin='.$this->id.'&content=validate_news&id_news='.$row['id_news'].'\',\'module_news_admin_content\')">Valider</a>';
	   } else {
              echo '<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_news/admin/ajax/content.php').'?id_admin='.$this->id.'&content=cancel_news&id_news='.$row['id_news'].'\',\'module_news_admin_content\')">Retirer</a>';
	   }
	        	    
	    echo '</td>
	    <td align="center"><a class="trash" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_news/admin/ajax/content.php').'?id_admin='.$this->id.'&content=del_news&id_news='.$row['id_news'].'\',\'module_news_admin_content\')', 'Etes-vous sûr de vouloir supprimer cette news ?').'"></a></td>

  		</tr>';
		}
		echo '
		</table>';
	
		$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/content.php').'?id_admin='.$this->id.'&amp;content=display_news&amp;page=[#]\',\'module_news_admin_content\')');
	}


	public function displayUpdateNews($id_news){
		global $core,$db,$page;
		$core->loadClass('texteditor');
		
		$res = $db->query("SELECT auteur, origine, titre, information FROM news_liste WHERE id_news='".$id_news."'");
		
		$result = $db->query("SELECT
										        categ.id,
														categ.name,
												  	IF(info.id IS NULL, 'false', 'true') as selected 
													FROM kernel_tag_category categ
														LEFT JOIN (kernel_tag_information info
															INNER JOIN kernel_tag_information_category info_categ)
														ON (categ.id = info_categ.idCategory
															AND info.tableOrig='news_liste'
															AND info.idOrig='".$id_news."'
															AND info.id=info_categ.idInformation)
		" );
																																	 
		$row = $db->fetchArray($res);
		
		echo '<form action="'.$core->makeUrl('modules/module_news/admin/ajax/content.php').'" onsubmit="AJAX.submitAndUpdate(this,true,\'module_news_admin_content\');return false">
				<div class="input-hidden">
					<input type="hidden" name="id_admin" value="'.$this->id.'" />
					<input type="hidden" name="content" value="save_news" />
					<input type="hidden" name="news_save_case" value="update" />
					<input type="hidden" name="id_news" value="'.$id_news.'" />
				</div>';
			
		echo '<p class="form-input"><label for="category">Type de news * :</label>';
		echo '<select multiple name="category[]" size="3">';
		while ($row_select = $db->fetchArray($result)) {
      if($row_select['selected'] == 'true' ) {
				$sel = 'selected="selected"';
			} else {
			  $sel ='';
			}
			echo '<option value="'.$row_select['id'].'" '.$sel.' >'.stripslashes($row_select['name']).'</option>';
		}
		echo '</select>
			</p>
			<p class="form-input">
				<label for="news_title" >Intitulé * :</label>
				<input type="text" name="news_title" size="40" value="'.stripslashes($row['titre']).'" />
			</p>
			<p class="form-textarea">
				<label for="news_description">Information * :</label>
			';
		$te = new TextEditor(
			'news_description',
			Array(
				'rows'=>12,
				'cols'=>50,
				'value'=>stripslashes($row['information'])
			)
		);
		$te->display();
		echo '
			</p>
			<p class="form-input">
				<label for="news_source">Source :</label>
				<input type="text" name="news_source" value="'.stripslashes($row['origine']).'" />
			</p>
			<p class="form-input">
				<label for="news_author">Auteur :</label>
				<input type="text" name="news_author" value="'.stripslashes($row['auteur']).'" />
			</p>
			<p class="form-submit">
				<input type="submit" value="Modifier" />
			</p>
		</form>';
	}

	public function displayAddNews(){
		global $core,$db;
		$core->loadClass('texteditor');
	
		$name_result = $db->query("SELECT firstname, lastname FROM trombi_trombi WHERE id_user = '" .$core->getUserId(). "'");
    while ($row = $db->fetchArray($name_result)) {
			$author = stripslashes($row['firstname'].' '.$row['lastname']);
		}
		
		$result = $db->query("SELECT id, name
													FROM kernel_tag_category");
	
			echo '<form action="'.$core->makeUrl('modules/module_news/admin/ajax/content.php').'" onsubmit="AJAX.submitAndUpdate(this,true,\'module_news_admin_content\');return false">
				<div class="input-hidden">
					<input type="hidden" name="id_admin" value="'.$this->id.'" />
					<input type="hidden" name="content" value="save_news" />
					<input type="hidden" name="news_save_case" value="add" />
				</div>';
			
			echo '<p class="form-input"><label for="category">Type de news * :</label>';
			echo '<select multiple name="category[]" size="3">';
			while ($row_select = $db->fetchArray($result)) {
				echo '<option value="'.$row_select['id'].'">'.stripslashes($row_select['name']).'</option>';
			}
			echo '</select>
			</p>
			<p class="form-input">
				<label for="news_title" >Intitulé * :</label>
				<input type="text" name="news_title" size="40" />
			</p>
			<p class="form-textarea">
				<label for="news_description">Information * :</label>
			';
			$te = new TextEditor(
				'news_description',
				Array(
					'rows'=>12,
					'cols'=>50,
				)
			);
			$te->display();
			echo '
			</p>
			<p class="form-input">
				<label for="news_source">Source :</label>
				<input type="text" name="news_source" />
			</p>
			<p class="form-input">
				<label for="news_author">Auteur :</label>
				<input type="text" name="news_author" value="'.$author.'" />
			</p>
			<p class="form-submit">
					<input type="submit" value="Ajouter" /></p>
			</form>';
	}

	
	public function saveNews($infos){
		global $core, $db;
    $core->loadClass('tag',dirname(__FILE__).'/../include/engine/tag');
	  $core->loadClass('tagInterface',dirname(__FILE__).'/../include/engine/tag');
    $tag = new Tag(array('news_liste' => 'id_news'),$core,$db);
		
		switch($infos['news_save_case']){
		
			case 'add':
				$db->query("INSERT INTO news_liste (auteur_login, date_creation, titre, information, auteur, valide, origine) VALUES (
					'".$core->getUserId()."', 
					NOW(),
					'".$db->escapeString(htmlentities($infos['news_title'], ENT_QUOTES, 'UTF-8'))."',
					'".$db->escapeString($infos['news_description'])."',
					'".$db->escapeString(htmlentities($infos['news_author'], ENT_QUOTES, 'UTF-8'))."',
					'Y',
					'".$db->escapeString(htmlentities($infos['news_source'], ENT_QUOTES, 'UTF-8'))."'
				)");
			
				$id = $db->insertId();
				$idInformation = $tag->getIdInformation('news_liste',$id);
				$tag->setInformationCategory($idInformation, (isset($infos['category']) ? $infos['category'] : null));
				break;
			
			case 'update':
				$id = $infos['id_news'];
 	 	 		$idInformation = $tag->getIdInformation('news_liste', $id);
				$tag->setInformationCategory($idInformation, (isset($infos['category']) ? $infos['category'] : null));
			
    		$db->query("UPDATE news_liste SET 
					titre = '".$db->escapeString(htmlentities($infos['news_title'], ENT_QUOTES, 'UTF-8'))."',
					information = '".$db->escapeString($infos['news_description'])."',
					auteur = '".$db->escapeString(htmlentities($infos['news_author'], ENT_QUOTES, 'UTF-8'))."',
					origine = '".$db->escapeString(htmlentities($infos['news_source'], ENT_QUOTES, 'UTF-8'))."'
					WHERE id_news = '".$infos['id_news']."'
				");
				break;
			
		default:
			//void
			break;
		}

		$this->displayNews();
			
	}
	

	public function deleteNews($id_news){
		global $db;
		$db->query("DELETE FROM news_liste WHERE id_news='".$id_news."'");
	}

	
	public function validateNews($id_news,$valid){
		global $db;
		$db->query("UPDATE news_liste SET valide='".$valid."' WHERE id_news='".$id_news."'");
	}
	
}
?>
