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

class SimpleHttp {

private $url;
private $cacheTtl;
private $cache;

	public final function __construct($url,$cacheTtl=24) {
		global $core;
		$this->url = $url;
		$this->cacheTtl = $cacheTtl;
		$this->cache = new Cache();
		$this->cacheFile = $core->getConfig('cacheServer').'/'.md5($url);
	}



	private function createCache(){
		
		try{
			$this->cache->start();
				echo $this->returnPage();
			$this->cache->stopAndWrite($this->cacheFile);
		}
		catch(FileException $fileError){
			throw $fileError;
		}	
	}

	public function getPage(){

		$page = '';
		try{
			if ($this->cache->isUsable($this->cacheFile,$this->cacheTtl)){
				$page = $this->cache->getFile($this->cacheFile);
			}else{
				$this->createCache();
				if (is_readable($this->cacheFile)){
					$page = $this->cache->getFile($this->cacheFile);
				}else{
				    $page = $this->returnPage();
					throw new FileException('The cache file we\'ve created is not readable so we don\'t use any cache to display this.',2001,1);
				}				
			}
		}
		catch(FileException $fileError){
			if ($fileError->getCode() < 2100){ // If we have a cache creation problem (a local file error 20xx)
				// We display the exception for debbuging purpose
				throw $fileError;
				// We use the normale page
				$page = $this->returnPage();
			}
			elseif($fileError->getCode() == 2101){ // If we cannot have acces to the file, or the url isn't right or the server is not available
				// In that case we change the user message and we re-throw the exception
				$fileError->userMessage = 'Sorry, service unavailable';
				throw $fileError; // We re-throw the exception
			}else{
				$fileError->getFullDebugMessage();
			}
								
		}
		return $page;
	}
			
		

	/**
	*
	*	
	*
	*
	*/

	private function returnPage(){
		global $core;
		// We create a buffer in which we will put the file
		$page = '';

		//if ($httpReturnCode = $core->url->checkUrl($this->url)){
		//	if ($httpReturnCode == '200'){ // If the http return code is a succes code : 200,
			
				//we put the page in the buffer	
				$handle = fopen($this->url, "r");
				if (isset($handle) && !empty($handle)) {
					while (!feof($handle)) {	
						$page .= fread($handle, 4096);
					}
				}else{
					throw new fileException('Unable to put the page in the buffer',2002,1);
				}
				
/*			}else{
				throw new fileException('This page is not available. http error code : '.$httpReturnCode,2101,1);
			}
		}else{
			throw new fileException("Mal-formed url",2100,1);
		}*/
		return $page;
	}
	
} // End of class
?>
