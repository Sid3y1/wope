<?php

$relativePath = '../../..';
$SECU = 'log';
include_once (dirname(__FILE__).'/'.$relativePath . '/include/main.inc');
// include_once('../module_photos.class.inc');

$core->loadClass('module_photos',dirname('__FILE__').'/..');


$id_module = $_GET['id_photos'];
$action = $_GET['geniutt_action'];

$photos = unserialize($_SESSION['module_photos_'.$id_module]);


switch ($action) {

	case 'nextPhoto':
		$photos->changePhoto($photos->photos_infos['current'], 'next');
	break;

	case 'prevPhoto':
		$photos->changePhoto($photos->photos_infos['current'], 'prev');
	break;

	default:
		//voif.
	break;

}

$_SESSION['module_photos_'.$id_module] = serialize($photos);

// End of the file.
?>
