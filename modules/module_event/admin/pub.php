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

include ('../../../include/main.inc');
global $db;

$aujourdhui = date("Y-m-d");
$req = $db->query("SELECT * FROM module_event where (jr_debut<='$aujourdhui' and jr_fin>='$aujourdhui') order by jr_debut asc");

if($db->numRows($req)>0){
	while ($resultat = $db->fetchArray($req)) { 
		$titre = $resultat['titre'];
		$texte = $resultat['texte'];
		$id_pub = $resultat['id'];		
		$url_image = 'pub/images/'.$id_pub.'.jpg';
	}
	if (is_file($url_image)) {
				
	?>
	<img style="float: left" src="<?php echo $url_image; ?>" alt="<?php echo $titre ?>" border="0" />
	<?php
	
	
	
	echo "<h1>$titre</h1>";
	echo "<p>$texte</p>";
	}
}else{
	echo "Pas d'information importante en ce moment.";
}

?>

