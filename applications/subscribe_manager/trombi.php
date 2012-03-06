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


$niveauSecu = 'all';       //Niveaux de securite (voir $session->authPage())
  $idPage = 133;
  //Obligatoire
  include ('../../include/main.inc');

  $page = new Page();
  $page->header();
$core->loadClass("module_trombi");
  $trombi = new module_trombi();


  $photo = $_GET['photo'];
$res = $db->query("SELECT  usersInfo . * FROM subscribeManager_gens, usersInfo WHERE subscribeManager_gens.login = usersInfo.login AND subscribeManager_gens.etat='".$_GET['etat']."' AND subscribeManager_gens.id_event='".$_GET['id_event']."'");
echo '<br /><h1>'.$_GET['titre'].'</h1>';
while($row = $db->fetchArray($res)){
?>
<table width="100%" bgcolor="#FFFFFF" cellspacing="0" border="0" style="border:solid 1px grey;" cellpadding="1">
              <tr class="contenu">
               <td rowspan="2" valign="top" width="50" height="62">
               <?
                   $url = '/' . $trombi->photo_mini . $row['idEtu'] . '.jpg';
				   if ($photo == 'true') {
                      echo '
					  <a href="javascript:openFiche(\''.$row['login'].'\')">
					    <img src="' . $url . '" alt="" border="0">
					  </a>';
                   } else {
                      echo '&nbsp;';
                   }
               ?>
               </td>
               <td colspan="2" align="left" bgcolor="#EFEFEF" height="16">
	        <a href="javascript:openFiche('<?=$row['login']?>')"><?=$row['nom']?> <?=$row['prenom']?></a>
	       </td>
              </tr>
              <tr>
               <td class="contenu" valign="top">
               <i><?=$row['branche']?></i><br>
	       <br>
               <a href="<?=$util->mail($row['email'])?>"><?=$row['email']?></a>
	       </td>
	       <td align="right" class="contenu" valign="top"><?=$trombi->set_tel($row['gsm'])?></td>
          </tr>
         </table>

<?php
}
$page->footer();
?>
