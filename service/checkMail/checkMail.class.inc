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


class CheckMail{

	public function displayNewMail(){
		global $core;

		$check_mail = $core->user->getMyPref('site_check_mail');

		if( !isset($_SESSION['newmail']) ){
			$_SESSION['newmail'] = Array('nb' => 0, 'messages' => Array());
		}

		if( $check_mail == 'true'){
	
			if( $mbox = @imap_open(
					'{'. 
						$core->getConfig('mainMail', 'host') . 
						':' . $core->getConfig('mainMail', 'port') . 
						'/'. $core->getConfig('mainMail', 'type') . 
						( $core->getConfig('mainMail', 'ssl') ? '/ssl' : '' ) . 
						( $core->getConfig('mainMail', 'validate-cert') ? '' : '/novalidate-cert' ) . 
					'}', 
					$core->user->getLogin(), 
					$core->user->getPass(),
					OP_READONLY
			) ){
	
				$new_messages = imap_search($mbox, 'NEW');
				$messages_nb = (isset($new_messages) && is_array($new_messages) ) ? count($new_messages) : 0 ;

				$mail_alert = '';
				$new_ids = Array();
				
				for($i = $messages_nb - 1; $i >= 0; $i--){
					$overview = imap_fetch_overview($mbox, $new_messages[$i]);
					if(is_array($overview)){
						foreach($overview as $val){

							if($i >= $_SESSION['newmail']['nb']){
								$msg = '<strong>'.( isset($val->subject) ? imap_utf8($val->subject) : '[Pas de sujet]' ).'</strong> <em>'.( isset($val->from) ? imap_utf8($val->from) : '').'</em><br />';
								$_SESSION['newmail']['messages'][$val->uid] = $msg;
								$mail_alert .= '<a href="'.$core->makeUrl('horde/imp/message.php').'?index='.$val->uid.'">'.$msg.'</a>';
							}
								
							$new_ids[] = $val->uid;
							
						}
					}
				}
		
				$_SESSION['newmail']['nb'] = $messages_nb;

				//if user saw his new mails, removes them from session
				foreach($_SESSION['newmail']['messages'] as $id => $msg){
					if( !in_array($id, $new_ids) ){
						unset($_SESSION['newmail']['messages'][$id]);
					}
				}
		
			}

			if( $mail_alert != '' ){
				$box_content = '<div class="new_mail_first_div"><span class="new_msg_alert new_msg_number"><a href="'.$core->makeUrl('horde/imp/message.php').'"><span>'.$messages_nb.'</span> message(s) non lu(s).</a></span></div>';
				$box_content .= '<p>'.$mail_alert;
				$box_content .= '</p>';
				echo $box_content;
			}
			
		}//end if checkMail = true
		
	}


	public function displayNewMailsNb(){
		global $core;
		$core->loadClass('htmlblock');
		$hb = new HtmlBlock();
		
		if( $core->user->getMyPref('site_check_mail') == 'true'){
			
			$tooltip = '';
			if( !isset($_SESSION['newmail']) ){
				if( $mbox = @imap_open(
					'{'. 
						$core->getConfig('mainMail', 'host') . 
						':' . $core->getConfig('mainMail', 'port') . 
						'/'. $core->getConfig('mainMail', 'type') . 
						( $core->getConfig('mainMail', 'ssl') ? '/ssl' : '' ) . 
						( $core->getConfig('mainMail', 'validate-cert') ? '' : '/novalidate-cert' ) . 
					'}', 
					$core->user->getLogin(), 
					$core->user->getPass(),
					OP_READONLY
				) ){

					$new_messages = imap_search($mbox, 'NEW');
			   	$messages_nb = (isset($new_messages) && is_array($new_messages)) ? count($new_messages) : 0 ;

					$_SESSION['newmail'] = Array('nb' => $messages_nb, 'messages' => Array());
					
					for($i = $messages_nb - 1; $i >= 0; $i--){
						$overview = imap_fetch_overview($mbox, $new_messages[$i]);
						if(is_array($overview)){
							foreach($overview as $val){
								$tooltip .= $_SESSION['newmail']['messages'][$val->uid] = '<strong>'.( isset($val->subject) ? imap_utf8($val->subject) : '[Pas de sujet]' ).'</strong> <em>'.( isset($val->from) ? imap_utf8($val->from) : '').'</em><br />';
								
							}
						}
					}
					
				}else{
					echo '<p>Connexion au serveur momentanément impossible.</p>';
					$_SESSION['newmail'] = Array('nb' => 0, 'messages' => Array());
				}
			
			}else{
	
				end($_SESSION['newmail']['messages']);
				for( $i = 0; $i < $_SESSION['newmail']['nb']; $i++ ){
					$tooltip .= current($_SESSION['newmail']['messages']);
					prev($_SESSION['newmail']['messages']);
				}
			
			}
			

			
			$tooltip = $hb->escapeTipContent($tooltip);
			
			echo '<a href="'.$core->makeUrl('horde/').'" '.( $_SESSION['newmail']['nb'] != 0 ? 'onmouseover="activateToolTips(this, \''.$tooltip.'\');"' : '').' >';
			
			switch($_SESSION['newmail']['nb']){
				case 0:
					echo 'Aucun nouveau message';
					break;
					
				case 1:
					echo '1 nouveau message';
					break;
					
				default:
					echo $_SESSION['newmail']['nb'].' nouveaux messages';
					break;
			}

			echo '</a>';
	
		}
	}

}
?>
