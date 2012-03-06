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


/***************************************************************************
*
*   Log de modification
*
*   -----------------------------------------------------------------------
*
*   Nom                        Date                         Commentaire
*
*
*
*
*
***************************************************************************/
?>
<?php
/******************* new.php ***************************************
Formulaire de création d'une nouvelle proposition de covoiturage

réalisation : JB Blanc (creation301@yahoo.fr)
date de création : 04/08/2003
date de dernière MAJ : 07/08/2003
**********************************************************************/
$niveauSecu = 'log';                                                 //Niveaux de sécurité (voir $session->authPage())
$idPage = 3;

include ('../../include/main.inc');

//Propre à la page
$core->localLoadClass('covoiturage') ;
$covoiturage= new covoiturage();

$page = new Page();
$page->header(locale::display('covoiturage','covoiturage'),'covoiturage');


if (isset($_POST['addNew']) && $_POST['addNew'] == 'true') {
//pre($_POST);

   $date = date("Y-m-d", mktime(0,0,0,$_POST['mois'], $_POST['jour'], $_POST['annee']));

   //$_POST['dptListe'] contient la liste des départements sélectionnées pour trajet sous la forme d1,d2,d3
   //cas spécial pour la corse et ses 2 dpts (rangés sous 201 et 202 dans la base mais reçus depuis flash sous la forme 2a et 2b)
   //sous Flash on gère les 2a et 2b, pas sous mySQL
   $_POST['dptListe'] = str_replace('2a','201',$_POST['dptListe']);
   $_POST['dptListe'] = str_replace('2b','202',$_POST['dptListe']);
   $SQLquery = "INSERT INTO covoitListe (type,login,tel,date,lieu_depart,dpt_depart,lieu_arrivee,dpt_arrivee,departements,description) VALUES ('".$_POST['type']."','".$core->getLogin()."','".$_POST['tel']."','".$date."','".$_POST['lieu_depart']."','".$_POST['dpt_depart']."','".$_POST['lieu_arrivee']."','".$_POST['dpt_arrivee']."','".$_POST['dptListe']."','".$_POST['description']."')";
   if ($db->query($SQLquery)){
      $page->msgInfo(locale::display('save_way','covoiturage'));
   }

   //On efface les anciens trajets pour éviter qu'il y en ait trop
   $db->query("DELETE FROM covoitListe WHERE date < CURDATE()");

} else {
?>
<script language="javascript">
 <!--
 var InternetExplorer = navigator.appName.indexOf("Microsoft") != -1;
 // Handle all the the FSCommand messages in a Flash movie
 function map_DoFSCommand(command, args) {
    if (command == "setDptListe") {
       document.covoitAjout.dptListe.value = args;
       //window.alert(document.covoitAjout.dptListe.value);
     }
 }

 // Hook for Internet Explorer
 if (navigator.appName && navigator.appName.indexOf("Microsoft") != -1 &&
    navigator.userAgent.indexOf("Windows") != -1 && navigator.userAgent.indexOf("Windows 3.1") == -1) {
    document.write('<SCRIPT LANGUAGE=VBScript\> \n');
    document.write('on error resume next \n');
    document.write('Sub map_FSCommand(ByVal command, ByVal args)\n');
    document.write('  call map_DoFSCommand(command, args)\n');
    document.write('end sub\n');
    document.write('</SCRIPT\> \n');
 }
 //-->
</script>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post" onsubmit="return confirm('Avez-vous bien cliqué sur les départements traversés ?');" name="covoitAjout">
 <input type="hidden" name="addNew" value="true" />
 <input type="hidden" name="dptListe" value="" size="30" maxlength="30" />

	 <div class="left">
	 <?php $page->moduleHeader('Détails du trajet',''); ?>
    <table>
     <tr>
      <td align="left"><?=locale::display('my_situation','covoiturage')?></td>
      <td>
       <select name="type">
        <option value="offre"><?=locale::display('divide_car','covoiturage')?></option>
        <option value="demande"><?=locale::display('seek_car','covoiturage')?></option>
       </select>
      </td>
     </tr>
     <tr>
      <td align="left"><?=locale::display('departure_date','covoiturage')?></td>
      <td>
       <input type="text" name="jour" value="<?=date('d')?>" size="2" maxlength="2" /> /
       <input type="text" name="mois" value="<?=date('m')?>" size="2" maxlength="2" /> /
       <input type="text" name="annee" value="<?=date('Y')?>" size="4" maxlength="4" />
      </td>
     </tr>
     <tr>
      <td align="left"><?=locale::display('departure_place','covoiturage')?></td>
      <td>
       <input type="text" name="lieu_depart" value="" size="30" maxlength="30" />
      </td>
     </tr>
     <tr>
      <td align="left"><?=locale::display('departure_zipcode','covoiturage')?></td>
      <td>
       <input type="text" name="dpt_depart" value="" size="2" maxlength="2" />
			</td>
     </tr>
     <tr>
      <td align="left"><?=locale::display('arrival_place','covoiturage')?></td>
      <td><input type=text name="lieu_arrivee" value="" size="30" maxlength="30" /></td>
     </tr>
     <tr>
      <td align="left"><?=locale::display('arrival_zipcode','covoiturage')?></td>
      <td>
       <input type="text" name="dpt_arrivee" value="" size="2" maxlength="2" />
			</td>
     </tr>
     <tr>
      <td align="left"><?=locale::display('phone','kernel')?></td><td><input type=text name=tel size="30" maxlength="30" value="" /></td>
     </tr>
     <tr>
      <td align="left"><?=locale::display('note','covoiturage')?></td><td><textarea cols=27 rows=5 wrap=virtual name="description"></textarea></td>
     </tr>
		 <tr>
		 	<td><input type="submit" value=<?=locale::display('save_proposal','covoiturage')?> /></td>
		</tr>
    </table>

<?php
		$page->moduleFooter();
?>
		</div>

	 <div class="information3 right">
	 	<?php $page->moduleheader('Départements traversés',''); ?>
	 	<div class="offre_demande_titre trajet"><span>S</span>électionnez les départements traversés : </div>
<?php
    /*infos sur les modes dans la définition de la fonction (covoiturage.class)*/
    $covoiturage->displayMap('modeB',340,340,null,false);
		$page->modulefooter();
?>
		</form>
	</div>

<?php
}
$page->footer();
?>
