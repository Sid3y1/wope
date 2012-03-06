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
 * This is the main class of the installer
 */
class Installer {
  
  /**
   * Attributs
   */

  /**
   * The config Array
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @var Array
   */
  private $config;
  
  /**
   * This variable contain the steps in session.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access priavte
   * @var Array
   */
  private $serializeSteps;
  
  /**
   * The array containing the name of the different steps. Its needed simplify some verification
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Array
   */
  private $steps;
  

  /**
   * Methods
   */
  
/**
   * Constructor
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function __construct(){
    include('config_installer.inc');
    $this->config = $config_installer;
     
   /**
 	* We start the session
 	*/

    session_start();
    
    $this->steps = Array('welcome','componentVerification','dbConfiguration','siteConfiguration','executeInstallation','createFirstUser','summary');
    
    // We include every class file we will need
	  include ('installerPage.class.inc');
    include ('step.class.inc');
	  include('./steps/welcome.class.inc');
	  include('./steps/componentVerification.class.inc');
	  include('./steps/dbConfiguration.class.inc');
	  include('./steps/siteConfiguration.class.inc');
	  include('./steps/executeInstallation.class.inc');
	  include('./steps/createFirstUser.class.inc');
	  include('./steps/summary.class.inc');

	
	// If we already have steps in the session, we retrieve them.
	if (isset($_SESSION['steps'])){
	  $this->serializeSteps = $_SESSION['steps'];
	}else{
      // We instanciate every steps we will need
	  $welcome = new Welcome('componentVerification','',_('Start installation'));
	  $componentVerification = new ComponentVerification('dbConfiguration','welcome',_('Component verification'),$this->config['mysqlVersion'],$this->config['phpVersion'],$this->config['phpModules']);
	  $dbConfiguration = new DbConfiguration('siteConfiguration','dbConfiguration',_('Database Configuration'));
	  $siteConfiguration = new SiteConfiguration('executeInstallation','dbConfiguration',_('Site configuration'));
	  $executeInstallation = new ExecuteInstallation('createFirstUser','siteConfiguration',_('Writing configuration file and creating Database'));
	  $createFirstUser = new CreateFirstUser('summary','executeInstallation','Create your first user');
	  $summary = new Summary('','createFirstUser',_('Summary'));
	  
	  // We put everything in session
	  $_SESSION['steps']['welcome'] = serialize($welcome);
	  $_SESSION['steps']['componentVerification'] = serialize($componentVerification);
	  $_SESSION['steps']['dbConfiguration'] = serialize($dbConfiguration);
	  $_SESSION['steps']['siteConfiguration'] = serialize($siteConfiguration);
	  $_SESSION['steps']['executeInstallation'] = serialize($executeInstallation);
	  $_SESSION['steps']['createFirstUser'] = serialize($createFirstUser);
	  $_SESSION['steps']['summary'] = serialize($summary);
	  
	  $this->serializeSteps = $_SESSION['steps'];
	}
	
  }

/**
   * Display the all installer
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function display(){

    // If a specific step is asked we retrieve it
    if (in_array($_REQUEST['step'],$this->steps)){
      $step = $this->getStep($_REQUEST['step']);
    }else{ // Otherwise we get the first step
      $step = $this->getStep('welcome');
    }
    $step->display();
  }
  
  /**
   * This function unserialize a step stored in session ans return it.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param String $step
   * @return Step $instance
   */
  public function getStep($step){
    
    if (isset($_SESSION['steps'][$step])){
      $instance = unserialize($this->serializeSteps[$step]);
    }
    return $instance;
 
  }
  
}




?>