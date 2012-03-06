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


//Le include doit deja contenir a fonction de connexion à Mysql

//Obligatoire
$relativePath = '../..';       //Chemin relatif mpour accéder à la racine de l'arborecense
$SECU = 'all';       //Niveaux de sécurité (voir $session->authPage())
include ($relativePath . '/include/main.inc');

$locale = Locale::getLocale();
// Gère l'affichage du forum
$core->localLoadClass('afficheforumbasic');
  
if($core->loadClass('afficheforum',dirname(__FILE__).'/../skins/'.$core->user->getSkin()))$afficheForum = new afficheforum();
else $afficheForum = new afficheforumBasic();


//Récupère les variables
//on récupère la variable de base de donnée par defaut
//il faut encore faire le boulot si jamais on est pas sur la ds par defaut
if (isset($_GET['db_name']))
  {
  $db_name = $_GET['db_name'];
  }
elseif (isset($_POST['db_name']))
  {
  $db_name = $_POST['db_name'];
  }
else
  {
  $db_name = 'local';
  }


if (isset($_GET['id_salon']))
{
	$id_salon = (int)$_GET['id_salon'];
	$_SESSION['id_salon'] = $id_salon;
}
elseif (isset($_SESSION['id_salon']))
{
	$id_salon = (int)$_SESSION['id_salon'];
}
//si on a pas d'id de salon on redirige
else
{
        $core->url->redirect($core -> makeUrl('applications/forum'));
}

if (!$afficheForum->forum->isReadableSalon($id_salon))
{
	$page -> msgError($locale->display('thread_not_readable','forum'));
	exit();
}
if (isset($_GET['page']))
  $num_page = $_GET['page'];

elseif (isset($_POST['page']))
  $num_page = $_POST['page'];

else
  $num_page = 1;


// Ci-dessous la fonction correspondante au changement de suivi grâce au post
if (isset($_POST["suivi"]))
{
	$id_thread = $_POST['id_thread'];
	$suivi = $afficheForum->forum->getSuivi($id_thread);
	$mail = (isset($_POST['mail']));
	$site = (isset($_POST['site']));
	$afficheForum->forum->typeSuivi($id_thread,$mail,$site);
}
$afficheForum->forum->modo = $afficheForum->forum->isModerateurSalon($id_salon);

if ($afficheForum->forum->modo)	
{
  if (isset($_POST['edit']))
  {
    $id_thread=$_POST['id_thread'];
    switch($_POST['etat'])
    {
      case "drop" :
        $afficheForum->forum->chgtEtatThread($id_thread, 'DROP');
        break;
      case "sticky" :
        $afficheForum->forum->chgtEtatThread($id_thread, 'STICKY');
        break;
      case "locked" :
        $afficheForum->forum->chgtEtatThread($id_thread, 'LOCKED');
        break;
      case "sticky-locked":
        $afficheForum->forum->chgtEtatThread($id_thread, 'STICKY_LOCKED');
        break;
      case "normal" :
        $afficheForum->forum->chgtEtatThread($id_thread, 'NORMAL');
        break;
    }
    $nom = $_POST['nom'];
    $id_salon_new = $_POST['id_salon'];
    if ($afficheForum->forum->isModerateurSalon($id_salon_new))
      $afficheForum->forum->updateThread($id_thread, $nom, $id_salon_new);
    
  }
}


if(isset($_GET['rm'])){
	$afficheForum->forum->readAllSalonMessages($id_salon);
}


$salon_name = $db -> result($db -> query("SELECT nom FROM forum_salon WHERE id='".$id_salon."'"),0);
$droit_ecriture = $db -> result($db -> query("SELECT droit_ecriture FROM forum_salon WHERE forum_salon.id='$id_salon' "),0);

$nb_pages = $afficheForum->forum->getNbPageSalon($id_salon, $afficheForum->forum->modo);

//Construction de la requête pour lister les sujets (thread) du salon
$query = "SELECT * FROM forum_thread";
if (!$afficheForum->forum->modo)
	$query .= " WHERE `drop`='false' AND id_salon='$id_salon'";
	
else
	$query .= " WHERE id_salon='$id_salon'";

$query .= " ORDER BY sticky ASC,date_dernier_poste DESC";
$query .= " LIMIT ".($num_page-1) * $afficheForum->forum->config['thread_per_page'].",".$afficheForum->forum->config['thread_per_page'];

$sujets = $db -> query($query);

$ARIANEWIRE = Array(
	'Accueil' => $core->makeUrl('index.php'),
	'Forum' => $core->makeUrl('applications/forum'),
	$salon_name => $core->makeUrl('applications/forum/thread.php').'?id_salon='.$id_salon.'&amp;db_name='.$db_name
);

//Propre à la page
$page = new Page();
$page -> header($locale->display('forum','kernel').' : '.$salon_name,'thread');

//Génération de l'HTML
$afficheForum -> afficheSalon($id_salon, $num_page, $db_name, $salon_name, $droit_ecriture, $nb_pages , $sujets);

// Affiche la légende des types de message (nonvu / nouveau msg / pas de nouveau)
$afficheForum->forumLegende('thread');

$page -> footer();
?>
