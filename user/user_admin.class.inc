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



class UserAdmin {

	/**
	 * $id id of this object, used to call sensible functions
	*/
	private $id;
	
	/**
	 * $path path to access this object
	*/
	private $path;

	private $config;
	
	public final function __construct($admin_path='user'){
		include('./config_user.inc');
		
		$this->id = rand(1000000,9999999);
		$this->path = $admin_path;
		$this->config = $config_user;
		$_SESSION['user_panel_'.$this->id] = serialize($this);
	}

	/**
	 * displayAdmin() calls admin of a part
	 * @param  $part    part to display (default : home)
	 * @param  $content (optionnal) content of the part
	 * @return void
	*/
	public function displayAdmin($part='',$content='',$option=''){
		$display_part = 'displayAdmin'.ucfirst(strtolower($part));
		switch($display_part){
			case 'displayAdmin':
				$this->displayAdminHome();
				break;
			default:
				$this->$display_part($content,$option);
		}
	}


/**** Below this line are functions called by displayAdmin() to display parts ****/

	/**
	 * displayAdminHome() displays user administration's home page
	 * @return void
	*/
	private function displayAdminHome(){
		global $core;
	/*  echo '<div class="admin_menu1">';
		$this->displayCurrentAlerts();
		echo '</div>';
		echo '<div class="admin_menu2">';*/
		$this->displayWelcomeMessage();
		//$this->displayMyVisitors();
	//	echo '</div>';
	}

	/**
	 * displayAdminAccount() displays user account preferences
	 * @param  $content content to display
	 * @param  $option  to give optional parameters
	 * @return void
	*/
	private function displayAdminAccount($content='default',$option=''){
		echo '<div class="admin_menu1">';
		$this->adminAccountMenu();
		echo '</div>';
		echo '<div class="admin_menu2" id="admin_account_content" >';
		$this->adminAccountContent($content);
		echo '</div>';
	}

	/**
	 * displayAdminInfo() displays user personal informations
	 * @param  $content content to display
	 * @param  $option  to give optional parameters
	 * @return void
	*/
	private function displayAdminInfo($content='default',$option=''){
		global $core, $page, $db;
		// Trombi part is managed by trombi.inc
		include_once(dirname(__FILE__)."/trombi.inc");
	}

	/**
	 * displayAdminPref() displays user preferences about the site
	 * @param  $content content to display
	 * @param  $option  to give optional parameters
	 * @return void
	*/
	private function displayAdminPref($content='default',$option=''){
		echo '<div class="admin_menu1">';
		$this->adminPrefMenu();
		echo '</div>';

		echo '<div id="admin_pref_content" class="admin_menu2">';
		$this->adminPrefContent($content);
		echo '</div>';
	}
	
	/**
	 * displayAdminCategory() displays user's followed categories management panel
	 * @return void
	*/
	private function displayAdminCategory(){
		global $page;
		$page->moduleHeader('Categories','');
		$this->listCategories();
		$page->moduleFooter();
	}

/**** End of parts display functions ****/	


/**** Home admin functions ****/

	/**
	 * displayWelcomeMessage() display a welcome message
	 * @return void
	*/
	private function displayWelcomeMessage(){
		global $page;
		$page->moduleHeader('Bienvenue','');
		echo $this->config['welcome_message'];
		$page->moduleFooter();
	}

	private function displayMyVisitors(){
		global $core, $page;
		
		if( $core->isTrombi() ){
			$core->loadClass('trombi');
			$trombi = new Trombi();
			
			echo '<a name="visit"></a>';
			$page->moduleHeader('Mes visiteurs','');
			echo '<div class="visit_prompt">Ces personnes ont visité ma fiche trombi dernièrement :</div>';
			$trombi->displayVisitorsOf($core->getUserId());
			$page->moduleFooter();
		}
	}

	private function displayCurrentAlerts(){
		global $page;
		if(!isset($page))$page = new Page();
		$page->moduleHeader('Informations','');
		echo '<p>Nous attirons votre attention sur les points suivants :</p>';
		echo '<p>Aucun point particulier en ce moment.</p>';
		$page->moduleFooter();
	}
	
/**** End of home admin functions ****/


/**** Account admin functions ****/

	private function adminAccountMenu(){
		global $core,$page;
		if(!isset($page))$page = new Page();
		
		$page->moduleHeader('Actions','');
		echo '<ul class="panel">';
		if( false ){
			echo '<li class="link_menu"><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_account.php').'?id_admin='.$this->id.'&wope_action=display_change_login\',\'admin_account_content\')">Changer mon identifiant</a></li>';
		}
		echo '<li><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_account.php').'?id_admin='.$this->id.'&wope_action=display_change_password\',\'admin_account_content\');">Changer mon mot de passe</a></li>';
		echo '<li><a onclick="AJAX.getAndEvalUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_account.php').'?id_admin='.$this->id.'&wope_action=display_destroy_account\',\'admin_account_content\');">Détruire mon compte</a></li>';
		echo '</ul>';
		$page->moduleFooter();
	}
	
	private function adminAccountContent($content=''){
		switch($content){
			default:
			case '':
			case 'default':
				$this->displayAccountDefaultContent();
				break;
			case 'login':
				$this->displayChangeLogin();
				break;
			case 'password':
				$this->displayChangePassword();
				break;
			case 'destruction':
				$this->displayDestroyAccount();
				break;
		}
	}

	public function displayAccountDefaultContent(){
		global $page;
		if(!isset($page))$page = new Page();
		$page->msgInfo('Veuillez choisir une action dans le menu.');
	}

	public function displayChangeLogin(){
		global $core,$page;
		if(!isset($page))$page = new Page();
		
		$page->moduleHeader('Changement d\'identifiant','');
		
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/admin_account.php').'" onsubmit="'.
			'AJAX.submitAndUpdate(this, false,\'admin_account_content\');'.
			'return false;'.
		'" >
		<input type="hidden" name="wope_action" value="save_user_login" />
		<input type="hidden" name="id_admin" value="'.$this->id.'" />
		<p class="form-input">
			<label>Identifiant actuel :</label>
			<input type="text" name="user_old_login" />
		</p>
		<p class="form-input">
			<label>Mot de passe :</label>
			<input type="password" name="user_pass" />
		</p>
		<p class="form-input">
			<label>Nouvel identifiant :</label>
			<input type="text" name="user_new_login" onblur="validateInput(this, \'LOGIN\');" />
		</p>
		<p class="form-submit"><input type="submit" value="Modifier" /></p>
		</form>';
		
		$page->moduleFooter();
	}

	public function saveUserLogin($post){
		global $db,$core,$page;
		if(!isset($page))$page = new Page();
		$form = new Form();

		$newLogin = stripslashes($post['user_new_login']);
		$oldLogin = stripslashes($post['user_old_login']);
		$pass = stripslashes($post['user_pass']);

		if( $oldLogin == $core->user->getLogin() && $pass == $core->user->getPass() ){
		
			if( $form->verifForm($newLogin, 'login') ){

				if($db->query("UPDATE usersInfo SET login='".$db->escapeString($newLogin)."' WHERE id='".$core->getUserId()."' ")){
					$core->user->saveInformations($newLogin,$pass);
					$core->saveInSession();
					$page->msgInfo('Identifiant enregistré. La modification est active dès à présent.');
				}else{
					$page->msgError('Erreur lors de l\'enregistrement.');
				}

			}else{
				$page->msgError('Le nouvel identifiant fourni n\'est pas valide.');
			}

		}else{
			$page->msgError('Le couple identifiant/mot de passe fourni n\'est pas valide.');
		}
	}
	
	public function displayChangePassword(){
		global $page,$core;
		if(!isset($page))$page = new Page();
		$page->moduleHeader('Changement de mot de passe','');

		echo '<div id="diff_pwd" style="display:none">';
		$page->msgError('Les deux mots de passe renseignés sont différents. Veuillez entrer deux mots de passe identiques.');
		echo '</div>';
		echo '<form id="frm_change_password" action="'.$core->makeUrl($this->path.'/ajax/admin_account.php').'" onsubmit="'.
			'if(this.user_pwd.value == this.user_pwd_conf.value){'.
				'AJAX.submitAndUpdate(this, false, \'admin_account_content\');'.
			'}else{'.
				'display(gE(\'diff_pwd\'));'.
			'};'.
			'return false;'.
		'" >
		<input type="hidden" name="wope_action" value="save_user_password" />
		<input type="hidden" name="id_admin" value="'.$this->id.'" />
		<p class="form-input">
			<label>Mot de passe actuel :</label>
			<input type="password" name="user_old_pwd" />
		</p>
		<p class="form-input">
			<label>Nouveau mot de passe :</label>
			<input type="password" name="user_pwd" id="user_pass" onblur="'.
							'if( validateInput(this, \'PASSWORD\') ){'.
								'show(gE(\'check_pass\'));'.
								'hide(gE(\'invalid_pass\'));'.
								'if( gE(\'user_pass_conf\').value != \'\' && this.value != gE(\'user_pass_conf\').value ){'.
									'show(gE(\'check_conf_alt\'));'.
									'hide(gE(\'check_conf\'));'.
								'}else{'.
									'hide(gE(\'check_conf_alt\'));'.
								'}'.
							'} else {'.
							  'hide(gE(\'check_pass\'));'.
								'show(gE(\'invalid_pass\'));'.
							'}'.

							'this.onkeyup = function(){'.
								'if( validateInput(this, \'PASSWORD\') ){'.
									'show(gE(\'check_pass\'));'.
									'hide(gE(\'invalid_pass\'));'.
									'if( gE(\'user_pass_conf\').value != \'\' && this.value != gE(\'user_pass_conf\').value ){'.
										'show(gE(\'check_conf_alt\'));'.
										'hide(gE(\'check_conf\'));'.
									'}else{'.
										'hide(gE(\'check_conf_alt\'));'.
									'}'.
								'} else {'.
								  'hide(gE(\'check_pass\'));'.
									'show(gE(\'invalid_pass\'));'.
								'}'.
							'}'.
			'"/>
			<div id="check_pass" class="check_input left" style="display: none;"></div>
			<div id="invalid_pass" style="display: none;"><div class="check_input_false left"></div><span class="error_message left" >Mot de passe invalide !</span></div>
			<div class="clear"></div>
		</p>
		<p class="form-input">
			<label>Confirmation :</label>
			<input type="password" name="user_pwd_conf" id="user_pass_conf" onblur="'.
							'if( this.value == gE(\'user_pass\').value ){'.
								'removeClassName(this.parentNode, \'error\');'.
								'show(gE(\'check_conf\'));'.
								'hide(gE(\'check_conf_alt\'));'.
							'}else{'.
								'addClassName(this.parentNode, \'error\');'.
								'show(gE(\'check_conf_alt\'));'.
								'hide(gE(\'check_conf\'));'.
							'}'.
							
							'this.onkeyup = function(){'.
								'if( this.value == gE(\'user_pass\').value ){'.
									'removeClassName(this.parentNode, \'error\');'.
									'show(gE(\'check_conf\'));'.
									'hide(gE(\'check_conf_alt\'));'.
								'}else{'.
									'addClassName(this.parentNode, \'error\');'.
									'show(gE(\'check_conf_alt\'));'.
									'hide(gE(\'check_conf\'));'.
								'}'.
							'}'.

			'" />
			<div id="check_conf" class="check_input left" style="display: none;"></div>
			<div id="check_conf_alt" style="display: none;">
				<div class="check_input_false left"></div>
				<span class="error_message">Mots de passe différents !</span>
			</div>
			<div class="clear"></div>
		</p>
		<p class="form-submit"><input type="submit" value="Modifier" /></p>
		</form>';
		$page->moduleFooter();
	}

	public function displayDestroyAccount(){
		global $page, $core;
		$core->loadClass('box');
		if(!isset($page)) $page = new Page();
		$box = new Box();

		$page->moduleHeader('Détruire mon compte', '');
		
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/admin_account.php').'" id="form_destroy_account" onsubmit="'.
			$box->confirm('AJAX.submitAndUpdate(gE(\'form_destroy_account\'), false,\'admin_account_content\');', 'Etes-vous bien sûr de vouloir détruire votre compte ?').
			'return false;'.
		'" >
		<input type="hidden" name="wope_action" value="destroy_account" />
		<input type="hidden" name="id_admin" value="'.$this->id.'" />
		<p class="form-input">
			<label>Identifiant :</label>
			<input type="text" name="user_login" />
		</p>
		<p class="form-input">
			<label>Mot de passe :</label>
			<input type="password" name="user_pass" />
		</p>
		<p class="form-submit"><input type="submit" value="Détruire" /></p>
		</form>';
	
		$page->moduleFooter();
	}

	public function destroyAccount($infos){
		global $core, $db, $page;
		if(!isset($page)) $page = new Page();
		
		if( stripslashes($infos['user_pass']) == $core->user->getPass() && stripslashes($infos['user_login']) == $core->user->getLogin() ){
		
			if( $db->query("DELETE FROM usersInfo WHERE id='".$core->getUserId()."' ") ){
				$page->msgInfo('Votre compte a été détruit avec succès. Cliquez <a href="'.$core->makeUrl('index.php?logout').'">ici</a> pour revenir à la <a href="'.$core->makeUrl('index.php?logout').'">page d\'accueil</a>.');
				session_unset();
		    session_destroy();
			}else{
				$page->msgError('Une erreur est apparue pendant la suppression de votre compte. Veuillez contacter l\'administrateur à l\'adresse suivante : <a href="mailto:'.$core->getConfig('mailWebmaster').'">'.$core->getConfig('mailWebmaster').'</a>');
			}
			
		}else{
			$page->msgError('Le couple identifiant/mot de passe fourni n\'est pas valide.');
		}
		
	}

	public function saveUserPassword($post){
		global $db,$core,$page;
		if(!isset($page)) $page = new Page();
		$form = new Form();

		$oldPass = stripslashes($post['user_old_pwd']);
		$newPass = stripslashes($post['user_pwd']);
		$newPassConf = stripslashes($post['user_pwd_conf']);
		
		if( $oldPass == $core->user->getPass() ){
		
			if( $newPass == $newPassConf ){
				if( $form->verifForm($newPass, 'password') ){
		
					if( $db->query("UPDATE usersInfo SET pass=PASSWORD('".$db->escapeString($newPass)."') WHERE id='".$core->getUserId()."' ") ){
					
						//exec('sh '.$core->getConfig('baseServer').'/include/scripts/change_mail_password.sh \''.$core->getLogin().'\' \''.$core->getConfig('local').'\' \''.$newPass.'\'');
						$core->user->saveInformations($core->getLogin(), $newPass);
						$core->saveInSession();
						$page->msgInfo('Mot de passe enregistré. La modification est active dès à présent.');
						
					}else{
						$page->msgError('La modification de votre mot de passe a échoué.');
					}
			
				}else{
					$page->msgError('Votre mot de passe est invalide.');
					$this->displayChangePassword();
				}
			}else{
				$page->msgError('Les mots de passe ne correspondent pas.');
				$this->displayChangePassword();
			}
			
		}else{
			$page->msgError('Vous avez commis une erreur en entrant votre mot de passe actuel.');
		}
		
	}
	
/**** End of account admin functions ****/


/**** Preferences admin functions ****/

	private function adminPrefMenu(){
		global $db,$page;
		if(!isset($page))$page = new Page();
		$page->moduleHeader('Sélectionnez une préférence :','');
	
		//List of preferences who need subtitles
		$sub_titles = Array('modules');
		//list is constructed recursively depending on desired subtitles
		$this->displayPrefTitles($sub_titles);
		
		$page->moduleFooter();
	}

	private function displayPrefTitles($sub_titles = Array()){
		global $core,$db;

		$old_title = $title ='';
		echo '<ul class="panel tree tree_prefs">';
		
  	$result = $db->query("SELECT pref_name FROM prefs_liste WHERE display='Y' ORDER By pref_name ASC");
		
		while ($row = $db->fetchArray($result)){
			$pref_name = $row['pref_name'];
		
			$sub_prefs = split("_", $pref_name);
			$title = $sub_prefs[0];

			//if title is a new title
			if($title != $old_title){
				//if title has subtitles
				if( in_array($title,$sub_titles) && isset($sub_prefs[1]) ){
					$rand_id = rand();
					echo '<li class="tree_element"><a class="expand_tree" onclick="display(gE(\''.$rand_id.'\'));this.firstChild.data=((this.firstChild.data == \'-\')?\'+\':\'-\')" >-</a><span class="user_right">'.ucfirst($title).'</span></li>';
					echo '<ul id="'.$rand_id.'" class="tree sub_tree">';
					
					$sub_result = $db->query("SELECT pref_name FROM prefs_liste WHERE display='Y' AND pref_name LIKE '".$title."%' ORDER By pref_name ASC");
					
					$sub_list = Array();
					while($sub_row = $db->fetchArray($sub_result) ){
						$tmp = split('_',str_replace($title.'_', '', $sub_row['pref_name']));
						$sub_list[] = $tmp[0];
					}
				
					$old_subtitle = '';
					foreach($sub_list as $subtitle){
						if($old_subtitle != $subtitle){
							$sweet_title = stripslashes( $db->result($db->query("SELECT title FROM kernel_module_module WHERE class = 'module_".$subtitle."' "), 0) );
							echo '<li class="tree_element"><a class="user_right" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_pref.php').'?id_admin='.$this->id.'&wope_action=display_user_pref&pref='.$title.'_'.$subtitle.'\',\'admin_pref_content\')">'.ucfirst($sweet_title).'</a></li>';
						}
						$old_subtitle = $subtitle;
					}
					echo '</ul>';
				}
				else{
					echo '<li class="tree_element"><a class="user_right" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_pref.php').'?id_admin='.$this->id.'&wope_action=display_user_pref&pref='.$title.'\',\'admin_pref_content\')">'.ucfirst($title).'</a></li>';
				}
			}//end if
			
			$old_title = $title;
		}//end if
		echo '</ul>';
					
	}

	private function adminPrefContent($content = 'default'){
		switch($content){
			case '':
			case 'default':
				$this->displayPrefDefaultContent();
				break;
			default:
				$this->displayPrefContent($content);
				break;
		}

	}

	public function displayPrefDefaultContent(){
		global $page;
		if(!isset($page))$page = new Page();
		$page->msgInfo('Veuillez choisir une action dans le menu.');		
	}

	public function displayPrefContent($pref){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		
		//construction of module header
		$pref_tab = split('_', $pref);

		if( isset($pref_tab[1]) ){
			$module_header = stripslashes( $db->result($db->query("SELECT title FROM kernel_module_module WHERE class = 'module_".str_replace('_',' ',$pref_tab[1])."' "), 0) );
		}else{
			$module_header = str_replace('_',' ',$pref_tab[0]);
		}
		
		$page->moduleHeader($module_header,'');
		
		$result = $db->query("SELECT pref_name,pref_value,pref_title,description,type,display FROM prefs_liste WHERE display='Y' AND pref_name LIKE '".$pref."%' ORDER By pref_name ASC");

		echo '<form id="frm_user_prefs" action="'.$core->makeUrl($this->path.'/ajax/admin_pref.php').'" onsubmit="'.
			'AJAX.submitAndUpdate(this, false, \'admin_pref_content\');'.
			'return false;'.
		'" >
  	<input type="hidden" name="wope_action" value="save_user_prefs" />
		<input type="hidden" name="id_admin" value="'.$this->id.'" />';

		////here : old prefs.php code is used and a little improved////
  	$titre='';
  	$titre_final='';
		while ($row = $db->fetchArray($result)){
			
	   	$titre_pref=split("_", $row['pref_name']);
			
		 	//is pref name a compound name ?
      if ( sizeof($titre_module=split("-", $titre_pref[1])) >1 ){
       	$titre_final="";
       	for ($i=0;$i<sizeof($titre_module);$i++)
       	{
         	$titre_final.=($i==0?ucfirst($titre_pref[0]):'').ucfirst($titre_module[$i])." ";
       	}
      }
    	else{
		    $titre_final=$titre_pref[0];
		  }

			//is it a new part
    	if ($titre!=$titre_final){
      	if ($titre!='')
      	{
					//previous part is closed
        	echo '<p class="form-submit"><input type="submit" value="Enregistrer" /></p>';
      	}
      	$titre=$titre_final;
    	}
		
    	echo '<p class="form-input" '. ( $row['type'] != 'select' ? 'onmouseover="'.$page->htmlBlock->tooltip(stripslashes($row['description'])).'"' : '' ) .'>';
    	//Id de l'input pour l'utilisation de LABELs
    	$idInput = preg_replace('/[^a-z]/', '', strtolower($row['pref_name']));
		
    	switch ($row['type']){
      case 'text':
      case 'integer':
	      echo '<label for="'. $idInput .'">'.$row['pref_title'].' :</label>
  	    <input type="text" id="'. $idInput .'" name="pref['.$row['pref_name'].']" size="20" maxlength="255" value="'.$core->user->getMyPref( $row['pref_name']).'" />';
    	  break;

      case 'timestamp':
	      echo '<label for="'. $idInput .'">'.$row['pref_title'].':</label>
	      <input type="text" id="'. $idInput .'" name="pref['.$row['pref_name'].']" size="20" maxlength="255" value="'.date("H:i:s d/m/Y",$core->user->getMyPref( $row['pref_name'])).'" />';
	      break;

      case 'checkbox':
	      echo '<input type="checkbox" name="" value="true" />'.$row['pref_title'];
  	    break;

      case 'raccourcis':
	      $configKey = $core->user->getMyPref($row['pref_name']);
	      echo '<label for="'. $idInput .'">'.$row['pref_title'].' :</label>
	      <select id="'. $idInput .'" name="pref['.$row['pref_name'].']">';
	      $alpha = "abcdefghijklmnopqrstuvwxyz";
	      for($i = 0;$i < 26; $i++){ 
        	if($configKey == $alpha[$i]){
          	echo '<option selected="selected">'.$alpha[$i].'</option>
						';
        	}
					else{
          	echo '<option>'.$alpha[$i].'</option>
          	';
	      	}
				}
  	    echo '</select>';
    	  break;

      case 'select':
	      echo '<label for="'.$idInput.'" onmouseover="'.$page->htmlBlock->tooltip(stripslashes($row['description'])).'">'.$row['pref_title'].' :</label>
  	    <select id="'.$idInput.'" name="pref['.$row['pref_name'].']">';

				//search of possible values
      	$result2=$db->query("SELECT pvals_name,pvals_value FROM prefs_values WHERE pvals_name='".$row['pref_name']."' ORDER BY pvals_value ASC");

      	while ($liste=$db->fetchArray($result2)){
        	//pense-bete module preferences : need translation
	        if ($liste['pvals_name']=="modules_pense-bete_coul_tres_urgent" || $liste['pvals_name']=="modules_pense-bete_coul_urgent" || $liste['pvals_name']=="modules_pense-bete_coul_peu_urgent")
  	      {
    	      //selected option is user current option
      	    if ($core->user->getMyPref($row['pref_name']) == $liste['pvals_value']){
        	    $sel=' selected="selected" ';
	          }
  	        else{
    	        $sel='';
	          }
  	        echo '<option value="'. $liste['pvals_value'].'" '.$sel.' >'.locale::display($liste['pvals_value'],'colors').'</option>';
	        }
	        else
	        {
	          //selected option is user current option
						if ($core->user->getMyPref($row['pref_name']) == $liste['pvals_value']){
	            $sel=' selected="selected" ';
	          }
	          else{
 	           $sel='';
	          }
	          echo '<option value="'. $liste['pvals_value'].'" '.$sel.' >'.$liste['pvals_value'].'</option>';
	        }
	      }
  	    echo '</select>';
    	  break;

      case 'radio':
	      echo '<label>
  	    '.$row['pref_title'].':
    	  </label>';
	      if ($core->user->getMyPref( $row['pref_name']) == 'true'){
	        $pref_true = ' checked="checked" ';
  	      $pref_false = '';
    	  }
      	else{
	        $pref_true = '';
	        $pref_false = ' checked="checked" ';
	      }
	      echo '<input type="radio" name="pref['.$row['pref_name'].']" value="true" '.$pref_true.' /><span style="position: relative">Oui</span>
	      <input type="radio" name="pref['.$row['pref_name'].']" value="false" '.$pref_false.' /><span style="position: relative">Non</span>';
      	break;
				
      case 'textarea':
	      echo '<label for="'. $idInput .'">'.$row['pref_title'].' :</label>
  	    <textarea id="'. $idInput .'" name="pref['.$row['pref_name'].']" cols="38">'.$core->user->getMyPref( $row['pref_name']).'</textarea>';
    	  break;
    	}//switch end
    	echo '</p>';
  	}//while end
  	echo '<p class="form-submit"><input type="submit" value="Enregistrer" /></p>
		</form>';
	
		////end of old code////

		$page->moduleFooter();
	}

	public function saveUserPrefs($prefs){
		global $core;
		$page = new Page();
		
		if(isset($prefs) && is_array($prefs)){
			foreach($prefs as $key => $val){
				$core->user->setPref($core->getUserId(), $key, $val);
			}
			$page->msgInfo('Préférences sauvegardées');
		}
	}
	
/**** End of preferences admin functions ****/

/*** Start preferences category function ***/
	public function listCategories() {
		global $core,$db;
		$core->loadClass("tag");
		$tag = new Tag(array(),$core,$db);
		
		$listCategory = $tag->getListCategory();
		echo "<div id=list_category>";
		echo "<table width='80%' align='center' class='data_table'>";
		echo "<tr>";
		echo "<th width='15%'>Voir</th>";
		echo "<th>Nom</th>";
		echo "<th>Description</th>";
		echo "</tr>";
		foreach ($listCategory as $category) {
			if ($category['isShow'] == 'true') {
				$style = '';
			} else {
				$style = ' no_active ';
			}
			echo '<tr class="'.$style.'">';
			echo '<td align="center">';
			$url = $core->makeUrl($this->path.'/ajax/admin_category.php')."?id_admin=".$this->id."&amp;id=".$category['id']."&amp;wope_action=";

			if ($category['isShow'] == 'true') {
				$checked = ' checked="checked" ';
				$action = 'hide';
			}else{
				$checked = '';
				$action = 'show';
			}
			
			echo '<input type="checkbox" '.$checked.' onclick="AJAX.getAndUpdate(\''.$url.$action.'_category\',\'list_category\');" />';
			echo "</td>";
			echo "<td>".$category['name']."</td>";
			echo "<td>".$category['description']."</td>";
			echo "</tr>";
		}
		echo "</table>";
		echo "</div>";
	}
	
	public function showCategory ($id) {
		global $core,$db;
		$core->loadClass("tag");
		$tag = new Tag(array(),$core,$db);
		$tag->showCategory($id);
		$this->listCategories();
	}
	
	public function hideCategory ($id) {
		global $core,$db;
		$core->loadClass("tag");
		$tag = new Tag(array(),$core,$db);
		$tag->hideCategory($id);
		$this->listCategories();
	}

/*** End preferences category function ***/
}


?>
