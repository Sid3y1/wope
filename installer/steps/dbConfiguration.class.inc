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
 * This class display the form needed to configure the mySQL connection and test if the information given by the user are correct
 */
class DbConfiguration extends Step {

	/**
	 * Attributs
	 */

  /**
   * The host of the mySQL server
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
	private $mysqlHost;
	
	/**
	 * The name of the user who will have access to the database
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @var String
	 */
	private $mysqlUser;
	
	/**
	 * The name of the mySQL base
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @var String
	 */
	private $mysqlBase;
	
	/**
	 * The mySQL password needed to access the mySQL base 
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @var String
	 */
	private $mysqlPassword;
	

	/**
	 * Methods
	 */

	/**
	 * Constuctor
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return void
	 */
	public function __construct($nextStep, $prevStep, $title){
		
      parent::__construct($nextStep,$prevStep,$title);
	}
	
	/**
	 * Some access methods
	 */

	/**
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return String
	 */
	public function getMysqlHost(){
	  return $this->mysqlHost;
	}
	
	/**
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return String
	 */
	public function getMysqlBase(){
	  return $this->mysqlBase;
	}
	
	/**
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return String
	 */
	public function getMysqlUser(){
	  return $this->mysqlUser;
	}
	
	/**
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return String
	 */	
	public function getMysqlPassword(){
	  return $this->mysqlPassword;
	}
	
	/**
	 * Display the forms needed to get the informations from the user
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return void
	 */
	public function displayContent(){
	  
	  $page = new InstallerPage();
	  
	  switch($_POST['wope_action']){
	    default:
	      $this->isNextStepAllowed = false;
		    $this->displayForm();
		  break;
		  
	    case 'store':
	      $this->storeFormInformation();
	      $testResult = $this->testConfiguration();
	      
	      $this->displayForm();
	      if ($testResult['connection'] && $testResult['baseSelection']){
	        $page->displayInfoMessage(_('Configuration Ok'));
	        $this->isNextStepAllowed = true;
	      }
	      elseif(!$testResult['connection']){
	        $this->isNextStepAllowed = false;
	        $page->displayErrorMessage(_('There is a problem in your mySQL configuration : Connection impossible'));
	      }
	      elseif($testResult['connection'] && !$testResult['baseSelection']){
	        $this->isNextStepAllowed = false;
	        $page->displayErrorMessage(_('There is a problem in your mySQL configuration :').' '.$this->mysqlBase.' '._('base does not exist'));
	      }
	    break;
	      
	  }
	}
	
	
	/**
	 * This function displays the form that the user have to fill in with the db informations.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @return void
	 */
	private function displayForm(){
	  echo '<form method="post" action="index.php">'
					.'<p><label>'._('mySQL host :').'</label><input type="text" name="mysqlhost" value="'.$this->mysqlHost.'" /></p>'
					.'<p><label>'._('mySQL user :').'</label><input type="text" name="mysqluser" value="'.$this->mysqlUser.'" /></p>'
					.'<p><label>'._('mySQL base :').'</label><input type="text" name="mysqlbase" value="'.$this->mysqlBase.'" /></p>'
					.'<p><label>'._('mySQL password :').'</label><input type="password" name="mysqlpassword" value="'.$this->mysqlPassword.'" /></p>'
					.'<p><input type="hidden" name="wope_action" value="store" /></p>'
					.'<p><input type="hidden" name="step" value="dbConfiguration" /></p>'
					.'<p><input type="submit" value="'._('Validate').'"</p>'
				.'</form>';
	}
	
	/**
	 * This function put the content of the form in the attributs and update the instance of the step stored in session.
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @return void
	 */
	private function storeFormInformation(){
	  $this->mysqlHost = $_POST['mysqlhost'];
	  $this->mysqlBase = $_POST['mysqlbase'];
	  $this->mysqlUser = $_POST['mysqluser'];
	  $this->mysqlPassword = $_POST['mysqlpassword'];
	  
	  $_SESSION['steps']['dbConfiguration'] = serialize($this);
	}
	
	/**
	 * This function test if the information given by the user are correct or not
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return Array
	 *
	 */
	public function testConfiguration(){
		
	  $link = @mysql_connect($this->mysqlHost, $this->mysqlUser, $this->mysqlPassword);
	  $return['connection'] = $link;
    $dbSelected = @mysql_select_db($this->mysqlBase, $link);
    $return['baseSelection'] = $dbSelected;

    return $return;
	  
	}
	
}