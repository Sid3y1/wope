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
$idPage = 9;
$niveauSecu = 'all';
include ('../../include/main.inc');
$page = new Page();
$page->header();
if(isset($_GET['valider']) && $core->verifDroits('admin')){
	$db->query("UPDATE surveyTools_sdg SET valid='Y' WHERE id_sdg='".$_GET['valider']."'");
}

if(isset($_GET['devalider']) && $core->verifDroits('admin')){
	$db->query("UPDATE surveyTools_sdg SET valid='N' WHERE id_sdg='".$_GET['devalider']."'");
}

?>
<h1>SurveyTools</h1>
<h4>By www-etu</h4>
<ul style="border:dashed 1px red">
<li><a href="add_sdg.php">Ajouter un sondage</a></li>
<li><a href="add_asq.php">Ajouter une question a un sondage</a></li>
</ul>
<?php
echo '<h2>Liste des sondages</h2>';
$res = $db->query("SELECT * FROM surveyTools_sdg WHERE  valid='Y'");
while($don = $db->fetchArray($res)){
	if($core->verifDroits($don['droit'])){
		echo '<li><a href="sondage.php?id_sdg='.$don['id_sdg'].'">'.$don['label'].'</a></li>';
	}
}

$res = $db->query("SELECT * FROM surveyTools_sdg WHERE admin='".$core->getLogin()."'");
if($core->verifDroits('admin')){
 $res = $db->query("SELECT * FROM surveyTools_sdg WHERE 1");
}
if($db->numRows($res) != 0){
	echo '<div style="background-color:#FFDDDD"><h2>Liste des resultats aux sondages !</h2>';
	while($don = $db->fetchArray($res)){
		if($core->verifDroits($don['droit'])){
			echo '<li><a href="resultats.php?id_sdg='.$don['id_sdg'].'">'.$don['label'].'</a></li>';
		}
	}
	echo '</div>';
}
if($core->verifDroits('admin')){
	echo '<div style="background-color:#FFAAAA"><h2>Liste des sondages a administrer </h2>';
	$res = $db->query("SELECT * FROM surveyTools_sdg WHERE 1");
	while($don = $db->fetchArray($res)){
		echo '<li><a href="sondage.php?id_sdg='.$don['id_sdg'].'">'.$don['label'].'</a>';
		if($don['valid']=='Y'){
			echo '<a href="index.php?devalider='.$don['id_sdg'].'"> [ Desactiver ]</a></li>';
		}else{
			echo '<a href="index.php?valider='.$don['id_sdg'].'"> [ Valider ]</a></li>';
		}
	}
echo '</div>';
}
$page->footer();
?>
