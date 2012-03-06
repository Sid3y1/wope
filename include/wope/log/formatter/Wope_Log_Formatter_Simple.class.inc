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
 * This is the file of the Wope_Log_Formatter_Simple class.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */

require_once('Zend/Log/Formatter/Simple.php');

/**
 * This class simply extends the Zend_Log_Formatter_Simple class.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 */
class Wope_Log_Formatter_Simple extends Zend_Log_Formatter_Simple
{
  
  /**
   * This function simply remove all the CR and then execute his parent function to format the log
   * message.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param array $event - The array containing the log message, the timestamp, etc.
   * @return void
   */
  public function format($event)
  {
    
    $event['message'] = str_replace("<br />"," ",$event['message']);
    $event['message'] = strip_tags($event['message']);
    $event['message'] = str_replace("\n"," ",$event['message']);
    $output = parent::format($event);
    
    return $output;
  }
  
}

?>