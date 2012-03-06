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
  $niveauSecu = 'all';       //Niveaux de sécurité (voir $session->authPage() )
  $idPage = 0;

  include ('../../../include/main.inc');

  $core->loadClass('Make_easyRSS');

// créer l'instance
$myrss = new Make_easyRSS();
/** OBLIGATOIRE **/
// on déclare le channel avec ses éléments title, link, description, language, copyright, webMaster
// obligatoires : title, link, description, language
// optionnels : copyright, webMaster
$myrss -> channel('www-etu - événements', 'http://www-etu.utt.fr', 'Agenda des événements à l\'utt', 'fr', '© 2005, Utt Net Group', 'ung@utt.fr');

/** OPTIONNEL **/
// on déclare l'élément image avec title, url, link, width, height, description
// obligatoires : title, url, link
// optionnels : width, height, description

//$myrss -> image('PHPSPIRIT.com', 'http://www.phpspirit.com/images/logobidon.gif','http://www.phpspirit.com/', 88, 31, 'Scripts et applications PHP de qualité');

/** OBLIGATOIRE  au moins 1 élément de type item **/
// on déclare les items avec title, link, description
// obligatoires: title et link
// optionnel : description
$result = $db->query("SELECT eventsListe.lieu AS lieu, eventsListe.actif AS actif, eventsType.image AS imageType, eventsListe.date AS date, eventsType.titre AS type, eventsListe.auteur AS auteur, eventsListe.description AS description, eventsListe.titre AS titre, eventsListe.id AS id, eventsListe.heure AS heure FROM eventsType INNER JOIN eventsListe ON eventsType.id = eventsListe.type WHERE eventsListe.actif = 'Y' AND date >= CURDATE() ORDER BY date ASC");

while ($row = $db->fetchArray($result)) {
   $myrss->add_item($util->formatDate($row['date'], "d/m/Y") . ' - ' . $row['type'] . ' - ' . $row['titre'], "http://www-etu.utt.fr/events/index.php#event$row[id]", " Lieu : $row[lieu] Heure : $row[heure] Auteur : $row[auteur]" . strtr($row['description'], "\n\r","  ") );

}

// on crée le fichier rss
echo $myrss->as_string();
?>
