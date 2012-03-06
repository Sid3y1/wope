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
 * This step is the first step of the installer. It just displays a small introduction to explain the installation process to the user. 
 */
class Welcome extends Step {
  
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
   * This function display the content of the step
   *
   */
  public function displayContent(){
    echo '<h1>'._('Welcome in the Wope installer').'</h1>';
    echo '<p>'._('Follow all the steps of this installer and you will have a working Wope installed on your server').'</p>';
  }

}
?>