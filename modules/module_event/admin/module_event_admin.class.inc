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


/******************* module_event_admin.class **********************************
Definition pour l'admin du module event
**********************************************************************/
class module_event_admin {
	
	private $id;

	public final function __construct()
	{
		$this->id = rand(1000000,9999999);
    $_SESSION['admin_event_'.$this->id] = serialize($this);
	}
						
	public function displayContent() {
		global $core,$db,$page;
		if(!isset($page))$page= new Page();

		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class = 'module_event' "), 0);
		$page->moduleHeader('Module '.$title,'');
		echo '<div id="module_event_admin_content">';
		$this->displayAds();
		echo '</div>';
		$page->moduleFooter();
	}

	public function displayMenu() {
		global $core,$db,$page;
		if(!isset($page))$page= new Page();
		
		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class = 'module_event' "), 0);
		$page->moduleHeader('Module '.$title,'');

		echo '<ul class="panel">
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_event/admin/ajax/content.php').'?id_admin='.$this->id.'&content=display_ads\',\'module_event_admin_content\')">Evénements</a></li>
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_event/admin/ajax/content.php').'?id_admin='.$this->id.'&content=add_ad\',\'module_event_admin_content\')">Ajouter un événement</a></li>
					</ul>';
					
		$page->moduleFooter();
	}

	public function displayAds(){
		global $core,$db;
		$core->loadClass('date');
		$core->loadClass('box');
		$date = new Date();
		$box = new Box();
		
		echo '<table align="center" class="data_table">';
		echo '<tr><th width="30%">Nom</th><th colspan=2 width="50%">Période de validité</th><th colspan=2 width="20%">Suppression</th></tr>
					<tr><th width="30%"></th><th width="25%">Du</th><th width="25%">Au</th><th width="20%"></th></tr>';
		$today = date("Y-m-d");
		$res = $db->query("SELECT id, nom, jr_debut, jr_fin  FROM module_event WHERE 1 ORDER BY jr_debut DESC");
//		 where (jr_debut>='$today') or (jr_debut<='$today' and jr_fin>='$today') 
		while ($row = $db->fetchArray($res)){
			$class='';
		  if ($row['jr_debut']>=$today){
				$class=' class="future_ad" ';
			}elseif($row['jr_debut']<=$today && $row['jr_fin']>=$today){
			  $class=' class="current_ad" ';
			}
			echo '<tr '.$class.' >
			  			<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_event/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_ad&id_ad='.$row['id'].'\',\'module_event_admin_content\')">'.$row['nom'].'</a></td>
				  		<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_event/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_ad&id_ad='.$row['id'].'\',\'module_event_admin_content\')">'.$date->reverseDate($row['jr_debut']).'</a></td>
					  	<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_event/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_ad&id_ad='.$row['id'].'\',\'module_event_admin_content\')">'.$date->reverseDate($row['jr_fin']).'</a></td>
							<td align="center"><a class="trash" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_event/admin/ajax/content.php').'?id_admin='.$this->id.'&content=del_ad&id_ad='.$row['id'].'\',\'module_event_admin_content\')', 'Etes-vous sûr de vouloir supprimer cet événement ?').'"></a></td>
						</tr>';
		}
		echo '</table>';
}

	public function displayUpdateAd($id_ad){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		$core->loadClass('date');
		$date = new Date();
		
		echo '<div id="update_ad_feedback" style="display:none" >';
			$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		
		$res = $db->query("SELECT nom, jr_debut, jr_fin, titre, texte, image FROM module_event WHERE id='".$id_ad."'");
		while($row = $db->fetchArray($res)){
			echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_event/admin/ajax/content.php').'" onsubmit="'.
				'display(gE(\'update_ad_feedback\'));'.
				'AJAX.submitAndUpdate(this, true, \'module_event_admin_content\');'.
				'return false;'.
			'">';
			echo '<div id="cal_container"></div>';
			echo '<input type="hidden" name="id_admin" value="'.$this->id.'" />';
			echo '<input type="hidden" name="content" value="save_ad" />';
			echo '<input type="hidden" name="ad_save_case" value="update" />';
			echo '<input type="hidden" name="id_ad" value="'.$id_ad.'" />';
			echo '<p class="form-input"><label>Nom :</label><input type="text" name="ad_name" size="20" value="'.$row['nom'].'" /></p>';
			echo '
			<p class="form-date">
				<label>Du :</label>
				<input type="text" id="date_ad_start" name="ad_start" size="10" value="'.$date->reverseDate($row['jr_debut']).'" />
				<a class="calendar_button" onclick="selectDate(\'cal_container\', '.
			  	'function(date){'.
					' gE(\'date_ad_start\').value = date; '.
					'}, true, \'FR\');'.
				'">&nbsp;</a>
			</p>';
			echo '
			<p class="form-date">
				<label>Au :</label>
				<input type="text" id="date_ad_end" name="ad_end" size="10" value="'.$date->reverseDate($row['jr_fin']).'" />
				<a class="calendar_button" onclick="selectDate(\'cal_container\', '.
			 	 'function(date){'.
					' gE(\'date_ad_end\').value = date; '.
					'}, true, \'FR\');'.
				'">&nbsp;</a>
			</p>';
			echo '<p class="form-input"><label>Intitulé :</label><input type="text" name="ad_title" size="40" value="'.$row['titre'].'" /></p>';
			echo '<p class="form-textarea"><label>Texte :</label><textarea name="ad_text" cols="50" rows="8">'.$row['texte'].'</textarea></p>';
			echo '<p class="form-input"><label>Image associée :</label>';
			if(isset($row['image']) && $row['image']!='' ){
			  echo '<img alt="Image de '.$row['nom'].'" src="'.$core->makeUrl('modules/module_event/images').'/'.$row['image'].'" /></p>
			  <p class="form-input"><label></label><input type="checkbox" name="del_ad_img" value="yes" /><span style="position: relative;"> Pas d\'image</span></p><p class="form-input"><label>Nouvelle image :</label>';
			}
			echo '<input type="file" name="ad_image" size="19" /></p>';
			echo '<p class="form-submit">
					<input type="submit" value="Modifier" />
				</p>
			</form>';
		}

	}

	public function displayAddAd(){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
									
		echo '<div id="add_ad_feedback" style="display:none" >';
		$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_event/admin/ajax/content.php').'" onsubmit="'.
			'display(gE(\'add_ad_feedback\'));'.
			'AJAX.submitAndUpdate(this, true, \'module_event_admin_content\');'.
			'return false;'.
		'">';
		echo '<div id="cal_container"></div>';
		echo '
		<div class="input-hidden">
			<input type="hidden" name="id_admin" value="'.$this->id.'" />
			<input type="hidden" name="content" value="save_ad" />
			<input type="hidden" name="ad_save_case" value="add" />
		</div>';
		echo '<p class="form-input"><label>Nom :</label><input type="text" name="ad_name" size="20" /></p>';
		echo '
		<p class="form-date">
			<label for="ad_start">Du :</label>
			<input type="text" id="date_ad_start" name="ad_start" size="10" />
			<a class="calendar_button" onclick="selectDate(\'cal_container\', '.
			  'function(date){'.
				' gE(\'date_ad_start\').value = date; '.
				'}, true, \'FR\');'.
			'">&nbsp;</a>
		</p>';
		echo '
		<p class="form-date">
			<label for="date_ad_end">Au :</label>
			<input type="text" name="ad_end" id="date_ad_end" size="10" />
			<a class="calendar_button" onclick="selectDate(\'cal_container\', '.
			  'function(date){'.
				' gE(\'date_ad_end\').value = date; '.
				'}, true, \'FR\');'.
			'">&nbsp;</a>
		</p>';
		echo '<p class="form-input"><label>Intitulé :</label><input type="text" name="ad_title" size="40" /></p>';
		echo '<p class="form-textarea"><label>Texte :</label><textarea name="ad_text" cols="50" rows="8"></textarea></p>';
		echo '<p class="form-input"><label>Image associée :</label><input type="file" name="ad_image" size="19" /></p>';
		echo '<p class="form-submit">
				<input type="submit" value="Ajouter" />
			</p>
		</form>';
	
	}

	public function deleteAd($id_ad){
		global $db;
		$res = $db->query("SELECT image FROM module_event WHERE id='".$id_ad."'");
		while($img = $db->fetchArray($res)){
			$f = '../images/'.$img['image'];
			if(is_file($f))	unlink($f);
		}
		$db->query("DELETE FROM module_event WHERE id='".$id_ad."'");
	}

	public function saveAd($infos, $image=''){
		global $core, $db;
		$core->loadClass('date');
		$date = new Date();
    $core->loadClass('file');
    $file = new file();
		
	 	$img_id=rand();
		
		switch($infos['ad_save_case']){
		case 'add':
			$infos['ad_start']=$date->reverseDate($infos['ad_start']);
			$infos['ad_end']=$date->reverseDate($infos['ad_end']);
			$db->query("INSERT INTO module_event VALUES ('','".$infos['ad_name']."','".$infos['ad_start']."','".$infos['ad_end']."','".$infos['ad_title']."','".$infos['ad_text']."','".((isset($image['name']))?$img_id.'.jpg':'')."')");
			break;
		case 'update':
			$infos['ad_start']=$date->reverseDate($infos['ad_start']);
			$infos['ad_end']=$date->reverseDate($infos['ad_end']);
			if((isset($image['name']) && $image['name']!='') || (isset($infos['del_ad_img']) && $infos['del_ad_img']=='yes')){
				$res = $db->query("SELECT image FROM module_event WHERE id='".$infos['id_ad']."'");
				while($img = $db->fetchArray($res)){
					$f = '../images/'.$img['image'];
					if(is_file($f))	unlink($f);
				}
			}
			if(isset($image['name']) && $image['name']!=''){
    		$db->query("UPDATE module_event SET nom='".$infos['ad_name']."', jr_debut='".$infos['ad_start']."', jr_fin='".$infos['ad_end']."', titre='".$infos['ad_title']."', texte='".$infos['ad_text']."', image='".$img_id.'.jpg'."' WHERE id='".$infos['id_ad']."'");
			}else{
    		$db->query("UPDATE module_event SET nom='".$infos['ad_name']."', jr_debut='".$infos['ad_start']."', jr_fin='".$infos['ad_end']."', titre='".$infos['ad_title']."', texte='".$infos['ad_text']."', image=".(($infos['del_ad_img']=='yes')?"NULL":"image")." WHERE id='".$infos['id_ad']."'");
			}
		break;
		default:
			//void
			break;
		}

		if(isset($image) && $image['name']!=''){
	 		$url = $core->getConfig('baseServer').'/modules/module_event/images/'.$img_id.'.jpg';
	 		switch($image['error']) {
	    	case 0:
	       	$size = getimagesize($image['tmp_name']);
					//type d'image : gif, jpeg, png acceptes
	       	if ($size[2] >= 1 && $size[2] <= 3) {
		  			//On enregistre l'image
		  			if(move_uploaded_file($image['tmp_name'], $url)){
             
            	 //On redimensionne l'image grace à la classe file
            	if($file->image_max_size($url,250,250) == false){
								$page->msgError(locale::display('img_error','event'));
							}
		  			}
	     		}
			}

		}

		$this->displayAds();
	}

}
?>
