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


/*
 * Created on Dec 10, 2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
$SECU= "log";
include (dirname(__FILE__)."/../../include/main.inc");

if (!isset($_GET['query'])) {
	exit ('{"results":[]}');
} elseif (strlen($_GET['query'])<3) {
	exit ('{"results":[]}');
}


function getValue($nom) {
	return mysql_escape_string($_GET['query']);
}

$query= "SELECT * FROM trombi_trombi, usersInfo WHERE trombi_trombi.id_user=usersInfo.id AND ";
$query.="(LOWER(usersInfo.login) LIKE CONCAT('%',LOWER('".htmlentities(getValue('fastsearch'), ENT_QUOTES, 'UTF-8')."'),'%')  OR LOWER(nickname) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(lastname) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(firstname) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%') ".(
    			preg_match("/^([0-1]?[0-9])\/([0-3]?[0-9])\/([2,1][0,9][0-9][0-9])/",getValue("fastsearch"), $temp)?
					" OR birthdate='".$temp[3]."-".$temp[2]."-".$temp[1]."'"
					:"")." OR LOWER(phone) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(email) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(email2) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(site) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%') )";
$query .= $my_query." LIMIT 0,10";
$result= $db->query($query);
echo '{"results" : [ ';
$first=true;
while ($value = $db->fetchArray($result, MYSQL_ASSOC)) {
	if ($first) {
		$first=false;
	} else {
		echo "], [";
	}
	$out="";
	$query=$_GET['query'];
	foreach (array_keys($value) as $key) {
		if ($out=="") {
			if (stripos($value[$key],$query)!==false) {
				$out=htmlspecialchars($value[$key],ENT_QUOTES,"UTF-8");	
			}
		}
	}
	//$out=htmlentities($value["login"],ENT_QUOTES,"UTF-8");
	$ext="<i> - ".$value["firstname"]." ".$value["lastname"]."</i>";
	echo '{"value" : "'.$out.'", "data":"'.$ext.'"}';
}
echo "]} ";

?>
