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


/**
 * Classe slowDb analyse les requêtes SQL
 * Usage :
 *         $chronos->start("monModule");
 *         ... Contenu à analyser ...
 *         $chronos->stop("monModule");
 * Après les données seront affichées sous la forme :
 *  monModule Temps_exécution (Pourcentage de la page si 'page' est chronométré)
 */
class SlowDb {
	var $startTime=array();
	var $stopTime=array();

   /**
    * Crée un objet chronos
    */
   function slowDb () {

   }
	
   /**
    * Récupère le temps en secondes
    */
   function getmicrotime(){
		list($usec, $sec) = explode(" ",microtime());
		return ((float)$usec + (float)$sec);
	}

   /**
    * Démarre le chrono
    * @param $module_name Nom du module à chronométrer
    *         le nom de module 'page' correspond au chargement complet de la page
    */
   function startQuery ($query) {
       $this->startTime["$query"]=$this->getmicrotime();
       $this->stopTime["$query"]=$this->getmicrotime();
   }

   /**
    * Arrète le chrono
    * @param $module_name Nom du module à stopper de chronométrer
    */
   function stop ($module_name) {
       $this->stopTime["$module_name"]=$this->getmicrotime();
   }

   /**
    * Affiche le résultat des chronos
    */
   function display() {
   	echo "<table align=center>";
       echo "<tr class=titre>";
       echo "<td colspan=3 align=center><b>.:: CHRONOS RESULT ::.</b></td>";
       echo "</tr>";
       //Rgarde si on a chronométré la page entière
       if (isset($this->startTime["page"])) {
       	$reference=($this->stopTime["page"]-$this->startTime["page"]);
       }

       //Affichage des valeurs de chaque module
    	foreach (array_keys($this->startTime) as $var_name) {
           echo "<tr class=titre>";
           echo "<td><b>$var_name</b></td>";
           echo "<td>".round($this->stopTime[$var_name]-$this->startTime[$var_name],4)."s</td>";
           if (isset($reference)) {
               echo "<td>(".round(
               	($this->stopTime[$var_name]-$this->startTime[$var_name])/$reference*100,1)."%)</td>";
           } else {
           	echo "<td>&nbsp;</td>";
           }
           echo "</tr>";
       }
       echo "</table>";
   }
}
?>
