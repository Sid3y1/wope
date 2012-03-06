<?php
$SECU = 'admin webmaster';
include('../../../../include/main.inc');
$core->loadClass('trade_admin',dirname(__FILE__).'/..');

$trade_admin = unserialize($_SESSION['trade_admin_'.$_REQUEST['id_admin']]);

switch($_REQUEST['geniutt_action']){
	case 'display_items':
		$trade_admin->displayItems($_GET['cur_page']);
		break;
	case 'display_search_item':
		$trade_admin->displaySearchItem();
		break;
	case 'display_categories':
		$trade_admin->displayCategories();
		break;
	case 'display_add_item':
		$trade_admin->displayAddItem($_GET['cur_page']);
		break;
	case 'display_add_category':
		$trade_admin->displayAddCategory();
		break;
	case 'display_update_item':
		$trade_admin->displayUpdateItem($_GET['item'], $_GET['case'], $_GET['cur_page']);
		break;
	case 'display_update_category':
		$trade_admin->displayUpdateCategory($_GET['cat'], $_GET['main']);
		break;
	case 'display_delete_category':
		$trade_admin->displayDeleteCategory($_GET['cat'], $_GET['name'], $_GET['main']);
		break;
	case 'delete_category':
		$trade_admin->deleteCategory($_POST);
		break;
	case 'save_item':
		$trade_admin->saveItem($_POST);
		break;
	case 'moderate_item':
		$trade_admin->moderateItem($_GET['item'], $_GET['moderated'], $_GET['case'], $_GET['cur_page']);
		break;
	case 'delete_item':
		$trade_admin->deleteItem($_GET['item'], $_GET['case'], $_GET['cur_page']);
		break;
	case 'update_category':
		$trade_admin->updateCategory($_POST);
		break;
	case 'add_category':
		$trade_admin->addCategory($_POST);
		break;
	case 'update_item':
		$trade_admin->updateItem($_POST);
		break;
	case 'search_item':
		$trade_admin->displayQueryResult( (isset($_GET['criteria_place']) ? $_GET['criteria_place'] : $_POST), (isset($_GET['cur_page'])) ? $_GET['cur_page'] : 1 );
		break;
	default:
		//empty
		break;
}
?>
