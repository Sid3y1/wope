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

class Cache {

/**
	* This function create the cache file, we give it the object, and the display method, it execute the display method and put the result in the cache file
	* @param $object
	* @param $displayMethod
	* @param $cacheFile
	*/
	public function createFile($object, $displayMethod, $cacheFile) {
		// On créé le fichier de cache
		if ($handle_local = @fopen($cacheFile,'w')){
			ob_start();
				$object->$displayMethod();
				$cacheContent = ob_get_contents();
			ob_end_clean();

			// We write the cache file
			fwrite($handle_local, $cacheContent);
		}else{
			throw  new FileException ($cacheFile.' n\'est pas accessible en ecriture',2002,1);
		}
	}

	public function start(){
 		ob_start();
	}

	public function stopAndClean(){
		ob_end_clean();
	}

	public function stopAndWrite($cacheFile){
		// On créé le fichier de cache
		if ($handle_local = @fopen($cacheFile,'w')){
			$cacheContent = ob_get_contents();
			ob_end_clean();
			// We write the cache file
			fwrite($handle_local, $cacheContent);
		}else{
		    ob_end_clean();
			throw  new FileException ($cacheFile.' n\'est pas accessible en ecriture',2002,1);
		}
	}

	public function stopDisplayAndWrite($cacheFile){
			// On créé le fichier de cache
		if ($handle_local = @fopen($cacheFile,'w')){
			$cacheContent = ob_get_contents();
			//We display the page directly
			ob_end_flush();
			// We write the cache file
			fwrite($handle_local, $cacheContent);
		}else{
			throw  new FileException ($cacheFile.' n\'est pas accessible en ecriture',2002,1);
		}
	}


	public function stopAndDisplay(){
		ob_end_flush();
	}
	
/**
	* @param $cacheFile
	*/
	public function getFile($cacheFile) {
		$file = '';
		if (is_readable($cacheFile)){
			$file = file_get_contents($cacheFile);
		}else{
			throw new FileException ($cacheFile.' n\'est pas accessible en lecture',2001,1);
		}
		return $file;
	}

/**
	* @param $cacheFile
	*/
	public function delete($cacheFile){
		if (is_writable($cacheFile)){
			unlink($cacheFile);
		}else{
			throw  new FileException ('Impossible de detruire le fichier '.$cacheFile,2002,1);
		}
	}


/**
	* @param $cacheFile
	* @param $cacheTtl in hours
	*/
	public function isUsable($cacheFile, $cacheTtl) {
		if(!file_exists($cacheFile) || date("YmdH", filemtime($cacheFile))+$cacheTtl < date("YmdH") || date("Ymd",filemtime($cacheFile)) != date("Ymd")){	
			return false;
		}
		else {
			return true;
		}
	}

} // End of class
?>
