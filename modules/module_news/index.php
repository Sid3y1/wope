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


  //Obligatoire
  $SECU = 'log';       //Niveaux de sécurité (voir $session->authPage() )
  include ('../../include/main.inc');
	$ARIANEWIRE= Array (
  	_('Homepage') => $core->makeUrl('index.php'),
	  _('News') => $core->makeUrl('modules/module_news')
	);
		
	$core->loadClass('module_news');
	$news = new Module_news();

  $page = new Page();
  $page->header('Actualité','actualite');

	$action=(isset($_GET['action']))?$_GET['action']:'';
	switch($action){
		default:
		case 'display':
			$id_type=isset($_GET['id_type'])?$_GET['id_type']:'';
			$id_news=isset($_GET['id_news'])?$_GET['id_news']:'';

			$news->displayAppHeader();
			$news->displayTypes();

			$cur_page = isset($_GET['page']) ? $_GET['page'] : 1 ;

			//on affiche les evenements qui doivent l'etre
   		if (!empty($id_type)) {
      	$news->displayNewsType($id_type, $cur_page);
   		}elseif(!empty($id_news)) {
      	$news->displayNewsId($id_news);
   		}else{
				$news->displayNewsType('all', $cur_page);
			}
			break;
		case 'add':
			$news->displayAppHeader();
			$news->displayAddForm();
			break;
		case 'save':
			$news->saveNews($_POST);
			break;
	}

$page->footer();
?>
