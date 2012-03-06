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


/**
	* Le support multifichier n'est pas encore pris en compte
	* La redimension des images est opérationnelle grâce à la librairie du site de la classe file !
	* Les vidéos n'ont pas encore de player (ca ne saurait tarder !)
	* Les prévisualisateurs sont configurables en haut de la classe et leur code doit être mis dans le switch (chercher: //PREVIEW PART
	* Atention en cas de modification de cette classe, il ne faut pas considérer que le Type mime entré est une infosure, il est possible de truquer cette info pour le client !
	* En terme de CQ il faut protéger le dossier dans lequel il y a les fichiers !
	* Ne pas oublier de metre  < enctype="multipart/form-data" > dans les attributs du formulaire, c'est IMPERATIF !
	*/


/**
	* FileManager 
	* Manage all files of the site
*/
class FileManager {

	/**
		* path 
		* Content files URL
		* 
		* @var mixed
		* @access private
		*/
	private $path;

	/**
		* type 
		* Type of file, associated file name and preview tool
		* type absent = no preview !
		* 
		* @var mixed
		* @access private
		*/
	private $type = Array(
			"application/x-flash-video"=>Array("name"=>"Video","preview"=>"video"),
			"audio/mpeg"=>Array("name"=>"Son","preview"=>"mp3"),
			"image/jpeg"=>Array("name"=>"Image","preview"=>"img"),
			"image/pjpeg"=>Array("name"=>"Image","preview"=>"img"),
			"image/gif"=>Array("name"=>"Image","preview"=>"img"),
			"image/png"=>Array("name"=>"Image","preview"=>"img")
			);

	/**
		* size 
		* Sizes of the 3 preview levels
		* 
		* @var mixed
		* @access private
		*/
	private $size = Array(
			"img"=>Array(
				1=>Array("width"=>75,"height"=>75),
				2=>Array("width"=>150,"height"=>150),
				3=>Array("width"=>300,"height"=>300),
				4=>Array("width"=>800,"height"=>600)
				),
			"mp3"=>Array(
				1=>Array("width"=>65,"height"=>20),
				2=>Array("width"=>200,"height"=>20),
				3=>Array("width"=>200,"height"=>20),
				4=>Array("width"=>200,"height"=>20)
				),
			"video"=>Array(
				1=>Array("width"=>50,"height"=>50),
				2=>Array("width"=>150,"height"=>150),
				3=>Array("width"=>300,"height"=>300),
				4=>Array("width"=>800,"height"=>600)
				));
				
	/**
		* ids 
		* ids of file uploads
		* 
		* @var int
		* @access private
		*/
	private $uploads = Array();


	private $config = Array();
	
	/**
		* __construct
		* Class constructor
		* 
		* @access public
		* @return void
		*/
	public function __construct(){
		global $core;
		include('config_fileManager.inc');

		$this->config = $config_fileManager;
		
		//recovering of files path
		$this->path = $core->getConfig('fileManager','path');
		//if no path, takes default
		if(!isset($this->path) || $this->path == '') {
			$this->path = $core->getConfig('baseServer').'/data/files';
		}
	}

	/**
	  * generateId() generates an random id, unique in this object
		*
		* @return int id
		*/
	private function generateId(){
		do{
			$id = rand();
		}while(in_array($id, $this->uploads));
		
		return $id;
	}
	

	/**
		* saveOrGet 
		* Save a new file or return id of existent one
		* 
		* @param string $type 
		* @param int $mine 
		* @param string $right_r 
		* @param string $right_w 
		* @param mixed $searchable 
		* @param string $description 
		* @param string $from 
		* @access public
		* @return void
		*/
	public function saveOrGet($type='',$right_r = 'log',$right_w = 'admin',$searchable = true,$description = '',$from = 'unknow',$display_errors = true){
		$result = Array();

		foreach($this->uploads as $id){
			if(isset($_POST['chosen_file_'.$id]) && $_POST['chosen_file_'.$id][0] != 0){
				$files = $_POST['chosen_file_'.$id];
				if(count($files) == 1){
					$result[$id] = $files[0];
				}else{
					//Le support du multifiles n'est pas encore complet !
					$result[$id] = (Array)$files;
				}
			}elseif(isset($_FILES['upload_or_choose_'.$id])){
				$result[$id] = $this->saveFile('upload_or_choose_'.$id,$right_r,$right_w,$searchable,$description,$from,$display_errors);
			}else{
				$result[$id] = false;
			}
		}

		return $result;
	}



	/**
		* saveOrGetImage 
		* Does the same that saveOrGet with images. Allows to choose a resolution
		* 
		* @param int $width 
		* @param int $height 
		* @param int $mine 
		* @param string $right_r 
		* @param string $right_w 
		* @param mixed $searchable 
		* @param string $description 
		* @param string $from 
		* @access public
		* @return void
		*/
	public function saveOrGetImage($width= 100,$height=100,$mine=true,$right_r = 'log',$right_w = 'admin',$searchable = true,$description = '',$from = 'unknow'){
		$type = 'image';
		$result = Array();
		
		foreach($this->uploads as $id){
			if(isset($_POST['chosen_file_'.$id]) && $_POST['chosen_file_'.$id][0] != 0){
				$files = $_POST['chosen_file_'.$id];
				if(count($files) == 1){
					//echo 'NBR';
					$result[$id] = $files[0];
				}else{
					//Le support du multifiles n'est pas encore complet !
					$result[$id] = (Array)$files;
				}
			}elseif(isset($_FILES['upload_or_choose'])){
				//echo 'save';
				$result[$id] = $this->saveImage('upload_or_choose',$width,$height,$right_r,$right_w,$searchable,$description,$from);
			}else{
				$result[$id] = false;
			}
		}
		return $result;
	}



	/**
		* uploadOrChoose 
		* Propose a l'utilisateur soit de telecharger un nouveau fichier soit de choisir un fihcier dans la liste (fonction proposeFiles() !
		* 
		* @param   boolean  $mine   is the user the owner of the file or not (site's file)
		* @param   string   $type   file's type
		* @param   string   $label  input's label
		* @param   boolean  $box    displays "Files select window in a dialbox" (true) or in a standard intern div (false)
		*
		* @return  void
		*/
	public function uploadOrChoose($mine = true, $type = '', $label = 'Fichier :', $box = true, $id = ''){
		global $core;
		$core->loadClass('htmlblock');
		$hb = new HtmlBlock();
		
		//allows to use several uploadOrChoose methods in the same page
		if($id == ''){
			$id = $this->generateId();
		}
		$this->uploads[] = $id;
		
		echo '
		<div class="clear"></div>
		<div class="upload_or_choose">';

		echo '
			<div class="form-input"><label><span>'.substr($label, 0, 1).'</span>'.substr($label, 1).'</label>';
		

		//first button : "My files"
		if($box){
		
			echo '<a class="my_files link_button" onclick="'.
			
				//resets inputs
				'gE(\'file_manager_propose_input_'.$id.'\').value = \'\';'.
				'gE(\'file_manager_propose_file_name_'.$id.'\').value = \'\';'.

				//draws file browser dialbox
				'box_content = \''.$hb->escapeTipContent($this->proposeFiles($id, $mine, $type, true)).'\';'.
				'dialBox.draw(box_content, \''.$hb->escapeTipContent('Sélectionner un fichier').'\', false);'.

				//display of the right input
				'hide(gE(\'file_manager_upload_input_div_'.$id.'\'));'.
				'show(gE(\'file_manager_propose_file_name_'.$id.'\'));'.
				
			'">Mes fichiers</a>';
			
		}else{
		
			echo '<a class="my_files link_button" onclick="'.
			
				//hides the input "new file"
				'hide(gE(\'file_manager_upload_input_div_'.$id.'\'));'.

				//shows the file browser
				'show(gE(\'choose_file_box_'.$id.'\'));'.
				
			'">Mes fichiers</a>';
			
		}

		//Second button : "New file"
		if($box){
		
			echo '<a class="new_file link_button" onclick="'.

				//resets inputs
				'gE(\'file_manager_propose_input_'.$id.'\').value = \'\';'.
				'gE(\'file_manager_propose_file_name_'.$id.'\').value = \'\';'.
			
				//display of the right input
				'hide(gE(\'file_manager_propose_file_name_'.$id.'\'));'.
				'show(gE(\'file_manager_upload_input_div_'.$id.'\'));'.
				
			'">Nouveau fichier</a>';
			
		}else{
		
			echo '<a class="new_file link_button" onclick="'.

				//resets inputs
				'gE(\'file_manager_propose_input_'.$id.'\').value = \'\';'.
				'gE(\'file_manager_propose_file_name_'.$id.'\').value = \'\';'.

				//hides the file browser
				'hide(gE(\'choose_file_box_'.$id.'\'));'.
				
				//display of the right input
				'hide(gE(\'file_manager_propose_file_name_'.$id.'\'));'.
				'show(gE(\'file_manager_upload_input_div_'.$id.'\'));'.
				
			'">Nouveau fichier</a>';
			
		}
		
		echo '
			</div>
			
			<div class="form-input">';
		
		//input 'file selected from "My Files" '
		echo '
			<input type="text" id="file_manager_propose_file_name_'.$id.'" value="" style="display: none;" />';

		//input 'upload new file'
		echo '
		<span style="display: none;" id="file_manager_upload_input_div_'.$id.'">
			<input type="file" name="upload_or_choose_'.$id.'" id="file_manager_upload_input_'.$id.'" onchange="'.
				//resets inputs
				'gE(\'file_manager_propose_input_'.$id.'\').value = \'\';'.
				'gE(\'file_manager_propose_file_name_'.$id.'\').value = \'\';'.

				//hide the other input
				'hide(gE(\'file_manager_propose_file_name_'.$id.'\'));';
								 
		if(!$box){
			//fide the file browser
			echo 'hide(gE(\'choose_file_box_'.$id.'\'));';
		}
		
		echo '" />';
		
		if($mine){
			$quotas = $this->getQuotas();
			echo '<span class="remaining_space">Espace restant : <span>'.$this->humanSize($quotas['remaining']).'</span></span>';
		}

		echo '</span>';
		
		//&nbsp; is important to avoid page deformation when inputs appear
		echo '&nbsp;
			</div>
		';
		
			
		echo '
			<div class="input-hidden">
				<input type="hidden" id="file_manager_propose_input_'.$id.'" name="chosen_file_'.$id.'[]" />';

		//bad way to keep current selected file id
		echo '
				<input type="hidden" id="file_manager_selected_id_'.$id.'" value="" />
			</div>
			';
		
		echo '<div class="clear"></div>
		';
	
		if(!$box){
			//display the file browser
			$this->proposeFiles($id, $mine, $type, false);
			echo '<div class="clear"></div>
			';
		}

		echo '</div>';
	}


	/**
		* proposeFiles 
		* Propose a l'utilisateur de choisir un fichier, le fichier est ensuite mis dnas un input type="hidden" de nom chosen_file[] (anticipation du multifiles !)!
		* 
		* @param string $type 
		* @param mixed $mine 
		* @param int $nbr 
		* @param int $page 
		* @access public
		* @return void
		*/
	private function proposeFiles($id, $mine = true, $type = '', $box = true){
		global $core,$db;
		$core->loadClass('date');

		//request construction
		$condition =' ( 0 ';
		foreach($core->user->droits as $right){
			$condition .= "OR right_r = '$right' ";
		}
		$condition .= ' ) ';
		
		$more = '';
		if($mine){
			$more = " AND login='".$core->getUserId()."' ";
		}
		
		$res = $db->query("SELECT id,name,size,date, filetype FROM kernel_file_manager_files WHERE $condition AND searchable=true $more AND filetype LIKE '%$type%' ORDER BY name ASC ");
		
		//CSS not dynamic => file placement in an array with calculated dimensions
		$files_nb = (int)$db->numRows($res);
		$cols_nb = ((int)$this->config['files_per_column'] <= 3 ) ? (int)$this->config['files_per_column'] : 3;
		$rows_nb = (int)( intval($files_nb / $cols_nb) + ( ($files_nb % $cols_nb != 0) ? 1 : 0 ) );
		
		$max_width = 600;
		$max_height = 300;
		$files_window_width = ($cols_nb * 200 <= $max_width) ? $cols_nb * 200 : $max_width;
		$files_window_height = ($rows_nb * 150 <= $max_height) ? $rows_nb * 150 : $max_height;
		
		//construction of the "window"
		$window = '';

		if($files_nb > 0){
			if($box){
				$window .= '<form onsubmit="return false;">';
			}else{
				$window .= '<div id="choose_file_box_'.$id.'" style="display: none;">';
			}
		
			$window .= '
				<p class="form-input">Sélectionnez votre fichier :</p>';

			if($box){
				$window .= '<div class="files_table_window" style="height: '.$files_window_height.'px; width: '.$files_window_width.'px; overflow: auto;" >';	
			}
		
			$window .= '<table class="files_table">';
		
			for ($i = 0; $i < $rows_nb; $i++){
			
				//rows
				$window .= '<tr>';
				
				for($j = 0; $j < $cols_nb; $j++){
					if($don = $db->fetchArray($res)){
						$date = new Date($don['date']);
						$name = stripslashes($don['name']);

						//data = file
						$window .= '<td class="file" id="td_'.$id.'_'.$don['id'].'" width="'.($files_window_width / $cols_nb).'" height="'.($files_window_height / $rows_nb).'">';
						
						$window .= '<a onclick="'.
						
								//remove "selected" style from the previous selected file thumbnail
								'if(gE(\'file_manager_selected_id_'.$id.'\').value != \'\'){removeClassName(gE(gE(\'file_manager_selected_id_'.$id.'\').value), \'selected\');};'.

								//bad way to keep current selected element id
								'gE(\'file_manager_selected_id_'.$id.'\').value = \'td_'.$id.'_'.$don['id'].'\';'.

								//add "selected" style to the current selected file thumbnail
								'addClassName(this.parentNode, \'selected\');'.

								//resets "new file" input
								'gE(\'file_manager_upload_input_'.$id.'\').value = \'\';'.
								
								//bad way to keep current selected file id
								'gE(\'file_manager_propose_input_'.$id.'\').value = '.$don['id'].';'.

								//display of the selected file name
								'file_name = \''.$name.'\';'.
								'gE(\'file_manager_propose_file_name_'.$id.'\').value = file_name;'.
								'show(gE(\'file_manager_propose_file_name_'.$id.'\'));';
					
						if($box){
							//display of the selected file name
							$window .= 'gE(\'file_manager_selected_file_name_'.$id.'\').value = file_name;';
						}else{
							//hides the file browser
							$window .= 'hide(gE(\'choose_file_box_'.$id.'\'));';
						}
					
						$window .= '">';
						
						//file thumbnail
						$window .= '<span>'.$this->getIcon($don['id'], stripslashes($don['filetype'])).'</span><br />';
						
						//file name and other informations
						$window .= '<span id="file_manager_id'.$don['id'].'_'.$id.'" class="file_manager_name">'.( (strlen($name) > 25) ? substr($name, 0, 22).'...' : $name ).'</span>
						<span>'.$this->humanSize($don['size']).' '.$date->display('/').'</span>';
					
						$window .= '</a>';
						
						//data
						$window .= '</td>
						';
						
					//no more datas => filling of the last row if necessary
					}else{
						$window .= '<td></td>';
					}
				}
				
				//row
				$window .= '</tr>
				';
			}
			
			$window .= '
					</table>';
			
			if(!$box){
				$window .= '
				</div>'; //choose_file_box
		
			}else{
		
				$window .= '</div>'; //files_table_window
				
				//recall of selected file
				$window .= '<p class="form-input">
					<label>Fichier :</label>
					<input type="text" id="file_manager_selected_file_name_'.$id.'" value="" style="width: '.($files_window_width / 2).'px;" />
				</p>';
				
				$window .= '<p class="form-submit">'.

					//validation
					'<input type="button" value="OK" onclick="dialBox.erase();" /> '.

					//cancellation
					'<input type="button" value="'.locale::display('cancel', 'kernel').'" onclick="'.
						
						//resets inputs
						'gE(\'file_manager_propose_input_'.$id.'\').value = \'\';'.
						'gE(\'file_manager_propose_file_name_'.$id.'\').value = \'\';'.

						//hide "selected file" input 
						'hide(gE(\'file_manager_propose_file_name_'.$id.'\'));'.

						//close the "window"
						'dialBox.erase();'.
					'" />';
					
				'</p>';
				$window .= '</form>';
			}
			
		}
		//if no files => information and closing
		else{
			$window .= '<p>Vous n\'avez pour l\'instant chargé aucun fichier dans votre espace.</p>
				<form onsubmit="return false;">
					<p class="form-submit">
						<input type="button" onclick="dialBox.erase();" value="'.locale::display('close', 'kernel').'" />
					</p>
				</form>';
		}
		
		if($box){
			return $window;
		}else{
			echo $window;
		}
	}


	public function getIcon($id, $type){
		global $core;

		$type = explode('/', $type);
		
		if($type[0] == 'image'){
				switch($type[1]){
					case 'gif':
					case 'jpeg':
					case 'pjpeg':
					case 'png':
						return $this->preview($id, 1, true);
					default:
						//empty
						break;
				}
		}
		
		if(is_file($core->getConfig('baseServer').'/include/engine/file/icons/'.$type[1].'.png')){
			return '<img src="'.$core->makeUrl('include/engine/file/icons/'.$type[1].'.png').'" alt="'.$type[1].' icon" width="75" height="75" />';
		}
		if(is_file($core->getConfig('baseServer').'/include/engine/file/icons/'.$type[0].'.png')){
			return '<img src="'.$core->makeUrl('include/engine/file/icons/'.$type[0].'.png').'" alt="'.$type[0].' icon" width="75" height="75" />';
		}
		
		return '<img src="'.$core->makeUrl('include/engine/file/icons/file.png').'" alt="file icon" width="75" height="75" />';
		
	}


	/**
		* upload 
		* Propose a l'utilisateur de telecharger un nouveau fichier
		* 
		* @param   string   $label  input's label
		*
		* @return  void
		*/
	public function upload($label = 'Ajouter un fichier :', $id = ''){
		global $core;
		
		//allows to use several uploadOrChoose methods in the same page
		if($id == ''){
			$id = $this->generateId();
		}
		$this->uploads[] = $id;
		
		
		echo '<p class="form-input">'.
			( ($label != '') ? '<label>'.$label.'</label>' : '' ).
			'<input type="file" name="upload_or_choose_'.$id.'" />
		</p>';
	
	}

	
	/**
		* moveFileToFm 
		* Sauvegarde un fichier présent sur le serveur dans le systeme de fichier !
		* 
		* @access public
		* @return void
		*/
	public function moveFileToFm($file, $name = '', $signup = false, $remove = true, $right_r = 'log', $right_w = 'admin', $searchable = true, $description = '', $from = 'unknow'){
		global $core,$db;
		//Recuperation des parametres du fichier
		$filename = @basename($file);
		$size = @filesize($file);

		//On verifie si le quotas de l'utilisateur permet d'uploader un fichier de cette taille !
		if(!$signup && !$this->verifQuota($size)){	
			return false;
		}
		
		$type = @mime_content_type($file);
		$tmpName = "TMP".rand();
		$alt_name = $filename;
		if ($name != '') {
			$alt_name = $name;
		}
		if(@copy($file, $this->path.'/'.$tmpName)){
			if ($signup == false) {
				$db->query("INSERT INTO kernel_file_manager_files 
					(name,size,filetype,right_r,right_w,searchable,old_id,login,description,date,application) VALUES ('$alt_name','$size','$type','$right_r','$right_w',$searchable,'','".$core->getUserId()."','$description',NOW(),'$from')");
			} else {
				$db->query("INSERT INTO kernel_file_manager_files 
					(name,size,filetype,right_r,right_w,searchable,old_id,login,description,date,application) VALUES ('$alt_name','$size','$type','$right_r','$right_w',$searchable,'','".$_SESSION['new_user']."','$description',NOW(),'$from')");
				}
			$id = $db->insertId();
			if(@rename($this->path.'/'.$tmpName,$this->path.'/'.$id)){
					@chmod($this->path.'/'.$id,0755);

				if ($remove) {
					@unlink($file);
				}
				return $id;
			}else{
				$db->query("DELETE FROM kernel_file_manager_files WHERE id='$id'");
				return false;
			}
		}else{
			return false;
		}
	}

	

	/**
		* saveFile 
		* Sauvegarde un fichier dans le systeme de fichier !
		* 
		* @access public
		* @return void
		*/
	public function saveFile($name,$right_r = 'log',$right_w = 'admin',$searchable = true,$description = '',$from = 'unknow',$display_errors = true){
		global $core, $db;
		$page = new Page();
		
		//Recuperation des parametres du fichier
		$filename = $_FILES[$name]['name'];
		$size = $_FILES[$name]['size'];

		//On verifie si le quotas de l'utilisateur permet d'uploader un fichier de cette taille !
		if(!$this->verifQuota($size)){
			if($display_errors){
				$page->msgError('Vous n\'avez plus assez de place pour ajouter ce fichier.');
			}
			return false;
		}
		
		$type = $_FILES[$name]['type'];
		$tmpName = 'TMP'.rand();
		if(	move_uploaded_file($_FILES[$name]['tmp_name'],$this->path.'/'.$tmpName)){
			$db->query("INSERT INTO kernel_file_manager_files 
					(name,size,filetype,right_r,right_w,searchable,old_id,login,description,date,application) VALUES ('$filename','$size','$type','$right_r','$right_w',$searchable,'','".$core->getUserId()."','$description',NOW(),'$from')");
			$id = $db->insertId();
			if(rename($this->path.'/'.$tmpName,$this->path.'/'.$id)){
				chmod($this->path.'/'.$id, 0755);
				return $id;
			}else{
				$db->query("DELETE FROM kernel_file_manager_files WHERE id='$id'");
				return false;
			}
		}else{
			return false;
		}
	}

	/**
		* saveImage 
		* Permet d'enregistrer une image en precisant les taille maxi !
		* 
		* @param mixed $name 
		* @param mixed $width 
		* @param mixed $height 
		* @param string $right_r 
		* @param string $right_w 
		* @param mixed $searchable 
		* @param string $description 
		* @param string $from 
		* @access public
		* @return void
		*/
	public function saveImage($name,$width= 100,$height=100,$right_r = 'log',$right_w = 'admin',$searchable = true,$description = '',$from = 'unknow',$display_errors){
		$id = $this->saveFile($name,$right_r,$right_w,$searchable,$description,$from,$display_errors);
		if($id !== false){
			$core->loadClass('file');
			$file = new File();
			$file->image_max_size($this->path.'/'.$id,$width,$height);
			chmod($this->path.'/'.$id,0755);
			$size = filesize($this->path.'/'.$id);
			$db->query("UPDATE kernel_file_manager_files SET size='$size' WHERE id='$id'");
		}else{
			return false;
		}
	}
	/**
		* verifQuota 
		* Verifie le quota, renvoi si oui ou non il reste size !
		* 
		* @param mixed $size 
		* @param string $login 
		* @access public
		* @return void
		*/
	public function verifQuota($size = 0,$login = ''){
		$q = $this->getQuotas($login);
		if($q['remaining'] == -1 || $q['remaining'] >= $size){
			return true;
		}else{
			return false;
		}
	}

	/**
		* getQuotas 
		* Renvoi un tableau composé des infos de quotas du mec !
		* 
		* @param string $login 
		* @access public
		* @return void
		*/
	public function getQuotas($login= ''){
		global $core,$db;
		if($login == ''){
			$login = $core->getUserId();
		}
		$res = $db->query("SELECT quota FROM kernel_file_manager_quotas WHERE login='$login'");
		if($db->numRows($res) == 0){
			$limit = $core->getConfig('fileManager','quota');
			$db->query("INSERT INTO kernel_file_manager_quotas (login, quota) VALUES ('".$login."', '".(int)$limit."') ");
		}else{
			$limit = $db->result($res,0);
		}
		$used = $db->result($db->query("SELECT SUM(size) FROM kernel_file_manager_files WHERE login = '$login'"),0);

		if($limit == -1){
			$rest = -1;
		}else{
			$rest = $limit - $used;
		}
		return Array("used"=>$used,"limit"=>$limit,"remaining"=>$rest);
	}

	/**
		* humanSize 
		* Retourne la taille lisible par un humain !
		* 
		* @param mixed $size 
		* @access public
		* @return void
		*/
	public function humanSize($size){
		$i=0;
		$iec = array("octets", "Ko", "Mo", "Go", "To", "Po", "Eo", "Zo", "Yo");
		while (($size/1024)>1) {
			$size=$size/1024;
			$i++;
		}
		return substr($size,0,strpos($size,'.')+4).' '.$iec[$i];
	}

	public function displayQuotas($user = 'user'){
		$returned = '<dl class="file_manager_quotas_list">';
		$quotas = $this->getQuotas($user == 'user' ? null : $user);
		$returned .= '
		<dt>Espace alloué :</dt>
		<dd>'.$this->humanSize($quotas['limit']).'</dd>
		<dt>Espace occupé :</dt>
		<dd>'.$this->humanSize($quotas['used']).'</dd>
		<dt>Espace restant :</dt>
		<dd>'.$this->humanSize($quotas['remaining']).'</dd>
		</dl>';

		return $returned;
	}

	public function fileExists($id){
		global $db;
		return ( $db->result($db->query("SELECT COUNT(id) FROM kernel_file_manager_files WHERE id='".$id."' "), 0) == 1);
	}

	/**
		* summarize
		* Envoi un resume du fichier (le titre...)
		* 
		* @param mixed $id 
		* @param string $option 
		* @param "size" $"size" 
		* @param "type" $"type" 
		* @param "preview" $"preview" 
		* @param "download" $"download" 
		* @access public
		* @return void
		*/
	public function summarize($id,$option = Array("name","size","type","previewlink","preview","download","download_name_size")){
		global $db,	$core;
		
		$result = $db->query("SELECT * FROM kernel_file_manager_files WHERE id='".$id."'");
		if($db->numRows($result) != 0){
		
			$don = $db->fetchArray($result);
			if(!$core->verifDroits($don['right_r'])){
				return 0;
			}
			if(in_array('name',$option)){
				return '<span class="file_manager_name">'.$don['name'].'</span>';
			}
			if(in_array('type',$option)){
				$type = isset($this->type[$don['filetype']]['name'])?$this->type[$don['filetype']]['name']:$don['filetype'];
				return '<span class="file_manager_name">'.$type.'</span>';
			}
			if(in_array('size',$option)){
				return '<span class="file_manager_size">'.$this->humanSize($don['size']).'</span>';
			}
			if(in_array('previewlink',$option)){
				return '<a href="'.$core->makeUrl('include/engine/file/fileManagerPreview.php').'?id='.$id.'&level=4" class="file_manager_preview">Preview</a>';
			}
			if(in_array('download',$option)){
				return '<a href="'.$core->makeUrl('include/engine/file/fileManagerDownload.php?id='.$id).'" class="file_manager_download">Download</a>';
			}
			if(in_array('preview',$option)){
				return $this->preview($id,1,1);
			}
			if(in_array('download_name_size',$option)){
				return '<a href="'.$core->makeUrl('include/engine/file/fileManagerDownload.php?id='.$id).'">'.$don['name'].' ('.$this->humanSize($don['size']).')</a>';
			}

		}
		
	}


	/**
		* download 
		* Permet tout simplement de telechager le fichier !
		* 
		* @param mixed $id 
		* @access public
		* @return void
		*/
	public function download($id){
		global $db,$core;
		$don = $db->fetchArray($db->query("SELECT size,name,right_r FROM kernel_file_manager_files WHERE id='$id'"));
		if(!$core->verifDroits($don['right_r'])){
			return 0;
		}
		header("HTTP/1.1 200 OK");
		header("Content-Length: ".$don['size']);
		header("Content-Type: application/force-download");
		header("Content-Disposition: attachment; filename=".urlencode(stripslashes($don['name'])));
		header("Content-Transfer-Encoding: binary");
		$fpath = $this->path.'/'.$id;
		$bufsize = 20000;
		if(file_exists($fpath) && $fh = fopen($fpath, "rb")){
			while($buf = fread($fh, $bufsize))
				print $buf;
			fclose($fh);
		}
		else
		{
			header("HTTP/1.1 404 Not Found");
		}
	}

	/**
		* preview 
		* Previsualisation du fichier (transformation en html, ouvrir un player flash)
		* 
		* @param int $id 
		* @param int $level 
		* @param bool $html 
		* @param bool $tooltip  useful to put the preview in a tooltip -> protect html special chars
		* @access public
		* @return void
		*/
	public function preview($id,$level = 1,$html = true){
		global $db,$core;
		
		$result = $db->query("SELECT filetype,right_r,description FROM kernel_file_manager_files WHERE id='$id'");
		
		if($db->numRows($result) != 0){
			$don = $db->fetchArray($result);
		
			if(!$core->verifDroits($don['right_r'])){
				return false;
			}
			//Si on a un previsualisateur pour le type de fichier !!!
			if(isset($this->type[$don['filetype']]['preview'])){
			//Si html, alors on afiche la partie html du truc !
			if($html){
				switch ($this->type[$don['filetype']]['preview']){
					//PREVIEW PART
					//Ici on retoruve la generation HTML de la previsualisation
					//chaque case correspond a un previsualisateur
					case 'img':
						return '<img src="'.$core->makeUrl('include/engine/file/fileManagerPreview.php').'?id='.$id.'&level='.$level.'&fromhtml" class="file_manager_preview_level'.$level.'" alt="'.htmlspecialchars($don['description']).'" />';
					case 'mp3':
						return '<object type="application/x-shockwave-flash" data="'.$core->makeUrl('include/engine/file/player/mp3.swf').'?son='.$core->makeUrl('include/engine/file/fileManagerPreview.php').urlencode('?id='.$id.'&level='.$level.'&fromhtml').'"  width="'.$this->size['mp3'][$level]['width'].'" height="'.$this->size['mp3'][$level]['height'].'">
							<param name="movie" value="'.$core->makeUrl('include/engine/file/player/mp3.swf').'?son='.$core->makeUrl('include/engine/file/fileManagerPreview.php').urlencode('?id='.$id.'&level='.$level.'&fromhtml').'"  />
							</object>';
					case 'video':
						/*
						//Bloc de previsualisation video !!
									return '<script type="text/javascript" src="'.$core->makeUrl('include/engine/file/player/flv.js').'" ></script>
									<script type="text/javascript">
									var FO = {	movie:"'.$core->makeUrl('include/engine/file/player/flv.swf').'",width:"'.$this->size['video'][$level]['width'].'px",height:"'.$this->size['video'][$level]['height'].'px",majorversion:"7",build:"0",bgcolor:"#FFFF00",flashvars:"file='.$core->makeUrl('data/files/15').'" };
									UFO.create(FO, "flvplayer");
									</script>';*/
					default:
						return '<p class="file_manager_no_preview">no preview</p>';
				}

			}else{
				switch ($this->type[$don['filetype']]['preview']){
					//PREVIEW PART
					//Ici on retrouve la deuxieme partie (la lecture du fichier)!
					//chaque case correspond a un previsualisateur
					case 'img':
						if(is_file($this->path.'/preview/'.$id.'_'.$level)){
							header("Content-Type: ".$db->result($db->query("SELECT filetype FROM kernel_file_manager_files WHERE id='$id'"),0));
							readfile($this->path.'/preview/'.$id.'_'.$level);
						}else{
							$core->loadClass('file');
							$file = new File();
							$tmpname = 'TMP' . rand();
							copy($this->path.'/'.$id,$this->path.'/preview/'.$tmpname);
							chmod( $this->path.'/preview/'.$tmpname,0755);
							$file->image_max_size($this->path.'/preview/'.$tmpname, $this->size['img'][$level]['width'], $this->size['img'][$level]['height']);
							rename($this->path.'/preview/'.$tmpname,$this->path.'/preview/'.$id.'_'.$level);
							chmod($this->path.'/preview/'.$id.'_'.$level,0755);
							header("Content-Type: ".$db->result($db->query("SELECT filetype FROM kernel_file_manager_files WHERE id='$id'"),0));
							readfile($this->path.'/preview/'.$id.'_'.$level);
						}
						break;
					case 'mp3':
						readfile($this->path.'/'.$id);
						break;
					case 'video':
						readfile($this->path.'/'.$id);
						/*
									La commande pour encoder une video en flv a l'aide de mencoder (mplayer):
									mencoder input.avi -o
									output.flv -of lavf -oac mp3lame
									-lameopts abr:br=56 -ovc lavc \
									-lavcopts vcodec=flv:vbitrate=500:mbd=2:mv0:trell:v4mv:cbp:last_pred=3 \
									-srate 22050

							*/
						break;
					default:
						echo '<p class="file_manager_no_preview">no preview</p>';
				}
			}
		}else{
			return '<p class="file_manager_no_preview">no preview</p>';

		}

		}else{
			return false;
		}
	}


	/**
		* remove 
		* Suprime un fichier
		* 
		* @param mixed $id 
		* @access public
		* @return void
		*/
	public function remove($id){
		global $core, $db;
		if(	$core->verifDroits($db->result($db->query("SELECT right_w FROM kernel_file_manager_files WHERE id='$id'"),0))){
			if(is_file($this->path.'/'.$id)){
				unlink($this->path.'/'.$id);
			}
			if(is_file($this->path.'/preview/'.$id.'_1')){
				unlink($this->path.'/preview/'.$id.'_1');
			}
			if(is_file($this->path.'/preview/'.$id.'_2')){
				unlink($this->path.'/preview/'.$id.'_2');
			}
			if(is_file($this->path.'/preview/'.$id.'_3')){
				unlink($this->path.'/preview/'.$id.'_3');
			}
			$db->query("DELETE FROM kernel_file_manager_files WHERE id='$id' ");
			return true;
		}else{
			return false;
		}
	}
	/**
		* allInformation 
		* Affiche les informations pour le fichier
		* 
		* @param mixed $id 
		* @access public
		* @return void
		*/
	public function allInformation($id){

	}



	/**
		* search 
		* Recherche un fichier
		* 
		* @param mixed $text 
		* @param mixed $login 
		* @access public
		* @return void
		*/
	public function search($text,$login = ''){
		global $core,$db;
		$text = $db->encapseString($text);
		$more = '';
		if($login != ''){
			$more =  ' AND login = \''.$login.'\' ';
		}
		$ids = Array();
		$res =	$db->query("SELECT id, right_r FROM kernel_file_manager_files WHERE searchable=true AND name LIKE '%".$text."%' $more");
		while($don = $db->fetchArray($res)){
			if($core->verifDroits($don['right_r'])){
				$ids = $don['id'];
			}
		}
		return $ids;
	}

}

?>
