<?php
/*
 * Created on Dec 10, 2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
$SECU= "log";
include (dirname(__FILE__)."/../../include/config.inc");

if (!isset($_GET['query'])) {
	exit ('{"results":[]}');
} elseif (strlen($_GET['query'])<3) {
	exit ('{"results":[]}');
}


function getValue($nom) {
	return mysql_escape_string($_GET['query']);
}

$query= "SELECT * FROM trombi_trombi, usersInfo WHERE trombi_trombi.id_user=usersInfo.id AND ";
$query.="(LOWER(usersInfo.login) LIKE CONCAT('%',LOWER('".htmlentities(getValue('fastsearch'), ENT_QUOTES, 'UTF-8')."'),'%') /*fastsearchsql*/)";
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
