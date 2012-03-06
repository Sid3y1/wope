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
	* Locale Gere les traductions
	* 
	* @author Sid 
	* @license PHP Version 5.0 {@link http://www.php.net/license/5_0.txt}
*/
class Locale {
	
	//VK : Singleton 
	private static $instance;
	
	
	/**
     * Locale constructor
     */
	private function __construct() {
	}
	
	/**
    * Function getLocale give access to the Locale object 
    * @return Locale the current locale object
    */
	public static function getLocale() {
		if (!isset(self::$instance)) {
           self::$instance = new Locale();
       }

       return self::$instance;
	}
	
	/**
    * Function __clone Prevent users to clone the instance 
    */
   	public function __clone() {
    	trigger_error('Clone is not allowed.', E_USER_ERROR);
   	}
   	
   	//end Singleton
   	
   	
	/**
		* display afiche simplement une traduction
		* 
		* @param mixed $word 
		* @param mixed $zone 
		* @access public
		* @return void
		*/
	function display($word,$zone){
		global $label;
		if($zone=='')
		{
			$zone = basename($_SERVER['PHP_SELF'],".php");
		}
		//$retour = ($label[$zone][$word]);
		return $label[$zone][$word];
	}
	
	/**
	* Renvoie une chaine de charactère comme la fonction "label" mais en permettant 
	* d'y insérer des éléments variables
	*   
	* Renvoie la chaine de caractère définie dans la classe label  
	* en remplacant les éléments [1], [2], [3], ... [9] par la valeur des variables 
	* entrées en paramètre.
	* 
	* @param $word le code correspondant à la phrase à insérer
	* @param $zone section de label  
	*
	* @return Element label correspondant avec les variables incluses
	* 
	* @see label  
	*/   
	function displayVar($word,			$zone,			$var1='',			$var2='',			$var3='',			$var4='',			$var5='',			$var6='',			$var7='',			$var8='',			$var9='')	{
		global $label;
		if($zone=='')
		{
			$zone = basename($_SERVER['PHP_SELF'],".php");
		}
		$retour = ($label[$zone][$word]);
		for($i=1;$i<10;$i++)
		{
			$name = 'var'.$i;
			$n = $$name;
			$retour = str_replace('['.$i.']',$n,$retour);
		}
		return $retour;
	}

}


