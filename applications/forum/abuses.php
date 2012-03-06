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


$relative_path = '../..';       //Chemin relatif mpour accéder à la racine de l'arborecense
$SECU = "moderateur";       //Niveaux de sécurité (voir $session->authPage())
include ($relative_path . '/include/main.inc');
include (dirname(__FILE__)."/afficheforumbasic.class.inc");
$affiche_forum = new afficheForumbasic();

if (isset($_GET['db_name'])){
  $db_name = $_GET['db_name'];
}else{
  $db_name = 'local';
}

if(isset($_GET['id_salon'])){
	$id_salon = (int)$_GET['id_salon'];
}

$salon_name = $db->result($db->query("SELECT nom FROM forum_salon WHERE id='".$id_salon."' "), 0);

$ARIANEWIRE = Array(
	'Accueil' => $core->makeUrl('index.php'),
	'Forum' => $core->makeUrl('applications/forum'),
	$salon_name => $core->makeUrl('applications/forum/thread.php').'?id_salon='.$id_salon.'&amp;db_name='.$db_name
);

$page = new Page();

$page->header(locale::display('forum_abuse_manage','forum'), 'forum');

echo '<div class="manage_abuses">';
$page->moduleHeader('Abus signalés','');

$affiche_forum->displayAbusesList($id_salon, $db_name);

$page->moduleFooter();
echo '</div>';

$page->footer();

?>
