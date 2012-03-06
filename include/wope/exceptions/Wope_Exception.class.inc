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
 * This is the file of the Wope_Exception class
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */


/**
 * This class is the main exception class of Wope it basicly extends the standard exception class of PHP.
 * 
 *
 * Description longue de la classe s'il y en a une
 *
 * @copyright  Wope
 * @author     Rémi Noel
 */
class Wope_Exception extends Exception
{
  
  /**
   * A message for the user. You can use this if you want to display a message for the user which is 
   * different from the regular debug message.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access protected
   * @var string
   */
  protected $userMessage;

  
  public function __construct($debugMessage, $userMessage=null)
  {
		parent::__construct($debugMessage);
		$this->userMessage = $userMessage;
  }
  
 
  public function getUserMessage()
  {
    return $this->userMessage;
  }
  
}

?>