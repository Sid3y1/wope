<?php

$SECU = 'user';

$ARIANEWIRE = Array (
	"Accueil" => "../index.php",
	"Album photo" => "index.php"
);
		
include_once ('../../include/main.inc');
include_once ('./photo_album.class.inc');
include ('./config_photo_album.inc');

$album = new Photo_Album();

$page = new Page();
$page->header('Album Photo','photo_album');
$core->javascriptCore->loadContextScripts('applications/photo_album','../..','photo_album');

echo '<div class="menu_block">';
$page->ModuleHeader('Menu','');

$album->displayMenu();

$page->Modulefooter();
echo '</div>';

if ( !isset($_POST['geniutt_action']) && !isset($_GET['ga']) ) {
	$module_title = 'Les dernières photos';
} elseif ( isset($_GET['ga']) && $_GET['ga'] == 'dUP') {
		$infos = $db->fetchArray($db->query("SELECT firstname, lastname FROM trombi_trombi WHERE id_user = '".$_GET['ud']."' "));
		$module_title = 'Photos de '.$infos['firstname'].' '.$infos['lastname'];
} elseif ( isset($_GET['ga']) && $_GET['ga'] == 'dPFM') {
		$module_title = 'Votre recherche';
} else {
		$module_title = 'Import de photo : Etape n°2';
}

echo '<div class="photo_album_block">';

$page->ModuleHeader('<span id="pa_part">'.$module_title.'</span>','');

// Display the "Google like" search
if ( !isset($_POST['geniutt_action']) || $_GET['ga'] == 'dPFM' || $_GET['ga'] == 'dUP') {
	$album->displayHomeSearch();
}

echo '<div id="container">';

// Display the step 2 of the photo import (we were obliged to do so because of an IE bug when there is an AJAX upload of a photo)
if (isset($_POST['geniutt_action']) && $_POST['geniutt_action'] == 'savePhoto') {
		$obj_album = unserialize($_SESSION['photo_album_'.$_REQUEST['id_album']]);
    $infos = $obj_album->savePhoto($_POST);
		$obj_album->displayAddStep2($infos['linking_status'], $infos['id_photo'], $infos['id_file'], $infos['id_album'], $infos['album_title']);		
} 

// Display the picture the user clicked on in the photos module
elseif (isset($_GET['ga']) && $_GET['ga'] == 'dPFM') {
		$query = $album->buildHomeQuery($_GET);
		$album->displayAlbum(1, $query, 'publicAlbums');
}

// Display the photos of the target user
elseif (isset($_GET['ga']) && $_GET['ga'] == 'dUP') {
	$album->displayUserPhotos($_GET['ud']);
}

// Display the last photos (default)
else {																																																												 
		$album->displayAlbum(1, $config['queries'][2], 'publicAlbums');
}

echo '</div>';
$page->Modulefooter();
echo '</div>';

$page->footer();



//End of this file.

?>
