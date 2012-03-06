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



function detail($values) {
	global $page,$core,$db;
	$core->loadClass('filemanager');
	$core->loadClass('tabs');

	$fm = new FileManager();
	
	ob_start();
	$locale=Locale::getLocale();
	echo '<div class="trombi_card">';
	$tabs_array['Informations générales']='Informations générales';
$tabs_array['Coordonnées personnelles']='Coordonnées personnelles';

	
	$menu = new Tabs($tabs_array);
	$menu->display();
?>
<dl>
	<?php
$menu->separatorStart("Informations générales");
?>
<dl>
<?php
if( $values['nickname'] != '' ){
	echo "
<dt>Pseudonyme :</dt>
<dd>".$values['nickname']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['lastname'] != '' ){
	echo "
<dt>Nom :</dt>
<dd>".$values['lastname']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['firstname'] != '' ){
	echo "
<dt>Prénom :</dt>
<dd>".$values['firstname']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['position'] != '' ){
	echo "
<dt>Poste occupé :</dt>
<dd>".$values['position']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['birthdate'] != '0000-00-00' && $values['birthdate'] != '00-00-0000' ){
	echo "
<dt>Date de naissance :</dt>
<dd>".( preg_match("/^([0-9]{4})\-([0-9]{2})\-([0-9]{2})/",$values['birthdate'], $temp) ? $temp[3]."/".$temp[2]."/".$temp[1] : $values['birthdate'])."</dd>
<div class=\"clear\"></div>
	";
}
?>
<?php
if( $values['signal_social'] != '' ){
	echo "
<dt>Signal social :</dt>
<dd>".$values['signal_social']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['photo'] != '' && $values['photo'] != 0 ) {
	$photo_photoUrl=$values['photo'];
?>
<dt>Photo :</dt>
<dd>
<?php
	echo $fm->preview($photo_photoUrl, 1);
?>
</dd>
</dl>
<dl>
<div class="clear"></div>

<?php	
}
?>
<?php
if( $values['hobbies'] != '' ){
	echo "
<dt>Vos passions :</dt>
<dd><br /><p>".$values['hobbies']."</p></dd>
<div class=\"clear\"></div>
";
}
?>

</dl>
<?php
$menu->separatorStop("Informations générales");
?><?php
$menu->separatorStart("Coordonnées personnelles");
?>
<dl>
<?php
if( $values['address'] != '' ){
	echo "
<dt>Rue :</dt>
<dd>".$values['address']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['address2'] != '' ){
	echo "
<dt>Complément adresse (appartement, étage...) :</dt>
<dd>".$values['address2']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['postal_code'] != '' ){
	echo "
<dt>Code Postal :</dt>
<dd>".$values['postal_code']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['city'] != '' ){
	echo "
<dt>Ville :</dt>
<dd>".$values['city']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['phone'] != '' ){
	echo "
<dt>Téléphone :</dt>
<dd>".$values['phone']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['email'] != '' ){
	echo "
<dt>Email Principal :</dt>
<dd>".$values['email']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['email2'] != '' ){
	echo "
<dt>Email secondaire :</dt>
<dd>".$values['email2']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['site'] != '' ){
	echo "
<dt>Site Web :</dt>
<dd>".$values['site']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['msn'] != '' ){
	echo "
<dt>Compte Msn :</dt>
<dd>".$values['msn']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['jabber'] != '' ){
	echo "
<dt>Compte Jabber/GoogleTalk :</dt>
<dd>".$values['jabber']."</dd>
<div class=\"clear\"></div>
";
}
?>
<?php
if( $values['yahoo'] != '' ){
	echo "
<dt>Compte Yahoo Messenger :</dt>
<dd>".$values['yahoo']."</dd>
<div class=\"clear\"></div>
";
}
?>

</dl>
<?php
$menu->separatorStop("Coordonnées personnelles");
?>
</dl>
</div>
<?
	$ret=ob_get_contents();
	ob_end_clean();
	return $ret;
}
?>
