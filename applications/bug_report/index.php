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


$SECU = 'log';     //Niveaux de sécurité (voir $session->authPage())

include ('../../include/main.inc');

$page = new Page();

$core->localLoadClass('bug_report');
$bug_report = new Bug_report();

$page->headerHTML('Rapport de bug','bug_report');

if ($_POST['bug_title']) {
	$bug_report->send_bug_report($_POST['bug_title'],$_POST['bug_desc'],stripslashes($_POST['bug_severity']),stripslashes($_POST['bug_type']),$page);
} else {
	$bug_report->display_form_bug();
}

echo '<p><a onclick="window.close()">Fermer cette fenetre</a></p>';

$page->footerHTML();

?>
