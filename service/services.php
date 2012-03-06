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
include_once('../main.inc');
$core->loadClass('new_mail', dirname(__FILE__).'/../engine/mail');


switch($_REQUEST['wope_action']){
	case 'reconnect':
		echo 'reconnected';
		break;
		
	case 'check_mail':
		$new_mail = new New_Mail();
		$new_mail->displayNewMail();
		break;
	
	case 'new_mails_nb':
		$new_mail = new New_Mail();
		$new_mail->displayNewMailsNb();
		break;
	
	default:
		//empty
		break;
}
?>
