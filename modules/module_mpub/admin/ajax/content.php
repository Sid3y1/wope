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



  $SECU='admin';
	include_once('../../../../include/main.inc');
	include_once('../module_mpub_admin.class.inc');
	$admin_mpub = unserialize($_SESSION['admin_mpub_'.$_REQUEST['id_admin']]);
	
	switch($_REQUEST['content']){
		case 'display_ads':
			$admin_mpub->displayAds();
			break;
		case 'add_ad':
			$admin_mpub->displayAddAd();
			break;
		case 'update_ad':
			$admin_mpub->displayUpdateAd($_GET['id_ad']);
			break;
		case 'del_ad':
			$admin_mpub->deleteAd($_GET['id_ad']);
			$admin_mpub->displayAds();
			break;
		case 'save_ad':
			$admin_mpub->saveAd($_REQUEST, (isset($_FILES['ad_image']))?$_FILES['ad_image']:'');
			$admin_mpub->displayAds();
			break;
		case 'validate_pub';
			$admin_mpub->validatePub($_GET['id_pub'],'Y');
			$admin_mpub->displayAds();
			break;
		case 'cancel_pub';
			$admin_mpub->validatePub($_GET['id_pub'],'N');
			$admin_mpub->displayAds();
			break;
		default:
			//void
			break;
	}
?>
