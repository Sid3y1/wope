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
 * file_put_contents  Fonction deja definie en php5 !
 * 
 * @param mixed $file 
 * @param mixed $string 
 * @access public
 * @return void
 */
/*
function file_put_contents($file, $string) {
	$f=fopen($file, 'a+');
	ftruncate($f, 0);
	fwrite($f, $string);
	fclose($f);
}
*/

function modif($file){
 $oldCont=file_get_contents($file);
 $cont = $oldCont;
	$cont = str_replace('$util->get_pref($session->login,','$core->user->getMyPref(',$cont);
	$cont = str_replace('util->loadClass(','core->loadClass(',$cont);
	$cont = str_replace('util->makeUrl(','core->makeUrl(',$cont);
	$cont = str_replace('session->login','core->getLogin()',$cont);
	$cont = str_replace('session->authOk','core->session->getAuthOk()',$cont);
	$cont = str_replace('$session->skin','$core->getSkin()',$cont);
  $cont = str_replace('label(','locale::display(',$cont);
	$cont = str_replace('$session->skin','$core->user->getSkin()',$cont);
	

	file_put_contents($file,$cont);
	if($oldCont != $cont){
	 echo "[Y] => /$file\n";
	}else{
	 echo "[N] => /$file\n";
	}
}

function nav($dir){
	$var = opendir($dir);
	while($file = readdir($var)){
		if($file != '.' && $file != '..' && $file != "robot.php"){
			if(!@opendir($dir.'/'.$file)){
				modif($dir.'/'.$file);
			}else{
				nav($dir.'/'.$file);
			}
		}
	}
}
nav('.');
?>

