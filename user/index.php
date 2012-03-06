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


$SECU='log';
include('../include/main.inc');
$ARIANEWIRE = Array('Accueil' => $core->makeUrl('index.php'),'Mon espace'=>$core->makeUrl('user'));

$core->localLoadClass('user_admin');
$admin = new UserAdmin('user');

$page = new Page();
$page->header('Mes Préférences','admin');

$core->javascriptCore->loadContextScripts('user_admin','..');
echo '<script type="text/javascript" src="../modules/module_calepin/javascript/module_calepin.js"></script>';

if(isset($_GET['part']) && $_GET['part']!='' ){
	$default_part = $_GET['part'];
}else{
	$default_part = 'home';
}

if(isset($_GET['content'])){
  $content = $_GET['content'];
}else{
	$content = '';
}

if(isset($_GET['option'])){
  $option = $_GET['option'];
}else{
	$option = '';
}
		
echo '<div class="meetmind_menu_block">
	<ul class="meetmind_menu">
		<li><a '.(($default_part=='home')?'class="selected_part"':'class="mm_first"').' href="index.php?part=home">Mon espace</a></li>
		<li><a '.(($default_part=='account')?'class="selected_part"':'').' href="index.php?part=account">Mon compte</a></li>
		<li><a '.(($default_part=='info')?'class="selected_part"':'').' href="index.php?part=info">Mes informations</a></li>
		<li><a '.(($default_part=='pref')?'class="selected_part"':'').' href="index.php?part=pref">Mes préférences</a></li>
		<li><a '.(($default_part=='category')?'class="selected_part"':'class="mm_last"').' href="index.php?part=category">Mes catégories</a></li>
	</ul>
</div>';
$admin->displayAdmin($default_part,$content,$option);

$page->footer();

?>
