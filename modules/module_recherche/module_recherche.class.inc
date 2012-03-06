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


class Module_recherche extends Module {
	public $tabs;

	public final function __construct($infos = Array() ){
		parent::__construct($infos);
	}

  public function display()
  {
    global $page, $core;
		echo '<div id="search_tab" class="left">';
		$this->web_search();
		echo '</div>';
		echo '<div id="search_menu" class="left">';
		$this->search_menu();
		echo '</div>';
	}


	public function search_menu(){
		global $core;
		echo '<ul class="panel search_menu">
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_recherche/ajax/ajax.php?menu=web').'\',\'search_tab\')">Web</a></li>
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_recherche/ajax/ajax.php?menu=pictures').'\',\'search_tab\')">Images</a></li>
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_recherche/ajax/ajax.php?menu=video').'\',\'search_tab\')">Vidéos</a></li>
			</ul>';
	}

	public function pictures_search(){
		global $core;

		$core->loadClass('tabs');
		$this->pictureTabs = new Tabs(Array("pictures_google"=>"Google","pictures_flickr"=>"Flickr","pictures_yahoo"=>'Yahoo'),'');
		
		$this->pictureTabs->display();
		$this->pictures_google_form();
		$this->pictures_yahoo_form();
		$this->pictures_flickr_form();
	}

	
	public function video_search(){
		global $core;
		
		$core->loadClass('tabs');
		$this->videoTabs = new Tabs(Array("video_google"=>"Google","video_youtube"=>"Youtube"),'');
		$this->videoTabs->display();

		$this->video_google_form();
		$this->video_youtube_form();
//		$this->video_daily_motion_form();
	}

	public function web_search(){
		global $core;
		
		$core->loadClass('tabs');
		$this->webTabs = new Tabs(Array("web_google"=>"Google","web_yahoo"=>"Yahoo","web_wikipedia"=>'Wikipedia',"web_exalead"=>"Exalead"),'');
		$this->webTabs->display();

		$this->web_google_form();
		$this->web_yahoo_form();
		$this->web_wikipedia_form();
		$this->web_exalead_form();
	}


// Formulaire de recherches d'images	
 	
	public function pictures_google_form(){
		global $core;
		$this->pictureTabs->separatorStart("pictures_google");
			echo '<form action="http://images.google.fr/images" name="f" target="_blank">
							<p class="form-input">
	  <a href="http://images.google.fr"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/google.gif').'" alt="Google" /></a>
	  <input type="text" name="q" maxlength="255" />
	  <input type="submit" name="btnG" value="Rechercher" />
	  <input type="hidden" name="hl" value="fr" />
	  <input type="hidden" name="ie" value="ISO-8859-1" />
	  <input type="hidden" name="oe" value="ISO-8859-1" />
						</p>
					</form>';
 

		$this->pictureTabs->separatorStop("pictures_google");

	}

	
	public function pictures_yahoo_form(){
		global $core;
		$this->pictureTabs->separatorStart("pictures_yahoo");
  		echo '<form name="sf1" action="http://fr.search.yahoo.com/search/images" target="_blank"><p class="form-input">
	<a href="http://www.yahoo.fr"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/yahoo.gif').'" alt="Yahoo" /></a>
	<input type="text" name="p" />
	<input type="submit" value="Rechercher" />
	<input type="hidden" name="fr" value="FP-tab-web-t" />
	<input type="hidden" name="ei" value="UTF-8" />
	<input type="hidden" name="meta" value="vc=" />
</p></form>';
 

		$this->pictureTabs->separatorStop("pictures_yahoo");

	}
	
	
	public function pictures_flickr_form(){
		global $core;

		$this->pictureTabs->separatorStart("pictures_flickr");
			echo '<form action="http://www.flickr.com/search" method="get" target="_blank"><p class="form-input">
	  <a href="http://www.flickr.com"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/flickr.gif').'" alt="Flickr" /></a>
	  <input type="text" name="q" />
		<input type="hidden" name="z" value="t" />
	  <input type="submit" value="Rechercher" />
	</p></form>';

		$this->pictureTabs->separatorStop("pictures_flickr");

	}


// Forumaire de recherche de videos
	
	public function video_google_form(){
		global $core;
		$this->videoTabs->separatorStart("video_google");
   		echo '<form action="http://video.google.fr/videosearch" method="get" target="_blank"><p class="form-input">
	  <a href="http://video.google.fr"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/google.gif').'" alt="Google Video" /></a>
	  <input type="text" name="q" maxlength="255" />
	  <input type="submit" value="Rechercher" />
	</p></form>';


		$this->videoTabs->separatorStop("video_google");

	}

	
	public function video_youtube_form(){
		global $core;
		$this->videoTabs->separatorStart("video_youtube");
    		echo '<form action="http://www.youtube.com/results" method="get" target="_blank"><p class="form-input">
	  <a href="http://www.youtube.com"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/youtube.gif').'" alt="Youtube" /></a>
	  <input type="text" name="search_query" maxlength="255" />
		<input type="hidden" name="search" value="Search" />
	  <input type="submit" value="Rechercher" />
	</p></form>';
  
		$this->videoTabs->separatorStop("video_youtube");

	}
	


	// Ne marche pas à terminer plus tard
	public function video_daily_motion_form(){
		global $core;
		$this->videoTabs->separatorStart("video_daily_motion");
   		echo '<form action="http://www.dailymotion.com" method="get" target="_blank"><p class="form-input">
	  <a href="http://www.dailymotion.com"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/daily_motion.gif').'" alt="Daily Motion" /></a>
	  <input type="text" name="search" maxlength="255" />
	  <input type="submit" value="Rechercher" />
	</p></form>';


		$this->videoTabs->separatorStop("video_daily_motion");

	}

	
// Formulaire de recherche de pages Webs

	private function web_google_form(){
		global $core;
		$this->webTabs->separatorStart("web_google");
   
		echo '<form method="get" action="http://www.google.fr/search" target="_blank"><p class="form-input">
	  <a href="http://www.google.fr" target="_blank"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/google.gif').'" alt="Google" /></a>
	  <input type="text" name="q" maxlength="255" />
	  <input type="submit" name="btnG" value="Rechercher" />
	  <input type="hidden" name="ie" value="UTF-8" />
	  <input type="hidden" name="hl" value="fr" />
	  <input type="hidden" name="ie" value="ISO-8859-1" />
	  <input type="hidden" name="oe" value="ISO-8859-1" />
	</p></form>';

		$this->webTabs->separatorStop("web_google");
	}

	private function web_yahoo_form(){
		global $core;

		$this->webTabs->separatorStart("web_yahoo");

		echo '<form name="sf1" action="http://fr.search.yahoo.com/search" target="_blank"><p class="form-input">
	<a href="http://www.yahoo.fr" target="_blank"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/yahoo.gif').'" alt="Yahoo" /></a>
	<input type="text" name="p" />
	<input type="submit" value="Rechercher" />
	<input type="hidden" name="fr" value="FP-tab-web-t" />
	<input type="hidden" name="ei" value="UTF-8" />
	<input type="hidden" name="meta" value="vc=" />
</p></form>';

		$this->webTabs->separatorStop("web_yahoo");
	}

	private function web_exalead_form(){
	global $core;

		$this->webTabs->separatorStart("web_exalead");

		echo '<form action="http://www.exalead.fr/search?" method="get"  target="_blank"><p class="form-input">
<a href="http://www.exalead.fr" target="_blank"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/exalead.gif').'" alt="Exalead" /></a>
	<input name="U" value="" type="hidden" />
	<input name="C" value="0MGwAMwA=" type="hidden" />
	<input name="2q" value="" type="hidden" />
	<input name="q" type="text"><input type="hidden" name="scope" value="all" />
	<input type="submit" value="Rechercher" >
</p></form>';

		$this->webTabs->separatorStop("web_exalead");
	}
	
	private function web_wikipedia_form(){
	global $core;
		$this->webTabs->separatorStart("web_wikipedia");
	
		echo '<form action="http://fr.wikipedia.org/wiki/Special:Search" target="_blank"><p class="form-input">
			<a href="http://fr.wikipedia.org" target="_blank"><img class="logo_recherche" src="'.$core->makeUrl('modules/module_recherche/images/wikipedia2.gif').'" alt="Wikipedia" /></a>
			<input name="search" type="text">
			<input name="fulltext" class="searchButton" value="Rechercher" type="submit">
		</p></form>';
		$this->webTabs->separatorStop("web_wikipedia");
	}

	 
}
?>
