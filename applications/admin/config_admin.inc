<?
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


/*** Administration' panel configuration ***/
$config_admin = Array();

//path to reach admin
$config_admin['path'] = 'applications/admin';
//number of displayed rows in each summary (in home part)
$config_admin['summary_rows_nb'] = 10;
//period (days) of research of things to be moderated
$config_admin['summary_period'] = 7;

$config_admin['parts'] = Array(
	'home' => Array(
		'title' => 'Home',
		'rights' => Array('admin', 'webmaster')
	),
	
	'general' => Array(
	  'title' => 'Général',
		'rights' => Array('admin')
	),

	'right' => Array(
		'title' => 'Droits',
		'rights' => Array('admin')
	),
	
	'app' => Array(
		'title' => 'Applications',
		'rights' => Array('admin', 'webmaster')
	),
	
	'module' => Array(
		'title' => 'Modules',
		'rights' => Array('admin', 'webmaster')
	),
	
	'user' => Array(
		'title' => 'Utilisateurs',
		'rights' => Array('admin')
	),

	'category' => Array(
		'title' => 'Catégories',
		'rights' => Array('admin')
	),
	
	'stat' => Array(
		'title' => 'Statistiques',
		'rights' => Array('admin', 'webmaster')
	)
);

$config_admin['critical_rights'] = 'super_admin';

/** Applications administration configuration **/
//To add an application "APP" :
//$config_apps[APP_NAME] = Array('title'=>APP_TITLE,'class'=>APP_CLASS,'url'=>APP_DIR);
$config_apps = Array();

$config_apps['faq'] = Array('title'=>'FAQ','class'=>'faq_admin','url'=>'applications/faq/admin');
$config_apps['bug_report'] = Array('title'=>'Rapport de bug','class'=>'bug_report_admin','url'=>'applications/bug_report/admin');
$config_apps['trade'] = Array('title'=>'Petites annonces','class'=>'trade_admin','url'=>'applications/trade/admin');


/** Applications, modules and others, which have a summary in home part **/
$config_summaries = Array();

$config_summaries[] = Array('source' => 'admin', 'part' => 'user');
$config_summaries[] = Array('source' => 'module', 'module' => 'module_agenda');
$config_summaries[] = Array('source' => 'module', 'module' => 'module_news');

?>
