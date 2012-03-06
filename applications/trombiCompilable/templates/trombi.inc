<?php

include('detail.inc');

class Trombi {

	private $id;
	
	public final function __construct(){
		do{
			$this->id = rand(1000000, 9999999);
		}while( isset($_SESSION['trombi_'.$this->id]) );
		
		$_SESSION['trombi_'.$this->id] = serialize($this);
	}

	public function openCard($user_id){
		global $db;
		$locale = Locale::getLocale();
	
		if(is_array($user_id)){
			$infos = $user_id;
		}else{
			$infos = $db->fetchArray($db->query("SELECT * FROM trombi_trombi, usersInfo WHERE trombi_trombi.id_user=usersInfo.id AND usersInfo.id='".$user_id."' LIMIT 1"));
		}

		$content = '<p>'.addslashes(detail($infos)).'</p><p align="center" class=\"form-confirm\"><input type=\"button\" value=\"'.$locale->display('close', 'kernel').'\" onclick=\"dialBox.erase();\" /></p>';
		$content = strtr($content,"\n\r\t\0","    ");

		$title = addslashes(ucfirst($locale->display('trombi_card', 'trombi')).stripslashes(' : '.$infos['firstname'].' '.$infos['lastname']));
		$title = strtr($title,"\n\r\t\0","    ");
		
		return 'dialBox.draw(\''.htmlentities($content,ENT_QUOTES,'UTF-8').'\', \''.htmlentities($title,ENT_QUOTES,'UTF-8').'\', true);';
	}

	public function saveStatus($status){
		global $core, $db;

		$db->query("UPDATE trombi_trombi SET signal_social = '".htmlentities($status, ENT_QUOTES, 'UTF-8')."' WHERE id_user = '".$core->getUserId()."' ");
	}

	public function displayStatusInput(){
		global $core, $db;
		$locale = Locale::getLocale();
		
		$status = stripslashes($db->result($db->query("SELECT signal_social FROM trombi_trombi WHERE id_user ='".$core->getUserId()."' LIMIT 1"), 0));
		$status = isset($status) && $status != '' ? $status : $locale->display('click_to_change_status', 'trombi');
		
		echo '<p class="social_status" onmouseover="activateToolTips(this,\'Entrez ici ce qui vous passe par la tête : humeur du moment, projets de la journée ou localisation par exemple.\');" onclick="hide(toolTips);editAndSend(this, \''.$core->makeUrl('trombi/ajax/social_status.php').'?id_trombi='.$this->id.'&geniutt_action=save_status\', \'social_status\', false, 51, 50);">'.$status.'</p>';	
	}

	public function displayPhoto($user_id = 'me'){
		global $core, $db;
		$core->loadClass('filemanager');
		$fm = new FileManager();
		
		$user_id = ( $user_id == 'me' ) ? $core->getUserId() : $user_id;
		$result = $db->query("SELECT photo FROM trombi_trombi WHERE id_user = '".$user_id."' LIMIT 1");
		if( isset($result) ){
			$photo = $db->result($result, 0);
		}
		
		$photo = ( isset($photo) && $photo != 0 ) ? $fm->preview($photo, 1, true) : '';
		return $photo;
	}
	
	public function displayUserPhotos($user_id = 'me') {
		global $core;
		$user_id = ( $user_id == 'me' ) ? $core->getUserId() : $user_id;
		
		$photos_button = '<a class="photos_button" href="'.$core->makeUrl('photo_album/index.php').'?ga=dUP&ud='.$user_id.'">Voir ses photos</a>';

		return $photos_button;
	}
	
	public function displayVisitorsOf($user_id = 'me'){
		global $core, $db;
		$core->loadClass('filemanager');
		$core->loadClass('date');
		$core->loadClass('link');
		$core->loadClass('htmlblock');
		$link = new Link();
		$fm = new FileManager();
		$hb = new HtmlBlock();

		$user_id = ( $user_id == 'me' ) ? $core->getUserId() : $user_id;
    $result = $db->query("
			SELECT tv.visits_count, DATE(tv.last_visit) AS last_visit_date, CONCAT(HOUR(tv.last_visit), 'h', MINUTE(tv.last_visit)) AS last_visit_time, tt.* 
			FROM trombi_visit tv INNER JOIN trombi_trombi tt ON tv.id_user = tt.id_user 
			WHERE tv.id_card = (SELECT id FROM trombi_trombi WHERE id_user = '".$user_id."' LIMIT 1) ORDER BY tv.last_visit DESC LIMIT 20
		");
		
		echo '<div class="user_list">';
		while( $user = $db->fetchArray($result) ){
			$strName = stripslashes(ucwords(strtolower($user['firstname'].' '.$user['lastname'])));
			$date = new Date($user['last_visit_date']);
		
			//preloads photo
		  $photo = $fm->preview($user['photo'], 3, true);
			echo '<span style="display: none;">'.$photo.'</span>';
			
			echo '
			<div class="user_row">
				<div class="user_photo">
					<div class="picture_block"><a class="avatar" onclick="dialBox.draw(\''.
						$hb->escapeTipContent('<div class="close_avatar_prev"><a class="link_button" onclick="dialBox.erase()">Fermer</a></div><div>'.$photo.'</div>').'\', \''.$strName.'\', true);" >'.
						'<span></span>'.$fm->preview($user['photo'], 1, true).'
					</a></div>
				</div>
				<div class="row_up">
					<h3>'.$strName.'</h3>
					<div class="mail_link">'.$link->mailto(stripslashes($user['email']), 'Le contacter', 'mini_mail').'</div>
					<div class="card_link"><a class="mini_trombi" href="javascript:'.$this->openCard($user).';void(0);">Voir sa fiche</a></div>
				</div>
				<div class="clear"></div>
				<div class="row_bottom">
					<div class="card_visits">'.$user['visits_count'].' consultation'.($user['visits_count'] != 1 ? 's' : '').' (dernière le : '.$date->display('/').' à '.$user['last_visit_time'].')</div>
				</div>
			</div>';
		}
		echo '</div>';
		
	}
	
}

?>
