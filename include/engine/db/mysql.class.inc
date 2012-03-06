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


class Mysql {
	private $connexion;


	public final function __construct () {

	}


	public function connect ($db) {
		if (!$this->connexion = mysql_connect($db->host,$db->user, $db->pass)) {
			$this->error();
			exit;
		}
		if (!$result = mysql_select_db($db->base, $this->connexion)) {
			$this->error();
			exit;
		}
		//      mysql_query("SET NAMES 'utf8'", $this->connexion);
		//      mysql_query("SET CHARACTER SET utf8", $this->connexion);
		return ($result);

	}

	/**
			Execute une requete
			debug == 1 => on affiche la requete

		*/
	public function query($query, $debug = 0) {
		if ($debug) {
			echo $query;
		}
		//$var = fopen('/home/www-etu/losmysql','a');
		//fwrite($var,$query);
		if (!$result = mysql_query($query, $this->connexion)) {
			$this->error();
		}
		return ($result);
	}

	//Retourne un champ de résultat
	public function result($result, $column) {
		$result = mysql_result($result,$column);
		return $result;
	}

	//Retourne l'id du dernier INSERT
	public function insertId() {
		$result = mysql_insert_id();
		return $result;
	}

	//Retourne  le nombre de lignes d'un SELECT
	public function numRows($result) {
		if ($result) {
			$result = mysql_num_rows($result);
			return $result;
		} else {
			return 0;
		}
	}

	//décompose un resultSet dans un tableau
	public function fetchArray($result,$type='') {
		if($type != ''){
			$result = mysql_fetch_array($result,$type);
		}else{
			$result = mysql_fetch_array($result);
		}
		return $result;
	}

	//retourne le nombre de lignes affectées par un UPDATE ou un DELETE
	public function affectedRows() {
		$result = mysql_affected_rows($this->connexion);
		return $result;
	}

	//retourne une chaîne protégée pour passage à mysql
	public function escapeString($str){
		$result = mysql_escape_string($str);
		return $result;
	}

	//Ferme une connexion
	public function close() {
		$result = mysql_close($this->connexion);
		return $result;
	}


	//Affiche l'erreur sql
	private function error() {
		global $_SERVER;
		echo '<div style="background-color:#DDD;font-weight:bold;-moz-border-radius: 8px;border:2px black solid;width:350px" onclick="this.style.display = \'none\';"><h1 style="background-color:#DD0000;color:white;border:solid 1px black">Erreur Mysql (n°'.mysql_errno().')</h1><p style="">'.mysql_error().'</p></div>';
		$file = fopen("sql.log","a",1);
		$text="\n Erreur n ".mysql_errno()." - ".date('r')." - ".$_SERVER["PHP_SELF"]."\n".mysql_error()."\n";
		fwrite($file, $text);
		fclose($file);




	}
}

?>
