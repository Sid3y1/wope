<?php

/**************************
*
*	PHOTO_ALBUM -> PHOTO_ALBUM.CLASS.INC
*		Includes all the methods used in the application	
*
*	@author : Sylvain Ramboz
* @date : 02/2007
*
***************************/

class Photo_album extends Module {

  private $login;
  private $config;
	public $current_query;

  public final function __construct() {
		do {
			$this->id = rand(1000000, 9999999);
		} while ( isset($_SESSION['photo_album_'.$this->id]) );

		$_SESSION['photo_album_'.$this->id] = serialize($this);

	}



	public function displayAlbum($cur_page = 1, $query, $album_style = 'myAlbum') {
		global $core, $db;

	$core->loadClass('fileManager');
	$fm = new FileManager();
	$core->loadClass('tag_interface');
	$tag = new tag_interface('photo_album_photos','id');
	$core->loadClass('Page');
	$page = new Page();
	$core->loadClass('trombi');
  $trombi = new Trombi();
	
	$start_row = ($cur_page - 1) * 10;
	$photos_result = $query;
	$this->current_query = $query;
	$_SESSION['photo_album_'.$this->id] = serialize($this);
	$my_photos = $db->query($photos_result." LIMIT ".$start_row.", 10");
	$my_photos_number = $db->numRows($my_photos);
	$results_nb = $db->numRows($db->query($photos_result));
	$i = 0;
	$j = 0;

	// These four variables will be storing the things to display
	$album_photo = '<div><input id="selected_photo" type="hidden" value="" /></div><ul id="off">';
	$photo_previews = '';
	$submenus = '';
	$tagging_display = '';
	
	if ($my_photos_number > 0) {
	// We fill the variables
	while ($photo = $db->fetchArray($my_photos)) {
		$photo_name = stripslashes($photo['name']);
		if (strlen(stripslashes($photo['name'])) > 32) {
			$photo_name = substr(stripslashes($photo['name']),0,28).'...';
		}
		// Last items of a line do not have right margin
		$no_margin = '';
		if ($i == 4 || $i == 9) {
			$no_margin = 'no_margin';
		}
		if ($i == 5) {
			$album_photo .= '<div class="clear"></div>';
			$j = 0;
		}

		// Items of the album photo
		$album_photo .= $this->buildAlbumItem($i, $no_margin, $photo['file_id'], $photo['name']);
		// Previews of the photos
		$photo_previews .= $this->buildPhotoPreview($i, $photo['file_id'], $photo['firstname'], $photo['lastname'], $photo['login'], $photo_name, $photo['name'], $photo['description']);
		// Tag block for each photo
		$tagging_infos = $tag->photoTag($photo['photo_id']); 
		$tagging_display .= '<div id="tagging_block_'.$i.'" class="tagging_block" style="display: none;">'
					.$tagging_infos['display']
				.'</div>';
		// Submenus for each photo 
		$submenus .= $this->buildSubMenu($i, $photo_name, $tagging_infos['id'], $photo['name'], $photo['file_id'], $photo['photo_id'], $photo['id_album'], stripslashes($photo['title']), $photo['login'], $cur_page);
			$i++;
			$j++;
		}

		$album_photo .= '</ul>';

		echo $album_photo;

		if ($results_nb <= 10) {
			echo '<div class="special_clear"></div>';
		}

		// DISPLAY OF THE PAGES LIST
		switch ($album_style) {

			case 'myAlbum':
				$page->htmlBlock->pagesList($photos_result, 10, $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayMyAlbum&cur_page=[#]\',\'container\')');
			break;

			case 'publicAlbums':
				$page->htmlBlock->pagesList($photos_result, 10, $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayPublicAlbums&cur_page=[#]\',\'container\')');
			break;
			
			case 'publicPhotos':
				$page->htmlBlock->pagesList($photos_result, 10, $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayPublicPhotos&cur_page=[#]\',\'container\')');
			break;
					
			case 'searchPhotos':
				$page->htmlBlock->pagesList($photos_result, 10, $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displaySearchedPhotos&cur_page=[#]\',\'container\')');
			break;

			default:
			break;
			
		}
		
		echo $photo_previews.$submenus.'<div class="clear"></div>'.$tagging_display.'<div class="clear"></div>';
		}
		else {
			echo '<p class="empty_album"><em>Aucun résultat.</em></p>';
		}
	}


	public function buildAlbumItem($i, $margin, $id_photo, $photo_title) {

		global $core;

		$core->loadClass('fileManager');
		$fm = new FileManager();
		
		$small_title = $photo_title;
		if (strlen($small_title) > 17) {
			$small_title = substr($photo_title, 0, 15).'...';
		}
		
		$album_item = '<li class="photos_li '.$margin.'">'
						.'<a class="img_link" id="img_link_'.$i.'" href="#nogo_'.$i.'" ' 

												 .' onmouseover="if (gE(\'selected_photo\').value) { hide(gE(\'img_\'+gE(\'selected_photo\').value)); } '
												 							 .'show(gE(\'img_'.$i.'\'));" '

												 .' onmouseout="hide(gE(\'img_'.$i.'\')); '
												 							.'if (gE(\'selected_photo\').value) { show(gE(\'img_\'+gE(\'selected_photo\').value)); }" ' 

												 .' onclick="if (gE(\'selected_photo\').value) {'
												 									.'var current_photo = gE(\'selected_photo\').value; '
																					.'hide(gE(\'img_\'+current_photo)); '
																					.'hide(gE(\'album_submenu_\'+current_photo)); '
																					.'hide(gE(\'tagging_block_\'+current_photo)); '
																		.'}'
												 						.'gE(\'selected_photo\').value = '.$i.'; '
																		.'show(gE(\'img_'.$i.'\')); '
																		.'show(gE(\'album_submenu_'.$i.'\')); '
																		.'show(gE(\'tagging_block_'.$i.'\'));" >'
																	 
								.'<em class="link_em">'.$fm->preview($id_photo, 1).'<p class="img_info">'.$small_title.'</p></em>'
						.'</a>'
				.'</li>';

				return $album_item;
	}
	

	public function buildPhotoPreview($i, $id_photo, $author_firstname, $author_lastname, $login, $cutted_title, $photo_title, $photo_description) {
		global $core;
		
		$core->loadClass('fileManager');
			$fm = new FileManager();
		$core->loadClass('tag_interface');
			$tag = new tag_interface('photo_album_photos','id');
		$core->loadClass('trombi');
 	  	$trombi = new Trombi();
		$core->loadClass('Page');
			$page = new Page();

		if (strlen($photo_description) > 640) {
			$photo_description = substr($photo_description, 0, 640).'[...]';
		}
	
		$photo_preview = '<div class="img_cell" id="img_'.$i.'" style="display: none;">'
											.'<div class="centering_block">'
												.'<span></span>'
												.$fm->preview($id_photo, 3)
											.'</div>'
											.'<div class="description_block">'
													.'<p class="img_title" onmouseover="activateToolTips(this,\''.$page->htmlBlock->escapeTipContent($photo_title).'\');">'.$cutted_title.'</p>'
													.'<p class="author_name"><a onclick="'.$trombi->openCard($login).'">~ '.stripslashes($author_firstname).' '.stripslashes($author_lastname).'</a></p>'
													.'<p class="img_description">'.stripslashes($photo_description).'</p>'
													.'<br />'
											.'</div>'
										.'</div>';

		return $photo_preview;
	}



	public function buildSubMenu($i, $cutted_name, $id_info, $photo_name, $id_file, $id_photo, $album = 'none', $album_title, $user_login, $cur_page) {
		global $core, $db;
		
		$core->loadClass('fileManager');
			$fm = new FileManager();
		$core->loadClass('tag_interface');
			$tag_int = new tag_interface('photo_album_photos','id');
		$core->loadClass('Page');
			$page = new Page();

		$submenu = '<div id="album_submenu_'.$i.'" class="album_submenu" style="display: none;">'
							.'<ul class="submenu_list">'
								.'<li id="img_name" onmouseover="activateToolTips(this,\''.stripslashes($photo_name).'\');">'.$cutted_name.' : </li>'
								
								.'<li>'
										.'<a href="#nogoA" onclick="window.open(\''.$core->makeUrl('include/engine/file/fileManagerPreview.php').'?id='.$id_file.'&level=4\',\'Fullview\',\'height=630,width=830,top=100,left=50,resizable=yes,scrollbars=yes\');" onmouseover="show(gE(\'see_full_'.$i.'\'));" onmouseout="hide(gE(\'see_full_'.$i.'\'));"><span class="action_icon see_full">&nbsp;</span>'
										.'</a>'
										.'<a href="#nogoA" id="see_full_'.$i.'" class="action_title" style="display: none;">Voir</a>'	
								.'</li>';
		// IF THE PHOTO BELONGS TO AN ALBUM
		if ( is_numeric($album) ) {	
		$submenu .=	 '<li>'
										.'<a href="#nogoB" onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayTargetAlbum&target='.$album.'\',\'container\'); gE(\'\pa_part\').innerHTML = \''.$page->htmlBlock->escapeTipContent($album_title).'\'; " onmouseover="show(gE(\'browse_album_'.$i.'\'));" onmouseout="hide(gE(\'browse_album_'.$i.'\'));"><span class="action_icon browse_album">&nbsp;</span>'
										.'</a>'
										.'<a href="#nogoB" id="browse_album_'.$i.'" class="action_title" style="display: none;">Parcourir l\'album</a>'	
								.'</li>';
		}

		$submenu .=	 '<li>'
										.'<a href="#nogoC" onclick="display(gE(\'cloud_of_tags_'.$id_info.'\'));" onmouseover="show(gE(\'see_tags_'.$i.'\'));" onmouseout="hide(gE(\'see_tags_'.$i.'\'));">'
												.'<span class="action_icon see_tags">&nbsp;</span>'
										.'</a>'
										.'<a href="#nogoC" id="see_tags_'.$i.'" class="action_title" style="display: none;">Voir les tags</a>'
								.'</li>'
								.'<li>'
										.'<a href="#nogoD" onclick="display(gE(\'cloud_form_'.$id_info.'\'));" onmouseover="show(gE(\'add_tag_'.$i.'\'));" onmouseout="hide(gE(\'add_tag_'.$i.'\'));">'
												.'<span class="action_icon add_tags">&nbsp;</span>'
										.'</a>'
										.'<a href="#nogoD" id="add_tag_'.$i.'" class="action_title" style="display: none;">Ajouter des tags</a>'
								.'</li>';

		$current_user = $core->getUserId();
		// IF THE PHOTO BELONGS TO THE USER (OR IF IT'S AN ADMIN), WE DISPLAY MORE OPTIONS	
		if ($current_user == $user_login || $core->verifDroits('admin')) {
			$submenu .=	'<li>'
										.'<a href="#nogoE" onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayEditPhoto&id_file='.$id_file.'&id_photo='.$id_photo.'\',\'container\'); gE(\'\pa_part\').innerHTML = \''.stripslashes($page->htmlBlock->escapeTipContent($photo_name)).' | Edition\'; hide(gE(\'home_search\'));" onmouseover="show(gE(\'modify_'.$i.'\'));" onmouseout="hide(gE(\'modify_'.$i.'\'));" >'
												.'<span class="action_icon edition">&nbsp;</span>'
										.'</a>'
										.'<a id="modify_'.$i.'" class="action_title" style="display: none;">Modifier</a>'
								.'</li>'
								
								.'<li>'
										.'<a href="#nogoF" onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=deletePhoto&cur_page='.$cur_page.'&id_photo='.$id_photo.'\',\'container\');" onmouseover="show(gE(\'delete_'.$i.'\'));" onmouseout="hide(gE(\'delete_'.$i.'\'));" >'
												.'<span class="action_icon erase">&nbsp;</span>'
										.'</a>'
										.'<a id="delete_'.$i.'" class="action_title" style="display: none;">Supprimer</a>'
								.'</li>';
		}

		// ONLY THE OWNER OF THE PHOTO CAN SHARE OR UNSHARE IT
		if ($current_user == $user_login) {
			$submenu .= '<li class="no_margin">'
										.$tag_int->displayShared($id_info)
								 .'</li>';
		}

		$submenu .= '</ul></div>';
		return $submenu;
	}



	public function displayAlbums($cur_page = 1, $query, $case = 'lastAlbums') {
		global $core, $db;

		$core->loadClass('fileManager');
		$fm = new FileManager();
		$core->loadClass('Page');
		$page = new Page();
		$core->loadClass('Date');
		$date = new Date();
	
		$start_row = ($cur_page - 1) * 20;
		$albums_result = $query;
		$this->current_query = $query;
		$albums = $db->query($albums_result." LIMIT ".$start_row.", 20");
		$albums_number = $db->numRows($albums);
		$i = 0;

		$albums_list = '<ul id="albums_list">';
		if ($albums_number > 0) {
			// We fill the variable
			while ($album = $db->fetchArray($albums)) {
				$album_title = stripslashes(html_entity_decode($album['title'], ENT_QUOTES, 'UTF-8'));
				$album_description = stripslashes(html_entity_decode($album['description'], ENT_QUOTES, 'UTF-8'));
				$mini_title = $album_title; 
				if (strlen($mini_title) > 20) {
					$mini_title = substr($album_title,0,20).'...';
				}

				if ($i == 5) {
					$albums_list .= '<div class="clear"></div>';
				}
				
				$tooltip_content = '<p class="tt_album_title">'.$album_title.'</p><p class="tt_album_description">'.$album_description.'</p>';
				$tooltip_content = $page->htmlBlock->escapeTipContent($tooltip_content);
				
				$albums_list .= '<li>'
														.'<a class="album_link" onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?'
																													.'id_album='.$this->id
																													.'&geniutt_action=displayTargetAlbum'
																													.'&target='.$album['id'].'\',\'container\'); '
																													.'hide(toolTips); '
																													.'gE(\'pa_part\').innerHTML = \''.$page->htmlBlock->escapeTipContent($album_title).'\';" ' 
																								.'onmouseover="activateToolTips(this,\''.$tooltip_content.'\');">'
																.'<div class="album_li">'
																		.'<span></span>'
																		.$fm->preview($album['cover'], 1)
																.'</div>'
														.'</a>'
														.'<p class="album_info">'.$mini_title.'</p>'
														.'<p class="album_date">'.$date->reverseDate($album['date']).'</p>'
											 .'</li>';
				$i++;
			}

			$albums_list .= '</ul>';
			echo $albums_list;
			
	switch ($case) {
	
		case 'LastAlbums':
			$page->htmlBlock->pagesList($albums_result, 20, $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayLastAlbums&target='.$album['id'].'&cur_page=[#]\',\'container\')');
		break;

			case 'searchAlbums':
				$page->htmlBlock->pagesList($albums_result, 20, $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displaySearchedAlbums&cur_page=[#]\',\'container\')');
			break;
			
			default:
			break;
		}

		} else {
			echo '<p class="empty_album"><em>Aucun résultat.</em></p>';
		}
	}



	public function displayMenu() {
		global $core;

		$core->loadClass('Page');
		$page = new Page();

		echo '<ul class="panel">'
						.'<li>'
								.'<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayMyAlbum\',\'container\'); gE(\'\pa_part\').innerHTML = \'Mes Photos\'; hide(gE(\'home_search\'));" >Mes Photos</a>'
						.'</li>'
						.'<li>'
								.'<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayLastAlbums\',\'container\'); gE(\'\pa_part\').innerHTML = \'Derniers albums\'; hide(gE(\'home_search\')); ">Albums Récents</a>'
						.'</li>'
						.'<li>'
								.'<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayPublicAlbums\',\'container\'); gE(\'\pa_part\').innerHTML = \'Dernières photos\'; hide(gE(\'home_search\')); ">Photos Récentes</a>'
						.'</li>'
						.'<li>'
								.'<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayAlbumsSearch\',\'container\'); gE(\'\pa_part\').innerHTML = \''.$page->htmlBlock->escapeTipContent("Recherche d'albums").'\'; hide(gE(\'home_search\')); ">Rechercher des albums</a>'
						.'</li>'
						.'<li>'
								.'<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayPhotosSearch\',\'container\'); gE(\'\pa_part\').innerHTML = \'Recherche de photos\'; hide(gE(\'home_search\')); ">Rechercher des photos</a>'
						.'</li>'
				.'</ul>'
				
				.'<div class="menu_clear"></div>'
				.'<a class="link_button" onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php').'?id_album='.$this->id.'&geniutt_action=displayAddPhoto\',\'container\'); gE(\'\pa_part\').innerHTML = \'Import de photo : Etape n°1\'; hide(gE(\'home_search\'));" >Importer une photo</a>'
				.'<div class="menu_clear"></div>';
	}



	public function displayAlbumsSearch() {
		global $core;

		echo '<form action="'.$core->makeUrl('applications/photo_album/ajax/ajax.php').'" method="post" enctype="multipart/form-data" '
		              .'onsubmit="AJAX.submitAndUpdate(this, false, \'container\'); return false;" >'

	      	  .'<input type="hidden" name="id_album" value="'.$this->id.'" />'
						.'<input type="hidden" name="geniutt_action" value="searchAlbums" />'
						
						.'<p class="form-input">'	
						.'<label for="s_atitle">Le titre de l\'album contient : </label>'
						.'<input type="text" name="s_atitle" id="s_atitle" value="" />'
						.'</p>'
						.'<p class="form-input">'
						.'<label for="s_adesc">Sa description contient : </label>'
						.'<input type="text" name="s_adesc" id="s_adesc" value="" />'
						.'</p>'
						.'<div class="special_clear"></div>'
						.'<p class="form-submit">'
						  .'<input type="submit" value="Rechercher" />'
						.'</p>'

				.'</form>';
	}



	public function displayHomeSearch() {
		global $core;
		echo '<form id="home_search" action="'.$core->makeUrl('applications/photo_album/ajax/ajax.php').'" method="post" enctype="multipart/form-data" '
		              .'onsubmit="AJAX.submitAndUpdate(this, false, \'container\'); return false;" >'
									
						.'<p class="search_block">'
			      	.'<input type="hidden" name="id_album" value="'.$this->id.'" />'
							.'<input type="hidden" name="geniutt_action" value="homeSearch" />'

							.'<input type="text" name="search_field" id="search_field" value="" /> '
							.'<input type="submit" value="Rechercher" />'
						.'</p>'
				.'</form>';
	}



	public function displayPhotosSearch() {
		global $core;

		echo '<form action="'.$core->makeUrl('applications/photo_album/ajax/ajax.php').'" method="post" enctype="multipart/form-data" '
		              .'onsubmit="AJAX.submitAndUpdate(this, false, \'container\'); return false;" >'

	      	  .'<input type="hidden" name="id_album" value="'.$this->id.'" />'
						.'<input type="hidden" name="geniutt_action" value="searchPhotos" />'
						
						.'<p class="form-input">'
						.'<label for="s_ptitle">Le titre de la photo contient : </label>'
						.'<input type="text" name="s_ptitle" id="s_ptitle" value="" />'
						.'</p>'
						.'<p class="form-input">'
						.'<label for="s_pdesc">Sa description contient : </label>'
						.'<input type="text" name="s_pdesc" id="s_pdesc" value="" />'
						.'</p>'
						.'<p class="form-input">'
						.'<label for="s_tag">Tags : </label>'
						.'<input type="text" name="s_tag" id="s_tag" value="" />'
						.'<p>'
						.'<div class="special_clear"></div>'				
						.'<p class="form-submit">'
						  .'<input type="submit" value="Rechercher" />'
						.'</p>'

				.'</form>';
	}
	


	public function buildHomeQuery($infos) {

		$built_query = "SELECT photo.id AS photo_id,
													 photo.id_photo AS file_id,
													 photo.id_album,
													 album.title,
													 file.name,
													 file.login,
													 file.description,
													 trombi.lastname,
													 trombi.firstname

										FROM   photo_album_albums album,
										       ( ( ( photo_album_photos photo
													 INNER JOIN kernel_file_manager_files file ON photo.id_photo = file.id )
													   INNER JOIN trombi_trombi trombi ON file.login = trombi.id_user )
													      INNER JOIN kernel_tag_information info ON photo.id = info.idOrig ),
													 kernel_tag_tag tag INNER JOIN kernel_tag_liste_tag liste_tag ON tag.idTagName = liste_tag.id
												 									
										WHERE (photo.id_album = album.id OR photo.id_album IS NULL)
											AND info.tableOrig = 'photo_album_photos'
											AND info.id = tag.idInformation 
											AND liste_tag.name = '@shared' ";

		if (isset($infos['search_field']) && $infos['search_field'] != '') {
			$built_query .= " AND info.id IN (SELECT tag2.idInformation FROM kernel_tag_tag tag2 INNER JOIN kernel_tag_liste_tag liste_tag2 ON tag2.idTagName = liste_tag2.id WHERE liste_tag2.name = '".strtolower($infos['search_field'])."'
								 			  OR LOWER(file.description) LIKE LOWER('%".strtolower($infos['search_field'])."%')
											  OR LOWER(file.name) LIKE LOWER('%".strtolower($infos['search_field'])."%')) ";
		}
		$built_query .= " GROUP BY photo.id ORDER BY photo.id DESC";

		return $built_query;
	}
	

	
	public function buildAlbumsQuery($infos) {
		global $db;

		$built_query = "SELECT MAX(photo.id_photo) AS cover,
													 album.id,
													 album.title,
													 album.description,
													 album.date

										FROM   photo_album_albums album,
													 photo_album_photos photo,
													 kernel_file_manager_files file,
													 kernel_tag_information info,
													 kernel_tag_tag tag,
													 kernel_tag_liste_tag liste_tag,
													 trombi_trombi trombi

										 WHERE photo.id_album = album.id
										 	 AND info.tableOrig = 'photo_album_photos'
											 AND file.id = photo.id_photo
											 AND photo.id = info.idOrig
											 AND info.id  = tag.idInformation
											 AND tag.idTagName = liste_tag.id 
											 AND liste_tag.name = '@shared' ";
											
		if (isset($infos['s_adesc']) && $infos['s_adesc'] != '') {
			$built_query .= " AND LOWER(album.description) LIKE LOWER('%".$infos['s_adesc']."%') ";
		}
		if (isset($infos['s_atitle']) && $infos['s_atitle'] != '') {
			$built_query .= " AND LOWER(album.title) LIKE LOWER('%".$infos['s_atitle']."%') ";
		}
										
		$built_query .=	 "GROUP BY album.id
											ORDER BY album.id DESC";

		return $built_query;
	}
	
	
	
	public function buildPhotosQuery($infos) {
		global $db;

		$built_query = "SELECT photo.id AS photo_id,
													 photo.id_photo AS file_id,
													 photo.id_album,
													 album.title,
													 file.name,
													 file.login,
													 file.description,
													 trombi.lastname,
													 trombi.firstname

										FROM   photo_album_albums album,
										       ( ( ( photo_album_photos photo
													 INNER JOIN kernel_file_manager_files file ON photo.id_photo = file.id )
													   INNER JOIN trombi_trombi trombi ON file.login = trombi.id_user )
													      INNER JOIN kernel_tag_information info ON photo.id = info.idOrig ),
													 kernel_tag_tag tag INNER JOIN kernel_tag_liste_tag liste_tag ON tag.idTagName = liste_tag.id
												 									
										WHERE (photo.id_album = album.id OR photo.id_album IS NULL)
											AND info.tableOrig = 'photo_album_photos'
											AND info.id = tag.idInformation 
											AND liste_tag.name = '@shared' ";

		if (isset($infos['s_tag']) && $infos['s_tag'] != '') {
			$built_query .= " AND info.id IN (SELECT tag2.idInformation FROM kernel_tag_tag tag2 INNER JOIN kernel_tag_liste_tag liste_tag2 ON tag2.idTagName = liste_tag2.id WHERE liste_tag2.name = '".strtolower($infos['s_tag'])."') ";
		}

		if (isset($infos['s_pdesc']) && $infos['s_pdesc'] != '') {
			$built_query .= " AND LOWER(file.description) LIKE LOWER('%".strtolower($infos['s_pdesc'])."%') ";
		}
		if (isset($infos['s_ptitle']) && $infos['s_ptitle'] != '') {
			$built_query .= " AND LOWER(file.name) LIKE LOWER('%".strtolower($infos['s_ptitle'])."%') ";
		}

		$built_query .= " GROUP BY photo.id ORDER BY photo.id DESC";
		return $built_query;
	}

	
	
	public function displayUserPhotos($id_user) {
		global $core, $db;
		
		$built_query = "SELECT photo.id AS photo_id,
													 photo.id_photo AS file_id,
													 photo.id_album,
													 album.title,
													 file.name,
													 file.login,
													 file.description,
													 trombi.lastname,
													 trombi.firstname

										FROM   photo_album_albums album,
										       ( ( ( photo_album_photos photo
													 INNER JOIN kernel_file_manager_files file ON photo.id_photo = file.id )
													   INNER JOIN trombi_trombi trombi ON file.login = trombi.id_user )
													      INNER JOIN kernel_tag_information info ON photo.id = info.idOrig ),
													 kernel_tag_tag tag INNER JOIN kernel_tag_liste_tag liste_tag ON tag.idTagName = liste_tag.id
												 									
										WHERE (photo.id_album = album.id OR photo.id_album IS NULL)
											AND info.tableOrig = 'photo_album_photos'
											AND info.id = tag.idInformation 
											AND liste_tag.name = '@shared' 
											AND file.login = ".$id_user."
										
										GROUP BY photo.id 
										ORDER BY photo.id DESC";
											
		$this->current_query = $built_query;
		$this->displayAlbum(1, $built_query, 'searchPhotos');
	}



	public function displayAdd() {
		global $core;

		$core->loadClass('fileManager');
		$fm = new FileManager();
		include (dirname(__FILE__).'/config_photo_album.inc');

		echo '<form action="'.$core->makeUrl('applications/photo_album/index.php').'" method="post" enctype="multipart/form-data"  > '
							
        .'<input type="hidden" name="id_album" value="'.$this->id.'" />'
        .'<input type="hidden" name="linking_status" id="linking_status" value="not_linked" />'
				.'<input type="hidden" name="info_linked_id" id="info_linked_id" value="0" />'
				.'<input type="hidden" name="info_linked_title" id="info_linked_title" value="0" />'
				.'<input type="hidden" name="geniutt_action" value="savePhoto" />';
																				
		$fm->uploadOrChoose(true, 'image', 'Sélectionnez votre image :', true, 'photo');
		$_SESSION['file_manager_'.$this->id] = serialize($fm);
		
		echo '<a id="linking_button" href="#nogo" onclick="display(gE(\'linking_block\')); '
																 .'var status = gE(\'linking_status\').value; '
																 .'if (status == \'linked\') { '
																 	 .'gE(\'linking_status\').value = \'not_linked\'; '
																	 .'show(gE(\'link_it\')); '
																	 .'hide(gE(\'dont_link_it\')); '
																 .'} ' 
																 .'else { '
																 	 .'gE(\'linking_status\').value = \'linked\'; '
																	 .'hide(gE(\'link_it\')); '
																	 .'show(gE(\'dont_link_it\')); '
																 .'}" '
																 .'><span id="link_it">Lier mon image à une information du site...</span>'
																 	.'<span id="dont_link_it" style="display: none;">Ne pas lier mon image.</span>'
																 .'</a>';
		
		echo '<div id="linking_block" style="display: none;">'
			.'<div><em>Note : votre image sera automatiquement partagée à la communauté.</em></div>'
					.'<p class="form-input">'
						.'<label for="table_name">Type d\'information : </label>'
						.'<select id="table_name" name="info_linked_table_name" onchange="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php')
																													.'?id_album='.$this->id
																													.'&info_year=\'+(gE(\'info_year\').options[gE(\'info_year\').selectedIndex].value)+\''
																													.'&info_month=\'+(gE(\'info_month\').options[gE(\'info_month\').selectedIndex].value)+\''
																													.'&table_name=\'+(this.options[this.selectedIndex].value)+\''
																													.'&geniutt_action=displaySelectInfos\''
																													.', \'linking_choice\');'
																													.'gE(\'info_linked_id\').value = 0;'
																													.'gE(\'info_linked_title\').value = 0; ">';
		$tb_max = 5 * ($config['tables_nb'] - 1) + 1;
		for ($i=1;$i<=$tb_max;$i=$i+5) {
			echo '<option value="'.$config['tables'][$i].'" ';
			if ($i == 1) {
				echo 'selected="selected" ';
			}
			echo '>'.$config['tables'][$i+1].'</option>';
		}
		
		echo '</select>'
				.'</p>';

		$now = getdate();
		$cur_year = $now['year'];
		$start_year = 2006;
		$end_year = $cur_year;
		echo '<p class="form-input">'
					.'<label>Période : </label>'
					.'<select id="info_month" name="info_month" onchange="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php')
																													.'?id_album='.$this->id
																													.'&info_year=\'+(gE(\'info_year\').options[gE(\'info_year\').selectedIndex].value)+\''
																													.'&info_month=\'+(this.options[this.selectedIndex].value)+\''
																													.'&table_name=\'+(gE(\'table_name\').options[gE(\'table_name\').selectedIndex].value)+\''
																													.'&geniutt_action=displaySelectInfos\''
																													.', \'linking_choice\'); ">';
						
		for ($i=1; $i <= 12; $i++) {
		$m = $i < 10 ? '0'.$i : $i;
			echo '<option value="'.$m.'" >'.$m.'</option>';
		}
						
		echo '</select>'
					 .'<select id="info_year" name="info_year" onchange="AJAX.getAndUpdate(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php')
																													.'?id_album='.$this->id
																													.'&info_year=\'+(this.options[this.selectedIndex].value)+\''
																													.'&info_month=\'+(gE(\'info_month\').options[gE(\'info_month\').selectedIndex].value)+\''
																													.'&table_name=\'+(gE(\'table_name\').options[gE(\'table_name\').selectedIndex].value)+\''
																													.'&geniutt_action=displaySelectInfos\''
																													.', \'linking_choice\'); ">';
						
		for($y = $start_year; $y <= $end_year; $y++) {
			if ($y==1) { echo '<option value="'.$y.'" selected="selected" >'.$y.'</option>';  }
			else { echo '<option value="'.$y.'" >'.$y.'</option>'; }
		}
		
		echo '</select>'

			.'</p>'

			.'<p class="form-input">'
					  .'<label>Lier à : </label>'
						.'<span id="linking_choice" name="linked_info">'
								.'<select id="info_linked_select">'
										.'<option>Sélectionnez votre information.</option>'
								.'</select>'
						.'</span>'
			.'</p>'
	.'</div>'
	
	.'<script type="text/javascript">
	function checkSize(textArea, maxLength){
		var textLength = textArea.value.length
		if( textLength > maxLength ){
			textArea.value = textArea.value.substring(0, maxLength);
		}
	}
	</script>'
	
			.'<p class="form-input txt_description">'
					.'<label for="description">Description : </label>'
					.'<br /><br /><em style="clear: both;">Votre description ne sera prise en compte que si vous avez sélectionné un nouveau fichier.</em>'
					.'<textarea id="description" name="description" onblur="checkSize(this, 650);" onchange="checkSize(this, 650);" onkeydown="checkSize(this, 650);" ></textarea>'
			.'</p>'

			.'<p class="link_photo">'
					.'<label for="link_photo">Partager mon image avec la communauté</label>'
					.'<input type="checkbox" id="link_photo" name="link_photo" checked = "checked" value="linked" />'
			.'</p>'
		
			.'<p class="form-submit">'
				  .'<input type="submit" value="Ajouter" />'
			.'</p>'
	
	.'</form>';
		
	}



	public function displayAddStep2($linking_status = 'not_linked', $id_photo, $id_file, $id_album = '', $album_title = '') {
		global $core;

			$core->loadClass('fileManager');
			$core->loadClass('htmlBlock');
			$fm = new FileManager();
			$hb = new HtmlBlock();

			$step2_form = '<form action="'.$core->makeUrl('applications/photo_album/ajax/ajax.php').'" method="post" enctype="multipart/form-data" '
							.'onsubmit="if (gE(\'tagging_status\').value == \'tagged\') { '
															.'AJAX.submitAndUpdate(this, false, \'container\'); '
															.'gE(\'pa_part\').innerHTML = \'Mes Photos\'; '
													.'} else { '
															.'dialBox.draw(\''.$hb->escapeTipContent('<p>Vous devez entrer au moins un tag avant de continuer</p><p class="centrage"><input type="button" value="OK" onclick="dialBox.erase();" /></p>').'\', \'Erreur\', false); '
														.'} '
													.'return false;" > '
							
							.'<input type="hidden" name="geniutt_action"  value="saveStep2" />'
							.'<input type="hidden" name="id_album"        value="'.$this->id.'" />'
							.'<input type="hidden" name="target_album"    value="'.$id_album.'" />'
							.'<input type="hidden" id="tagging_status"    value="not_tagged" />'

						  .'<div class="edition_photo">'.$fm->preview($id_file, 1).'</div>'
							.'<div class="clear"></div>';

			if ($linking_status == 'linked' && $album_title != 'old_album') {
				$step2_form .=	 '<p>'
													.'<em>Un album va être créé pour stocker les images en rapport avec l\'information que vous avez sélectionnée.<br />Vous pouvez modifier ici le titre et la description de cet album :</em>'
												.'</p>'
												.'<p class="form-input">'
														.'<label for="title">Titre : </label>'
														.'<input type="text" id="title" name="title" value="'.stripslashes(html_entity_decode($album_title, ENT_QUOTES, 'UTF-8')).'" />'
												.'</p>'
												.'<p class="form-input">'
														.'<label for="description">Description : </label>'
														.'<textarea id="description" name="description"></textarea>'
												.'</p>';
			}

		$step2_form .= '<p>'
										.'<em>Un tag est une information permettant de définir l\'image, pour la retrouver plus facilement par exemple.<br /> Veuillez donner au moins un tag à votre photo : </em>'
									.'</p>';
									
		$step2_form .=
				 '<div class="bloc_ajout_tag">'
						.'<label for="my_tagging" class="champ_ajouter_tag left">Ajouter un tag : </label>'
						.'<input type="text" id="my_tagging" name="my_tagging" class="mon_tagging_input left" onfocus="hide(gE(\'msg_confirmation\'));" value="" />'
								.'<a class="bouton_ajout_tag left" ' 
											.'onclick="AJAX.getAndCall(\''.$core->makeUrl('applications/photo_album/ajax/ajax.php')
																	.'?id_album='.$this->id
																	.'&amp;geniutt_action=addtagToPhoto'
																	.'&amp;photo_tag=\' + gE(\'my_tagging\').value + \''
																	.'&amp;id_information='.$id_photo.'\' , '
																	.'function() { '
																			.'if (gE(\'my_tagging\').value != \'\') {'
																					.'gE(\'tagging_status\').value = \'tagged\'; '
																					.'show(gE(\'msg_confirmation\')); '
																					.'gE(\'my_tagging\').value = \'\'; '
																			.'} '
																			.'else { '
																					.'dialBox.draw(\''.$hb->escapeTipContent('<p>Vous avez entré un tag vide</p><p class="centrage"><input type="button" value="OK" onclick="dialBox.erase();" /></p>').'\', \'Erreur\', false);'
																			.'}'
																	.'} ) " >'
							  .'</a>'
				  .'</div>'
					.'<span id="msg_confirmation" style="display: none;">Tag ajouté !</span>'

					.'<div class="special_clear"></div>'
					.'<p class="form-submit">'
				  	.'<input type="submit" value="Continuer" />'
					.'</p>'

			 .'</form>';
		
		echo $step2_form;
	}



	public function saveStep2($id_album, $title, $description) {
		global $db;

		$db->query("UPDATE photo_album_albums 
										SET title = '".$db->escapeString(htmlentities($title, ENT_QUOTES, 'UTF-8'))."', 
										description = '".$db->escapeString(htmlentities($description, ENT_QUOTES, 'UTF-8'))."' 
								WHERE id = '".$id_album."'"
							);
	}



	public function savePhoto($infos) {
		global $core, $db;
				
		$core->loadClass('fileManager');
		$fm = unserialize($_SESSION['file_manager_'.$this->id]);

		$core->loadClass('date');
		$date = new Date();
		$submission_date = date("Y-m-d");
		
		$core->loadClass('tag');
		$core->loadClass('tagInterface');
		$tag = new Tag(array('photo_album_photos' => 'id'),$core,$db);

		$photo_id = $fm->saveOrGet('image', 'log', 'log', true, $db->escapeString(htmlentities($infos['description'], ENT_QUOTES, 'UTF-8')));
		$data['linking_status'] = $infos['linking_status'];

		// IF THE UPLOAD OF THE PHOTO IS A SUCCESS
		if ($photo_id['photo'] != false) {
				
			// CASE OF A LINKED PHOTO
			if (isset($infos['linking_status']) && $infos['linking_status'] == 'linked' && ($infos['info_linked_table_name'] != '0' && $infos['info_linked_id'] != '0')) {
				$seek_album = $db->query("SELECT id AS album FROM photo_album_albums WHERE id_info = '".$infos['info_linked_id']."' AND info_table_name = '".$infos['info_linked_table_name']."'");
				// IF AN ALBUM RELATED EXISTS
				if ($db->numRows($seek_album) > 0) {
					$album = $db->result($seek_album, 'album');
				}
				// IF NOT, WE CREATE IT
				else {
					$album_title = $db->escapeString(htmlentities($infos['info_linked_title'], ENT_QUOTES, 'UTF-8'));
					$db->query("INSERT INTO photo_album_albums (title, id_info, info_table_name, date) VALUES ('".$album_title."', '".$infos['info_linked_id']."', '".$infos['info_linked_table_name']."', '".$submission_date."')");
					$album = $db->insertId();
				}
				$db->query("INSERT INTO photo_album_photos (id_user, id_photo, id_album, date) VALUES ('".$core->getUserId()."', '".$photo_id['photo']."', '".$album."', '".$submission_date."')");
				$new_photo_id = $db->insertId();
				// The picture is automaticly shared
				$id_photo_to_share = $tag->getIdInformation('photo_album_photos', $new_photo_id);
				$tag->shareInformation($id_photo_to_share);
			}
			// END OF THE LINKED PHOTO CASE
			else {
				$db->query("INSERT INTO photo_album_photos (id_user, id_photo, date) VALUES ('".$core->getUserId()."', '".$photo_id['photo']."', '".$submission_date."')");	
				$new_photo_id = $db->insertId();
				if ( isset($infos['link_photo']) ) {
					$id_photo_to_share = $tag->getIdInformation('photo_album_photos', $new_photo_id);
					$tag->shareInformation($id_photo_to_share);
				}
				$album = '';
			}	
		}
		// IF THE UPLOAD OF THE PHOTO FAILED
		else {
			echo '<p class="empty_album"><em>L\'envoi du fichier a échoué. Vous avez oublié d\'en sélectionner un, il était trop gros ou son format n\'était pas supporté !</em></p>';
		}
		if (isset($album_title)) {
			$data['album_title'] = $album_title;
		} else {
				$data['album_title'] = 'old_album';
		}
		$data['id_album'] = $album;
		$data['id_photo'] = $new_photo_id;
		$data['id_file'] = $photo_id['photo'];
		return $data;
	}


	public function updatePhoto($id_photo, $id_file) {
		global $core, $db;
		$core->loadClass('magicform');
		$date = date('d-m-Y');
		$core->loadClass('fileManager');
		$fm = new FileManager();
			
		$seek_albums = "SELECT album.id, album.title FROM photo_album_albums album";
		$seek_file = "SELECT name, description FROM kernel_file_manager_files WHERE id = '".$id_file."'";
		$albums = $db->query($seek_albums);
		$t_file = $db->query($seek_file);
		
		while ($target = $db->fetchArray($t_file)) {
			$edit_form = '<form action="'.$core->makeUrl('applications/photo_album/ajax/ajax.php').'" method="post" enctype="multipart/form-data" '
			              .'onsubmit="AJAX.submitAndUpdate(this, false, \'container\'); '
								    .'return false;" > '

										.'<input type="hidden" name="geniutt_action"  value="saveModifsPhoto" />'
										.'<input type="hidden" name="id_photo"  value="'.$id_photo.'" />'
										.'<input type="hidden" name="id_file"   value="'.$id_file.'" />'
										.'<input type="hidden" name="id_album"  value="'.$this->id.'" />'
										
										.'<div class="edition_photo">'.$fm->preview($id_file, 1).'</div>'
										.'<div class="clear"></div>'
										.'<p class="form-input">'
											.'<label for="title">Titre : </label>'
											.'<input type="text" id="title" name="title" value="'.stripslashes($target['name']).'" />'
										.'</p>'
						
										.'<p class="form-input">'
											.'<label for="description">Description : </label>'
											.'<textarea id="description" name="description" >'.stripslashes($target['description']).'</textarea>'
										.'</p>'
										
										.'<p class="form-input">'
											.'<label for="album_linked">Mettre dans un album : </label>'
											.'<select id="album_linked" name="album_linked" >';
		}
		
		$current_album = $db->result($db->query("SELECT id_album AS album_linked FROM photo_album_photos WHERE id='".$id_photo."'"), 'album_linked');

		$select_built = '';
		$unlink_photo = '';
		$first_option = '<option value="" >Choisissez un album.</option>';			
		while ($album = $db->fetchArray($albums)) {
			if ($current_album == $album['id']) {
				$first_option = '<option value="'.$album['id'].'" selected="selected" >'.stripslashes($album['title']).'</option>';
			}
			else {
				$select_built .= '<option value="'.$album['id'].'" >'.stripslashes($album['title']).'</option>';
			}
		}

		$edit_form .= $first_option.$select_built;
		
		$checked = '';
		if ($first_option == '<option value="" >Choisissez un album.</option>') {
			$checked = 'checked="checked"';
		}
		$edit_form .=    	'</select>'
										.'</p>'
										.'<p class="unlink_album">'
												.'<label for="unlink_album">Ne pas lier cette image</label>'
												.'<input type="checkbox" id="unlink_album" name="unlink_album" '.$checked.' value="unlinked" />'
										.'</p>'

										.'<p class="form-submit">'
											  .'<input type="submit" value="Modifier" />'
										.'</p>'
								.'</form>';

		echo $edit_form;
	}



	public function saveModifsPhoto($infos) {
		global $core, $db;

		$core->loadClass('tag');
		$tag = new Tag(array('photo_album_photos' => 'id'),$core,$db);
	
		$db->query("UPDATE kernel_file_manager_files 
											 SET description = '".$infos['description']."', 
											 		 name = '".$infos['title']."' 
											 WHERE id = '".$infos['id_file']."' 
							 ");

		if (!isset($infos['unlink_album']) && $infos['album_linked'] != 'none') {
			$id_photo_to_share = $tag->getIdInformation('photo_album_photos', $infos['id_photo']);
  	  $tag->shareInformation($id_photo_to_share);
			$db->query("UPDATE photo_album_photos 
												 SET id_album = '".$infos['album_linked']."' 
												 WHERE id = '".$infos['id_photo']."' 
								 ");
		}

		if (isset($infos['unlink_album'])) {
			$id_photo_to_share = $tag->getIdInformation('photo_album_photos', $infos['id_photo']);
  	   $tag->shareInformation($id_photo_to_share);
			$db->query("UPDATE photo_album_photos 
												 SET id_album = NULL 
												 WHERE id = '".$infos['id_photo']."' 
								 ");
		}

	}



	public function deletePhoto($id_photo) {
		global $core, $db;

		$db->query("DELETE FROM photo_album_photos WHERE id = '".$id_photo."'");

	}
	
	
	
	public function buildSelect($query, $info_id, $info_title) {
		global $db;
		
	  $result = $db->query($query);
	  $result_nb = $db->numRows($result);
		echo '<select id="info_linked" onchange="'
																					.'gE(\'info_linked_id\').value = this.options[this.selectedIndex].value;'
																					.'gE(\'info_linked_title\').value = this.options[this.selectedIndex].innerHTML; ">';
		if ($result_nb > 0) {
			echo '<option>Sélectionnez votre information.</option>';
			while ($infos_displayed = $db->fetchArray($result)) {
				$title = html_entity_decode(stripslashes($infos_displayed[$info_title]), ENT_QUOTES, 'UTF-8');
				$cutTitle = substr($title, 0, 40);
				if( $title != $cutTitle ){
					$cutTitle .= '...';
				}
				echo '<option value="'.$infos_displayed[$info_id].'">'.htmlentities($cutTitle, ENT_QUOTES, 'UTF-8').'</option>';
			}
			echo '</select>';
		} else {
				echo '<option value="0" selected="selected">Aucun résultat ce mois-ci.</option></select>';
		}																																												
	}
	
	
	
	public function displaySelectInfos($infos) {
			global $core, $db;
			include ('config_photo_album.inc');
			
			$tb = Array();
			$tb_max = 5 * ($config['tables_nb'] - 1) + 1;
			$j = 1;
			
			for ($i=1;$i<=$tb_max;$i=$i+5) {
				$tb['name'][$j] = $config['tables'][$i];
				$tb['position'][$j] = $i;
				$j++;
			}
			
			for ($k=1;$k<=$config['tables_nb'];$k++) {
				$info_title[$k] = stripslashes($config['tables'][$tb['position'][$k]+2]);
				$info_id[$k] = $config['tables'][$tb['position'][$k]+3];
				$info_date[$k] = $config['tables'][$tb['position'][$k]+4];
				$query[$k] = "SELECT ".$info_id[$k].", ".$info_title[$k].", ".$info_date[$k]." FROM ".$tb['name'][$k]." WHERE ".$info_date[$k]." < '".$infos['info_year']."-".$infos['info_month']."-31' AND ".$info_date[$k]." > '".$infos['info_year']."-".$infos['info_month']."-01' ORDER BY ".$info_title[$k]." ASC";
			}

			switch ($infos['table_name']) {	
				case $tb['name'][1]:
					$this->buildSelect($query[1], $info_id[1], $info_title[1]);
				break;

				case $tb['name'][2]:
					$this->buildSelect($query[2], $info_id[2], $info_title[2]);
				break;
					
				case $tb['name'][3]:
					$this->buildSelect($query[3], $info_id[3], $info_title[3]);
				break;
				
				default:
					//void
				break;		
			}
	}

}
//End of this file.

?>
