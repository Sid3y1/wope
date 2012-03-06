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

$SECU = 'user';
include('../../include/main.inc');

$action=isset($_GET['action'])?$_GET['action']:"";

switch($action){
	case 'del_foto':
  	$del_foto_url = '../photo_joke/'.$_GET['cible'].'.jpg';
		unlink($del_foto_url);
		break;
	default:
		//void
		break;
}
																																					
echo '<form enctype="multipart/form-data" action="'.$core->makeUrl('user/coordonnees.php').'?action=load_foto" method="post" id="foto_form">';
echo '<fieldset><legend>Informations personnelles</legend>';
echo '<p class="form-input"><label>Nouvelle photo :</label>';
echo '<input type="file" name="new_user_foto" size="54"></p>';
echo '<p class="form-submit"><input type="submit" value="Ajouter la Photo" /></p>';
echo '</fieldset>';
echo '</form>';
																					
?>
