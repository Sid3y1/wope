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
   $SECU='moderateur';
   include_once($relativePath . '/include/main.inc');
	 
	$ARIANEWIRE = Array(
		'Accueil' => $core->makeUrl('index.php'),
		'Forum' => $core->makeUrl('applications/forum')
	);

	$core->loadClass('form');
	$form = new Form();
	 
   //Initialisation du forum
	 $core->localLoadClass('afficheforumbasic');
	 $affiche_forum = new afficheforumBasic();

   if(isset($_GET['id_salon'])) {
        $id_salon = $_GET['id_salon'];
        $salon = $affiche_forum->forum->getSalonData($id_salon);
				
	 		$ARIANEWIRE[$salon['nom']] = $core->makeUrl('applications/forum/thread.php').'?id_salon='.$id_salon.'&db_name='.$db_name;
   		//Propre à la page
   		$page = new Page();
   		$page->header(locale::display('forum_edit_room','forum'), 'forum');
	 		$page->moduleHeader('Edition du salon','edit_salon');
		
			$affiche_forum->displayDeleteSalon($id_salon);
			echo '<div class="clear"></div>';

   } else {
   		$page = new Page();
   		$page->header('Forum : nouveau salon','forum');
	 		$page->moduleHeader('Nouveau salon','edit_salon');
        $id_salon="new";
        $salon['id']="new";
        $salon['nom']="";
        $salon['descriptif']="";
        $salon['droit_lecture']="all";
        $salon['droit_ecriture']="user";
        $salon['droit_attachement']="moderateur";
        $salon['droit_moderation']="moderateur";
  }

	echo '
	<form class="edit_form" action="salon.php" method="POST" name="post_salon">
  	<input type="hidden" name="uniqid" value="'.uniqid("",true).uniqid("",true).'" />
    <input type="hidden" name="id_salon" value="'.$id_salon.'" />
		<p class="form-input"><label>'.locale::display('name','kernel').' :</label>
      <input type=text name="nom" size="50" class="forum_form_titre" maxlength="70" value="'.$salon['nom'].'" /></p>
    <p class="form-input"><label>'.locale::display('description','kernel').' :</label>
      <textarea name="descriptif" cols="75" rows="10" class="forum_form">'.$salon['descriptif'].'</textarea></p>
		<p class="form-input"><label>'.locale::display('right_read','forum').' :</label>
			<select name="droit_lecture" multiple="multiple" size="4">';
	$form->selectFromDb('listes', 'liste', '', '', $salon["droit_lecture"], '');
	echo '</select></p>
    <p class="form-input"><label>'.locale::display('right_write','forum').' :</label>
			<select name="droit_ecriture" multiple="multiple" size="4">';
	$form->selectFromDb('listes', 'liste', '', '', $salon["droit_ecriture"], '');
	echo '</select></p>
    <p class="form-input"><label>'.locale::display('right_attach','forum').' :</label>
			<select name="droit_attachement" multiple="multiple" size="4">';
	$form->selectFromDb('listes', 'liste', '', '', $salon["droit_attachement"], '');
	echo '</select></p>
    <p class="form-input"><label>'.locale::display('right_mod','forum').' :</label>
			<select name="droit_moderation" multiple="multiple" size="4" >';
	$form->selectFromDb('listes', 'liste', '', '', $salon["droit_moderation"], '');
	echo '</select></p>
    <p class="form-submit"><input type="submit" value="'.locale::display('send','kernel').'" name="data"></p>
  </form>';
	
	$page->moduleFooter();
  $page->footer();
?>
