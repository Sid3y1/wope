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


/******************* module_welcome_admin.class **********************************
Definition pour l'admin du module welcome
**********************************************************************/
class module_welcome_admin {
	
	private $id;

	public final function __construct()
	{
		$this->id = rand(1000000,9999999);
    $_SESSION['admin_welcome_'.$this->id] = serialize($this);
	}
						
	public function displayContent() {
		global $core,$db,$page;
		if(!isset($page))$page= new Page();

		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class = 'module_welcome' "), 0);
		$page->moduleHeader('Module '.$title,'');
		echo '<div id="module_welcome_admin_content">';
		$this->displayMessages();
		echo '</div>';
		$page->moduleFooter();
	}

	public function displayMenu() {
		global $core,$db,$page;
		if(!isset($page))$page= new Page();
		
		$title = $db->result($db->query("SELECT title FROM kernel_module_module WHERE class = 'module_welcome' "), 0);
		$page->moduleHeader('Module '.$title,'');

		echo '<ul class="panel">
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_welcome/admin/ajax/content.php').'?id_admin='.$this->id.'&content=display_messages\',\'module_welcome_admin_content\')">Voir les messages de bienvenue</a></li>
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_welcome/admin/ajax/content.php').'?id_admin='.$this->id.'&content=add_message\',\'module_welcome_admin_content\')">Ajouter un message</a></li>
					</ul>';
					
		$page->moduleFooter();
	}

	public function displayMessages(){
		global $core,$db,$page;
		$core->loadClass('date');
		$core->loadClass('box');
		$date = new Date();
		$box = new Box();
		if(!isset($page))$page = new Page();
	
		echo '<table align="center" class="data_table">';
		echo '<tr><th width="60%">Titre</th><th width="20%">Affiché</th><th width="20%">Suppression</th></tr>';
		$res = $db->query("SELECT * FROM module_welcome");
		
		while ($row = $db->fetchArray($res)){
			$checked="";
		  if ($row['valid'] == 'Y'){
				$checked=" checked ";
			}
			echo '<tr>
			  			<td><a onclick="hide(toolTips); AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_welcome/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update_message&id_message='.$row['id'].'\',\'module_welcome_admin_content\')" onmouseover="activateToolTips(this,\''.$page->htmlBlock->escapeTipContent(stripslashes($row['text'])).'\')">'.$row['title'].'</a></td>
			  			<td align="center"><input type="radio" name="message_valid"'.$checked.' onclick="AJAX.getAndCall(\''.$core->makeUrl('modules/module_welcome/admin/ajax/content.php').'?id_admin='.$this->id.'&content=validate_message&id_message='.$row['id'].'\')" /></td>
							<td align="center"><a class="trash" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_welcome/admin/ajax/content.php').'?id_admin='.$this->id.'&content=del_message&id_message='.$row['id'].'\',\'module_welcome_admin_content\')', 'Etes-vous sûr de vouloir supprimer ce message de bienvenue ?').'"></a></td>
						</tr>';
		}
		echo '</table>';
}

	public function displayUpdateMessage($id_message){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		
		echo '<div id="update_message_feedback" style="display:none" >';
			$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		
		$res = $db->query("SELECT * FROM module_welcome WHERE id='".$id_message."'");
		while($row = $db->fetchArray($res)){
			echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_welcome/admin/ajax/content.php').'" onsubmit="'.
				'display(gE(\'update_message_feedback\'));'.
				'AJAX.submitAndUpdate(this, true, \'module_welcome_admin_content\');'.
				'return false;'.
			'">';
			echo '<input type="hidden" name="id_admin" value="'.$this->id.'" />';
			echo '<input type="hidden" name="content" value="save_message" />';
			echo '<input type="hidden" name="message_save_case" value="update" />';
			echo '<input type="hidden" name="id_message" value="'.$id_message.'" />';
			echo '<p class="form-input"><label>Titre :</label><input type="text" name="message_title" size="20" value="'.$row['title'].'" /></p>';
			echo '<p class="form-textarea"><label>Texte :</label><textarea name="message_text" cols="50" rows="8">'.$row['text'].'</textarea></p>';
			echo '<p class="form-input"><label>Image associée :</label>';
			if(isset($row['image']) && $row['image']!='' ){
			  echo '<img alt="Image de '.$row['title'].'" src="'.$core->makeUrl('modules/module_welcome/images').'/'.$row['image'].'" /></p>
			  <p class="form-input"><label></label><input type="checkbox" name="del_message_img" value="yes" /><span> Pas d\'image</span></p><p class="form-input"><label>Nouvelle image :</label>';
			}
			echo '<input type="file" name="message_image" size="19" /></p>';
			echo '<p class="form-submit">
					<input type="submit" value="Modifier" />
				</p>
			</form>';
		}

	}

	public function displayAddMessage(){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
									
		echo '<div id="add_message_feedback" style="display:none" >';
		$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('modules/module_welcome/admin/ajax/content.php').'" onsubmit="'.
			'display(gE(\'add_message_feedback\'));'.
			'AJAX.submitAndUpdate(this, true, \'module_welcome_admin_content\');'.
			'return false;'.
		'">';
		echo '<input type="hidden" name="id_admin" value="'.$this->id.'" />';
		echo '<input type="hidden" name="content" value="save_message" />';
		echo '<input type="hidden" name="message_save_case" value="add" />';
		echo '<p class="form-input"><label>Intitulé :</label><input type="text" name="message_title" size="40" /></p>';
		echo '<p class="form-textarea"><label>Texte :</label><textarea name="message_text" cols="50" rows="8"></textarea></p>';
		echo '<p class="form-input"><label>Image associée :</label><input type="file" name="message_image" size="19" /></p>';

		echo '<p class="form-submit">
				<input type="submit" value="Ajouter" />
			</p>
		</form>';
	
	}

	public function deleteMessage($id_message){
		global $db;
		$res = $db->query("SELECT image FROM module_welcome WHERE id='".$id_message."'");
		while($img = $db->fetchArray($res)){
			$f = '../images/'.$img['image'];
			if(is_file($f))	unlink($f);
		}
		$db->query("DELETE FROM module_welcome WHERE id='".$id_message."'");
	}

	public function saveMessage($infos, $image=''){
		global $core, $db;
		$core->loadClass('date');
		$date = new Date();
    $core->loadClass('file');
    $file = new file();
		
	 	$img_id=rand();
		
		switch($infos['message_save_case']){
		case 'add':
			$db->query("INSERT INTO module_welcome (title, text, image) VALUES ('".$infos['message_title']."','".$infos['message_text']."','".((isset($image['name']))?$img_id.'.jpg':'')."')");
			break;
		case 'update':
			if((isset($image['name']) && $image['name']!='') || (isset($infos['del_message_img']) && $infos['del_message_img']=='yes')){
				$res = $db->query("SELECT image FROM module_welcome WHERE id='".$infos['id_message']."'");
				while($img = $db->fetchArray($res)){
					$f = '../images/'.$img['image'];
					if(is_file($f))	unlink($f);
				}
			}
			if(isset($image['name']) && $image['name']!=''){
    		$db->query("UPDATE module_welcome SET title='".$infos['message_title']."', text='".$infos['message_text']."', image='".$img_id.'.jpg'."' WHERE id='".$infos['id_message']."'");
			}else{
    		$db->query("UPDATE module_welcome SET title='".$infos['message_title']."', text='".$infos['message_text']."', image=".(($infos['del_message_img']=='yes')?"NULL":"image")." WHERE id='".$infos['id_message']."'");
			}
		break;
		default:
			//void
			break;
		}

		if(isset($image) && $image['name']!=''){
	 		$url = $core->getConfig('baseServer').'/modules/module_welcome/images/'.$img_id.'.jpg';
	 		switch($image['error']) {
	    	case 0:
	       	$size = getimagesize($image['tmp_name']);
					//type d'image : gif, jpeg, png acceptes
	       	if ($size[2] >= 1 && $size[2] <= 3) {
		  			//On enregistre l'image
		  			if(move_uploaded_file($image['tmp_name'], $url)){
             
            	 //On redimensionne l'image grace à la classe file
            	if($file->image_max_size($url,250,250) == false){
								$page->msgError(locale::display('img_error','welcome'));
							}
		  			}
	     		}
			}

		}

		$this->displayMessages();
	}

	public function validateMessage($id_message){
		global $db;
		$db->query("UPDATE module_welcome SET valid='N' WHERE valid='Y'");
		$db->query("UPDATE module_welcome SET valid='Y' WHERE id='".$id_message."'");	
	}

}
?>
