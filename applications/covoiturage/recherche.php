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
/******************* recherche.php ***************************************
Page de recherche d'une proposition de covoiturage

réalisation : Ken LE PRADO  (ken@leprado.com)
date de création : 20/06/2004
date de dernière MAJ : 20/06/2004
**********************************************************************/
//Obligatoire
$niveauSecu = 'all';                                                //Niveaux de sécurité (voir $session->authPage())
$idPage = 55;

include ('../../include/main.inc');

//Propre à la page
$page = new Page();
$page->header(locale::display('covoiturage','covoiturage'),'covoiturage');

$core->localLoadClass('covoiturage');
$covoiturage= new covoiturage();
?>
	<div class="centrage">
		<span class="panel_h" style="margin-left: auto; margin-right: auto">
			<a href="<?=$core->makeUrl('applications/covoiturage/index.php')?>"><?=locale::display('proposal_list','covoiturage')?></a> |
			<a href="<?=$core->makeUrl('applications/covoiturage/new.php')?>"><?=locale::display('new_proposal','covoiturage')?></a>
		</span>
	</div>

<table class="plan_de_travail" width="100%" align="center">
 <tr>
  <td width="350" valign="top">
    <?
      $page->moduleheader(locale::display('seek','covoiturage'), locale::display('ignore','covoiturage'));
    ?>
    <form action="<?=$_SERVER['PHP_SELF']?>" method="POST">
     <?=locale::display('tick_dpt_seeked','covoiturage')?><br><br>
     <?=locale::display('dpt','covoiturage')?>&nbsp;&nbsp;<input name="departement" value="<?=$_POST['departement']?>" type="text" size="2" maxlength="2">
     <input type="submit" value="chercher">
    </form>
    <?
      $page->modulefooter();
    ?>

  </td>


  <td valign="top" class="titre" align="left">
  <div class="information2">

<?php
$page->moduleheader('Résultats',''); ?>

  <?php
  if (!empty($_POST['departement'])) {
   $covoiturage->listeDeplacement ('Offre', $_POST['departement']);
   echo '<br>';
   $covoiturage->listeDeplacement ('Demande', $_POST['departement']);
  } else {
   echo locale::display('specify_zipcode','covoiturage');  //LABELPROB oliv => balise<b> dans le text...
  }
  ?>
  </td>
	<?php
$page->modulefooter();
?>
 </div>
 </tr>
</table>
<?

$page->footer();
?>
