<?php

/**************************
*
*	PHOTO_ALBUM -> AJAX.PHP
*		Executes all the AJAX requests
*
*	@author : Sylvain Ramboz
* @date : 02/2007
*
***************************/

$SECU = 'user';
$relativePath = '../../../';
include_once ($relativePath . 'include/main.inc');
include_once ('../photo_album.class.inc');
include ('../config_photo_album.inc');
		
		$core->loadClass('tag',dirname(__FILE__).'/../../../../include/engine/tag');
	  $tag = new Tag('photo_album_photos',$core,$db);

$album = unserialize($_SESSION['photo_album_'.$_REQUEST['id_album']]);

switch ($_REQUEST['geniutt_action']) {


	// I. DISPLAY OF THE ALBUMS
	case 'displayMyAlbum':
		if (isset($_GET['cur_page'])) {
			$album->displayAlbum($_GET['cur_page'], $config['queries'][1]);
		} else {
				$album->displayAlbum(1, $config['queries'][1]);
		}
	break;

	case 'displayPublicAlbums':
		if (isset($_GET['cur_page'])) {
			$album->displayAlbum($_GET['cur_page'], $config['queries'][2], 'publicAlbums');
		} else {
				$album->displayAlbum(1, $config['queries'][2], 'publicAlbums');
		}
	break;

	case 'displayLastAlbums':
		if (isset($_GET['cur_page'])) {
			$album->displayAlbums($_GET['cur_page'], $config['queries'][3]);
		} else {
				$album->displayAlbums(1, $config['queries'][3]);
		}
	break;


	// II. DISPLAY OF A TARGET ALBUM
	case 'displayTargetAlbum':
		if (isset($_GET['cur_page'])) {
			$album->displayAlbum($_GET['cur_page'], $config['queries'][4].$_GET['target']." ORDER BY photo.id DESC");
		} else {
				$album->displayAlbum(1, $config['queries'][4].$_GET['target']);
		}
	break;


	// III. CREATION OF A PHOTO
		// Method which display the first step of the creation (the method to save the informations is launched in index.php)
	case 'displayAddPhoto':
		$album->displayAdd();
	break;
		// Method which build the select to link a photo to an information of the site (in the 1st step)
	case 'displaySelectInfos':
		$album->displaySelectInfos($_GET);
	break;
		// 2nd Step : this method allows user to add tags to his new photo	
	case 'addtagToPhoto':
		$id_info_to_tag = $tag->getIdInformation('photo_album_photos', $_GET['id_information']);
		if ($_GET['photo_tag'] != '') {
			$id_tag_to_add = $tag->getIdTagName($_GET['photo_tag']);
  	  $tag->addTagInformation($id_info_to_tag, $id_tag_to_add, false);
		}
	break;
		// Method which saves the 2nd step
	case 'saveStep2':
		if (isset($_POST['title'])) {
			if ($_POST['title'] != '') {
				$album->saveStep2($_POST['target_album'], $_POST['title'], $_POST['description']);
			}
		}
		$album->displayAlbum(1, $config['queries'][1]);
	break;

	// III. EDITION
		// Modify Tagging, Link, Description & Title
	case 'displayEditPhoto':
		$album->updatePhoto($_GET['id_photo'], $_GET['id_file']);
	break;
		// Delete Photo (from the album only)
	case 'deletePhoto':
		$album->deletePhoto($_GET['id_photo']);
		$album->displayAlbum(1, $config['queries'][1]);
	break;
		// Method which saves the modifications
	case 'saveModifsPhoto':
		$album->saveModifsPhoto($_POST);
		$album->displayAlbum(1, $config['queries'][1]);
	break;


	// IV. PHOTOS AND ALBUMS SEARCH
		// IV Albums
	case 'displayAlbumsSearch':
		$album->displayAlbumsSearch();
	break;
	case 'searchAlbums':
		$search_query = $album->buildAlbumsQuery($_POST);
		$album->displayAlbums(1, $search_query, 'searchAlbums');
	break;
	case 'displaySearchedAlbums':
		$album->displayAlbums($_GET['cur_page'], $album->current_query, 'searchAlbums');
	break;

		// Photos
	case 'displayPhotosSearch':
		$album->displayPhotosSearch();
	break;	
	case 'searchPhotos':
		$search_query = $album->buildPhotosQuery($_POST);
		$album->displayAlbum(1, $search_query, 'searchPhotos');
	break;
	case 'displaySearchedPhotos':
		$album->displayAlbum($_GET['cur_page'], $album->current_query, 'searchPhotos');
	break;
	
		// Home Search
	case 'homeSearch':
		$search_query = $album->buildHomeQuery($_POST);
		$album->displayAlbum(1, $search_query);
	break;


	default:
		//void
	break;
}
$_SESSION['photo_album_'.$_REQUEST['id_album']] = serialize($album);



//End of this file.
?>
