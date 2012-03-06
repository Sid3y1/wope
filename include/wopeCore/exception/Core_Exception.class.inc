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
 * This is the file of the Core_Exception class.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */


/**
 * This class is here to provide tools to handle exceptions in Wope.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 */
class Core_Exception
{
  
  /**
   * This function log the content of the exception, meaning that the exception will be displayed
   * and then written in the log file depending if the log level is high enough.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param Exception $exception - The exception we want to log.
   * @param string $level - The level at which we want to log the exception
   * @return void
   */
  public static function logFullDebugMessage($exception, $level)
  {
    global $core;
    $message = '<h4>Exception '.get_class($exception).': '.$exception->getMessage().'</h4>'
						 .'<strong>In the file :</strong> '.$exception->getFile().'<br />'
						 .'<strong>At line :</strong> '.$exception->getLine().'<br />'
						 .'<strong>The full trace is :</strong><br />'
						 .'<pre>'.$exception->getTraceAsString().'</pre>';

    try{
      $core->log->$level($message);
    }catch(Exception $e){}
  }

  /**
   * This function simply displays the full debug message of an exception on screen.
   * There no filtering or log of any kind.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param Exception $exception - The exception we want to display.
   * @return void
   */
  public static function displayFullDebugMessage($exception)
  {
    $message = '<div class="debug">
							   <h4>Exception '.get_class($exception).': '.$exception->getMessage().'</h4>
						     <strong>In the file :</strong> '.$exception->getFile().'<br />
						     <strong>At line :</strong> '.$exception->getLine().'<br />
						     <strong>The full trace is :</strong><br />
						     <pre>'.
									$exception->getTraceAsString().'
								 </pre>
								</div>';
		echo $message;
  }
}

?>