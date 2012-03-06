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
$LOCALE = '';
include ('../include/main.inc');
$ARIANEWIRE = Array('Accueil' => $core->makeUrl('index.php'));

$page = new Page();
$page->header();

$all="";
$separator="\n";

$page->moduleHeader('Compilation des fichiers :','');
echo '<ul>';
$js = scandir('.');
foreach($js as $file){
	if(basename($file,'.js') != basename($file) && $file != 'javascript_core.js'){
		$all .= file_get_contents($file).$separator;
		echo '<li>'.$file.'</li>';
	}
}
echo '</ul>';
$page->moduleFooter();

$file = fopen('javascript_core.js','w');
fwrite($file,$all);
fclose($file);

//Page vide
$page->footer();

?>
