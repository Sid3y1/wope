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
 * This class is where we put all the graphical elements of the installer. We cannot use the regular Page class of the site because its not installed yet :-)
 */
class InstallerPage {
  
  /**
   * Attributs
   */

  /**
   * Methods
   */

  /**
   * Constructor
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function __construct(){
    
  }
  
  /**
   * This function displays the header of the installer
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayHeader(){
   
    // The head
    echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
		<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">
			<head>
				<title>Wope | Installer</title>
				<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
				<meta http-equiv="Content-Script-Type" content="text/javascript" />
				<meta http-equiv="Content-Style-Type" content="text/css" />
				<link rel="stylesheet" type="text/css" media="screen" title="Normal" href="./skin/style.css" />
			</head>
			<body>
				<div>';
    
    // The body of the header
    echo '';
		
  }
  
  /**
   * This function displays the footer of the installer
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayFooter(){
    
    // The body of the footer
    echo '';
    
    echo '		</div>
						</body>
					</html>';
  }
  
  /**
   * This function displays the header of a standard block content 
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayBlockHeader(){
    echo '<div style="border: 2px solid black">';   
  }
  
  /**
   * This function displays the footer of a standard block content 
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayBlockFooter(){
    echo '</div>'; 
  }
  
  /**
   * Display an error message
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param String $message
   * @return void
   */
  public function displayErrorMessage($message){
     
    echo '<div class="error">'.$message.'</div>';
  }
  
  /**
   * Display a information message
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param String $message
   * @return void
   */
  public function displayInfoMessage($message){
     
    echo '<div class="info">'.$message.'</div>';
  }
}
?>