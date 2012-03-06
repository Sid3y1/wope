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
 * This is the file of the Wope_Db class
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */

require_once('Zend/Db.php');


/**
 * This classe is the db class of Wope. It extends the Zend_Db class.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 */
class Wope_Db extends Zend_Db
{
  
  /**
	 * Factory for Zend_Db_Adapter_Abstract classes.
   *
   * Additional keys are processed as key-value pairs for the adapter config array.
   *
   * @param  string $adapterName   Name of the adapter to return: 'pdo_mysql' -> Zend_Db_Adapter_Pdo_Mysql
   * @param  array  $config        OPTIONAL; an array of adapter configuration keys.
   * @return Wope_Db_Adapter_Abstract
   * @throws Wope_Exception
   */
  public static function factory($adapterName, array $config = array())
  {
    if (!is_string($adapterName) or !strlen($adapterName)) {
      throw new Wope_Exception('Adapter name must be specified in a string.');
    }

    $adapterName = strtolower($adapterName); // normalize input
    $adapterName = 'Wope_Db_Adapter_' .
    str_replace(' ', '_' , ucwords(str_replace('_', ' ', $adapterName)));

    try{
      Wope_Loader::loadClass($adapterName,dirname(__FILE__).'/adapter');
      return new $adapterName($config);
    }catch(Wope_Exception $we){
      throw $we;
      return false;
    }

    
  }
}

?>