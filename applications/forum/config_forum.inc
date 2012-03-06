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


//Taille maximum d'un mot d'un sujet
$config['subject_max_length'] = 100;
//Taille maximum d'un mot d'un message
$config['msg_max_length'] = 100;
//Taille maximum d'un mot d'un surnom
$config['nick_max_length'] = 15;
$config['msg_per_page'] = 25;
$config['thread_per_page'] = 15;
$config['score'] = 1; // Mettre le score à -1 pour avoir une modération avant l'affichage
//this defines time intervals to automatically save drafts
$config['draft_save_interval'] = 60000;
?>
