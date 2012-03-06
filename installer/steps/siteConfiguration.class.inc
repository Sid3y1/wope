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
 * This class ask the user to give the configuration information and store them for the creation of the configuration file.
 */
class SiteConfiguration extends Step {

  /**
   * Attributs
   */
  
  /**
   * The name of the site
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  private $siteName;
  
  /**
   * The directory where the site is placed (on the Url level)
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  private $baseDir;
  
  /**
   * The Url where the site is hosted
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  private $baseUrl;
  
  /**
   * The directory where the site is placed (on the filesystem level)
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  private $baseServer;
  
  /**
   * The Email which will be used on every part of the site.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  private $email;
  
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
  public function __construct($nextStep, $prevStep, $title){
		
    parent::__construct($nextStep,$prevStep,$title);
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
	      if ($testResult['isBaseServerExists'] && $testResult['isBaseServerWritable']){
	        $page->displayInfoMessage(_('Configuration Ok'));
	        $this->isNextStepAllowed = true;
	      }
//	      elseif(!$testResult['isBaseServerExists']){
//	        $this->isNextStepAllowed = false;
//	        $page->displayErrorMessage(_('Base server : The directory that you gave does not exist'));
//	      }
	      elseif($testResult['isBaseServerWritable']){
	        $this->isNextStepAllowed = false;
	        $page->displayErrorMessage(_('Base server : The directory is not writable. I will cause problem during the creation of the config file'));
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
					.'<p><label>'._('Site name :').'</label><input type="text" name="site_name" value="'.$this->siteName.'" /></p>'
					.'<p><label>'._('Base directory :').'</label><input type="text" name="base_dir" value="'.$this->baseDir.'" /></p>'
					.'<p><label>'._('Base url:').'</label><input type="text" name="base_url" value="'.$this->baseUrl.'" /></p>'
					.'<p><label>'._('Base server :').'</label><input type="text" name="base_server" value="'.$this->baseServer.'" /></p>'
					.'<p><label>'._('Email :').'</label><input type="text" name="email" value="'.$this->email.'" /></p>'
					.'<p><input type="hidden" name="wope_action" value="store" /></p>'
					.'<p><input type="hidden" name="step" value="siteConfiguration" /></p>'
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
	  $this->siteName = $_POST['site_name'];
	  $this->baseDir = $_POST['base_dir'];
	  $this->baseUrl = $_POST['base_url'];
	  $this->baseServer = $_POST['base_server'];
	  $this->email = $_POST['email'];
	  
	  $_SESSION['steps']['siteConfiguration'] = serialize($this);
	}
	
 
  /**
   * This method test if the informations given by the user are correct and return an Array with as a key the name of the configuration variable and a boolean
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @return Array
   */
  private function testConfiguration(){
    
    $return = Array();

    //$return['isBaseServerExists'] = file_exists($this->baseServer);
    $return['isBaseServerWritable'] = is_writable($this->baseServer);

  }
  
}

?>