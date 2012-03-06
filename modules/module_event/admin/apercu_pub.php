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


  //Obligatoire
  $relativePath = '../../..';       //Chemin relatif mpour accéder à la racine de l'arborecense
  $niveauSecu = 'admin';       //Niveaux de sécurité (voir $session->authPage() )
  $idPage = 127;

  include ($relativePath . '/include/main.inc');

  // AUTHORISATION NIVEAU GROUPE

  //Propre à la page
  $page = new Page();
  $page->header('Administration module pub');
  $core->loadClass('date');
  $date=new Date();
  
  	//requete SQL de sélection
	$req = $db->query("SELECT * FROM module_event where id='".$_GET['id']."'");
	//on récupère le 1er enregistrement possible avec ses valeurs et on quitte la boucle

		
?>
<br>
<font size="+1"><b>Administration du module de pub - Aperçu d'une pub</b></font>
<p>
<?
	if ($resultat = $db->fetcharray($req)) {
		?><p>
		<?
		echo $resultat['nom']."<br>";
		echo "du ".$date->formatDate($resultat['jr_debut'])." au ".$date->formatDate($resultat['jr_fin']);
		?>
		<p>
		<table border="1" bordercolor="#145286" cellspacing="0">
		<tr><td border="0" bordercolor="#FFFFFF" width="500">
	
		<!--tableau pour la pub-->
			<table border="0" width="100%">
			<!-- une première ligne avec une seule grande cellule qui contient le titre -->
			<tr>
			<td colspan="2" align="center"><b><? echo $resultat['titre'] ?></b></td>
			</tr>
		
			<!-- une deuxième ligne avec cellule qui contient l'image et l'autre le texte -->
			<tr>
			<? 
			//test si on a une image valide et si oui on l'affiche'
			$url_image = "images/".$resultat['id'].".jpg";
			if (is_file($url_image)) {
				?><td width="5"><img src="<? echo $url_image ?>"/></td><?}
			?>
			<td align="center">
			<? echo $resultat['texte'] ?>
			</td>
			</tr>
			</table>
		</td></tr>
		</table>
		</p>
		
<p>
<a href="modif_pub.php?id=<? echo $resultat['id'] ?>">Modifier</a>
<p>
<?
		}
	//si l'id est incorrect
	else
		{$page->msgError("Pub inexistante");}		

		

?>
<a href="index.php">Retour</a>
<?
  $page->footer();
?>
