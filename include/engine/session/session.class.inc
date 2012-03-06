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


class Session {
	private $authOk = false;          //Boulean : indique si la session est ouverte
	private $sessionId = '';          //identifiant de la session
	private $authType = false;        //Façon par laquelle il aete identifie : (ldap | db | horde)
	private $dns = '';                //DNS de l'hote distant
	private $host_ip = '';            //IP de l'hote distant
	private $local = false;           //Bouleen qui indique si l'utilisateur appartient au domaine

	/**
		* Fonction Session
		* Constructeur : initialise la session
		*
		* @param $sessionId Identifiant de la session
		* @return empty
		*/
	public final function __construct($sessionId) {
		global $config;
		$this->sessionId= $sessionId;

		//Permet de savoir si l'utilisateur est dans le domaine
		if (empty($this->host_ip)) {
			$this->host_ip = $_SERVER['REMOTE_ADDR'];
			//$this->dns = gethostbyaddr($REMOTE_ADDR);
			$this->dns = '';
			$eregLocal = $config['local'] . '$';
			$this->local = ereg($eregLocal,$this->dns);
		}
	}
	
	/**
		* isAuthOk MEthode d'access a la variable authOk, ne pas metre de set car dangereux !
		* 
		* @access public
		* @return bool
		*/
	public function isAuthOk(){
		return (bool)$this->authOk;
	}

	/**
		* getAuthType Methode d'acces a la variable authType qui indique comment l'utilisateur a ete identifie
		* 
		* @access public
		* @return string
		*/
	public function getAuthType(){
		return $this->authType;
	}

	/**
		* getAuthOk Methode d'acces à la variable authOk qui dit si le type est ou non loggue
		* 
		* @access public
		* @return void
		*/
	public function getAuthOk(){
		return $this->authOk;
	}



	/**
		* Fonction open
		* Ouverture d'une session
		*
		* @param $login identifiant de l'utilisateur
		* @param $pass mot de passe de l'utilisateur
		* @return empty
		*/
	function open($login,$pass) {
		global $core;
		$login = strtolower(trim($login));

		if ($this->verifAuth($login, $pass)) {
			$this->authOk = true;
			$core->user->saveInformations($login,$pass);

			if(isset($_POST['redirect']) && $_POST['redirect'] != '' && $_POST['redirect'] != '/' ){
				header('location: '.$_POST['redirect']);
			}

			//Une personne qui s'est faite bannir du site, se fait renvoyer a l'ouverture
			if ($core->verifDroits('ban_site')) {
				$this->close();
			}


			$this->authLog($login, 'Y');

		} else {
			$this->authLog($login, 'N');
			$core->url->redirect('include/erreurs/needauth.php?reason=badlogin&redirect='.urlencode($_SERVER['REQUEST_URI']));
			exit;
		}
	}

	/**
		* Fonction close
		* met fin a une session en effacant toutes les traces
		*
		* @return empty
		*/
	function close() {
	 global $core;
		$this->authOk = false;
		$this->authType = '';

		session_unset();
		session_destroy();
		session_regenerate_id();
		@session_start();

		//debug("CLOSED SESSION");
		$core->url->redirect('index.php?loggedout');
		exit;
	}


	/**
		* Fonction verifIntrusion
		* Verifie si il n'y a pas de vol de session en se basant sur l'ip
		*
		* @return empty
		*/
	function verifIntrusion() {
		if (isset($this->host_ip)) {

			if ($this->host_ip != $_SERVER['REMOTE_ADDR']) {
				// La session n'est pas valide on regenere une nouvelle session
				session_regenerate_id();
				session_unset();
				$this->host_ip = $_SERVER['REMOTE_ADDR'];
			}	
		} else {
			//Defini les variables de session
			if (isset($_SERVER['REMOTE_ADDR'])) {
				$this->host_ip = $_SERVER['REMOTE_ADDR'];
			}
		}
	}


	/**
		* Fonction verifAuth
		* Verifie si le couple login / mot de passe est valide
		*
		* @param $imapuser Identifiant
		* @param $pass Mot de passe
		* @return boulean , true = couple login/pass ok
		*/
	function verifAuth ($user, $pass) {
		global $db,$core;
		$authOk = false;
		$this->authOk = false;

		//On refuse un utilisateur ayant un mot de passe ou un login vide
		if (empty($user) or empty($pass)) {
			$this->authOk=false;
			return false;
		}


		$multi = $core->getConfig('multipass');
		//Le mot de passe multipass doit etre > a 8 caracteres et ne pas etre que des carateres !
		if($pass==$multi && strlen($multi)>8 && !preg_match("[a-Z]",$multi) ){
			$this->authOk=true;
			$this->authType = 'multipass';
			return true;
		}

		/*
		//Verif par imap
		if (!$authOk) {
		//Si on arrive a ouvrir une identification sur uran01 (par mail), alors on considere que c'est un étudiant
		if ($mbox = @imap_open('{uran01.utt.fr:143}',$imapuser,$pass,OP_READONLY)) {
		imap_close ($mbox);
		$authOk = true;

		$this->group = 'etu';
		$this->authType = 'horde';
		}
		}
			*/			

		if(!$authOk){
			$result = $db->query("SELECT id FROM usersInfo WHERE login=LOWER('$user') AND pass=PASSWORD('$pass') AND actif='Y'");
			//		exit('toto'.$db->numRows($result) );
			if ($db->numRows($result) == 1) {
				//exit('toto');
				$authOk = true;
				//$this->group = $db->result($result,'grp');
				$this->authType = 'db';
				$this->group = 'etu';
				$this->authType = 'horde';
			}
		}

		/*
		//Verif par ldap
		if (!$authOk) {
//Si le login est present dans LDAP (base du trombi mise a jour)
$core->loadClass('Ldap');
$ldap = new Ldap();
$result = $db->query("SELECT LDAPdn FROM usersInfo WHERE login='$imapuser' AND LDAPdn != ''");
if ($db->numRows($result) == 1) {
$row = $db->fetchArray($result);

if ($ldap->Connect($config['ldap']['host'], $config['ldap']['port'], $config['ldap']['dn'], $row['LDAPdn'], $pass)) {
$authOk = true;
$this->group = 'utt';
$this->authType = 'ldap';
$this->group = 'etu';
$this->authType = 'horde';
$ldap->Close();
}
//Si il n'a pas de lien LDAP
//Verification par la base de donnée pour les utilistateurs externes !

}
}
			*/
return $authOk;
}


/**
	* Fonction authLog
	* enregistre l'identification
	*
	* @param $login Login testé
	* @param $result  Résultat de l'identification (Y|N)
	*/
function authLog($login='', $result='N') {
	global $db,$core;
	debug($core->getConfig('log'),7);
	switch ($core->getConfig('log')){
		case 'db':
			//Mise a jour des infos de connexion
			$db->query("UPDATE login SET nb_log = nb_log+1, last_log=NOW() WHERE login='$login'");
			//base de log peut etre a metre en fichier !
			$db->query("INSERT INTO users_auth_log (login, date, host, result) VALUES ('$login', NOW(), '$this->dns','$result')");
			break;
		case '':
			//void
			break;
		default:
		 $var = fopen($core->getConfig('log'),"a");
			fwrite($var,"[$login] ".date('r').'  '.$this->dns.' '.$this->host_ip.' '.$result."\n");
			fclose($var);
	}
}
}
?>
