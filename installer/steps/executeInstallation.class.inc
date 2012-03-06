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
 * With this step we write in the config file all the information given by the user and we execute the SQL dump
 */
class ExecuteInstallation extends Step {
  
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
   * This function asks the user if he wants to install the site. And displays the message after the installation (failure or success).
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayContent(){
    
    $page = new InstallerPage();
    
    switch ($_REQUEST['wope_action']){
      default:
        $this->isNextStepAllowed = false;
        echo '<h2>'._('Do you really want to install Wope').'</h2>';
        echo '<h2>'._('If you say yes, the config file will be written and the SQL dump executed').'</h2>';
        echo'<form method="post" action="index.php">'
		.'<input type="hidden" value="install" name="wope_action" />'
		.'<input type="hidden" value="executeInstallation" name="step" />'
		.'<input type="submit" value="Install" />';
      break;
      
      case 'install':
        // We execute the dump
        $result = $this->executeDump('../dump-wope.sql');
        if ($result){
          $page->displayInfoMessage(_('The database was successfuly created'));
        }else{
          $this->isNextStepAllowed = false;
          $page->displayErrorMessage(_('Database creation failed. Modifiy your database configuration or try to execute the dump yourself using mysqldump'));
        }
        // We write the config file.
	    $result = $this->writeConfig();
        if ($result){
          $page->displayInfoMessage(_('The configuration file was successfuly written'));
          $this->isNextStepAllowed = true;
        }else{
          $this->isNextStepAllowed = false;
          $page->displayErrorMessage(_('Configuration file creation failed. Please pay attention to the writting permissions in config file directory'));
        }
      break;
    }
    
  }
  
  /**
   * This function execute the dump with the configuration given by the user. If this is a success the function return true, false otherwise.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access protected
   * @param String $dumpFileUrl
   * @return boolean
   */
  protected function executeDump($dumpFileUrl){

    $dbConfiguration = unserialize($_SESSION['steps']['dbConfiguration']);
    
    $dbName = $dbConfiguration->getMysqlBase();
    $dbHost = $dbConfiguration->getMysqlHost();
    $dbUser = $dbConfiguration->getMysqlUser();
    $dbPassword = $dbConfiguration->getMysqlPassword();
    
    $mysql = new PDO('mysql:host=localhost;dbname='.$dbName, $dbUser, $dbPassword);
    $fileContent = file($dumpFileUrl);
    
    $this->parseDump($mysql, $fileContent);
    
    return true;
  }
  
  /**
   * This function parse the dump file and execute the SQL directives.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @param PDO $connexion
   * @param String $fileContent
   * @return boolean
   */
  private function parseDump($connexion,$fileContent){

  $query = "";
  foreach($fileContent as $sql_line) {
    $tsl = trim($sql_line);
    if (($tsl != "") && (substr($tsl, 0, 2) != "--") && (substr($tsl, 0, 1) != "#")) {
      $query .= $sql_line;
      if(preg_match("/;\s*$/", $sql_line))
      { 
        $connexion->exec($query);

        $query = "";
      }
    }
  }
  return true;
  }
  
  /**
   * This function modifiy the model of config file to implement user configuration and then copy it in the right directory. In case of success the function return true, false otherwise.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access protected
   * @return boolean
   */
  protected function writeConfig(){
    return true;
  }
  
  
  
}

?>