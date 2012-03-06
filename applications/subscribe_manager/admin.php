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


function maj($id_event){
 global $db;
 $place = $db->result($db->query("SELECT max_place FROM subscribeManager_event WHERE id_event='$id_event'"),0) - $db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE id_event='$id_event' and etat='Y'"),0);
 $db->query("UPDATE subscribeManager_gens SET etat='Y' WHERE id_event='$id_event' LIMIT ".$place);
}
function liste($rep){
 global $db;
 echo '<ol>';
 while($row = $db->fetchArray($rep)){
  $lui = $db->fetchArray($db->query("SELECT * FROM usersInfo WHERE login='".$row['login']."'"));
  echo '<li><a href="javascript:openFiche(\''.$lui['login'].'\');">'.$lui['nom'].' '.$lui['prenom'].'</a> '.$lui['gsm'].'</li>';
 }
 echo '</ol>';
}

function mails($rep){
 global $db,$nom;
 $debut = '';
 $lesmails = '';
 while($row = $db->fetchArray($rep)){
  $lesmails = $lesmails.$debut.$row['login'].'@utt.fr ';
  $debut = ' ; ';
 }
 echo '<a href="javascript:open_compose_win(\'popup=1&to='.$lesmails.'&cc=&bcc=&msg=&subject=[SubscribeManager]'.$nom.'&thismailbox=INBOX\');">'.$lesmails.'</a>';
}
maj($id_event);


$don = $db->fetchArray($db->query("SELECT * FROM subscribeManager_event WHERE id_event='".$id_event."'"));
 echo '<h1>'.$don['nom'].'</h1>';
 $nom = $don['nom'];
 echo '<em>'.$don['description'].'</em>';
 $oqp = $db->result($db->query("SELECT count(*) FROM subscribeManager_gens WHERE id_event='".$don['id_event']."' AND etat='Y'"),0) ;
 echo '<br/>Fin le '.$don['date_expire'].'. reste '.($don['max_place']-$oqp).' place(s) <br/>('.$oqp.'/'.$don['max_place'].')';
 
$inscrit = $db->query("SELECT * FROM subscribeManager_gens WHERE id_event='$id_event' AND etat='Y'");
$desinscrit = $db->query("SELECT * FROM subscribeManager_gens WHERE id_event='$id_event' AND etat='N'");
$onliste = $db->query("SELECT * FROM subscribeManager_gens WHERE id_event='$id_event' AND etat='P'");



?>
<table border="1" cellpadding="0" class="quoi">
<tr><td></td><td>Inscrit</td><td>Liste d'atente</td><td>Desinscrit</td></tr>
<tr><td>Nombre</td><td><?=$db->numRows($inscrit)?></td><td><?=$db->numRows($onliste)?></td><td><?=$db->numRows($desinscrit)?></td></tr>
<tr><td>Liste</td><td><?liste($inscrit)?></td><td><?liste($onliste)?></td><td><?liste($desinscrit)?></td></tr>
<?php
$inscrit = $db->query("SELECT * FROM subscribeManager_gens WHERE id_event='$id_event' AND etat='Y'");
$desinscrit = $db->query("SELECT * FROM subscribeManager_gens WHERE id_event='$id_event' AND etat='N'");
$onliste = $db->query("SELECT * FROM subscribeManager_gens WHERE id_event='$id_event' AND etat='P'");
?>
<tr><td>Emails</td><td><?mails($inscrit)?></td><td><?mails($onliste)?></td><td><?mails($desinscrit)?></td></tr>
<tr><td>Trombi</td>
<td>
<a href="trombi.php?titre=Inscrit&etat=Y&id_event=<?=$id_event?>&photo=true">Voir avec les photos</a><br>
<a href="trombi.php?titre=Inscrit&etat=Y&id_event=<?=$id_event?>&photo=false">Voir sans les photos</a>
</td>
<td>
<a href="trombi.php?titre=Liste%20d\'atente&etat=P&id_event=<?=$id_event?>&photo=true">Voir avec les photos</a><br>
<a href="trombi.php?titre=Liste%20d\'atente&etat=P&id_event=<?=$id_event?>&photo=false">Voir sans les photos</a>
</td>
<td>
<a href="trombi.php?titre=Desinscrit&etat=N&id_event=<?=$id_event?>&photo=true">Voir avec les photos</a><br>
<a href="trombi.php?titre=Desinscrit&etat=N&id_event=<?=$id_event?>&photo=false">Voir sans les photos</a>
</td>
</tr>

</table>

<?php
$res = $db->query("SELECT nom,prenom FROM subscribeManager_gens , usersInfo WHERE subscribeManager_gens.login = usersInfo.login AND id_event='$id_event' ORDER BY nom");
echo '<br /><div style="border:2px solid black">';
while($don = $db->fetchArray($res)){
echo $don['nom'].' '.$don['prenom'].'<br />';
}
echo '</div>';

$page->footer();
?>
