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
 * This class defines the basic elements of page
 */
class PageBasic
{

  /**
   * The array defining the ariane Wire
   *
   * @author Mathieu Chateau
   * @copyright Wope
   * @access private
   * @var Array
   */
	private $arianeWire;

	/**
	 * The title of the page
	 *
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access private
	 * @var String
	 */
	private $title;

	/**
	 * The instance of the htmlBlock class
	 * 
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access private
	 * @var HtmlBlock
	 */
	public $htmlBlock;

	/**
	 * Constructor
	 * 
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @return void
	 */
	public final function __construct(){
		global $ARIANEWIRE,$core;
		debug("constructeur de page",10);	
		$this->arianeWire = $ARIANEWIRE;
		$core->loadClass('htmlBlock');
		$this->htmlBlock = new HtmlBlock;
	}

	/**
	 * This method is only here for backward compatibility reasons and should not be used.
	 * It will be removed as soon as its not used anywhere.
	 */
	public	function adminLink($admin_part='home',$content='default',$right){
		global $core;
		$core->loadClass('link');
		$link = new Link();
		echo $link->toAdmin($admin_part,$content,$right);
	}

	/**
	 * This function displays the header of the page.
	 * 
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @param String $titre The title of the page
	 * @param String $classPage The class define what css and what Js script we want to load
	 * @param Boolean $arianeWire Do we want to display the ariane wire ?
	 * @param Boolean $menu Do we want to display the menu ?
	 * @return void
	 */
	public function header($titre = '', $classPage = '',$arianeWire=true,$menu=true){
		global $core;
	
		$this->headerHTML($titre,$classPage);
		$stringPageId = ($classPage != '' ? 'id="'. $classPage .'"' : '');
	
		echo '<div class="page"' . $stringPageId . '>';
		echo '<div class="page_header">
			<a class="home" href="'. $core->makeUrl('/') .'"></a>';		  
		if($menu){
		  $this->htmlBlock->menu->indexMenu->display();
		}
		echo '</div>';
	
		if($arianeWire){
		  $this->arianeWireDisplay();
		}	

	}

	/**
	 * Display the footer of the page.
	 *
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @param boolean $arianeWire
	 * @return void
	 */
	public function footer ($arianeWire=true) {
		global $core;
	
		if($arianeWire){
		  $this->arianeWireDisplay();
		}
		  
			echo '<div id="footer">
			</div>
	
			<script type="text/javascript">deletePopup(\'load\');</script>';


		$core->saveInSession();
		
		$this->footerHTML();
	}

	/**
	* This function display the footer and exit the script
	* 
	* @author Mathieu Chateau
	* @copyright Wope
	* @access public
	* @param String $text The error message which will be displayed just before the footer
	* @return void
	*/
	public function quit($text=''){
		if($text!='')
		{
			$this -> msgError($text);
		}
		$this -> footer();
		exit();
	}

	/**
	 * This function display the HTML header (html tag, head tag, etc)
	 * 
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @param String $titre The title of the page
	 * @param String $classPage It defines whate css and js script we want to load
	 * @return void
	 */
	public function headerHTML($titre = '',$classPage){
		global $core,$config;
		if (!empty($titre))
		{
			$this->title = $titre;
		}

?>
		<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
		<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">
		<head>
		<title><?=$core->getConfig('site_name').' | '.$this->title?></title>
		<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
<?php
	
		$meta_tags = $core->getConfig('meta');
		if(isset($meta_tags) && is_array($meta_tags)){
			foreach($meta_tags as $name => $content){
				echo '<meta name="'.$name.'" content="'.$content.'" />
				';
			}
		}
	
?>
		<meta http-equiv="Content-Script-Type" content="text/javascript" />
		<meta http-equiv="Content-Style-Type" content="text/css" />
		<link rel="icon" href="<?=$core->makeUrl( 'skins/'.$core->user->getSkin().'/images/favicon.png')?>" type="image/x-png" />
<?php
		
		//Fonction qui appele les pages css et javascripts 
		$this->loadExternalPages($classPage);
		

		//periodic reconnection and daemons launching
		if( $core->session->isAuthOk() ){

			echo '<script type="text/javascript">';
			
			if( $core->getConfig('timeUnit') && $core->getConfig('timeUnit') != '' ){
				echo 'function keepConnectionAlive(){
					AJAX.getAndCall(\''.$core->makeUrl('service/keepConnection/ajax/keepConnection.php').'?wope_action=reconnect\');
					if('.$core->getConfig('timeUnit').' != 0 ){
						setTimeout("keepConnectionAlive()", '.$core->getConfig('timeUnit').');
					}
				}
				';
			}

			if( $core->getConfig('timeToCheckMail') && $core->getConfig('timeToCheckMail') != '' && $core->user->getMyPref('site_check_mail') == 'true'){
				echo 'function checkMail(){
					AJAX.getAndCall(\''.$core->makeUrl('service/checkMail/ajax/checkMail.php?wope_action=check_mail').'\','.
						'function(){'.
							'NEWMAIL.activate(this.responseText);'.
							'if(gE(\'identification_new_mails_nb\')){'.
								'AJAX.getAndUpdate(\''.$core->makeUrl('service/checkMail/ajax/checkMail.php').'?wope_action=new_mails_nb\', \'identification_new_mails_nb\');'.
							'}'.
							'setTimeout("NEWMAIL.hide()", 5000);'.
						'}'.
					');
					if('.$core->getConfig('timeToCheckMail').' != 0 ){
						setTimeout("checkMail()",'.$core->getConfig('timeToCheckMail').');
					}
				}
		
				addEventListener(document, \'DOMContentLoaded\', checkMail, false);
				';
			}

			if( $core->getConfig('timeUnit') && $core->getConfig('timeUnit') != '' ){
				echo '
				addEventListener(document, \'DOMContentLoaded\', keepConnectionAlive, false);
				';
			}
			
			echo '</script>';
		}

?>
		</head>
		<body>
			
			<div class="chat_text" id="chat_text" style="display: none">&nbsp;</div>
		  <div id="HUD" class="loading">Chargement ...</div>
		  <div id="NEWMAIL" class="new_mail" style="display: none">New mail</div>
			
<?php
		
	  	//si navigation by clavier !
	  	if($core->user->getPref($core->getUserId(),"site_keynav")=='true'){
			echo '<div>';
	  		echo '<a href="'.$core->makeUrl(".").'" accesskey="'.$core->user->getPref($core->getUserId(),"raccourcis_accueil").'"></a>';
	  		echo '<a href="'.$core->makeUrl("forum").'" accesskey="'.$core->user->getPref($core->getUserId(),"raccourcis_forum").'"></a>';
	  		echo '<a onkeypress="widget();" accesskey="'.$core->user->getPref($core->getUserId(),"raccourcis_widget").'"></a>';
	  		echo '<a href="'.$core->makeUrl("mail").'" accesskey="'.$core->user->getPref($core->getUserId(),"raccourcis_mail").'"></a>';
	  		echo '<a href="'.$core->user->mailTo().'" accesskey="'.$core->user->getPref($core->getUserId(),"raccourcis_createmail").'"></a>';
			echo '</div>';
	  	}


        if ($core->getConfig('isPhpmyvisitesAvailable')){
			echo '<div style="display: none" />';
			echo '<!-- phpmyvisites -->
			<a href="http://www.phpmyvisites.net/" title="phpMyVisites | Open source web analytics"
			onclick="window.open(this.href);return(false);"><script type="text/javascript">
			<!--
			var a_vars = Array();
			var pagename=\'\';
	
			var phpmyvisitesSite = 3;
			var phpmyvisitesURL = "'.$core->getConfig('phpmyvisitesUrl').'/phpmyvisites.php"
			//-->
			</script>
			<script language="javascript" src="'.$core->getConfig('phpmyvisitesUrl').'/phpmyvisites.js" type="text/javascript"></script>
			<noscript><p>phpMyVisites | Open source web analytics
			<img src="'.$core->getConfig('phpmyvisitesUrl').'/phpmyvisites.php" alt="Statistics" style="border:0" />
			</p></noscript></a>
			<!-- /phpmyvisites --> ';
			echo    '</div>';
		}
				
	}

	/**
	 * Load the CSS and Js script 
	 *
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @param String $classPage Gives us the name of the things to load.
	 * @return void
	 */
	public function loadExternalPages($classPage)
	{  
		global $core,$db;
		//On a le moteur javascript qui a une classe php chez lui pour pouvoir etre mis a jour plus facilement
		$core->javascriptCore->loadScripts();
		debug('loadCss',10);
		
		echo '<link rel="stylesheet" type="text/css" media="screen" title="Normal" href="'.$core->makeUrl( 'skins/'.$core->user->getSkin().'/style.css').'" />
		';
	
		$classes = explode(' ', $classPage);
		if(	is_array($classes) ){
		
			foreach($classes as $class){
				if(is_file(dirname(__FILE__) . '/../../../skins/'.$core->user->getSkin().'/css/parties/'.$class.'.style.css')){
					echo '<link rel="stylesheet" type="text/css" media="screen" title="Normal" href="'.$core->makeUrl('skins/'.$core->user->getSkin().'/css/parties/'.$class.'.style.css').'" />
					';
				}else{
	 				echo '<link rel="stylesheet" type="text/css" media="screen" title="Normal" href="'.$class.'.style.css" />
					';
				}
			}
			
		}
	
		if($classPage == 'index'){
			$userId = $core->getUserId();
		  $modules = $db->query("SELECT mm.class FROM kernel_module_module mm INNER JOIN kernel_module_available ma ON mm.id = ma.id_module
			WHERE ( ma.strict= 'Y' AND '".$userId."' IN ( SELECT login FROM kernel_right_user_get WHERE idRight = ma.right ) )
					OR
					( ma.strict= 'N' AND '".$userId."' IN ( SELECT login  FROM kernel_right_user WHERE idRight = ma.right ) )
			");
			while( $mod = $db->fetchArray($modules) ){
				if( is_file(dirname(__FILE__) . '/../../../skins/'.$core->user->getSkin().'/css/modules/'.$mod['class'].'.style.css') ){
					echo '<link rel="stylesheet" type="text/css" media="screen" title="Normal" href="'.$core->makeUrl('skins/'.$core->user->getSkin().'/css/modules/'.$mod['class'].'.style.css').'" />';
				}
			}
											
		}
		
			
	}

	/**
	 * This function displays the HTML footer
	 * 
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @return void
	 */
	public function footerHTML () {
		global $db,$core;
			$db->close();
			$core->getConfig('pageFooter');
		echo '
				</div>
		   </body>
			</html>';
	}

	/**
	 * Load CSS
	 * 
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @param String $page The name of the CSS to load
	 * @return void
	 */
	public function css($page){
		global $core, $session;
		
		if(strpos($page,'.css') === false)
		{
			$page = $page.'.css';
		}
	
		if(file_exists($core -> makeUrl('skins/'.$core->user->getSkin().'/css/'.$page))){
			echo '<link rel="stylesheet" type="text/css" media="screen" href="' . $core -> makeUrl( 'skins/'.$core->user->getSkin().'/css/'.$page) . '" />';
		}else{
			echo '<link rel="stylesheet" type="text/css" media="screen" href="' . $page . '" />';
		}
	
	}

	/**
	 * Display an information message
	 *
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @param String $msgText The message to display
	 * @return void
	 */
	public function msgInfo($msgText)
	{
		$this->blockHeader(_('Information'), '', 'information');
		echo '<p>'. $msgText .'</p>';
		$this->blockFooter();
	}

	/**
	 * Display an error message
	 *
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @param String $msgText The message to display
	 * @return void
	 */
	public function msgError($msgText){	  
		$this->blockHeader(_('Error'));
		echo '<p>'. $msgText .'</p>';
		$this->blockFooter('');
	}

	/**
	 * Display an warning message
	 *
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @param String $msgText The message to display
	 * @return void
	 */
	public function msgWarning($msgText){	  
		$this->blockHeader(_('Warning'));
		echo '<p>'. $msgText .'</p>';
		$this->blockFooter('');
	}

	/**
	 * Display an alert message
	 *
	 * @author Mathieu Chateau
	 * @copyright Wope
	 * @access public
	 * @param String $msgText The message to display
	 * @return void
	 */
	public function msgAlert($msgText)
	{
		$this -> msgWarning($msgText);
	}

	/**
	 * This function define the head of a standard HTML block in Wope.
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @param String $blockTitle The title we display in the header of the block
	 * @param String $imagePath The path of the image displayed in the header.
	 * @return void
	 */
	public function blockHeader($blockTitle,$imagePath='',$blockClass=''){

	  echo '<div class="block '.$blockClass.'">
		  <div class="block_border">
		    <div class="title">
			<div class="extraDiv1"><span>&nbsp;</span></div>
			<h2>'.$blockTitle.'</h2>
		        <div class="clear"></div>
		    </div>
		    <div class="content">';
	  
	}
	
		/**
	 * This function define the footer of a standard HTML block in Wope.
	 * 
	 * @author Rémi Noel
	 * @copyright Wope
	 * @access public
	 * @return void
	 */
	public function blockFooter (){	
	  
		echo '<div class="clear"></div>
		        </div>
		      </div>
		    </div>';
	}
	
	/**
	 * This method is only here for backward compatibility reasons and should not be used.
	 * It will be removed as soon as its not used anywhere.
	 */
	public function moduleHeader($moduleTitle) {
		echo '<div class="module">
			<div class="module_border">
			  <div class="title">
			      <div class="extraDiv1"><span>&nbsp;</span></div>
			      <div class="extraDiv2"><span>&nbsp;</span></div>
			      <h2>'.$moduleTitle.'</h2>
		              <div class="clear">
			  </div>
			</div>
		      <div class="content">';
	}
	
	
	/**
	 * This method is only here for backward compatibility reasons and should not be used.
	 * It will be removed as soon as its not used anywhere.
	 */
	public function moduleFooter (){	
	  echo '<div class="clear"></div>
		    </div>
		  </div>
		</div>';
	}

	/**
	*
	* Fonction barreNavigation
	* Affiche une barre de navigation du type : "Accueil > Trombi > Recherche"
	*
	* @return void
	*/
	public function arianeWireDisplay(){
		if( isset($this->arianeWire) && is_array($this->arianeWire) && sizeof($this->arianeWire)>0 ) {
			echo '<ul class="ariane_wire">';
			$ariane_size = sizeof($this->arianeWire);
			$i = 1;
			foreach($this->arianeWire as $name => $link){
				echo '<li><a href="'.$link.'">'.$name;
				echo '</a></li>';
				if ($i<$ariane_size){ 
					echo' >'; 
				}
				$i++;
			} 
			echo '</ul>';
		}
	}
				
}

?>
