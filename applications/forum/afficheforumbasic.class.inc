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


/*
    génération de l'html de la page forum en fonction du style (comme un template)

*/


class afficheforumBasic {

	private $id;

	private $is_trombi;

	public $forum;
	
  /*
   *
   * Fonction afficheforumBasic
   * Constructeur de la class afficheforumBasic pour l'instancier lors de l'appel
   *
   * @return void
   *
   */

	public final function __construct(){
		global $core;

		$core->localLoadClass('forum');
		$this->forum = new Forum();
		$this->id = rand(1000000,9999999);
		$this->is_trombi = $core->isTrombi();
		$_SESSION['affiche_forum_'.$this->id] = serialize($this);
	}



  /*
   *
   * Fonction salon
   * Genère un salon
   *
   * @param $alt
   * @param $db_name
   * @param $salon
   * @param $last_sujets
   * @return void
   *
   */

	public function salon($alt, $db_name, $salon, $last_sujets, $last_poster, $class){
		global $core, $db, $page;
		$locale = Locale::getLocale();
		$core->loadClass('date');
		$date = new Date();

		if($this->is_trombi){
			$core->loadClass('trombi');
			$trombi = new Trombi();
		}

		$is_new_m = false;
		$new_m_result = $db->query("SELECT COUNT(*)
			FROM 
				(	forum_message fm
    			INNER JOIN forum_login fl 
		   			ON fm.id_thread = fl.thread
				)
				INNER JOIN forum_thread ft
				  ON fm.id_thread = ft.id
				WHERE ft.id_salon = '".$salon['id']."'
					AND fm.id > fl.id_last_msg
					AND fl.login = '".$core->getUserId()."'
		");
		
		if( $db->result($new_m_result, 0) == 0 ){
			$new_t_result = $db->query("SELECT COUNT(*)
				FROM forum_login fl2
				  INNER JOIN forum_thread ft2
					ON fl2.thread = ft2.id
				WHERE fl2.login = '".$core->getUserId()."'
					AND ft2.id_salon = '".$salon['id']."'
			");

			if( $db->result($new_t_result, 0) != $salon['nb_thread'] ){
				$is_new_m = true;
			}
		}else{
			$is_new_m = true;
		}
		
		if($is_new_m){
			$new_messages = 'new_messages';
		}else{
			$new_messages = '';
		}
		
		echo '<div class="salon '.$class.'">
						<div class="actions_menu left">
							<ul>
								<li><a class="add_new" href="new_thread.php?db_name='.$db_name.'&id_salon='.$salon['id'].'"></a></li>
								<li><a class="see_most_crowded '.$new_messages.' " onclick="'.
							'if(window.getComputedStyle(gE(\'last_active_topics_list_'.$salon['id'].'\'), \'\').display == \'none\'){'.
								'addClassName(this.parentNode, \'displayed\');'.
								'show(gE(\'last_active_topics_list_'.$salon['id'].'\'));'.
							'}else{'.
								'removeClassName(this.parentNode, \'displayed\');'.
						  	'hide(gE(\'last_active_topics_list_'.$salon['id'].'\'));'.
						'}"></a></li>
							</ul>
						</div>
						<div class="thread_title_block left">
            <p class="thread_title"><a href="thread.php?id_salon='. $salon['id'] .'&amp;db_name='. $db_name .'">'. $salon['nom'] .'</a>'.
					
				'<p class="thread_description">'.nl2br($salon['descriptif']).'</p>
			</div>';

		echo '<div class="salon_last_msg right" >';

		$tooltip = $page->htmlBlock->escapeTipContent('<div class="details"><div class="pseudo_auteur">'.$last_poster['pseudo'].'</div><div class="date_publication">le '.$date->formatDate($last_poster['date'],"d/m/y").' à '.$date->formatDate($last_poster['date'],"H\hi").'</div></div>');

		echo '<p>'.$date->formatDate($last_poster['date'], 'd/m/y').' - '.$date->formatDate($last_poster['date'], 'H\hi').'</p>';
		echo '<p class="last_msg_author_block"><a class="last_msg_author" onmouseover="activateToolTips(this,\'';
		
		if($this->is_trombi && $this->forum->get_pref($last_poster['login'], 'forum_myavatar') == 'true' ){
			echo $page->htmlBlock->escapeTipContent('<div class="avatar_lmessage">'.
			$trombi->displayPhoto($last_poster['login']).
			'</div>');
		}
		
		echo $tooltip;
		echo '\');" onclick="';
		
		if($this->is_trombi){
			echo 'hide(toolTips);'.$trombi->openCard($last_poster['login']).';';
		}
		
		echo '" >'.$last_poster['pseudo'].'</a></p>';
		echo '</div>';
			
		echo '<div class="salon_msg_nb right" onmouseover="activateToolTips(this,\'Nombre de messages postés dans ce salon\');">';
		echo $salon['nb_msg'];
		echo '</div>';
		
		echo '<div class="thread_nb right" onmouseover="activateToolTips(this,\'Nombre de topics créés dans ce salon\');">';
		echo $salon['nb_thread'];
		echo '</div>';

		if ($this->forum->modo) {
			
			echo '<div class="actions_menu right">
							<ul>
								<li><a class="admin_topic" href="edit_salon.php?id_salon='. $salon['id'] .'&amp;db_name='. $db_name .'" onmouseover="activateToolTips(this, \''.$locale->display('edit_forum','forum').'\')"></a></li>
								<li><a class="admin_abuses" href="abuses.php?id_salon='. $salon['id'] .'&amp;db_name='. $db_name .'" class="abuse" onmouseover="activateToolTips(this, \''.$locale->display('see_abuse_msg','forum').'\')" ></a></li>
							</ul>
						</div>';
		}
	
		echo '<div class="clear"></div>
            <ul class="threads_list" id="last_active_topics_list_'.$salon['id'].'" style="display: none;">';
						
		while($last_sujet = $db->fetchArray($last_sujets )){
		
			$last_unread_msg = $this->forum->getLastUnreadMessage($last_sujet['id']);
  			$last_msg = $last_unread_msg ? $last_unread_msg : $this->forum->getLastMessage($last_sujet['id']);
			
			if(!$this->forum->isVisited($last_sujet['id']) && $core->session->isAuthOk()){
				$row_class = "new_msg";
			}else{
				$row_class = $this->forum->getNbNvxPost($last_sujet['id']) > 0 ? "new_msg" : "nonew_msg";
			}

			echo '<li><a href="affiche_thread.php?id_thread='. $last_sujet['id'] .'&amp;db_name='. $db_name .'#'.$last_msg.'" class="link '. $row_class .'">'. $this->forum->parseSujet($last_sujet['nom_thread']) .'</a> ';
			echo '</li>';
		}

		echo '</ul>
		</div><div class="clear"></div>';
		
	}



  /*
   *
   * Fonction legende
   * Affiche la légende des types de message (nonvu / nouveau msg / pas de nouveau)
   *
   * @return void
   *
   */
  
	public function forumLegende($place = ''){
		$locale = Locale::getLocale();
    ?>
    <ul class="legend">
			
			<?php switch ($place) {
			
				case 'thread' : ?>
				
      <li class="noview_msg left">
				<a class="noview_msg left" >&nbsp;</a>
				<span class="left">
					<?=$locale->display('forum_not_seen','forum')?>
				</span>
			</li>
			
      <li class="new_msg left">
				<a class="new_msg left" >&nbsp;</a>
				<span class="left">
					<?=$locale->display('new_msg','forum')?>
				</span>
			</li>
			
      <li class="nonew_msg left">
				<a class="nonew_msg left" >&nbsp;</a>
				<span class="left">
					<?=$locale->display('no_msg','forum')?>
				</span>
			</li>

			<?php	break; case 'salon' : ?>

      <li class="left">
				<a class="new_messages left" >&nbsp;</a>
				<span class="left">
					Message(s) non lu(s)
				</span>
			</li>
			
      <li class="left">
				<a class="see_most_crowded left" >&nbsp;</a>
				<span class="left">
					Pas de nouveaux messages
				</span>
			</li>
			
      <li class="add_new left">
				<a class="add_new left" >&nbsp;</a>
				<span class="left">
					Ajouter un sujet au salon
				</span>
			</li>

			<?php break; } ?>
			
    </ul>
		<div class="clear"></div>
    <?php
  }



  /*
   *
   * Fonction afficheSalon
   * Génère la liste des sujets d'un salon (= 1 thème) (en fonction de la page en cours)
   *
   * @param $id_salon
   * @param $num_page
   * @param $db_name
   * @param $salon_name
   * @param $droit_ecriture
   * @param $nb_pages
   * @param $sujets
   * @return void
   *
   */

  function afficheSalon($id_salon, $num_page, $db_name, $salon_name, $droit_ecriture, $nb_pages , $sujets){
    global $core, $db, $page;
		$locale = Locale::getLocale();
    
    $core->loadClass('date');
		$core->loadClass('htmlblock');
    $date = new Date();
	  $hb = new HtmlBlock();
		
		if($this->is_trombi){
			$core->loadClass('trombi');
			$trombi = new Trombi();
		}
			
    echo '<div class="module threads">
        <div class="title">
          <div class="border-left">
            <div></div>
          </div>
          <div class="border-right">
            <div></div>
          </div>
          <h2>';
		if($prev_salon = $this->forum->getPreviousSalon($id_salon)){
			echo '<a class="goto_prev" href="thread.php?id_salon='.$prev_salon.'&amp;page=1&amp;db_name='.$db_name.'" onmouseover="activateToolTips(this, \'Aller au Salon précédent\')">&nbsp;</a>';
		}
		
    echo '<span class="left">'.$salon_name.'</span>';
		
		if($next_salon = $this->forum->getNextSalon($id_salon)){
			echo '<a class="goto_next" href="thread.php?id_salon='.$next_salon.'&amp;page=1&amp;db_name='.$db_name.'" onmouseover="activateToolTips(this, \'Aller au Salon suivant\')">&nbsp;</a>';
		}
    
		echo '</h2>';
          
		echo'<a class="mark_as_read" href="thread.php?id_salon='.$id_salon.'&amp;db_name='.$db_name.'&amp;rm" onmouseover="activateToolTips(this, \'Tous les salons seront marqués comme lus\')">Marquer tout comme lu</a>';

    echo '<div class="clear"></div>
      </div>
      <div class="content">'; 

			/************* BOUTONS D'AJOUT DE SUJET & RETOUR ACCUEIL *************/

				echo'<a class="link_button stepBack" href="salon.php">Retour à l\'accueil du forum</a>';
				
		    if($this->forum->isWriteableSalon($id_salon)) {

						 echo'<a class="link_button new_thread" href="new_thread.php?db_name='.$db_name.'&id_salon='.$id_salon.'">Nouveau topic</a>';
				}
		echo'<div class="clear"></div>';

																/*************/
    $alt = 2;
    while ($sujet = $db->fetchArray($sujets))
    {
			//allow us to altern style
      $alt = ($alt == 2)? 1 : 2;

			$last_unread_msg = $this->forum->getLastUnreadMessage($sujet['id']);
  		$last_msg =	( $last_unread_msg ) ? $last_unread_msg : $this->forum->getLastMessage($sujet['id']);
			
      // Ici on recupere ce qu'on avait dans salon.php
      $class = (!$this->forum->isVisited($sujet['id']) && $core->session->isAuthOk()) ?
        "noview_msg" :
        (($this->forum->getNbNvxPost($sujet['id']) > 0) ?
        "new_msg" :
        "nonew_msg");

      $nb_messag=$db->result($db->query("SELECT Count(id) FROM forum_message WHERE id_thread='" .$sujet['id']."' AND score='1' "),0);
			$result = $db->query("SELECT login,surnom,date FROM forum_message WHERE id_thread='".$sujet['id']."' AND score='1' ORDER BY date DESC LIMIT 1");
      $creator = $db->fetchArray($db->query("SELECT login, date FROM forum_message WHERE id_thread = '".$sujet['id']."' ORDER BY id ASC LIMIT 1"));
			$last_message = $db->fetchArray($result);

			$thread_status = '';
			// DO NOT DELETE : VERY IMPORTANT VARIABLE TO DEBUG IE DISPLAY
			$locked = '';

			// We check the status of the thread (Added 12/2006)
      if ($sujet['drop'] == 'true') {
				$thread_status .= ' hidden';
			}
			if ($sujet['locked'] == 'true') {
				$thread_status .= ' locked';
				$locked = ' locked';
			}
			if ($sujet['sticky'] == 'true') {
				$thread_status .= ' sticky';
			}
			
      echo '<div class="thread color'.$alt.$thread_status.'">';

     	echo '<div class="actions_menu left">
					 	<ul>
							<li><a class="'.$class.$locked.'"></a></li>
						</ul>
						</div>
					 <div class="thread_title_block left">
        			<a href="affiche_thread.php?id_thread='.$sujet['id'].'&amp;db_name='. $db_name .'#'.$last_msg.'" class="thread_title '.$class.' left" >'.$this->forum->parseSujet($sujet['nom_thread']).'</a>
				<p class="thread_infos">
        	<span class="left">par&nbsp;</span>
					<a class="thread_author left"'. 
						'onmouseover="activateToolTips(this,\'';
							if($this->is_trombi && $this->forum->get_pref($creator['login'], 'forum_myavatar') == 'true' ){
								echo $page->htmlBlock->escapeTipContent('<div class="avatar_lmessage">'.
								$trombi->displayPhoto($creator['login']).
								'</div>');
							}
							echo $page->htmlBlock->escapeTipContent('<div class="details"><div class="pseudo_auteur">'.$sujet['createur'].'</div><div class="date_publication"> le '.$date->formatDate($creator['date'],"d/m/y").' à '.$date->formatDate($creator['date'],"H\hi").'</div>').'\')" '.
						' onclick="';

						if($this->is_trombi){
							echo $trombi->openCard($sujet['login']);
						}
        	echo ';" >'.$sujet['createur'].'</a>
				</p>';


			echo '<div class="clear"></div><div class="goto">';
			$this->forum->displayThreadPages($sujet['id'], $db_name, $this->forum->modo);
			echo '</div></div>';
			
			echo '<div class="thread_last_msg right">';
			$result = $this->forum->getLastThreadMessageInfos($sujet['id']);
			echo '<p>'.$date->formatDate($result['date'], 'd/m/y').' - '.$date->formatDate($result['date'], 'H\hi').'</p>';
			echo '<p class="last_msg_author_block"><a class="last_msg_author"'.  				
						'onmouseover="activateToolTips(this,\'';
							echo $page->htmlBlock->escapeTipContent('<b>Dernier message par :</b>');
							if($this->is_trombi && $this->forum->get_pref($last_message['login'], 'forum_myavatar') == 'true' ){
								echo $page->htmlBlock->escapeTipContent('<div class="avatar_lmessage">'.
								$trombi->displayPhoto($last_message['login']).
								'</div>');
							}
							echo $page->htmlBlock->escapeTipContent('<div class="details"><div class="pseudo_auteur">'.$last_message['surnom'].'</div><div class="date_publication"> le '.$date->formatDate($last_message['date'],"d/m/y").' à '.$date->formatDate($last_message['date'],"H\hi").'</div>');
						echo '\')" '.
							 'onclick="';
						if($this->is_trombi){
							echo $trombi->openCard($last_message['login']);
						}
						echo '">'.$result['surnom'].'</a></p>';
			echo '</div>';
		
			echo '<div class="thread_msg_nb right" onmouseover="activateToolTips(this,\'Nombre de messages postés dans ce topic\');">';
			echo $sujet['nb_msg'];
			echo '</div>';


      if ($this->forum->modo) {
			
				echo'
						<div class="actions_menu right">
							<ul>
								<li><a class="admin_topic" href="edit_thread.php?id_thread='.$sujet['id'].'&amp;db_name='. $db_name .'" onmouseover="activateToolTips(this, \'Administration de ce sujet\')"></a></li>
							</ul>
						</div>';

			}

        echo '<div class="clear"></div></div>';
    }
    
    echo '<div class="pages">';
		$hb->pagesList("SELECT id FROM forum_thread WHERE id_salon='".$id_salon."' ", $this->forum->config['thread_per_page'], $num_page, 'href', 'thread.php?id_salon='. $id_salon .'&amp;db_name='. $db_name .'&amp;page=[#]', 'Page : ');
    echo '<div class="clear"></div></div>
      </div>
    </div>';
    
  }


	public function followSite($id_thread, $site){
		global $db, $core;
		$db->query("UPDATE forum_login SET site='".$site."' WHERE login='".$core->getUserId()."' AND thread='".$id_thread."' ");
	}


	public function followMail($id_thread, $mail){
		global $db, $core;
		$db->query("UPDATE forum_login SET mail='".$mail."' WHERE login='".$core->getUserId()."' AND thread='".$id_thread."' ");
	}


	public function displayFollowSiteButton($id_thread, $site){
		global $core;
		$locale = Locale::getLocale();
		
	  if($site == 'true'){
    	echo '
				<a onclick="'.
					'AJAX.getAndCall(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=follow_s&amp;s=false&amp;t='.$id_thread.'\', function(){'.
						'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=display_button_s&amp;s=false&amp;t='.$id_thread.'\', \'follow_site_button_top\');'.
						'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=display_button_s&amp;s=false&amp;t='.$id_thread.'\', \'follow_site_button_bottom\');'.
					'});'.
				'" class="site_follow icon" onmouseover="activateToolTips(this, \''.$locale->display('deactivate_follow_site','forum').'\')">
        </a>';
				
    }else{
      echo '
				<a onclick="'.
					'AJAX.getAndCall(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=follow_s&amp;s=true&amp;t='.$id_thread.'\', function(){'.
						'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=display_button_s&amp;s=true&amp;t='.$id_thread.'\', \'follow_site_button_top\');'.
						'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=display_button_s&amp;s=true&amp;t='.$id_thread.'\', \'follow_site_button_bottom\');'.
					'});'.
				'" class="site_follow icon" onmouseover="activateToolTips(this, \''.$locale->display('activate_follow_site','forum').'\')">
        </a>';
    }
	}


	public function displayFollowMailButton($id_thread, $mail){
		global $core;
		
    if($mail == 'true'){
      echo '
				<a onclick="'.
					'AJAX.getAndCall(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=follow_m&amp;m=false&amp;t='.$id_thread.'\', function(){'.
						'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=display_button_m&amp;m=false&amp;t='.$id_thread.'\', \'follow_mail_button_top\');'.
						'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=display_button_m&amp;m=false&amp;t='.$id_thread.'\', \'follow_mail_button_bottom\');'.
					'});'.
				'" class="mail_follow icon" onmouseover="activateToolTips(this, \''.$locale->display('deactivate_follow_mail','forum').'\')">
      	</a>';
					
    }else{
      echo '
				<a onclick="'.
					'AJAX.getAndCall(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=follow_m&amp;m=true&amp;t='.$id_thread.'\', function(){'.
						'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=display_button_m&amp;m=true&amp;t='.$id_thread.'\', \'follow_mail_button_top\');'.
						'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=display_button_m&amp;m=true&amp;t='.$id_thread.'\', \'follow_mail_button_bottom\');'.
					'});'.
				'" class="mail_follow icon" onmouseover="activateToolTips(this, \''.$locale->display('activate_follow_mail','forum').'\')">
      	</a>';
		}
	}
	

  /*
   *
   * Fonction afficheMessages
   * Génère la liste des messages d'un sujet (en fonction de la page en cours)
   *
   * @param $id_salon
   * @param $id_sujet
   * @param $db_name
   * @param $salon_name nom du salon
   * @param $sujet_name nom du sujet
   * @param $nb_pages nombre de page que compose le sujet
   * @param $num_page numéro de la page en cours
   * @param $suivi tableau contenant les informations en rapport avec le suivit d'un sujet (thread)
   * @param $messages resource Mysql contenant les messages à afficher
   * @param $last_msg
   * @param $droit_ecriture
   * @return void
   *
   */

  function afficheMessages($id_salon, $id_sujet, $db_name, $salon_name, $sujet_name, $nb_pages, $num_page, $suivi, $messages, $droit_ecriture){
    global $db, $core;
		$locale = Locale::getLocale();
  
	 	$core->loadClass('htmlblock');
	  $hb = new HtmlBlock();
    $core->loadClass('mail');
    $mail=new Mail();
    $core->loadClass('date');
    $date=new Date();
		
		echo '
		<a class="link_button left" href="new_thread.php?db_name='.$db_name.'&amp;id_salon='.$id_salon.'">Nouveau topic</a>';
		
   	echo '
		<a class="link_button right" href="affiche_thread.php?id_thread='.$id_sujet.'&amp;db_name='. $db_name .'#reply">Répondre</a>'; 
		
		echo '
		<div class="clear"></div>
		<div id="messages_block" class="module messages">
      <div class="title">
        <h2>';
		if($prev_thread = $this->forum->getPreviousThread($id_sujet, $id_salon)){
			echo '<a class="goto_prev" href="affiche_thread.php?id_thread='.$prev_thread.'&amp;page=1&amp;db_name='. $db_name .'">&nbsp;</a>';
		}
    
		echo '<span class="left">'.$sujet_name.'</span>';
		
		if($next_thread = $this->forum->getNextThread($id_sujet, $id_salon)){
			echo '<a class="goto_next" href="affiche_thread.php?id_thread='.$next_thread.'&amp;page=1&amp;db_name='. $db_name .'">&nbsp;</a>';
		}
		
    echo '</h2>
        <ul>';
 		echo '<li id="follow_site_button_top">';
		$this->displayFollowSiteButton($id_sujet, $suivi['site']);
		echo '</li>';
//not yet functional
//	 	echo '<li id="follow_mail_button_top">';
//		$this->displayFollowMailButton($id_sujet, $suivi['mail']);
//		echo '</li>';
    echo '
				</ul>
        <div class="clear"></div>
      </div>
      <div class="content">';
	
		$pagesNb = (int)$db->result($db->query("SELECT COUNT(*) FROM forum_message WHERE id_thread='".$id_sujet."' AND score = '1' "), 0);
		$hb->pagesList($pagesNb, $this->forum->config['msg_per_page'], $num_page, 'href', 'affiche_thread.php?id_thread='. $id_sujet .'&amp;db_name='. $db_name .'&amp;page=[#]', '');
		
    echo '<div class="clear"></div>'; 
    
    $last_sujet_msg = '';
		//number of messages
		$msg_nb = $db->numRows($messages);
		//message iterator
		$msg = 1;
		//$style is useful to altern message style
		$style = true;
    while ($message = $db->fetchArray($messages))
    {
			if ($msg == 1 && $msg != $msg_nb) {
				$msg_position = 'first';
			} elseif ($msg == $msg_nb && $msg > 1) {
					$msg_position = 'last';
				}	elseif ($msg == $msg_nb && $msg == 1) {
						$msg_position = 'alone';
					} else {
						$msg_position = '';
					}
      $alt = ($style) ? '2' : '1';
      echo '
				<div id="message_div_'.$message['id'].'" class="message color'.$alt.' '.$msg_position.'">';
				
			$style = !$style;
			
			//to go to the last message
     	echo ($msg == $msg_nb) ? '<a name="last"></a>' : '';
			$msg ++;
			
		 	$this->displayMessage('view', $message, $db_name);

			echo '
				</div>';
			
      $last_sujet_msg = $message['id'];
      $last_msg_titre = $message['sujet'];

    } 
		 
    //Debut du formulaire si formulaire il y a !
    $this->forum->readThread($id_sujet, $last_sujet_msg);
			
    if(!$core->verifDroits('ban_forum') && $this->forum->isWriteable($id_sujet) && $nb_pages == $num_page){
			$this->displayAddMessage('thread', $id_sujet, $db_name, $last_msg_titre, $num_page, $id_salon);
    }
      
		$hb->pagesList($pagesNb, $this->forum->config['msg_per_page'], $num_page, 'href', 'affiche_thread.php?id_thread='. $id_sujet .'&amp;db_name='. $db_name .'&amp;page=[#]', '');
    
		echo '<div class="clear"></div>
    </div>';
		
    echo'<div class="title">
      <h2>';

		if($prev_thread){
			echo '<a class="goto_prev" href="affiche_thread.php?id_thread='.$prev_thread.'&amp;page=1&amp;db_name='. $db_name .'">&nbsp;</a>';
		}
    
		echo '<span class="left">'.$sujet_name.'</span>';
		
		if($next_thread){
			echo '<a class="goto_next" href="affiche_thread.php?id_thread='.$next_thread.'&amp;page=1&amp;db_name='. $db_name .'">&nbsp;</a>';
		}

				
    echo'</h2>
			<ul>';
				
 		echo '<li id="follow_site_button_bottom">';
		$this->displayFollowSiteButton($id_sujet, $suivi['site']);
		echo '</li>';

//not yet functional
//	 	echo '<li id="follow_mail_button_bottom">';
//		$this->displayFollowMailButton($id_sujet, $suivi['mail']);
//		echo '</li>';
  
    echo '
				</ul>
        <div class="clear"></div>
      </div>
    </div>';
  }


	public function displayAddMessage($case, $id, $db_name, $last_msg_title ='', $num_page = '', $id_salon = ''){
		global $core, $db;
		$locale = Locale::getLocale();
		$core->loadClass('fileManager');
		$fm = new FileManager();
		
		do{
		  $fm_id = rand();
		}while(isset($_SESSION['file_manager_'.$fm_id]));

							
    //div for ajax preview
    echo '<div id="preview_reply" class="message preview_msg" style="display: none;"></div>';
		
		//anchor
   	echo '<a name="reply"></a>';
		
	
    echo '<form method="post" enctype="multipart/form-data" id="post_message" onsubmit="'.
			//if preview was activate before, restores form attributes changed by AJAX.submitForm() (or not changed, but I'm careful)
			'this.action = \'affiche_thread.php?db_name='.$db_name.(($case == 'thread') ? '&amp;id_thread='. $id .'&amp;page='. $num_page .'#last' : '').'\';'.
			'this.target = \'\';'.
			'this.method = \'post\';'.
			'this.enctype = \'multipart/form-data\';'.
		'" >
		<div>
     	<input type="hidden" name="uniqid" value="'.md5(microtime().getmypid()).'" />';
			
		//first message of a new thread
    if($case == 'salon'){
			echo '<input type="hidden" name="id_thread" value="new_thread" />
     	<input type="hidden" name="id_salon" value="'.$id.'" />';
		//message in an existent thread
		}else{
			echo '<input type="hidden" name="id_thread" value="'.$id.'" />';
			echo '<input type="hidden" name="id_salon" value="'.$id_salon.'" />';
		}
			
    echo '<input type="hidden" name="id_aforum" value="'.$this->id.'" />
     	<input type="hidden" name="wope_action" value="preview_m" />';
		//here are input used to display preview
		echo '
     	<input type="hidden" name="poster_login" value="'.$core->getUserId().'" />
     	<input type="hidden" name="id_message" value="reply" />
     	<input type="hidden" name="fm_id" value="'.$fm_id.'" />
		';
		echo '
		 </div>
		 <div class="clear"></div>';
		 
    echo '<input type="hidden" name="email" size="20" id="input-mail" maxlength="200" value="'.$core->user->getPref($core->getUserId(),"forum_email").'" />'; 
    echo '<input type="hidden" name="myavatar" value="true" />';
    echo '<input type="hidden" name="mysignature" value="true" />';
	
		$reply_message = '';
		$reply_nick = '';
		$reply_title = $last_msg_title;
		$answer = ($last_msg_title != '') ? true : false;
		
		if($case == 'thread'){
			$onkeyup = 'this.form.save_button.disabled = false;';
			$draft = $this->forum->getDraft($id);
			if($draft['subject'] != ''){
				$answer = false;
				$reply_title = stripslashes($draft['subject']);
			}
			if($draft['message'] != ''){
				$reply_message = stripslashes($draft['message']);
			}
		}
		
		$this->displayMessageArea('reply', $answer, $reply_title, $reply_message, $reply_nick, $onkeyup);
					
    if( $this->forum->isAttachement($id) || ($case == 'salon' && $core->verifDroits($db->result($db->query("SELECT droit_attachement FROM forum_salon WHERE id='".$id."' "), 0)) ) ){
			echo '<div id="file_manager_area" style="display: none;">';
			$fm->uploadOrChoose(true, '', '', true, 'forum_file');
			echo '</div>';
    }
		
    echo '
			<div class="buttons_block right">
				<input type="hidden" value="true" name="data" />';
		//user has the possibility to manually save his text
		if($case == 'thread' && $core->getUserId() != 0 && $core->getUserId() != ''){
			echo '<input type="button" value="'.$locale->display('save_draft','forum').'" name="save_button" onclick="'.
						'this.form.wope_action.value = \'save_d\';'.
						'this.form.action = \''.$core->makeUrl('applications/forum/ajax/forum.php').'\';'.
						'AJAX.submitAndCall(this.form, false);'.
						'this.disabled = true'.
					'" tabindex="3" /> ';
		}
		echo '<input type="button" value="'.$locale->display('preview','forum').'" name="preview_button" onclick="'.
						'this.form.wope_action.value = \'preview_m\';'.
						'show(gE(\'preview_reply\'));'.
						'this.form.action = \''.$core->makeUrl('applications/forum/ajax/forum.php').'\';'.
						'AJAX.submitAndUpdate(this.form, true, \'preview_reply\');'.
					'" tabindex="4" /> '.
				'<input type="submit" value="'.$locale->display('post','forum').'" name="submit_button" tabindex="5" />
			</div>
			<div class="clear"></div>
    </form>';
		
		if ($case == 'thread' && $core->getUserId() != 0 && $core->getUserId() != '') {
			echo '<script type="text/javascript">
				var message_text = gE(\'post_message\').message.value;
				var message_subject = gE(\'post_message\').sujet.value;
				
				function saveDraft(){
					setTimeout("'.
						'if(message_text != gE(\'post_message\').message.value || message_subject != gE(\'post_message\').sujet.value){'.
							'message_text = gE(\'post_message\').message.value;'.
							'message_subject = gE(\'post_message\').sujet.value;'.
							'gE(\'post_message\').wope_action.value = \'save_d\';'.
							'gE(\'post_message\').action = \''.$core->makeUrl('applications/forum/ajax/forum.php').'\';'.
							'AJAX.submitAndCall(gE(\'post_message\'), false);'.
							'gE(\'post_message\').save_button.disabled = true;'.
						'}'.
						'saveDraft();'.
					'", '.$this->forum->config['draft_save_interval'].');
				}
				saveDraft();
			</script>';

		}
	
		echo '<div class="clear"></div>';
		$_SESSION['file_manager_'.$fm_id] = serialize($fm);
		
	}

	public function displayMessage($case, $message, $db_name=''){
		global $core, $db, $page;
		$locale = Locale::getLocale();
		if(!isset($page))$page = new Page();
		$core->loadClass('date');
		$core->loadClass('filemanager');
		$core->loadClass('box');
		$box = new Box();
		$date = new Date();
		$fm = new FileManager();
		$form = new Form();
		if($this->is_trombi){
			$core->loadClass('trombi');
			$trombi = new Trombi();
		}

		//ajax
		if(!is_array($message)) {
			$message = $db->fetchArray($db->query("SELECT fm.*, DATE(ui.dateCreation) AS inscription_date , COUNT(um.id) AS author_msg_nb FROM forum_message um RIGHT JOIN( forum_message fm INNER JOIN usersInfo ui ON fm.login=ui.id  ) ON um.login = fm.login WHERE fm.id='".$message."' GROUP BY fm.id"));
		}
		
			echo '<a name="'.$message['id'].'" ></a>
			<div id="censured_alert_'.$message['id'].'" class="message censured" '.(($message['score']==1)?' style="display: none;" ':'').'>Message censuré</div>
      <div class="message_hud left">
        <a class="nickname" ';
				
			if($this->is_trombi){
				echo ' onclick="';
				echo $trombi->openCard($message['login']);
				echo '" ';
			}
			
			echo ' >'.
      	  $this->forum->parseSurnom($message['surnom']).
        '</a>';
        
      //gestion de l'avatar
      if( $this->is_trombi && $this->forum->get_pref($message['login'], 'forum_myavatar') == 'true' ){
				
				$censured = '';
				if( $this->forum->modo && $this->forum->get_pref($message['login'], 'forum_modoavatar') == 'true' ){
					$censured = ' censured';
				}
					
        echo '<div class="avatar'.$censured.'" >';
				echo $trombi->displayPhoto($message['login']);
				echo '</div>';
      }
      
			echo '<p class="subscription_date"><span>Inscrit le :</span> '.( isset($message['inscription_date']) ? $date->formatDate($message['inscription_date'], 'd/m/Y') : '' ).'</p>
						<p class="messages_nb"><span>Messages :</span> '.( isset($message['author_msg_nb']) ? $message['author_msg_nb'] : '' ).'</p>';
		
      echo '<ul>';
			
			$email = $this->forum->get_pref($message['login'], 'forum_email');
      if( $email != '' && $form->verifForm($email, 'email') )
      {
        //gestion du mail de l'auteur du message       
        echo '<li class="left"><a class="mail icon" href="mailto:'.$email.'" onmouseover="activateToolTips(this, \''. $page->htmlBlock->escapeTipContent($locale->display('mail','kernel')) .'\')" >
					<span style="display: none;">'.
          	$locale->display('mail','kernel').
					'</span></a></li>';
			}
			
			if($this->is_trombi){
          //Si la personne a rentré un mail on met un lien trombi
          echo '<li class="left"><a class="trombi icon" onclick="';
					echo $trombi->openCard($message['login']);
					echo '" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent($locale->display('profile','kernel')).'\')"><span style="display:none">'. $locale->display('profile','kernel') .'</span></a></li>';
			}
			
      echo '</ul>';
   					
      echo '</div>
        <div class="message_content_block right">
          <div class="info">
            <p class="message_title left">'. $message['sujet'] .'</p>
            <p class="date left">'.$date->formatDate($message['date'],"d M y H:i").'</p>
            <ul class="left">';
			
			switch($case) {

				case 'view';
		      echo '<li><a onclick="quoteMessage(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&wope_action=quote_m&m='.$message['id'].'\', \'textarea_message_reply\', true);" class="quote icon" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent($locale->display('reply_quoting_msg','forum')).'\')"><span style="display:none">'.$locale->display('quote_msg','forum').'</span></a></li>';
//currently too disturbing for users
//		      echo '<li><a onclick="quoteMessage(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&wope_action=quote_m&m='.$message['id'].'\', \'textarea_message_reply\', false);" class="quote icon" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent($locale->display('quote_msg','forum')).'\')"><span style="display:none">'.$locale->display('quote_msg','forum').'</span></a></li>';

      //gestion des boutons editer / rediter / haut / bas / signaler un abus /citer
			
//		      echo '<li><a onclick="openModo(\''.$core->makeUrl('applications/forum').'\',\''.$message['id'].'\',\''.$id_sujet.'\');" class="abus icon" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent($locale->display('abuse_msg','forum')).'\')"><span style="display:none">'.$locale->display('abuse_msg','forum').'</span></a></li>';
					if($core->session->isAuthOk()){
		      	echo '<li><a onclick="hide(toolTips);'.$this->displayAnnounceAbuse($message['id'], $message['surnom']).'" class="abus icon" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent($locale->display('abuse_msg','forum')).'\')"><span style="display:none">'.$locale->display('abuse_msg','forum').'</span></a></li>';
					}
			
		      if($this->forum->modo || $core->getUserId() == $message['login']){
        		echo '<li><a class="reedit icon" '.
							' href="'.$core->makeUrl('applications/forum/edit_message.php').'?id_thread='.$message['id_thread'].'&amp;id_message='.$message['id'].'&amp;ui='.$core->getUserId().'&amp;db_name='.$db_name.'" '.
							//for ajax edition, see forum/ajax/forum.php and displayEditMessage()
							//' onclick="hide(toolTips);AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=edit_m&amp;m='.$message['id'].'&amp;db_name='.$db_name.'\',\'message_div_'.$message['id'].'\')" '.
							' onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent($locale->display('edit','kernel')).'\')"></a></li>';
					}

      		if($this->forum->modo){
						echo'<li><a class="censurer" onclick="AJAX.getAndCall(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=censure_m&amp;m='.$message['id'].'\', function(){display(gE(\'censured_alert_'.$message['id'].'\'));} )" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent($locale->display('censure', 'forum')).'\')" ></a></li>';
      		}
					
      		echo '<li><a href="#" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent($locale->display('go_to_top','kernel')).'\')" class="top icon"><span style="display:none">'.$locale->display('go_to_top','kernel').'</span></a></li>
        	<li><a href="#footer" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent($locale->display('go_to_bottom','kernel')).'\')" class="bottom icon"><span style="display:none">'.$locale->display('go_to_bottom','kernel').'</span></a></li>';
					
					break;
				case 'preview':
					echo'<li><a class="censurer" onclick="hide(toolTips);gE(\'preview_'.$message['id'].'\').innerHTML = \'\'" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent('Fermer l\'aperçu').'\')" ></a></li>';				
					break;
				default:
					//empty
					break;
			}
      
      echo '</ul>
          <div class="clear"></div>
        </div>
        <div class="text">
          '.( isset($message['message']) && $message['message'] != '' ? $this->forum->parseMessage($message['message']) : '').'
        </div>';
				
			echo '<div class="message_file" id="message_file_'.$message['id'].'">';
			//displays possible linked file
		 	if( isset($message['file']) && $message['file'] != 0 && $message['file'] != '' && $fm->fileExists($message['file']) ) {
				echo '<div class="file_details left"><span>Fichier joint : </span>';
				echo $fm->summarize($message['file'], Array('download_name_size'));
				echo '</div>';
 
				if($this->forum->modo || $message['login'] == $core->getUserId()) {
					echo '<a onclick="'.$box->confirm(
						'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=remove_f&amp;m='.$message['id'].'\', \'message_file_'.$message['id'].'\');',
						'Etes-vous sûr de vouloir enlever ce fichier du message ?'
					).'" class="delete left" onmouseover="activateToolTips(this, \''.$page->htmlBlock->escapeTipContent('Supprimer le fichier').'\')"></a>';
				}
			}
			echo '<div class="clear"></div></div>';

			echo '<div class="message_end"></div>
						<div class="message_footer">'.( isset($message['signature']) && $message['signature'] != '' ? $this->forum->parseMessage($message['signature']) : '').'</div>
     		    </div>';

	}


	public function displayEditMessage($id_message, $db_name){
		global $db, $core;
		$locale = Locale::getLocale();

		if($this->is_trombi){
			$core->loadClass('trombi');
			$trombi = new Trombi();
		}
		
		$message = $db->fetchArray($db->query("SELECT fm.id, fm.id_thread, fm.login, fm.score, fm.date, fm.email, fm.avatar, fm.myavatar, fm.file, fm.surnom, fm.sujet, fm.message FROM forum_message fm WHERE fm.id='".$id_message."' "));

		$thread = $this->forum->getThreadData($message['id_thread']);
		
		echo '<div class="edit_msg">';
		echo '<div id="preview_'.$message['id'].'" class="message preview_msg" style="display: none;"></div>';

  	echo '<form id="post_message_'.$message['id'].'" method="post" '.
			//for standard submit
			' action="'.$core->makeUrl('applications/forum/affiche_thread.php').'#'.$id_message.'" '.
			
			//for ajax submit
			//' action="'.$core->makeUrl('applications/forum/ajax/forum.php').'" '.
			
			' onsubmit="'.
					'this.wope_action.value = \'save_edit_m\';'.
					//for standard submit
					'this.action = \''.$core->makeUrl('applications/forum/affiche_thread.php').'#'.$id_message.'\';'.
					
			//for ajax submit
			//		'this.form.action = \''.$core->makeUrl('applications/forum/ajax/forum.php').'\';'.
			//		'AJAX.submitAndUpdate(this, false, \'message_div_'.$message['id'].'\');'.
			//		'return false;'.
			
			'" '.
		'>
		<!--onSubmit="return verif_edit_message_vide_forum()">-->'; // ?
	
		echo '<div style="display: none;">
							<input type="hidden" name="wope_action" value="save_edit_m" />
							<input type="hidden" name="id_aforum" value="'.$this->id.'" />
							<input type="hidden" name="db_name" value="'.$db_name.'" />
				      <input type="hidden" name="id_thread" value="'.$message['id_thread'].'" />
		    		  <input type="hidden" name="id_message" value="'.$message['id'].'" />
 		      		<input type="hidden" name="poster_login" value="'.$message['login'].'" />
		     	  	<input type="hidden" name="myavatar" value="'.$message['myavatar'].'" />
		      		<input type="hidden" name="avatar_image" value="'.$message['avatar'].'" />
					</div>';
		

		if($this->forum->modo){
			echo '<div class="censure_msg left">
			<p class="radio_title">Censurer le message :</p>
			 
      <p class="radio_items"><input type="radio" name="censure" value="false" ';
			 
      if($message['score']=='1'){
			 	echo ' checked="checked" ';
			}
			echo ' />'.$locale->display('no','kernel').'<br />
        <input type="radio" name="censure" value="true" ';
				
			if($message['score']=='-1'){
				echo ' checked="checked" ';
			}
			
			echo ' />'.$locale->display('yes','kernel').'
				</p>
			</div>';
		}
					      				
        
			echo '<div class="censure_avatar left">
				<p class="radio_title">Censurer l\'avatar :</p>';
          
    	if($message['myavatar'] == 'true'){
        if($this->forum->modo){
					echo '<p class="radio_items">
										<input type="radio" name="avatar" value="true" ';
										if($core->user->getPref($message['login'],"forum_modoavatar") == 'true') {
											echo ' checked="checked" ';
										}
										echo ' />'.$locale->display('blocked','forum').'
										<br />
										<input type="radio" name="avatar" value="false" ';
										if($core->user->getPref($message['login'],"forum_modoavatar") == 'false') {
											echo ' checked="checked" ';
										}
										echo ' />'.$locale->display('authorized','forum').'
								</p>';
				}
				
				if($this->is_trombi && $this->forum->get_pref($message['login'], 'forum_myavatar') == 'true' ){
      		echo '<div class="avatar">';
					echo $trombi->displayPhoto($message['login']);
					echo '</div>';
				}
				
				echo '</div>';
      } else {
      		echo ''.$locale->display('no_avatar','forum').'</div>';
      	}

				
/*			echo '<div class="author_nickname left">
							<p class="input_nickname_title">Email :</p>
    					<input type="text" class="input_content" name="email" id="input-mail" maxlength="200" value="'.$message['email'].'" />
						</div>';*/

		echo '<div class="end_admin_part clear"></div>';
		$this->displayMessageArea($message['id'], false, $message['sujet'], $message['message'], $message['surnom']);
		
	  echo '<div class="actions_edit_block right">

							<div class="buttons_block right">'.
								'<input type="button" value="'.$locale->display('preview','forum').'" onclick="'.
									'this.form.action = \''.$core->makeUrl('applications/forum/ajax/forum.php').'\';'.
									'this.form.wope_action.value = \'preview_m\';'.
									'show(gE(\'preview_'.$message['id'].'\'));'.
									'AJAX.submitAndUpdate(this.form, false, \'preview_'.$message['id'].'\');'.
									'" tabindex="3" /> '.
								'<input type="submit" value="'.$locale->display('post','forum').'" name="edit" tabindex="4"/> '.
								'<input type="button" tabindex="5" value="'.$locale->display('cancel','forum').'" onclick="'.
								//for standard cancelling
									'window.location.href = \''.$core->makeUrl('applications/forum/affiche_thread.php').'?id_thread='.$message['id_thread'].'&amp;db_name='.$db_name.'#'.$id_message.'\';'.
									
								//for ajax cancelling
								//	'AJAX.getAndUpdate(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=display_m&amp;m='.$message['id'].'&amp;db_name='.$db_name.'\',\'message_div_'.$message['id'].'\');'.
								
								'" />'.
							'</div>';
		
		echo '	<div class="clear"></div>	
							<div class="comments_block right">
								<label for="com">
									<a onclick="display(gE(\'com_'.$message['id'].'\'))">Commenter l\'édition...</a>
								</label>
						 		<textarea class="comms_textarea" id="com_'.$message['id'].'" name="com" cols="75" rows="5" style="display: none;"></textarea>
      				</div>
					</div>	
				<div class="clear"></div>
  		</form>
		</div>';
		
	}
	
	
	public function displayMessageArea($id, $answer = false, $topic='', $message = '', $nickname = '', $onkeyup = ''){
		global $core;
		$locale = Locale::getLocale();
		
			$nick = $nickname != '' ? $nickname : $core->user->getPref($core->getUserId(),"forum_pseudo");
			
			echo '<div class="edit_options left">
    					<p class="form-input">
    						<label for="input-pseudo">'.$locale->display('nickname','kernel').' :</label>
    						<input type="text" name="surnom" size="20" id="input-pseudo" maxlength="20" value="'.( $nick != '' ? $nick : ( $core->user->getNickname() != '' ? $core->user->getNickname() : 'Anonyme' ) ).'" />
    					</p>';
			
		echo '<div class="smileys_list">';
		$this->forum->displaySmileys($id);
		echo '<div class="show_other_smilies"><a onclick="display(gE(\'more_smileys_'.$id.'\')); display(gE(\'smileys_showed\')); display(gE(\'smileys_hidden\'))"><span id="smileys_hidden">Autres</span><span id="smileys_showed" style="display: none;">Masquer les</span> smileys...</a></div>';
		echo '<div class="clear"></div>
					<div class="other_smilies_block"><span id="more_smileys_'.$id.'" style="display: none;">';
		$this->forum->displayMoreSmileys($id);
		echo '</span>';
		echo '</div></div>';
			
		echo '</div>'; 
		
		echo '<div class="edit_block right">
						<p class="form-input">
      				<label for="sujet">'.$locale->display('subject','kernel').' :</label>
      				<input type="text" name="sujet" id="sujet_'.$id.'" onkeyup="'.$onkeyup.'" maxlength="70" value="';
		if ($answer) {
    	//we don't write "re :" more than one time
    	if (!preg_match("/^".$locale->display('re','kernel')."/i", $topic)) {
				echo $locale->display('re','kernel').': ';
			}
		}
		echo $topic;
		echo '" tabindex="1" /></p>';
																																																		
		echo '
		<div class="clear"></div>
		<div class="edition_icons_block left">
		<p class="list_icons">
	    <a class="bold icon" onclick="applyBBStyle(gE(\'textarea_message_'.$id.'\'),\'b\',\'/b\',\''.$locale->display('bold_text','forum').'\');" onmouseover="activateToolTips(this,\''.$locale->display('bold_selected_text','forum').'\')" ><span style="display:none">'.$locale->display('bold','kernel').'</span></a>
      <a class="italic icon" onclick="applyBBStyle(gE(\'textarea_message_'.$id.'\'),\'i\',\'/i\',\''.$locale->display('italic_text','forum').'\');" onmouseover="activateToolTips(this,\''.$locale->display('italic_selected_text','forum').'\')" ><span style="display:none">'.$locale->display('italic','kernel').'</span></a>
      <a class="underline icon" onclick="applyBBStyle(gE(\'textarea_message_'.$id.'\'),\'u\',\'/u\',\''.$locale->display('underline_text','forum').'\');" onmouseover="activateToolTips(this,\''.$locale->display('underline_selected_text','forum').'\')" ><span style="display:none">'.$locale->display('underligned','kernel').'</span></a>
      <a class="quote icon" onclick="applyBBStyle(gE(\'textarea_message_'.$id.'\'),\'quote\',\'/quote\',\''.$locale->display('quote_text','forum').'\',\''.$locale->display('quote_author','forum').'\');" onmouseover="activateToolTips(this,\''.$locale->display('quote_selected_text','forum').'\')" ><span style="display:none">'.$locale->display('quote','kernel').'</span></a>
      <a class="weblink icon" onclick="applyBBStyle(gE(\'textarea_message_'.$id.'\'),\'url\',\'/url\',\''.$locale->display('link_text','forum').'\',\''.$locale->display('link_url','forum').'\');" onmouseover="activateToolTips(this,\''.$locale->display('link_selected_text','forum').'\')" ><span style="display:none">'.$locale->display('bond','kernel').'</span></a>
      <a class="mail icon" onclick="applyBBStyle(gE(\'textarea_message_'.$id.'\'),\'mail\',\'/mail\',\''.$locale->display('mail_text','forum').'\',\''.$locale->display('mail_address','forum').'\');" onmouseover="activateToolTips(this,\''.$locale->display('mail_selected_text','forum').'\')" ><span style="display:none">'.$locale->display('mail','kernel').'</span></a>
      <a class="add_file icon" onclick="display(gE(\'file_manager_area\'));" onmouseover="activateToolTips(this,\'Joindre un fichier\')" ><span style="display:none">Joindre un fichier</span></a>
    </p>
		</div>
		<div class="clear"></div>';

		echo '<textarea class="left" name="message" id="textarea_message_'.$id.'" onkeyup="'.$onkeyup.'" cols="75" rows="10" tabindex="2">';
		echo $message;

		echo '</textarea>
		<div class="size_icons left">
			<a class="minus icon" onclick="decreaseAreaSize(gE(\'textarea_message_'.$id.'\'), 5, 5)">&nbsp;</a>
			<a class="plus icon" onclick="increaseAreaSize(gE(\'textarea_message_'.$id.'\'), 25, 5)">&nbsp;</a>
		</div>
		<div class="clear" style="width: 100px;"></div>
	</div>';

	}

	public function previewMessage($post){
		global $core, $db;
		$core->loadClass('date');
    $date = new Date();

		$message['id'] = $post['id_message'];
		$message['id_thread'] = $post['id_thread'];
		$message['login'] = $post['poster_login'];
		$message['score'] = isset($post['censure']) && ($post['censure'] == 'true') ? '-1' : '1';
		$message['surnom'] = stripslashes($post['surnom']);
		$message['myavatar'] = $post['myavatar'];
		//$message['email'] = isset($post['email']) ? stripslashes($post['email']) : '';
		$message['sujet'] = stripslashes($post['sujet']);
		$message['message'] = stripslashes($post['message']);
		$message['date'] = '';

		$this->displayMessage('preview', $message, ( isset($post['db_name']) ? $post['db_name'] : null) );
}																																

	public function displayDeleteThread($id_thread){
		global $core;
		$core->loadClass('box');
		$box = new Box();
		
    echo '<a class="admin_link right" onclick="'.$box->confirm('AJAX.getAndCall(\''.$core->makeUrl("application/forum/ajax/forum.php").'?id_aforum='.$this->id.'&wope_action=delete_t&t='.$id_thread.'\'); document.location = \''.$core->makeUrl("applications/forum/salon.php").'\';','Etes-vous sûr de vouloir supprimer le sujet ? Tous les messages lui appartenant seront détruits.').'">Effacer ce sujet</a>';
	}

	public function displayDeleteSalon($id_salon){
		global $core;
		$core->loadClass('box');
		$box = new Box();
	  echo '<a class="admin_link right" onclick="'.$box->confirm('AJAX.getAndCall(\''.$core->makeUrl("application/forum/ajax/forum.php").'?id_aforum='.$this->id.'&wope_action=delete_s&s='.$id_salon.'\'); document.location = \''.$core->makeUrl("applications/forum/salon.php").'\';','Etes-vous sûr de vouloir supprimer le salon ? Tous les messages lui appartenant seront détruits.').'">Effacer ce salon</a>';
	}

	
	/**
	 * displayAnnounceAbuse() displays a formulary in a box to announce an abuse in a post
	 * 
	 * @param  integer  $msg_id  id of the post
	 * @param  string   $nick    nickname of the poster
	 *
	 * @return string   javascript code
	 */
	private function displayAnnounceAbuse($msg_id, $nick){
		global $core, $page;
		$locale = Locale::getLocale();
	
		$content = '<div id="empty_comment" style="display: none;" >';
		$content .= 'Vous n\'avez pas entré de commentaire.';
		
		$content .= '</div>
		<div id="announce_abuse_feedback" style="display: none;" >';
		$content .= 'Envoi en cours, veuillez patienter...';
		
		$content .= '</div>
		<p>'.$locale->display('announce_modo', 'forum').$nick.$locale->display('announce_modo_2', 'forum').'</p>
		<form action="'.$core->makeUrl("applications/forum/ajax/forum.php").'" onsubmit="'.
			'if(this.comment.value != \'\'){'.
				'hide(gE(\'empty_comment\'));'.
				'show(gE(\'announce_abuse_feedback\'));'.
				'AJAX.submitAndCall(this, false, function(){'.
					'dialBox.erase();'.
				'});'.
			'}else{'.
				'show(gE(\'empty_comment\'));'.
			'}'.
			'return false;'.
		'" >
			<div class="input-hidden">
				<input type="hidden" name="msg_id" value="'.$msg_id.'" />
				<input type="hidden" name="wope_action" value="announce_abuse" />
				<input type="hidden" name="id_aforum" value="'.$this->id.'" />
			</div>
			<div class="form-textarea">
				<textarea name="comment" cols="60" rows="6" onblur="validateInput(this, \'SOMETHING\');" >'.
						$locale->display('your_comment', 'forum').
			 '</textarea>
			</div>
			<p class="form-submit"><input type="submit" value="'.$locale->display('send', 'kernel').'" />
				<input type="button" value="'.$locale->display('cancel','kernel').'" onclick="dialBox.erase();" />
			</p>
		</form>';
		$content = strtr($content,"\n\r\t\0","    ");
		return 'dialBox.draw(\''.$page->htmlBlock->escapeTipContent($content).'\', \''.$page->htmlBlock->escapeTipContent($locale->display('announce_title', 'forum').' :').'\', false);';
	}

	
	public function displayAbusesList($id_salon, $db_name = ''){
		global $db, $core;
		$core->loadClass('date');
		$locale = Locale::getLocale();
		$date = new Date();
		
		$result = $db->query("SELECT fa.id_abus, fa.id_message, fa.login, fa.date, fm.id_thread, fa.is_read, fm.surnom, tt.firstname, tt.lastname, fa.com FROM ( (forum_abus fa LEFT JOIN trombi_trombi tt ON fa.login = tt.id_user) INNER JOIN forum_message fm ON fm.id = fa.id_message ) INNER JOIN forum_thread ft ON fm.id_thread = ft.id WHERE is_read = 'false' AND ft.id_salon = '".$id_salon."' ORDER BY fa.date ASC");
	
		if($db->numRows($result) > 0){
			echo '
			<a id="abuses_go_back" class="link_button" href="'.$core->makeUrl('applications/forum/salon.php').'">Retour à l\'accueil du forum</a>
			<table class="data_table" width="98%">
  	  	<tr>
					<th width="20%" >Soumis par</th>
					<th width="40%" >'.$locale->display('comment','kernel').'</th>
					<th width="20%" >Message écrit par</th>
					<th width="10%" align="center"></th>
					<th width="10%" align="center"></th>
				</tr>';

			while($abuse = $db->fetchArray($result)){
				echo '<tr id="infos_'.$abuse['id_abus'].'">
					<td><a onclick="display(gE(\'more_infos_'.$abuse['id_abus'].'\'));">+</a> '.$abuse['firstname'].' '.$abuse['lastname'].'</td>
					<td class="abuse_comment">'.$abuse['com'].'</td>
					<td align="center" >'.$abuse['surnom'].'</td>
					<td align="center"><a href="'.$core->makeUrl('applications/forum/affiche_thread.php').'?id_thread='.$abuse['id_thread'].'&amp;db_name='.$db_name.'#'.$abuse['id_message'].'" onmouseover="activateToolTips(this,\''.htmlentities("Voir le message contenant l\'abus signal&eacute;").'\');">Voir</a></td>
					<td align="center">
						<a '.
							' onclick="hide(toolTips);AJAX.getAndCall(\''.$core->makeUrl('applications/forum/ajax/forum.php').'?id_aforum='.$this->id.'&amp;wope_action=archive_abuse&amp;a='.$abuse['id_abus'].'\', function(){remove(gE(\'infos_'.$abuse['id_abus'].'\')); remove(gE(\'more_infos_'.$abuse['id_abus'].'\')); });" '.
							' onmouseover="activateToolTips(this, \'Cliquez ici si vous avez traité l\\\'abus\');" '.
						'>Archiver</a></td>
				</tr>
				<tr id="more_infos_'.$abuse['id_abus'].'" style="display: none;">
					<td colspan="5" class="abuse_more_infos">le '.$date->formatDate($abuse['date'], 'd/m/Y').'</td>
				</tr>';

			}
			echo '</table>';
			
		}else{
			echo '<p>'.$locale->display('no_abuse', 'forum').'</p>';
		}
	}


}// Fin de la classe afficheforumBasic

?>
