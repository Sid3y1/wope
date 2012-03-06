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


//Obligatoire
$SECU = 'log';       //Niveaux de sÃ©curitÃ© (voir $session->authPage() )

$ARIANEWIRE = Array (
	"Accueil" => "../../index.php",
	"Liens Utiles" => "index.php"
);
		

include ('../../include/main.inc');

$core->loadClass('module_links');
$links = new Module_links();

$page = new Page();
$page->header('Liens utiles','liens_utiles');

echo '<div class="menu_block left">';
$page->ModuleHeader('Menu','');
	$links->displayTypes();
	echo '<div class="clear"></div>';
	$page->adminLink('module','module_links','admin');

$page->ModuleFooter();
echo '</div>';

echo '<div class="list_block left">';
$page->ModuleHeader('Liste des liens utiles','');
	echo '<div id="LinksList">';

	switch ($_GET['display']) {

	case 'all' :
	$links->displayLinksList('','','');
	break;

	default :
	$links->displayLinksType($_GET['display']);
	break;
	
	}
	echo '</div>';
	
$page->ModuleFooter();
echo '</div>';

echo '<div class="clear"></div>';

$page->footer();

?>
