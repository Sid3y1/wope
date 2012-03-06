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


class Pdo_mysql {

	private $dbh;

	private $type = 'mysql';

	public function __construct(){
	}
	
	public function connect ($db) {
		try{
			//$this->dbh = new PDO($this->type.':host='.$db->host.';dbname='.$db->base, $db->user, $db->pass, array(PDO::ATTR_PERSISTENT => true ));
			$this->dbh = new PDO($this->type.':host='.$db->host.';dbname='.$db->base, $db->user, $db->pass, array(PDO::ATTR_PERSISTENT => true));
			$this->host = $db->host;
			$this->base = $db->base;
			$this->user = $db->user;
			$this->pass = $db->pass;
		} catch (PDOException $e) {
			$this->error($e);
			exit;
		}
		
		return $this->dbh;
	}

	/**
			Execute une requete
			debug == 1 => on affiche la requete

		*/
	public function query($request, $debug = 0) {
		$result = false;
		
		if ($debug) {
			echo $request;
		}
		
		try{
			$this->dbh = new PDO($this->type.':host='.$this->host.';dbname='.$this->base, $this->user, $this->pass);
			$result = $this->dbh->query($request);
			$this->last_request = $request;
			$this->last_statement = $result;
		} catch (PDOException $e){
			$this->error($e);
		}
		
		return $result;
	}

	//Retourne un champ de résultat
	public function result($sth, $column) {
		$row = $sth->fetch();
		$result = $row[$column];
		return $result;
	}

	//Retourne l'id du dernier INSERT
	public function insertId() {
		$result = false;

		try{
			$this->dbh = new PDO($this->type.':host='.$this->host.';dbname='.$this->base, $this->user, $this->pass);
			$result = $this->dbh->lastInsertId();
		} catch (PDOException $e) {
			$this->error($e);
		}
		
		return $result;
	}

	//Retourne  le nombre de lignes d'un SELECT
	public function numRows($sth) {
		$result = false;
		
		try{
			if($sth === $this->last_statement){
				$this->dbh = new PDO($this->type.':host='.$this->host.';dbname='.$this->base, $this->user, $this->pass);
				$st = $this->dbh->query($this->last_request);
				$result = count($st->fetchAll());
				/*$st = $this->dbh->query("SELECT FOUND_ROWS() AS num");
				$row = $st->fetch();
				$result = $row['num'];*/
			}
		} catch (PDOException $e) {
			$this->error($e, $st);
		}
			
		return $result;
	}

	//décompose un resultSet dans un tableau
	public function fetchArray($sth, $type = '') {
		$result = false;
		
		try{
		
			if($type != ''){
				$result = $sth->fetch($type);
			}else{
				$result = $sth->fetch();
			}
			
		} catch (PDOException $e) {
			$this->error($e, $sth);
		}
		
		return $result;
	}

	public function fetchAll($sth, $type = ''){
		$result = false;
		
		try{
		
			if($type != ''){
				$result = $sth->fetchAll($type);
			}else{
				$result = $sth->fetchAll();
			}
			
		} catch (PDOException $e) {
			$this->error($e, $sth);
		}
		
		return $result;
	
	}

	public function fetchObj($sth, $class = ''){
		$result = false;
		
		try{
			
			if($class != ''){
				$result = $sth->fetchObj($class);
			}else{
				$result = $sth->fetchObj();
			}
			
		} catch (PDOException $e) {
			$this->error($e, $sth);
		}
		
		return $result;
	
	}

	//retourne le nombre de lignes affectées par un UPDATE ou un DELETE
	public function affectedRows() {
		try{
		
			throw new DbException('PDO::affectedRows() : Unknown method', 3003, 1 );
			
		} catch (DbException $e) {
			$this->error($e);
		}
		
		return false;
	}

	//retourne une chaîne protégée pour passage à mysql
	public function escapeString($str){
		$result = mysql_escape_string($str);
		return $result;
	}

	//Ferme une connexion
	public function close() {
		$result = false;
		$this->dbh = null;
		$this->last_statement = null;
		
		if(!isset($this->dbh) && !isset($this->last_statement)){
			$result = true;
		}

		return $result;
	}


	//Affiche l'erreur sql
	private function error($e, $sth = null) {
		global $_SERVER;
		
			$this->dbh = new PDO($this->type.':host='.$this->host.';dbname='.$this->base, $this->user, $this->pass);
		$obj = isset($sth) ? $sth : $this->dbh ? $this->dbh : null;

		echo '<div style="background-color:#DDD;font-weight:bold;-moz-border-radius: 8px;border:2px black solid;width:350px" onclick="this.style.display = \'none\';"><h1 style="background-color:#DD0000;color:white;border:solid 1px black">'.$e->getMessage().'</h1></div>';
		
		if($obj){
			$info = $obj->errorInfo();
			$code = $obj->errorCode();
			
			echo '<div style="background-color:#DDD;font-weight:bold;-moz-border-radius: 8px;border:2px black solid;width:350px" onclick="this.style.display = \'none\';"><h1 style="background-color:#DD0000;color:white;border:solid 1px black">Erreur Mysql (n°'.$code.')</h1><p style="">'.$info.'</p></div>';
		
			$file = fopen("sql.log","a",1);
			$text="\n Erreur n ".$code." - ".date('r')." - ".$_SERVER["PHP_SELF"]."\n".$info."\n";
			fwrite($file, $text);
			fclose($file);
		}

	}
	
}

?>
