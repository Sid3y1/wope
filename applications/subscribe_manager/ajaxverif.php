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


//Le include doit deja contenir la fonction de connexion a Mysql
$idPage = 133;
//Obligatoire
$relativePath = '../..';       //Chemin relatif mpour acceder a� la racine de l'arborecense
//$niveauSecu = array('etu','ext');       //Niveaux de securite (voir $session->authPage())
$niveauSecu='etu';
include ($relativePath . '/include/main.inc');
//Propre a� la page
$page = new Page();


$id_event = $_GET['id_event'];
if($db->result($db->query("SELECT login_admin FROM subscribeManager_event WHERE id_event='".$id_event."'"),0)!=$core->getLogin()){
	echo 'Vous n\'etes pas l\'admin !!!<h1>'.$id_event.'</h1>';
	exit();
}

//echo '<h1>Page</h1>';
if(!isset($_GET['id_event']) || $_GET['id_event']==''){
	exit("PAS D ID [EZREUR FATALE]");
}
if(!isset($_GET['code']) || $_GET['code']==''){
	exit("PAS DE CODE [EZREUR FATALE]");
}





$res = $db->query("SELECT * FROM usersInfo WHERE codebarre='".$_GET['code']."'");
if($db->numRows($res)==0){
	//ici faudrais metre un formulaire de login
	$logins = "'toto','coucou','boubou'";
	?>
		<form action="ajaxwho.php?code=cores" method="get" onsubmit="ajaxId('ajaxwho.php?id_event=<?=$id_event?>&code=cores&codebarre='+document.getElementById('coresCode').value+'&login='+document.getElementById('coresLogin').value,'who');return false;" >
		<p><label for="codebarre">Code:</label><input type="text" size="20" name="codebarre"  value="<?=$_GET['code']?>" id="coresCode" /></p>
		<p><label for="login">login:</label><input type="text" size="20" name="login" value="" id="coresLogin" /></p>
		<input type="button" value="Cr�er la correspondance" onclick="ajaxId('ajaxwho.php?id_event=<?=$id_event?>&code=cores&codebarre='+document.getElementById('coresCode').value+'&login='+document.getElementById('coresLogin').value,'who');return false;" />
		<a href="#" onclick="alert('ajaxwho.php?id_event=<?=$id_event?>&code=cores&codebarre='+document.getElementById('coresCode').value+'&login='+document.getElementById('coresLogin').value,'who');return false;">PROUUUT</a>
		</form>

		<script type="text/javascript">
	</script>
		<?
}else{
	$lui = $db->fetchArray($res);
	echo $lui['login'];
	echo '<h1>'.$lui['prenom'].' '.$lui['nom'].'</h1><h2>'.$lui['codebarre'].' '.$lui['datecodebarre'].'</h2><p><img src="'.$core->makeUrl('../user/photo_etu_big/'. $lui['idEtu'] . '.jpg').'" /></p>';
	echo '<h2>Inscription:</h2>';	
	$id_event=$_GET['id_event'];
	if($db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE login='".$lui['login']."' AND id_event='$id_event'"),0)==0){
		//if(0){
		$db->query("INSERT INTO `subscribeManager_gens` ( `id` , `login` , `id_event` , `date` , `com` , `etat` ) VALUES ( '', '".$lui['login']."', '".$id_event."',now(), 'By code Barre', 'Y')"); 	
	}else{
		$page->msgError("Deja present dans la base !");
	}
	$nbr = $db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE id_event='$id_event'"),0);
	echo '<h1 style="border:2px solid black">'.$nbr.' Inscrits</h1>';
	?>
		<input type="button" onclick="ajaxId('ajaxwho.php?id_event=<?=$id_event?>&unlogin=<?=$lui['login']?>','who');" value="[    DESINSCRIRE    ]" />
		<?php
	}
	?>
