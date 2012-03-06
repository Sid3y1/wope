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
	* affichage des variables dans un format special (pour débug)
	* @param $var variable a afficher
	*/
function pre($var)
{
	echo '<pre style="background-color:#DD0000;border:dashed 1px #330000;color:white;" onclick="this.style.display = \'none\'">';
	print_r($var);
	echo '</pre>';
}

/**
	* debug permet de gerer les affichages de texte pour le debuggage
	* 
	* @param mixed $data 
	* @param mixed $rang 
	* @access public
	* @return void
	*/
function debug($data,$level=0){
//niveau de debugage
$niveau = 0;
//Protege les headers (ne rien afficher avant l'envoi des headers ! )
$protectHeader = false;

	if(headers_sent() || !$protectHeader){
		if($level < $niveau){
			if(is_array($data) || is_object($data)){
				pre($data);
			}else{
				echo '<span style="color:#FFFFFF;background-color:#DD0000;border:solid 1px #330000;font-size:12px;" onclick="this.style.display = \'none\'">&nbsp;'.$data.'&nbsp;<br /></span>';
			}
		}	
	}
}


/**
 * This is the former locale system which is here just for backward compatibility reasons.
 * As soon as this old system will be remove from every page of the site this will be remove as well.
 */
	$lang = 'fr_FR';
	if(isset($LOCALE)){
		$LOCALE = explode(" ",$LOCALE);
		include_once(dirname(__FILE__) . '/../locale/'.$lang.'/kernel.locale.inc');
		foreach($LOCALE as $nom){
			if($nom != ''){
				$file = $nom.'.locale.inc';
				include_once(dirname(__FILE__) . '/../locale/'.$lang.'/'.$file);
			}
		}
	}else{
		$dir = opendir(dirname(__FILE__) . '/../locale/'.$lang.'/');
		while(($file = readdir($dir))){
			if(!($file[0] == '.'))
				include_once(dirname(__FILE__) . '/../locale/'.$lang.'/'.$file);
		}
	}

/**
 * We include here the main configuration file (which himself includes all the other configuration files).
 */
include_once (dirname(__FILE__) . '/../conf/main.conf.inc');

/**
 * We start the session.
 * We use the name of the site (defined in the main configuration file) to name the session.
 * If you have multiple instance of Wope on your server it will avoid session conflict (if you have named your site differently)
 */
session_name(preg_replace("[\W]","",$config['site_name']));
session_start();

/**
 * We activate the error displays.
 * We use the errorDisplay configuration entry to controle the type of error which will be displayed.
 */
ini_set("display_errors","On");
ini_set("error_reporting",$config['errorDisplay']);
error_reporting($config['errorDisplay']);


//Infos sur la page en cours Obsolete mais retrocompatibilite
//$curPage['relativePath'] = dirname(__FILE__)."/..";


/**
 * We load the main classes
 */


/**
 * Just here for backward compatibility reasons : this is the old locale system
 */
include_once (dirname(__FILE__) . '/engine/locale/locale.class.inc');	
/**
 * The main class
 */
include_once (dirname(__FILE__) . '/engine/core.class.inc');	
/**
 * The main db class
 */
include_once (dirname(__FILE__) . '/engine/db/_db.class.inc');	
/**
 * The mysql class
 */
include_once (dirname(__FILE__) . '/engine/db/mysql.class.inc');
/**
 * The session class
 */
include_once (dirname(__FILE__) . '/engine/session/session.class.inc');
/**
 * The user class : everything related to the user is run here
 */
include_once (dirname(__FILE__) . '/engine/user/user.class.inc');
/**
 * The pageBasic class : this is the which defined every basic elements of a page
 */
include_once (dirname(__FILE__) . '/engine/page/pageBasic.class.inc');
/**
 * htmlBlock class : every standard block of html are defined here
 */
include_once (dirname(__FILE__) . '/engine/page/htmlBlock.class.inc');
/**
 * The menu class : Its used to define every menu on the site
 */
include_once (dirname(__FILE__) . '/engine/page/menu/menu.class.inc');
/**
 * The form class defines all the standard form in the site
 */
include_once (dirname(__FILE__) . '/engine/page/form/form.class.inc');
/**
 * The box class : is used to display javascript popup
 */
include_once (dirname(__FILE__) . '/engine/page/box/box.class.inc');
/**
 * This class is used to generate the main menu of the site
 */
include_once (dirname(__FILE__) . '/engine/page/menu/indexMenuBasic.class.inc');
/**
 * Everything related to Urls is here
 */
include_once (dirname(__FILE__) . '/engine/type/url.class.inc');
/**
 * The main class which defines modules (mainly for the personal start page)
 */
include_once (dirname(__FILE__) . '/engine/page/module/moduleBasic.class.inc');
/**
 * Use to call the jevascript pages
 */
include_once (dirname(__FILE__) . '/../javascript/javascriptCore.class.inc');
/**
 * An old cache class which should be removed
 */
include_once (dirname(__FILE__) . '/engine/cache/htmlCache.class.inc');
/**
 * The basic cache class
 */
include_once (dirname(__FILE__) . '/engine/cache/cache.class.inc');
/**
 * Another cache class which can be used to generate a cache per user
 */
include_once (dirname(__FILE__) . '/engine/cache/userCache.class.inc');
/**
 * All the exceptions classes
 */
include_once (dirname(__FILE__) . '/engine/error/exception/generalException.class.inc');
include_once (dirname(__FILE__) . '/engine/error/exception/fileException.class.inc');
include_once (dirname(__FILE__) . '/engine/error/exception/dbException.class.inc');


############################################################

require_once(dirname(__FILE__)).'/wopeLib/loader/Wope_Loader.class.inc';
Zend_Loader::registerAutoload('Wope_Loader');




/**
 * This part of the file run the storage of the classes. Especialy the db classes which have to be waked up before using.
 */
if (isset($_SESSION['coreSerialisation']) ){
	//Alors on peut deserialiser l'objet, la fonction __wakeup de core va etre lancee normalement !
	$coreSerie = $_SESSION['coreSerialisation'];
	$core = unserialize($coreSerie);
	//On recupere alors DB puisque __wakeup reouvre la ressource
	$db = $core->getDb();

}else{ // If there is nothing in session we create the session

	$core = new Core();
	$db = $core->getDb();
	$config = "DATA ERASED PLEASE ACCESS WITH core private atribute";
	$database = "DATA ERASED PLEASE ACCESS WITH core private atribute";
	// We store the instance of core.
	$core->saveInSession();
}
// We check if the user as the permission to see the page
if( !$core->verifDroits(  (isset($niveauSecu) && !isset($SECU))  ?  $niveauSecu  :  ( isset($SECU) ? $SECU : "" )  ) ){
	$core->url->redirect('include/erreurs/needauth.php?reason=page');
	exit();
}
// If the user try to log in
if (isset($_POST['login']))
{
	$core->session->open($_POST['user'],$_POST['pass']);
}
// Or if try to log out
elseif (isset($_POST['logout'])||isset($_GET['logout']))
{
	$core->session->close();
}


if(is_file(dirname(__FILE__) . '/../skins/'.$core->user->getSkin().'/page.class.inc'))include_once (dirname(__FILE__) . '/../skins/'.$core->user->getSkin().'/page.class.inc');
if(is_file(dirname(__FILE__) . '/../skins/'.$core->user->getSkin().'/module.class.inc'))include_once (dirname(__FILE__) . '/../skins/'.$core->user->getSkin().'/module.class.inc');
if(is_file(dirname(__FILE__) . '/../skins/'.$core->user->getSkin().'/indexMenu.class.inc'))include_once (dirname(__FILE__) . '/../skins/'.$core->user->getSkin().'/indexMenu.class.inc');

?>
