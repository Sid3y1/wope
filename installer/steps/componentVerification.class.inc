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
 * This class aims to verify if the apache server contains all the required modules and if the mySQL server version is at least the 5.0
 */
class ComponentVerification extends Step {
  
  /**
   * Attributs
   */
  
  /**
   * The minimum mySQL version needed by the application
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  private $mysqlVersion;
  
  /**
   * The minimum PHP version needed by the application
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  private $phpVersion;
  
  /**
   * The list of the PHP modules needed by the application to work properly
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Array
   */
  private $phpModules;
  
  /**
   * Methods
   */
	
  /**
   * Constructor
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param $mysqlVersion String
   * @param $phpVersion String
   * @param $phpModules Array
   * @return void
   */
  public function __construct($nextStep, $prevStep, $title, $mysqlVersion, $phpVersion, $phpModules){

  	parent::__construct($nextStep,$prevStep,$title);
	$this->mysqlVersion = $mysqlVersion;
	$this->phpVersion = $phpVersion;
	$this->phpModules = $phpModules;
  }
	
  /**
   * Display the results of the tests
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayContent(){
    
    
    echo '<table>';
    
    // We display the result of the phpVersion test
		echo 	'<tr>
							<td>'._('PHP version').' (> '.$this->config['phpVersion'].') :</td>';
    if($this->testPhpVersion()){
      echo '<td class="green">'.phpversion().'</td>';
    }else{
      echo '<td class="red">'.phpversion().'</td>';
    }
    echo '</tr>';
      
    // We display the result of the mysqlVersion test
	      $phpModules = $this->parsePhpModules();
    		echo 	'<tr>
							<td>'._('mySQL version').' (> '.$this->config['mysqlVersion'].') :</td>';
    if($this->testMysqlVersion()){
      echo '<td class="green">'.$phpModules['mysql']['Client API version'].'</td>';
    }else{
      echo '<td class="red">'.$phpModules['mysql']['Client API version'].'</td>';
    }
    echo '</tr>';
    echo '</table>';
    
    // We display the result of phpModules test
	  echo '<table>';
	  echo '<tr>
							<td>'._('PHP extensions').'</td>
							<td>'._('Status').'</td>
						</tr>';
    $modulesResult = $this->testPhpModules();
    foreach ($modulesResult as $module => $status){
      echo '<tr>
							<td>'.$module.'</td>';
      if($status == 1){
        echo '<td class="green">'._('OK').'</td>';
      }else{
        echo '<td class="red">'._('Missing').'</td>';
      }
      echo '</tr>';
    }
    echo '</table>';
					
    

  }
	/**
	 * This function parse the phpinfo to get all the information about the php modules (function found on php.net http://fr2.php.net/phpinfo)
	 * 
	 * @author unknown
	 * @copyright code@adspeed.com
	 * @access private
	 * @return Array
	 */
  private function parsePhpModules() {
    ob_start();
    phpinfo(INFO_MODULES);
    $s = ob_get_contents();
    ob_end_clean();
 
    $s = strip_tags($s,'<h2><th><td>');
    $s = preg_replace('/<th[^>]*>([^<]+)<\/th>/',"<info>\\1</info>",$s);
    $s = preg_replace('/<td[^>]*>([^<]+)<\/td>/',"<info>\\1</info>",$s);
    $vTmp = preg_split('/(<h2>[^<]+<\/h2>)/',$s,-1,PREG_SPLIT_DELIM_CAPTURE);
    $vModules = array();
    for ($i=1;$i<count($vTmp);$i++) {
      if (preg_match('/<h2>([^<]+)<\/h2>/',$vTmp[$i],$vMat)) {
        $vName = trim($vMat[1]);
        $vTmp2 = explode("\n",$vTmp[$i+1]);
        foreach ($vTmp2 AS $vOne) {
          $vPat = '<info>([^<]+)<\/info>';
          $vPat3 = "/$vPat\s*$vPat\s*$vPat/";
          $vPat2 = "/$vPat\s*$vPat/";
          if (preg_match($vPat3,$vOne,$vMat)) { // 3cols
	          $vModules[$vName][trim($vMat[1])] = array(trim($vMat[2]),trim($vMat[3]));
          } elseif (preg_match($vPat2,$vOne,$vMat)) { // 2cols
	          $vModules[$vName][trim($vMat[1])] = trim($vMat[2]);
          }
        }
      }
    }
    return $vModules;
  }
  
  /**
   * Test the version of the mySQL server. We take the minimum version needed by the application ($mysqlVersion attribut) and the function return if the version is compatible.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @return boolean
   */
  private function testMysqlVersion(){
		
    $phpModules = $this->parsePhpModules();
    $installedVersion = $phpModules['mysql']['Client API version'];
    $requiredVersion = $this->config['mysqlVersion'];
    
    // If the highest version is the installed version
    if (version_compare($installedVersion,$requiredVersion) > -1 ){
      $return = true;
    }else{
      $return =false;
    }
    
    return $return;
  }
  
  /**
   * Test the version of PHP. We give the minimum version needed by the application ($phpVersion attribut) and the function return if the version is compatible.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @return boolean
   */
  private function testPhpVersion(){
    
    $installedVersion = phpversion();
    $requiredVersion = $this->config['mysqlVersion'];
    
    // If the highest version is the installed version
    if (version_compare($installedVersion,$requiredVersion) > -1){
      $return = true;
    }else{
      $return =false;
    }
    
    return $return;
  }
  
  /**
   * The function tests if all the php modules defined in the attribut $phpModules are enabled and return an array with the answer for each modules. 
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @return Array
   */
  private function testPhpModules(){
    
    $loadedExtensions = get_loaded_extensions();
    $requiredExtensions = $this->config['phpModules'];

    foreach ($requiredExtensions as $extension){
      if (in_array($extension, $loadedExtensions)){
        $return[$extension] = true;
      }else{
        $return[$extension] = false;        
      }
    }
    
    return $return;
  }
  
}


?>