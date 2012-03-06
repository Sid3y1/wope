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


/*************************************************************************
*   Log de modification
*
*   -----------------------------------------------------------------------
*
*   Nom			Date 			Commentaire
*
*
*
*
*                                  
***************************************************************************/
?>
<?php
/******************* mapGenerateXML.php ***************************************
Script passerelle entre la carte en Flash et la base de données
La carte en flash appelle ce script en interne et reçoit le code XML qu'il génère en sortie
Le flux XML contient des infos sur les offres/demandes et les zones (dpts) concernées par ces offres & demandes

réalisation : JB Blanc (creation301@yahoo.fr)
date de création : 04/08/2003
date de dernière MAJ : 07/08/2003
**********************************************************************/
//Obligatoire
$niveauSecu = 'all';           //Niveaux de sécurité (voir $session->authPage())

include ('../../include/main.inc');

//GENERATION DU XML
$SQLquery = "SELECT type, dpt_depart, dpt_arrivee, departements FROM covoitListe "
        ."WHERE date > NOW() "
        ."ORDER BY type ASC";
$result = $db->query($SQLquery);
if ($db->numRows($result) > 0){
        $XMLoutput = "<?xml version=\"1.0\" encoding=\"windows-1252\"?>\n";
        $XMLoutput .= "<carte>\n";
        while ($row = $db->fetchArray($result)){
                $dptListe = explode(',',$row['departements']);
                if(!in_array($row['dpt_depart'],$dptListe)){array_push($dptListe,$row['dpt_depart']);}
                if(!in_array($row['dpt_arrivee'],$dptListe)){array_push($dptListe,$row['dpt_arrivee']);}
                if($row['type'] == 'offre'){
                        $XMLoutput .= "        <offre>\n";
                        foreach($dptListe as $idDpt){
                                //cas spécial pour la corse et ses 2 dpts (rangés sous 201 et 202 dans la base)
                                //sous Flash on gère les 2a et 2b, pas sous mySQL
                                if($idDpt == 201){$idDpt="2a";}elseif($idDpt == 202){$idDpt="2b";}
                                $XMLoutput .="                <zone id=\"".$idDpt."\"/>\n";
                        }
                        $XMLoutput .= "        </offre>\n";
                }else if($row['type'] == 'demande'){
                        $XMLoutput .= "        <demande>\n";
                        foreach($dptListe as $idDpt){
                                //cas spécial pour la corse et ses 2 dpts (rangés sous 201 et 202 dans la base)
                                //sous Flash on gère les 2a et 2b, pas sous mySQL
                                if($idDpt == 201){$idDpt="2a";}elseif($idDpt == 202){$idDpt="2b";}
                                $XMLoutput .="                <zone id=\"".$idDpt."\"/>\n";
                        }
                        $XMLoutput .= "        </demande>\n";
                }
        }
        $XMLoutput .= "</carte>";
        //envoi du code XML au flash
        echo $XMLoutput;//ATTENTION A NE RIEN AFFICHER DE PLUS SINON LE DOCUMENT XML GENERE ICI SERA INCORRECT NIVEAU SYNTAXE
}?>
