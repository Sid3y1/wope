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
 * This is the file of the Wope loader class
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */

require_once('Zend/Loader.php');
require_once(dirname(__FILE__).'/../exceptions/Wope_Exception.class.inc');

/**
 * This classe extends the Zend_Loader classe and is used to load the classe form the Wope
 * library and core classes of the site
 *
 * @copyright  Wope
 * @author     Rémi Noel
 */
class Wope_Loader extends Zend_Loader
{
  
  /**
   * This function load the classes listed in the autoload array
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $class The name of the class we want to load
   * @param array|string $dirs An array of directory or a directory where the function should look to load the class
   * @return void
   */
  public static function loadClass($class, $dirs = null)
  {

    if (class_exists($class, false) || interface_exists($class, false)) {
      return;
    }

    if ((null !== $dirs) && !is_string($dirs) && !is_array($dirs)) {
      throw new Wope_Exception('Directory argument must be a string or an array');
    }
    if (null === $dirs) {
      $dirs = array();
    }
    if (is_string($dirs)) {
      $dirs = (array) $dirs;
    }

    require(dirname(__FILE__) . '/../../__autoload.inc');


    if (array_key_exists($class,$wopeAutoload)) { // We look in the autoload array
      $file = basename($wopeAutoload[$class]);
      $dirs[] = dirname($wopeAutoload[$class]);
    } else { // Otherwise it is in directories given in param
      $file = $class . '.class.inc';
    }
    
    try{
      self::loadFile($file, $dirs, true);
    }catch(Zend_Exception $e){
      throw new Wope_Exception($e->getMessage());
    }

    if (!class_exists($class, false) && !interface_exists($class, false)) {
      throw new Wope_Exception("File \"$file\" was loaded but class \"$class\" was not found in the file");
    }
  }

  /**
   * This function is used to autoload classes in the Wope framework.
   * 
   * It simply override the autoload function of the Zend_Loader class and is used by the
   * registerAutoload methode of the Zend_Loader class.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $class - The name of the class we want to load 
   * @return string|boolean - Classe name on success / false on failure
   */
  public static function autoload($class)
  {
    try {
      self::loadClass($class);
      return $class;
    } catch (Exception $e) {
      return false;
    }
  }
}

?>