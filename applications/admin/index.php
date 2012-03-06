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

include('config_admin.inc');

$OPT = Array(
	'pre_scripts' => Array('editor')
);

//SECU must contain the less important right of admin
$SECU = Array();
foreach($config_admin['parts'] as $part){
	foreach($part['rights'] as $right){
		if(!in_array($right, $SECU)){
			$SECU[] = $right;
		}
	}
}

include('../../include/main.inc');
$ARIANEWIRE = Array(
	'Accueil' => $core->makeUrl('index.php'),
	'Panneau d\'administration' => $core->makeUrl('applications/admin')
);

$core->localLoadClass('admin');
$admin = new Admin();

$page = new Page();
$page->header('Panneau d\'administration','admin');

//loads modules admins javascript
$core->javascriptCore->loadContextScripts('admin','..');

//just for notes module (in "home" part)
echo '<script type="text/javascript" src="../modules/module_notes/javascript/module_notes.js"></script>';

//transmission of variables
if(isset($_GET['part']) && $_GET['part']!='' ){
	$admin_part = $_GET['part'];
}else{
	//default case
	$admin_part = 'home';
}

if(isset($_GET['content']) && $_GET['content']!='' ){
  $content = $_GET['content'];
}else{
	$content = '';
}
		
if(isset($_GET['option']) && $_GET['option']!='' ){
  $option = $_GET['option'];
}else{
	$option = '';
}


//selection of tabs to display
$parts = Array();
foreach($config_admin['parts'] as $part => $options){
	if( $core->verifDroits($options['rights']) ){
		$options['part'] = $part;
		$parts[] = $options;
	}
}

echo '<div class="meetmind_menu_block">
					<ul class="meetmind_menu">';

//displays tabs
$parts_nb = count($parts);

for($i = 0; $i < $parts_nb; $i++){
	echo '<li><a '.( $admin_part == $parts[$i]['part'] ? 'class="selected_part"' : ($i == 0 ? 'class="mm_first"' : ( ($i == $parts_nb - 1) ? 'class="mm_last"' : '') ) ).' href="index.php?part='.$parts[$i]['part'].'">'.$parts[$i]['title'].'</a></li>
	';
}

echo '</ul>
			</div><div class="clear"></div>';

$admin->displayAdmin($admin_part,$content,$option);

$page->footer();

?>
