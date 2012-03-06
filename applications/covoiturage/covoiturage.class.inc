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


/******************* covoiturage.class ***************************************
Définition de la classe covoiturage

Liste des propriétés
--------------------
$couleurCovoiturage : tableau contenant les valeurs hexa des couleurs à utiliser sur la carte de France


Liste des méthodes
------------------

resume() : affiche le module covoiturage (page d'accueil)
listeDeplacement($type) : affiche la liste des propositions pour un type donné (offre ou demande) sous forme d'un tableau
getDeplacement($id) : renvoie un tableau (depuis la base) contenant les infos pour une proposition donnée (id de la proposition)
displayMap($mode,$width,$height,$dptListToSelect,$read_only) : affiche la carte de France (en flash) selon des paramètres d'entrée spécifiques

réalisation : JB Blanc (creation301@yahoo.fr)
date de création : 04/08/2003
date de dernière MAJ : 07/08/2003
**********************************************************************/

class Covoiturage {
   var $couleurCovoiturage = array('offre' => "#FF0000",'demande' => "#00FF00",'cumul' => "#0000FF",'defaut' => "#FFFFFF");

   function Covoiturage() {

   }

   // Affiche le nombre d'offres & demandes, ainsi que la carte (en miniature)
   // qui résume les propositions
   function resume() {
      global $db, $page, $core;

      $nbOffres = mysql_result($db->query("SELECT COUNT(*) AS nb FROM covoitListe WHERE date > NOW() AND type='Offre'"), 'nb');
      $nbDemandes = mysql_result($db->query("SELECT COUNT(*) AS nb FROM covoitListe WHERE date > NOW() AND type='Demande'"), 'nb');
     ?>
         <?=locale::display('currently_consultable','covoiturage')?><?print($nbDemandes+$nbOffres<2?"":"s")?> : <br><br>
           <b><?=$nbOffres?></b> <?=locale::display('offer','covoiturage')?><?print($nbOffres<2?"":"s")?><br>
           <b><?=$nbDemandes?></b> <?=locale::display('request','covoiturage')?><?print($nbDemandes<2?"":"s")?><br>
		    <p>
                    <form action="<?=$core->makeUrl('applications/covoiturage/recherche.php')?>" method="POST">
                     <?=locale::display('dpt','covoiturage')?> : <input name="departement" type="text" size="2" maxlength="2">
                     <input type="submit" value="<?=locale::display('seek','kernel')?>">
                    </form>
		    </p>
		    <br />
           <a href="<?=$core->makeUrl('applications/covoiturage/index.php')?>"><?=locale::display('post_detail_way','covoiturage')?></a><br>
           <a href="<?=$core->makeUrl('applications/covoiturage/new.php')?>"><?=locale::display('add_new_way','covoiturage')?></a>
  <?
   }

   //affiche sous forme d'un tableau la liste (résumé)
   //de toutes les propositions d'un même type (offre ou demande)
  function listeDeplacement($type, $recherche = '')
  {
    global $db, $themecolor,$core;


		$core->loadClass('Date');

		$mydate = new Date();
		$first_letter = substr($type,0,1);
		$end_title = substr($type,1,120);
    echo '<div class="offre_demande_titre"><span>'.ucfirst($first_letter).'</span>'.$end_title.'</div>';

    if (empty($recherche))
      $sqlRecherche = '';
    else
      $sqlRecherche = " AND (departements LIKE '%$recherche%' OR dpt_depart = '$recherche' OR dpt_arrivee = '$recherche')";

    $SQLquery = "SELECT id, lieu_depart, dpt_depart, lieu_arrivee, dpt_arrivee, date FROM covoitListe WHERE date >= NOW() AND type='".$type."' $sqlRecherche ORDER BY date ASC";
    $result = $db->query($SQLquery);

    if ($db->numRows($result) > 0)
    {
      ?>
      <table class="cov_results" width="360">
        <thead>
          <tr>
           <th align="center"><?=locale::display('date','kernel')?></th>
           <th align="center"><?=locale::display('departure','covoiturage')?> Départ</th>
           <th align="center"><?=locale::display('arrival','covoiturage')?> Arrivée</th>
          </tr>
        </thead>
        <tbody>
        <?php
        while ($row = $db->fetchArray($result))
        {
          $row = array_map("strip_tags",$row);
          ?>
          <tr onclick="document.location.href='<?=$core->makeUrl('applications/covoiturage/detail.php')?>?id=<?=$row['id']?>';">
            <td align="center"><?=$mydate->formatDate($row['date'], "d/m/Y")?></td>
            <td align="center"><?=$row['lieu_depart']?>
            <?php
            if($row['dpt_depart'] > 0){//0 valeur par défaut si e dpt n'est pas renseigné
              echo ' ('. $row['dpt_depart']. ')';
            }
            ?></td>
            <td align="center"><?=$row['lieu_arrivee']?>
            <?php
            if($row['dpt_arrivee'] > 0){//0 valeur par défaut si e dpt n'est pas renseigné
              echo ' ('. $row['dpt_arrivee'] .')';
            }
            ?></td>
          </tr>
        <?php
        }
        ?>
        <tbody>
      </table><?php
      }
      else {
         if($type=='Offre')
          echo '<p>'.locale::display('no_way_proposed','covoiturage').'</p>';
         else echo '<p>'.locale::display('no_way_searched','covoiturage').'</p>';
      }
   }

    // renvoie un tableau qui contient toutes les infos pour une proposition
    // donnée (id proposition en entrée)
    function getDeplacement($id){
       global $db,$core;
       $result = $db->query("SELECT * FROM covoitListe WHERE id='".$id."'");
       if ($db->numRows($result) == 1) {
          return (array_map("strip_tags",$db->fetchArray($result)));
       }else {
          return false;
       }
    }

 function displayMap($mode,$width,$height,$dptListToSelect,$read_only){
    ##############################
    # Affiche la carte de France segmentée en départements. Ces dpts vont se colorer dynamiquement en fonction des offres
    # et demandes en cours pour le covoiturage. Pour cela, la carte (en Flash) va éxécuter un script qui lui renvoie
    # sous forme de flux XML les offres & demandes pour les zones concernées. L'url de ce script (en PHP) est la suivante :
    #     [rep. covoiturage]/mapGenerateXML.php
    # Note : il n'existe qu'un unique fichier swf qui gère tous les types de besoins en carte quelque soit la page qui le nécessite
    #
    # PARAMETRES EN ENTREE
    # --------------------
    # $width & $height sont les dimensions de la carte à l'affichage (veiller à les laisser égales si possible)
    # $mode définit le type d'affichage de la carte (oticks, détails,...); à modifier en fonction de la taille d'affichage de la carte
    # modeA, null : aucune restrition, mode par défaut
    # modeB : le module de survol des dpts n'affiche plus que le nom des dpts (pas les offres et demandes);
    # on peut en plus cliquer sur un dpt pour le sélectionner et accumuler une liste de n° de dpts transferables
    # dans un formulaire HTML via Javascript en utilisant la syntaxe JS suivante :
    # <script language="javascript">
    #  <!--
    #  var InternetExplorer = navigator.appName.indexOf("Microsoft") != -1;
    #  // Handle all the the FSCommand messages in a Flash movie
    #  function map_DoFSCommand(command, args) {
    #     if (command == "setDptListe") {
    #        document.covoitAjout.dptListe.value = args;
    #        //window.alert(document.[nom de votre formulaire].[nom de votre champ].value);
    #     }
    #  }
    #  // Hook for Internet Explorer
    #  if (navigator.appName && navigator.appName.indexOf("Microsoft") != -1 &&
    #     navigator.userAgent.indexOf("Windows") != -1 && navigator.userAgent.indexOf("Windows 3.1") == -1) {
    #     document.write('<SCRIPT LANGUAGE=VBScript\> \n');
    #     document.write('on error resume next \n');
    #     document.write('Sub map_FSCommand(ByVal command, ByVal args)\n');
    #     document.write('  call map_DoFSCommand(command, args)\n');
    #     document.write('end sub\n');
    #     document.write('</SCRIPT\> \n');
    #  }
    #  //-->
    #  </script>
    #
    # ->à utiliser pour la sélection des dpts à la création de l'offre par exemple
    # modeC : les textes trop petits et illisibles à taille réduite sont supprimés, et la légende
    #  ainsi que le module de survol des dpts;
    #  les dpts sont cependant toujours colorés
    # ->à utiliser pour les petites tailles d'affichage, les vignettes
    # $dptListe : liste des départements à préselectionner (utilisé dans le cas du mode B uniquement)
    # $read_only : dans le cas du mode B, définit si l'utilisateur a le droit ou non d'interagir sur les dpts en cliquant
    #
    # AUTRES VARIABLES (qui sont aussi transmis au fichier flash
    # ----------------
    # couleur* : les couleurs qui s'afficheront dans les zones dpt (variables de classe cvoiturage définies à l'instanciation)
    # absolutePath : le path qui mène à /covoiturage (par défaut $config['baseUrl'])
    ##############################
    global $core;
    $absoluteBaseUrl = $core->getConfig('baseUrl');

        $url_swf = $core->makeUrl('applications/covoiturage/swf/map.swf') .'?displayMode='. $mode .'&amp;couleurDemande='. $this->couleurCovoiturage['demande'] .'&amp;couleurOffre='. $this->couleurCovoiturage['offre'] .'&amp;couleurCumul='. $this->couleurCovoiturage['cumul'] .'&amp;couleurDefaut='. $this->couleurCovoiturage['defaut'] .'&amp;absolutePath='. $absoluteBaseUrl .'&amp;dptListToSelect='. $dptListToSelect .'&amp;read_only='. $read_only;
        echo '<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="'. $width .'" height="'. $height .'">'
          .'<param name="movie" value="'. $url_swf .'" />'
          .'<param name="wmode" value="transparent" />'
					.'<embed flashvars="" src="'.$url_swf.'" width="'. $width .'" height="'. $height .'" quality="high" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" menu="false" ></embed>'
					.'</object>';

   }
}
?>
