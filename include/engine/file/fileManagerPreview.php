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
include( '../../main.inc');
//Recupere l'id du fichier 
$id = (int) $_GET['id'];

//Niveau de previsulisation de 1 a 3 !
$level = (int) $_GET['level'];
//Recupere si on va devoir aficher le html ou le fichier de preview
$html = (bool) !isset($_GET['fromhtml']);

$core->loadClass('fileManager');
$file_manager = new FileManager();
echo $file_manager->preview($id,$level,$html);
?>
