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


/*
  Rôle du fichier :
    génération de l'html de la page forum en fonction du style (comme un template)
*/

// Obligatoire
$relativePath = '../..';// Chemin relatif mpour accéder à la racine de l'arborecense
$SECU = 'all';// Niveaux de sécurité (voir $session->authPage())
include ($relativePath . '/include/main.inc');
$ARIANEWIRE = Array('Accueil'=>$core->makeUrl('index.php'),'Forum'=>$core->makeUrl('applications/forum'));
// Propre à la page
$page = new Page();
$locale = Locale::getLocale();


// Gère l'affichage du forum
$core->localLoadClass('afficheforumbasic');
  
if($core->loadClass('afficheforum',dirname(__FILE__). '/../skins/'.$core->user->getSkin()))
$afficheForum = new afficheforum();
else $afficheForum = new afficheforumBasic();


$core->loadClass('chronos');
$chronos = new Chronos();
$chronos->start("forum");

$chronos->start("Initialisation");

$page->header($locale->display('forum','kernel'), 'forum');
// Initialisation du forum
$db_name = "";

if(isset($_GET['rm'])){
	$afficheForum->forum->readAllMessages();
}

foreach(array_keys($afficheForum->forum->db_array) as $db_name)
{
  $db->connect($afficheForum->forum->db_array[$db_name]["host"],$afficheForum->forum->db_array[$db_name]["base"],$afficheForum->forum->db_array[$db_name]["user"],$afficheForum->forum->db_array[$db_name]["pass"]);
  if (isset($_POST['uniqid']))
  {
		$nom = $_POST['nom'];
		$descriptif = $_POST['descriptif'];
		$droit_lecture = $_POST['droit_lecture'];
		$salon['droit_ecriture'] = $droit_ecriture = $_POST['droit_ecriture'];
		$droit_moderation = $_POST['droit_moderation'];
		$droit_attachement = $_POST['droit_attachement'];
		$uniqid = $_POST['uniqid'];
		$id_salon = $_POST['id_salon'];
		if ($id_salon == "new")
		{
		 $afficheForum->forum->newSalon($nom, $descriptif, $droit_lecture, $droit_ecriture, $droit_moderation, $droit_attachement, $uniqid);
		}
		else
		{
		 $afficheForum->forum->updateSalon($id_salon, $nom, $descriptif, $droit_lecture, $droit_ecriture, $droit_moderation, $droit_attachement);
		}
  }
  $chronos->stop("Initialisation");
  $alt='alton';
  $req = "";
  
    
  if(isset($core->user->droits))
  {
		if($core->verifDroits('moderateur')){
			echo'<a class="link_button left" href="edit_salon.php?db_name='. $db_name .'">Ajouter un nouveau salon</a>';
		}
		
		echo'
				 <div class="clear"></div>
				 <div class="module salons">
				 	<div class="title">
						<div class="extraDiv1">
							<span>&nbsp;</span>
						</div>
						<h2>Index du Forum</h2>
						<a class="mark_as_read" href="salon.php?rm">Marquer tous les messages comme lus</a>
						<div class="clear"></div>
					</div>
					<div class="content">';
			
    foreach($core->user->droits as $droit)
    {
      $req .= " OR droit_lecture = '".$droit."' ";
    }
    $ressalon = $db->query("SELECT * FROM forum_salon WHERE droit_lecture='all' ".$req." ORDER BY nom ASC");
		
		$i = 1;
    while($salon = $db->fetchArray($ressalon))
    { 
			if ($i%2) {
				$class = 'color1';
			} else {
					$class = 'color2';
				}
				
      $alt = ($alt == 'alton') ? "altoff" : "alton";
  		
		  $last_poster = $db->fetchArray($db->query("SELECT ft.dernier_login AS login,  ft.dernier_poste AS pseudo, ft.date_dernier_poste AS date FROM forum_thread ft INNER JOIN usersInfo ui ON ft.dernier_login = ui.id WHERE ft.id_salon='".$salon['id']."' AND ft.drop = 'false' ORDER BY ft.date_dernier_poste DESC LIMIT 1 "));
			
	    $last_sujets = $db->query("SELECT ft.id, ft.nom_thread FROM forum_thread ft WHERE ft.id_salon='".$salon['id']."' AND ft.drop='false' ORDER BY ft.date_dernier_poste DESC LIMIT 0,4");
	    
	    //Génère un salon
			
	    $afficheForum->salon($alt, $db_name, $salon, $last_sujets, $last_poster, $class);
   		$i++; 
    }
  }
}// Fin foreach

echo '</div></div>';

$afficheForum->forumLegende('salon');

$chronos->stop("forum");
//$chronos -> display();
$page->footer();
  
?>
