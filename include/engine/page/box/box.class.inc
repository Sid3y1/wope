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
	* Box classe
	* 
	* @package 
	* @version $id$
	* @copyright Copyright
	* @author Sid 
	* @license PHP Version 5.0 {@link http://www.php.net/license/5_0.txt}
	*/
class Box {

	public final function __construct(  ) {
	}

	/**
		* help afiche une bule d'aide 
		* @param string $txt 
		* @access public
		* @return string
		*/
	public function help($txt){
		return '
			<div class="input">
			<a class="help" href="#" title="Aide">
			<span class="info_bubble">
			<span class="info_bubble-content">
			'.$txt.'
			</span>
			<span class="info_bubble-bottom">&nbsp;</span>
			</span>
			</a>
			</div>
			';
	}

	/**
	 * confirm Demande une confirmation avant de faire une action ! 
	 * 
	 * @param mixed $action 
	 * @param mixed $message 
	 * @access public
	 * @return string javascript !
	 */
	public function confirm($action,$message = "Etes vous sur ?"){
		$locale = Locale::getLocale();

		$content = '<p>'.addslashes($message).'</p><p align="center" class=\"form-confirm\"><input type=\"button\" value=\"'.$locale->display('yes', 'kernel').'\" onclick=\"dialBox.erase();'.addslashes($action).'\" />   <input type=\"button\" value=\"'.$locale->display('no', 'kernel').'\" onclick=\"dialBox.erase();\" /></p>';
	 $content=strtr($content,"\n\r\t\0","    ");
	 return 'dialBox.draw(\''.htmlentities($content,ENT_QUOTES,'UTF-8').'\', \'Confirmation\', false);';

	}
	
	
	/**
	 * info() displays an infomation and a button to close it 
	 * 
	 * @param mixed $message 
	 * @access public
	 * @return string javascript !
	 */

	public function info($message = ''){
	  
		$locale = Locale::getLocale();

		$content = '<p>'.addslashes($message).'</p><p align="center" class=\"form-confirm\"><input type=\"button\" value=\"'.$locale->display('close', 'kernel').'\" onclick=\"dialBox.erase();\" /></p>';
		$content=strtr($content,"\n\r\t\0","    ");
		return 'dialBox.draw(\''.htmlentities($content,ENT_QUOTES,'UTF-8').'\', \'\', false);';
	}

	/**
	 * prompt() displays an non-displayed input of a formulary in a box 
	 * 
	 * @param string $input_type
	 * @param string $input_id
	 * @param mixed $prompt
	 * @param string $label
	 * @param string $callback
	 * @access public
	 * @return string javascript !
	 */

	public function prompt($input_type, $input_id, $prompt = '', $label = '', $callback = ''){
		$locale = Locale::getLocale();

		$content = '<p>'.addslashes($prompt).'</p><form><p class=\"form-input\"><label>'.$label.'</label>';

		switch($input_type){
			case 'text':
				$content .= "<input type=\"text\" name=\"box_prompt_input\" id=\"box_prompt_input\" onblur=\"validateInput(this, \'SOMETHING\');\" />";
				break;
			default:
				//empty
				break;
		}
		
		$content .= "</p><p><input type=\"button\" value=\"OK\" onclick=\"".
			"if(this.form.box_prompt_input.value != \'\'){gE(\'".$input_id."\').value = this.form.box_prompt_input.value;".
			$callback.";".
			"dialBox.erase()};".
		"\" />".
		"<input type=\"button\" value=\"".$locale->display('cancel','kernel')."\" onclick=\"dialBox.erase();\" />".
		"</p></form>";
		$content=strtr($content,"\n\r\t\0","    ");
		return 'dialBox.draw(\''.htmlentities($content,ENT_QUOTES,'UTF-8').'\', \''.$locale->display('entry','kernel').' :\', false);';
	}

}
?>
