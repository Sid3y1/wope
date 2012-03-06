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
 * This step allow the user to create the admin account.
 */
class CreateFirstUser extends Step {
  
  /**
   * The name of the first user
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  private $username;
  
    /**
   * The password of the first user
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var String
   */
  private $password;
  
  
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
   * This function will display the form asking the user give the information needed to create the first user.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return void
   */
  public function displayContent(){
    
    $page =new InstallerPage();
    
    switch($_REQUEST['wope_action']){
      default:
        $this->isNextStepAllowed = false;
        echo '<h2>'._('Create the administration account of the site').'<h2>';
        echo '<form method="post" action="index.php">'
			.'<input type="text" name="username" value="'.$this->username.'"'
			.'<input type="password" name="password" value="'.$this->password.'"'
			.'<input type="password" name="password2" value="'.$this->password.'"'
			.'<input type="hidden" name="wope_action" value="create_user" />'
			.'<input type="hidden" name="step" value="createFirstUser" />';
      break;
      
      case 'create_user':
        if ($_POST['password'] == $_POST['password2']){
          $result = $this->createUser();
          if ($result){
            $this->isNextStepAllowed = true;
            $page->displayInfoMessage('User creation successful');
          }else{
            $page->displayErrorMessage('User creation failed. Check that your database');
          }
        }else{
          $page->displayErrorMessage('The two passwords do not match');
        }
      break;
    }
  }

  
  /**
   * This function create the user (execute the SQL requests)
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @return boolean
   */
  private function createUser(){
    
    return $result;
  }
}

?>