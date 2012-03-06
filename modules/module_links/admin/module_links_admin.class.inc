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


class module_links_admin {
	
	private $id;

	public final function __construct()
	{
		$this->id = rand(1000000,9999999);
    $_SESSION['admin_links_'.$this->id] = serialize($this);
	}



	public function displayContent() {
		global $core,$db,$page;
		if(!isset($page))$page= new Page();
		
		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class='module_links' "), 0);
		$page->moduleHeader(ucfirst($title), '');
		echo '<div id="module_links_admin_content">';
		$this->displayLinks();
		echo '</div>';
		$page->moduleFooter();
	}


	public function displayMenu() {
		global $core,$page,$db;
		if(!isset($page))$page= new Page();

		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class='module_links' "), 0);
		$page->moduleHeader(ucfirst($title), '');
		echo '<ul class="panel">
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&content=display_links\',\'module_links_admin_content\')">Listes des liens</a></li>';
		echo '	<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&content=add_link\',\'module_links_admin_content\')">Ajouter un lien</a></li>
					</ul>';
		$page->moduleFooter();
	}



	public function displayLinks(){
		global $core,$db, $page;
		if(!isset($page))$page = new Page();
		$core->loadClass('box');
		$box = new Box();
		
		echo '<table align="center" class="data_table">';
		echo '<tr><th width="10%">Nom</th><th width="25%">Lien url</th><th width="20%">Catégorie(s)</th><th width="5%">Visible</th><th width="5%">Suppression</th></tr>';
		
		$res = $db->query("SELECT 
													ml.id, 
													ml.name, 
													ml.url, 
													ml.description, 
													ml.date, 
													ml.valid,
													categ.name AS type,
													categ.image AS image
													
													FROM module_links ml
														LEFT JOIN ( kernel_tag_information info
															LEFT JOIN ( kernel_tag_information_category info_categ
																INNER JOIN kernel_tag_category categ
																ON info_categ.idCategory = categ.id )
															ON	info.id = info_categ.idInformation )
														ON ml.id = info.idOrig
													
													WHERE info.tableOrig = 'module_links'

												UNION SELECT
													ml2.id, 
													ml2.name, 
													ml2.url, 
													ml2.description, 
													ml2.date, 
													ml2.valid,
													'' AS type,
													'' AS image

													FROM module_links ml2

													WHERE  ml2.id NOT IN (SELECT idOrig FROM kernel_tag_information WHERE tableOrig = 'module_links')

													GROUP BY id
													ORDER BY id DESC");
		
		while ($row = $db->fetchArray($res)){

		$class='';
		if ($row['valid']=='N') {
			$class = 'class="no_active"';
		}
									
    if (strlen(stripslashes($row['url'])) > 45) {
    	$link_url = substr(stripslashes($row['url']),0,42).'...';
    } else {
    		$link_url = stripslashes($row['url']);
    	}																																							
		
		$tooltip_content = $page->htmlBlock->escapeTipContent(stripslashes($row['url']));
		
		echo '<tr '.$class.'>
			  			<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_link&id_link='.$row['id'].'\',\'module_links_admin_content\')">'.stripslashes($row['name']).'</a></td>
							
							<td align="left"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_link&id_link='.$row['id'].'\',\'module_links_admin_content\')" onmouseover="activateToolTips(this,\''.$tooltip_content.'\');">'.$link_url.'</a></td>

							<td align="left"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_link&id_link='.$row['id'].'\',\'module_links_admin_content\')">'.stripslashes($row['type']).'</a></td>';


if ($row['valid']=='N') {
		echo '<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&content=validate_link&id_link='.$row['id'].'\',\'module_links_admin_content\')">N</a></td>';
			} else {
						echo '<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&content=cancel_link&id_link='.$row['id'].'\',\'module_links_admin_content\')">Y</a></td>';
							  }

			echo'	<td align="center"><a class="trash" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&content=del_link&id_link='.$row['id'].'\',\'module_links_admin_content\');', 'Etes-vous sûr de vouloir supprimer ce lien ?').'"></a></td>
						</tr>';
		}
		echo '</table>';
}



	public function validateLink($id_link,$valide) {
		global $db;
		$db->query("UPDATE module_links SET valid='".$valide."' WHERE id='".$id_link."'");
	}
					


	public function displayUpdateLink($id_link){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		
		echo '<div id="update_ad_feedback" style="display:none" >';
		$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		
		$core->loadClass('magicform');
		$form_options = Array (

			'update' => true,
			'rowToUpdate' => Array('field'=>'id','pattern'=>$id_link),
			'button' => 'Modifier',
			'name' => '',
			'onsubmit' => 'AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&amp;content=display_links\', \'module_links_admin_content\');',
			'only' => Array(
					'name',
					'url',
					'description',
			),

			'legend' => Array ('url' => 'Url du lien :', 'name' => 'Nom du lien :','description' => 'Description :'),
			'message' => 'Lien enregistré',
			'once' => true,
			'onsubmit' => 'AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&content=display_links\',\'module_links_admin_content\');',
			'feedback' => 'info',
			'describe' => Array (

				'id' => Array("type"=>"int(10)", "vide"=>"NO", "key"=>"1", "default"=>"", "extra"=>"auto_increment"),
				'name' => Array("type"=>"varchar(200)", "vide"=>"", "key"=>"", "default"=>"", "extra"=>"", "size"=>"200"),
				'url' => Array("type"=>"varchar(200)", "vide"=>"NO", "key"=>"", "default"=>"", "extra"=>"", "size"=>"200"),
				'description' => Array("type"=>"text", "vide"=>"YES", "key"=>"", "default"=>"NULL", "extra"=>""),
				'date' => Array("type"=>"date", "vide"=>"", "key"=>"", "default"=>"", "extra"=>""),
				'valid' => Array("type"=>"enum('Y','N')", "vide"=>"NO", "key"=>"", "default"=>"Y", "extra"=>""),
					
			), 
			
			'analyse' => false,
			'category' => Array ('legend' => 'Type de lien * :', 'position' => 2)

			);

			$mf = new MagicForm('module_links',$form_options);
			$mf->display();

			echo '<p>* Si vous ne trouvez pas de catégorie adéquate, contactez le <a href="mailto:' . $core->getConfig('mailWebmaster') . '">webmaster</a> pour la rajouter.</p>';


	}

	public function displayAddLink(){
		global $core,$db,$page;
		
  	$core->loadClass('magicform');

		$date = date('Y-m-d');

		if(!isset($page))$page = new Page();
									
		echo '<div id="add_link_feedback" style="display:none" >';
		$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';

		$form_options = Array (
				'update' => false,
				'button' => 'Ajouter',
				'name' => 'Nouveau lien utile',
				'onsubmit' => 'AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_links/admin/ajax/content.php').'?id_admin='.$this->id.'&amp;content=display_links\',\'module_links_admin_content\');',

				'only' => Array (
						'name',
						'date',
						'url',
						'description',
				),

				'legend' => Array (
						'url' => 'Url du lien :',
						'name' => 'Nom du lien :',
						'description' => 'Description :'
				),

				'category' => Array (
					'legend' => 'Type de lien* :',
					'position' => 2
				),

				'message' => 'Lien enregistré',
				'once' => true,
				'feedback' => 'info',

	      'forceType'=>Array(
		        'date'=>Array('type'=>'hidden', 'value'=>$date)
	      ),

				'forceValue'=>Array('url'=>Array('value'=>'http://')),

				'describe' => Array (

					'id' => Array("type"=>"int(10)", "vide"=>"NO", "key"=>"1", "default"=>"", "extra"=>"auto_increment"),
					'name' => Array("type"=>"varchar(60)", "vide"=>"", "key"=>"", "default"=>"", "extra"=>""),
					'url' => Array("type"=>"varchar(60)", "vide"=>"NO", "key"=>"", "default"=>"", "extra"=>""),
					'description' => Array("type"=>"text", "vide"=>"YES", "key"=>"", "default"=>"NULL", "extra"=>""),
					'date' => Array("type"=>"date", "vide"=>"", "key"=>"", "default"=>"", "extra"=>""),
					'valid' => Array("type"=>"enum('Y','N')", "vide"=>"NO", "key"=>"", "default"=>"Y", "extra"=>""),
					
				), 'analyse' => false);

				$mf = new MagicForm('module_links', $form_options);

				$mf->display();
				echo '<p>* Si vous ne trouvez pas de catégorie adéquate, contactez le <a href="mailto:' . $core->getConfig('mailWebmaster') . '">webmaster</a> pour la rajouter.</p>';
						 
	}




	public function deleteLink($id_link){
		global $db;
				$db->query("DELETE FROM module_links WHERE id='".$id_link."'");
	}


	
	public function saveLink($infos){
		global $core, $db;
		
		switch($infos['link_save_case']){
		
			case 'add':
				$db->query("INSERT INTO module_links VALUES ('','".$infos['link_name']."','".$infos['link_url']."','".$infos['link_description']."',NOW(),'".$infos['link_valid']."')");
			break;
			
			case 'update':
    		$db->query("UPDATE module_links SET name='".$infos['link_name']."', url='".$infos['link_url']."', description='".$infos['link_description']."' WHERE id='".$infos['id_link']."'");
			break;
			
			default : break;
		}


	}


}

?>
