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


class Mail {

	private $mailWebmaster = '';
	
	private $mailAsso = '';
	
	private $to = '';
	
	private $from = '';
	
	private $txt_msg = '';
	
	private $html_msg = '';
	
	private $subject = '';
	
	private $headers = '';
	
	private $include_dir = '';
	
	public function __construct(){
		global $core, $curPage;
		$this->mailWebmaster = $core->getConfig('mailWebmaster');
		$this->mailAsso = $core->getConfig('mailAsso');
//		$this->includeDir = $curPage['relativePath'] . $core->getConfig('includeDir');

	}

	public function send($from, $to, $message, $subject, $template = false) {
		global $core;
		$core->loadClass("string");
		$string = new String();

		if(is_array($message)){
			$this->html = true;
		
			foreach($message as $msg){
				if(!$string->isUTF8($msg)){
					$string->convertUTF8($msg);
				}
			}

			$this->text_msg = $message[0];
			$this->html_msg = $message[1];
		}else{
			$this->html = false;
			
			if(!$string->isUTF8($message)){
				$string->convertUTF8($message);
			}

			$this->text_msg = $message;
		}

		if(!$string->isUTF8($subject)){
			$string->convertUTF8($subject);
		}

		//On ajoute les entetes du mail
		if ($from == 'admin') {$from = 'Webmaster <' .$this->mailWebmaster . '>';}
		if ($to == 'admin')   {$to = 'Webmaster <' .$this->mailWebmaster . '>';}

		$this->to = $to;
		$this->from = $from;

		//Gestion du sujet
		$this->subject = '['.$core->getConfig('site_name').'] ' . $subject;
		
		//Création des entetes
		$this->headers = '';
		$this->headers .= "From: ".$this->from."\n";
		// Re-chemin de retour pour les erreurs
		//$this->headers .= "Return-Path: ".$this->from."\n";
		//specify MIME version 1.0
		$this->headers .= "MIME-Version: 1.0\n";

		$body = '';
		if($this->html){
			
			//unique boundary
			$boundary = uniqid("==Multipart_Boundary_");

			//tell e-mail client this e-mail contains//alternate versions
			$this->headers .= "Content-Type: multipart/alternative; boundary=\"$boundary\"\n\n";

			//plain text version of message
			$body .= "\n--$boundary\n";
		}
		
		$this->headers .= "Content-Type: text/plain; charset=UTF-8\n";
		$body .= $this->text_msg;

		if($this->html){
			//HTML version of message
			$body .= "\n\n--$boundary\n".
			"Content-Type: text/html; charset=UTF-8\n".
			$this->html_msg."\n".
			"\n--$boundary--\n";
		}

		if( mail($this->to, $this->subject, $body, $this->headers) ){
			debug('mail sent at '.$this->to);
			return true;
		}else{
			debug('mail error ...');
			debug($this->to);
			debug($this->subject);
			debug($body);
			debug($this->headers);
			return false;
		}
	}

}

?>
