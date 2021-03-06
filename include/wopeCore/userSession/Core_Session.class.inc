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
 * This is the file of the Core_Session class.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */


/**
 * This class is the session class of Wope. It is used to manage the user 
 * session (openning, closing, etc)
 *
 * @copyright  Wope
 * @author     Rémi Noel
 */
class Core_UserSession
{

  /**
   * The instance of the class
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Core_UserSession
   */
	private static $instance;
	
	/**
	 * The session namespace where we will store the instance of the class.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @var Wope_Session_Namespace
	 */
	private $sessionNamespace;
  
  /**
   * Tells us if the user is authentified or not
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var boolean
   */
	private $authOk;

	/**
	 * This is the get method of the authOk variable.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return boolean - The content of the variable
	 */
	public function isAuthOk()
	{
		return (bool)$this->authOk;
	}

	/**
	 * The setter function of the authOk variabl
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @param boolean $value
	 * @return void
	 */
	private function setAuthOk($value){
	  $this->authOk = (boolean)$value;
	  $this->saveInSession();
	}
	
	private function __construct()
	{
	  $this->authOk = false;
	  try{
	    $this->sessionNamespace = new Wope_Session_Namespace(get_class($this));
	  }catch(Exception $e){
	    Core_Exception::logFullDebugMessage($e,3);
	  }
	}
	
  /**
	 * The standard getInstance function which return the instance of the class.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @return Core_UserSession
	 */
  public static function getInstance()
  {
    if (isset(self::$instance)){
      return self::$instance;
    }elseif(!isset(self::$instance) && isset($this->sessionNamespace->instance)){
      try{
        self::$instance = $this->getInstanceFromSession();
      }catch(Wope_Exception $we){
        Core_Exception::logFullDebugMessage($e,4);
      }
      
      return self::$instance;
    }else{
      self::$instance = new Core_UserSession();
      return self::$instance;
    }
  }

  /**
   * This function is here to retrieve the serialize instance of the class
   *
   * @return unknown
   */
  private static function getInstanceFromSession()
  {
    if (isset($this->sessionNamespace->instance) && get_class($this->sessionNamespace->instance) == get_class($this)){
      $instance = unserialize($this->sessionNamespace->instance);
    }else{
      throw new Wope_Exception('There is no instance of '.get_class($this).' in session');
    }
    
    return $instance;
  }
  
  /**
   * This function save the object in session
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function saveInSession()
  {
    $this->sessionNamespace->instance = serialize($this);
  }
  
	/**
		* This function open the session, meaning it verify the identity of the user and then 
		* load all his information.
		*
		* @param string $login - The login of the user
		* @param string $password - The password of the user
		* @return empty
		*/
	public function open($login,$password)
	{
		global $core;
		//TODO Utiliser les methodes du ZF
		$login = strtolower(trim($login));
		try{
		  if (Core_Auth::runAuthProcess($login, $password)) {
		    $this->setAuthOk(true);
		
		    $core->user->saveInformations($login,$password);

		    // If the user has been banned we cloose his session.
			  if ($core->verifDroits('ban_site')) {
				  $this->close();
			  }
		  		 
			  // We log the session openning
			  $this->authLog($login, 'Y');
			  
			  return true;
		  } else {
			  $this->authLog($login, 'N');
			  return false;
			  exit;
		  }
	  }catch(Exception $e){
	    throw $e;
	  }
	}

	/**
	 * This function closes the session of the user.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return void
	 */
	public function close()
	{
	 global $core;
		$this->setAuthOk(false);


		Wope_Session::
		Wope_Session::destroy();
		session_regenerate_id();
		@session_start();

		$core->url->redirect('index.php?loggedout');
		exit;
	}

	public function saveInSession()
	{
	  
	}
	
	/**
	 * This function log the events on the session.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @param string $login - The login which was used to open the session.
	 * @param string $result - The result of the attempt (Y or N)
	 * @return void
	 */
  private function authLog($login, $result='N')
  {
    global $core;
    //TODO Db logging would be interesting
    try{
      $core->log->info($login.' '.$result);
    }catch(Exception $e){
      echo $e->getTraceAsString();
      Core_Exception::displayFullDebugMessage($e);
    }
	}

}
?>
