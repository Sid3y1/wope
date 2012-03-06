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


   //Le include doit deja contenir la fonction de connexion a Mysql
   $idPage = 131;
   //Obligatoire
   $relativePath = '../..';       //Chemin relatif pour acceder a la racine de l'arborecense
   //$niveauSecu = array('etu','ext');       //Niveaux de securite (voir $session->authPage())
   $niveauSecu='all';
   include ($relativePath . '/include/main.inc');
   //Propre a  la page
   $page = new Page();
   $page->header("SubscribeManager");

function maj($id_event){
 global $db;
 $place = $db->result($db->query("SELECT max_place FROM subscribeManager_event WHERE id_event='$id_event'"),0) - $db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE id_event='$id_event' and etat='Y'"),0);
 $db->query("UPDATE subscribeManager_gens SET etat='Y' WHERE id_event='$id_event' AND etat='P' LIMIT ".$place);
}

if(isset($_POST['newevent'])){
 if($_POST['pass']=='yes'){
  if($_POST['passwd1']!=$_POST['passwd2']){
   echo 'Erreur mots de passe différents !';
   exit();
  }else{
   $passwd = $_POST['passwd1'];
  }
 }else{
  $passwd = "";
 }
 $core->loadClass('date');
 $date = new Date();
 
 $db->query("INSERT INTO `subscribeManager_event` ( `id_event` , `nom` , `description` , `login_admin`  , `passwd_event` , `date_creation` , `max_place` , `date_start` , `date_expire` , `droit_event` , `reserve` , `public`, `inscription_public`) VALUES ( '', '".$_POST['nom']."', '".$_POST['description']."', '".$core->getLogin()."', '$passwd',now() ,'".$_POST['max_place']."', '".$date->reverseDate($_POST['date_start'])."','".$date->reverseDate($_POST['date_expire'])."', 'all', '".$_POST['reserve']."', '".$_POST['public']."','".$_POST['inscription_public']."' )");
 echo 'Ajout effectué avec succès !';
}



if(isset($_POST['suscribe'])){
 $id_event = $_POST['id_event']/7895123;
 if($db->result($db->query("SELECT passwd_event FROM subscribeManager_event WHERE id_event='".$id_event."'"),0)!=""){
  if($db->result($db->query("SELECT passwd_event FROM subscribeManager_event WHERE id_event='".$id_event."'"),0)!=$_POST['passwd']){
   echo 'Mauvais mot de passe';
   exit();
  }
 }
 if($db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE login='".$core->getLogin()."' AND id_event='$id_event'"),0)==0){
  $db->query("INSERT INTO `subscribeManager_gens` ( `id` , `login` , `id_event` , `date` , `com` , `etat` ) VALUES ( '', '".$core->getLogin()."', '".$id_event."',now(), '".$_POST['com']."', 'P')"); 
 }else{
  $db->query("UPDATE subscribeManager_gens SET etat='P',com='".$_POST['com']."' WHERE login = '".$core->getLogin()."' AND id_event='".$id_event."'");
 }
 maj($id_event); 
 echo 'Inscription enregistrée';
}

if(isset($_POST['unsuscribe'])){
 $id_event = $_POST['id_event']/7895123;
 $db->query("UPDATE subscribeManager_gens SET etat='N' WHERE login = '".$core->getLogin()."' AND id_event='".$id_event."'");
 echo 'Désinscription enregistrée';
}
//fin de detection de post de formulaires !

$res = $db->query("SELECT * FROM subscribeManager_event WHERE login_admin='".$core->getLogin()."'");
if($db->numRows($res)!=0){
 echo '<div style="border:dashed grey 1px"><h1>Administration</h1><ul>';
 while($adm = $db->fetchArray($res)){
  echo '<li style="padding:7px;"><a href="admin.php?id_event='.$adm['id_event'].'">'.$adm['nom'].'</a><span style="margin-left:50px;padding:5px;border:solid 1px blue">CODE BARRE :<a href="addothers.php?id_event='.$adm['id_event'].'"> Inscriptions</a> / <a href="verifentrance.php?id_event='.$adm['id_event'].'">Etat des entrées</a></span></li>';
 }
 echo '</ul></div>';
}



if(isset($_GET['direct_event'])){
 //Dirige vers un evenement directement !
 maj($_GET['direct_event']);
 $don = $db->fetchArray($db->query("SELECT * FROM subscribeManager_event WHERE id_event='".$_GET['direct_event']."'"));
 echo '<h1>'.$don['nom'].'</h1>';
 echo '<em>'.$don['description'].'</em>';
 $oqp = $db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE id_event='".$don['id_event']."' AND etat='Y'"),0) ;
 echo '<br/>Fin le '.$don['date_expire'].'. reste '.($don['max_place']-$oqp).' place(s) <br/>('.$oqp.'/'.$don['max_place'].')';
 $reste = $don['max_place']-$oqp;
 if( ( $reste>0||$don['reserve']=='Y') && $db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE login='".$core->getLogin()."' AND etat='Y' AND id_event='".$don['id_event']."'"),0)==0){
?>
<form action="index.php" method="post" name="subscribe">
<input type="hidden" name="id_event" value="<?=($don['id_event']*7895123)?>"/><br />
<em>Commentaires :</em><br />
<textarea name="com" rows=5 cols=37>
<?php
  if($db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE login='".$core->getLogin()."' AND etat='N' AND id_event='".$don['id_event']."'"),0)==1){
   echo $db->result($db->query("SELECT com FROM subscribeManager_gens WHERE login='".$core->getLogin()."' AND id_event='".$don['id_event']."'"),0);
  }
?>
</textarea><br/>
Les commentaires ne sont pas obligatoires.<br />
<?php
if($db->result($db->query("SELECT passwd_event FROM subscribeManager_event WHERE id_event='".$don['id_event']."'"),0)!=""){
?>
<em>Mot de passe:</em><br/>
<input type="password" name="passwd"/><br />
<?php
}
?>
<input type="hidden" name="suscribe"/>
<input type="submit" value="- S'inscrire -" name="subscribe" />
</form>

<?php
 }elseif($db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE id_event='".$don['id_event']."' AND login='".$core->getLogin()."' AND etat!='N'"),0)==1){
?>
<form action="index.php" method="post" name="unsuscribe">
<input type="hidden" name="id_event" value="<?=($don['id_event']*7895123)?>"/>
<input type="hidden" name="unsuscribe"/>
<input type="submit" value="- Se desinscrire -" name="unsubscribe" />
</form>
<?php
 }
 if($don['public']=='Y'){
  $result = $db->query("SELECT usersInfo.nom,usersInfo.prenom FROM usersInfo,subscribeManager_gens WHERE subscribeManager_gens.id_event='".$don['id_event']."' AND subscribeManager_gens.etat='Y' AND subscribeManager_gens.login = usersInfo.login");
  echo '<dl>';
  while($row = $db->fetchArray($result)){
   echo '<li>'.$row['prenom'].' '.$row['nom'].'</li>';
  }
 }
}else{
 echo '<ul>';
 $res = $db->query("SELECT * FROM subscribeManager_event WHERE date_expire > now() AND date_start < now() AND inscription_public='Y'");
 while($don = $db->fetchArray($res)){
  $reste = - $db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE id_event='".$don['id_event']."' AND etat='Y'"),0) + $don['max_place'];
  echo '<li><a href="index.php?direct_event='.$don['id_event'].'">'.$don['nom'].'</a> (fin le '.$don['date_expire'].'. reste '.$reste.' place(s) )<strong>';
  $moi = $db->query("SELECT * FROM subscribeManager_gens WHERE login = '".$core->getLogin()."' AND id_event='".$don['id_event']."'");
  if($db->numRows($moi)!=0){
   $moi = $db->fetchArray($moi);
   switch($moi['etat']){
    case 'Y':
     echo '[Inscrit]';
    break;
    case 'N':
     echo '[Desinscrit]';
    break;
    case 'P':
     echo '[Sur liste d\'atente]';
   }
  }else{
   echo '[Pas inscrit]';
  }
  echo '</strong></li>'; 
 }
 echo '</ul>';
 echo '<a href="#" onclick="gE(\'addevent\').display();">Ajouter un évenement</a>';
 echo '<div id="addevent" style="display:none">';
?>
<form action="index.php" method="post" name="newevent">
<br />
<em>Nom:</em><br />
<input type="text" name="nom" size=30><br />
<em>Description :</em><br/>
<textarea name="description" rows=5 cols=27></textarea><br />


<input type="radio" name="pass" value="yes" checked onFocus="document.getElementById('pass').style.display='block';" >
Demander un mot de passe aux gens qui veulent s'inscrire.<br />
<input type="radio" name="pass" value="no" onFocus="document.getElementById('pass').style.display='none';" >
Ne pas demander de mot de passe.<br />
<div id="pass" style="display:block">
<em>Mot de passe:</em><br />
<input type="password" name="passwd1" /><br />
<input type="password" name="passwd2" /><br />
Ce mot de passe sera demandé a toute personne voulant s'inscrire.
</div>
<br />
<em>Date de début: </em><br/>
<input type="text" name="date_start">&nbsp;<i>(jj/mm/aaaa)</i><br/>

<em>Date de fin: </em><br/>
<input type="text" name="date_expire">&nbsp;<i>(jj/mm/aaaa)</i><br/>

<em>Nombre MAXIMUM de places </em><br/>
<input type="text" name="max_place"><br/>

<input type="radio" name="reserve" value="Y"checked />Autoriser plus de place que prévu (réservé mais pas inscrit)<br/>
<input type="radio" name="reserve" value="N" />Bloquer les inscriptions au nombre maximum.<br/>
<br/>
<input type="radio" name="public" value="Y" checked />Rendre la liste des inscrits publique<br/>
<input type="radio" name="public" value="N" />Liste des inscrits privée.<br />
<br />
<input type="radio" name="inscription_public" value="Y" checked />Rendre l'inscription publique<br/>
<input type="radio" name="inscription_public" value="N" />Inscription privée (uniquement l'admininistrateur.<br />

<input type="submit" value="Envoyer" name="newevent" />
</form>
<?
 echo '</div>';
}

   
   $page->footer();
?>
