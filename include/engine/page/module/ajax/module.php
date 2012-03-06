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


$SECU = 'log';
include ( '../../../../main.inc');
include ('../../../../../applications/pwp/personalWopePage.class.inc');
$core->loadClass('ModuleBasic');

$page = new Page();
$pwp = new PersonalWopePage();

try{

	if( isset($_GET['ga']) && isset($_GET['mid']) && $_GET['ga'] != '' && $_GET['mid'] != '' ){
		$action  = $_GET['ga'];
		$mId = $_GET['mid'];

		if( $action == 'add' ){
			if( isset($_GET['cid']) && $_GET['cid'] != '' ){
				$cId = $_GET['cid'];
			
				if( !$module = $pwp->addModule($mId, $cId) ){
					throw new GException('addModule() failed to add module '.$mId.' at column '.$cId.'.', 4000, 1, 'Une erreur est apparue lors de l\'ajout de ce module.');
				}
				
			}else{
				throw new GException('Column ID missing.', 4000, 1, 'Une erreur est apparue lors de l\'ajout de ce module.');
			}
			
		}else{
			$core->loadClass( $_SESSION['userModules'][ $core->getUserId() ]['modules'][$mId]['class'] );
			$module = unserialize( $_SESSION['userModules'][ $core->getUserId() ]['modules'][$mId]['object'] );
		}

		switch($action){
			case 'up':
				$module->moveUp();
				break;
			case 'down':
				$module->moveDown();
				break;
			case 'del':
				$module->remove();
				break;
			case 'add':
				//the argument notices the module is in a special new div
				$module->displayModule(true);
				break;
			case 'minimaxi':
				$module->miniMaximize();
				break;
			default:
				//void
				break;
		}

	}else{
		throw new GException('Module ID or action missing.', 4000, 1, 'Une erreur est apparue lors de la gestion de ce module.');
	}
	
}catch(GException $e){
	$page->msgError( $e->getUserMessage() );
}

?>
