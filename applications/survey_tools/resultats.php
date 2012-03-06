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

$id_sdg = $_GET['id_sdg'];
$don = $db->fetchArray($db->query("SELECT * FROM surveyTools_sdg WHERE id_sdg='$id_sdg'"));
if(!in_array($don['droit'],$session->droits)){
	$page->msgError("Vous n'avez pas les droits pour repondre a ce sondage");
	$page->footer();
	exit();
}
?>
<style>
.sdg{
border:solid 1px blue;
}
.sdg td {
border:solid 1px black;
}
</style>
<?php
if(isset($_POST['add'])){
	foreach($marqueur as $id => $asq){
		foreach ($asq as $login => $flag){
			$db->query("UPDATE surveyTools_ans SET marqueur='$flag' WHERE id_asq = '$id' AND login='$login'");
		}
	}

}

echo '<h1>'.$don['label'].'</h1><h3>'.$don['description'].'</h3>';
$nbr = $db->result($db->query("SELECT count( DISTINCT login ) FROM surveyTools_ans WHERE id_sdg='$id_sdg'"),0);
echo '<h1 style="color:red">'.$nbr.' R�ponsses pour ce sondage !</h1>';
echo '<a href="resultats.php?id_sdg='.$id_sdg.'&force">Forcer l\'affichage des r�ponsses cach�es</a>';
echo '<form action="resultats.php?id_sdg='.$id_sdg.'" method="post">';
$res = $db->query("SELECT * FROM surveyTools_asq WHERE id_sdg='$id_sdg'");
while($don = $db->fetchArray($res)){
	?>
		<h2><?=$don['label']?></h2>
		<?php
		switch($don['type']){
			case 'int':
				$int = $db->fetchArray($db->query("SELECT min(ans) as min, max(ans) as max,AVG(ans) as moy FROM surveyTools_ans WHERE id_asq='".$don['id_asq']."'"));;
				echo '<table class="sdg"><tr><th>min</th><th>max</th><th>moy</th></tr><tr><td>'.$int['min'].'</td><td>'.$int['max'].'</td><td>'.$int['moy'].'</td></tr></table>';
				break;

			case 'text':
				$resu = $db->query("SELECT id_asq,ans,login,marqueur FROM surveyTools_ans WHERE id_sdg='$id_sdg' AND id_asq='".$don['id_asq']."'");
				echo '<table class="sdg"><tr><th>Texte</th><th>Qui ?</th><th>important</th><th>Cach�</th><th>Normal</th></tr>';
				while($donuts = $db->fetchArray($resu)){
					if(isset($donuts['ans'])&&$donuts['ans']!=''){
						$N ='';
						$I ='';
						$H = '';
						$style="";
						switch($donuts['marqueur']){
							case 'N':
								$N = 'checked';
								break;
							case 'I':
								$style="background-color:#FF9999";
								$I = 'checked';
								break;
							case 'H':
								$style="background-color:#CCCCCC";
								$H = 'checked';
						}
						if(isset($_GET['force']) || $H==''){
							echo '<tr><td style="'.$style.'">'.$donuts['ans'].'</td><td><a href="javascript://" onclick="openFiche(\''.$donuts['login'].'\');">'.$donuts['login'].'</a></td><td><input type="radio" name="marqueur['.$donuts['id_asq'].']['.$donuts['login'].']" value="I" '.$I.' /></td><td><input type="radio" name="marqueur['.$donuts['id_asq'].']['.$donuts['login'].']" value="H" '.$H.' /></td><td><input type="radio" value="N" name="marqueur['.$donuts['id_asq'].']['.$donuts['login'].']" '.$N.' /></td></tr>'; 
						}
					}
				}
				echo '</table>';
				break;

			case 'radio':
				$result = $db->query("SELECT * FROM surveyTools_asq_ans WHERE id_asq='".$don['id_asq']."'");
				$total = $db->result($db->query("SELECT count(ans) FROM surveyTools_ans WHERE id_asq='".$don['id_asq']."'"),0);

				echo '<table><tr><td><table class="sdg"><tr><th>label</th><th>nbr</th><th>percent</th></tr>';
				$label=Array();
				$data=Array();
				while($row = $db->fetchArray($result)){
					$nbr = $db->result($db->query("SELECT count(ans) FROM surveyTools_ans WHERE id_asq='".$don['id_asq']."' AND ans='".$row['id_ans']."'"),0);
					echo '<tr><td>'.$row['label'].'</td><td>'.$nbr.'</td><td>'.ceil(($nbr*100)/$total).'</td></tr>';
					$data[]=$nbr;
					$label[]=str_replace("'","",str_replace('"','',$row['label']));
				}
				echo '</table></td><td>';
				$data = str_replace('"','THISISTHAT',serialize($data));
				$label = str_replace('"','THISISTHAT',serialize($label));
				echo '<img src="graph.php?label='.$label.'&data='.$data.'" style="float:right" /></td></table>';
				break;

			case 'multiple':
				$result = $db->query("SELECT * FROM surveyTools_asq_ans WHERE id_asq='".$don['id_asq']."'");
				$total = $db->result($db->query("SELECT count(ans) FROM surveyTools_ans WHERE id_asq='".$don['id_asq']."'"),0);
				$label='';
				$data='';

				echo '<table><tr><td><table class="sdg"><tr><th>label</th><th>nbr</th><th>percent</th></tr>';
				while($row = $db->fetchArray($result)){
					$nbr = $db->result($db->query("SELECT count(ans) FROM surveyTools_ans WHERE id_asq='".$don['id_asq']."' AND ans='".$row['id_ans']."'"),0);
					echo '<tr><td>'.$row['label'].'</td><td>'.$nbr.'</td><td>'.ceil(($nbr*100)/$total).'</td></tr>';
					$data[]=$nbr;
					$label[]=str_replace("'","",str_replace('"','',$row['label']));
				}
				echo '</table></td><td>';
				$data = str_replace('"','THISISTHAT',serialize($data));
				$label = str_replace('"','THISISTHAT',serialize($label));
				echo '<img src="graph.php?label='.$label.'&data='.$data.'" style="float:right" /></td></table>';
				break;
			default:
		}
}
echo '<input type="submit" name="add" value="Envoyer ..." />';
echo '</form>';
$page->footer();
?>
