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
 * This is the file of the Core_Auth class.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */


/**
 * This class is the main authentification class of Wope.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 */
class Core_Auth
{
	/**
	 * This function is here to instanciate and initialize the authentification adapter we give him in param
	 * It returns the adapter fully configured.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @param string $authAdapter - The name of the adapter we want to get back.
	 * @return Zend_Auth_Adapter_Abstract - The adpater we wanted.
	 */
  public static function getAuthAdapter($authAdapter,$config)
  {
    switch ($authAdapter){
      default:
        try{
          $db = Core_Db::getDbAdapter();
          $adapter = new Wope_Auth_Adapter_DbTable($db, 'usersInfo', 'login', 'pass');
          $adapter->setIdentity($config['username']);
          $adapter->setCredential($config['password']);
          $adapter->setCredentialTreatment('PASSWORD(?)'); 
          return $adapter;
        }catch(Wope_Exception_Db $wde){
          throw $wde;
          return false;
        }catch(Zend_Auth_Exception $zae){
          throw new Wope_Exception_Auth($zae->getMessage());
          return false;
        }catch (Exception $e){
          Core_Exception::displayFullDebugMessage($e);
          return false;
        }
        break;
    }
  }
  
  public static function runAuthProcess($login,$password)
  {
    global $core;
    try{
      foreach($core->getConfig('auth','process') as $adapterName){
        $config['username'] = $login;
        $config['password'] = $password;
        $adapter = self::getAuthAdapter($adapterName,$config);
        $result = $adapter->authenticate();
        if ($result->isValid()){
          break;
        }
      }
    }catch(Exception $e){
      throw $e;
    }
    return $result->isValid();
  }
}

?>