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


//Le include doit deja contenir a fonction de connexion à Mysql
	//Obligatoire
  $relativePath = '../..';       //Chemin relatif mpour accéder à la racine de l'arborecense
  //$niveauSecu = array('etu','ext');       //Niveaux de sécurité (voir $session->authPage())
  $SECU='all';
  include ($relativePath . '/include/main.inc');

  //Initialisation du forum
	$core->localLoadClass('afficheforumbasic');
	$affiche_forum = new afficheforumBasic();
	 
 if (isset($_GET['id_salon'])) {
 	 $id_salon = $_GET['id_salon'];
 }else{
	 $id_salon=(int)$_SESSION['id_salon'];
 }

 if(isset($_REQUEST['db_name'])){
		$db_name = $_REQUEST['db_name'];
 }

	$salon_tmp = $affiche_forum->forum->getSalonData($id_salon);
	$salon_name = $salon_tmp['nom'];

	$ARIANEWIRE = Array(
		'Accueil' => $core->makeUrl('index.php'),
		'Forum' => $core->makeUrl('applications/forum'),
		$salon_name => $core->makeUrl('applications/forum/thread.php').'?id_salon='.$id_salon.'&amp;db_name='.$db_name
	);
	
	//Propre à la page
	$page = new Page();
	$page->header(locale::display('forum_new_thread','forum'),'forum');
	
	echo '<div class="new_thread">';
	$page->moduleHeader('Nouveau Sujet','new_thread');
	
  //div for ajax preview
  echo '<div id="preview_reply" class="message preview_msg"></div>
				<div class="clear"></div>';

	$affiche_forum->displayAddMessage('salon', $id_salon, $db_name);
		
	$page->moduleFooter();
	echo '</div>';
	$page->footer();

?>
