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
 * Core 
	* Classe moteur du site.
 * 
 * @package kernel
 * @version 8.0
 * @copyright Copyright
 * @author Sid 
 * @license PHP Version 5.0 {@link http://www.php.net/license/5_0.txt}
 */
class Core
{

	/**
		* @access public
		*/
	public $javascriptCore;


	/**
		* @access public
		*/
	public $url;


	/**
		* @access public
		*/
	public $session;


	/**
		* @access private
		*/
	private $ldap;

	/**
		* @access private
		*/
	private $config;

	/**
		* @access private
		*/
	private $db;

	/**
		* @access private
		*/
	public $user;

	/**
		* @access private
		*/
	private $database;




	/**
		* __construct Constructeur de la classe plutot baleze pusiqu'il load toutes les classes !
		* 
		* @final
		* @access public
		* @return void
		*/
	public final function __construct() {
		global $db,$config;
		//Une bonne partie des classes sont instanciees dans cette classe !

		$this->session = new Session(session_id());
		//Pour instancier Url on lui envoie les parametres de config, ça évitera de les re-recuperer a chaque fois !
		$this->url = new Url($this->config['baseUrl'],$this->config['baseUrls'],$this->config['baseDir']);
		$this->javascriptCore = new JavascriptCore();
		$this->user = new User();
		$this->config = $config;
		//Pour des raisons evidentes de CQ on vide la variable $config au plus vite !
		$config = "DATA ERASED PLEASE ACCESS WITH core private atribute";
		
  		$this->db = new Db_wope ($this->config['database']['type']);
		$this->db->connect($this->config['database']['host'],$this->config['database']['base'],$this->config['database']['user'],$this->config['database']['pass']);


	}



	
	/**
	 * __wakeup Fonction dite magic ! Elle est executee au reveil de la serialisation de l'objet, recupere la resource precedement sauvee
	 * 
	 * @final
	 * @access public
	 * @return void
	 */
	public final function __wakeup(){
		$this->db->connect($this->config['database']['host'],$this->config['database']['base'],$this->config['database']['user'],$this->config['database']['pass']);
	}
	
 /**
  * getDb Access a $db
  * 
  * @access public
  * @return void
  */
 public function getDb(){
  return $this->db;
	}
	
	
	/**
		* Enregistrement de l'objet dans $_SESSION['coreClass'];
		* @return 
		* @access public
		*/
	public function saveInSession( ) {
		/*if (isset($this->config['database']['pdo'])){
		  if($this->config['database']['pdo']){
		      $this->db->close();
		  }
		}*/
		
		$_SESSION['coreSerialisation'] = serialize($this);
		/*if (isset($this->config['database']['pdo'])){
		  if($this->config['database']['pdo']){
		      $this->db->connect($this->config['database']['host'],$this->config['database']['base'],$this->config['database']['user'],$this->config['database']['pass']);
		  }
		}*/
		
	} // end of member function saveInSession

	/**
		* Fonction loadClass
		* Charge une classe en incluant le fichier correspondant
		*
		* @param $className Nom de la classe (attention a la casse) le fichier inclu doit avoir avoir comme nom : "$className.class.inc"
		*/
	public function loadClass($className,$where = '') {
		
		if(class_exists($className))return true;

//Definition l'endroit ou sont stocke les fichiers de classe pour pouvoir les charger plus facilement !
		$whereIsMyClass = Array(
				'box'=>'include/engine/page/box/box.class.inc',
				'date'=>'include/engine/type/date.class.inc',
				'string'=>'include/engine/type/string.class.inc',
				'modulebasic'=>'include/engine/module/moduleBasic.class.inc',
				'magicform'=>'include/engine/page/form/magicForm.class.inc',
				'form'=>'include/engine/page/form/form.class.inc',
				'texteditor'=>'include/engine/page/form/textEditor.class.inc',
				'rss'=>'include/engine/markupLanguage/rss/parseRss.class.inc',
				'bbcode'=>'include/engine/markupLanguage/bbCode/bbCode.class.inc',
				'fpdf'=>'include/engine/lib/pdf/fpdf.class.inc',
				'fpdf_extended'=>'include/engine/lib/pdf/fpdf_extended.class.inc',
				'mail'=>'include/engine/mail/mail.class.inc',
				'ldap'=>'include/engine/protocol/ldap/ldap.class.inc',
				'chronos'=>'include/engine/error/debug/chronos.class.inc',
				'slow_db'=>'include/engine/db/slow_db.class.inc',
				'unzip'=>'include/engine/lib/unzip.class.inc',
				'zip'=>'include/engine/lib/zip.class.inc',
				'file'=>'include/engine/file/file.class.inc',
				'filemanager'=>'include/engine/file/fileManager.class.inc',
				'tag'=>'include/engine/tag/tag.class.inc',
				'tag_interface'=>'include/engine/tag/tag_interface.class.inc',
				'tabs'=>'include/engine/page/menu/tabs.class.inc',
				'link'=>'include/engine/page/link/link.class.inc',
				'checkmail'=>'service/checkMail/checkMail.class.inc',
				'rss'=>'include/engine/markupLanguage/rss/parseRss.class.inc',
				'simplehttp'=>'include/engine/protocol/http/simpleHttp.class.inc',
				'htmlblock'=>'include/engine/page/htmlBlock.class.inc'
				//'trombi'=>'applications/trombi/trombi.class.inc'
				//"keepconnection"=>"service/keepConnection/keepConnection.class.inc"
				);

		if( $this->isTrombi() ){
			$whereIsMyClass['trombi'] = "applications/trombi/trombi.class.inc";
		}

		if(isset($whereIsMyClass[strtolower($className)])){

		  
			$file  = dirname(__FILE__).'/../../'. $whereIsMyClass[strtolower($className)];
		}else{
			//We try to check if it's a module ! they are in a different folder
			if(strpos($className,'module_')!==false){
				$file = dirname(__FILE__) . '/../../modules/' .  strtolower($className).'/'. strtolower($className) . '.class.inc';
			}elseif($where == ''){
				$file = dirname(__FILE__) . '/' . strtolower($className) . '.class.inc';
			}else{
			  if($where[(strlen($where) - 1 )] != '/'){
					$where .= '/';
				}
				$file = $where . strtolower($className) . '.class.inc';
			}
		}
		

		if (is_file($file)) {
			if(!class_exists($className)){
				include_once ($file);
				return true;
			}
		}
		
		return false;
		
	} // end of member function LoadClass


	/**
		* Fonction localLoadClass
		* Charge une classe en prenant soin d'inclure le fichier avant.
		* Attention la classe est presente dans le dossier ou elle est appelee
		* 
		* @param $className Nom de la classe (attention a la casse) le fichier inclus doit avoir comme nom : "$className.class.inc"
		*/

	public function localLoadClass($className) {
		$className[0] = strtolower($className[0]);
		$file = ($className).'.class.inc';
		if (is_file($file)) {
			if (!class_exists ( $className)) {
				include_once ($file);
				return true;
			}
		}else{
			return false;
		}
	} // end of member function localLoadClass


	
 /**
  * makeUrl Encapsulation de la fonction makeUrl de la classe URL
  * 
  * @param mixed $url 
  * @access public
  * @return void
  */
 public function makeUrl($url){
	 return $this->url->makeUrl($url);
	}


	/**
	 * getConfig Pour recuperer une donnee de la variable config !
	 * 
	 * @param mixed $key1 
	 * @param string $key2 
	 * @param string $key3 
	 * @param string $key4 
	 * @access public
	 * @return void
	 */
	public function getConfig($key1,$key2='',$key3='',$key4=''){
  	if($key4 != ''){
			if(isset($this->config[$key1][$key2][$key3][$key4]))
		 		return $this->config[$key1][$key2][$key3][$key4];
		}elseif($key3 != ''){
			if(isset($this->config[$key1][$key2][$key3]))
		 		return $this->config[$key1][$key2][$key3];
		}elseif($key2 != ''){
			if(isset($this->config[$key1][$key2]))
		 		return $this->config[$key1][$key2];
		}elseif($key1 != ''){
			if(isset($this->config[$key1]))
		 		return $this->config[$key1];
		}
	}

 /**
  * getLogin recupere le login du gars
  * 
  * @access public
  * @return void
  */
 public function getLogin(){
  return $this->user->getLogin();
	}

 /**
  * getUserId recupere le login du gars
  * 
  * @access public
  * @return void
  */
 public function getUserId(){
  return $this->user->getUserId();
	}


/**
 * isUser est ce que l'utilisateur est identife
 * 
 * @access public
 * @return void
 */
public function isUser(){
	return (bool)($this->getLogin()!='');
}

public function isTrombi(){
	return file_exists($this->getConfig('baseServer').'/applications/trombi');
}

/**
 * verifDroits Encapsulation de la fonction qui se trouve dans user
 * 
 * @param mixed $droit 
 * @access public
 * @return void
 */
public function verifDroits($droit){
return (bool)($this->user->verifDroits($droit));
}

/**
 * mage_is_loaded verifie si le module ou la partie est implemente par Mage
 * 
 * @param mixed $object 
 * @access public
 * @return bool
 */
public function mage_is_loaded($object,$display=false,$feature=''){
	global $db,$page;
	
	$result=$db->query("SELECT * FROM mage_etat WHERE name='".$object."'");
	$is_loaded=($db->numRows($result)>0);
	
	if(!$is_loaded && $display){
		if($feature=='')$page->msgInfo(locale::displayVar('package_no_loaded','kernel',$object,$core->getConfig('mailWebmaster')));
		else $page->msgInfo(locale::displayVar('package_no_loaded','kernel',$feature,$object,$core->getConfig('mailWebmaster')));
	}
}

} // end of Core
?>
