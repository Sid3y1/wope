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


//Le include doit deja contenir la fonction de connexion a�Mysql
$idPage = 131;
//Obligatoire
$relativePath = '../..';       //Chemin relatif pour acceder a la racine de l'arborecense
//$niveauSecu = array('etu','ext');       //Niveaux de securite (voir $session->authPage())
$niveauSecu='all';
include ($relativePath . '/include/main.inc');
//Propre a� la page
$page = new Page();
$page->header("SubscribeManager");
$id_event = $_GET['id_event'];
if($db->result($db->query("SELECT login_admin FROM subscribeManager_event WHERE id_event='".$id_event."'"),0)!=$core->getLogin()){
	echo 'Vous n\'etes pas l\'admin !!!';
	exit();
}
if(isset($_GET['id_event'])){
	if($db->result($db->query("SELECT count( * ) FROM usersInfo WHERE login = '".$_POST['login']."'"),0)==1){
		$db->query("INSERT INTO `subscribeManager_gens` ( `id` , `login` , `id_event` , `date` , `com` , `etat` ) VALUES ( '', '".trim($_POST['login'])."', '".$_GET['id_event']."',now(), 'By login', 'Y')"); 	
		$page->msgInfo("Login Ajoute");
	}
	$util->redirect('subscribeManager/addothers.php?id_event='.$id_event);
}else{
	$page->mdgError("Pas de login c pas cool :(");
}
$page->footer();
?>
