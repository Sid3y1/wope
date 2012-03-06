<?php
$SECU = 'user';
$relative_path = '../../';
include($relative_path.'include/main.inc');
include('config_trade.inc');

$ARIANEWIRE = Array (
	'Accueil' => $core->makeUrl('/index.php'),
	'Annonces' => $core->makeUrl('/applications/trade')
);

$page = new Page();
$page->header('Petites annonces','trade');

$core->javascriptCore->loadContextScripts('applications/trade','../..','trade');

$core->localLoadClass('trade');

$trade = new Trade();


$part = (isset($_GET['part'])?$_GET['part']:null);
$content = (isset($_GET['content'])?$_GET['content']:null);
$option = (isset($_GET['option'])?$_GET['option']:null);

echo '<div>'.$page->adminLink('app','trade',$config_trade['admin_rights']).'</div><div class="clear"></div>';

echo '<div class="meetmind_menu_block">
				<ul class="meetmind_menu">
					<li class="trade_menu_bug_ie"><a class="mm_first '.(($part == '') ? 'selected_part' : '').'" href="'.$core->makeUrl('applications/trade/index.php').'">Rechercher</a></li>
					<li><a class="'.(($part == 'create') ? 'selected_part' : '').'" href="'.$core->makeUrl('applications/trade/index.php').'?part=create">Créer une annonce</a></li>
					<li><a class="'.(($part == 'myspace' && $option != 'false') ? 'selected_part' : '').' mm_last" href="'.$core->makeUrl('applications/trade/index.php').'?part=myspace&content='.$core->getUserId().'&option=true">Mon espace</a></li>
				</ul>
			</div>';

$trade->display($part,$content,$option);

$page->footer();
?>
