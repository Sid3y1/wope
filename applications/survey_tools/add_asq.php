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
	switch($_POST['type']){

		case 'radio':
		case 'multiple':
			$db->query("INSERT INTO surveyTools_asq (id_sdg,label,type)VALUES('".$_GET['id_sdg']."','".$_POST['label']."','".$_POST['type']."')");
			$id_asq = $db->result($db->query("SELECT max(id_asq) FROM surveyTools_asq WHERE 1"),0);
			$sol = $_POST['sol'];
			var_dump($sol);
			foreach($sol as $s){
				if($s!=''){
					$db->query("INSERT INTO surveyTools_asq_ans (id_asq,label)VALUES('$id_asq','$s')");
				}
			}
			break;
		case 'text':
		case 'int':
			$db->query("INSERT INTO surveyTools_asq (id_sdg,label,type)VALUES('".$_GET['id_sdg']."','".$_POST['label']."','".$_POST['type']."')");
			break;

		default:
	}
	$page->msgInfo("Ajout: Ok ! (Je crois en tout cas)");
	}



	if(!isset($_GET['id_sdg'])){
		$res = $db->query("SELECT label,id_sdg FROM surveyTools_sdg WHERE admin='".$core->getLogin()."'");
		$nbr = $db->numRows($res);
		if($nbr==0){
			$page->msgError('Vous n\'estes administrateur d\'aucun sondage<br /><a href="add_sdg">Ajouter un sondage</a>');
			$page->footer();
			exit();
		}elseif($nbr>1){
			?>
				<form action="add_asq.php" method="get">
				<select name="id_sdg">
				<?php
				while($don = $db->fetchArray($res)){
					echo '<option value="'.$don['id_sdg'].'">'.$don['label'].'</option>';
				}
			?>
				</select>
				<input type="submit" value="Ok..." />
				</form>
				<?php
		}else{
			$don = $db->fetchArray($res);
			$id_sdg = $don['id_sdg'];
		}
	}
	echo $id_sdg;
	?>

		<form action="add_asq.php?id_sdg=<?=$id_sdg?>" method="post">
		Question: <input type="text" name="label"><br />
		Type: <select name="type">
		<option>radio</option>
		<option>multiple</option>
		<option>int</option>
		<option>text</option>
		</select>
		<input type="submit" value="Ajouter" name="add" /><br />
		Solutions (si multiple ou radio):<br />
																																				<input type="text" name="sol[1]" /><br />
																																					<input type="text" name="sol[2]" /><br />
																																					<input type="text" name="sol[3]" /><br />
																																					<input type="text" name="sol[4]" /><br />
																																					<input type="text" name="sol[5]" /><br />
																																					<input type="text" name="sol[6]" /><br />
																																					<input type="text" name="sol[7]" /><br />
																																					<input type="text" name="sol[8]" /><br />
																																					<input type="text" name="sol[9]" /><br />
																																					<input type="text" name="sol[10]" /><br />
																																					<input type="submit" value="Ajouter" name="add" />
																																					</form>

																																					<?php
																																					$page->footer();
	?>
