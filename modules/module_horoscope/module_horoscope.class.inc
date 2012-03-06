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
 * The horoscope module class. It displays an RSS feed for each horoscope sign. 
 * It allows the user to choose his sign throught the preference system.
 */
class Module_horoscope extends Module {

  /**
   * The preference to be used in the module.
   * In this case the horoscope sign.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @var String
   */
	public $sign;

	/**
	 * The content of the config file
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @var Array
	 */
	private $config;
	
	/**
	 * The path to the module.
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @var String
	 */
	private $path;
	
	/**
	 * Constructor
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @param Array
	 */
	public final function __construct( $informations = Array() ) {
		parent::__construct($informations);
		global $core;
		include('config_horoscope.inc');
		$this->config = $config;
		$this->sign = $core->user->getMyPref('module_horoscope_sign');
		if ($this->sign != 'none'){
		  $this->dynamicTitle = $this->sign;
		}
		$this->path = $this->config['path'];
	}

	/**
	 * The regular display method of a module.
	 * This one display the content of the module throught the User_cache class.
	 *
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return void
	 */
  public function display() {
    global $db, $core, $chronos;
		echo '<div id="horoscope_content">';
		if ($this->sign != 'none'){
		  try{
		    $cache = new User_cache('horoscope','1',$this, 'displayContent');
		    $cache->displayCache();
		  }
		  catch (FileException $e){
		    echo $e->getFullDebugMessage();
		    return false;
		  }
		}else{
		  $this->displayChooseSign();
		}
		echo '</div>';
  }

  /**
   * Display the content of the module. It will be used to generate the cache.
   * 
   * @author Rémi Noel
   * @copyright Wope
   * @access public
   * @return boolean
   */
	public function displayContent() {
		$locale = Locale::getLocale();
		if( !$horoscope = $this->getData($this->sign) ){
			echo "<h4>Service indisponible pour l'instant veuillez réessayer ultérieurement</h4>";
			return false;
		}
				
		echo $horoscope;
		echo '<div class="clear"></div>';

	}

	/**
	 *	This function parse the rss feed and return an array which contain every data we need
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @param String $sign
	 * @return Array
	 */
	private function getData($sign) {
		global $core;
	
		$core->loadClass('rss');	
		try{
			$rss = new ParseRss('http://www.asiaflash.com/horoscope/rss_horojour_'.$sign.'.xml');
		}catch(FileException $e){
			echo $e->getFullDebugMessage();
			return false;
		}
		
		$horoscope = Array();
		$items = $rss->getItems();
		foreach ($items as $item){
			$content = $item->description[0];
		}
		return $content;
	}
	
	/**
	 * This function is used when the the user displays the module for the first and he does'nt have any preference.
	 * In that case it display a form which asks the user to choose his sign.
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access private
	 * @return void
	 */
	private function displayChooseSign(){
	  global $core, $db;
	  
	  $res = $db->query("SELECT * FROM prefs_values WHERE pvals_name='module_horoscope_sign' ORDER BY pvals_name asc");
	  
		echo '<form onsubmit="'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'ajax/horoscope.php').'?wope_action=update_sign&module_id='.$this->getId().'&sign=\'+this.choose_sign.options[this.choose_sign.selectedIndex].value,\'horoscope_content\');'.
					'return false;'.
					'">';
		echo '<p class="form-input"><select name="choose_sign">';
		echo '<option selected="selected" disabled="disabled">Sélectionner votre signe</option>';
		while($data = $db->fetchArray($res)){
		  if ($data['pvals_value']!='none'){
		    echo '<option value="'.$data['pvals_value'].'">'.$data['pvals_value'].'</option>';
		  }
		}
		echo '</select></p>';
		echo '<p class="form-submit"><input type="submit" value="Envoyer" /></p>';
		echo '</form>';
	}

	public function chooseSign($sign){
	  global $core, $user;
	    
	  $core->user->setPref($core->getUserId(),'module_horoscope_sign',$sign);
	  $this->sign = $sign;	  
	  $this->saveInSession();
	}

}

?>
