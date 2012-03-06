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


/******************* index.php (covoiturage) ***************************************
Page d'accueil de la rubrique cvoiturage en mode plein page.
Affiche la liste des offres & demandes ainsi que la carte de France qui résume le tout

réalisation : JB Blanc (creation301@yahoo.fr)
date de création : 04/08/2003
date de dernière MAJ : 20/06/2004
**********************************************************************/
//Obligatoire
$SECU = 'log';                                                //Niveaux de sécurité (voir $session->authPage())

include ('../../include/main.inc');

//Propre à la page
$page = new Page();
$page->header(locale::display('covoiturage','covoiturage'),'covoiturage');

$core->localLoadClass('covoiturage');
$covoiturage= new covoiturage();?>
<div class="covoiturage_droite">
	<? $page->moduleHeader('Résultats',''); ?>
	<ul class="panel">
		<li><a href="<?=$core->makeUrl('applications/covoiturage/recherche.php')?>"><?=locale::display('seek','covoiturage')?></a></li>
		<li><a href="<?=$core->makeUrl('applications/covoiturage/new.php')?>"><?=locale::display('new_proposal','covoiturage')?></a></li>
	</ul>

	<? $covoiturage->listeDeplacement('Offre'); ?>
	<? $covoiturage->listeDeplacement('Demande'); ?>
	<? $page->moduleFooter(); ?>
</div>
<div class="covoiturage_gauche">
	<? $page->moduleHeader('Offre & demande',''); ?>
	<? $covoiturage->displayMap('modeA',370,370,null,null); ?>
	<? $page->moduleFooter(); ?>
</div>

<?

/*infos sur les modes dans la définition de la fonction (covoiturage.class)*/

$page->footer();
?>
