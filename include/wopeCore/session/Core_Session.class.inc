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
class Core_Session {
  
  /**
   * Tells us if the user is authentified or not
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var boolean
   */
	private $authOk = false;
	
	/**
	 * This id of the session.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @var int
	 */
	private $sessionId = '';

	/**
	 * The constructor.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @param int $sessionId - The id of the session
	 * @return void
	 */
	public final function __construct($sessionId)
	{
		$this->sessionId= $sessionId;
	}
	
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
		    $this->authOk = true;
		
		    $core->user->saveInformations($login,$password);

		    // If the user has been banned we clase his session.
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
		$this->authOk = false;

		//TODO Utiliser la gestion de session Zend
		session_unset();
		session_destroy();
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
