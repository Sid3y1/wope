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
$idPage = 124;
$niveauSecu = 'all';
include ('../../include/main.inc');
$page = new Page();
$page->header();

$id_sdg = $_GET['id_sdg'];
if(!isset($_GET['id_sdg'])){
echo '<h2>Liste des sondages</h2>';
	$res = $db->query("SELECT * FROM surveyTools_sdg WHERE  valid='Y'");
	while($don = $db->fetchArray($res)){
		if(in_array($don['droit'],$session->droits)){
			echo '<li><a href="sondage.php?id_sdg='.$don['id_sdg'].'">'.$don['label'].'</a></li>';
   
		}
	}
	$page->footer();
	exit();
}

$don = $db->fetchArray($db->query("SELECT * FROM surveyTools_sdg WHERE id_sdg='$id_sdg'"));
if ($don['valid'] == 'N'){
 $page->msgError("Ce sondage n'a pas �t� valid�");
	$page->footer();
	exit();
}

//echo $don['droit'];
//var_dump($session->droits);
if(!in_array($don['droit'],$session->droits)){
	$page->msgError("Vous n'avez pas les droits pour repondre a ce sondage");
	$page->footer();
	exit();
}
$donne = $db->result($db->query("SELECT count(*) FROM surveyTools_ans WHERE id_sdg='$id_sdg' and login='".$core->getLogin()."'"),0);
if($donne!= 0){
	$page->msgError("Vous avez deja repondu a ce sondage");
	$page->footer();
	exit();
}

if(isset($_POST['add'])){

	$ans = $_POST['ans'];
	foreach($ans as $id_asq => $a){
		if(is_array($a)){
			foreach($a as $multiple){
				$db->query("INSERT INTO surveyTools_ans (login,id_sdg,id_asq,ans,ip)VALUES('".$core->getLogin()."','$id_sdg','$id_asq','$multiple','$REMOTTE_ADDR')");
			}
		}else{
			$db->query("INSERT INTO surveyTools_ans (login,id_sdg,id_asq,ans,ip)VALUES('".$core->getLogin()."','$id_sdg','$id_asq','$a','$REMOTTE_ADDR')");
		}
	}
	$page->msgInfo("Merci d'avoir repondu a ce sondage");
	$page->footer();
	exit();
}
echo '<fieldset><legend style="color:red;font-size:20px;border:solid 1px black;">'.$don['label'].'</legend><h3>'.$don['description'].'</h3></fieldset>';
$res = $db->query("SELECT * FROM surveyTools_asq WHERE id_sdg='$id_sdg'");
echo '<form action="sondage.php?id_sdg='.$id_sdg.'" method="post">';
echo '<p>';
while($don = $db->fetchArray($res)){
	?>
		</p><p><fieldset><legend style="font-size:15px;color:blue;"><?=$don['label']?></legend>
		<?php
		switch($don['type']){
			case 'int':
				echo '<p><input onchange="if(isNaN(this.value)){alert(\'doit etre un nombre\');this.value=\'\';}" type="text" size="5" name="ans['.$don['id_asq'].']" /></p>';
				break;

			case 'text':
				echo '<p><textarea name="ans['.$don['id_asq'].']" /></textarea></p>';
				break;

			case 'radio':
				$result = $db->query("SELECT * FROM surveyTools_asq_ans WHERE id_asq='".$don['id_asq']."'");
				while($row = $db->fetchArray($result)){
					echo '<p><input type="radio" name="ans['.$don['id_asq'].']" value="'.$row['id_ans'].'" />'.$row['label'].'</p>';
				}
				break;

			case 'multiple':
				$result = $db->query("SELECT * FROM surveyTools_asq_ans WHERE id_asq='".$don['id_asq']."'");
				while($row = $db->fetchArray($result)){
					echo '<p><input type="checkbox" name="ans['.$don['id_asq'].'][]" value="'.$row['id_ans'].'" />'.$row['label'].'</p>';
				}
				break;
			default:
		}
	echo '</fieldset>';
}
echo '</p><input type="submit" name="add" value="Envoyer ..." />';
echo '</form>';
$page->footer();
?>
