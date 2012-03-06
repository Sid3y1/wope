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


class module_mpub_admin {
	
	private $id;

	public final function __construct()
	{
		$this->id = rand(1000000,9999999);
    $_SESSION['admin_mpub_'.$this->id] = serialize($this);
	}




	public function displayContent() {
		global $core,$page;
		if(!isset($page))$page= new Page();
		$page->moduleHeader('Bandeau de pub','');
		echo '<div id="module_mpub_admin_content">';
		$this->displayAds();
		echo '</div>';
		$page->moduleFooter();
	}



	public function displayMenu() {
		global $core,$page;
		if(!isset($page))$page= new Page();
		$page->moduleHeader('Bandeau de Publicité : Menu','');
		echo '<ul class="panel">
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'?id_admin='.$this->id.'&content=display_ads\',\'module_mpub_admin_content\')">Listes des publicités : modification & suppression</a></li>';
		echo '	<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'?id_admin='.$this->id.'&content=add_ad\',\'module_mpub_admin_content\')">Ajouter une publicité</a></li>
					</ul>';
		$page->moduleFooter();
	}



	public function displayAds(){
		global $core,$db;
		$core->loadClass('box');
		$box = new Box();
		
		echo '<table align="center" class="data_table">';
		echo '<tr>'.
				 		'<th width="10%">Titre</th><th width="25%">Lien hypertexte</th><th width="5%">Valide ?</th><th width="10%">Emplacement</th><th width="5%">Suppression</th>'.
				 '</tr>';
		
		$res = $db->query("SELECT id, nom, link, valide, place, image FROM module_mpub ORDER BY id DESC");
		
		while ($row = $db->fetchArray($res)){
			$class='';
		if ($row['valide']=='N') {
			$class = 'class="no_active"';
		}
			
			echo '<tr '.$class.' >
			  			<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_ad&id_ad='.$row['id'].'\',\'module_mpub_admin_content\')">'.$row['nom'].'</a></td>
							<td align="left"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_ad&id_ad='.$row['id'].'\',\'module_mpub_admin_content\')">'.$row['link'].'</a></td>';

	if ($row['valide']=='N') {
		echo '<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'?id_admin='.$this->id.'&content=validate_pub&id_pub='.$row['id'].'\',\'module_mpub_admin_content\')">N</a></td>';
	} else {
			echo '<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'?id_admin='.$this->id.'&content=cancel_pub&id_pub='.$row['id'].'\',\'module_mpub_admin_content\')">Y</a></td>';
	  }

  		echo '<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_ad&id_ad='.$row['id'].'\',\'module_mpub_admin_content\')">'.$row['place'].'</a></td>';

			echo'
						<td align="center"><a class="trash" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'?id_admin='.$this->id.'&content=del_ad&id_ad='.$row['id'].'\',\'module_mpub_admin_content\')', 'Etes-vous sûr de vouloir supprimer cette publicité ?').'"></a></td>
						</tr>';
		}
		echo '</table>';
}

	public function validatePub($id_pub,$valide){
		global $db;
		$db->query("UPDATE module_mpub SET valide='".$valide."' WHERE id='".$id_pub."'");
	}


	public function displayUpdateAd($id_ad){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		
		echo '<div id="update_ad_feedback" style="display:none" >';
			$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		
		$res = $db->query("SELECT id, nom, link, valide, image FROM module_mpub WHERE id='".$id_ad."'");
		while($row = $db->fetchArray($res)){
			echo '<form enctype="multipart/form-data"  action="'.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'" onsubmit="'.
				'display(gE(\'update_ad_feedback\'));'.
				'AJAX.submitAndUpdate(this, true, \'module_mpub_admin_content\');'.
				'return false;'.
			'" >';
			echo '<input type="hidden" name="id_admin" value="'.$this->id.'" />';
			echo '<input type="hidden" name="content" value="save_ad" />';
			echo '<input type="hidden" name="ad_valid" value="'.$row['valide'].'" />';
			echo '<input type="hidden" name="ad_place" value="'.$row['place'].'" />';
			echo '<input type="hidden" name="ad_save_case" value="update" />';
			echo '<input type="hidden" name="id_ad" value="'.$id_ad.'" />';
			echo '<p class="form-input"><label>Nom :</label><input type="text" name="ad_name" size="20" value="'.$row['nom'].'" /></p>';
			echo '<p class="form-input"><label>Lien Hypertexte :</label><input type="text" name="ad_hypertext" size="60" value="'.$row['link'].'" /></p>';

			if(isset($row['image']) && $row['image']!='' ){
			  echo '<label>Image actuelle de '.$row['nom'].': </label><br /><br /><img alt="Image de '.$row['nom'].'" src="'.$core->makeUrl('modules/module_mpub/images').'/'.$row['image'].'" /></p><br />
			  <p class="form-input"><label class="for-checkbox">Supprimer l\'image : </label><input type="checkbox" name="del_ad_img" value="yes" /></p><br /><p class="form-input"><label>Nouvelle image :</label>';
			}
			echo '<input type="file" name="ad_image" size="19" /></p><br />';
			echo '<p class="form-submit"><input type="submit" value="Modifier" /></p>
			</form>';
		}

	}





	public function displayAddAd(){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
									
		echo '<div id="add_ad_feedback" style="display:none" >';
		$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_mpub/admin/ajax/content.php').'" onsubmit="'.
			'display(gE(\'add_ad_feedback\'));'.
			'AJAX.submitAndUpdate(this, true, \'module_mpub_admin_content\');'.
			'return false;'.
		'" >';
		echo '<input type="hidden" name="id_admin" value="'.$this->id.'" />';
		echo '<input type="hidden" name="content" value="save_ad" />';
		echo '<input type="hidden" name="ad_save_case" value="add" />';
		echo '<input type="hidden" name="ad_place" value="'.$row['place'].'" />';
		echo '<p class="form-input"><label>Nom :</label><input type="text" name="ad_name" size="20" value="'.$row['nom'].'" /></p>';
		echo '<p class="form-input"><label>Lien Hypertexte :</label><input type="text" name="ad_hypertext" size="60" value="'.$row['link'].'" /></p>';
		echo '<p class="form-input"><label>Valide :</label><select name="ad_valid"><option value="Y">Oui</option><option value="N">Non</option></select></p>';
		echo '<p class="form-input"><label>Emplacement :</label><select name="ad_place"><option value="top">Côté &nbsp;&nbsp; (dimensions maximales : 220px x 320px)</option><option value="bottom">Footer (dimensions maximales : 600px x 110px)</option></select></p>';
		echo '<p class="form-input"><label>Image associée :</label><input type="file" name="ad_image" size="19" /></p>';

		echo '<p class="form-submit"><input type="submit" value="Ajouter" /></p>
		</form>';
	
	}





	public function deleteAd($id_ad){
		global $db;
		$res = $db->query("SELECT image FROM module_mpub WHERE id='".$id_ad."'");

		while($img = $db->fetchArray($res)){
			$f = '../images/'.$img['image'];
			if(is_file($f))	unlink($f); 
		{
				$db->query("DELETE FROM module_mpub WHERE id='".$id_ad."'");
			}
		}
		
	}



	
	public function saveAd($infos, $image=''){
		global $core, $db;
    $core->loadClass('file');
    $file = new file();
	
		$img_id = rand();
		
		switch($infos['ad_save_case']){
		
			case 'add':
				$db->query("INSERT INTO module_mpub VALUES ('','".$infos['ad_name']."','".$infos['ad_hypertext']."','".$infos['ad_valid']."', '".$infos['ad_place']."', '".((isset($image['name']))?$img_id.'.jpg':'')."')");
			break;
			
			case 'update':
				if((isset($image['name']) && $image['name']!='') || (isset($infos['del_ad_img']) && $infos['del_ad_img']=='yes')){
					$res = $db->query("SELECT image FROM module_mpub WHERE id='".$infos['id_ad']."'");
					while($img = $db->fetchArray($res)){
						$f = '../images/'.$img['image'];
						if(is_file($f))	unlink($f);
					}
				}
				
				if(isset($image['name']) && $image['name']!=''){
    			$db->query("UPDATE module_mpub SET nom='".$infos['ad_name']."', link='".$infos['ad_hypertext']."', valide='".$infos['ad_valid']."', image='".$img_id.".jpg' WHERE id='".$infos['id_ad']."'");
				} 
				else{
    			$db->query("UPDATE module_mpub SET nom='".$infos['ad_name']."', link='".$infos['ad_hypertext']."', valide='".$infos['ad_valid']."', image=".(($infos['del_ad_img']=='yes')?"NULL":"image")." WHERE id='".$infos['id_ad']."'");
				}
			break;
			
			default : break;
		}

		if(isset($image) && $image['name']!=''){
	 		$url = $core->getConfig('baseServer').'/modules/module_mpub/images/'.$img_id.'.jpg';
	 		switch($image['error']) {
	    	case 0:
	       	$size = getimagesize($image['tmp_name']);
					//type d'image : gif, jpeg, png acceptes
	       	if ($size[2] >= 1 && $size[2] <= 3) {
		  			//On enregistre l'image
		  			if(move_uploaded_file($image['tmp_name'], $url)){
             
            	 //On redimensionne l'image grace à la classe file
							 switch ($infos['place']) {
            		
								case 'top' :
									if ($file->image_max_size($url,202,302) == false) {
										$page->msgError(locale::display('img_error','event'));
									}
									break;

								case 'bottom' :
									if ($file->image_max_size($url,202,302) == false) {
										$page->msgError(locale::display('img_error','event'));
									}
									break;

							}
		  			}
	     		}
			}

		}
	}



}

?>
