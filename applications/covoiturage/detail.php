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
/******************* detail.php ***************************************
Page d'édition d'une proposition existante

réalisation : JB Blanc (creation301@yahoo.fr)
date de création : 04/08/2003
date de dernière MAJ : 20/06/2004
**********************************************************************/
//Obligatoire
$SECU = 'log';                                                 //Niveaux de sécurité (voir $session->authPage())

include ('../../include/main.inc');

//Propre à la page
$core->localLoadClass('covoiturage') ;
$core->loadClass('Date') ;
$core->loadClass('Mail') ;


$mydate = new Date();
$mymail = new Mail();

$covoiturage= new Covoiturage();

$page = new Page();
$page->header(locale::display('covoiturage','covoiturage'));

if(isset($_GET['id'])){
        $id = $_GET['id'];
}

//INTERACTIONS POSSIBLES SI LA PERSONNE QUI CONSULTE EST LE CREATEUR DE LA PROPOSITION
if (isset($_POST['delete'])) {
        $db->query("DELETE FROM covoitListe WHERE id='".$_POST['id']."'");
}

if (isset($_POST['modif'])) {
   $date = date("Y-m-d", mktime(23,59,59,$_POST['mois'], $_POST['jour'], $_POST['annee']));

   //$_POST['dptListe'] contient la liste des départements sélectionnées pour trajet sous la forme d1,d2,d3
   //cas spécial pour la corse et ses 2 dpts (rangés sous 201 et 202 dans la base mais reçus depuis flash sous la forme 2a et 2b)
   //sous Flash on gère les 2a et 2b, pas sous mySQL
   $_POST['dptListe'] = str_replace('2a','201',$_POST['dptListe']);
   $_POST['dptListe'] = str_replace('2b','202',$_POST['dptListe']);

    $SQLquery = "UPDATE covoitListe SET "
            ."type='".$_POST['type']."',"
            ."lieu_depart='".$_POST['lieu_depart']."',"
            ."dpt_depart='".$_POST['dpt_depart']."',"
            ."lieu_arrivee='".$_POST['lieu_arrivee']."',"
            ."dpt_arrivee='".$_POST['dpt_arrivee']."',"
            ."departements='".$_POST['dptListe']."',"
            ."description='".$_POST['description']."',"
            ."tel='".$_POST['tel']."',"
            ."date='".$date."' "
            ."WHERE id='".$_POST['id']."'";
    if ($db->query($SQLquery)){
            $page->msgInfo("Le trajet a bien été modifié.");
    }

    $id = $_POST['id'];//on renseigne l'id pour la suite (pas de GET cette fois)
}

if ($dep = $covoiturage->getDeplacement($id)) {//on recupere les infos sur la proposition sous forme d'un tableau
   ereg( "([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})", $dep['date'], $regs );
   if ( ($core->getLogin() == $dep['login']) || $core->verifDroits('admin') ) {
   //L'utilisateur est le créateur de la proposition
?>
<script language="javascript">
    <!--
    var InternetExplorer = navigator.appName.indexOf("Microsoft") != -1;
    // Handle all the the FSCommand messages in a Flash movie
    function map_DoFSCommand(command, args) {
        if (command == "setDptListe") {
                document.covoitMAJ.dptListe.value = args;
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
<form action="<?=$_SERVER['PHP_SELF']?>" method="post" name="covoitMAJ">
    <input type="hidden" name="id" value="<?=$id?>"><?
    //cas spécial pour la corse et ses 2 dpts (rangés sous 201 et 202 dans la base)
    //sous Flash on gère les 2a et 2b, pas sous mySQL
    $dep['departements'] = str_replace('201','2a',$dep['departements']);
    $dep['departements'] = str_replace('202','2b',$dep['departements']);?>
    <input type="hidden" name="dptListe" value="<?=$dep['departements']?>" size="30" maxlength="30">
    <table align=center cellpadding="0" cellspacing="0" border="0">
     <tr>
      <td>
       <table>
        <tr>
          <td align="right"><?=locale::display('my_situation','covoiturage')?></td>
          <td>
           <select name=type>
            <option value=<?=locale::display('offer','covoiturage')?><? if($dep['type']=='offre'){echo " selected";}?>><?=locale::display('divide_car','covoiturage')?>
            <option value=<?=locale::display('request','covoiturage')?><? if($dep['type']=='demande'){echo " selected";}?>><?=locale::display('seek_car','covoiturage')?>
           </select>
          </td>
         </tr>
         <tr>
          <td align="right"><?=locale::display('departure_date','covoiturage')?></td>
          <td>
           <input type=text name="jour" value="<?=$regs[3]?>" size=2 maxlength=2> /
           <input type=text name="mois" value="<?=$regs[2]?>" size=2 maxlength=2> /
           <input type=text name="annee" value="<?=$regs[1]?>" size="4" maxlength="4">
          </td>
         </tr>
         <tr>
          <td align="right"><?=locale::display('departure_place','covoiturage')?></td>
          <td>
           <input type="text" name="lieu_depart" value="<?=$dep['lieu_depart']?>" size="30" maxlength="30">
          </td>
         </tr>
         <tr>
          <td align="right"><?=locale::display('departure_zipcode','covoiturage')?></td>
          <td>
           <input type="text" name="dpt_depart" value="<?=$dep['dpt_depart']?>" size="2" maxlength="2">
          </td>
         </tr>
         <tr>
          <td align="right"><?=locale::display('arrival_place','covoiturage')?></td>
          <td><input type=text name="lieu_arrivee" value="<?=$dep['lieu_arrivee']?>" size="30" maxlength="30"></td>
         </tr>
         <tr>
          <td align="right"><?=locale::display('arrival_zipcode','covoiturage')?></td>
          <td>
           <input type="text" name="dpt_arrivee" value="<?=$dep['dpt_arrivee']?>" size="2" maxlength="2">
          </td>
         </tr>
         <tr>
          <td align="right"><?=locale::display('phone','kernel')?></td><td><input type=text name=tel size="30" maxlength="30" value="<?=$dep['tel']?>"></td>
         </tr>
         <tr>
          <td align="right"><?=locale::display('note','covoiturage')?></td><td><textarea cols=25 rows=5 wrap=virtual name="description"><?=$dep['description']?></textarea></td>
         </tr>
                </table>
       </td>
       <td align="center"><?=locale::display('dpt_crossed','covoiturage')?><br><?
        $covoiturage->displayMap('modeB',340,340,$dep['departements'],false);/*infos sur les modes dans la définition de la fonction (covoiturage.class)*/?>
       </td>
      </tr>
      <tr>
       <td colspan=2 align="center">
        <input type="submit" name="delete" value="Supprimer la proposition">&nbsp;&nbsp;&nbsp;&nbsp;
        <input type="submit" name="modif" value="Modifier la proposition">
       </td>
      </tr>
     </table>
    </form>
<?
} else {
//Affichage classique du détail d'une offre (lecture seule)
?>
        <table align=center cellpadding="0" cellspacing="0" border="0">
     <tr>
      <td>
       <table width="350">
        <tr>
         <th><?=locale::display('way','covoiturage')?></th>
         <td>
         De : <? echo $dep['lieu_depart']."(".$dep['dpt_depart'].")";?><br>
         A : <? echo $dep['lieu_arrivee']."(".$dep['dpt_arrivee'].")";?>
         </td>
        </tr>
        <tr height="10">
         <td colspan="2">&nbsp;</td>
        </tr>
        <tr>
         <th><?=locale::display('date','kernel')?></th>
         <td><?=$mydate->formatDate($dep['date'], "d/m/Y")?></td>
        </tr>
        <tr height="10">
         <td colspan="2">&nbsp;</td>
        </tr>
        <tr>
         <th><?=locale::display('proposed_by','covoiturage')?></th>
         <td>
          <a href="<?=$mymail->mail($core->user->email)?>"><?=$dep['login']?></a> (<?=$dep['tel']?>)
         </td>
        </tr>
        <tr>
         <td colspan="2">&nbsp;</td>
        </tr>
        <tr>
         <td colspan="2"><?=nl2br($dep['description'])?></td>
        </tr>
           </table>
      </td>
      <td align="center" width="350"><?=locale::display('dpt_crossed','covoiturage')?><br>
      <?
        $covoiturage->displayMap('modeB',340,340,$dep['departements'],true);/*infos sur les modes dans la définition de la fonction (covoiturage.class)*/
      ?>
      </td>
     </tr>
        </table><?
   }
} else {
        $page->msgError(locale::display('proposal_out_date','covoiturage'));
}
$page->footer();
?>
