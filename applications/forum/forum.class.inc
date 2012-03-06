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


class Forum {

 public $config = Array();
 
 public $modo;
 
 public $db_array = Array();
 
 private $db_local;
 
 //Liste des smileys
 public $smileys = Array (
		':-\)' => 'regular.png',
		';-\)' => 'wink.png',
		':-D' => 'megasmile.png',
		':lol:' => 'teeth.png',
		':-p'	=>	'tongue.png',
		':cry:' => 'cry.png',
		':-\(' => 'sad.png',
		':-\|' => 'blank_look.png',
		':sleepy:' => 'sleepy.png',
		':sorry:' => 'sorry.png',
		':sick:' => 'sick.png',
		':eyeroll:' => 'eyeroll.png',
		':omg:' => 'omg.png',
		':shade:' => 'shade.png',
		':sshh:' => 'sshh.png',
		':nerd:' => 'nerd.png',
		':confused:' => 'confused.png'
);

	public $more_smileys = Array (
	':airplane:' => 'airplane.png',
	':angel:' => 'angel.png',
	':angry:' => 'angry.png',
	':arrogant:' => 'arrogant.png',
	':asl:' => 'asl.png',
	':bat:' => 'bat.png',
	':bearingteeth:' => 'bearingteeth.png',
	':beer:' => 'beer.png',
	':boy_hug:' => 'boy_hug.png',
	':boy:' => 'boy.png',
	':brb:' => 'brb.png',
	':cake:' => 'cake.png',
	':camera:' => 'camera.png',
	':car:' => 'car.png',
	':cat:' => 'cat.png',
	':cigarette:' => 'cigarette.png',
	':clock:' => 'clock.png',
	':cocktail:' => 'cocktail.png',
	':coins:' => 'coins.png',
	':computer:' => 'computer.png',
	':cup:' => 'cup.png',
	':devil:' => 'devil.png',
	':diablo:' => 'diablo.png',
	':disgust:' => 'disgust.png',
	':dog:' => 'dog.png',
	':dunno:' => 'dunno.png',
	':email:' => 'email.png',
	':embarrassed:' => 'embarrassed.png',
	':envelope:' => 'envelope.png',
	':film:' => 'film.png',
	':girl_hug:' => 'girl_hug.png',
	':girl:' => 'girl.png',
	':handcuffs:' => 'handcuffs.png',
	':ip:' => 'ip.png',
	':kiss:' => 'kiss.png',
	':lightning:' => 'lightning.png',
	':love:' => 'love.png',
	':mail:' => 'mail.png',
	':messenger:' => 'messenger.png',
	':mobile_phone:' => 'mobile_phone.png',
	':moon:' => 'moon.png',
	':note:' => 'note.png',
	':palms:' => 'palms.png',
	':party:' => 'party.png',
	':phone:' => 'phone.png',
	':pizza:' => 'pizza.png',
	':plate:' => 'plate.png',
	':present:' => 'present.png',
	':rainbow:' => 'rainbow.png',
	':rose:' => 'rose.png',
	':sarcastic:' => 'sarcastic.png',
	':secret:' => 'secret.png',
	':sheep:' => 'sheep.png',
	':shout:' => 'shout.png',
	':sleepy2:' => 'sleepy2.png',
	':snail:' => 'snail.png',
	':soccer:' => 'soccer.png',
	':star:' => 'star.png',
	':storm:' => 'storm.png',
	':sun:' => 'sun.png',
	':thinking:' => 'thinking.png',
	':thumbs_down:' => 'thumbs_down.png',
	':thumbs_up:' => 'thumbs_up.png',
	':turtle:' => 'turtle.png',
	':ugly:' => 'ugly.png',
	':umbrella:' => 'umbrella.png',
	':unlove:' => 'unlove.png',
	':wilted_rose:' => 'wilted_rose.png'
	);
	
	private $same_smileys = Array(
		':-D' => 'megasmile.png',
		':-p'	=> 'tongue.png'
	);

   public final public function __construct($db_name_ext='',$db_host_ext='',$db_user_ext='',$db_pass_ext=''){
    global $db,$core;
		include('config_forum.inc');

		$this->config = $config;
    
    //Base dans laquele on touve les preference ( option pas finie )!
    
    $db_local = "";
    $db_pref = "";
    $db_name = ($db_name_ext!='')?$db_name_ext:$db_local;
    
    //Ici on utilise les done du config_secure mais on peut utiliser d'autres valeurs ecrites en dur !
    $this->db_array[$db_name]["base"] = ($db_name_ext!='')?$db_name_ext:$core->getConfig('database','base');
    $this->db_array[$db_name]["host"] = ($db_host_ext!='')?$db_host_ext:$core->getConfig('database','host');
    $this->db_array[$db_name]["user"] = ($db_user_ext!='')?$db_user_ext:$core->getConfig('database','user');
    $this->db_array[$db_name]["pass"] = ($db_pass_ext!='')?$db_pass_ext:$core->getConfig('database','pass');
    
    
    $db->connect($this->db_array[$db_name]["host"],$this->db_array[$db_name]["base"],$this->db_array[$db_name]["user"],$this->db_array[$db_name]["pass"]);
 

    if ($core->verifDroits('moderateur')) {
     $this->modo = true;
    }else{
     $this->modo = false;
    }
   }
 
/*
 *
 * Fonction getNbPageThread
 * calcule le nombre de page du thread
 *
 * @param $id_salon l'id du salon
 * @param $showModerate ??
 * @return le nombre de page du thread
 *
 */
	public function getNbPageSalon($id_salon, $showModerate = false){
		global $db, $session, $core;
		$id_salon = (int)$id_salon;
	
		$droit = $db->result($db->query("SELECT droit_lecture FROM forum_salon WHERE forum_salon.id='$id_salon' ") ,0);
	
		if(($droit == 'all') || $core->verifDroits($droit)){
			$showModerate = (bool)$showModerate;
			$query = "SELECT COUNT(id) FROM forum_thread WHERE id_salon=".$id_salon;
			if(!$showModerate) $query .= " AND 'drop'='false' ";
		
			$result = $db->query($query);
			return (int)(($db->result($result, 0) / $this->config['thread_per_page'])) +1;
		
		}else{
			return 1;
		}
	}

 //Nombre de nvx post du thread !
	public function getNbNvxPost($id) {
  global $db,$core;
  $id=intval($id);
  $query="SELECT COUNT(fm.id) FROM forum_message fm,forum_login fl WHERE fm.id_thread='$id' AND fl.thread=fm.id_thread AND fm.id > fl.id_last_msg AND fl.login='".$core->getUserId()."'";
  $result=$db->query($query);
  return $db->result($result,0);
 }

 public function isVisited($id) {
  global $db,$core;
  $id=intval($id);
  $query="SELECT Count(thread) FROM forum_login WHERE login='".$core->getUserId()."' AND thread='$id'";
  $result=$db->query($query);
  return ($db->result($result,0)==1);
 }

	public function readAllMessages(){
		global $core, $db;

		$db->query("UPDATE forum_login fl SET fl.id_last_msg = (SELECT MAX(id) FROM forum_message fm WHERE fm.id_thread = fl.thread) WHERE fl.login='".$core->getUserId()."' ");
	}
	
	public function readAllSalonMessages($id_salon){
    global $core, $db;

	  $db->query("UPDATE forum_login fl SET fl.id_last_msg = (SELECT MAX(id) FROM forum_message fm WHERE fm.id_thread = fl.thread) WHERE fl.login='".$core->getUserId()."' AND fl.thread IN (SELECT id FROM forum_thread WHERE id_salon='".$id_salon."'	) ");
	}
					
	public function getLastReadMessage($topic_id){
		global $core, $db;
		
		$result_last = $db->query("SELECT fl.id_last_msg FROM forum_login fl INNER JOIN forum_message fm ON  fl.id_last_msg = fm.id WHERE fl.login='".$core->getUserId()."' AND fl.thread='".$topic_id."' AND fm.score='1' ");
    if( $db->numRows($result_last) == 1 ){
		  $last_read_msg = $db->result($result_last,0);
		}else{
		  $last_read_msg = $this->getLastMessage($topic_id);
		}

		return $last_read_msg;
	}

	public function getLastMessage($topic_id){
		global $core, $db;
		
		$last_msg = $db->result($db->query("SELECT MAX(id) FROM forum_message WHERE id_thread='".$topic_id."' AND score='1' "),0);

		return $last_msg;
	}

	public function getLastSalonMessageInfos($salon_id){
		global $core, $db;
		
		$last_msg = $db->fetchArray($db->query("SELECT surnom, date FROM forum_message
																				INNER JOIN forum_thread ON forum_message.id_thread = forum_thread.id
																				WHERE forum_thread.id_salon = '".$salon_id."' 
																				AND forum_message.id = (SELECT MAX(forum_message.id) FROM forum_message
																				INNER JOIN forum_thread ON forum_message.id_thread = forum_thread.id
																				WHERE forum_thread.id_salon = '".$salon_id."' 

																				
																				
																				
																				
																				)"));

		return $last_msg;
	}
	
	public function getLastThreadMessageInfos($thread_id){
		global $core, $db;
		
		$last_msg = $db->fetchArray($db->query("SELECT avatar, forum_message.login, surnom, date FROM forum_message
																				INNER JOIN forum_thread ON forum_message.id_thread = forum_thread.id
																				WHERE forum_message.id_thread = '".$thread_id."' 
																				AND forum_message.id = (SELECT MAX(forum_message.id) FROM forum_message
																				INNER JOIN forum_thread ON forum_message.id_thread = forum_thread.id
																				WHERE forum_message.id_thread = '".$thread_id."' 

																				
																				
																				
																				
																				)"));

		return $last_msg;
	}


	public function getLastUnreadMessage($topic_id){
		global $core, $db;
		
		$result = $db->query("SELECT id FROM forum_message WHERE id_thread='".$topic_id."' AND id > ( SELECT fl.id_last_msg FROM forum_login fl INNER JOIN forum_message fm ON  fl.id_last_msg = fm.id WHERE fl.login='".$core->getUserId()."' AND fl.thread='".$topic_id."' AND fm.score='1' )  AND score='1' ORDER BY id ASC LIMIT 1");
		
		if( $db->numRows($result) == 1 ){
			$last_unread_msg = $db->result($result, 0);
		}else{
			$last_unread_msg = false;
		}

		return $last_unread_msg;
	}

	public function getPreviousThread($id_thread, $id_salon){
		global $db;

		$result = $db->query("SELECT id FROM forum_thread WHERE date_dernier_poste >= (SELECT date_dernier_poste FROM forum_thread WHERE id = '".$id_thread."') AND id != '".$id_thread."' AND id_salon = '".$id_salon."' ORDER BY date_dernier_poste ASC LIMIT 1");
		if($db->numRows($result) == 1){
			$prev_thread = $db->result($result, 0);
			if(!$this->isReadable($prev_thread)){
				$prev_thread = false;
			}
		}else{
			$prev_thread = false;
		}
		
		return $prev_thread;
	}

	public function getNextThread($id_thread, $id_salon){
		global $db;
		$result = $db->query("SELECT id FROM forum_thread WHERE date_dernier_poste <= (SELECT date_dernier_poste FROM forum_thread WHERE id = '".$id_thread."') AND id != '".$id_thread."' AND id_salon = '".$id_salon."' ORDER BY date_dernier_poste DESC LIMIT 1");
		if($db->numRows($result) == 1){
			$next_thread = $db->result($result, 0);
			if(!$this->isReadable($next_thread)){
				$next_thread = false;
			}
		}else{
			$next_thread = false;
		}
		
		return $next_thread;
	}

	public function getPreviousSalon($id_salon){
		global $db;
	
		$result = $db->query("SELECT id FROM forum_salon WHERE nom <= (SELECT nom FROM forum_salon WHERE id = '".$id_salon."') AND id != '".$id_salon."' ORDER BY nom DESC LIMIT 1");
		if($db->numRows($result) == 1){
			$prev_salon = $db->result($result, 0);
			if(!$this->isReadableSalon($prev_salon)){
				$prev_salon = false;
			}
		}else{
			$prev_salon = false;
		}
		
		return $prev_salon;
	}

	public function getNextSalon($id_salon){
		global $db;
	
		$result = $db->query("SELECT  id FROM forum_salon WHERE nom >= (SELECT nom FROM forum_salon WHERE id = '".$id_salon."') AND  id != '".$id_salon."' ORDER BY nom ASC LIMIT 1 ");
		if($db->numRows($result) == 1){
			$next_salon = $db->result($result, 0);
			if(!$this->isReadableSalon($next_salon)){
				$next_salon = false;
			}
		}else{
			$next_salon = false;
		}
		
		return $next_salon;
	}


/*** WARNING : Don't use those 4 methods to display a standard forum page ***/
/*** (those informations are in tables concerned -> faster) ***/

	public function getSalonMsgNb($id_salon){
		global $db;
		$nb = $db->result($db->query("SELECT COUNT(id) FROM forum_message WHERE id_thread IN (SELECT id FROM forum_thread WHERE id_salon='".$id_salon."' ) "), 0);
		return $nb;
	}

	public function getThreadNb($id_salon){
		global $db;
		$nb = $db->result($db->query("SELECT COUNT(id) FROM forum_thread WHERE id_salon='".$id_salon."' "), 0);
		return $nb;
	}

	public function getMsgNb($id_thread){
		global $db;
		$nb = $db->result($db->query("SELECT COUNT(id) FROM forum_message WHERE id_thread='".$id_thread."' "), 0);
		return $nb;
	}
	
	public function userPostNb($user_id){
		global $db;
		$nb = $db->result($db->query("SELECT COUNT(id) FROM forum_message WHERE login='".$user_id."' "), 0);
		return $nb;
	}
	
/*** End of warning***/

	
	public function parseSujet ($texte,$longueur_max = '') {
		$longueur_max = ($longueur_max == '') ? $this->config['subject_max_length'] : $longueur_max;
  //Enlï¿½e tous les caractï¿½es html^Mre
  //$texte = html_entity_decode($texte,ENT_QUOTES);
  //$texte = htmlspecialchars($texte);
  //Vï¿½ifie que l'on ne dï¿½asse pas la taille maximum du s
  //Si on la dï¿½asse on rajoute 3 petits poi
  $texte=preg_replace("/(.{".($longueur_max-3)."}).+/","\\1...",$texte);
  //Autorise le langage forum :i: :b: :s: :u:
  $texte=preg_replace("/\[i\]([^\[]*)\[\/i\]/","<i>\\1</i>",$texte);
  $texte=preg_replace("/\[b\]([^\[]*)\[\/b\]/","<b>\\1</b>",$texte);
  $texte=preg_replace("/\[s\]([^\[]*)\[\/s\]/","<u>\\1</u>",$texte);
  return $texte;
 }

	public function isReadableSalon($id_salon) {
  global $db,$session,$core;
  if(!isset($id_salon)) {
   return false;
  }
  $id_salon=(int)$id_salon;
  $query="SELECT droit_lecture FROM forum_salon WHERE forum_salon.id='$id_salon' ";
  $result=$db->query($query);
  $droit=$db->result($result,0);
  return ($droit=='all') || $core->verifDroits($droit);
 }

	public function isModerateurSalon($id_salon) {
  global $db,$session,$core;
  if(!isset($id_salon)) {
   return false;
  }
  $id_salon=(int)$id_salon;
  $query="SELECT droit_moderation FROM forum_salon WHERE forum_salon.id='".$id_salon."'";
  $result=$db->query($query);
  $droit=$db->result($result,0);
  
	return ($droit=='all') || $core->verifDroits($droit);
 }
 
	public function chgtEtatThread($id,$etat){
    global $db;
    $id=intval($id);
    $set="";
    switch($etat)
        {
        case 'DROP' :
          $set="`drop`='true',sticky='false',locked='false'";
        break;
        case 'STICKY' :
          $set="`drop`='false',sticky='true',locked='false'";
        break;
        case 'LOCKED' :
          $set="`drop`='false',sticky='false',locked='true'";
        break;
        case 'STICKY_LOCKED':
          $set="`drop`='false',sticky='true',locked='true'";
        break;
        case 'NORMAL' :
          $set="`drop`='false',sticky='false',locked='false'";
        break;
        }
    $query ="UPDATE forum_thread SET ".$set." WHERE id='".$id."'";
    $db->query($query);
    }

	public function updateThread($id,$new_nom,$id_salon) {
  	global $db;
  	$id=intval($id);
  	$new_nom=htmlentities ($new_nom,ENT_QUOTES,"UTF-8");
  	$id_salon=intval($id_salon);
  	$query="UPDATE forum_thread SET nom_thread='$new_nom',id_salon='$id_salon' WHERE id='$id'";
  	$result=$db->query($query);
	}
 
	public function isModerateur($id_thread) {
  	global $db,$session,$core;
  	if(!isset($id_thread)) {
   		return false;
  	}
  	$id_thread=(int)$id_thread;
  	$query="SELECT forum_salon.droit_moderation FROM forum_salon,forum_thread WHERE forum_thread.id='$id_thread' AND forum_thread.id_salon=forum_salon.id";
  	$result=$db->query($query);
  	$droit=$db->result($result,0);
  	return ($droit=='all') || $core->verifDroits($droit);
 }

 public function editMessage($post) {
  global $core, $db;

  $id=intval($post['id_message']);
  $surnom=htmlentities ($post['surnom'],ENT_QUOTES,"UTF-8");
  $email=isset($post['email']) ? htmlentities ($post['email'],ENT_QUOTES,"UTF-8") : '';
  $com=htmlentities ($post['com'],ENT_QUOTES,"UTF-8");
  $new_sujet=htmlentities ($post['sujet'],ENT_QUOTES,"UTF-8");
  $new_message=htmlentities ($post['message'],ENT_QUOTES,"UTF-8");
	$score = ($post['censure'] == 'true') ? '-1' : '1';
	
	//systeme de censure de l'avatar
	if(isset($post['avatar'])){
		$avatar = $post['avatar'];
		$core->user->setPref($post['poster_login'],'forum_modoavatar',$avatar);
	}

  $message = $db->fetchArray($db->query("SELECT surnom, email, sujet, message FROM forum_message WHERE id='".$id."' "));
	
  $query = "INSERT INTO forum_modo (id_message, sujet, message, surnom, email, com, login_modo) VALUES ( '".$id."', '".$message['sujet']."', '".$message['message']."', '".$message['surnom']."', '".$message['email']."', '".$com."', '".$core->getUserId()."' ) ";
  $db->query($query);
	
  $query = "UPDATE forum_message SET sujet='$new_sujet',message='$new_message', surnom='$surnom', email='$email', score='$score' WHERE id='$id'";
  $db->query($query);
	
	}

	public function quoteMessage($id_message){
		global $db, $core;
		$msg = $db->fetchArray($db->query("SELECT id_thread, surnom, message FROM forum_message WHERE id='".$id_message."' "));
		
		$message = '[citer='.stripslashes($msg['surnom']).']'.stripslashes($msg['message']).'[/citer]';
		
		$result = $db->query("SELECT id, message FROM forum_draft WHERE id_user='".$core->getUserId()."' AND id_thread='".$msg['id_thread']."' ");
		
		if($db->numRows($result) != 0){
			$draft = $db->fetchArray($result);
			$message = stripslashes($draft['message']).$message;
			
			$db->query("UPDATE forum_draft SET message='".$db->escapeString($message)."' WHERE id_user='".$core->getUserId()."' AND id_thread='".$msg['id_thread']."' ");
		}else{
			$db->query("INSERT INTO forum_draft (id_user, id_thread, subject, message) VALUES ( '".$core->getUserId()."', '".$msg['id_thread']."', '', '".$db->escapeString($msg['message'])."' )");
		}
	
		echo html_entity_decode($message, ENT_QUOTES, 'UTF-8');
	}

	public function saveDraft($post){
		global $core, $db;
	
		$result = $db->query("SELECT id FROM forum_draft WHERE id_user='".$core->getUserId()."' AND id_thread='".$post['id_thread']."' ");
		
		if($db->numRows($result) != 0){
						
			$db->query("UPDATE forum_draft SET subject='".$db->escapeString($post['sujet'])."', message='".$db->escapeString($post['message'])."' WHERE id_user='".$core->getUserId()."' AND id_thread='".$post['id_thread']."' ");
		}else{
			$db->query("INSERT INTO forum_draft (id_user, id_thread, subject, message) VALUES ( '".$core->getUserId()."', '".$post['id_thread']."', '".$db->escapeString($post['sujet'])."', '".$db->escapeString($post['message'])."' )");
		}
	
	}




	public function getDraft($id_thread){
		global $db, $core;
		return $db->fetchArray($db->query("SELECT subject, message FROM forum_draft WHERE id_user='".$core->getUserId()."' AND id_thread='".$id_thread."' "));
	}
	
 public function getPage($id_message, $showModerate = false) {
  global $db, $core;
  $id_message=intval($id_message);
  $showModerate=(bool)$showModerate;
	
  //Rï¿½upï¿½e le nombre de mes
  $query = " SELECT count(fm1.id) FROM forum_message fm1, forum_message fm2 WHERE fm1.id_thread=fm2.id_thread AND fm2.id='".$id_message."' AND fm1.id < '".$id_message."' ";
  if (!$showModerate) {
   $query .= " AND fm1.score > 0 ";
  }
  $result = $db->query($query);
  $nb_message = $db->result($result,0);
  //Recherche de la page de l'id ï¿½affich
  return (int)( $nb_message / $this->config['msg_per_page'])+1;
 }

 public function isWriteableSalon($id_salon) {
  global $db,$session, $core;
  if(!isset($id_salon) || $core->verifDroits('ban_forum')) {
   return false;
  }
  $id_salon=(int)$id_salon;
  $query="SELECT droit_ecriture FROM forum_salon WHERE forum_salon.id='$id_salon' ";
  $result=$db->query($query);
  $droit=$db->result($result,0);
  return ($droit=='all') || $core->verifDroits($droit);
 }

	public function newThread($id_salon,$nomThread,$createur){
  	global $db,$core;
    $nomThread=htmlentities ($nomThread,ENT_QUOTES,"UTF-8");
    $createur=htmlentities ($createur,ENT_QUOTES,"UTF-8");
    $id_salon=(int)$id_salon; 
    $db->query("INSERT INTO forum_thread (id,login,nom_thread,createur,date_creation,id_salon, nb_msg) VALUES ('','".$core->getUserId()."','$nomThread','$createur',now(),$id_salon, 0)");
		$id_thread = $db->insertId();
		$db->query("UPDATE forum_salon SET nb_thread = nb_thread + 1 WHERE id='".$id_salon."' ");
    return $id_thread;
	}

  public function isAttachement($id_thread) {
    global $db,$core;
    if(!isset($id_thread)) {
     return false;
    }
    $id_thread = (int)$id_thread;
    $result = $db->query("SELECT forum_salon.droit_attachement FROM forum_salon, forum_thread WHERE forum_thread.id='$id_thread' AND forum_thread.id_salon=forum_salon.id");
    $right = $db->result($result, 0);
    return ($right == 'all') || $core->verifDroits($right);
   }
   
 //entrée des infos d'un nouveau message  
 public function newMessage($id_thread,$id_salon, $sujet,$message,$signature,$surnom,$uniqid,$email,$avatar,$myavatar,$fm_id){
    global $db,$core;
		$core->loadClass('fileManager');
		$fm = unserialize($_SESSION['file_manager_'.$fm_id]);

		$file_ids = $fm->saveOrGet('', 'log', 'log');
		
    $id_thread=intval($id_thread);
    //Converti les données rentrées en ht
    $sujet = htmlentities ($sujet,ENT_QUOTES,"UTF-8");
    $message = htmlentities ($message,ENT_QUOTES,"UTF-8");
    $surnom = htmlentities ($surnom,ENT_QUOTES,"UTF-8");
    $uniqid = htmlentities ($uniqid,ENT_QUOTES,"UTF-8");
  	$avatar = htmlentities ($avatar,ENT_QUOTES,"UTF-8");
    $myavatar = htmlentities ($myavatar,ENT_QUOTES,"UTF-8");
		$file = isset($file_ids['forum_file']) ? "'".$file_ids['forum_file']."'" : 'NULL' ;
    $email = htmlentities($email,ENT_QUOTES,"UTF-8");
    
    //Vérifie que l'id n'est pas déjà rentré dans la 
    $query="SELECT count(uniqid) FROM forum_message WHERE uniqid='$uniqid' AND id_thread='$id_thread'";
    $result=$db->query($query);
    if ($db->result($result,0)==0)
        {
        //Ajoute le message
        //$nb_ligne=$this->getNbreLigne($message);
        //La ligne juste au desu elle est trop trop conne car elle sert a rien ! Mais bon des foois fo pas chercher
        $query="INSERT INTO forum_message (id_thread,login,sujet,message,surnom,date,score,host,uniqid,email,avatar,signature,myavatar,file) VALUES ('".$id_thread."','".$core->getUserId()."','".$sujet."','".$message."','".$surnom."',NOW(),'".$this->config['score']."','".gethostbyaddr($_SERVER['REMOTE_ADDR'])."','".$uniqid."','".$email."','".$core->user->getAvatar()."','".$signature."','".$myavatar."',".$file.")";
        $db->query($query);
        $ret=$db->insertId();
        //Actualise les infos du thread
        $db->query("UPDATE forum_thread SET dernier_login = ".$core->getUserId().", dernier_poste = '".$surnom."', date_dernier_poste = NOW(), nb_msg = nb_msg + 1  WHERE id='".$id_thread."'");
        //Actualise les infos du salon
        $db->query("UPDATE forum_salon SET nb_msg = nb_msg + 1  WHERE id='".$id_salon."'");
				//delete draft associated to the thread
				$db->query("DELETE FROM forum_draft WHERE id_user='".$core->getUserId()."' AND id_thread='".$id_thread."' ");
    
        //$this->sendMail($ret);
        return $ret;
        }
    else
        {
        return -1;
        }
    }

	public function parseSurnom ($texte,$longueur_max = '') {
		$longueur_max = ($longueur_max == '') ? $this->config['nick_max_length'] : $longueur_max ;
  	//Enl#e tous les caract#es html
  	//$texte = html_entity_decode($texte,ENT_QUOTES);
  	//$texte = htmlspecialchars($texte);
  	$texte=preg_replace("/([^ ]{".$longueur_max."})/","\\1\n\\2",$texte);
  	return $texte;
	}

	public function parseMessage($text, $max_word_length = ''){
		global $core;
		//$core->loadClass('bbcode');
		include_once(dirname(__FILE__).'/../../include/engine/page/form/bbCode.class.inc');
		$bb = new BBCode();

		$max_word_length = ($max_word_length == '') ? $this->config['msg_max_length'] : $max_word_length;
		
		$text = $bb->parse($text);
		
  	//Smileys
		$smileys = array_merge($this->smileys, $this->more_smileys, $this->same_smileys);
		
  	foreach(array_keys($smileys) as $smiley_name) {
   		$text = preg_replace('/'.$smiley_name.'/','<img src="'.$core->makeUrl('/skins/'.$core->user->getPref($core->getUserId(),'site_skin').'/images/parties/forum/smileys/'.$smileys[$smiley_name]).'" alt="'.str_replace('\\','',$smiley_name).'" />',$text);
  	}

		return $text;
		
	}

	//kept for the moment
	public function parseMessageOld($texte,$longueur_max = '') {
  	global $core,$db;
  	$core->loadClass('string');
		
		$longueur_max = ($longueur_max == '') ? $this->config['msg_max_length'] : $longueur_max;
		
  	//Enlève tous les caractères ht
  	//$texte = html_entity_decode($texte,ENT_QUOTES,"UTF-8");
  	//$texte = htmlspecialchars($texte);
  	$texte=preg_replace("/([^ ]{".$longueur_max."})/","\\1\n\\2",$texte);
  	//Autorise le langage forum
  	$texte=preg_replace("/\[i\]([^\[]*)\[\/i\]/","<i>\\1</i>",$texte);
  	$texte=preg_replace("/\[b\]([^\[]*)\[\/b\]/","<b>\\1</b>",$texte);
  	$texte=preg_replace("/\[g\]([^\[]*)\[\/g\]/","<b>\\1</b>",$texte);
  	$texte=preg_replace("/\[s\]([^\[]*)\[\/s\]/","<u>\\1</u>",$texte);
  	$texte=preg_replace("/\[u\]([^\[]*)\[\/u\]/","<u>\\1</u>",$texte);
      
	
 		$texte=preg_replace("/((http:|www)[^ \[\]\n\r]*)/","[url]http://\\1[/url]",$texte);
  	$texte=preg_replace("/("
  	."[^\]= \[\n\r\@]+"
  	."@"
  	."[^ \]\[\r\n\@]+"
  	."\."
  	."[^ \]\[\r\n\@]{2,3}"
  	.")/","[mail]\\1[/mail]",$texte);
  	//Vire le double http://
  	$texte=preg_replace("/(http:\/\/http:\/\/)/","http://",$texte);
	
  	//Vire les doubles url, doubles mail, doubles citations ou double sigantures
  	$texte=preg_replace("/\[url=\[url\]([^\[]*)\[\/url\]\]/","[url=\\1]",$texte);
  	$texte=preg_replace("/\[url\]\[url\]([^\[]*)\[\/url\]/","[url]\\1",$texte);
  	$texte=preg_replace("/\[mail=\[mail\]([^\[]*)\[\/mail\]\]/","[mail=\\1]",$texte);
  	$texte=preg_replace("/\[mail\]\[mail\]([^\[]*)\[\/mail\]/","[mail]\\1",$texte);
  	$texte=preg_replace("/\[citer=\[citer\]([^\[]*)\[\/citer\]\]/","[citer=\\1]",$texte);
  	$texte=preg_replace("/\[quote=\[quote\]([^\[]*)\[\/quote\]\]/","[quote=\\1]",$texte);
  	$texte=preg_replace("/\[signer\]\[signer\]([^\[]*)\[\/signer\]/","[signer]\\1",$texte);
  	$texte=preg_replace("/\[sign\]\[sign\]([^\[]*)\[\/sign\]/","[sign]\\1",$texte);
	
  	//Affiche les emails, les urls, les citations et la signature
  	$texte=preg_replace("/\[url=([^\]]*)\](.*?)\[\/url\]/",
  	"<a class='forum_message' target=\"_blanc\" href='\\1'>\\2</a>",$texte);
  	$texte=preg_replace("/\[url\](.*?)\[\/url\]/",
  	"<a class='forum_message' target=\"_blanc\"  href='\\1'>\\1</a>",$texte);
  	$texte=preg_replace("/\[mail=([^\]]*)\](.*?)\[\/mail\]/",
  	"<a class='forum_message' href='mailto:\\1'>\\2</a>",$texte);
  	$texte=preg_replace("/\[mail\](.*?)\[\/mail\]/",
  	"<a class='forum_message' href='mailto:\\1'>\\1</a>",$texte);
  	$texte=preg_replace("/\[img\](.*?)\[\/img\]/",
  	"<img src='\\1' />",$texte);
  	$texte=preg_replace("/\[img=([^\]]*)\](.*?)\[\/img\]/",
  	"<img alt='\\1' src='\\2' />",$texte);

		$texte = $this->parseQuotes($texte);
/*	
 		$texte=preg_replace("/\[signer\]([^\[]*)\[\/signer\]/","<br /><br /><div class=\"signature\">--------------------------------------<br />\\1</div>",$texte);  */

		$smileys = array_merge($this->smileys, $this->more_smileys, $this->same_smileys);
  	//Smileys
  	foreach(array_keys($smileys) as $nom_smiley) {
   		$texte=preg_replace('/'.$nom_smiley.'/','<img src="'.$core->makeUrl('/skins/'.$core->user->getPref($core->getUserId(),'site_skin').'/images/parties/forum/smileys/'.$smileys[$nom_smiley]).'" alt="'.str_replace('\\','',$nom_smiley).'" />',$texte);
  	}
  	//Efface tous les caractères 'forum
  	$texte=preg_replace("/:[a-z A-Z]*:/","",$texte);
  	//Met en forme les espaces
  	/*if (preg_match("/   /",$texte)) {
  	$texte="<pre>".$texte."</pre>";
  	} else {
  	*/
  
  	$string=new String();
  	$texte = $string->wordwrap3($texte,$longueur_max);
  	$texte=nl2br($texte);
  	//}
  	return $texte;
	}

	//kept for the moment
	private function parseQuotes($text, $style = true){
		//colors are changed depending on the "depth" of the quotation
		$class = ($style) ? '1' : '2';

		
		//$nb = substr_count($text, "[citer") -1;

		//we attempt to select all pairs of "citer" tags
		//preg_match_all("/(?:(?:\[citer\])|(?:\[citer=([^\]]*)\]))(.*)(?:\[\/citer\])/", $text, $quotes);
		preg_match_all("/(?:(?:\[citer\])|(?:\[citer=([^\]]*)\]))(.*)(?:\[\/citer\](?!(?:.*\[\/citer\].*)|(?:.*\[citer\].*)|(?:\[citer=([^\]]*)\])))/", $text, $quotes);
		
		$l = count($quotes[0]);

		//for each "citer" block
		for($i=0; $i< $l; $i++){
			//if the quotation has an author
			if($quotes[1][$i] != ''){
	  		$text=str_replace($quotes[0][$i],
		  		'<div class="bb_quote quote'.$class.'"><p class="bb_quote_author">'.$quotes[1][$i].' '.locale::display('said','forum').' :</p><div class="bb_quote_text">'.$this->parseQuotes($quotes[2][$i], !$style).'</div></div>',$text);
			}else{
		  	$text=str_replace($quotes[0][$i],
				'<div class="bb_quote quote'.$class.'"><p class="bb_quote_author">Citation :</p><div class="bb_quote_text">'.$this->parseQuotes($quotes[2][$i], !$style).'</div></div>',$text);
			}
		
		}

		preg_match_all("/(?:(?:\[quote\])|(?:\[quote=([^\]]*)\]))(.*)(?:\[\/quote\](?!(?:.*\[\/quote\].*)|(?:.*\[quote\].*)|(?:\[quote=([^\]]*)\])))/", $text, $quotes);
		
		$l = count($quotes[0]);

		//for each "citer" block
		for($i=0; $i< $l; $i++){
			//if the quotation has an author
			if($quotes[1][$i] != ''){
	  		$text=str_replace($quotes[0][$i],
		  		'<div class="bb_quote quote'.$class.'"><p class="bb_quote_author">'.$quotes[1][$i].' '.locale::display('said','forum').' :</p><div class="bb_quote_text">'.$this->parseQuotes($quotes[2][$i], !$style).'</div></div>',$text);
			}else{
		  	$text=str_replace($quotes[0][$i],
				'<div class="bb_quote quote'.$class.'"><p class="bb_quote_author">Citation :</p><div class="bb_quote_text">'.$this->parseQuotes($quotes[2][$i], !$style).'</div></div>',$text);
			}
		
		}

		return $text;
	}

	public function readThread($id,$id_last_message_read) {
  global $db,$core;
  $id=intval($id);
  $id_last_message_read=intval($id_last_message_read);
  if ($core->session->getAuthOk()) {
   //Regarde si la personne a déjà consulté ce thread
   $query="SELECT count(login) FROM forum_login WHERE login='".$core->getUserId()."' AND thread='$id'";
   $result=$db->query($query);
   if ($db->result($result,0)>0) {
    //Récupère l'ancien dernier message 
    $query="SELECT id_last_msg FROM forum_login WHERE login='".$core->getUserId()."' AND thread='$id'";
    $old_id=$db->result($db->query($query),0);
    if ($id_last_message_read> $old_id) {
     //La personne a déjà consulté le thread
     $query= "UPDATE forum_login SET id_last_msg='$id_last_message_read' WHERE login='".$core->getUserId()."' AND thread='$id'";
     $db->query($query);
    }
   } else {
    //La personne n'a pas déjà consulté le thread
    $query="INSERT INTO forum_login (login,thread,id_last_msg) VALUES ('".$core->getUserId()."','$id','$id_last_message_read')";
    $db->query($query);
   }
  }
 } 
 
 public function isReadable($id_thread) {
  global $db,$session,$core;
  if(!isset($id_thread)) {
   return false;
  }
  $id_thread=(int)$id_thread;
  $query="SELECT forum_salon.droit_lecture FROM forum_salon,forum_thread WHERE forum_thread.id='$id_thread' AND forum_thread.id_salon=forum_salon.id";
  $result=$db->query($query);
  $droit=$db->result($result,0);
  return ($droit=='all') || $core->verifDroits($droit);
 }
 
 public function isWriteable($id_thread) {
  global $db,$session,$core;
  if(!isset($id_thread)) {
   return false;
  }
  $id_thread=(int)$id_thread;
  $query="SELECT forum_salon.droit_ecriture FROM forum_salon,forum_thread WHERE forum_thread.id='$id_thread' AND forum_thread.id_salon=forum_salon.id";
  $result=$db->query($query);
  $droit=$db->result($result,0);
  return ($droit=='all') || $core->verifDroits($droit);
 }


 public function newSalon($nom,$descriptif,$droit_lecture,$droit_ecriture,$droit_moderation,$droit_attachement,$uniqid) {
	global $db;
	$nom=htmlentities($nom,ENT_QUOTES,"UTF-8");
	$descriptif=htmlentities($descriptif,ENT_QUOTES,"UTF-8");
	$droit_lecture=htmlentities($droit_lecture,ENT_QUOTES,"UTF-8");
	$droit_ecriture=htmlentities($droit_ecriture,ENT_QUOTES,"UTF-8");
	$droit_moderation=htmlentities($droit_moderation,ENT_QUOTES,"UTF-8");
	$droit_attachement=htmlentities($droit_attachement,ENT_QUOTES,"UTF-8");
	$uniqid = $uniqid;
	
	$query = "SELECT id FROM forum_salon WHERE uniqid='".$uniqid."'";
	if ($db->numRows($db->query($query)) == 0) {
		
		$query = "INSERT INTO forum_salon ("
			."id, nom, descriptif, droit_lecture, droit_ecriture, droit_moderation, droit_attachement, uniqid , nb_msg, nb_thread"
			." ) VALUES ( "
			."'','".$nom."','".$descriptif."','".$droit_lecture."','".$droit_ecriture."','".$droit_moderation."','".$droit_attachement."', '".$uniqid."', 0, 0 "
			.")";

		$db->query($query);
		return $db->insertId();
	} else {
		return -1;
	}
  }

 public function set_pref($user_id,$pref_name,$value){
//ca peut paraitre con mais c'ets pour si jamais on fait diferament !
  global $db,$db_name,$db_pref,$core,$db_array,$db_local;
  if($db_name!=$db_local){
   $db->connect($db_array[$db_local]["host"],$db_array[$db_local]["base"],$db_array[$db_local]["user"],$db_array[$db_local]["pass"]);
  }
  $ret = $core->user->setPref($user_id,$pref_name,$value);
  if($db_name!=$db_local){
   $db->connect($db_array[$db_name]["host"],$db_array[$db_name]["base"],$db_array[$db_name]["user"],$db_array[$db_name]["pass"]);
  }
  return $ret;
 }

function get_pref($user_id,$pref_name){
//ca peut paraitre con mais c'ets pour si jamais on fait diferament !
  global $db,$db_name,$db_pref,$core,$db_array,$db_local;
  if($db_name!=$db_local){
   $db->connect($db_array[$db_local]["host"],$db_array[$db_local]["base"],$db_array[$db_local]["user"],$db_array[$db_local]["pass"]);
  }
  $ret = $core->user->getPref($user_id,$pref_name);
  if($db_name!=$db_local){
   $db->connect($db_array[$db_name]["host"],$db_array[$db_name]["base"],$db_array[$db_name]["user"],$db_array[$db_name]["pass"]);
  }
  return $ret;
 }


/***********************************************************************************************

Fonctions copiée de l'ancienne classe de forum
Ne gerent pas l'aspect multibase.

Mais en attendant ca permet au forum de fonctionner

*******************************************************************************************************/

/**
    * Défini le type de suivi
    * @param $id id du thread
    * @param $mail suivi par mail
    * @param $site suivi par le site
    */
    
  public function typeSuivi($id,$mail,$site)
      {
      global $db,$core;
      $id=intval($id);
      if ($mail==1)
        {
        $mail=true;
        }
      if ($mail==0)
        {
        $mail=false;
        }
      if ($site==1)
        {
        $site=true;
        }
      if ($site==0)
        {
        $site=false;
        }
      if ($this->isReadable($id))
          {
          $query="UPDATE forum_login SET
                  mail='".$mail."',
                  site='".$site."'
                  WHERE login='".$core->getUserId()."'
                  AND thread='$id'";
          $result=$db->query($query);
          }
      }

   /**
    * Défini le type de suivi
    * @param $id Id du thread
    */
    
  public function getSuivi($id)
      {
      global $db,$core;
      $id=intval($id);
      $query="SELECT mail,site
              FROM forum_login
              WHERE login='".$core->getUserId()."'
              AND thread='$id'";
      $result=$db->query($query);
      return $db->fetchArray($result);
      }
      
  /**
   * Fonction getSalonData récupère les données associées au thread courant
   * @param $id Id du salon à afficher
   * @return les données du salon
   */
  public function getSalonData ($id)
      {
      global $db;
      $id=(int)$id;
      $query="SELECT * FROM forum_salon WHERE id='$id'";
      $result=$db->query($query);
      return $db->fetchArray($result);
      }
      
  /**
   * Fonction updateSalon ajoute un salon au forum
   * @param $id_salon Id du salon à modifier
   * @param $nom Nom du salon
   * @param $descritpif Descriptif du salon
   * @param $droit_lecture Groupe ayant un doit en lecture
   * @param $droit_ecriture Groupe ayant un droit en écriture
   * @param $droit_moderation Groupe ayant un accès en modération
   * @return id du salon ajouté
   */
  public function updateSalon ($id_salon,$nom,$descriptif,$droit_lecture,$droit_ecriture,$droit_moderation,$droit_attachement)
      {
      global $db,$session;
      $id_salon=(int)$id_salon;
      $nom=htmlentities($nom,ENT_QUOTES,"UTF-8");
      $descriptif=htmlentities($descriptif,ENT_QUOTES,"UTF-8");
      $droit_lecture=htmlentities($droit_lecture,ENT_QUOTES,"UTF-8");
      $droit_ecriture=htmlentities($droit_ecriture,ENT_QUOTES,"UTF-8");
      $droit_moderation=htmlentities($droit_moderation,ENT_QUOTES,"UTF-8");
      $droit_attachement=htmlentities($droit_attachement,ENT_QUOTES,"UTF-8");
      if ($this->salonExist($id_salon))
          {
          $query="UPDATE forum_salon SET
                  nom='$nom',
                  descriptif='$descriptif',
                  droit_lecture='$droit_lecture',
                  droit_ecriture='$droit_ecriture',
                  droit_moderation='$droit_moderation',
                  droit_attachement='$droit_attachement' 
                  WHERE id='$id_salon'";
          $result=$db->query($query);
          }
      }
      
  /**
   * Fonction salonExist ajoute
   * @param $id_salon Id du salon
   * @return true si le salon existe
   */
  public function salonExist ($id_salon)
      {
      global $db;
      $id_salon=(int)$id_salon;
      $query="SELECT count(id) FROM forum_salon WHERE id='$id_salon'";
      $result=$db->query($query);
      return $db->result($result,0)>0;
      }

/**
* Fonction retournant les infos d'un thread
* @param $id id du thread
* @return tableau des infos de ce thread
*/
function getThreadData ($id)
{
  global $db;
  $id=intval($id);
  $query="SELECT * FROM forum_thread WHERE id='$id'";
  $result=$db->query($query);
  return $db->fetchArray($result);
}
function avatar($user_id, $censure = 0)
{
  global $core;
  $local = $core->getConfig('config','baseServer').'/user/photo_joke/'.$user_id.'.jpg';
  $localWeb = $core->getConfig('config','baseUrl').'/user/photo_joke/'.$user_id.'.jpg';
  //Afaire : si le fichier est sur un autre serveur un telechargement du fichier !
  $distant = $core->getConfig('config','avatar').'/'.$user_id.'.jpg';

  if(file_exists($local) && file_exists($distant))
  {
    //return '<img src="'.$localWeb.'" class="'.($censure ?'avatar_censure':'avatar').'" />';
    return $localWeb;
  }
  elseif(file_exists($distant))
  {
    copy($distant, $local);
    //return '<img src="'.$localWeb.'" class="'.($censure ?'avatar_censure':'avatar').'" />';
    return $localWeb;
  }
  elseif(!file_exists($distant) && file_exists($local))
  {
    unlink($local);
  }
}

/**
    * Retourne les id des threads suivi par le site
    * @return le tableau des id
    */
   public function getThreadSuivi() {
        global $db,$session,$core;
        //On limite l'affichage Ã  10 threads
        $query="SELECT id "
             ." FROM forum_login,forum_thread "
             ." WHERE forum_login.login='".$core->getUserId()."'"
             ."   AND forum_thread.id=thread "
             ."   AND site='true'"
             ." ORDER BY date_dernier_poste DESC"
             ." LIMIT 0,10";
        $result=$db->query($query);
        $ret=array();
        while ($row=$db->fetchArray($result)) {
           $ret[]=$row['id'];
        }
        return $ret;
   }


	public function removeFile($id_message){
		global $db;
		$db->query("UPDATE forum_message SET file=(NULL) WHERE id='".$id_message."' ");
	}
	
/**
	 * FONCTION DeleteSalon() ~ Ajout Sylv 26/10/2006
	 * Détruit le salon correspondant à l'id en argument, ainsi que l'ensemble des sujets & messages y étant contenus
	 * @return void
	 */

	 public function deleteSalon($id_salon) {

	  global $db;

		$db->query("DELETE FROM forum_abus WHERE id_message IN (SELECT id FROM forum_message WHERE id_thread IN (SELECT id FROM forum_thread WHERE id_salon='".$id_salon."' ) )");
		$db->query("DELETE FROM forum_message WHERE id_thread IN (SELECT id FROM forum_thread WHERE id_salon='".$id_salon."' ) ");
		$db->query("DELETE FROM forum_thread WHERE id_salon='".$id_salon."' ");
	  $db->query("DELETE FROM forum_salon WHERE id = '".$id_salon."'");
	 
	 }

/**
	 * FONCTION DeleteThread() ~ Ajout Sylv 26/10/2006
	 * Détruit le thread correspondant à l'id en argument, ainsi que l'ensemble des messages y étant contenus
	 * @return void
	 */

	 public function deleteThread($id_thread) {

	 	global $db;
		if($db->query("UPDATE forum_salon SET nb_thread = nb_thread - 1, nb_msg = nb_msg - (SELECT COUNT(id) FROM forum_message WHERE id_thread='".$id_thread."' ) WHERE id = (SELECT id_salon FROM forum_thread WHERE id='".$id_thread."' ) ")){
			$db->query("DELETE FROM forum_abus WHERE id_message IN (SELECT id FROM forum_message WHERE id_thread = '".$id_thread."' )");
			$db->query("DELETE FROM forum_message WHERE id_thread = '".$id_thread."' ");
			$db->query("DELETE FROM forum_thread WHERE id = '".$id_thread."'");
		}

	}

	/**
	  *	censureMessage()	: Permets de censurer/décensurer un message sur le forum
	  *
	  * @param : $MessageCible->Id du message à censurer
	  * @return void
		*
	  * @comm : appelée en AJAX plus haut
	  */
	public function censureMessage($message) {
		global $db, $core;
	
		$res = $db->query("SELECT score FROM forum_message WHERE id = '".$message."'");

		while ($row = $db->fetchArray($res)) {
			if ($row['score'] == 0 || $row['score'] == 1) {
				$db->query("UPDATE forum_message SET score = -1 WHERE id = '".$message."'");
			}else{
				$db->query("UPDATE forum_message SET score = 1 WHERE id = '".$message."'");
			}
		}

	}

	public function announceAbuse($post){
		global $core, $db;
		
		$db->query("INSERT INTO forum_abus (id_message, login, date, com, is_read) VALUES (".$post['msg_id'].", '".$core->getUserId()."', NOW(), '".$db->escapeString($post['comment'])."', 'false') ");
	}

	public function archiveAbuse($id_abuse){
		global $db;
		
		$db->query("UPDATE forum_abus SET is_read='true' WHERE id_abus='".$id_abuse."'");
	}

	public function displayThreadPages($thread_id, $db_name, $moderator = false){
		global $core, $db;
		$core->loadClass('htmlblock');
	  $hb = new HtmlBlock();
		
		$score = '';
		if(!$moderator){
			$score = " AND score > 0 ";
		}
		
		$nbPages = $db->result($db->query("SELECT COUNT(*) FROM forum_message WHERE id_thread='".$thread_id."' ".$score), 0);
		$hb->pagesList( (int)$nbPages, $this->config['msg_per_page'], 0, 'href', 'affiche_thread.php?id_thread='. $thread_id .'&amp;db_name='. $db_name .'&amp;page=[#]', 'Aller à la page : ');
	}
	
	public function displaySmileys($id){
		global $core;
		foreach($this->smileys as $code => $image){
		  echo '<a href="javascript:putSmiley(gE(\'textarea_message_'.$id.'\'),\''.$code.'\')"><img src="'.$core->makeUrl('skins/meetmind/images/parties/forum/smileys/'.$image).'" alt="'.stripslashes($code).'" /></a> ';
		}
	}

	public function displayMoreSmileys($id){
		global $core;
		foreach($this->more_smileys as $code => $image){
		  echo '<a href="javascript:putSmiley(gE(\'textarea_message_'.$id.'\'),\''.$code.'\')"><img src="'.$core->makeUrl('skins/meetmind/images/parties/forum/smileys/'.$image).'" alt="'.stripslashes($code).'" /></a> ';
		}							
	}
	
/*** FIN DE LA CLASSE FORUM.CLASS.INC ***/

}
