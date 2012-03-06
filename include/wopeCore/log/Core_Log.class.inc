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
 * This is the file of the Core_Log class.
 *
 * @copyright  Wope
 * @author     Rémi Noel
 * @since      File available since Release 1.0
 */


/**
 * This classe encapsulate the Wope_Log class. It allows the use of two type of logging
 * and simplify the "setup" of the logging system by instanciting everything on his constructor. 
 *
 * @copyright  Wope
 * @author     Foo
 */
class Core_Log
{
  /**
   * The instance of Wope_Log which will be used to log our messages.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Wope_Log
   */
  private $logger;
  
  /**
   * This is the writer which will display the messages directly on the standard output 
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Wope_Log_Writer_Stream
   */
  private $displayWriter;
  
  /**
   * This is the writer which will write the messages on file. 
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Wope_Log_Writer_Stream
   */
  private $fileWriter;
  
  /**
   * This formatter will be used to display the message on the standard output with specific html style.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Wope_Log_Formatter_Html
   */
  private $htmlFormatter;
  
  /**
   * This formatter will be used to write the message with the standard in line formatting.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Wope_Log_Formatter_Simple
   */
  private $simpleFormatter;
  
  /**
   * This filter will be used to decide at which level the messages should be displayed.
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access private
   * @var Wope_Log_Filter_Priority
   */
  private $priorityFilter;
  
  
  /**
   * The constructor instanciate every classes needed for the log process
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param  $displayLevel
   * @param unknown_type $logFile
   */
  public function __construct($logFile,$displayLevel,$fileLevel)
  {
    global $core;
    
    try{
      $this->displayWriter          = new Wope_Log_Writer_Stream('php://output');
      $this->fileWriter             = new Wope_Log_Writer_Stream($logFile);
      $this->htmlFormatter          = new Wope_Log_Formatter_Html();
      $this->simpleFormatter        = new Wope_Log_Formatter_Simple();
      $this->priorityFilterDisplay  = new Wope_Log_Filter_Priority($displayLevel);
      $this->priorityFilterFile     = new Wope_Log_Filter_Priority($fileLevel);
      $this->logger                 = new Wope_Log();
    
      $this->displayWriter->setFormatter($this->htmlFormatter);
      $this->displayWriter->addFilter($this->priorityFilterDisplay);
    
      $this->fileWriter->setFormatter($this->simpleFormatter);
      $this->fileWriter->addFilter($this->priorityFilterFile);
   
      $this->logger->addWriter($this->displayWriter);
      $this->logger->addWriter($this->fileWriter);
    }catch(Wope_Exception $we){
      Core_Exception::displayFullDebugMessage($we);
    }catch(Zend_Exception $ze){
      Core_Exception::displayFullDebugMessage($ze);
    }catch(Exception $e){
      Core_Exception::displayFullDebugMessage($e);
    }
  }

  /**
   * The function create an log message of level : emerg
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $message - The log message
   * @return void
   */
  public function emerg($message)
  {
    try{
      $this->logger->emerg($message);
    }catch(Zend_Exception $e){
      throw new Wope_Exception($e->getMessage());
    }
  }

  /**
   * The function create an log message of level : alert
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $message - The log message
   * @return void
   */  
  public function alert($message)
  {
    try{
      $this->logger->alert($message);
    }catch(Zend_Exception $e){
      throw new Wope_Exception($e->getMessage());
    }
  }
  
  /**
   * The function create an log message of level : crit
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $message - The log message
   * @return void
   */  
  public function crit($message)
  {
    try{
      $this->logger->crit($message);
    }catch(Zend_Exception $e){
      throw new Wope_Exception($e->getMessage());
    }
  }
  
  /**
   * The function create an log message of level : err
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $message - The log message
   * @return void
   */  
  public function err($message)
  {
    try{
      $this->logger->err($message);
    }catch(Zend_Exception $e){
      throw new Wope_Exception($e->getMessage());
    }
  }
  
  /**
   * The function create an log message of level : warn
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $message - The log message
   * @return void
   */  
  public function warn($message)
  {
    try{
      $this->logger->warn($message);
    }catch(Zend_Exception $e){
      throw new Wope_Exception($e->getMessage());
    }
  }
  
  /**
   * The function create an log message of level : notice
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $message - The log message
   * @return void
   */  
  public function notice($message)
  {
    try{
      $this->logger->notice($message);
    }catch(Zend_Exception $e){
      throw new Wope_Exception($e->getMessage());
    }
  }

  /**
   * The function create an log message of level : info
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $message - The log message
   * @return void
   */
  public function info($message)
  {
    try{
      $this->logger->info($message);
    }catch(Zend_Exception $e){
      throw new Wope_Exception($e->getMessage());
    }
  }
  
  /**
   * The function create an log message of level : debug
   *
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @param string $message - The log message
   * @return void
   */
  public function debug($message)
  {
    try{
      $this->logger->debug($message);
    }catch(Zend_Exception $e){
      throw new Wope_Exception($e->getMessage());
    }
  }
  
}

?>