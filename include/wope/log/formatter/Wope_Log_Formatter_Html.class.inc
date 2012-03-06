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
 * This is the file of the Wope_Log_Formatter_Html class.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */

require_once 'Zend/Log/Formatter/Interface.php';

/**
 * This class is a formatter for the Wope_Log class. It allows an Html formatting for display purpose.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 */
class Wope_Log_Formatter_Html implements Zend_Log_Formatter_Interface
{

  /**
   * Format the log message with html for display purpose 
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param array $event - The event array which contain the message timestamp, etc.
   * @return string - Return the html formatted message to be displayed.
   */
  public function format($event)
  {

//			EMERG   = 0;  // Emergency: system is unusable
//			ALERT   = 1;  // Alert: action must be taken immediately
//			CRIT    = 2;  // Critical: critical conditions
//			ERR     = 3;  // Error: error conditions
//			WARN    = 4;  // Warning: warning conditions
//			NOTICE  = 5;  // Notice: normal but significant condition
//			INFO    = 6;  // Informational: informational messages
//			DEBUG   = 7;  // Debug: debug messages


    switch ($event['priority']){
      case 0:
        $cssClass = 'log_emerg';
				break;
      
      case 1:
        $cssClass = 'log_alert';
        break;
        
      case 2:
        $cssClass = 'log_crit';
        break;
        
      case 3:
        $cssClass = 'log_err';
        break;
      
      case 4:
        $cssClass = 'log_warn';
        break;
        
      case 5:
        $cssClass = 'log_notice';
        break;
        
      case 6:
        $cssClass = 'log_info';
        break;
        
      case 7:
        $cssClass = 'log_debug';
        break;
    }
    
    $output = '<div class="'.$cssClass.'">'
						.'<h3>Log message ('.$event['priorityName'].') :</h3>'
							.$event['message']
					.'</div>';
    return $output;
  }
}

?>