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


//	Valid 7.0 9/10/05

  $niveauSecu = 'admin';       //Niveaux de sécurité (voir $session->authPage())
  $idPage = 88;
  //Obligatoire
  include (dirname(__FILE__).'/../../include/main.inc');


  //Propre à la page
  $page = new Page();
  $page->header();


function myBourrinUTF8($data) {
	$data=htmlentities($data,ENT_NOQUOTES,"UTF-8");
	$replace= array ( 
			'&Atilde;&copy;' => 'é',
			'&Atilde;&uml;' => 'è',
			'&acirc;&sbquo;&not;' => '€',
			'&Atilde;&reg;' => 'î',
			'&Atilde;&sect;' => 'ç',
			'&Atilde;&euro;' => 'À',
			'&Atilde;&Dagger;' => 'Ç',
			'&Atilde;&nbsp;' => 'à',
			'&Acirc;&rsquo;' => '\'',
			'&Atilde;&acute;' => 'ô',
			'&Atilde;&ordf;' => 'ê',
			'&Acirc;&euro;' => '€',
			'&Atilde;&raquo;' => 'û',
			'&Atilde;&cent;' => 'î',
			'&Atilde;&sup1;' => 'ù',
			'&Atilde;&fnof;' => 'à',
			'&Acirc;&oelig;' => 'œ');
	return str_replace(array_keys($replace),$replace,$data);
}

function myValidUtf8 ($string,$dohtmlentities=true) {
	//	$string=utf8_decode($string);
	if ($dohtmlentities) {
		$string=htmlentities($string,ENT_NOQUOTES,"UTF-8");
	}
	//echo "Debug : ".$string."<br>\n";
	$encoding=mb_detect_encoding($string,"UTF-8,ISO-8859-1, ASCII");
	//echo "Debug2 : ".$encoding."<br>\n";
	// les caractères Ã et Â ne sont pas en français ...
	if (preg_match("/&(#[0-9]*|Atilde|Acirc)\;/m",$string)) {
		return false;
	} else if ($encoding!="UTF-8") {
		return false;
	} else {
		return true;
	}
}

function makeQuery($table,$row,$tab_key,$tab_field,$action) {
	global $core;
	$sets="";
	foreach($tab_field as $field) {
		if ($sets=="") {
			eval('$data='.$action.'($row[$field]);');
			$sets=$field."='"
				.mysql_real_escape_string($data)
				."'";
		} else {
			eval('$data='.$action.'($row[$field]);');
			$sets.=",".$field."='"
				.mysql_real_escape_string($data)
				."'";
		}
	}
	$where="";	
	foreach($tab_key as $key) {
		if ($where=="") {
			$where=$key."='"
				.mysql_real_escape_string($row[$key])
				."'";
		} else {
			$where.=" and ".$key."='"
				.mysql_real_escape_string($row[$key])
						."'";
		}
	}
	return "UPDATE `$table` SET ".$sets." WHERE ".$where;
}

set_time_limit(0);
flush();
$core->loadClass("string");
$string=new String();
	
//empty bad_data.sql
$fp=fopen("bad_data.sql","w");
fclose($fp);

$query="show tables";
$resulttables=$db->query($query);
while ($tables=$db->fetcharray($resulttables)) {
	$table=$tables[0];
	echo "<b>".$table."</b>: ";
	$query="describe `$table`";
	$result=$db->query($query);
	$tab_field=array();
	$tab_key=array();
	while ($row=$db->fetcharray($result)) {
		if (preg_match("/(char|text)/i",$row[1])) {
			$tab_field[]=$row[0];
		}
		if ($row[3]=="PRI") {
			$tab_key[]=$row[0];
		}
	}
	if (sizeof($tab_field)>0 && sizeof($tab_key)>0) {
		$keys="";
		foreach ($tab_key as $key) {
			if($keys=="") {
				$keys=$key;
			} else {
				$keys.=",".$key;
			}
		}
		$fields="";
		foreach ($tab_field as $field) {
			if ($fields=="") {
				$fields="`".$field."`";
			} else {
				$fields.=",`".$field."`";
			}
		}

		$query="SELECT CONCAT($fields), $fields, $keys from `$table`";
		$result=$db->query($query);
		$num_row=0;
		$utf8_row=0;
		$iso_row=0;
		$double_utf8_row=0;
		$unknown_row=0;
		$nopb_row=0;
		
		while ($row=$db->fetchArray($result)) {
			$num_row++;
			$data=$row[0];
			$utf8=myValidUtf8($data);
			$iso=myValidUtf8($string->convertUTF8($data));
			$double_utf8=myValidUtf8(utf8_decode($data));
			if ($utf8 && $iso) {
				//no action
				$nopb_row++;
			} else if ($utf8) {
				//no action
				$utf8_row++;
			} else if ($iso) {			
				// transfer from utf-8 to iso
				$query=makeQuery($table,$row,$tab_key,$tab_field,'$util->convertUTF8');
				$db->query($query);
				$iso_row++;
			} else if ($double_utf8) { 
				// un-utf-8
				$query=makeQuery($table,$row,$tab_key,$tab_field,'utf8_decode');
				$db->query($query);
				$double_utf8_row++;
			} else {
				
				//let's try jojo le bourrin
				$query=makeQuery($table,$row,$tab_key,$tab_field,'myBourrinUTF8');
				if (myValidUtf8(myBourrinUTF8($data),false)) {
					//jojo worked
					$db->query($query);
				} else {
					//make a file with the unknown query so that we can fix them by hand
					$fp=fopen("bad_data.sql","a");
					fwrite($fp,$query."\n");
					fclose($fp);
				}
				$unknown_row++;
			}
		}
		if ($num_row >0) {
			if ($nopb_row+$utf8_row==$num_row) {
				echo "<font color=green>UTF-8</font>";
			} elseif ($nopb_row+$iso_row==$num_row) {
				echo "<font color=orange>ISO-9661-1</font>";
			} elseif ($nopb_row+$double_utf8_row==$num_row) { 
				echo "<font color=orange>2*UTF-8</font>";
			} else {
				echo " no Pb (".round($nopb_row/$num_row*100,2)."%)";
				echo " <font color=green>UTF-8 (".round($utf8_row/$num_row*100,2)."%)</font>";
				echo " <font color=orange>ISO-9661-1 (".round($iso_row/$num_row*100,2)."%)</font>";
				echo " <font color=orange>2*UTF-8 (".round($double_utf8_row/$num_row*100,2)."%)</font>";
				echo " <font color=red>Unknown (".round($unknown_row/$num_row*100,2)."%)</font>";
			}
		}else {
			echo " <font color=green>No data</font>";
		}
		echo " <br>\n";
		flush();
	} else {
		echo " <font color=green>skipped</font><br>\n";
	}
}
$page->footer();
?>
