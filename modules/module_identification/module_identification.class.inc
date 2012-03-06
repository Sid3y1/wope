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


class Module_identification extends Module {

	private $config;

	public final function __construct($informations = Array()) {
		global $core;

		parent::__construct($informations);
	
		include(dirname(__FILE__).'/config_module_identification.inc');
		$this->config = $config_module_identification;
		$this->is_trombi = $core->isTrombi();
	}

	public function display() {
		global $core,$db,$page;
		
		include(dirname(__FILE__).'/../../applications/admin/config_admin.inc');
		
		$core->loadClass('checkmail');
		$new_mail = new CheckMail();
		
		//Affichage d'un formulaire d'identification

		//if(isset($_POST['user']) && isset($_POST['pass']))$core->session->open($_POST['user'],$_POST['pass']);
			if ($core->session->isAuthOk()) {
			//identifié

				//on donne le droit log a l'utilisateur
				if(!$core->verifDroits('log')){
					$core->user->droits[] = 'log';
				}
			
				echo '<div>
					<h3 class="welcome">'.$core->user->firstname.' '.$core->user->lastname.'</h3>';

				if($this->is_trombi){
					$core->loadClass('trombi');
					$trombi = new Trombi();

					$trombi->displayStatusInput();
				
					echo '<div class="picture_block"><div class="avatar" title="avatar"><span></span>';
					echo $trombi->displayPhoto();
					echo '</div></div>';
				}

				echo '<div class="right_block">
					<div class="menu_block"><ul class="user-menu panel">';

				foreach ($this->config['menu'] as $name => $url){
					echo '<li><a class="preference link_menu" href="'.$core->makeUrl($url).'"><span class="icon"></span><span class="text">'.$name.'</span></a></li>';
				}
			
				echo '</ul></div></div>
				<div class="clear"></div>';
				
				if($core->user->getMyPref('site_check_mail') == 'true'){
					echo '<div id="identification_new_mails_nb" class="new_mails_nb">';
					$new_mail->displayNewMailsNb();
					echo '</div>';
				}

				if( file_exists($core->getConfig('baseServer').'/invitation') ){
					echo '<p class="invite_block"><a class="invite" href="'.$core->makeUrl('invitation').'">'._('Invite your friends').'</a></p>';
				}

				$admin_rights = Array();
				foreach($config_admin['parts'] as $options){
					foreach($options['rights'] as $right){
						if( !in_array($right, $admin_rights) ){
					    $admin_rights[] = $right;
						}
					}
				}

				echo $page->adminLink('','',$admin_rights);
				
				echo'
				<div class="clear"></div></div>';

			} else {
				//Pas identifié
				if (empty($_GET['redirect'])) {
				/*
				eregi('^https?://[^/]+(/.*)$',$core->getConfig('baseUrl'),$ret);
				$repertoire=$ret[1];
				eregi("$repertoire(/.*)",$_SERVER['REQUEST_URI'],$ret);
				$nextPage = $ret[1];
			*/
				//A refaire trop dangereux !!
					$nextPage= "";
				} else {
					$nextPage = $_GET['redirect'];
				}

			?>

				<script language='javascript' type="text/javascript">
				function https()
				{
					if (!window.document.formAuth.auth_https.checked)
					{
						window.open('<?=$core->makeUrl('include/erreurs/no_https.php')?>', 'Attention', 'width=400,height=200');
						document.formAuth.action='<?=$core->getConfig('baseUrl')?>';
					}
					else
					{
						document.formAuth.action='<?=$core->getConfig('baseUrls')?>';
					}
  			}
  			</script>
				<div class="formulaire_identification">
				<form action="<?=$core->getConfig('baseUrl').$core->getConfig('baseDir')."/index.php"?>" style="margin-bottom:0px" method="POST" name="formAuth">
				<?php
					/*
					 * 	Désactivation des https
					 	<form action="<?=$core->getConfig('baseUrls')?>" style="margin-bottom:0px" method="POST" name="formAuth">
					 */
				?>
				<input type="hidden" name="redirect" value="<?=((isset($nextPage)&&$nextPage!='/')?$nextPage:"")?>">

				<p class="form-ligne">
				
  				<label for="user"><? echo _('Login') ?>: </label>
  				<input type="text" id="user" name="user" maxlength="50" tabindex="1" value="<?=(isset($_SESSION['connect_user']) ? $_SESSION['connect_user'] : '')?>" />
				</p>
				
				<div class="clear"></div>

				<p class="form-ligne">
  				<label for="password"><? echo _('Password') ?>: </label>
  				<input type="password" id="password" name="pass" maxlength="20" tabindex="2" value="<?=(isset($_SESSION['connect_pass']) ? $_SESSION['connect_pass'] : '')?>" />
				</p>

				<div class="clear"></div>

				  <?php

					if( isset($_SESSION['connect_user']) ){
						unset($_SESSION['connect_user']);
					}
					if( isset($_SESSION['connect_pass']) ){
						unset($_SESSION['connect_pass']);
					}
					
				  /*
				     <p class="form-input">
				     <input type="checkbox" checked="checked" id="auth_https" name="auth_https" onclick="https()" tabindex="4" />
				     <label for="auth_https" class="for-checkbox">Utiliser https</label>
					 </p>
				   */
				   ?>
        <p class="form-submit">
				  <input type="submit" class="alternative_button" name="login" value="Identification" tabindex="3" />
        </p>
        </div>
				<div class="clear"></div>
				<ul class="panel">
				  <li><a class="new_account link_menu" href="<?=$core->makeUrl('applications/signup')?>"><span class="text"><? echo _('Sign up') ?></span></a></li>
				  <li><a class="forgotten_pass link_menu" href="<?=$core->makeUrl('user/recover_pass.php')?>"><span class="text"><? echo _('Lost your password ?') ?></span></a></li>
				</ul>
				</form>
								<?php
		}
	}



}
