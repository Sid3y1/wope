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
 * This is the file of the Core_Db class.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */


/**
 * This class is the main Db class of Wope.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 */
class Core_Db
{
	/**
	 * This function is here to help us get a db adapter automaticly. It uses the config array to determine
	 * what adapter we have to create and what configuration we need to use.
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return Zend_Db_Adapter_Abstract
	 */
  public static function getDbAdapter()
  {
    global $core;
    try{
      $db = $db = Wope_Db::factory($core->getConfig('database','adapter'),array(
    'host'     => $core->getConfig('database','host'),
    'username' => $core->getConfig('database','user'),
    'password' => $core->getConfig('database','pass'),
    'dbname'   => $core->getConfig('database','base')
));
    return $db;
    }catch(Zend_Db_Exception $e){
      throw new Wope_Exception_Db($e->getMessage);
      return false;
    }
  }

  /**
   * This function is used to format the exception thrown by the Zend_Db_Adapter_Pdo_Mysql class.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param Zend_Db_Exception $dbException - The exception we want to format.
   * @param Wope_Log $logger - The logger used to log the exception message.
   * @return void
   */
  public static function logDbError($dbException)
  {
    global $core;
    $message = '<h4>Db error : '.$dbException->getMessage().'</h4>'
						 .'<pre>'.$dbException->getTraceAsString().'</pre>';

    try{
      $core->log->warn($message);
    }catch(Wope_Exception $we){
      Core_Exception::logFullDebugMessage($we, 'warn');
    }catch(Exception $e){
      Core_Exception::displayFullDebugMessage($e);
    }
  }
}

?>