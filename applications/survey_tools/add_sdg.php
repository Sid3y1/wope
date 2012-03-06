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

if(isset($_POST['add'])){
$db->query("INSERT INTO surveyTools_sdg (admin,droit,debut,fin,label,description)VALUES('".$core->getLogin()."','".$_POST['droit']."','".$_POST['debut']."','".$_POST['fin']."','".$_POST['label']."','".$_POST['description']."')");
$page->msgInfo("Ajout : OK ! (normalement)");
}

?>
<form action="add_sdg.php" method="post">
<label>Nom:</label><input type="text" name="label" /><br />
<label>Description:</label><textarea name="description" ></textarea><br />
<!--
<label>date Debut</label><input type="text" name="debut"><br />
<label>date Fin</label><input type="text" name="fin"><br />
-->
<label>droit</label><input type="text" name="droit" value="etu"><br />
<input type="submit" value="Creer" name="add" />
</form>
<?php
$page->footer();
?>
