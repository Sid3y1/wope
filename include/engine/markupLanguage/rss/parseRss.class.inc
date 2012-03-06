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


class ParseRss {

	/**
	* The feed we want to parse
	*/

	private $items;
	
	public $title;
	public $link;
	public $description;
	public $copyright;
	public $language;
	public $webMaster;
	public $ttl;

	public final function __construct($feedUrl,$cacheTtl) {
		global $core;
		$core->loadClass('simplehttp');
		
		try{
			if( !$http = new SimpleHttp($feedUrl,$cacheTtl) ){
				throw new FileException('An error occured loading '.$feedUrl, 2100, 1, 'Une erreur est apparue lors de l\'accès à un document distant.');
			}
			
			$feed = $http->getPage();
			if( !$rss = @simplexml_load_string($feed) ){
				throw new FileException('An error occured loading XML '.$feedUrl, 2100, 1, 'Une erreur est apparue lors de la lecture d\'un document distant.');
			}

			$feed = $rss->channel;

			$this->title = (string)$feed->title;
			$this->link = (string)$feed->link;
			$this->items = $feed->item;
			$this->description = (string)$feed->description;
			$this->copyright = (string)$feed->copyright;
			$this->language = (string)$feed->language;
			$this->webaster = (string)$feed->webMaster;
			$this->ttl = (int)$feed->ttl;
		}
		catch(FileException $e){
		    throw $e;
			return false;
		}

	}

	public function getItems() {
		return $this->items;
	}

}
?>
