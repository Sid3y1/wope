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


class Page extends PageBasic{
  
  public function header($titre = '', $classPage = '',$arianeWire = true,$menu=true){
  	global $core;
  
  	$this->headerHTML($titre,$classPage);
  	$stringPageId = ($classPage != '' ? ' id="'. $classPage .'"' : '');
  
    /*
  	echo '<script type="text/javascript">
  		printMiniPopup(\'load\',\''.Locale::display('Loading ...','kernel').'\',\'loading\');
  	</script>';
  	*/
  
  	echo '<div class="page"' . $stringPageId . '>';
  
  
  	echo '<div class="page_header">
  	  <h1>Wope</h1><p>'._('Web portal').'</p>
  		<a class="home" href="'. $core->makeUrl('/') .'"></a>
  		<a class="contact" href="mailto:contact@wope-project.org">Nous contacter</a>';
  	if($menu){
      $this->htmlBlock->menu->indexMenu->display();
  	}
  	echo '</div>';
  
  	if($arianeWire){
      $this->arianeWireDisplay();
  	}	
  }
  	
  	
  public function footer($arianeWire=true){
  	global $core, $db;
  	
		if($arianeWire) {
      $this->arianeWireDisplay();
		}
			
  		echo '<div class="clear"></div>';
			echo '<div id="footer">'
						.'<p>'
							.'<span>Wope Alpha </span>| Your web portal <br /><a onclick="window.open(\''.$core->makeUrl('./applications/bug_report/index.php').'\',\'bug_report\',\'height=450,width=400,top=100,left=50,resizable=yes,scrollbars=yes\')">Signaler un bug</a>'
						.'</p>'
					.'</div>';

  	$core->saveInSession();
  	$this->footerHTML();
  }
  	
}
?>
