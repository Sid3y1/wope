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
 * class Url gestion des url
 */
class Url
{

 private $baseUrl;

	private $baseUrls;

	private $baseDir;

  /**
   * __construct Constructeur qui permet de recuperer les variables de configurations
   * 
   * @param mixed $config 
   * @final
   * @access public
   * @return void
   */
	public final function __construct(){
  	global $config;
		$this->baseUrl = $config['baseUrl'];
	 	$this->baseUrls = $config['baseUrls'];
	 	$this->baseDir = $config['baseDir'];
	}
		
		
		/**
  * makeUrl Nouvelle fonction MakeUrl qui ne depend pas du serveur
  * 
  * @param string $path 
  * @access public
  * @return void
  */
 	public function makeUrl($path = ''){
		if ($path[0] == '/') {
			$base = $this->baseDir;
		} else {
			$base = $this->baseDir . '/';
		}
		$url = htmlentities($base . $path);
		return($url);
	} // end of member function makeUrl


  /**
		* Fonction redirect
		* Redirige l'utilisateur vers une autre page
		*
		* @param $url Url relatif de la page
		*/
	public function redirect($url) {
		//ereg("^(.*)/[^/]*$",$_SERVER['PHP_SELF'],$res_ereg);
		//$absolute_url="http://".$_SERVER['HTTP_HOST'].$res_ereg[1]."/".$url;
		if (!preg_match('/http:/',$url)) {
			$absolute_url=$this->makeUrl($url);
		} else {
			$absolute_url=$url;
		}
		if (!headers_sent ()) {
			header("HTTP/1.1 301 Moved Permanently");
			header("Location: $absolute_url");
			header("Connection: close");
			exit();
		} else {
			echo "\n<meta http-equiv=\"refresh\" content=\"0;URL=$absolute_url\">\n";
		}
	} // end of member function redirect

	public function checkUrl($url){
    // vérification du protocole HTTP
    if (!eregi("^http://", $url)) {
        return false;
    }
    $details = parse_url($url);
    // vérification du port 80 lié au protocole HTTP
    if (!isset($details['port'])) {
        $details['port'] = "80";
    }
    // vérification du chemin
    if (!isset($details['path'])) {
        $details['path'] = "/";
    }
    // vérification de l'adresse du serveur
    if (!ereg("[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+", $details['host'])){
        $details['host'] = gethostbyname($details['host']);
    }
    // ouverture d'une connexion vers le serveur
    if ($sock = @fsockopen( $details['host'], $details['port'], $errno, $errstr, 5)){
        $requete = "GET ".$details['path']." HTTP/1.1\r\n";
        $requete .= "Host: ".$details['host']."\r\n\r\n";
        // envoie d'une requête HTTP GET
        fputs($sock, $requete);
        // reception de la réponse du serveur
        $str = fgets($sock, 1024);
        while(!ereg('^HTTP/1.1 ', $str)){
            $str = fgets($sock, 1024);
        }
        // fermeture de la connexion
        fclose($sock);
        list($http, $code, $texte) = explode(" ", $str, 3);
        return $code;
    }
    return false;
}


} // end of Url
?>
