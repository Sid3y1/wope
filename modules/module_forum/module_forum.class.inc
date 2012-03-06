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


class Module_forum extends Module {
	
	private $id;
	
	public final function __construct($informations = Array()) {
		parent::__construct($informations);
	
		$this->id = rand(1000000,9999999);
		$_SESSION['module_forum_'.$this->id] = serialize($this);
	}
	
	private function miniParse($text=''){
		global $core;
		$core->loadClass('htmlblock');
		$hb = new HtmlBlock();
		
		$text=preg_replace("/\[i\]([^\[]*)\[\/i\]/","<i>\\1</i>",$text);
		$text=preg_replace("/\[b\]([^\[]*)\[\/b\]/","<strong>\\1</strong>",$text);
		$text=preg_replace("/\[s\]([^\[]*)\[\/s\]/","<u>\\1</u>",$text);
		$text=preg_replace("/\[.*\]([^\[]*)\[\/.*\]/","",$text);
		$text=preg_replace("/:([^\[]*):/","",$text);
		return $text;
	}

	private function displayForm()
	{
		global $core, $db;

		echo'
		<div id="create_new_thread" style="display: block; display: none">
			<form method="get" action="'.$core->makeUrl('applications/forum/new_thread.php').'">
					<p class="titre_menu">Dans quel salon voulez-vous créer votre sujet ?</p>
					<div class="clear"></div>
					<select class="left" name="id_salon">';
						$res = $db->query("SELECT id, nom, droit_ecriture FROM forum_salon ORDER BY nom ASC");
						while ($row = $db->fetchArray($res)) {
							if( $core->verifDroits(stripslashes($row['droit_ecriture'])) ){
								echo'<option value="'.$row['id'].'">'.stripslashes($row['nom']).'</option>';
							}
						}
		echo'	</select>
					<input class="admin_link left" type="submit" value="Créer" />
					<div class="clear"></div>
			 </form>
		</div>';
	}

	public function display()
	{
		global $core,$db;
		$core->loadClass('htmlblock');
		$hb = new HtmlBlock();
		
		$core->loadClass('forum', dirname(__FILE__).'/../../applications/forum/');
		$forum = new Forum();
		
		echo '<p class="forum_suivi_nbmessage premier_titre"><span class="L">L</span>iste des sujets que vous avez décidé de suivre :</p>';
		echo '<div id="followed_threads">';
		$this->displayFollowedThreads('../..');
		echo '</div>';

		$limit = $core->user->getMyPref("forum_nbrsuivit");
		$query = "SELECT fm.id, fm.id_thread, fm.login, fm.date, fm.surnom, fm.sujet, fm.message "
			." FROM forum_message fm "
			." WHERE fm.score='1' "
			." ORDER BY fm.date DESC "
			." LIMIT ".$limit." ";
			
		$result = $db->query($query);
			
		switch((int)$limit){
			case 0:
				break;
			case 1:
				echo '<p class="forum_suivi_nbmessage deuxieme_titre"><span class="L">D</span>ernier message sur le forum :</p>';
				break;
			default:
				echo '<p class="forum_suivi_nbmessage deuxieme_titre"><span class="L">L</span>iste des <b class="suivi_nb2">'. $limit .'</b> derniers messages sur le forum :</p>';
				break;
		}
		
		if($limit != 0){
				while( $message = $db->fetchArray($result) ){
					if( $forum->isReadable($message['id_thread']) ){
						$message_tab[] = $message;
					}
				}
				
				echo '<div>';
				$core->loadClass('date');
				$date = new Date();
				
				for ($i=0; $i <= $limit - 1; $i++) {
					if (isset($message_tab[$i])) {
						$message = $message_tab[$i];
						$id_message = $message['id'];
						
						$author = html_entity_decode($forum->parseSurnom(stripslashes($message['surnom'])), ENT_QUOTES, 'UTF-8');
						$text = $this->miniParse(stripslashes($message['message']));
						$topic = html_entity_decode(stripslashes($message['sujet']), ENT_QUOTES, 'UTF-8');
						$mDate = $date->formatDate($message['date'],"d/m à H:i");

						$msg_page = $forum->getPage($id_message, $forum->isModerateur($message['id_thread']) );
					
						$cutTitle = substr($topic,0,45);
						$thread_name = $cutTitle != $topic ? $cutTitle.'...' : $topic;
							
						$cutName = substr($author,0,11);
						$author_name = $cutName != $author_name ? $cutName.'..' : $author;

						$first_letter = substr($topic, 0, 1);
						$end_title = substr($topic, 1, 200);
							
						$tipContent =
						'<ul class="details_tt">
							<li class="sujet_message"><span>'.$first_letter.'</span>'.$end_title.'</li>
							<li class="date_publication">le '.$mDate.'</li>
							<li class="pseudo_auteur"><span>'.$author.'</span>'.' a écrit : </li>
						</ul>
						<div class="clear" style="height: 40px;"></div>
						<div class="le_texte">'.$text.'</div>';
							
						$tipPhoto = '';
						if($core->isTrombi()){
							$core->loadClass('trombi');
							$trombi = new Trombi();
							$tipPhoto = '<div class="picture_block">'.$trombi->displayPhoto($message['login'], true).'</div>';
						}
							
						echo '
						<div class="liste_topics '.( $i%2 ? 'l1' : 'l2').'">
							<span class="date_publication left">'.$mDate.' &nbsp;</span>
								<a class="left" onmouseover="'.$hb->tooltip($tipPhoto.$tipContent).'" href="'
                				.$core->makeUrl('/forum/affiche_thread.php?id_thread='
									.$message['id_thread']
									.'&amp;page='.$msg_page
									.'#'.$id_message).'">'
									.$thread_name
								.'</a>
							<span class="auteur right"> Par '.$author_name.'</span>
							<div class="clear"></div>
						</div>';
					}
				}
				echo '</div>';
				
				

				echo '
				<div>
					<a class="link_button" onclick="display(gE(\'create_new_thread\')); display(gE(\'creer_new_t\')); display(gE(\'masquer_new_t\'))">
						<span id="creer_new_t" >Créer un nouveau sujet</span>
						<span id="masquer_new_t" style="display: none;">Masquer la création de sujet</span>
					</a>
				</div>
				<div class="clear"></div>';

				$this->displayForm();
			}
	}

	public function displayFollowedThreads(){
		global $core, $db;
		include_once(dirname(__FILE__)."/../../applications/forum/forum.class.inc");
		$forum = new Forum();
		$core->loadClass('htmlblock');
		$hb = new htmlBlock();
	
		$tab_id=$forum->getThreadSuivi();
		if(count($tab_id) > 0)
		{
			echo '<div class="suivi">';
		
			$i=1;
			foreach($tab_id as $id)
			{
				$i++;
				$thread=$forum->getThreadData($id);

				//recovering of last un-read message id, or by default of last message id, in order to construct the url
				$last_unread_msg = $forum->getLastUnreadMessage($id);
				$last_msg = ($last_unread_msg) ? $last_unread_msg : $forum->getLastMessage($id) ;

				$msg_page = $forum->getPage($last_msg, $forum->isModerateur($id) );
				
				echo '<div class="liste_topics '; 
				
				if ($i%2) {
					echo 'l1 ';
				} else {
					echo 'l2 ';
				}
				
				echo'suivi_ligne"><a class="suivi_titre" href="'.$core->makeUrl('applications/forum/affiche_thread.php').'?id_thread='.$id.'&amp;page='.$msg_page.'#'.$last_msg.'" ';
				$thread_name = stripslashes($thread['nom_thread']);
				$cut = substr($thread_name,0,60);
				if($cut == $thread_name){
					echo '>'.$cut;
				}else{
					echo ' onmouseover="activateToolTips(this,\''.$hb->escapeTipContent($thread_name).'\');">'.$cut.'...';
				}
				echo '</a>';
				$nbNvxPost=$forum->getNbNvxPost($id);
				echo '<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_forum/ajax/module_forum.php').'?id_module_forum='.$this->id.'&action=remove_followed_thread&id_thread='.$id.'\',\'followed_threads\')" class="trash icon"></a><span class="suivi_nb">';
				if($nbNvxPost==1){
					echo '(1 message non lu)</span>';
				}elseif($nbNvxPost>1){
				  echo ' ('. $nbNvxPost .' messages non lu)';
			  }else{
					echo ' (Pas de message non lu)';
				}
				echo '</span></div><div class="clear"></div>';				
			}
			echo '</div><div class="clear"></div>';
		}
		else{
			echo '<p class="titre_suivi_forum">Vous n\'avez choisi de suivre aucun sujet.</p>';
		}
	}

	public function removeFollowedThread($id_thread){
		global $core,$db;
		$db->query("UPDATE forum_login SET site='false' WHERE login='".$core->getUserId()."' AND thread='".$id_thread."' ");
	}

}

?>
