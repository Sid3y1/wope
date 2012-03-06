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

class Admin {

	/**
	 * \brief  Contents administration's panel configuration
	*/
	private $config;
	
	/**
	 * \brief  Contents apps which have admin
	*/
	private $config_apps;
	
	/**
	 * \brief Contents apps, modules and others, 
	 *  which have a summary in home part
	*/
	private $config_summaries;

	/**
	 * \brief ID of this object, used to call sensible functions
	*/
	private $id;

	/**
	 * \brief Link between name and id
	 */
	private $rightCache=null;

	/**
	 * \brief Path to access this object
	*/
	private $path;
	
	public final function __construct(){
		include('config_admin.inc');
		$this->config_apps = $config_apps;
		$this->config_summaries = $config_summaries;
		$this->config = $config_admin;
		$this->path = $this->config['path'];
		
		$this->id = rand(1000000,9999999);
		$_SESSION['admin_panel_'.$this->id] = serialize($this);
	}

	/**
	 * \brief Calls admin of a part
	 *
	 * \param  $part    part to display (default : home)
	 * \param  $content (optionnal) content of the part
	 *
	 * \return void
	*/
	public function displayAdmin($part = '', $content = '', $option = null){
		global $core, $page;

		if( $core->verifDroits($this->config['parts'][$part]['rights']) ){
			$display_part = 'displayAdmin'.ucfirst(strtolower($part));
			switch($display_part){
				case 'displayAdmin':
					$this->displayAdminHome();
					break;
				default:
					$this->$display_part($content, isset($option) ? $option : null );
					break;
			}
			
		}else{
			$this->displayRightError();
		}
	}

	private function displayRightError(){
		global $page;
		
		$page->msgError('Vous ne possédez pas les droits requis pour accéder à cette page.');
	}


/**** Below this line are functions called by displayAdmin() to display parts ****/

	/**
	 * displayAdminHome() displays administration's home page
	 * @return void
	*/
	private function displayAdminHome(){
		global $core;
		
		$summaries_nb = count($this->config_summaries);
		$summaries1 = intval($summaries_nb / 2);
		$summaries2 = $summaries1 + $summaries_nb % 2;
		
	  echo '<div class="admin_column1">';
		$this->displayAdminNotes();

		for($i = 0; $i<$summaries1; $i++){
			switch($this->config_summaries[$i]['source']){
				case 'admin':
					$method = 'admin'.ucfirst(strtolower($this->config_summaries[$i]['part'])).'Summary';
					$this->$method($this->config['summary_rows_nb'], $this->config['summary_period']);
					break;
					
				case 'module':
					include_once('../modules/'.$this->config_summaries[$i]['module'].'/admin/'.$this->config_summaries[$i]['module'].'_admin.class.inc');
					$toload = $this->config_summaries[$i]['module'].'_admin';
					$module_summary = new $toload();
					$module_summary->displaySummary($this->config['summary_rows_nb'], $this->config['summary_period']);
					break;
					
				case 'app':
					include_once('../'.$this->config_summaries[$i]['app']['url'].'/'.$this->config_summaries[$i]['app']['class'].'.class.inc');
					$toload = $this->config_summaries[$i]['app']['class'];
					$app_summary = new $toload();
					$app_summary->displaySummary($this->config['summary_rows_nb'], $this->config['summary_period']);
					break;
				
				default:
					//empty
					break;
			}
		}
		echo '</div>';
		echo '<div class="admin_column2">';
		for($i = 0; $i<$summaries2; $i++){
			$j = $summaries1 + $i;
			switch($this->config_summaries[$j]['source']){
				case 'admin':
					$method = 'admin'.ucfirst(strtolower($this->config_summaries[$j]['part'])).'Summary';
					$this->$method($this->config['summary_rows_nb'], $this->config['summary_period']);
					break;
					
				case 'module':
					include_once('../../modules/'.$this->config_summaries[$j]['module'].'/admin/'.$this->config_summaries[$j]['module'].'_admin.class.inc');
					$toload = $this->config_summaries[$j]['module'].'_admin';
					$module_summary = new $toload();
					$module_summary->displaySummary($this->config['summary_rows_nb'], $this->config['summary_period']);
					break;
					
				case 'app':
					include_once('../'.$this->config_summaries[$j]['app']['url'].'/'.$this->config_summaries[$j]['app']['class'].'.class.inc');
					$toload = $this->config_summaries[$j]['app']['class'];
					$app_summary = new $toload();
					$app_summary->displaySummary($this->config['summary_rows_nb'], $this->config['summary_period']);
					break;
					
				default:
					//empty
					break;
			}
		}
	
		echo '</div>';
	}

	/**
	 * displayAdminGeneral() displays general preferences
	 * @return void
	*/
	private function displayAdminGeneral(){
		global $core;
		echo '<div class="admin_content">';
		if ($core->verifDroits($this->config['critical_rights'])){
			$this->displayMenuPrefs();
		}
		$this->displaySkinPrefs();
		echo '</div>';
	}

	/**
	 * displayAdminRight() display rights administration
	 * @return void
	*/
	private function displayAdminRight($content='default'){
		global $page,$core;
		if(!isset($page))$page=new Page();
		echo '<div class="admin_menu1">';
		$page->moduleHeader('Administrer le droit','');
		$this->displayRightsList();
		echo '<form><p class="form_submit"><input type="button" value="Menu droits" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_menu.php').'?part=right\',\'admin_right_content\')" /></p></form>';
		$page->moduleFooter();
		echo '</div>';
		echo '<div class="admin_menu2" id="admin_right_content" >';
		$this->adminRightMenu($content);
		echo '</div>';
	}

	/**
	 * displayAdminApp() displays apps administration
	 * @param  $content content to display
	 * @return void
	*/
	private function displayAdminApp($content='default', $option = null){
		global $core,$page;
		if(!isset($page))$page = new Page();

		foreach($this->config_apps as $app){
			$core->javascriptCore->loadContextScripts($app['url'], '..', $app['class']);
			include( dirname(__FILE__) . '/../../'.  str_replace('/admin', '', $app['url']) . '/config_'.str_replace('_admin', '', $app['class']).'.inc' );
		}

		if( !isset($this->config_apps[$content]) ){
			$content='default';
		}else{
			$cnf = 'config_'.$content;
			$cnf = $$cnf;
		}
		
		echo '<div class="admin_menu1">';
		$page->moduleHeader('Administrer l\'application :','');
		echo '<form onsubmit="'.
				'if(this.app_select.selectedIndex >= 1){'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_content.php').'?part=app&content=\'+this.app_select.options[this.app_select.selectedIndex].value,\'admin_app_content\');'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_menu.php').'?part=app&content=\'+this.app_select.options[this.app_select.selectedIndex].value,\'admin_app_menu\');'.
					'this.reset();'.
					'return false;'.
				'}else{'.
					'return false;'.
			'}">';
		echo '<p class="form-input"><select name="app_select">';
		echo '<option selected="selected" disabled="disabled">Sélectionner une application</option>';
		foreach($this->config_apps as $name => $param){
			$conf = 'config_'.str_replace('_admin', '', $param['class']);
			$conf = $$conf;
			
			if( isset( $conf['admin_rights'] ) && $core->verifDroits( $conf['admin_rights'] ) ){
		  	echo '<option value="'.$name.'">'.$param['title'].'</option>';
			}
		}
		echo '</select></p>';
		echo '<p class="form-submit"><input type="submit" value="Afficher" /></p>';
		echo '</form>';
		$page->moduleFooter();
		echo '</div>';

		echo '<div id="admin_app_menu" class="admin_menu2">';
		
		if( $content == 'default' || $core->verifDroits($cnf['admin_rights']) ){
			//second argument is a bad way to load classes in adminAppMenu() depending on call place : here or in an ajax page
			$this->adminAppMenu($content, isset($option) ? $option : null);
		}else{
			$this->displayRightError();
		}
		
		echo '</div>';
		echo '<div class="clear"></div>';
		echo '<div id="admin_app_content" class="admin_content">';
		
		if( $content == 'default' || $core->verifDroits($cnf['admin_rights'])){
			//second argument is a bad way to load classes in adminAppContent() depending on call place : here or in an ajax page
			$this->adminAppContent($content, isset($option) ? $option : null);
		}
		
		echo '</div>';
	}

	/**
	 * displayAdminModule() displays modules administration
	 * @param  $content content to display
	 * @return void
	*/
	private function displayAdminModule($content='default', $option = null){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		
		$moduleListe = Array();
		//$test is useful to test if $content is valid
		$modules_admin = $db->query("SELECT class, title FROM kernel_module_module WHERE admin_available = 'Y' ORDER BY title ASC");
		while($mod = $db->fetchArray($modules_admin)){
			include( dirname(__FILE__) . '/../../modules/'. $mod['class'] . '/config_'.$mod['class'].'.inc' );
			$moduleListe[] = Array ('title'=>stripslashes($mod['title']), 'class'=>$mod['class']);
			$test[] = $mod['class'];
		}
		
		if(!in_array($content,$test)){
			$content='default';
		}else{
			$cnf = 'config_'.$content;
			$cnf = $$cnf;
		}
		
		//Selection of module admin
		echo '<div class="admin_menu1">';
		$page->moduleHeader('Administrer le module :','');
		echo '<form onsubmit="'.
			'if(this.module_select.selectedIndex >= 1){'.
				'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_content.php').'?part=module&content=\'+this.module_select.options[this.module_select.selectedIndex].value,\'admin_module_content\');'.
				'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_menu.php').'?part=module&menu=\'+this.module_select.options[this.module_select.selectedIndex].value,\'admin_module_menu\');'.
				'this.reset();'.
				'return false;'.
			'}else{'.
				'return false;'.
			'}" >';
		//On chang, an ajax page is loaded and call the display method of module admin
		echo '<p class="form-input"><select name="module_select">';
		echo '<option selected="selected" disabled="disabled">Sélectionner un module</option>';
		foreach($moduleListe as $mod){
			$conf = 'config_'.$mod['class'];
			$conf = $$conf;
			
			if( isset( $conf['admin_rights'] ) && $core->verifDroits( $conf['admin_rights'] ) ){
	  		echo '<option value="'.$mod['class'].'">'.$mod['title'].'</option>';
			}
		}
		echo '</select></p>';
		echo '<p class="form-submit"><input type="submit" value="Afficher" /></p>';
		echo '<p class="form-submit"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_module.php').'?ida='.$this->id.'&ga=dml\',\'admin_module_content\');AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_menu.php').'?part=module&menu=default\',\'admin_module_menu\')">Revenir au menu des modules</a></p>';
		echo '</form>';
		$page->moduleFooter();
		echo '</div>';

		//Menu of module admin
		echo '<div id="admin_module_menu" class="admin_menu2">';
		
		if( $content == 'default' || $core->verifDroits($cnf['admin_rights']) ){
			$this->adminModuleMenu($content, isset($option) ? $option : null);
		}else{
			$this->displayRightError();
		}

		echo '</div>';
		echo '<div class="clear"></div>';

		//Content of module admin
		echo '<div id="admin_module_content" class="admin_content">';
		
		if( $content == 'default' || $core->verifDroits($cnf['admin_rights']) ){
			//second argument is a bad way to load classes in adminModuleContent() depending on call place : here or in an ajax page
			$this->adminModuleContent($content, isset($option) ? $option : null);
		}
		
		echo '</div>';
	}


	/**
	 * displayAdminUser() displays users administration
	 * @return void
	*/
	private function displayAdminUser($user_pattern='none', $option=''){
		global $core,$page;
		if(!isset($page))$page = new Page();
		
		//Selection of user to manage
		echo '<div class="admin_menu1">';
		$page->moduleHeader('Administrer l\'utilisateur :','');
		echo '<form id="user_select" onsubmit="'.
			'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_result&option=search&q=\'+gE(\'user_pattern\').value,\'admin_user_content\');'.
			'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_menu&option=search&q=\'+gE(\'user_pattern\').value,\'admin_user_menu\');'.
			'this.reset();'.
			'return false">';
		echo '<p class="form_input"><input type="texte" id="user_pattern" name="user_pattern" /><input type="submit" value="Chercher" /></p>';
		echo '<p class="form-submit"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_result&q=all_users\',\'admin_user_content\');AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_menu\',\'admin_user_menu\')">Revenir au menu des utilisateurs</a></p>';
		echo '</form>';
		$page->moduleFooter();
		echo '</div>';

		//Menu of user admin
		echo '<div id="admin_user_menu" class="admin_menu2">';
		$this->adminUserMenu($user_pattern, $option);
		echo '</div>';
		echo '<div class="clear"></div>';

		//Content of user admin
		echo '<div id="admin_user_content" class="admin_content">';
		$this->adminUserContent($user_pattern, $option);
		echo '</div>';
	}

	/**
	 * displayAdminCategory() displays stats of the administration page
	 * @return void
	*/
	private function displayAdminCategory(){
		global $core,$page;
		echo '<div class="admin_content">';
		$page->moduleHeader('Administrer les categories','');
		switch ($_GET['wope_action']) {
			case 'addCategory' :
				$this->addCategory();
				$this->listCategory();
				break;
			case 'showAddCategory' :
				$this->showAddCategory();
				break;
			case 'updateCategory' :
				$this->updateCategory();
				$this->listCategory();
				break;
			case 'showUpdateCategory' :
				$id=(int)$_GET['id'];
				$this->showUpdateCategory($id);
				break;
			case 'delCategory' :
				$id=(int)$_GET['id'];
				$this->deleteCategory($id);
				$this->listCategory();
				break;
			case 'listCategory':
			default :
				$this->listCategory();
				break;	
		}
		$page->moduleFooter();
		echo '</div>';
	}

	/**
	 * displayAdminStat() displays stats of the administration page
	 * @return void
	*/
	private function displayAdminStat(){
		global $core,$page;
		if(!isset($page))$page = new Page();
		echo '<div class="admin_column1">';
		$this->displayStatTools();
		$this->displayLastUsers();
		echo '</div>';
		echo '<div class="admin_column2">';
		$this->displayMostUsedModules();
		echo '</div>';
	}

/**** End of parts display functions ****/	


/**** Home admin functions ****/

	/**
	 * displayAdminNotes() display notepad
	 * @return void
	*/
	private function displayAdminNotes(){
		global $core,$page;
		if(!isset($page))$page = new Page();
		$core->loadClass('module_notes');
		$notes = new Module_notes(true);
		$page->moduleHeader('Notes','');
		$notes->display();
		$page->moduleFooter();
	}


/**** End of home admin functions ****/


/**** General admin functions ****/

	public function displayMenuPrefs(){
		global $core, $page;
		if(!isset($page))$page = new Page();
		$page->moduleHeader('Menu','');
		echo '<div class="centrage"><span class="panel_h mini"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=display_add_menu_entry\',\'menu_prefs\')">Ajouter une entrée</a> | ';
		echo '<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=display_menu_list\',\'menu_prefs\')">Voir les entrées du menu</a> | ';
		echo '<a onclick=\''.
			'AJAX.getAndCall("'.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=empty_menu_cache",'.
			'function(){var el = gE("empty_cache_feedback");show(el);}'.
		')\'>Vider les caches</a></span></div>';

		echo '<div id="empty_cache_feedback" style="display: none;">';
		$page->msgInfo('Cache supprimé');
		echo '</div>';
		
		echo '<div id="menu_prefs">';
		$this->displayMenuList();
		echo '</div>';
		$page->moduleFooter();
	}

	public function displayMenuList(){
		global $core, $db, $page;
		if(!isset($page))$page = new Page();
		$core->loadClass('box');
		$box = new Box();
		
		echo '<div style="display:none">';
		$page->msgInfo('Modifications effectuées.');
		echo '</div>';
		$result = $db->query("SELECT ksm.id_menu, ksm.href, ksm.name, ksm.image, kr.name as permission_name ,ksm.permission, ksm.valid, ksm.rank FROM kernel_skin_menu ksm, kernel_right kr WHERE kr.id=ksm.permission ORDER BY ksm.rank");
		echo '<table align="center" class="data_table" width="95%">
			<tr>
				<th width="20%">Nom</th>
				<th width="30%">URL</th>
				<th width="20%">Image</th>
				<th width="20%">Droit</th>
				<th width="10%">Affichage</th>
				<th widht="5%">Suppression</th>
			</tr>';
		while($row = $db->fetchArray($result)){
			$class = '';
			if($row['valid'] == 'N') $class = ' class="no_valid" ';
			echo '<tr '.$class.' >
			<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=display_update_menu_entry&id_entry='.$row['id_menu'].'\',\'menu_prefs\')">'.stripslashes($row['name']).'</a></td>
			<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=display_update_menu_entry&id_entry='.$row['id_menu'].'\',\'menu_prefs\')">'.stripslashes($row['href']).'</a></td>
			<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=display_update_menu_entry&id_entry='.$row['id_menu'].'\',\'menu_prefs\')">'.stripslashes($row['image']).'</a></td>
			<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=display_update_menu_entry&id_entry='.$row['id_menu'].'\',\'menu_prefs\')">'.stripslashes($row['permission_name']).'</a></td>
			<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=validate_menu_entry&valid='.(($row['valid']=='Y')?'N':'Y').'&id_entry='.$row['id_menu'].'\',\'menu_prefs\')">'.$row['valid'].'</a></td>
			<td align="center"><a onclick="'.$box->confirm(
				'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=del_menu_entry&id_entry='.$row['id_menu'].'\',\'menu_prefs\')',
				'Etes-vous sûr de vouloir supprimer ce lien ?'
			).'" class="trash">&nbsp;</a></td>
		</tr>';
		}
		echo '</table>';

	}

	public function displayAddMenuEntry($post = ''){
		global $core,$page,$db;
		if(!isset($page))$page = new Page();
		
		$val = false;
		if(is_array($post)){
			$val = true;
		}
		
		echo '<div id="add_menu_entry_feedback" style="display: none;" >';
    $page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/admin_general.php').'" onsubmit="display(gE(\'add_menu_entry_feedback\'));AJAX.submitAndUpdate(this, false, \'menu_prefs\');return false;" >
			<input type="hidden" name="wope_action" value="save_menu_entry" />
			<input type="hidden" name="save_case" value="add" />
			<input type="hidden" name="id_admin" value="'.$this->id.'" />
			<p class="form-input"><label>Nom :</label><input type="text" name="entry_name" value="'.($val ? $post['entry_name'] : '').'" /></p>
			<p class="form-input"><label>URL :</label><input type="text" name="entry_url" value="'.($val ? $post['entry_url'] : '').'" /></p>
			<p class="form-input"><label>Image :</label><input type="text" name="entry_image" value="'.($val ? $post['entry_image'] : '').'" /></p>
			<p class="form-input"><label>Droit :</label>
				<select name="entry_permission">';
		        $res = $db->query('SELECT id , name FROM kernel_right');
		        while($permission = $db->fetchArray($res)){
		          echo '<option value="'.$permission['id'].'">'.$permission['name'].'</option>';
		        }
				
			echo '</select></p>
			<p class="form-input"><label>Rang :</label><input type="text" name="entry_rank" value="'.($val ? $post['entry_rank'] : '').'" /></p>
			<p class="form-radio"><label>Afficher l\'entrée dès maintenant : &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;</label>
				<input type="radio" name="display_entry" value="Y" '.($val ? ($post['display_entry']=='Y' ? 'checked="checked"' : '') : 'checked="checked"').' /><span> Oui &nbsp; &nbsp; &nbsp; &nbsp;</span>
				<input type="radio" name="display_entry" value="N" '.(($val && $post['display_entry']=='N') ? 'checked="checked"' : '').' /><span> Non</span>
			</p>
			<p class="form-submit">
				<input type="submit" value="Ajouter" />
				<input type="button" value="Annuler" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=display_menu_list\', \'menu_prefs\');" />
			</p>
		</form>';	
	}

	public function saveMenuEntry($infos){
		global $db;
		$page = new Page();

		if($infos['entry_url'] != '' && $infos['entry_name'] != ''){
			switch($infos['save_case']){
				case 'add':
					$db->query("INSERT INTO kernel_skin_menu (href,name,rank,image,permission,valid) VALUES ('".$db->escapeString($infos['entry_url'])."','".$db->escapeString($infos['entry_name'])."','".$db->escapeString($infos['entry_rank'])."','".$infos['entry_image']."','".$infos['entry_permission']."','".$infos['display_entry']."')");
					$id_menu = $db->insertId();
					break;
				case 'update':
					$db->query("UPDATE kernel_skin_menu SET 
								href='".$db->escapeString($infos['entry_url'])."',
								name='".$db->escapeString($infos['entry_name'])."',
								rank='".$db->escapeString($infos['entry_rank'])."',
								image='".$db->escapeString($infos['entry_image'])."',
								permission='".$db->escapeString($infos['entry_permission'])."',
								valid='".$infos['display_entry']."'
								WHERE id_menu='".$infos['id_entry']."'");
					break;
				default:
					//empty
					break;
			}
			
			$this->emptyMenuCache();
			$this->displayMenuList();
			
		}else{
			$page->msgError('Vous devez fournir un nom et une URL à votre lien.');
			switch($infos['save_case']){
			  case 'add':
					$this->displayAddMenuEntry($infos);
				 	break;
				case 'update':
					$this->displayUpdateMenuEntry($infos['id_entry']);
					break;
				default:
					//empty
					break;
			}
		}
	}
	
	public function emptyMenuCache(){
		global $core;
		
		$dir = opendir($core->getConfig('cacheServer'));
	
	 	while ($f = readdir($dir)){
			@unlink($core->getConfig('cacheServer').'/menu/'.$f);
		}
	}
	
	public function displayUpdateMenuEntry($id_entry){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		
		$result = $db->query("SELECT name, valid, href, permission, image,rank
							 FROM kernel_skin_menu
							 WHERE id_menu='".$id_entry."'");
		echo '<div id="update_menu_entry_feedback" style="display:none" >';
    $page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		while($row = $db->fetchArray($result)){
			echo '<form action="'.$core->makeUrl($this->path.'/ajax/admin_general.php').'" onsubmit="display(gE(\'update_menu_entry_feedback\'));AJAX.submitAndUpdate(this, false, \'menu_prefs\');return false;" >
				<input type="hidden" name="wope_action" value="save_menu_entry" />
				<input type="hidden" name="save_case" value="update" />
				<input type="hidden" name="id_admin" value="'.$this->id.'" />
				<input type="hidden" name="id_entry" value="'.$id_entry.'" />
				<p class="form-input"><label>Nom :</label><input type="text" name="entry_name" value="'.stripslashes($row['name']).'" /></p>
				<p class="form-input"><label>URL :</label><input type="text" name="entry_url" value="'.stripslashes($row['href']).'" /></p>
				<p class="form-input"><label>Image :</label><input type="text" name="entry_image" value="'.stripslashes($row['image']).'" /></p>
				<p class="form-input"><label>Droit :</label>
					<select name="entry_permission">';
		        $res = $db->query('SELECT id , name FROM kernel_right');
		        while($permission = $db->fetchArray($res)){
		          echo '<option '.(($row[permission]==$permission['id'])?'selected ':'').'value="'.$permission['id'].'">'.$permission['name'].'</option>';
		        }
				
			echo '</select></p>
				<p class="form-input"><label>Rang :</label><input type="text" name="entry_rank" value="'.stripslashes($row['rank']).'" /></p>
				<p class="form-input"><label>Afficher :</label>
					<input type="radio" name="display_entry" value="Y" '.(($row['valid']=='Y')?'checked="checked"':'').' /><span>Oui</span>
					<input type="radio" name="display_entry" value="N" '.(($row['valid']=='N')?'checked="checked"':'').'/><span>Non</span>
				</p>
				<p class="form-submit">
					<input type="submit" value="Modifier" />
					<input type="button" value="Annuler" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=display_menu_list\', \'menu_prefs\');" />
				</p>
			</form>';
		}
	}
	
	public function deleteMenuEntry($id_entry){
		global $db;
		$db->query("DELETE FROM kernel_skin_menu WHERE id_menu='".$id_entry."'");
		$db->query("DELETE FROM kernel_skin_menu WHERE id_menu='".$id_entry."'");
		$this->emptyMenuCache();
		$this->displayMenuList();
	}
	
	public function validateMenuEntry($id_entry, $valid){
		global $db;
		$db->query("UPDATE kernel_skin_menu SET valid='".$valid."' WHERE id_menu='".$id_entry."'");
		$this->emptyMenuCache();
		$this->displayMenuList();
	}
	

	public function updateSkin($new_skin = ''){
		global $core, $db, $page;
		if(!isset($page))$page = new Page();
		switch($new_skin){
			case '':
				//void
				break;
			default:
			  $db->query("UPDATE prefs_liste SET pref_value='".$db->escapeString($new_skin)."' WHERE pref_name='site_skin'");
			  $db->query("UPDATE usersprefs2 SET pref_value='".$db->escapeString($new_skin)."' WHERE pref_name='site_skin'");
			  $core->user->setPref($core->getUserId(), 'site_skin', $new_skin);
				$page->msgInfo('Modification effectuée. Le changement sera effectif à la prochaine connexion.
				<a href="'.$core->makeUrl('index.php?logout').'">Se reconnecter</a>');
				break;
		}
		$this->displayUpdateSkin($new_skin);
	}

	private function displaySkinPrefs(){
		global $page;
		if(!isset($page))$page = new Page();
		$page->moduleHeader('Skins','');
		echo '<div id="default_skin">';
		$this->displayUpdateSkin();
		echo '</div>';
		$page->moduleFooter();
	}

	public function displayUpdateSkin(){
		global $core, $db;
		echo '<form>
					<p class="form-input" style="width: 500px"><label>Skin par défaut du portail :</label>
					<select name="new_skin" id="new_skin">';
		$dir = $core->getConfig('baseServer').'/skins';
		$current_skin = $db->result($db->query("SELECT pref_value FROM prefs_liste WHERE pref_name='site_skin'"),0);
		$skins = $db->query("SELECT pvals_value FROM prefs_values WHERE pvals_name='site_skin'");
	  while ($skin = $db->fetchArray($skins)) {
			$skin_name = stripslashes($skin['pvals_value']);
      echo '<option value="'.$skin_name.'" '.(( $skin_name == stripslashes($current_skin) ) ? ' selected="selected" ' : '').'>'.$skin_name.'</option>';
    }
	  echo '</select>
	  	<input type="button" value="Modifier" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_general.php').'?id_admin='.$this->id.'&wope_action=update_skin&new_skin=\'+gE(\'new_skin\').options[gE(\'new_skin\').selectedIndex].value,\'default_skin\')" />
		</form>';
	}
	
/**** End of general admin functions ****/


/**** Rights admin functions ****/

	public function adminRightMenu($right){
		
		echo '<div id="admin_right_content">';
		switch($right){
			case '':
			case 'default':
				$this->adminRightMenuDefault();
				break;
			default:
				$this->displayRightContent($right);
				break;
		}
		echo '</div>';
	}

	public function adminRightMenuDefault(){
		global $core,$page;
		if(!isset($page))$page=new Page();
		$page->moduleHeader('Droits','');
		echo '<a class="admin_link" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_add_right\',\'admin_right_content\')">Ajouter un droit</a>';
		$page->moduleFooter();
	}
	
	public function displayRightContent($right){
		global $core,$page,$db;
		if(!isset($page))$page=new Page();
		$right_name=htmlentities($right, ENT_QUOTES, "UTF-8");
		$right=(int)$right;
		if ($right_name<>$right) {
			$result = $db->query("SELECT d1.id FROM kernel_right d1 WHERE d1.name='$right_name'");
			$right=$db->result($result,0);
			$this->rightCache[$right] = $right_name;
		} else {
			if ($this->rightCache==null) {
				$result = $db->query("SELECT d1.name FROM kernel_right d1 WHERE d1.id=$right");
				$this->rightCache[$right] = $db->result($result,0);
			}
		}
		$page->moduleHeader('Droit : '.$this->rightCache[$right],'');
		echo '<div id="right_desc">';
		$this->displayRightDescription($right);
		echo '</div>';
		$page->moduleFooter();
		$page->moduleHeader('Administrateurs de '.$this->rightCache[$right],'');
		echo '<div id="admin_users_list">';
		$this->displayAdminUsersList($right);
		echo '</div>';
		echo '<p class="action">
			<a onclick="display(gE(\'frm_add_admin_to_right\'));">
				<span class="titre_menu"><span>D</span>onner ce privilège à un nouvel utilisateur ? </span>
			</a>
		</p>';
		echo '<form class="form_give" style="display: none" id="frm_add_admin_to_right" onsubmit="'.
			'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=add_admin_to_right&right='.$right.'&user=\'+gE(\'new_admin_user\').value,\'admin_users_list\');'.
			'gE(\'frm_add_admin_to_right\').reset();'.
			'return false;'.
		'">
			<p class="form-input">
				<label>ID :</label>
				<input type="text" id="new_admin_user" name="new_admin_user" />
			</p>
			<p class="form-submit">
				<input type="submit" value="Ajouter" />
			</p>
		</form>';
		$page->moduleFooter();
		$page->moduleHeader('Utilisateurs ayant le droit : '.$this->rightCache[$right],'');
		echo '<div id="right_users_list">';
		$this->displayRightUsersList($right);
		echo '</div>';
		echo '<p class="action">
			<a onclick="display(gE(\'frm_add_right_to_user\'));">
				<span class="titre_menu"><span>D</span>onner ce droit à un nouvel utilisateur ?</span>
			</a>
		</p>';
		echo '<form class="form_give" style="display: none" id="frm_add_right_to_user" onsubmit="'.
			'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=add_right_to_user&right='.$right.'&user=\'+gE(\'new_user_to_right\').value,\'right_users_list\');'.
			'gE(\'frm_add_right_to_user\').reset();'.
			'return false;'.
		'">
			<p class="form-input">
				<label>ID :</label>
				<input type="text" id="new_user_to_right" name="new_user_to_right" />
			</p>
			<p class="form-submit">
				<input type="submit" value="Ajouter" />
			</p>
		</form>';
		$page->moduleFooter();
	}

	public function displayRightDescription($right){
		global $db,$core;
		$box = new Box();
		
		//right description
		$desc = $db->result($db->query("SELECT description FROM kernel_right WHERE id='".$right."'"),0);

		echo '<div class="centrage"><span class="panel_h">';
		
		//right udpating
		echo '<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_update_right&right='.$right.'\',\'admin_right_content\')">Modifier le droit</a> | ';
		//right deleting
		echo '<a onclick="'.$box->confirm('AJAX.getAndCall(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=delete_right&right='.$right.'\', function(){AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_right_deleted\',\'admin_right_content\');AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_rights_tree\',\'rights_tree\');});','Etes-vous sûr de vouloir supprimer le droit '.$this->rightCache[$right].' ? Les droits en héritant seront également supprimés!').'">Supprimer '.$this->rightCache[$right].'</a>';
		
		echo'</span></div>';

		if($desc == '') $desc = 'Pas de description actuellement';
		echo '<p><span class="titre_menu"><span>D</span>escription :</span><em>'.stripslashes($desc).'</em></p>';
		
		//right father updating
		echo '<div id="update_father_right">';
		$this->displayFatherRight($right);
		echo '</div>';
	}

	public function displayUpdateRight($right){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		$temp = $db->fetchArray($db->query("SELECT name,description FROM kernel_right WHERE id='".$right."'"));
		$desc=$temp['description'];
		$this->rightCache[$right]=$temp['name'];
		
		echo '<div id="update_right_feedback" style="display:none" >';
    $page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		
		echo '<div id="right_name_missing" style="display:none" >';
    $page->msgInfo('Veuillez donner un nom à ce droit.');
		echo '</div>';
		
		$page->moduleHeader('Modification de : '.$this->rightCache[$right],'');
		echo '<form method="post" action="'.$core->makeUrl($this->path.'/ajax/admin_right.php').'" onsubmit="'.
			'if(this.right_name.value != \'\'){'.
				'hide(gE(\'right_name_missing\'));'.
				'display(gE(\'update_right_feedback\'));'.
				'AJAX.submitAndCall(this, false, function(){'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_right&right='.$right.'\', \'admin_right_content\');'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_rights_tree\',\'rights_tree\');'.
				'});'.
			'}else{'.
				'show(gE(\'right_name_missing\'));'.
			'}'.
			'return false; " >
			<input type="hidden" name="id_admin" value="'.$this->id.'" />
			<input type="hidden" name="wope_action" value="save_right" />
			<input type="hidden" name="right_save_case" value="update" />
			<input type="hidden" name="right_id" value="'.$right.'" />
			<p class="form-input"><label>Intitulé :</label><input type="text" id="right_name" name="right_name" value="'.$this->rightCache[$right].'" /></p>
			<p class="form-input"><label>Description :</label><textarea cols="30" rows="4" name="right_desc" >'.stripslashes($desc).'</textarea></p>
			<p class="form-input"><label>Parent :</label><select id="right_id_parent" name="right_id_parent">';
			$this->selectFatherRights($right);
			echo '</select>
			<p class="form-submit">
				<input type="submit" value="Modifier" />
				<input type="button" value="Annuler" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&amp;wope_action=display_right&amp;right='.$right.'\', \'admin_right_content\');" />
			</p>
		</form>';
		$page->moduleFooter();
	}

	public function displayAddRight($post = ''){
		global $core,$db;
		if(!isset($page))$page = new Page();
		
		echo '<div id="add_right_feedback" style="display:none" >';
    $page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		
		echo '<div id="right_name_missing" style="display:none" >';
    $page->msgInfo('Veuillez donner un nom au droit à ajouter.');
		echo '</div>';
		
		$page->moduleHeader('Nouveau droit','');
		echo '<form method="post" action="'.$core->makeUrl($this->path.'/ajax/admin_right.php').'" onsubmit="'.
			'if(this.right_name.value != \'\'){'.
				'hide(gE(\'right_name_missing\'));'.
				'show(gE(\'add_right_feedback\'));'.
				'AJAX.submitAndCall(this, false, function(){'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&amp;wope_action=display_right_added&amp;right=\'+gE(\'right_name\').value, \'admin_right_content\');'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&amp;wope_action=display_rights_tree\',\'rights_tree\');'.
				'});'.
			'}else{'.
				'show(gE(\'right_name_missing\'));'.
			'}'.
			'return false;'.
		'" >
		<input type="hidden" name="id_admin" value="'.$this->id.'" />
			<input type="hidden" name="wope_action" value="save_right" />
			<input type="hidden" name="right_save_case" value="add" />
			<p class="form-input"><label>Intitulé :</label><input type="text" id="right_name" name="right_name" /></p>
			<p class="form-input"><label>Description :</label><textarea cols="30" rows="4" name="right_desc" ></textarea></p>
			<p class="form-input"><label>Parent :</label><select id="right_id_parent" name="right_id_parent">';
			$this->selectFatherRights();
			
			echo '</select></p>
			<p class="form-submit">
				<input type="submit" value="Ajouter" />
				<input type="button" value="Annuler" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_right_default\', \'admin_right_content\');" />
			</p>
		</form>';
		$page->moduleFooter();
	}
	
	public function displayFatherRight($right){
		global $core,$db;
		$form = new Form();
		if ($this->rightCache==null) {
			$result = $db->query("SELECT d1.name FROM kernel_right d1 WHERE d1.id=$right");
			$this->rightCache[$right] = $db->result($result,0);
		}
		echo '<p><span class="titre_menu"><span>C</span>hanger le droit dont hérite "'.$this->rightCache[$right].'" :</span></p>';
		echo '<form ';
			echo 'onsubmit="AJAX.getAndCall(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=update_father_right&right='.$right.'&father=\'+gE(\'father_right\').options[gE(\'father_right\').selectedIndex].value, function(){'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_father_right&amp;right='.$right.'\', \'update_father_right\');'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_rights_tree\',\'rights_tree\');'.
				'}'.
			');'.
			'return false;" >';

		echo '<p class="form-input"><label>Nouveau père : </label>
				<select id="father_right">';
					/*<option value="null" '.(($selected=='')?'selected="selected"':'').' >Aucun père</option>';*/
					$this->selectFatherRights($right);
		echo '</select></p>';
		
		echo '<p class="form-submit"><input type="submit" value="Modifier" /></p>
			</form>';
	}

	private function selectFatherRights($right=-1){
		global $db, $core;
		if ($right<>-1) {
			$right=(int) $right;
			$selected = $db->result($db->query("SELECT parent FROM kernel_right WHERE id='".$right."'",true),0);
			$query="SELECT d1.id, d1.name, d1.parent
					FROM kernel_right d1,kernel_right d2
					WHERE d2.id=$right AND (d1.lft<d2.lft or d1.rgt>d2.rgt)
					GROUP BY d1.id
				  	ORDER BY d1.lft";
		} else {
			$selected="1";
			$query="SELECT d1.id, d1.name, d1.parent
								FROM kernel_right d1
								GROUP BY d1.id
							  	ORDER BY d1.lft";
		}
		$result = $db->query($query);
		$pile=array();
		$level=0;
		
		while ($row=$db->fetchArray($result)) {
			if (!in_array($row['parent'],array_keys($pile))) {
				$pile[$row['parent']]=$level++;
			} else {
				$level=$pile[$row['parent']]+1;
			}
			
			if ($row['id']<>$right) {
				echo '<option value="'.$row['id'].'" '.(($selected==$row['id'])?'selected="selected"':'').' >';
				for ($i=1;$i<$level;$i++) {
					echo "+";
				}
				echo stripslashes($row['name']).'</option>';
			}
		}
		/*$sub_request = '';
		$sub_sub_request = '';
		$sub_sub_request = $this->fatherRightsSubRequest($right);
		if($sub_sub_request != ''){
			$sub_request = " AND liste NOT IN ( ".$sub_sub_request." ) ";
		}
		$result = $db->query("SELECT liste FROM listes WHERE liste!='".$right."' ".$sub_request." ORDER BY liste");
		while($row = $db->fetchArray($result)){
			echo '<option value="'.$row['liste'].'" '.(($selected==$row['liste'])?'selected="selected"':'').' >'.$row['liste'].'</option>';
		}*/
		
	}

	/*private function fatherRightsSubRequest($right){
		global $db;
		$request='';
		$result = $db->query("SELECT liste FROM listes WHERE parent='".$right."'");
		if($db->numRows($result)!=0){
			 $request = "SELECT liste FROM listes WHERE parent='".$right."' ";
			while($row = $db->fetchArray($result)){
				$sub_req = $this->fatherRightsSubRequest($row['liste']);
				if($sub_req != ''){
					$request .= " OR liste IN ( ".$sub_req." ) ";
				}
			}
		}
		return $request;
	}*/

	public function updateFatherRight($right,$father){
		global $db;
		/*if($father=='null'){
			$db->query("UPDATE listes SET parent=NULL WHERE liste='".$right."'");
		}else{
			$db->query("UPDATE listes SET parent='".$father."' WHERE liste='".$right."'");
		}*/
		$right=(int)$right;
		$father=(int)$father;
		$result=$db->query("SELECT name, description FROM kernel_right WHERE id=$right");
		if ($row=$db->fetchArray($result)) {
			$row["description"]=addslashes($row["description"]);
			$row["name"]=addslashes($row["name"]);
			$db->query("CALL updateRight($right,'".$row["name"]."','".$row["description"]."',$father)");
		}
	}

	public function removeRightFromUser($right,$user){
		global $db;
		$right=(int)$right;
		$user=(int)$user;
		$db->query("DELETE FROM kernel_right_user_get WHERE login='".$user."' AND idRight='".$right."'");
		$this->displayRightUsersList($right);
	}

	public function removeAdminFromRight($right,$user){
		global $db;
		$right=(int)$right;
		$user=(int)$user;
		$db->query("DELETE FROM kernel_right_user_set WHERE login='".$user."' AND idRight='".$right."'");
		$this->displayAdminUsersList($right);
	}

	public function addRightToUser($right,$user){
		global $db;
		$right=(int)$right;
		$user_id=(int)$user;
		
		if ($user_id != $user) {
			$user_name=htmlentities($user,ENT_QUOTES,"UTF-8");
			$user_id=$db->result($db->query("SELECT id FROM usersInfo WHERE login='$user_name'"),0);	
		}
		if ($user_id != 0) {
			$db->query("INSERT INTO kernel_right_user_get (login, idRight) VALUES (".$user_id.", '".$right."') ");
		}
		$this->displayRightUsersList($right);
	}

	public function addAdminToRight($right,$user){
		global $db;
		$right=(int)$right;
		$user=(int)$user;

		$db->query("INSERT INTO kernel_right_user_set (login, idRight) VALUES ('".$user."', '".$right."') ");
		$this->displayAdminUsersList($right);
	}

	public function deleteRight($right){
		global $db;
		//$father = $db->result($db->query("SELECT parent FROM listes WHERE liste='".$right."' "),0);
		/*$db->query("UPDATE listes SET parent='".$father."' WHERE parent='".$father."' ");
		$db->query("DELETE FROM kernel_right_user_get WHERE idRight='".$right."'");
		$db->query("DELETE FROM droits_module WHERE droits='".$right."'");
		$db->query("DELETE FROM kernel_right_user_set WHERE droit='".$right."'");
		$db->query("UPDATE usersInfo SET grp='' WHERE grp='".$right."'");
		$db->query("DELETE FROM listes WHERE liste='".$right."'");*/
		$right=(int)$right;
		// This procedure will cascade on the other tables
		$db->query("CALL deleteRight($right)");
		
	}

	public function saveRight($infos){
		global $db;
		$page = new Page();

		if($infos['right_name'] != ''){
			switch($infos['right_save_case']){
				case 'update':
				/*$db->query("UPDATE listes SET parent='".$infos['right_name']."' WHERE parent='".$infos['right_old_name']."' ");
				$db->query("UPDATE listes SET liste='".$infos['right_name']."', description='".$infos['right_desc']."' WHERE liste='".$infos['right_old_name']."' ");
				$db->query("UPDATE droits_etudiants SET nom_liste='".$infos['right_name']."' WHERE nom_liste='".$infos['right_old_name']."'");
				$db->query("UPDATE droits_module SET droits='".$infos['right_name']."' WHERE droits='".$infos['right_old_name']."'");
				$db->query("UPDATE droit_attribution SET droit='".$infos['right_name']."' WHERE droit='".$infos['right_old_name']."'");
				$db->query("UPDATE usersInfo SET grp='".$infos['right_name']."' WHERE grp='".$infos['right_old_name']."'");
				*/
					$db->query("CALL updateRight ('".$infos['right_id']."','".$db->escapeString($infos['right_name'])."', '".$db->escapeString($infos['right_desc'])."','".$infos['right_id_parent']."')");
					break;
				case 'add':
					//$db->query("INSERT INTO listes (liste,parent,description) VALUES ('".$infos['right_name']."', NULL, '".$infos['right_desc']."')");
					$db->query("CALL insertRight ('".$db->escapeString($infos['right_name'])."', ".$infos['right_id_parent'].", '".$db->escapeString($infos['right_desc'])."')");
					break;
				default:
					//void
					break;
			}

		}
	}
	
	public function displayAdminUsersList($right){
		global $db,$core;
		$core->loadClass('date');
		$core->loadClass('box');
		$date = new Date();
		$box = new Box();
		
		$right=(int)$right;
		$result = $db->query("SELECT de.expire, trombi.id_user, trombi.lastname, trombi.firstname FROM kernel_right_user_set de INNER JOIN trombi_trombi trombi ON de.login=trombi.id_user WHERE idRight='$right' ORDER BY login ASC");
		echo '<table align="center" width="90%" class="data_table">
			<tr><th width="50%" >Utilisateur</th><th width="25%">Date d\'expiration</th><th width="25%">Retirer</th></tr>';
		while($row = $db->fetchArray($result)){
			echo '<tr>
				<td><a href="index.php?part=user&content='.$row['id'].'&option=see">'.stripslashes($row['firstname']).' '.stripslashes($row['lastname']).' ('.$row['id_user'].')</a></td>
				<td align="center"><a href="index.php?part=user&content='.$row['id_user'].'&option=see">'.$date->reverseDate($row['date_fin']).'</a></td>
				<td align="center"><a onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=remove_admin_from_right&right='.$right.'&user='.$row['id_user'].'\',\'admin_users_list\');', 'Etes-vous sûr de vouloir supprimer le privilège de '.stripslashes($row['firstname']).' '.stripslashes($row['lastname']).' ?').'" class="trash"></a></td>
			</tr>';
		}
		if($db->numRows($result)==0){
			echo '<td align="center" colspan="3">Aucun résultat</td>';
		}
		echo '</table>';
	}
	
	public function displayRightUsersList($right){
		global $db,$core;
		$core->loadClass('date');
		$core->loadClass('box');
		$date = new Date();
		$box = new Box();
		
		$right = (int)$right;
		$result = $db->query("SELECT trombi.id_user, trombi.lastname, trombi.firstname FROM kernel_right_user_get krus, trombi_trombi trombi WHERE krus.idRight='$right' AND krus.login=trombi.id_user ORDER BY trombi.lastname LIMIT 40");
		echo '<table align="center" width="90%" class="data_table">
			<tr><th width="70%" >Utilisateur</th><th width="30%" >Retirer</th></tr>';
		while($row = $db->fetchArray($result)){
			echo '<tr>
				<td><a href="index.php?part=user&content='.$row['id_user'].'">'.$row['firstname'].' '.$row['lastname'].' ('.$row['id_user'].')</a></td>
				<td align="center"><a onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=remove_right_from_user&right='.$right.'&user='.$row['id_user'].'\',\'right_users_list\');', 'Etes-vous sûr de vouloir supprimer le droit de '.stripslashes($row['firstname']).' '.stripslashes($row['lastname']).' ?').'" class="trash"></a></td>
			</tr>';
		}
		if($db->numRows($result)==0){
			echo '<td align="center" colspan="2">Aucun résultat</td>';
		}elseif($db->numRows($result)>=40){
			echo '<td align="center" colspan="2">Utiliser l\'administration des utilisateurs pour gérer leurs droits</td>';
		}
		echo '</table>';
	}

	private function displayRightsList(){
		echo '<p><span class="titre_menu right_tree"><span>C</span>liquez sur un droit pour le modifier</span></p>';
		echo '<p><div id="rights_tree" >';
		$this->displayRightsTree();
		echo '</div></p>';
	}

	public function displayRightsTree($parent='',$rand_id='',$branch=0){
		global $db,$core;
		usleep(200000);
		$result = $db->query("SELECT d1.id, d1.name, d1.parent, IF(lft=rgt-1,'child','node') as type
								FROM kernel_right d1
								GROUP BY d1.id
							  	ORDER BY d1.lft");
		$pile=array();
		$level=0;
		$previous=0;
		$previous_id='';
		if ($this->rightCache==null) {
			$updateRightCache=true;
		}
		while ($row=$db->fetchArray($result)) {
			if ($updateRightCache) {
				$this->rightCache[$row['id']] = stripslashes($row['name']);
			}
			if (!in_array($row['parent'],array_keys($pile))) {
				$pile[$row['parent']]=$level++;
			} else {
				$level=$pile[$row['parent']]+1;
			}
			
			if ($row['type']=="child") {
				$special_class=	'exp_child';
			} else {
				$special_class=	'exp';	
			}
			
			if ($previous < $level) {
				echo '<ul id="list_right_'.$row['id'].'" class="tree" >';
				if ($row['type']=="node") {
					echo '<a class="expand_tree" onclick="display(gE(\'right_'.$row['id'].'\'));this.firstChild.data=((this.firstChild.data == \'-\')?\'+\':\'-\')" >-</a>';
				}
				echo '<a class="user_right '.$special_class.' branch_'.$level.'" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_right&right='.$row['id'].'\',\'admin_right_content\')">'.$row['name'].'</a>';
				echo '<div id="right_'.$row['id'].'">';
			
			} elseif ($previous > $level) {
				for ($i=($previous - $level);$i>0;$i--) {
					echo '</div>';
					echo '</ul>';
				}
				echo '</div>';
				echo '</ul>';
				echo '<ul id="list_right_'.$row['id'].'" class="tree" >';
				if ($row['type']=="node") {
					echo '<a class="expand_tree" onclick="display(gE(\'right_'.$row['id'].'\'));this.firstChild.data=((this.firstChild.data == \'-\')?\'+\':\'-\')" >-</a>';
				}
				echo '<a class="user_right '.$special_class.' branch_'.$level.'" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_right&right='.$row['id'].'\',\'admin_right_content\')">'.$row['name'].'</a>';
				echo '<div id="right_'.$row['id'].'">';
				
			} else {
				echo '</div>';
				echo '</ul>';
				echo '<ul id="list_right_'.$row['id'].'" class="tree" >';
				if ($row['type']=="node") {
					echo '<a class="expand_tree" onclick="display(gE(\'right_'.$row['id'].'\'));this.firstChild.data=((this.firstChild.data == \'-\')?\'+\':\'-\')" >-</a>';
				}
				echo '<a class="user_right '.$special_class.' branch_'.$level.'" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_right&right='.$row['id'].'\',\'admin_right_content\')">'.$row['name'].'</a>';
				echo '<div id="right_'.$row['id'].'">';
			}
			
			$previous=$level;
		}
		for ($i=$level;$i>0;$i--) {
			echo '</div>';
			echo '</ul>';
			echo '</li>';
		}
		/*
		if($parent==''){
			$result = $db->query("SELECT liste FROM listes WHERE parent IS NULL ORDER BY liste");
			$ul_id = 'root';
			$rand_id=rand();
		}else{
			$result = $db->query("SELECT liste FROM listes WHERE parent='".$parent."' ORDER BY liste");
			$ul_id = $parent;
		}
		$branch++;
		echo '<div id="'.$rand_id.'"><ul id="list_right_'.$ul_id.'" class="tree" >
		';
		while($row = $db->fetchArray($result)){
				$rand_id=rand();
				echo '<li class="tree_element">';
			if($db->numRows($db->query("SELECT liste FROM listes WHERE parent='".$row['liste']."'"))!=0){
				echo '<a class="expand_tree" onclick="display(gE(\''.$rand_id.'\'));this.firstChild.data=((this.firstChild.data == \'-\')?\'+\':\'-\')" >-</a>';
				$special_class='exp';
			} else { 
				$special_class='exp_child';
			}
			echo '<a class="user_right '.$special_class.' branch_'.$branch.'" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_right.php').'?id_admin='.$this->id.'&wope_action=display_right&right='.$row['liste'].'\',\'admin_right_content\')">'.$row['liste'].'</a></li>
			';
								$this->displayRightsTree($row['liste'],$rand_id,$branch);
		}
		echo '</ul></div>
		';*/
		
	}

/**** End of rights admin functions ****/


/**** Apps admin functions ****/

	public function adminAppContent($name = 'default', $option = null){
		global $core;
		
		switch($name){
			case 'default':
			case '':
				//void
				break;
			default:
				$core->loadClass($this->config_apps[$name]['class'], dirname(__FILE__).'/../../'.$this->config_apps[$name]['url']);
				$object = new $this->config_apps[$name]['class']();
				$object->displayContent( isset($option) ? $option : null );
				break;
		}
	}
	
	public function adminAppMenu($name = 'default', $option = null){
		global $core, $page;
		if(!isset($page))$page=new Page();
		
		switch($name){
			case 'default':
			case '':
				$page->moduleHeader('Applications','');
				echo '<p>Veuillez sélectionner une application à administrer.</p>';
				$page->moduleFooter();
				break;
			default:
				$core->loadClass($this->config_apps[$name]['class'], dirname(__FILE__).'/../../'.$this->config_apps[$name]['url']);
				$object = new $this->config_apps[$name]['class']();
				$object->displayMenu( isset($option) ? $option : null );
				break;
		}
	}

/**** End of apps admin functions ****/


/**** Modules admin functions ****/


	public function adminModuleMenu($module = 'default', $option = null){
		global $core,$page;
		if(!isset($page))$page=new Page();
		
		switch($module){
			case 'default':
			case '':
				$page->moduleHeader('Modules','');
				echo 
				'<ul class="panel">
					<li><a onclick="AJAX.getAndUpdate(\'ajax/admin_module.php?ida='.$this->id.'&ga=dam\',\'admin_module_content\')"/>Ajouter un module</a></li>
					<li><a onclick="AJAX.getAndUpdate(\'ajax/admin_module.php?ida='.$this->id.'&ga=dml\',\'admin_module_content\')"/>Gérer les modules installés</a></li>';
				if( $core->verifDroits('super_admin') ){
					echo '<li><a onclick="AJAX.getAndUpdate(\'ajax/admin_module.php?ida='.$this->id.'&ga=dtl\',\'admin_module_content\')"/>Gérer les onglets et colonnes</a></li>';
				}
				echo '</ul>
				';
				$page->moduleFooter();
				break;
			default:
				include_once(dirname(__FILE__).'/../../modules/'.$module.'/admin/'.$module.'_admin.class.inc');
				$toload = $module.'_admin';
				$module_admin = new $toload();
				$module_admin->displayMenu( isset($option) ? $option : null );
				break;
		}
	}

	public function adminModuleContent($module = 'default', $option = null){
		switch($module){
			case 'default':
			case '':
				$this->displayModulesList();
				break;
			default:
				include_once(dirname(__FILE__).'/../../modules/'.$module.'/admin/'.$module.'_admin.class.inc');
				$toload = $module.'_admin';
				$module_admin = new $toload();
				$module_admin->displayContent( isset($option) ? $option : null );
				break;
		}
	}

	public function displayModulesList(){
		global $core,$db,$page;
		$box = new Box();
		if(!isset($page))$page = new Page();

		$requestSelect = " SELECT 
			mm.id,
			mm.instantiable,
			mm.admin_available,
			mm.default_module,
			mm.default_theme,
			mm.default_column,
			mm.default_position,
			mc.title AS columnTitle,
			mc.position AS columnPosition,
			mt.title AS tabTitle,
			mt.default_position AS tabPosition,
			mm.class,
			mm.title,
			mm.description
		";
		$requestCount = " SELECT COUNT(*) ";
		$requestMiddle = " 
			FROM ( kernel_module_module mm 
				LEFT JOIN kernel_module_column mc ON mm.default_column = mc.id )
					INNER JOIN kernel_module_tab mt ON mc.id_tab = mt.id
		";
		$requestCondition = " WHERE 1 ";
		$requestEnd = " ORDER BY mm.title ASC ";

		$modulesNb = $db->result($db->query($requestCount.$requestMiddle.$requestCondition.$requestEnd), 0);

		if( $modulesNb != 0 ){
			$modulesResult = $db->query($requestSelect.$requestMiddle.$requestCondition.$requestEnd);
		
			//general modules administration is displayed
			$page->moduleHeader('Modules installés','');
		
			echo '<table align="center" width="99%" class="enhanced_data_table">
				<tr>
				<th>Intitulé</th>
				<th>Module</th>
				<th>Clonable</th>
				<th>Administrable</th>
				<th>Par défaut</th>
				<th>Autorisé pour</th>
				<th>Obligatoire pour</th>
				<th></th>
				<th></th>
				</tr>';
		
			//style flag
			$style = true;
			while( $row = $db->fetchArray($modulesResult) ){
				$class = 'line_'.($style ? 'a' : 'b');
				$style = !$style;
			
				$mandatory = false;
				
				//rights strings
				$aRights = $mRights = '';
				//comma flags
				$aC = $mC = false;
				
				$availableResult = $db->query("SELECT kr.name, ma.strict FROM kernel_module_available ma INNER JOIN kernel_right kr ON ma.right = kr.id WHERE ma.id_module = '".$row['id']."' ORDER BY name ASC");
				while( $aRow = $db->fetchArray($availableResult) ){
					$aRights .= ( $aC ? ',<br />' : '').$aRow['name'].( $aRow['strict'] == 'Y' ? ' strictement' : ' et héritiers');
					$aC = true;
				}
				
				$mandatoryResult = $db->query("SELECT kr.name, mma.strict FROM kernel_module_mandatory mma INNER JOIN kernel_right kr ON mma.right = kr.id WHERE mma.id_module = '".$row['id']."' ");
				while( $mRow = $db->fetchArray($mandatoryResult) ){
					$mandatory = true;
					$mRights .= ( $mC ? ',<br />' : '').$mRow['name'].( $mRow['strict'] == 'Y' ? ' strictement' : ' et héritiers');
					$mC = true;
				}
			
				echo '
					<tr class="'.$class.'">
						<td><strong>'.stripslashes($row['title']).'</strong></td>
						<td>'.stripslashes($row['class']).'</td>
						<td align="center">'.( $row['instantiable'] == 'Y' ? 'Oui' : 'Non' ).'</td>
						<td align="center">'.( $row['admin_available'] == 'Y' ? 'Oui' : 'Non' ).'</td>
						<td align="center">'.( $row['default_module'] == 'Y' ? 'Oui' : 'Non' ).'</td>
						<td>'.stripslashes($aRights).'</td>
						<td>'.stripslashes($mRights).'</td>
						<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_module.php').'?ida='.$this->id.'&ga=dum&m='.$row['id'].'\',\'admin_module_content\');" class="edit">&nbsp;</a></td>
						<td align="center"><a onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_module.php').'?ida='.$this->id.'&ga=dm&m='.$row['id'].'\',\'admin_module_content\')','Etes-vous sûr de vouloir supprimer le module "'.stripslashes($row['title']).'" ('.stripslashes($row['class']).') ?').'" class="trash">&nbsp;</a></td>
				';

				if( $row['default_module'] == 'Y' || $mandatory ){
					echo '
						<tr class="'.$class.' details">
							<td>&nbsp;&nbsp;&nbsp;&nbsp;Position :</td>
							<td colspan="6">'.stripslashes($row['tabTitle']).' ('.$row['tabPosition'].'e onglet) : '.stripslashes($row['columnTitle']).' ('.$row['columnPosition'].'e colonne) : position '.$row['default_position'].'</td>
							<td></td><td></td>
						</tr>
					';
				}
			
				if( $row['description'] != ''){
					echo '
						<tr class="'.$class.' details">
							<td>&nbsp;&nbsp;&nbsp;&nbsp;Description :</td>
							<td colspan="6">'.stripslashes($row['description']).'</td>
							<td></td><td></td>
						</tr>
					';
				}

			}

			echo '</table>';
		}

		$page->moduleFooter();
	}
	
	public function deleteModule($mId){
		global $db;
		if( !$db->query("DELETE FROM kernel_module_module WHERE id = '".$mId."' ") ){
			throw new DbException('An error occured deleting module :'.$mId, 3000, 1, 'Une erreur est apparue lors de la suppression de ce module.');
		}
		$this->displayModulesList();
	}
	
	public function displayEditModule($case, $post = ''){
		global $core, $db, $page;
		$form = new Form();
		if(!isset($page))$page = new Page();

		$val = false;
		if( $case == 'update' || ( $case == 'add' && is_array($post) ) ){
			$val = true;
		}
		
		$update = false;
		if( $case == 'update' && !is_array($post) ){
			$update = true;
		
			$moduleResult = $db->query(" 
				SELECT 
					mm.id AS module_id,
					mm.instantiable AS module_instantiable,
					mm.admin_available AS module_administrable,
					mm.default_module AS module_default,
					mm.default_theme AS module_theme,
					mm.default_column AS module_column,
					mm.default_position AS module_position,
					mm.class AS module_class,
					mm.title AS module_title,
					mm.description AS module_description
				FROM ( kernel_module_module mm 
					LEFT JOIN kernel_module_column mc ON mm.default_column = mc.id )
						INNER JOIN kernel_module_tab mt ON mc.id_tab = mt.id
				WHERE mm.id = '".$post."'
			");

			$post = $db->fetchArray($moduleResult);
		
			$availableResult = $db->query("SELECT `right`, strict FROM kernel_module_available WHERE id_module = '".$post['module_id']."' ");
			while( $aRow = $db->fetchArray($availableResult) ){
				$post['module_arights'][] = $aRow['right'];
				if( $aRow['strict'] == 'Y' ){
					$post['module_astrict'][] = $aRow['right'];
				}
			}
			
			$mandatoryResult = $db->query("SELECT `right`, strict FROM kernel_module_mandatory WHERE id_module = '".$post['module_id']."' ");
			while( $mRow = $db->fetchArray($mandatoryResult) ){
				$post['module_mrights'][] = $mRow['right'];
				if( $mRow['strict'] == 'Y' ){
					$post['module_mstrict'][] = $mRow['right'];
				}
			}

		}


		$aRights = $mRights = '';
		$rightsResult = $db->query("SELECT id, name FROM kernel_right");
		while( $right = $db->fetchArray($rightsResult) ){
			$selARight = $selMRight = $selAStrict = $selMStrict = '';
			if( $val ){
				if( isset($post['module_arights']) && is_array($post['module_arights']) && in_array($right['id'], $post['module_arights']) ){
					$selARight = ' checked="checked" ';
				}
				if( isset($post['module_astrict']) && is_array($post['module_astrict']) && in_array($right['id'], $post['module_astrict']) ){
					$selAStrict = ' checked="checked" ';
				}
				if( isset($post['module_mrights']) && is_array($post['module_mrights']) && in_array($right['id'], $post['module_mrights']) ){
					$selMRight = ' checked="checked" ';
				}
				if( isset($post['module_mstrict']) && is_array($post['module_mstrict']) && in_array($right['id'], $post['module_mstrict']) ){
					$selMStrict = ' checked="checked" ';
				}
			}


			$aRights .= '<div style="float: left;"><div style="float: left; width: 170px;">'.stripslashes($right['name']).'</div>'.
			'<div style="width: 70px; float: left;"><input type="checkbox" name="module_arights[]" value="'.$right['id'].'" '.$selARight.' /></div>'.
			'<span style="float: left;"><input type="checkbox" name="module_astrict[]" value="'.$right['id'].'" '.$selAStrict.' /></span>'.
			'</div>';
			
			$mRights .= '<div style="float: left;"><div style="float: left; width: 170px;">'.stripslashes($right['name']).'</div>'.
			'<div style="width: 70px; float: left;"><input type="checkbox" name="module_mrights[]" value="'.$right['id'].'" '.$selMRight.' /></div>'.
			'<span style="float: left;"><input type="checkbox" name="module_mstrict[]" value="'.$right['id'].'" '.$selMStrict.' /></span>'.
			'</div>';
		}


		$colResult = $db->query("
			SELECT 
				mc.id,
				mt.default_position AS tabPos,
				mc.position AS colPos,
				mc.title AS colTitle,
				mt.title AS tabTitle
			FROM kernel_module_column mc
				INNER JOIN kernel_module_tab mt ON mc.id_tab = mt.id
			ORDER BY mt.default_position ASC, mc.position ASC
		");


		$page->moduleHeader('Ajout','');

		echo '<div id="add_module_feedback" style="display:none" >';
		  $page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
							
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/admin_module.php').'" onsubmit="'.
			'AJAX.submitAndUpdate(this, false, \'admin_module_content\');'.
			'display(gE(\'add_module_feedback\'));'.
			'return false;'.
		'" >
			<div class="input-hidden">
				<input type="hidden" name="ida" value="'.$this->id.'" />
				<input type="hidden" name="ga" value="sm" />
				<input type="hidden" name="save_case" value="'.$case.'" />';
		if( $case == 'update' ){
			echo '<input type="hidden" name="module_id" value="'.$post['module_id'].'" />';
		}
		echo '</div>
			<p class="form-input">
				<label for="module_class">Classe * :</label>
				<input type="text" name="module_class" value="'.($val ? ( $update ? stripslashes(html_entity_decode($post['module_class'], ENT_QUOTES, 'UTF-8')) : $post['module_class']) : '' ).'" />
			</p>
			<p class="form-input">
				<label for="module_title">Titre * :</label>
				<input type="text" name="module_title" value="'.($val ? ( $update ? stripslashes(html_entity_decode($post['module_title'], ENT_QUOTES, 'UTF-8')) : $post['module_title']) : '' ).'" />
			</p>
			<p class="form-textarea">
				<label for="module_description">Description :</label>
				<textarea name="module_description" cols="40" rows="5">'.($val ? ( $update ? stripslashes(html_entity_decode($post['module_description'], ENT_QUOTES, 'UTF-8')) : $post['module_title']) : '' ).'</textarea>
			</p>
			<p class="form-input">
				<label for="module_instantiable">Clonable :</label>
				<input type="checkbox" name="module_instantiable" value="Y" '.(  $val && $post['module_instantiable'] == 'Y' ? 'checked="checked"' : '' ).' />
			</p>
			<p class="form-input">
				<label for="module_administrable">Administrable :</label>
				<input type="checkbox" name="module_administrable" value="Y" '.(  $val && $post['module_administrable'] == 'Y' ? 'checked="checked"' : '' ).' />
			</p>
			<p class="form-input">'.
				'<label>Disponible pour * :</label>'.
				'<div style="overflow: auto; width: 350px;">'.
					'<div style="float: left;">'.
						'<div style="float: left; width: 155px;">Droit</div>'.
						'<div style="width: 70px; float: left;">Disponible</div>'.
						'<span style="float: left;">Strictement</span>'.
					'</div>'.
				'</div>'.
				'<div style="float: left; overflow: auto; height: 80px; width: 350px; border: 1px solid grey;">'.
					$aRights.
				'</div>'.
			'</p>
			<p class="form-input">
				<label>Obligatoire pour :</label>'.
				'<div style="overflow: auto; width: 350px;">'.
					'<div style="float: left;">'.
						'<div style="float: left; width: 155px;">Droit</div>'.
						'<div style="width: 70px; float: left;">Obligatoire</div>'.
						'<span style="float: left;">Strictement</span>'.
					'</div>'.
				'</div>'.
				'<div style="float: left; overflow: auto; height: 80px; width: 350px; border: 1px solid grey;">'.
					$mRights.
				'</div>'.
			'</p>
			<p class="form-input">
				<label for="module_default">Par défaut :</label>
				<input type="checkbox" name="module_default" value="Y" '.(  $val && $post['module_default'] == 'Y' ? 'checked="checked"' : '' ).' />
			</p>
			<p>&nbsp;</p>
			<p>Les informations suivantes sont utiles si votre module est obligatoire pour certains utilisateurs ou que vous avez coché l\'option "Par défaut" :</p>
			<p class="form-input">
				<label for="module_column">Colonne :</label>
				<select name="module_column">
		';

		while( $col = $db->fetchArray($colResult) ){
			if( $val &&  $col['id'] == $post['module_column'] ){
				$sel = ' selected="selected" ';
			}else{
				$sel = '';
			}
			
			$option = stripslashes( ( $col['colTitle'] != '' ? $col['colTitle'] : $col['colPos'].'e colonne' ).' ('.( $col['tabTitle'] != '' ? $col['tabTitle'] : $col['tabPos'].'e onglet' ).')' );
			echo '<option value="'.$col['id'].'" '.$sel.' >'.$option.'</option>';
		}
		echo '
				</select>
			</p>
			<p class="form-input">
				<label for="module_position">Position :</label>
				<input type="text" name="module_position" value="'.($val ? $post['module_position'] : '1' ).'" />
			</p>';
		/*		<p class="form-input">
					<label for="module_theme">Thème :</label>
					<input type="text" name="module_theme" value="'.($val ? $post['module_theme'] : '' ).'" />
				</p>*/
		echo '
			<p class="form-submit">
				<input type="submit" value="'.( $case == 'add' ? 'Ajouter' : 'Modifier' ).'" />
				<input type="button" value="Annuler" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_module.php').'?ida='.$this->id.'&ga=dml\', \'admin_module_content\');" />
			</p>
		<form>';
		$page->moduleFooter();
	}
	
	public function saveModule($infos){
		global $db, $core;
		$core->loadClass('form');
		$page = new Page();
		$form = new Form();
	
		$saveOk = true;
		
		$instantiable = isset($infos['module_instantiable']) ? $infos['module_instantiable'] : 'N' ;
		$admin = isset($infos['module_administrable']) ? $infos['module_administrable'] : 'N' ;
		$default = isset($infos['module_default']) ? $infos['module_default'] : 'N' ;
		$mandatory = isset($infos['module_mrights']) && is_array($infos['module_mrights']) && count($infos['module_mrights']) != 0;

		if( $infos['module_class'] == '' || $infos['module_title'] == '' || !isset($infos['module_arights']) || !is_array($infos['module_arights']) || count($infos['module_arights']) == 0 ){
			$saveOk = false;
			$page->msgError('Veuillez remplir les champs marqués d\'une étoile.');
		}

		if( $default == 'Y' || $mandatory ){
			$pos = false;
			if( isset($infos['module_position']) ){
				if( $form->verifForm($infos['module_position'], 'int') ){
					$pos = true;
				}else{
					$page->msgWarning('La position fournie n\'est pas un entier. Le module a été ajouté à la fin de la colonne.');
				}
			}

			if( !$pos ){
				$position = " (SELECT MAX(default_position) + 1 FROM kernel_module_module WHERE default_column = '".$infos['module_column']."') ";
			}else{
				$position = " '".$db->escapeString($infos['module_position'])."' ";
			}
		}
		
		try{
		
		if( $saveOk ){
			switch ($infos['save_case']){
			
				case 'add':
					if($db->query('INSERT INTO kernel_module_module (class, title, description, instantiable, admin_available, default_module'.( $default == 'Y' || $mandatory == 'Y' ? ', default_column, default_position' : '' ).") VALUES (
						'".htmlentities($infos['module_class'], ENT_QUOTES, 'UTF-8')."',
						'".htmlentities($infos['module_title'], ENT_QUOTES, 'UTF-8')."',
						'".htmlentities($infos['module_description'], ENT_QUOTES, 'UTF-8')."',
						'".$instantiable."',
						'".$admin."',
						'".$default."'".
						( $default == 'Y' || $mandatory == 'Y' ? ", '".$infos['module_column']."', ".$position." " : '' )."
					) ") ){
						$page->msgInfo('Module ajouté avec succès.');
						$mId = $db->insertId();
					}else{
						throw new DbException('An error occured adding module :'.htmlentities($infos['module_class'], ENT_QUOTES, 'UTF-8'), 3000, 1, 'Une erreur est apparue lors de l\'ajout du module.');
					}
					break;
					
				case 'update':
					if(	$db->query("
						UPDATE kernel_module_module SET 
							class = '".htmlentities($infos['module_class'], ENT_QUOTES, 'UTF-8')."',
							title = '".htmlentities($infos['module_title'], ENT_QUOTES, 'UTF-8')."',
							description = '".htmlentities($infos['module_description'], ENT_QUOTES, 'UTF-8')."',
							instantiable = '".$instantiable."',
							admin_available = '".$admin."',
							default_module = '".$default."'".
							( $default == 'Y' || $mandatory == 'Y' ? ", default_column = '".$infos['module_column']."', default_position = ".$position : '' )."
						WHERE id = '".$infos['module_id']."'
					") ){
							$page->msgInfo('Module modifié avec succès.');
							$mId = $infos['module_id'];
					}else{
						throw new DbException('An error occured updating module :'.$infos['module_id'].' ('.htmlentities($infos['module_class'], ENT_QUOTES, 'UTF-8').')', 3000, 1, 'Une erreur est apparue lors de la mise à jour du module.');
					}
					break;
					
				default:
					//void
					break;
			}
		}
		
		
		//rights for which module is available
		if( $saveOk ){
			if( !$db->query("DELETE FROM kernel_module_available WHERE id_module = '".$mId."' ") ){
				$saveOk = false;
				throw new DbException('An error occured removing availability rights from module :'.$mId.' ('.htmlentities($infos['module_class'], ENT_QUOTES, 'UTF-8').')', 3000, 1, 'Une erreur est apparue lors de l\'enregistrement du module.');
			}
			if( $saveOk ){
				//comma flag
				$aC = false;
				$rights = '';
				foreach( $infos['module_arights'] as $idRight ){
					$strict = isset($infos['module_astrict']) && is_array($infos['module_astrict']) && in_array($idRight, $infos['module_astrict']) ? 'Y' : 'N';
					$rights .= ( $aC ? ',' : '' )."('".$mId."', '".$idRight."', '".$strict."')";
					$aC = true;
				}
				if( !$db->query("INSERT INTO kernel_module_available (id_module, `right`, strict) VALUES ".$rights." ") ){
					$saveOk = false;
					throw new DbException('An error occured adding availability rights to module :'.$mId.' ('.htmlentities($infos['module_class'], ENT_QUOTES, 'UTF-8').')', 3000, 1, 'Une erreur est apparue lors de l\'enregistrement du module.');
				}
			}
		}

		
		//rights for which module is mandatory
		if( $saveOk ){
			if( !$db->query("DELETE FROM kernel_module_mandatory WHERE id_module = '".$mId."' ") ){
				$saveOk = false;
				throw new DbException('An error occured removing rights for which module is mandatory :'.$mId.' ('.htmlentities($infos['module_class'], ENT_QUOTES, 'UTF-8').')', 3000, 1, 'Une erreur est apparue lors de l\'enregistrement du module.');
			}
			if( $saveOk && $mandatory){
				//comma flag
				$mC = false;
				$rights = '';
				foreach( $infos['module_mrights'] as $idRight ){
					$strict = isset($infos['module_mstrict']) && is_array($infos['module_mstrict']) && in_array($idRight, $infos['module_mstrict']) ? 'Y' : 'N';
					$rights .= ( $mC ? ',' : '' )."('".$mId."', '".$idRight."', '".$strict."')";
					$mC = true;
				}
				if( !$db->query("INSERT INTO kernel_module_mandatory (id_module, `right`, strict) VALUES ".$rights." ") ){
					$saveOk = false;
					throw new DbException('An error occured adding rights for which module is mandatory :'.$mId.' ('.htmlentities($infos['module_class'], ENT_QUOTES, 'UTF-8').')', 3000, 1, 'Une erreur est apparue lors de l\'enregistrement du module.');
				}
			}
		}

		}catch(DbException $e){
			$page->msgError($e->getUserMessage());
		}

		if( $saveOk ){
			$this->displayModulesList();
		}else{
			$this->displayEditModule($infos['save_case'], $infos);
		}
		
	}
	
	
/**** End of modules admin functions ****/


/**** Users admin functions ****/

	public function adminUserMenu($user_pattern='none', $option='search'){
		global $db;
		switch($user_pattern){
			case '':
			case 'none':
			case 'default':
				$this->adminUserMenuDefault();
				break;
			default:
				if($option == 'see'){
					$this->adminUserMenuUser($user_pattern);
				}else{
					$this->adminUserMenuDefault();
				}
				break;
		}
	}


	private function adminUserSummary($users_nb, $period){
		global $page, $core, $db;
		if(!isset($page))$page = new Page();

		$result = $db->query("SELECT tt.id_user, tt.firstname, tt.lastname FROM usersInfo ui LEFT JOIN trombi_trombi tt ON ui.id = tt.id_user WHERE ui.actif='N' AND ui.id <> 1 ORDER BY dateCreation DESC LIMIT ".$users_nb." ");
		$nb = $db->result($db->query("SELECT COUNT(*) FROM usersInfo ui LEFT JOIN trombi_trombi tt ON ui.id = tt.id_user WHERE ui.actif='N' AND ui.id <> 1 "), 0);

		$page->moduleHeader('Comptes à activer ( '.(int)$nb.' )', '');
		echo '<h4>Dernières demandes d\'inscription :</h4>';
	
		if($db->numRows($result) != 0 ) { 
			$i = 0;
			$line_style = '';
			while($row = $db->fetchArray($result)) {
				if ($i%2) {
					$line_style = 'line_1';
				} else {
						$line_style = 'line_2';
					}
				echo '<div class="admin_home_item_list '.$line_style.'"><a href="'.$core->makeUrl($this->path.'/index.php').'?part=user&amp;option=see&amp;content='.$row['id_user'].'">'.stripslashes($row['firstname'].' '.$row['lastname']).'</a></div>';
				$i++;
			}
		}else{
			echo '<div><i>Aucun compte à activer.</i></div>';
		}

		echo '<div class="end_list_clear"></div><div>';
		$page->adminLink('user', 'last_users', 'admin');
		echo '</div>';
		
		$page->moduleFooter();
	}
	
	
	//Warning
	//my pages system is quite ugly, I wasn't gifted --> see my trade application to do better
	public function adminUserContent($user_pattern='none', $option='search', $cur_page=1, $nb_per_page='20'){
		global $db;
		switch($user_pattern){
			case '':
			case 'none':
			case 'default':
				$this->displayUsersList('all_users',$cur_page, $nb_per_page);
				break;
			default:
				if($option == 'see'){
					$this->displayUser($user_pattern);
				}else{
					$this->displayUsersList($user_pattern,$cur_page, $nb_per_page);
				}
				break;
		}
	}

	public function adminUserMenuUser($user_id=''){
		global $core, $db, $page;
		$box = new Box();

		$result = $db->query("SELECT lastname, firstname FROM trombi_trombi WHERE id_user='".$user_id."'");
		$row = $db->fetchArray($result);
		
		if(!isset($page))$page = new Page();
		$page->moduleHeader('Utilisateur : '.$row['firstname'].' '.$row['lastname'],'');
		echo '<ul class="panel"><li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_update_user&user='.$user_id.'\',\'admin_user_content\')" >Modifier les paramètres de l\'utilisateur</a></li>';
		echo '<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_update_trombi_card&user='.$user_id.'\',\'admin_user_content\')" >Modifier sa fiche trombi</a></li>';
		echo '<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_user_prefs&user='.$user_id.'\',\'admin_user_content\')" >Voir ses préférences</a></li>';
		echo '<li><a onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=change_password&user='.$user_id.'\',\'admin_user_content\')','Etes-vous sûr de vouloir réinitialiser le mot de passe de '.ucfirst($row['firstname']).' '.ucfirst($row['lastname']).' ? Un nouveau mot de passe lui sera envoyé par mail.').'" >Réinitialiser le mot de passe</a></li></ul>';
		$page->moduleFooter();
	}

	public function adminUserMenuDefault(){
		global $core, $page;
		if(!isset($page))$page = new Page();
		$page->moduleHeader('Utilisateurs','');
		echo '<ul class="panel"><li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_add_user\',\'admin_user_content\')" >Ajouter un utilisateur</a></li>';
		echo '<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_result&option=search&q=last_users\',\'admin_user_content\')" >Derniers utilisateurs inscrits</a></li></ul>';
		$page->moduleFooter();
	}

	public function displayUsersList($user_pattern='default',$cur_page = 1, $nb_per_page = 20){
		global $core,$db,$page;
		$box = new Box();
		if(!isset($page))$page = new Page();
		if($cur_page=='')$cur_page=1;
		if($nb_per_page=='')$nb_per_page = 20;
		$start_row = ($cur_page - 1) * ($nb_per_page);

		//request construction
		$request_start = 'SELECT ui.id, tr.lastname, tr.firstname, ui.actif ';
		$request_middle = ' FROM usersInfo ui LEFT JOIN trombi_trombi tr ON ui.id=tr.id_user WHERE ui.id <> 1 ';
		$request_end = ' GROUP BY ui.id ';
		
		switch($user_pattern){
			case '':
			case 'all_users':
			case 'default':
			case 'none':
				$page->moduleHeader('Tous les utilisateurs','');
				$request_end .= ' ORDER BY lastname ASC ';
				break;
			case 'last_users':
				$page->moduleHeader('Derniers utilisateurs inscrits','');
				$request_end .= ' ORDER BY dateCreation DESC ';
				break;
			default:
				$page->moduleHeader('Recherche utilisateurs : '.$user_pattern,'');
				$request_middle .= " AND ( ui.id='".$db->escapeString($user_pattern)."' OR tr.lastname LIKE '%".$db->escapeString($user_pattern)."%' OR tr.firstname LIKE '%".$db->escapeString($user_pattern)."%' ) ";
				$request_end .= ' ORDER BY tr.lastname ASC ';
				break;
		}
		$request_end .= " LIMIT ".$start_row.",".$nb_per_page." ";
		$display_request = $request_start.$request_middle.$request_end;

		$pages_nb = (int)$db->result($db->query("SELECT COUNT(ui.id) ".$request_middle), 0);
		
		//query
		$result = $db->query($display_request);

		if($db->numRows($result)==0){
			echo 'Aucun résultat';
		}else{
		
			//display
			echo '<table align="center" width="95%" class="data_table" >
			<tr>
				<th>Id</th>
				<th>Nom</th>
				<th>Prenom</th>
				<th>Droits</th>
				<th>Actif</th>
				<th>Editer</th>
				<th>Suppression</th>
			</tr>';
			while($row = $db->fetchArray($result)){
				$rights='';
				$rights_result = $db->query("SELECT name FROM kernel_right_user_get, kernel_right WHERE login='".$row['id']."' AND kernel_right.id=kernel_right_user_get.idRight");
				while($rights_row = $db->fetchArray($rights_result)){
					$rights .= stripslashes($rights_row['name']).' ';
				}
			
				echo '<tr>
					<td align="right"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_user&user='.$row['id'].'\',\'admin_user_content\');AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_menu_user&user='.$row['id'].'\',\'admin_user_menu\')">'.$row['id'].'</a></td>
					<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_user&user='.$row['id'].'\',\'admin_user_content\');AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_menu_user&user='.$row['id'].'\',\'admin_user_menu\')">'.stripslashes($row['lastname']).'</a></td>
					<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_user&user='.$row['id'].'\',\'admin_user_content\');AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_menu_user&user='.$row['id'].'\',\'admin_user_menu\')">'.stripslashes($row['firstname']).'</a></td>
					<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_user&user='.$row['id'].'\',\'admin_user_content\');AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_menu_user&user='.$row['id'].'\',\'admin_user_menu\')">'.$rights.'</a></td>';
					//(in)activation of user and callback of current display
					echo '<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=active_user&active='.(($row['actif']=='Y')?'N':'Y').'&user='.$row['id'].'&option=search&nb_per_page='.$nb_per_page.'&cur_page='.$cur_page.'&q='.$user_pattern.'\',\'admin_user_content\')"><span class="'.(($row['actif']=='Y')?'yes':'no').'">'.$row['actif'].'</span></a></td>
					<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_update_user&user='.$row['id'].'\',\'admin_user_content\');AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_menu_user&user='.$row['id'].'\',\'admin_user_menu\')" class="edit">&nbsp;</a></td>
					<td align="center"><a onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=delete_user&user='.$row['id'].'&nb_per_page='.$nb_per_page.'&cur_page='.$cur_page.'&q='.$user_pattern.'\',\'admin_user_content\')','Etes-vous sûr de vouloir supprimer l\'utilisateur '.stripslashes($row['firstname'].' '.$row['lastname']).' ('.$row['id'].') ?').'" class="trash">&nbsp;</a></td>
				</tr>';
			}
			echo '</table>';
			$page->htmlBlock->pagesList((int)$pages_nb, $nb_per_page, $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&wope_action=display_result&option=search&nb_per_page='.$nb_per_page.'&cur_page=[#]&q='.$user_pattern.'\',\'admin_user_content\')');
		}

		$page->moduleFooter();
	}

	public function displayUser($user_id){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();

		if($core->isTrombi()){
			include(dirname(__FILE__).'/../trombi/detail.inc');
		}

		$result = $db->query("SELECT * FROM trombi_trombi WHERE id_user='".$user_id."'");
		$row = $db->fetchArray($result);
		
		$page->moduleHeader('Utilisateur : '.$row['firstname'].' '.$row['lastname'],'');
		if($core->isTrombi()){
			echo detail($row);
		}
		$page->moduleFooter();
	}

	public function displayAddUser($post = ''){
		global $core,$page;
		$form = new Form();
		if(!isset($page))$page = new Page();
		$date=date('Y-m-d');

		$val = false;
		if(is_array($post)){
			$val = true;
		}
		
		$page->moduleHeader('Nouvel utilisateur','');
		echo '<div id="add_user_feedback" style="display:none" >';
    $page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		echo '<form id="frm_add_user" action="'.$core->makeUrl($this->path.'/ajax/admin_user.php').'" enctype="multipart/form-data" onsubmit="'.
				'AJAX.submitAndUpdate(this,false, \'admin_user_content\');'.
				'display(gE(\'add_user_feedback\'));'.
				'return false;'.
			'" >
			<input type="hidden" name="user_creation_date" value="'.$date.'" />
			<input type="hidden" name="wope_action" value="save_user" />
			<input type="hidden" name="save_case" value="add" />
			<input type="hidden" name="id_admin" value="'.$this->id.'" />
			<p class="form-input"><label>Prénom * :</label><input type="text" name="user_firstname" value="'.($val ? $post['user_firstname'] : '').'" /></p>
			<p class="form-input"><label>Nom * :</label><input type="text" name="user_lastname" value="'.($val ? $post['user_lastname'] : '').'" /></p>
			<p class="form-input"><label>Email * :</label><input type="text" name="user_email" value="'.($val ? $post['user_email'] : '').'" /></p>
			<p class="form-input"><label>Login * :</label><input type="text" name="user_login" value="'.($val ? $post['user_login'] : '').'" /></p>
			<p class="form-input"><label>Mot de passe * :</label><input type="password" name="user_pwd" /></p>
			<p class="form-input"><label>Confirmation * :</label><input type="password" name="user_pwd_conf" /></p>
			<p class="form-input"><label>Droits * :</label><select name="user_rights[]" multiple="multiple" size="4">';
		$form->selectFromDb('kernel_right','name', '', '', ($val ? $post['user_rights'] : ''), 'id');
		echo '</select></p>
			<p class="form-input"><label>Activer l\'utilisateur :</label>
				<input type="radio" name="active_user" value="Y" '.( $val ? ($post['active_user'] == 'Y' ? 'checked="checked"' : '') : 'checked="checked"' ).' /><span>Oui</span>
				<input type="radio" name="active_user" value="N" '.( ($val && $post['active_user'] == 'N') ? 'checked="checked"' : '').' /><span>Non</span>
			</p>
			<p class="form-submit">
				<input type="submit" value="Ajouter" />
				<input type="button" value="Annuler" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&amp;wope_action=display_result\', \'admin_user_content\');" />
			</p>
		</form>';
		$page->moduleFooter();
	}

	public function displayUpdateUser($user_id){
		global $core,$db,$page;
		$form = new Form();
		if(!isset($page))$page = new Page();
		
		$user_result = $db->query("SELECT ui.login, tr.lastname, tr.firstname, ui.pass, tr.email, ui.actif FROM usersInfo ui LEFT JOIN trombi_trombi tr ON ui.id=tr.id_user WHERE ui.id='".$user_id."'");
		$rights=Array();
		$rights_result = $db->query("SELECT idRight FROM kernel_right_user_get WHERE login='".$user_id."'");
		while($rights_row = $db->fetchArray($rights_result)){
			$rights[] = stripslashes($rights_row['idRight']);
		}
	
		$row = $db->fetchArray($user_result);
	
		$page->moduleHeader('Modifier : '.stripslashes($row['firstname'].' '.$row['lastname']),'');
		echo '<div id="update_user_feedback" style="display:none" >';
    $page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';

			echo '<form id="frm_update_user" action="'.$core->makeUrl($this->path.'/ajax/admin_user.php').'" onsubmit="'.
				'AJAX.submitAndUpdate(this,false,\'admin_user_content\');'.
				'display(gE(\'update_user_feedback\'));'.
				'return false;'.
				'" >
			<input type="hidden" name="wope_action" value="save_user" />
			<input type="hidden" name="save_case" value="update" />
			<input type="hidden" name="id_admin" value="'.$this->id.'" />
			<input type="hidden" name="user_id" value="'.$user_id.'" />
			<p class="form-input"><label>Login * :</label><input type="text" name="user_login" value="'.stripslashes($row['login']).'" /></p>
			<p class="form-input"><label>Prénom * :</label><input type="text" name="user_firstname" value="'.stripslashes($row['firstname']).'" /></p>
			<p class="form-input"><label>Nom * :</label><input type="text" name="user_lastname" value="'.stripslashes($row['lastname']).'" /></p>
			<p class="form-input"><label>Email * :</label><input type="text" name="user_email" value="'.stripslashes($row['email']).'" /></p>
			<p class="form-input"><input type="checkbox" name="chgpasswd" value="yes" onclick="display(gE(\'frm_passwd\'))" /><span> Changer le mot de passe</span>
			<div id="frm_passwd" style="display:none">
				<p class="form-input"><label>Mot de passe :</label><input type="password" name="user_pwd" /></p>
				<p class="form-input"><label>Confirmation :</label><input type="password" name="user_pwd_conf" /></p>
			</div>
			<p class="form-input"><label>Droits * :</label><select name="user_rights[]" multiple="multiple" size="4">';
			$form->selectFromDb('kernel_right','name','','',$rights,'id');
			echo '</select></p>
			<p class="form-input"><label>Utilisateur actif :</label>
				<input type="radio" name="active_user" value="Y" '.(($row['actif']=='Y')?'checked="checked"':'').' /><span>Oui</span>
				<input type="radio" name="active_user" value="N" '.(($row['actif']=='N')?'checked="checked"':'').' /><span>Non</span>
			</p>
			<p class="form-submit">
				<input type="submit" value="Modifier" />
				<input type="button" value="Annuler" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/admin_user.php').'?id_admin='.$this->id.'&amp;wope_action=display_user&amp;user='.$user_id.'\', \'admin_user_content\');" />
			</p>
			</form>';
		$page->moduleFooter();
	}

	public function saveUser($infos){
		global $db, $core;
		$page = new Page();

		$error = false;

		if($infos['user_login'] != '' && $infos['user_lastname'] != '' && $infos['user_firstname'] != '' && $infos['user_email'] != '' && isset($infos['user_rights'][0]) ){
			switch($infos['save_case']){
			
				case 'add':
					$user_id=(int)$user_id;
				
					if($infos['user_pwd'] != '' && $infos['user_pwd'] == $infos['user_pwd_conf']){
						$db->query("INSERT INTO usersInfo (login,pass,dateCreation,actif) VALUES(".
							"'".$db->escapeString($infos['user_login'])."', ".
							"PASSWORD('".$db->escapeString($infos['user_pwd'])."'), ".
							"NOW(), ".
							"'".$infos['active_user']."' ".
						")");

						$user_id = $db->insertId();
						
						$db->query("INSERT INTO trombi_trombi (id_user, lastname, firstname, email) VALUES(".
							"'".$user_id."', ".
							"'".$db->escapeString($infos['user_lastname'])."', ".
							"'".$db->escapeString($infos['user_firstname'])."', ".
							"'".$db->escapeString($infos['user_email'])."' ".
						")");

						$user_rights_query = '';
						foreach($infos['user_rights'] as $right){
							$right = (int)$right;
							if ($user_rights_query == '') {
								$user_rights_query = "(".$user_id.",".$db->escapeString($right).")";
							} else {
								$user_rights_query .= ",(".$user_id.",".$db->escapeString($right).")";
							}
						}
						if ($user_rights_query != '') {
							$db->query("INSERT INTO kernel_right_user_get (login,idRight) VALUES ".$user_rights_query);
						}

					}else{
						$error = true;
						$page->msgWarning('Les mots de passes entrés ne correspondent pas.');
					}
					break;
					
				case 'update':
					$infos['user_id']=(int)$infos['user_id'];
					$request = "UPDATE usersInfo SET login='".$db->escapeString($infos['user_login'])."'";

					if( isset($infos['chgpasswd']) && 'yes' == $infos['chgpasswd'] ){
						if($infos['user_pwd'] == $infos['user_pwd_conf'] && $infos['user_pwd'] != ''){
							//exec('sh '.$core->getConfig('baseServer').'/include/scripts/change_mail_password.sh \''.$core->getLogin().'\' \''.$core->getConfig('local').'\' \''.$infos['user_pwd'].'\'');
							$request .= " , pass = PASSWORD('".$db->escapeString($infos['user_pwd'])."') ";
						}else{
							$error = true;
							$page->msgWarning('Les mots de passes entrés ne correspondent pas.');
						}
					}
					
					$request .= " , actif='".$infos['active_user']."' WHERE id='".$infos['user_id']."'";
					$db->query($request);
				
				
					$db->query("UPDATE trombi_trombi SET ".
						" lastname = '".$db->escapeString($infos['user_lastname'])."', ".
						" firstname = '".$db->escapeString($infos['user_firstname'])."', ".
						" email = '".$db->escapeString($infos['user_email'])."' ".
					" WHERE id_user='".$infos['user_id']."' ");
			
					$db->query("DELETE FROM kernel_right_user_get WHERE login='".$infos['user_id']."'");
					
					$user_rights_query = '';
					foreach($infos['user_rights'] as $right){
						$right = (int)$right;
						if ($user_rights_query == '') {
							$user_rights_query = "(".$infos['user_id'].",".$db->escapeString($right).")";
						} else {
							$user_rights_query .= ",(".$infos['user_id'].",".$db->escapeString($right).")";
						}
					}
					if ($user_rights_query != '') {
						$db->query("INSERT INTO kernel_right_user_get (login,idRight) VALUES ".$user_rights_query);
					}

					break;
					
				default:
					//void
					break;
			}

		}else{
			$error = true;
			$page->msgError('Veuillez remplir les champs marqués d\'une étoile.');
		}

		if($error){

			switch($infos['save_case']){
				case 'update':
					$this->displayUpdateUser($infos['user_id']);
					break;
				
				case 'add':
					$this->displayAddUser($infos);
					break;
				
				default:
					//empty
					break;
			}

		}else{
		
			switch($infos['save_case']){
				case 'update':
					$this->displayUser($infos['user_id']);
					break;
				
				case 'add':
					$this->displayUsersList('last_users');
					break;
				
				default:
					//empty
					break;
			}

		}
		
	}


// Supprimé pour cause de passage au trombiCompilable

	public function displayUpdateTrombiCard($user_id){
		global $core,$db,$page;
		$core->loadClass('magicform');
			
		if(!isset($page))$page = new Page();
		
		
		$page->moduleHeader('Fiche trombi de '.$row['prenom'].' '.$row['nom'],'');
		echo '<p>Not yet implemented</p>';
		$page->moduleFooter();
	}

	public function displayUserPrefs($user_id){
		global $db,$core,$page;
		if(!isset($page))$page = new Page();
		
		$result = $db->query("SELECT lastname, firstname FROM trombi_trombi WHERE id_user='".$user_id."'");
		$row = $db->fetchArray($result);
		
		$page->moduleHeader('Préférences de '.stripslashes($row['firstname'].' '.$row['lastname']),'');
		$result = $db->query("SELECT * FROM usersprefs2 WHERE login='".$user_id."'");

		if($db->numRows($result) > 0){
			while($row = $db->fetchArray($result)){
				echo '<div><span class="nom_champ">'.stripslashes($row['pref_name']).'</span> : <span class="contenu_champ">'.stripslashes($row['pref_value']).'</span></div>';
			}
		}else{
			echo '<p>Cet utilisateur n\'a réglé aucune préférence.</p>';
		}
		
		$page->moduleFooter();
	}

	public function lostPassword($user_id){
		global $db,$core,$page;
		$core->loadClass('mail');
		$mail = new Mail();
		if(!isset($page))$page = new Page();
		
		$new_passwd = substr(md5($user_id.rand()),2,8);
		$to = $core->user->getEmail($user_id);
		$login = $core->getLogin();
		
		if( $to == $login.'@'.$core->getConfig('local') ){
			$to = stripslashes( $db->result($db->query("SELECT trombi_trombi.email2 FROM trombi_trombi WHERE id_user='$user_id' LIMIT 1"), 0) );
		}
		
		$from = $core->getConfig('mailWebmaster');
		$subject = 'Votre nouveau mot de passe';
		$msg =' Bonjour,
'.
		'	Voici votre nouveau mot de passe pour accéder à '.$core->getConfig('site_name').': '.$new_passwd.'
'.
		'	Vous pouvez dès à présent vous logger ici : '.$core->getConfig('baseUrl').'
'.

		'	Cordialement,
'.
		'	l\'équipe de '.$core->getConfig('site_name');
		
		if($db->query("UPDATE usersInfo SET pass=PASSWORD('".$db->escapeString($new_passwd)."') WHERE id='".$user_id."' ")){
			exec('sh '.$core->getConfig('baseServer').'/include/scripts/change_mail_password.sh '.$core->getLogin().' '.$core->getConfig('local').' '.$new_passwd);
			if($mail->send($from, $to, $msg, $subject)){
      	$page->msgInfo('Nouveau mot de passe envoyé');
			}else{
		  	$page->msgError('Échec envoi mail');
			}
		}else{
		  $page->msgError('Échec entrée mot de passe dans la base');
		}
	}

	public function activeUser($user_id,$active){
		global $db;
		$db->query("UPDATE usersInfo SET actif='".$active."' WHERE id='".$user_id."'");
	}

	public function deleteUser($user){
		global $db;
		$db->query("DELETE FROM usersInfo WHERE id='".$user."'");
		// Automatic : $db->query("DELETE FROM kernel_droit_user_set WHERE login='".$user."'");
		$db->query("DELETE FROM usersmenu WHERE login='".$user."'");
		$db->query("DELETE FROM usersmodule WHERE login='".$user."'");
		$db->query("DELETE FROM usersprefs2 WHERE login='".$user."'");
	}
	
/**** End of users admin functions ****/


/*** Start of Category admin functions ***/
	public function showAddCategory () {
		global $core, $page;
		
		$tag=$this->getTag();

		echo '<div id="image_missing_error" style="display: none;">';
		$page->msgError('Veuillez choisir une image pour cette catégorie');
		echo '</div>';
		
		echo '<div id="add_category_feedback" style="display: none;">';
		$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		
		echo "<div>";
		echo "<form action='".$core->makeUrl("applications/admin/index.php")."?wope_action=addCategory&part=category'";
		echo ' enctype="multipart/form-data" method="POST" onsubmit="if(this.image.value == \'\'){'.
			'show(gE(\'image_missing_error\'));'.
			'return false;'.
		'}else{'.
			'hide(gE(\'image_missing_error\'));'.
			'show(gE(\'add_category_feedback\'));'.
		'}">';
		echo "<p class='form-input'><label>Nom : </label><input name='name' value='' type='text' /></p>";
		echo "<p class='form-input'><label>Description : </label><input name='description' type='text' /></p>";
		echo "<p class='form-input'><label>Droit affichage : </label><select name='right_get'>";
				$this->selectRights();
		echo "</select></p>";
		echo "<p class='form-input'><label>Droit attribution : </label><select name='right_set'>";
				$this->selectRights();
		echo "</select></p>";
		echo "<p class='form-input'><label>Image: </label><input name='image' type='file' /></p>";
    	echo "<p class='form-input'><label>Affiché par défault: </label><input name=default_show type=checkbox></p>";
    	echo "<p class='form-submit'><input type='submit' value='Ajouter' />";
    	echo ' <input type="button" value="Annuler" onclick="document.location.href=\''.$core->makeUrl('applications/admin/index.php').'?wope_action=listCategory&part=category\'" /></p>';
		echo "</form>";
		echo "</div>";
	}
	
	public function addCategory () {
		global $core;
		$tag=$this->getTag();
		$core->loadClass("File");
		$file = new File();
		
		if (isset($_POST['name'])) {
			$image_name=md5($_POST['name'].time());
			$image=null;
			if(!empty($_FILES["image"])) {

   				$uploaddir = $core->getConfig('filemanager','path')."/categ_img/";
   				if (preg_match('/^.+\.(gif|jpe?g|png)$/i',$_FILES['image']['name'],$temp)) {
   					$uploaddir.=$image_name.'.'.$temp[1];
   					if(move_uploaded_file($_FILES["image"]["tmp_name"], $uploaddir)) {
						$image=$image_name.'.'.$temp[1];
						$file->image_resize($uploaddir,50,50);
						//echo "Image saved as $image\n<br>";
				   	}	
   				}
			}
			
			$name=$_POST['name'];
			$description=$_POST['description'];
			$rightGet=(int)$_POST['right_get'];
			$rightSet=(int)$_POST['right_set'];
			$defaultShow=isset($_POST['default_show']);
			if ($tag->addCategory($name, $rightGet, $rightSet, $image, $defaultShow,$description)) {
				//echo "Category saved";	
			} else {
				//echo "Error saving the category";
			}
		}
	}
	
	public function showUpdateCategory ($id) {
		global $core, $page;
		
		$tag=$this->getTag();
		$category=$tag->getInfoCategoryAdmin($id);
		$name=htmlentities($category['name'],ENT_QUOTES,"UTF-8");
		$rightGet=$category['rightGet'];
		$rightSet=$category['rightSet'];
		$description=htmlentities($category['description'],ENT_QUOTES,"UTF-8");
		
		echo '<div id="update_category_feedback" style="display: none;">';
		$page->msgInfo('Envoi en cours, veuillez patienter...');
		echo '</div>';
		
		echo "<form action='".$core->makeUrl("applications/admin/index.php")."?wope_action=updateCategory&part=category' ";
		echo ' enctype="multipart/form-data" method="POST" onsubmit="'.
			'show(gE(\'update_category_feedback\'));'.
		'">';
		echo "<input type='hidden' name='id' value='$id'>";
		echo "<p class='form-input'><label>Nom : </label><input name='name' value='$name' type='text' /></p>";
		echo "<p class='form-input'><label>Description : </label><input name='description' value='$description' type='text' /></p>";
		echo "<p class='form-input'><label>Droit affichage : </label><select name='right_get'>";
				$this->selectRights($rightGet);
		echo "</select></p>";
		echo "<p class='form-input'><label>Droit attribution : </label><select name='right_set'>";
				$this->selectRights($rightSet);
		echo "</select></p>";
		echo "<p class='form-input'><label>Image: </label><input name='image' type='file' /></p>";
		echo "<p class='form-input'><label>Affiché par défault: </label><input name=default_show type=checkbox ".($category['defaultShow']=="true"?"checked":"")."></p>";
    	echo "<p class='form-submit'><input type='submit' value='Mettre à jour' />";
    	echo ' <input type="button" value="Annuler" onclick="document.location.href=\''.$core->makeUrl('applications/admin/index.php').'?wope_action=listCategory&part=category\'" /></p>';
		echo "</form>";
	}
	
	public function updateCategory () {
		global $core;
		$core->loadClass("File");
		$file = new File();
		
		
		$tag=$this->getTag();
		if (isset($_POST['id'])) {
			$image_name=md5($_POST['name'].time());
			$image=null;
			if(!empty($_FILES["image"])) {

   				$uploaddir = dirname(__FILE__)."/../../data/files/categ_img/";
   				if (preg_match('/^.+\.(gif|jpe?g|png)$/i',$_FILES['image']['name'],$temp)) {
   					$uploaddir.=$image_name.'.'.$temp[1];
   					if(move_uploaded_file($_FILES["image"]["tmp_name"], $uploaddir)) {
						$image=$image_name.'.'.$temp[1];
						$file->image_resize($uploaddir,50,50);
						//echo "Image saved as $image\n<br>";
					}	
   				}
			}
			$id=(int)$_POST['id'];
			$name=$_POST['name'];
			$description=$_POST['description'];
			$rightGet=(int)$_POST['right_get'];
			$rightSet=(int)$_POST['right_set'];
			$defaultShow=isset($_POST['default_show'])?true:false;
			
			if ($tag->modifyCategory($id, $name, $rightGet, $rightSet, $image, $defaultShow, $description)) {
				//echo "Category saved<br>";	
			} else {
				//echo "Error saving the category<br>";
			}
		}
	}
	
	
	
	public function listCategory () {
		global $core;
		$box = new Box();
		$tag=$this->getTag();
		$listCategory=$tag->getListCategoryAdmin();
		
		echo "<div class='centrage'><span class='panel_h'>";
		echo "<a href='".$core->makeUrl("applications/admin/index.php")."?wope_action=showAddCategory&part=category'>Ajouter une catégorie</a>";
		echo "</span></div>";
		
		echo "<table width='95%' align='center' class='data_table'>";
		echo "<tr>";
		echo "<th>Image</th>";
		echo "<th>Nom</th>";
		echo "<th>Droit affichage</th>";
		echo "<th>Droit attribution</th>";
		echo "<th>Affiché par défault</th>";
		echo "<th>Modifier</th>";
		echo "<th>Supprimer</th>";
		echo "</tr>";
		foreach ($listCategory as $category) {
			echo "<tr>";
			
			echo "<td><img src='".$core->makeUrl("data/files/categ_img/".$category['image'])."'></td>";
			echo "<td>".$category['name']."</td>";
			echo "<td>".$category['rightGet']."</td>";
			echo "<td>".$category['rightSet']."</td>";
			echo "<td align='center'>".($category['defaultShow']=="true"?"Y":"N")."</td>";
			echo "<td align='center'><a class=edit href='".$core->makeUrl("applications/admin/index.php")."?wope_action=showUpdateCategory&part=category&id=".$category['id']."'> </a></td>";
			echo '<td align="center"><a class=trash onclick="'.$box->confirm("location.href ='".$core->makeUrl("applications/admin/index.php")."?wope_action=delCategory&part=category&id=".$category['id']."';","Etes vous sûr de vouloir supprimer la catégorie <i>".$category['name']."</i>").'"> </a></td>';
			echo "</tr>\n";
		}
		echo "</table>";
		
	}

	public function deleteCategory($id) {
		$tag=$this->getTag();
		if ($tag->removeCategory($id)) {
			//echo "Category deleted";	
		} else {
			//echo "Error deleting category";	
		}		
	}
	
	private function getTag() {
		global $core,$db;
		$core->loadClass("tag");
		$tag = new Tag(array(),$core,$db);
		return $tag;
	}
	
	private function selectRights ($right=-1) {
		global $db, $core;
		if ($right!=-1) {
			$selected=(int) $right;
			$query="SELECT d1.id, d1.name, d1.parent
					FROM kernel_right d1
					GROUP BY d1.id
				  	ORDER BY d1.lft";
		} else {
			$selected="1";
			$query="SELECT d1.id, d1.name, d1.parent
								FROM kernel_right d1
								GROUP BY d1.id
							  	ORDER BY d1.lft";
		}
		$result = $db->query($query);
		$pile=array();
		$level=0;
		
		while ($row=$db->fetchArray($result)) {
			if (!in_array($row['parent'],array_keys($pile))) {
				$pile[$row['parent']]=$level++;
			} else {
				$level=$pile[$row['parent']]+1;
			}
			
			echo '<option value="'.$row['id'].'" '.(($selected==$row['id'])?'selected="selected"':'').' >';
			for ($i=1;$i<$level;$i++) {
				echo "+";
			}
			echo $row['name'].'</option>';
		}	
	}
	
	
	
/*** End of Category admin functions ***/

/**** Stats admin functions ****/

	
	private function displayLastUsers(){
		global $core,$db,$page;
		if(!isset($page))$page = new Page();
		
		$users_nb = $db->result($db->query("SELECT COUNT(*) FROM usersInfo WHERE 1"), 0);
		
		$page->moduleHeader('Derniers utilisateurs inscrits ('.$users_nb.' utilisateurs)','');
		echo '<ul class="panel stats">';
		$lastSubscrib=$db->query("SELECT tr.lastname, tr.firstname, tr.id_user FROM trombi_trombi tr INNER JOIN usersInfo ui ON ui.id=tr.id_user ORDER BY ui.dateCreation DESC LIMIT 5");
		while($user=$db->fetchArray($lastSubscrib)){
			echo '<li><a href="'.$core->makeUrl($this->path.'/index.php').'?part=user&content='.$user['id'].'&option=see" >'.$user['firstname'].' '.$user['lastname'].'</a></li>';
		}
		echo '</ul>';
		$page->moduleFooter();

	}

	private function displayMostUsedModules(){
		global $db,$page;
		if(!isset($page))$page = new Page();
		$page->moduleHeader('Modules les plus utilisés','');
		echo '<ul class="panel stats">';
		$nb=$db->result($db->query("SELECT COUNT(*) FROM usersInfo WHERE 1"),0);
		$usedModules=$db->query("SELECT module, COUNT(*) AS nb FROM usersmodule GROUP BY module ORDER BY 2 DESC LIMIT 5");
		while($usMod=$db->fetchArray($usedModules)){
			$prop = round($usMod['nb']/$nb*100);
			echo '<li>'.$usMod['module'].' ( affiché chez '.$prop.'% des utilisateurs)</li>';
		}
		echo '</ul>';
		$page->moduleFooter();
	}

	private function displayStatTools(){
		global $db,$page;
		if(!isset($page))$page = new Page();

		$page->moduleHeader('Autres outils', '');
		echo '<p><a href="phpinfo.php">PHP Info</a></p>';
		$page->moduleFooter();
	}


/**** End of stats admin functions ****/


}
?>
