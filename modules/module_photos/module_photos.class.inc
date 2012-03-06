<?php


/**************************
*
*	MODULE PHOTO -> MODULE_PHOTO.CLASS.INC
*		Extension of the Application Photo Album.	
*
*	@author : Sylvain Ramboz
* @date : 02/2007
*
***************************/

class Module_photos extends Module {

	private $id;
	public $photos_infos;

	public final function __construct( $informations = Array() ) {
		parent::__construct($informations);
		
		do {
			$this->id = rand(1000000,9999999);
		} while ( isset($_SESSION['module_photos_'.$this->id]) );
		$_SESSION['module_photos_'.$this->id] = serialize($this);
	}

  public function display() {
		global $db, $core;

		$core->loadClass('FileManager');
		$fm = new FileManager();
			
		$p_query = "SELECT id_photo AS id,
											 file.name AS name
											 
								FROM photo_album_photos photo,
								     kernel_file_manager_files file,
										 kernel_tag_information info,
										 kernel_tag_tag tag,
										 kernel_tag_liste_tag liste_tag
										 
								WHERE file.id = id_photo
									AND info.tableOrig = 'photo_album_photos'
									AND photo.id = info.idOrig
									AND info.id = tag.idInformation
									AND tag.idTagName = liste_tag.id
									AND liste_tag.name = '@shared' 
								
								GROUP BY photo.id
								ORDER BY photo.id DESC
								LIMIT 50";

		$res = $db->query($p_query);
		$tot = $db->numRows($res);
    $displayed = rand(1,$tot);
		$photos['total'] = $tot;
		$photos['current'] = $displayed;
		$i = 1;
				
		while ( $p_found = $db->fetchArray($res) ) {
			$photos[$i]['name'] = $p_found['name'];
			$photos[$i]['id'] = $p_found['id'];
			$i++;
		}
		$this->photos_infos = $photos;

		echo '<div id="photos">';

		$this->loadPhoto($photos[$displayed]['id'], $photos[$displayed]['name']);

		echo '</div>'
				.'<div class="buttons_block">'
						.'<a class="p_buttons prev_photo" onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_photos/ajax/photos.php').'?id_photos='.$this->id.'&geniutt_action=prevPhoto\',\'photos\');" >&nbsp;</a>'
						.'<a class="p_buttons next_photo" onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_photos/ajax/photos.php').'?id_photos='.$this->id.'&geniutt_action=nextPhoto\',\'photos\');" >&nbsp;</a>'
						.'<div class="clear"></div>'
				.'</div>'
				
				.'<script type="text/javascript">'
					.'function refreshPhoto() {'
									.'AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_photos/ajax/photos.php')
											.'?id_photos='.$this->id
											.'&geniutt_action=nextPhoto\', \'photos\');'
							.'setTimeout("refreshPhoto()", 60000);'
					.'}'
					.'addEventListener(document, \'DOMContentLoaded\', refreshPhoto, false);'
				.'</script>';
																																																																															
		$_SESSION['module_photos_'.$this->id] = serialize($this);
  }


	public function loadPhoto($id_file, $name_file) {
		global $core;

		$core->loadClass('FileManager');
		$fm = new FileManager();

		$mini = $fm->preview($id_file, 2);
		$name = $name_file;

		if ( strlen($name) > 37 ) {
			$name = substr($name, 0, 35).'...';
		}	

		echo '<a class="img_link" href="'.$core->makeUrl('photo_album/index.php').'?ga=dPFM&search_field='.$name_file.'" >'
				 		.'<span></span>'
						.$mini
				.'</a>'
				.'<p class="img_infos">'
						.$name
				.'</p>';
	}


	public function changePhoto($id_current, $case) {

		switch ($case) {

			case 'prev':
				if ($this->photos_infos['current'] - 1 == 0) {
					$this->photos_infos['current'] = $this->photos_infos['total'];
				}
				else {
					$this->photos_infos['current']--;
				}
			break;

			case 'next':
				if ($this->photos_infos['current'] == $this->photos_infos['total']) {
					$this->photos_infos['current'] = 1;
				}
				else {
					$this->photos_infos['current']++;
				}
			break;

			default:
				//void.
			break;

		}
		$this->loadPhoto($this->photos_infos[$this->photos_infos['current']]['id'], $this->photos_infos[$this->photos_infos['current']]['name']);
	}


// End of the file.
}
?>
