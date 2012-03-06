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


class Bug_report {

  public final function __construct() {
		//void
	}

	public function display_form_bug() {
		global $db;
		$severity_options='';
		$type_options='';
		$result_severity = $db->query("SELECT severity FROM bug_report_severity");
		$result_type = $db->query("SELECT type FROM bug_report_type");
		while($severity = $db->fetchArray($result_severity)){
			$severity_options.='<option value="'.$severity['severity'].'">'.stripslashes($severity['severity']).'</option>';
		}
		while($type = $db->fetchArray($result_type)){
			$type_options.='<option value="'.$type['type'].'">'.stripslashes($type['type']).'</option>';
		}
		echo <<<EOF
		<form action="index.php" method="post">
      <p class="form-input">		
			<label>Bug :</label>
			<input type="text" name="bug_title" id="bug_title" size="32" />
			</p>
			<p class="form-input">
			<label>Description :</label>
			<textarea name="bug_desc" id="bug_desc" cols="30" rows="8"></textarea>
      </p>
			<p class="form-input">
			<label>Gravité du bug :</label>
			<select name="bug_severity" id="bug_severity">
				$severity_options
			</select>
      </p>
			<p class="form-input">
			<label>Type de bug :</label>
			<select name="bug_type" id="bug_type">
				$type_options
			</select>
      </p>
      <p class="form-input">
			<input type="submit" value="Signaler le bug" />
      </p>
		</form>
EOF;
	}

	public function send_bug_report($title,$desc,$severity,$type,$page){
		global $core,$db,$config;
		$core->loadClass('mail');
		$mail = new Mail();
		if(!isset($title) || ($title=='')){
			$page->msgError('Veuillez remplir l\'intitulé du bug');
			$this->display_form_bug();
		}
		else{
			$to = $core->getConfig('mailBug');
			$from = $core->user->getEmail($core->getUserId());
			$subject = 'Bug report : '.$title;
			$msg = <<<EOF
Bug : $title

Description :
$desc

Gravité : $severity
Type : $type
EOF;
			if($mail->send($from, $to, $msg, $subject)){
				$page->msgInfo('Rapport envoyé ');;
			}else{
				$page->msgError('Échec envoi mail');
			}

			echo '<p><a href="" class="other_bug">Signaler un autre bug</a></p>';
		}
	}
	
}
?>			
