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


/******************* evenement.class ***************************************
     Définition pour le evenement

  .seuls les utilisateurs ayant le droit 'admin_www-etu' peuvent voir tous
   les évènements et les administrer.
   Pour changer le droit, modifier la valeur de $droit_admin à la troisieme ligne
   des fontions resume et detailEvenement





  **********************************************************************/
class Evenement {
   var $droit_admin = 'admin';    //Droit d'administration

   function Evenement () {


   }

   function resume() {
      global $db, $util, $page, $themecolor, $session;

      echo '<table width="100%" border="0" cellspacing="0" cellpadding="1" class="contenu"><tr>';
      for ($i = 0; $i < 7 ; $i++) {
          $date = mktime(0,0,0,date("m"),date("d") + $i, date("Y"));

          if (date("w", $date) == 0 OR date("w", $date) == 6) {
             $color = '#1EAEEB';
          } else {
             $color = '';
          }
          echo '<td class="col_ev" valign="top" align="center" nowrap height="45">';

      $result = $db->query("SELECT eventsType.imageMini AS imageType, eventsType.titre AS titre , eventsListe.id AS id,eventsListe.titre AS titreEvent, eventsListe.description AS description FROM eventsType INNER JOIN eventsListe ON eventsType.id = eventsListe.type AND eventsListe.date = '".date("Y-m-d",$date)."' AND eventsListe.actif = 'Y'");

          echo '<a href="events/index.php?date_af='.date("Y-m-d",$date).'"><b><font color="'.$color.'">' .substr($util->jour[date("w",$date)],0,3) . ' ' . date("d",$date) . '</font></b></a><br>';
          if ($db->numRows($result) > 0) {
              $compteur_e=0;
              while ($row = $db->fetchArray($result)) {
                 $compteur_e++;
                 if ($compteur_e < 3) {
                    echo '<a href="events/index.php#event'.$row['id'].'" title="'.$row['titre'].' : ' . $row['titreEvent'] . '"><img border="0" src="events/images/'.$row['imageType'].'" hspace="2" vspace="2" width="22" height="22" alt=""></a>';
                 } elseif ($compteur_e==3) {
                    echo "..";
                 }
              }
          }

          echo '</td>';

          if ($i !=6 ) {
             echo '<td class="sep_logo"><img src="' . $core->makeUrl('../skins/meetmind/images/dot.gif') . '" width="1" height="45" alt=""></td>';
          }


      }
      echo '</tr>
           <tr>
            <th colspan="13" align="right">
            <a href="' . $core->makeUrl('/events/new.php') . '">'.locale::display('add_event','include').'</a>
            <a href="' . $core->makeUrl('/events/index.php') . '">'.locale::display('post_event','include').'</a>
           </th>
           </tr>
          </table>';

   }

}

?>
