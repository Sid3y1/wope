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


class Html_cache {
//
	private $cache_file; 	// Name of the cache file

	private $cache_ttl;		// The time in hour we want the file to live, after this time, the file is recreated


	public final function __construct($cache_file, $cache_ttl) {
		global $core;	
		
		$this->cache_ttl	= $cache_ttl;
		$this->cache_file = $core->getConfig('cacheServer').'/'.$cache_file;
	}

	public function createFile($content) {
		// On créé le fichier de cache
		if (!$handle_local = fopen($this->cache_file,'w')){
			debug('Erreur');
			exit;
		}
	
		// On écrit le fichier de cache
		fwrite($handle_local, $content);
	}

	public function readFile() {
		echo file_get_contents($this->cache_file);
	}

	public function isUsable() {
		if(!file_exists($this->cache_file) || date("YmdH", filemtime($this->cache_file))+$this->cache_ttl < date("YmdH") || date("Ymd",filemtime($this->cache_file)) != date("Ymd")){	
			return false;
		}
		else {
			return true;
		}
	}
		
		
		

}
?>
