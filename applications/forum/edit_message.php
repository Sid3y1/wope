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


   $relative_path = '../..';
	 $SECU = "log";
   include ($relative_path . '/include/main.inc');
   
	 //Initialisation du forum
	 $core->localLoadClass('afficheforumbasic');
	 $affiche_forum = new afficheforumBasic();

   $id_thread=$_GET['id_thread'];
   $id_message=$_GET['id_message'];
	 $db_name = $_GET['db_name'];
	 $ui = (int)$_GET['ui'];
   $thread = $affiche_forum->forum->getThreadData($id_thread);
   $id_salon = $thread['id_salon'];
   $salon_tmp = $affiche_forum->forum->getSalonData($id_salon);
	 $salon_name = $salon_tmp['nom'];
	 
	 $ARIANEWIRE = Array(
	 	'Accueil' => $core->makeUrl('index.php'),
		'Forum' => $core->makeUrl('applications/forum'),
		$salon_name => $core->makeUrl('applications/forum/thread.php').'?id_salon='.$id_salon.'&db_name='.$db_name ,
		$thread['nom_thread'] => $core->makeUrl('applications/forum/affiche_thread.php').'?id_thread='.$id_thread.'&db_name='.$db_name
	 );
	 
   $page = new Page('Forum :','');
	 $page->header(locale::display('forum_edit_msg','forum'), 'forum');

   if ( (int)$core->getUserId() != (int)$ui && !$affiche_forum->forum->isModerateur($id_thread) ){
	 	$page->msgError('Vous n\'avez pas les droits requis pour accèder à cette page.');
	 	$page->footer();
   	exit();
   }
      

	echo '
	<div id="messages_block" class="module messages">
	  <div class="title">
	    <h2>
			</h2>
			<div class="clear"></div>
    </div>
		<div class="content">
	';
		
	echo '<div id="message_div_'.$id_message.'" class="message color2 alone">';
	$affiche_forum->displayEditMessage($id_message, $db_name);
	echo '</div>';

	echo '
		</div>
	</div>';
	
	$page->footer();
?>
