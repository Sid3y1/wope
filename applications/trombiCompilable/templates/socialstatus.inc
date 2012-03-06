<?php
$SECU = 'log';
$relative_path = '../..';
include($relative_path.'/include/config.inc');
$core->loadClass('trombi', dirname(__FILE__).'/..');

$action = isset($_GET['geniutt_action']) ? $_GET['geniutt_action'] : ( isset($_POST['geniutt_action']) ? $_POST['geniutt_action'] : '' ) ;

$id_trombi = isset($_GET['id_trombi']) ? $_GET['id_trombi'] : ( isset($_POST['id_trombi']) ? $_POST['id_trombi'] : '' );

$trombi = unserialize($_SESSION['trombi_'.$id_trombi]);

switch($action){

	case 'save_status':
		$trombi->saveStatus($_POST['social_status']);
		break;
		
	default:
		break;
		
}

?>
