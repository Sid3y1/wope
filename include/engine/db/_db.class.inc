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


class Db_wope {
	public $dbType;
	public $type = '';
	public $host = '';
	public $pass = '';
	public $user = '';
	public $base = '';
	public $chronos = '';


	/**
		* Fonction Db
		* Constructeur de la classe, déclare la classe qui va hériter de celle-ci
		*
		* @param $type type de la base utilisé. la classe correspondante doit exister (ex : mysql.class.inc)
		* @return boulean indique si le type est connu, true = type connu
		*/

	public final function  __construct ($type) {
		$this->type = $type;
		if(class_exists(strtolower($type))){
			$this->dbType = new $type();
			return true;
		}else{
			return false;
		}
	}

	/**
		* __wakeup Fonction de reveil de la serialization ! recrée la connextion
		* 
		* @final
		* @access public
		* @return void
		*/
	public final function __wakeup(){
		/*
					$this->__construct($this->type);
					$this->connect($this->host,$this->base,$this->user,$this->pass);
			*/
	}



	/**
		* Fonction connect
		* Connection à la base
		*
		* @param $hostin nom de l'hote
		* @param $hostin nom de la base
		* @param $hostin nom de l'utilisateur
		* @param $hostin mot de passe
		* @return boulean etat de la connection : true = connecté
		*/

	public function  connect ($hostIn, $baseIn, $userIn, $passIn) {
		$this->host = $hostIn;
		$this->user = $userIn;
		$this->pass = $passIn;
		$this->base = $baseIn;
		
		return ($this->dbType->connect($this));
	}


	/*
	//Sélectionne une nouvelle base
	public function  selectDb ($query) {
	$result = $this->dbType->query($query);
	return $result;
	}
		*/


	/**
		* Fonction query
		* Execution d'une requete
		*
		* @param $query Requete SQL à executer
		* @param $debug Boulean qui indique si la requete doit etre affichée
		* @return resultset Résultat de la requete, false si echec
		*/

	public function  query ($query, $debug = false) {
		if (isset($_GET["profiler"])) {
			$this->chronos->start($query);
		}
		$result = $this->dbType->query($query, $debug);
		if (isset($_GET["profiler"])) {
			$this->chronos->stop($query);
		}
		return $result;
	}

	/**
		* Fonction result
		* Récupération d'un champs d'un resultset
		*
		* @param $result resultset provennant d'une requete
		* @param $column nom ou numero de la colonne souhaitée
		* @return valeur de la colonne
		*/

	public function  result($res, $column) {

		if ( isset($res) && $res != null && $this->dbType->numRows($res) != 0) {
			$result = $this->dbType->result($res, $column);
		}else{
			$result = false;
		}

		return $result;
		
	}

	/**
		* Fonction insertId
		* Retourne l'identifiant de l'enregistrement qui vient juste d'etre créé par un INSERT
		*
		* @return identifiant de l'enregistrement
		*/

	public function  insertId() {
		$result = $this->dbType->insertId();
		return $result;
	}


	/**
		* Fonction numRows
		* Retourne le nombre d'enregistrement d'une requete SELECT
		*
		* @param $result Resultat d'une requete
		* @return nombre d'enregistrements
		*/

	public function  numRows($result) {
		$result = $this->dbType->numRows($result);
		return $result;
	}

	/**
		* Fonction fetchArray
		* Repartit un enregistrement dans un tableau associatif et pass à l'enregistrement suivant
		*
		* @param $result résultat d'une requête SELECT
		* @return Array Tableau associatif comportant les valeurs de l'enregistrement courant. false si il n'y a plus d'enregistrement
		*/
	public function  fetchArray($result,$type='') {
		$result = $this->dbType->fetchArray($result,$type);
		return $result;
	}


	/**
		* Fonction affectedRows
		* Retourne le nombre de lignes affectées par une requete (UPDATE / DELETE)
		*
		* @return nombre de lignes
		*/

	public function  affectedRows() {
		$result = $this->dbType->affectedRows();
		return $result;
	}


	/**
		* Fonction escape_string
		* Protege une chaine  pour la passer à query
		* 
		* @param $str chaine a proteger
		* @return chaine protegee
		*/
	public function escapeString($str){
		$result = $this->dbType->escapeString($str);
		return $result;
	}

	public function  showChronos() {
		$this->chronos->display();
	}


	/**
		* Fonction close
		* Ferme la connection
		*
		* @return boulean indique si la deconnexion s'est bien effectuée
		*/

	public function  close() {
		$result = $this->dbType->close();
		return $result;
	}
}

?>
