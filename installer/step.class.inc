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
 * This class is the basic class for a step. Every step will extend this class. It contains basic information about the step (name, previous step, next step) and some basic actions. 
 */
class Step {
  
  /**
   * Attributs
   */
  
  /**
   * The name of the next step. Its needed to create the link beteween all the steps.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access protected
   * @var String
   */
  protected $nextStep;
  
  /**
   * Same as above except this for the previous step
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access protected
   * @var String
   */
  protected $prevStep;
  
  /**
   * The name of the step
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  protected $title; 
  
    /**
   * The config file
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Array
   */
  protected $config; 
  
  /**
   * This variable activate or deactivate the next step button.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access protected
   * @var boolean
   */
  protected $isNextStepAllowed;
  
  
  
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
  public function __construct($nextStep,$prevStep,$title){

    include('config_installer.inc');
    $this->config = $config_installer;
  	$this->nextStep = $nextStep;
  	$this->prevStep = $prevStep;
  	$this->isNextStepAllowed = true;
  	$this->title = $title;
  }

  /**
   * This method display the button needed to go to the next step of the installer
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayNextButton(){
    if ($this->isNextStepAllowed){
      echo '<a id="nextButton" href="index.php?step='.$this->nextStep.'">'._('Next Step').'</a>';
    }else{

    }

  }
  
  /**
   * This method display the button needed to go to the previous step of the installer
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayPrevButton(){
  	echo '<a id="prevButton" href="index.php?step='.$this->prevStep.'">'._('Previous Step').'</a>';
  }
  
  /**
   * This function displays all the element of the step : the content, the button, ...
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function display(){
  	echo '<div class="leftColumn">';
  	$this->displayLeftColumn();
  	echo '</div>';
  	
  	echo '<div class="rightColumn">
						<div class="stepContent">';
  	$this->displayContent();
  	echo '</div>
					<div class="buttonBlock">';
		$this->displayPrevButton();
		$this->displayNextButton();
		echo '	</div>
					</div>';
  	
  }
  
  /**
   * This function displays the content of the step, its used in the display() function. There is no content in this function since its always redefined for a specific step.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayContent(){
  	
  }
  
  /**
   * This function display the left column where we print if each step is finished or not
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayLeftColumn(){
    
  }
  
}


?>