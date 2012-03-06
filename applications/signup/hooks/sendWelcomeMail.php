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


$SECU = 'all';
$relative_path = '../..';
include_once($relative_path.'/include/main.inc');
global $core;

/*****/
$DOMAIN 	= 'aube-eco.com';
$MESSAGE[1] 	= '<h4>Bienvenue sur le site communautaire de la CCI de Troyes et de l\'Aube.</h4>

<p>Vous venez de rejoindre le réseau de professionnels de votre région.
Grâce à ce site vous allez dès aujourd\'hui pouvoir :</p>

<ul>
<li>Trouver des clients, des partenaires ou de futurs collaborateurs,</li>
<li>Vous présenter et donner de la visibilité à vos projets et passions,</li>
<li>Présenter votre entreprise, ses produits,</li>
<li>Vous tenir informé à tout moment des événements économiques locaux et nationaux,</li>
<li>Disposer d\'une adresse mail simple et professionnelle,</li>
<li>Donner votre opinion, connaître celle des autres,</li>
<li>Echanger et partager des informations, des documents et des biens.</li>
</ul>

<p>Pour bien démarrer n\'hésitez pas à :</p>

<ol>
<li>Prendre le temps nécessaire pour remplir votre profil ; plus celui-ci sera complet plus il deviendra intéressant et sera potentiellement consulté.</li>

<li>Mettre le site dans vos favoris.</li>

<li>Vous promener sur les différents services proposés (mail,trombinoscope, forum, opportunités d\'affaire, ...)</li>
</ol>

<p>Une très bonne navigation,<br />
pour l\'équipe de la CCI de Troyes et de l\'Aube,<br />
Laurence HULIN<br />
<a href="mailto:Laurence.hulin@aube-eco.fr"</a></p>';

$core->loadClass('string');

$string = new String();

$MESSAGE[0] = $string->htmlToText($MESSAGE[1]);

$SUBJECT 	= 'Bienvenue sur le portail communautaire de la CCI de Troyes et de l\'aube';

$temp_object = unserialize($_SESSION['quickSignup_'.$_SESSION['steps_id']]);

$core->loadClass('mail');
$mail = new Mail();

$from = $core->getConfig('mailWebmaster');
$to = $temp_object->dataform['signup_login'].'@'.$DOMAIN;

$mail->send($from, $to, $MESSAGE, $SUBJECT);
?>
