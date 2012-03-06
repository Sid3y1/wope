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


$config = Array();


// On définit ici le message de confirmation de réussite de l'inscription
$config['endMessage'] = '<p class="input_description">Félicitations ! Vous venez de créer un compte.</p>'; 

$config['tos'] = "";

$config['isMailCreated'] = false;  		 			// Boolean // On définit ici si une adresse mail est crée à l'inscription
$config['newAccountRight'] = 'user'; 					// String		// On définit ici le droit que l'utilisateur nouvellement créé aura
$config['stepsNumber'] = '4';       				  // Integer  // On définit ici le nombre d'étapes que contient le processus d'inscription
$config['activeUser'] = true;             // Boolean // On définit ici si l'inscription est directe ou si elle doit être validée par l'admin

/**
* On définit ici les différentes étapes de l'inscription dans l'ordre dans lequel elle vont être affichées
* Les étapes dont le nom est précédé d'un diese sont obligatoires 
*
* # basicInfos 				--> Information minimales(login, nom, prénom, email)
* 	avatar						--> On demande à l'utilisateur d'uploader son avatar
* 	moreInfos					--> On demande des infos plus completes à l'utilisateur
* 	invitation				--> On demande à l'utilisateur d'inviter ses amis à le rejoindre sur le service
* # termsOfService		--> On demande à l'utilisateur d'accepter les conditions d'utilisation
* # summary						--> On fait le résumé des infos entré par l'utilisateur
*
* Si des étapes sont rajoutées plus tard merci de les documenter ici
*
**/
$config['steps'] = Array(
	'step_quickSignup', //Step 0
//	'step_avatarInfos', //Step 1 -> buggy : stepsNumber = 4
	'step_othersInfos', // Step 2
	'step_termsOfService', //Step 3
	'step_summary' //Step 4
);

/**
* On met dans ce tableau les noms des fichiers php que l'on veut executer après la création du compte ces fichiers doivent être placé dans le dossier hooks
*/

$config['hooks']['post_creation'] = Array();


/**
* On met dans ce tableau les noms des fichiers php que l'on veut executer entre le remplissage des infos par l'utilisateur et la création du compte ces fichiers doivent être placés dans le dossier hooks
*/
$config['hooks']['pre_creation'] = Array();


?>
