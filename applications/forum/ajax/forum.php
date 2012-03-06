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


	$SECU = 'all';
	$relatedPath = '../../..';

	include_once($relatedPath.'/include/main.inc');
	$core->loadClass('forum',dirname('__FILE__').'/..');
	$core->loadClass('afficheforumbasic',dirname('__FILE__').'/..');
	$forum_basic = unserialize($_SESSION['affiche_forum_'.$_REQUEST['id_aforum']]);

	switch($_REQUEST['wope_action']) {
		case 'censure_m':
			$forum_basic->forum->censureMessage($_GET['m']);
			break;
		case 'delete_s':
			$forum_basic->forum->deleteSalon($_GET['s']);
			break;
		case 'delete_t':
			$forum_basic->forum->deleteThread($_GET['t']);
			break;
		case 'remove_f':
			$forum_basic->forum->removeFile($_GET['m']);
			break;
		case 'edit_m':
			$forum_basic->displayEditMessage($_GET['m'], $_GET['db_name']);
			break;
//for ajax message edition
/*		case 'save_edit_m':
			$forum_basic->forum->editMessage($_POST);
			$forum_basic->displayMessage('view', $_POST['id_message'], $_POST['db_name']);
			break;*/
		case 'display_m':
			$forum_basic->displayMessage('view', $_GET['m'], $_GET['db_name']);
			break;
		case 'quote_m':
			$forum_basic->forum->quoteMessage($_GET['m']);
			break;
		case 'preview_m':
			$forum_basic->previewMessage($_POST);
			break;
		case 'save_d':
			$forum_basic->forum->saveDraft($_POST);
			break;
		case 'follow_s':
			$forum_basic->followSite($_GET['t'], $_GET['s']);
			break;
		case 'follow_m':
			$forum_basic->followMail($_GET['t'], $_GET['m']);
			break;
		case 'display_button_s':
			$forum_basic->displayFollowSiteButton($_GET['t'], $_GET['s']);
			break;
		case 'display_button_m':
			$forum_basic->displayFollowMailButton($_GET['t'], $_GET['m']);
			break;
		case 'announce_abuse':
			$forum_basic->forum->announceAbuse($_POST);
			break;
		case 'archive_abuse':
			$forum_basic->forum->archiveAbuse($_GET['a']);
			break;
		default:
			//void
			break;
	}
?>
