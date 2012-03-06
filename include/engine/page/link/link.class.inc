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

/**
	* class Link
	*/
class Link {

	public final function __construct(){
		//void
	}

	public 	function toAdmin($admin_part='home',$content='default',$right){
		global $core;
		$locale = Locale::getLocale();

		$link = '';
		
		if($core->verifDroits($right)){
			$link = '<a class="admin_link" href="'.$core->makeUrl('applications/admin/index.php').'?part='.$admin_part.'&content='.$content.'">'.$locale->display('admin','kernel').'</a>';
		}
		return $link;
	}

	public function mailTo($email, $text = null, $class = null, $attributes = null){
		global $core;
		
		if( !isset($text) || $text == '' ){
			$text = $email;
		}

		if( !isset($attributes) ){
			$attributes = '';
		}

		if( !isset($class) ){
			$class = '';
		}else{
			$class = ' '.$class;
		}
		
		if(!$webmail = $core->getConfig('mainMail', 'webmail')){
			$webmail = 'none';
		}
		
		$email = urlencode($email);
				
		switch( $webmail ){
			case 'horde':
				$href = 'javascript:hide(toolTips);popup_imp(\'/include/engine/page/link/hordeCompose.php\', 700, 650, \'to='.$email.'&thismailbox=INBOX\');';
				break;

			default:
				$href = 'mailto:'.$email;
				break;
		}
		
		$link = '<a class="mailto'.$class.'" '.$attributes.' href="'.$href.'">'.$text.'</a>';
		return $link;
	}
}
?>
