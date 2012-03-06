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



	$SECU='log';
	$relative_path = '../..';
	include_once('../../include/main.inc');
	$core->loadClass('tag',dirname(__FILE__).'/../include/engine/tag');
	$core->loadClass('tag_interface',dirname(__FILE__).'/../include/engine/tag');
	$get=array();
	if (isset($_GET['action']) && isset($_GET['hash'])) {
		$sha=sha1($_GET['action']."tag");
		if ($_GET['hash']=$sha) {
			parse_str($_GET['action'],$get);
		}
	}
	
	if (sizeof($get)==0) {
		exit();
	} 
	
	$tableNames=array($get["table_name"]=>$get["table_name_id"]);
	$tag = new Tag($tableNames,$core,$db);
	$tagInterface=new tag_interface($get["table_name"],$get["table_name_id"]);
	
	switch ($get['action']) {
		case "digg_information" :
			$tag->diggInformation($get['id_information']);
			echo $tag->getNbDigg($get['id_information']);
			break;
		case "add_comment" :
			$tag->addComment($get['id_information'],$_GET['texte']);
			echo $tagInterface->displayCommsIdInformation($get['id_information']);
			break;
		case "star_information" :
			$tag->starInformation($get['id_information']);
			break;
		case "unstar_information" :
			$tag->unstarInformation($get['id_information']);
			break;
		case "share_information" :
			$tag->shareInformation($get['id_information']);
			break;
		case "unshare_information" :
			$tag->unshareInformation($get['id_information']);
			break;
		case "add_tag" :
			if ($_GET['tag_name'] != '') {
				$idTag = $tag->getIdTagName($_GET['tag_name']);
				$tag->addTagInformation($get['id_information'],$idTag,false);
				echo $tagInterface->displayTagUser($get['id_information']);
			}
			break;
		case "remove_tag" :
			$idTag=$tag->getIdTagName($_GET['tag_name']);
			$tag->removeTagInformation($get['id_information'],$idTag);
			echo $tagInterface->displayTagUser($get['id_information']);
			break;
	}
	
?>
