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
	* The purpose of this class is to generate a cache file per user
	* for applications or modules where the user can configure things (like Menu, or meteo)
	*/
class User_cache extends Cache {

	private $cacheTtl;			// The time in hour we want the file to live, after this time, the file is regenerated
	private $cacheFile;			// The complete path to the cache file
	private $object;				// The object 
	private $displayMethod;	// The display method we want to execute in the object
	private $debug;          // If the debug option is on the cache will not be used.


	public final function __construct($cacheDir, $cacheTtl, $object, $displayMethod, $debug=false) {
		global $core;	

		$this->debug = $debug;
		$this->cacheTtl	= $cacheTtl;
		$this->cacheFile = $core->getConfig('cacheServer').'/'.$cacheDir.'/'.$core->getUserId();
		$this->object = $object;
		$this->displayMethod = $displayMethod;
	}

	public function displayCache() {
		try{
		    if ($this->debug==true){
		      	$this->displayRealContent();
		    }
			elseif ($this->isUsable($this->cacheFile, $this->cacheTtl)) {
				echo $this->getFile($this->cacheFile);
			} else {
				// If not we try to create it !
				$this->createFile($this->object, $this->displayMethod, $this->cacheFile);
				if (is_readable($this->cacheFile)){
					echo $this->getFile($this->cacheFile);
				}else {
					throw new FileException ('Le fichier de cache '.$this->cacheFile.' qui vient d\'etre généré n\'est pas lisible',2001,1);
				}
			}	
		}
		catch(FileException $error){
			$this->displayRealContent();
			throw $error;
		}
	}

	private function displayRealContent() {
		$object = $this->object;
		$displayMethod = $this->displayMethod;
		$object->$displayMethod();
	}

	

} // End of class
?>
