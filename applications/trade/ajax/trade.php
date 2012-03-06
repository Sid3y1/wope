<?php
$SECU = 'user';
$relative_path = '../../..';
include($relative_path.'/include/main.inc');

$core->loadClass('trade',dirname(__FILE__).'/..');
$trade = unserialize($_SESSION['trade_'.$_REQUEST['id_trade']]);
$page = new Page();

switch($_REQUEST['geniutt_action']){
	case 'display_last_items':
		$trade->displayLastItems();
		break;
	case 'display_search_module':
		$trade->displaySearchModule();
		break;
	case 'display_trader_items':
		$trade->displayTraderItems($_GET['trader'], $_GET['myspace']);
		break;
	case 'display_menu_myspace':
		$trade->displayMenuMyspace($_GET['trader'], $_GET['myspace']);
		break;
	case 'display_myspace':
		$page->msgInfo('Modification enregistrée');
		$trade->displayMyspace($_GET['trader'], $_GET['myspace']);
		break;
	case 'search':
		$trade->displayQueryResult( (isset($_GET['criteria_place']) ? $_GET['criteria_place'] : $_POST), (isset($_GET['cur_page'])) ? $_GET['cur_page'] : 1 );
		break;
	case 'display_photo':
		$trade->displayPhoto($_GET['photo']);
		break;
	case 'save_item':
		$trade->saveItem($_POST);
		break;
	case 'alert_cat':
		$trade->alertCat($_GET['cat'], $_GET['is_cat'], $_GET['type'], $_GET['alert']);
		break;
	case 'delete_item':
		$trade->deleteItem($_GET['item']);
		break;
	case 'update_item':
		$trade->displayUpdateItem($_GET['item']);
		break;
	default:
		//void
		break;
}
?>
