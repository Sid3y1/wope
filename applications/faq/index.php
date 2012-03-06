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
$relative_path = '../..';
include($relative_path.'/include/main.inc');

$ARIANEWIRE = Array (
	_('Homepage') => "../index.php",
	_('FAQ') => "index.php"
);

$core->localLoadClass('faq');

$faq = new Faq();
$page = new Page();

$page->header(_('FAQ'),'faq');

echo '<p>'.$page->adminLink('app','faq','webmaster admin').'</p><div class="clear"></div>';
$page->moduleHeader(_('Frequently ask questions'),'');
$faq->display();
$page->moduleFooter();

$page->footer();
?>
