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
    génération de l'html de la page forum en fonction du style (comme un template)
  
*/

//Obligatoire
$relativePath = '../..';       //Chemin relatif mpour accéder à la racine de l'arborecense
$SECU = 'all';       //Niveaux de sécurité (voir $session->authPage())

include ($relativePath . '/include/main.inc');

$ARIANEWIRE = Array(
	'Accueil'=>$core->makeUrl('index.php'),
	'Forum'=>$core->makeUrl('applications/forum')
);

$locale = Locale::getLocale();

// Gère l'affichage du forum
$core->localLoadClass('afficheforumbasic');
  
if($core->loadClass('afficheforum',dirname(__FILE__).'/../skins/'.$core->user->getSkin()))$afficheForum = new afficheforum();
else $afficheForum = new afficheforumBasic();

$core->loadClass('chronos',"include");
$chronos = new Chronos();
$chronos->start("forum");


//Récupère les variables
//on récupère la variable de base de donnée par defaut
//il faut encore faire le boulot si jamais on est pas sur la ds par defaut
$db_name = isset($_GET['db_name']) ? $_GET['db_name'] : ( isset($_POST['db_name']) ? $_POST['db_name'] : '' );

$action = isset($_GET['wope_action']) ? $_GET['wope_action'] : ( isset($_POST['wope_action']) ? $_POST['wope_action'] : '' );

$id_sujet = isset($_GET['id_thread']) ? $_GET['id_thread'] : ( isset($_POST['id_thread']) ? $_POST['id_thread'] : -1 );
 
$id_salon = $id_sujet != 'new_thread' ? $db->result($db->query("SELECT id_salon FROM forum_thread WHERE id='".$id_sujet."'"),0) : $_POST['id_salon'];

$id_message = isset($_GET['id_message']) ? $_GET['id_message'] : ( isset($_POST['id_message']) ? $_POST['id_message'] : '' );

$salon_name = isset($id_salon) && $id_salon != '' ? $db->result($db->query("SELECT nom FROM forum_salon WHERE id='".$id_salon."'"),0) : '';
$sujet_name = isset($id_sujet) && $id_sujet != '' ? $afficheForum->forum->parseSujet($db->result($db->query("SELECT nom_thread FROM forum_thread WHERE id='".$id_sujet."'"), 0)) : '';

if($salon_name != ''){$ARIANEWIRE[$salon_name] = $core->makeUrl('applications/forum/thread.php').'?id_salon='.$id_salon.'&db_name='.$db_name;}
if($sujet_name != ''){$ARIANEWIRE[$sujet_name] = $core->makeUrl('applications/forum/affiche_thread.php').'?id_thread='.$id_sujet.'&db_name='.$db_name;}
 
$page = new Page();
$page->header($locale->display('forum','kernel').' : '.$salon_name.' : '.$sujet_name, 'forum');

//part reserved to moderators
$afficheForum->forum->modo = $afficheForum->forum->isModerateurSalon($id_salon);


if($action == 'save_edit_m'){
	if($_POST['poster_login'] == $core->getUserId() || $afficheForum->forum->modo){
		$afficheForum->forum->editMessage($_POST);
	}
}


if ($afficheForum->forum->modo)
{
	if (isset($_GET['efface_attachement']))
  {
  	$id_message=$_GET['id_message'];
    $db->query("UPDATE forum_message SET attachement='', dl_size='0',dl_actif='0' WHERE id='".$id_message."'");
    $dir = dirname(__FILE__)."/attachement/";
    $d = opendir($dir);
    while ($file = readdir($d))
    {
    	if (is_file($dir.$file) && ereg("^".$id_message."_.*$", $file))
      {
      	if (!unlink ($dir.$file))
        {
					//Propre à la page
        	$page->msgError($locale->display('del_error','forum').' : ' . $file);
        }
      }
    }
    closedir($d);
	}
}

//end of part reserved to moderators

if( $id_message != '' ){
	$num_page = $afficheForum->forum->getPage($id_message, $afficheForum->forum->modo);
}elseif( isset($_GET['page']) ){
	$num_page = $_GET['page'];
}else{
	//Ben la je plain le mec qui doit comprendre la ligne d'en dessous !
	$requete = "SELECT count(login) FROM forum_login WHERE login='".$core->getUserId()."' AND thread='$id_sujet'";
	$requete2 = "SELECT id_last_msg FROM forum_login WHERE login='".$core->getUserId()."' AND thread='$id_sujet'";
	$num_page = $afficheForum->forum->getPage(($db->result($db->query($requete),0)>0) ? ($db->result($db->query($requete2),0)):0, $afficheForum->forum->modo);
	//TORENEGOCIATE
}

//Ajouts d'enregistrement
if (isset($_POST['data']) && $afficheForum->forum->isWriteableSalon($id_salon))
{    
  //Défini les préférences
  $surnom = (isset($_POST['surnom']))?$_POST['surnom']:'';
	$afficheForum->forum->set_pref($core->getUserId(),'forum_pseudo',$_POST['surnom']);
  $email = (isset($_POST['email']))?$_POST['email']:'';
	$afficheForum->forum->set_pref($core->user->getUserId(),'forum_email',$_POST['email']);
  $myavatar = (isset($_POST['myavatar']))?$_POST['myavatar']:'';
	$afficheForum->forum->set_pref($core->user->getUserId(),'forum_myavatar',$_POST['myavatar']);
  $mysignature = (isset($_POST['mysignature']))?$_POST['mysignature']:'';
	$core->user->setPref($core->user->getUserId(),'forum_mysignature',$_POST['mysignature']);

  //Regarde si il s'agit de l'ajout d'un message à un thread ou autre chose
  if ($id_sujet == "new_thread")
  {
  	$id_sujet = $afficheForum->forum->newThread($id_salon,$_POST['sujet'],$_POST['surnom']);
	}

  if (!isset($_FILES['upload']))
  {
    $upload_name='';
    $upload_size='';
	}
  else 
  {
  	$upload_name=$_FILES['upload']['name'];
  	$upload_size=$_FILES['upload']['size'];
	}
        
  if($afficheForum->forum->get_pref($core->user->getUserId(),"forum_mysignature")=='true')
  {
    $signature = $afficheForum->forum->get_pref($core->getUserId(),"forum_signature");
	}
  else
  {
  	$signature='';
	}
	
  //ici on entre le message avec toutes les infos
  if($afficheForum->forum->isAttachement($id_sujet))
  {
    $id_message = $afficheForum->forum->newMessage($id_sujet, $_POST['id_salon'],$_POST['sujet'],$_POST['message'],$signature,$_POST['surnom'],$_POST['uniqid'], isset($_POST['email']) ? $_POST['email'] : '', isset($_POST['avatar']) ? $_POST['avatar'] : '', $afficheForum->forum->get_pref($core->getUserId(),"forum_myavatar"),$_POST['fm_id']);
	}
  else
  {
  	$id_message = $afficheForum->forum->newMessage($id_sujet, $_POST['id_salon'],$_POST['sujet'],$_POST['message'],$signature,$_POST['surnom'],$_POST['uniqid'], isset($_POST['email']) ? $_POST['email'] : '', isset($_POST['avatar']) ? $_POST['avatar'] : '', $afficheForum->forum->get_pref($core->getUserId(),"forum_myavatar"),'');
	}
	
  if ($id_message>0)
  {
  	$num_page = $afficheForum->forum->getPage($id_message,$afficheForum->forum->modo);
	}
  else
  {
  	$num_page = $afficheForum->forum->getPage($_POST['id_message'],$afficheForum->forum->modo);
	}

  if (isset($_FILES['upload']) && $_FILES['upload']['size']>0 && $afficheForum->forum->isAttachement($id_sujet))
  {
  	move_uploaded_file($_FILES['upload']['tmp_name'],dirname(__FILE__).'/attachement/'.$id_message."_".str_replace(' ','_',$_FILES['upload']['name']));
	}

}

//on vérifie si l'utilisateur a le droit d'etre ici
if (!$afficheForum->forum->isReadable($id_sujet))
{
	$page->msgInfo($locale->display('relog','kernel'));
	$page->footer();
	exit();
}
 

//Vérifie que le thread est accessible
$requete = "SELECT count(id) FROM forum_thread WHERE id='".$id_sujet."'";
if( $db->result( $db->query($requete.( (!$afficheForum->forum->modo) ? "AND `drop`='false'" : "" )) , 0 ) == 1 )
{


	//ce qui suit n'est pas utilisé ??
  $requete = "SELECT max(id) FROM forum_message WHERE id_thread='".$id_sujet."'";
  $lastMessage = $db->result($db->query($requete.((!$afficheForum->forum->modo) ? "AND score>'0'" : "")),0);
    
  $requete = "SELECT Count(id) FROM forum_message WHERE id_thread='".$id_sujet."'";        
  $nb_pages = intval(($db->result($db->query($requete . ((!$afficheForum->forum->modo)? " AND score > 0" : "")), 0)-1) / $afficheForum->forum->config['msg_per_page'] ) + 1;
    
  $requete = "SELECT mail,site FROM forum_login WHERE login='".$core->getUserId()."' AND thread='".$id_sujet."'";
  $suivi = $db->fetchArray($db->query($requete));
  $query = "SELECT fm.*, DATE(ui.dateCreation) AS inscription_date, COUNT(um.id) AS author_msg_nb FROM forum_message um RIGHT JOIN( forum_message fm INNER JOIN usersInfo ui ON fm.login=ui.id ) ON um.login = fm.login WHERE fm.id_thread='".$id_sujet."'";
  if (!$afficheForum->forum->modo)
  {
  	$query .= " AND fm.score > 0 ";
	}
  
	$query .= " GROUP BY fm.id ORDER BY fm.id LIMIT " . ($num_page - 1) * $afficheForum->forum->config['msg_per_page'] . "," . $afficheForum->forum->config['msg_per_page'];
  $messages = $db->query($query);
  
  //on détermine les valeurs par defaut, si jamais on a pas de message dans le topic :
  
  $droit_ecriture = $db->result($db->query("SELECT forum_salon.droit_ecriture FROM forum_salon,forum_thread WHERE forum_thread.id='".$id_sujet."' AND forum_thread.id_salon=forum_salon.id"),0);

  $afficheForum->afficheMessages($id_salon, $id_sujet, $db_name, $salon_name, $sujet_name, $nb_pages, $num_page, $suivi, $messages, $droit_ecriture);

}
else
{
  echo $locale->display('no_thread','forum');
}

$chronos->stop('forum');

if($core->verifDroits('admin')){
	$chronos->display();
}
$page->footer();

?>
