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


/**
 * 30/10/2006 => Sid => Ajout de la methode getConditionDroits($name) 
 */


/**
 * User 
	* GEre les utilisateurs !
 * 
 * @package 
 * @version $id$
 * @author Sid ( CHATEAU Mathieu ) 
 * @license PHP Version 5.0 {@link http://www.php.net/license/5_0.txt}
 */
class User
{

	/**
		* @access protected
		*/
	protected $login;

	/**
		* @access protected
		*/
	protected $id;
	
	/**
		* @access protected
		*/
	protected $nickname;

	/**
		* @access protected
		*/
	public $firstname;

	/**
		* @access protected
		*/
	public $lastname;

	/**
		* @access protected
		*/
	public $email;

	/**
		* @access protected
		*/
	protected $group;

	/**
		* @access protected
		*/
	protected $pass;

	/**
		* @access private
		*/
	private $prefs;

	/**
		* @access public
		*/
	public $droits;
	
	/**
		* @access public
		*/
	protected $avatar;
	
	
	/**
		* __construct Constructeur de User, donne a l'utilisateur le droit all
		* 
		* @final
		* @access public
		* @return void
		*/
	public final function __construct() {
		$this->droits = Array("all");
		$this->id = 1;
	}


	public function getInformations(){
		global $db;

		return $db->fetchArray($db->query("SELECT trombi_trombi.id_user, usersInfo.login, trombi_trombi.lastname trombi_trombi.firstname, trombi_trombi.birthdate, trombi_trombi.email, trombi_trombi.signal_social FROM trombi_trombi WHERE id_user='".$this->id."'"));
	}



	/**
		* saveInformations enregistre les informations de l'utilisateur (appele par session.class.inc)
		* 
		* @param mixed $login 
		* @param mixed $pass 
		* @access public
		* @return void
		*/
	public function saveInformations($login,$pass){
		global $db;
		$this->login = $login;
		$this->pass = $pass;
		
		//Recuperation du prenom et de l'email
		$res = $db->query("SELECT 
			ui.id,
			tt.firstname,
			tt.lastname,
			tt.nickname,
			tt.email,
			tt.photo,
			tt.birthdate,
			tt.signal_social
			
			FROM usersInfo ui LEFT JOIN trombi_trombi tt ON tt.id_user = ui.id
			WHERE ui.login='".$db->escapeString($login)."' LIMIT 1");
			
		if ($db->numRows($res) == 1) {
			$resp = $db->fetchArray($res);
			$this->firstname = stripslashes($resp['firstname']);
			$this->lastname = stripslashes($resp['lastname']);
			$this->nickname = stripslashes($resp['nickname']);
			$this->birthdate = stripslashes($resp['birthdate']);
			$this->avatar = stripslashes($resp['photo']);
			$this->status = stripslashes($resp['signal_social']);
			$this->email = stripslashes($resp['email']);
			$this->id = $resp['id'];
		} else {
			$this->firstname = $login;
			$this->lastname = '';
			$this->email = '';
			$this->nickname = '';
			$this->birthdate = '';
			$this->avatar = '';
			$this->status = '';
			$this->email = '';
			$this->id = 1;
		}
		//Charge les droits de l'utilisateur !
		$this->getDroits();
	}



	/**
		* verifDroits renvoie si oui ou non l'utilisateur a le bon droit !!!  /!\ pour les anciens developpeurs : cette fonction ne servait pas à ca avant !
		* 
		* @param mixed $droit 
		* @access public
		* @return void
		*/
 public function verifDroits($droits) {
 		if(!isset($droits) || $droits==''){
			return false;
		}
		if(!is_array($this->droits)){
			return false;
		}
		if(is_string($droits) && strstr($droits," ")){
			$droits = explode(" ",$droits);
		}
		if(is_array($droits)){
			foreach($droits as $droit){
				if(in_array($droit,$this->droits)) {
					return true;
				}
			}
		}else{
			if(in_array($droits,$this->droits)) {
				return true;
			}
		}	
		return false;
	} // end of member function verifDroits


	/**
		* Fonction getEmail
		* Retourne l'email d'un utilisateur
		*
		* @param $login login dont on veut le mail
		* @return string email de l'utilisateur
		*/

	public function getEmail($user_id = null) {
		global $db;

		if(!isset($user_id) || $user_id == $this->id){
			return $this->email;
		}
		$result = $db->query("SELECT trombi_trombi.email FROM trombi_trombi WHERE id_user='$user_id' LIMIT 1");
		if ($db->numRows($result) == 1) {
			return stripslashes($db->result($result, 'email'));
		}

		return false;

	} // end of member function getEmail


	public function getNickname($user_id = ''){
		global $db;
		
		$nick = '';
		if($user_id == '' || $user_id == $this->id){
		  $nick = $this->nickname;
		}else{
			$nick = $db->result($db->query("SELECT nickname FROM trombi_trombi WHERE id_user='".$user_id."' LIMIT 1"), 0);
		}

		return $nick;
	
	}

	/**
	 * getConditionDroits 
		* Recupere une condition pour les requètes sql, pour la remplir il faudra que l'utilisateur ai le droit
	 * 
	 * @access public
	 * @return void
	 */
	public function getConditionDroits($name){
		$condition =' ( 0 ';
		foreach($this->droits as $droit){
			//On ne protege pas le nom pour pouvoir le faire avec des trucs du genre b.droits !
			$condition .= "OR ".$name."  = '$droit' ";
		}
		$condition .= ' ) ';
		
		return $condition;
	}





	/**
		* Fonction getDroits
		* recupere et charge les droits a attribuer a un utilisateur, verifie la hierarchie
		*
		* @return empty
		*/
	public function getDroits() {
		global $db, $util;

		$this->droits = Array('all');
		
		//$this->group a ete defini lors de verifAuth. Il est defini en fonction de l'identification
		if (!in_array($this->group, $this->droits)) {
			$this->droits[] = $this->group;
		}
		
		$result = $db->query("SELECT kernel_right.name 
							FROM kernel_right_user, kernel_right
							WHERE kernel_right_user.login='".$this->id."'
								AND kernel_right.id=kernel_right_user.idRight");
		while ($row = $db->fetchArray($result)) {
			$this->droits[] = stripslashes($row['name']);	
		}
	}

	
	/**
		* Fonction setPéref
		* Change les preferences d'un utilisateur
		*
		* @param $login login de l'utilisateur
		* @param $pref_name Nom de la preference
		* @param $pref_value Valeur de la preference
		*/
	public function setPref($user_id,$pref_name,$pref_value) {
		global $db,$core,$_memcache;
		
		$pref_name=htmlentities($pref_name,ENT_QUOTES,"UTF-8");
		$pref_value=htmlentities(trim($pref_value),ENT_QUOTES,"UTF-8");
		// if memcache is enabled
		if ($core->getConfig('memcache','enable')) {
			// define memcache pref
			$_memcache->set($user_id."<>".$pref_name,$pref_value);
		}
		$result = $db->query("SELECT COUNT(*) FROM usersprefs2 WHERE login='".$user_id."' AND pref_name='".$db->escapeString($pref_name)."'");
		// Si l'utilisateur n'a jamais modifie la preference et qu'il s'appuie sur le modele par defaut
		if ($db->result($result, 0) == 0){
			$db->query("INSERT INTO usersprefs2 (login,pref_name,pref_value) VALUES ('".$user_id."','".$db->escapeString($pref_name)."','".$db->escapeString($pref_value)."')");
		}else{
			$db->query("UPDATE usersprefs2 SET pref_value='".$db->escapeString($pref_value)."' WHERE login='".$user_id."' AND pref_name='".$db->escapeString($pref_name)."'");
		}
		//NE PAS OUBLIER DE CHANGER LE "CACHE"!!!!
		$this->prefs[$user_id][$pref_name] = $pref_value;
		$core->saveInSession();
	} // end of member function set_pref


	/**
		* Fonction getPref
		* Extraire les preferences d'un utilisateur. Si elles ne sont pas definies on prend les valeurs par defaut!
		*
		* @param $login login de l'utilisateur
		* @param $pref_name Nom de la preference
		* @return valeur de la preference
		*/
	public function getPref($user_id,$pref_name) {
		global $db,$core,$_memcache;
		
		if(isset($this->prefs[$user_id][$pref_name])){
			return $this->prefs[$user_id][$pref_name];
		}

		//replacement of accentuated chars by common chars
		if (preg_match("/&.+;/",$pref_name)) {
			$pref_name = $db->escapeString($pref_name);
		} else {
			$pref_name = htmlentities($pref_name,ENT_QUOTES,"UTF-8");
		}

		//if memcache is enabled we try to get the data from it
		$mem_cache_succeed=false;
		if ($core->getConfig('memcache','enable')) {
			$data=$_memcache->get($user_id."<>".$pref_name);
			//have we got the data
			if ($data!==FALSE) {
				//was it a real data or a "go to see default"
				if ($data==='<DEF>') {
					$data=$_memcache->get("<DEF>".$pref_name);
					//did we get the default data
					if ($data!==FALSE) {
						$mem_cache_succeed=true;
					}
				} else {
					$mem_cache_succeed=true;
				}
			}
		}
		$mem_cache_succeed=false;
		// the data where not in cache
		if (!$mem_cache_succeed) {
			$req1 = $db->query("SELECT pref_value FROM usersprefs2 WHERE login='".$user_id."' AND pref_name='".$pref_name."'");
			$result = $db->fetchArray($req1);
			if (!isset($result['pref_value'])) {
				// prefs_liste is the table where you can find the default preference
				$req2 = $db->query("SELECT pref_value FROM prefs_liste WHERE pref_name='".$pref_name."'");
				$result = $db->fetchArray($req2);
				$data = stripslashes($result['pref_value']);
				if ($core->getConfig('memcache','enable')) {
					// save the result in memcache
					$_memcache->set($user_id.'<>'.$pref_name,'<DEF>');
					$_memcache->set('<DEF>'.$pref_name,$data);
				}
			} else {
				$data=stripslashes($result['pref_value']);
				if ($core->getConfig('memcache','enable')) {
					// save the result in memcache
					$_memcache->set($user_id.'<>'.$pref_name,$data);
				}
			}

		}
		//			debug( "getPref(".$pref_name.")=".$data."",10);
		$this->prefs[$user_id][$pref_name] = $data;
		return $data;
	}


	/**
		* Fonction newPref
		* Insere une nouvelle preference dans la table des preferences par defaut
		* Normalement tres peu utilisé
		*
		* @param $pref_name Nom de la preference
		* @param $pref_value Valeur de la preference
		* @return valeur de la preference
		*/
	public function newPref($pref_name,$pref_value) {
		global $db,$core,$_memcache;
		$pref_name=htmlentities($pref_name,ENT_QUOTES);
		$pref_value=htmlentities($pref_value,ENT_QUOTES);
		if ($core->getConfig('memcache','enable')) {
			$_memcache->set("<DEF>".$pref_name,$pref_value);
		}
		$query="SELECT count(pref_name) FROM prefs_liste WHERE pref_name='".$db->escapeString($pref_name)."'";
		$result=$db->query($query);
		if ($db->result($result,0)==0) {
			$db->query("INSERT INTO prefs_liste (pref_name,pref_value) VALUES ('".$db->escapeString($pref_name)."','".$db->escapeString($pref_value)."')");
		} else {
			$db->query("UPDATE prefs_liste SET pref_value='".$db->escapeString($pref_value)."' WHERE pref_name='".$db->escapeString($pref_name)."'");
		}
	}


	/**
		* getLogin Fonction d'access au login 
		* 
		* @access public
		* @return this user's login
		*/
	public function getLogin(){
		return $this->login;
	}

	/**
		* getUserId Fonction d'access à l'id
		* 
		* @access public
		* @return this user's id
		*/
	public function getUserId(){
		return $this->id;
	}

	/**
		* getPass 
		* Recupere le pass de l'utilisateur
		* 
		* @access public
		* @return this user's password
		*/
	public function getPass(){
		return $this->pass;
	}

	/**
		* getAvatar récupère l'adresse de l'avatar de l'utilisateur
		* 
		* @access public
		* @return this user's avatar id in the file manager
		*/
 	public function getAvatar(){
		return $this->avatar;
	}

	public function getFirstname(){
		return $this->firstname;
	}
	
	public function getLastname(){
		return $this->lastname;
	}


	/**
		* getMyPref encapsulationde la fonction getPref: il s'agit de diminuer la complexite de developement ! 
		* 
		* @param mixed $prefName 
		* @access public
		* @return void
		*/
	public function getMyPref($prefName){
		return $this->getPref($this->id,$prefName);
	}

	/**
		* getSkin Recupere le skin de la persone !
		* 
		* @access public
		* @return void
		*/
	public function getSkin(){
		return $this->getPref($this->id,"site_skin");
	}


	/**
		* mailTo Renvoi le lien pour composer un mail en fonction des preferences de l'utilisateur !
		* 
		* @param string $email 
		* @param string $cc 
		* @param string $cci 
		* @param string $subject 
		* @param string $body 
		* @access public
		* @return void
		*/
	public function mailTo ($email='',$cc='',$cci='',$subject='',$body='') {
		return "mailto:$email";
		/*
		//Ancien systeme a revoir completement !
		if($this->session->login == '' || $this->session->authType != 'horde' || 1){
		}elseif(Util::get_pref($this->session->login,'site_webmail')=='IMP' || 1){
		return "javascript:open_compose_win('popup=1&amp;to=".urlencode($email)."&amp;cc=&amp;bcc=&amp;msg=&amp;subject=&amp;thismailbox=INBOX');";
		}else{
		return $this->makeUrl('webmail/mail.php?_task=mail&_action=compose&_to='.$email.'&_cc='.$cc.'&_bcc='.$cci.'&_subject='.$subject.'&_body='.$body);
		}
			*/
	}

	public function loadPrefs(){
		global $db;

		$result = $db->query('SELECT * FROM usersprefs2 WHERE login="'.$this->id.'"');

		while($row = $db->fetchArray($result)){
			$this->prefs[$this->id][stripslashes($row['pref_name'])] = stripslashes($row['pref_value']);
		}
	}

	public function showPrefs(){
		debug($this->prefs);
	}


} // end of User
?>
