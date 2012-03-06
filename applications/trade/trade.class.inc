<?php
class Trade{

	/*
	 * id of the object
	 */
	private $id;

	/*
	 * path to access the object
	 */
	private $path;

	/*
	 * array of configuration values
	 */
	private $config;

	/*
	 * can we use the trombi (rogues gallery) ?
	 */
	private $is_trombi;

	/*
	 * Item categories in a strig
	 * Useful to pass it to JS code
	 */
	private $str_cat;
	
	public final function __construct($path = ''){
		global $core;
		$this->path = ($path == '') ? 'applications/trade' : $path;
		
		include('config_trade.inc');
		$this->config = $config_trade;
		$this->is_trombi = $core->isTrombi();
		
		$this->loadCategories();
		$this->id = rand(1000000,9999999);
		$_SESSION['trade_'.$this->id]= serialize($this);
	}
	
	private function loadCategories(){
		global $db;
		$result = $db->query("SELECT id, name, father_id FROM trade_category WHERE father_id != 0");
		$cat = Array();
		while($row = $db->fetchArray($result)){
			if(!isset($cat[$row['father_id']])){
				$cat[$row['father_id']] = '';
			}else{
				$cat[$row['father_id']] .= '#,#';
			}
			$cat[$row['father_id']] .= $row['id'].'#-#'.$row['name'];
		}

		foreach($cat as $key=>$val){
			if(!isset($scat)){
				$scat = '';
			}else{
				$scat .= '#;#';
			}
			$scat .= $key.'#=>#'.$val;
		}

		$this->str_cat = $scat;
		
		$_SESSION['trade_'.$this->id]= serialize($this);
	}
	
  private function cutText ($text,$max_length=LG_MAX_TITRE) {
	  $text=preg_replace("/(.{".($max_length-3)."}).+/","\\1...",$text);
 		return $text;
  }

	public function display($part,$content=null,$option=null){
	
		//first column
		echo '<div class="column0" id="trade_menu">';
    $display_menu = 'displayMenu'.ucfirst(strtolower($part));
		switch($display_menu){
			case 'displayMenuItem':
		    $this->displayMenuItem($content);
				break;
			case 'displayMenuMyspace':
		    $this->displayMenuMyspace($content, $option);
				break;
			case 'displayMenuCategory':
				$this->displayMenuCategory();
				break;
			default:
		  case 'displayMenu':
				//default case : nothing is specified
		  	$this->displayMenuDefault();
			  break;
		}
		echo '</div>';

		//second column
		echo '<div class="column1" id="trade_content">';
    $display_content = 'display'.ucfirst(strtolower($part));
		
		switch($display_content){
		  case 'display':
				//default case : nothing is specified
		  	$this->displayLastItems();
			  break;
			default:
				//other cases : calling of the right function
		    $this->$display_content($content,$option);
				break;
		}
																																										
		echo '</div>';
		echo '<div class="clear"></div>';
	}


/**** Here are methods displaying content of left column ****/

	private function displayMenuDefault(){
		$this->displaySearchModule();
	}

	private function displayMenuCategory(){
		$this->displaySearchModule();
		echo '<div id="categories_followup">';
		$this->displayMenuAlert();
		echo '</div>';
	}

	public function displaySearchModule(){
		global $core, $page, $db;
		if(!isset($page)) $page = new Page();
		$core->loadClass('tabs');
		$tabs = new Tabs(Array('simple'=>'Simple', 'advanced'=>'Avancée'));
		$form = new Form();

		$page->moduleHeader('Recherche :','');
		$tabs->display();

		$tabs->separatorStart('simple');
		
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade.php').'" onsubmit="'.
			'AJAX.submitAndUpdate(this, false, \'trade_content\');'.
			'return false;'.
			'">';
		echo '<div>
				<input type="hidden" name="id_trade" value="'.$this->id.'" />
				<input type="hidden" name="geniutt_action" value="search" />
				<input type="hidden" name="search_type" value="simple" />
			</div>
			<p><label><span>L</span>\'annonce contient l\'expression :</label></p>
			<p class="form-input"><input type="text" name="query" /></p>
			<p class="form-submit"><input type="submit" value="Chercher" /></p>';
		echo '</form>';
		
		$tabs->separatorStop('simple');
		$tabs->separatorStart('advanced');
		
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade.php').'" onsubmit="'.
			'AJAX.submitAndUpdate(this, false, \'trade_content\');'.
			'return false;'.
			'">';
		echo '<div>
				<input type="hidden" name="id_trade" value="'.$this->id.'" />
				<input type="hidden" name="geniutt_action" value="search" />
				<input type="hidden" name="search_type" value="advanced" />
			</div>
			<p><label><span>T</span>ype d\'annonce :</label></p>
			<p class="form-input">
				<select name="type">
					<option value="all" selected="selected">Tous les types</option>
					<option value="selling">Offre</option>
					<option value="buying">Demande</option>
				</select>
			</p>
			<p><label><span>C</span>atégorie :</label></p>
			<p class="form-input">
				<select name="category" onchange="trade_searchSubCateg(this, \'cat_select\',\''.$this->str_cat.'\')">
					<option value="all" selected="selected">Toutes les catégories</option>
		';
		$form->selectFromDb('trade_category', 'name', " WHERE father_id = 0 ", '', '', 'id');
		echo '
				</select>
			</p>
			<p><label><span>S</span>ous-catégorie :</label></p>
			<p class="form-input" id="search_subcategory">
		';
		$this->displaySearchSubCategory('none');
		echo '
			</p>
			<p><label><span>L</span>\'intitulé contient :</label></p>
			<p class="form-input"><input type="text" name="title" /></p>
			<p><label><span>L</span>\'annonce contient :</label></p>
			<p class="form-input"><input type="text" name="description" /></p>
			<p><label><span>P</span>rix inférieur à :</label></p>
			<p class="form-input"><input type="text" name="price" /><span>&nbsp;€</span></p>
			<p class="form-submit"><input type="submit" value="Chercher" /></p>';
		echo '</form>';
		
		$tabs->separatorStop('advanced');
		
		$page->moduleFooter();

	}
	
	private function displaySearchSubCategory($category){
		global $core, $page;
		if(!isset($page)) $page = new Page();
		$form = new Form();
	
		echo '
			<select name="subcategory" id="cat_select">
				<option value="all" selected="selected">Toutes les sous-catégories</option>
		';
		if($category != 'none'){
			$form->selectFromDb('trade_category', 'name', " WHERE father_id='".$category."' ", '', '', 'id');
		}
		echo '
			</select>
		';
	}
	
	private function displayMenuItem($item_id=null){
		global $db, $core;
		
		//done here to limit queries
		$item = $db->fetchArray($db->query("SELECT id, id_trader, main_photo, second_photo FROM trade_item WHERE id='".$item_id."'"));
		
		if(!isset($item)) {
			$this->displayError('action');
		}else{
			$this->displayItemPhotos($item);
			if($item['id_trader'] != $core->getUserId()){
				$this->displayMenuTrader($item);
			}
			if($core->verifDroits($this->config['admin_rights']) || $core->getUserId() == $item['id_trader']){
				$this->displayMenuItemActions($item);
			}
		}
	}

	private function displayMenuItemActions($item){
		global $core, $db;
		$core->loadClass('box');
		if(!isset($page)) $page = new Page();
		$box = new Box();

		$page->moduleHeader('Actions :','');
			echo '<ul class="panel actions_menu">';
			echo '<li><a '.
				' onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=update_item&amp;item='.$item['id'].'\',\'trade_content\')" '.
				'>Modifier l\'annonce</a></li>';
			echo '<li><a '.
				' onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=delete_item&amp;item='.$item['id'].'\',\'trade_content\');'.
					'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=display_menu_myspace&amp;trader='.$item['id_tader'].'&amp;myspace='.(($core->getUserId() == $item['id_trader']) ? 'true' : 'false').'\',\'trade_menu\');',
					'Etes-vous sûr de vouloir supprimer l\'annonce ?').
				'">Supprimer l\'annonce</a></li>';
			echo '</ul>';
		$page->modulefooter();

	}

	public function displayMenuMyspace($user_id, $myspace = false){

		if(!isset($user_id)){
		  $this->displayError('action');
		}
		
		$myspace = ($myspace == 'true') ? true : false;

		if($myspace){
			echo '<div id="categories_followup">';
			$this->displayMenuAlert();
			echo '</div>';
		}else{
			$this->displaySearchModule();
		}

	}

	public function displayMenuAlert($open_cat = ''){
		global $core, $db;
		if(!isset($page)) $page = new Page();
		
		$user_result =  $db->query("SELECT tc.father_id, ta.id_category, ta.mail, ta.site FROM trade_alert ta INNER JOIN trade_category tc ON ta.id_category = tc.id WHERE id_trader='".$core->getUserId()."' ");
		while($cat_user = $db->fetchArray($user_result)){
			if ($cat_user['father_id'] != 0) {
				$alerts[$cat_user['father_id']][$cat_user['id_category']]['mail'] = $cat_user['mail'];
				$alerts[$cat_user['father_id']][$cat_user['id_category']]['site'] = $cat_user['site'];
			}
			else {
				$father_alerts[$cat_user['id_category']]['mail'] = $cat_user['mail'];
				$father_alerts[$cat_user['id_category']]['site'] = $cat_user['site'];
			}
		}
	
		$page->moduleHeader('Alertes :','');
	
		echo '<p class="information">Cliquez une icône pour suivre l\'activité d\'une catégorie :</p>';
	
		$cat_result = $db->query("SELECT id, name, father_id FROM trade_category WHERE father_id = '0' ORDER BY name ASC");
		if($db->numRows($cat_result) != 0){

			echo '<ul class="tree">';
			while($cat = $db->fetchArray($cat_result)){
				$subcat_result = $db->query("SELECT id, name FROM trade_category WHERE father_id = '".$cat['id']."' ORDER BY name ASC");
				$subcat_nb = $db->numRows($subcat_result);
				
				$mail_alert = 'Y';
				$site_alert = 'Y';
				
				if(isset($alerts[$cat['id']])) {
					foreach( $alerts[$cat['id']] as $sub_alert ) {
						if($sub_alert['mail'] == 'N') {
							$mail_alert = 'N';
						}
						if($sub_alert['site'] == 'N') {
							$site_alert = 'N';
						}
					}
				} else {
						if($father_alerts[$cat['id']]['mail'] != 'Y') {
							$mail_alert = 'N';
						}
						if($father_alerts[$cat['id']]['site'] != 'Y') {
							$site_alert = 'N';
						}
				  } 
			
				$cat_id = 'cat_'.$cat['id'];

				echo '
				<li class="tree_element">';
				if($subcat_nb != 0){
					echo '<a class="left expand_tree" onclick="display(gE(\''.$cat_id.'\'));gE(\'plus_minus_'.$cat['id'].'\').innerHTML=((gE(\'plus_minus_'.$cat['id'].'\').innerHTML == \'-\')?\'+\':\'-\')">
						<span id="plus_minus_'.$cat['id'].'" class="left">'.( ($open_cat != $cat['id']) ? '+' : '-' ).'</span>'.
	 				 '<span class="left tree_title">&nbsp;'.stripslashes($cat['name']).'</span>
					</a>';
				}else{
					echo '<a href="javascript:void(0);">
						<span id="plus_minus_'.$cat['id'].'" class="left">&nbsp;</span>'.
						'<span class="left tree_title">&nbsp;'.stripslashes($cat['name']).'</span>
					</a>';
				}
					
				$this->displayMailAlertButton($cat['id'], $mail_alert, 'true');
				echo ' ';
				$this->displaySiteAlertButton($cat['id'], $site_alert, 'true');
				
				// DO NOT DELETE THE NEXT CLEAR DIV : IT'S USED TO DEBUG DISPLAY IN IE 6 !
				echo '</li><div class="clear"></div>';

				if($subcat_nb != 0){
					echo '<ul id="'.$cat_id.'" '.( ($open_cat != $cat['id']) ? 'style="display: none;"' : '' ).'>';
					while($subcat = $db->fetchArray($subcat_result)){
						echo '<li class="tree_element">'.
						'<span class="left tree_title">'.stripslashes($subcat['name']).'</span>';
					
						$this->displayMailAlertButton($subcat['id'], $alerts[$cat['id']][$subcat['id']]['mail'], 'false');
						echo ' ';
						$this->displaySiteAlertButton($subcat['id'], $alerts[$cat['id']][$subcat['id']]['site'], 'false');

						echo '</li>';
						echo '<div class="clear"></div>';
					}
					echo '</ul>';
				}
			}
			echo '</ul>';
		}
		$page->moduleFooter();
	}

			
	private function displayMailAlertButton($cat_id, $mail, $is_cat){
		global $core;
		//
		if( $mail == 'Y' ){
			$class = 'mail_selected';
			$alert = 'N';
		}else{
			$class = '';
			$alert = 'Y';
		}
		
		echo '<a class="mail_alert '.$class.' left" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=alert_cat&amp;cat='.$cat_id.'&amp;is_cat='.$is_cat.'&amp;type=mail&amp;alert='.$alert.'\', \'categories_followup\');" onmouseover="activateToolTips(this,\'Suivi d\\\'une catégorie par E-mail\')" onclick="hide(toolTips)"></a>';
	}

	private function displaySiteAlertButton($cat_id, $site, $is_cat){
		global $core;
		//
		if( $site == 'Y' ){
			$class = 'site_selected';
			$alert = 'N';
		}else{
			$class = '';
			$alert = 'Y';
		}
		
		echo '<a class="site_alert '.$class.' left" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=alert_cat&amp;cat='.$cat_id.'&amp;is_cat='.$is_cat.'&amp;type=site&amp;alert='.$alert.'\', \'categories_followup\');" onmouseover="activateToolTips(this,\'Suivi d\\\'une catégorie sur le site (module)\')" onclick="hide(toolTips)"></a>';

	}

	public function alertCat($cat_id, $is_main_cat, $type, $alert){
		global $db, $core;
		
		//$is_cat is a string : is it a category or a subcategory
		if($is_main_cat == 'true' && $cat_id != 0){
			if($result = $db->query("SELECT id FROM trade_category WHERE father_id='".$cat_id."' ")){
				while($row = $db->fetchArray($result)){
					$this->saveAlert($row['id'], $type, $alert);
				}
			}
			$this->saveAlert($cat_id, $type, $alert);
			$display_cat = $cat_id;
		}
		
		elseif($is_main_cat != 'true' && $cat_id != 0) {
			$result = $db->query("SELECT father_id FROM trade_category WHERE id='".$cat_id."' ");
			$main_cat = $db->result($result, 0);
			
			$this->saveAlert($cat_id, $type, $alert);

			$display_cat = $main_cat;
		}

		else {
			$this->saveAlert($cat_id, $type, $alert);
			$display_cat = $cat_id;
		}
		
		$this->displayMenuAlert($display_cat);

	}

	private function saveAlert($cat_id, $type, $alert){
		global $core, $db;
		
		//if row doesn't exist, creates it
		if( $db->numRows($db->query("SELECT id_category FROM trade_alert WHERE id_trader='".$core->getUserId()."' AND id_category='".$cat_id."' ")) == 0 ){
		
			$db->query("INSERT INTO trade_alert (id_category, id_trader, ".$type.") VALUE ('".$cat_id."', '".$core->getUserId()."', '".$alert."')" );
		
		//else update it
		}else{
		
			$db->query("UPDATE trade_alert SET ".$type."='".$alert."' WHERE id_category='".$cat_id."' AND id_trader='".$core->getUserId()."' " );
			
		}
		
	}

	private function displayMenuTrader($item){
		global $core, $db;
		$core->loadClass('filemanager');
		$fm = new FileManager();
		if(!isset($page)) $page = new Page();

		$user = $db->fetchArray($db->query("SELECT firstname, lastname, photo FROM trombi_trombi WHERE id_user='".$item['id_trader']."'"));

		$page->moduleHeader(stripslashes($user['firstname'].' '.$user['lastname']),'');
			echo '<p>';
			echo $fm->preview($user['photo'], 1);
			echo '</p>';

			echo '<ul class="panel">';
			if($this->is_trombi){
				$core->loadClass('trombi');
				$trombi = new Trombi();
				echo '<li><a onclick="'.$trombi->openCard($item['id_trader']).'">Voir sa fiche trombi</a></li>';
			}
			if( $item['id_trader'] != $core->getUserId() ){
				echo '<li><a '.
					' onmouseover="activateToolTips(this,\'L\\\'espace de l\\\'utilisateur vous renseigne sur lui-m&ecirc;me, ses annonces, sa note, etc...\')" '.
					' onclick="hide(toolTips)" '.
					' href="'.$core->makeUrl($this->path.'/index.php').'?part=myspace&content='.$item['id_trader'].'&option=false" '.
					'>Voir son espace</a></li>';
			}
			echo '</ul>';
		$page->modulefooter();
	}


	private function displayItemPhotos($item){
		global $core;
		$core->loadClass('fileManager');
		$fm = new FileManager();
		if(!isset($page)) $page = new Page();
		
		$page->moduleHeader('Photos de l\'objet','');
		
		//displays reduction of item main photo
		if ( isset($item['main_photo']) && $item['main_photo'] != '' && $item['main_photo'] != 0) {
			echo '<p>Cliquer sur une photo pour l\'agrandir :</p>';
			echo '<p class="photo">
				<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&geniutt_action=display_photo&photo='.$item['main_photo'].'\',\'item_photo\')">';
			echo $fm->preview($item['main_photo'], 1, true);
			echo '
				</a>
			</p>';

			//displays reduction of item second photo
			if( isset($item['second_photo']) && $item['second_photo'] != '' && $item['second_photo'] != 0 ){
				echo '<p class="photo">
					<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&geniutt_action=display_photo&photo='.$item['second_photo'].'\',\'item_photo\')">';
				echo $fm->preview($item['second_photo'], 1, true);
				echo '
					</a>
				</p>';
			}
		}else{
			echo '<p>Aucune photo disponible</p>';
		}
		
		$page->modulefooter();
	}

/**** End of methods displaying content of left column ****/




/**** Here are function displaying content of div : "trade_content" ****/


//** Methods to navigate within items **//


	public function displayQueryResult($data, $current_page){
		global $db, $core, $page;
		if(!isset($page)) $page = new Page();
		$form = new Form();

		//this array will be useful to notice several errors to the user
		$warning = Array();

		//changing of page, search is recovered from session
		if($data == 'session'){
			$data = unserialize($_SESSION['trade_'.$this->id.'_search']);
		//first page, search comes from formulary, we save search criteria
		}else{
			$_SESSION['trade_'.$this->id.'_search']= serialize($data);
		}
	
		//pages management
		$start_row = ($current_page - 1) * ($this->config['items_per_page']);

		if(isset($data['search_type']) && $data['search_type'] != '' ){
		
			switch($data['search_type']){
				case 'simple':
					if($data['query'] != ''){
						$where = " AND ( title LIKE '%".$data['query']."%' OR description LIKE '%".$data['query']."%' ) ";
					}else{
						$where = false;
					}
					break;
				case 'advanced':
					//finally, user can make an empty research to have all items
					//a non empty field is required
					//if( ($data['type'] != 'all') || ($data['category'] != 'all') || ($data['subcategory'] != 'all') || ($data['title'] != '') || ($data['description'] != '') || ($data['price'] != '') ){
					
						$cat_where = '';
						//user selected a category and a subcategory
						if( ($data['category'] != 'all') && ($data['subcategory'] != 'all') ){
							$cat_where =" AND id_category  = '".$data['subcategory']."' AND id_category IN (SELECT id FROM trade_category WHERE father_id = '".$data['category']."' ) ";
						
						//user selected only a category
						}elseif($data['category'] != 'all'){
							$cat_where = " AND id_category = '".$data['category']."' OR id_category IN (SELECT id FROM trade_category WHERE father_id = '".$data['category']."' ) "; 
						}
					
						$type_where = '';
						if( $data['type'] != 'all' ){
							$type_where = " AND trade_type = '".$data['type']."' ";
						}
							
						$price_where = '';
						if( $data['price'] != '' ){
							if( $form->verifForm($data['price'], 'money') ){
								$price_where = " AND price < '".$db->escapeString($data['price'])."' ";
							}else{
								$warning[] = 'price';
							}
						}
						
						$where = $cat_where.
							$type_where.
							" AND title LIKE '%".$db->escapeString($data['title'])."%' ".
							" AND description LIKE '%".$db->escapeString($data['description'])."%' ".
							$price_where;

					//finally, user can make an empty research to have all items
					//all fields are empty
					//}else{
					//	$where = false;
					//}
					
					break;
			}
		
			//finally, user can make an empty research to have all items
			//if($where){
				
				//only valid items are displayed
				$where .= "  AND moderated='N' AND end_date > NOW() ";
				$request_select = "SELECT id, id_category, id_trader, trade_type, title, description, price, main_photo, contact_firstname, contact_lastname FROM trade_item ";
				$where = " WHERE 1 ".$where;
				
				$result_search = $db->query($request_select.$where." ORDER BY id DESC LIMIT ".$start_row.",".$this->config['items_per_page']);
				$count = $db->result($db->query("SELECT COUNT(*) FROM trade_item ".$where), 0);
				
				if($db->numRows($result_search) != 0){
					//if there is non critical errors, displays these errors
					if(isset($warning[0]))$this->displayWarning($warning);
		
					//displays pages
					echo '<div>';
					$page->htmlBlock->pagesList((int)$count, $this->config['items_per_page'], $current_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&geniutt_action=search&criteria_place=session&cur_page=[#]\',\'trade_content\')');
					echo '</div>';
						
					while($item = $db->fetchArray($result_search)){
						$this->displayItemFromList($item);
					}
				
					//displays pages
					echo '<div>';
					$page->htmlBlock->pagesList((int)$count, $this->config['items_per_page'], $current_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&geniutt_action=search&criteria_place=session&cur_page=[#]\',\'trade_content\')');
					echo '</div>';
					
				}else{
					$this->displayInfo('no_result');
				}
			//finally, user can make an empty research to have all items
			//}else{
			//	$this->displayError('empty_field');
			//}
		
		
		}else{
			$this->displayError('action');
		}
	}

	public function displayLastItems(){
		global $db, $page;
		if(!isset($page)) $page = new Page();
	
		echo '<div class="last_trade_items">';
		$page->moduleHeader('Dernières annonces parues : ','');
		$page->moduleFooter();
		echo '</div>';
		
		$result_last = $db->query("SELECT id, id_category, id_trader, trade_type, title, description, price, main_photo, contact_firstname, contact_lastname FROM trade_item WHERE moderated='N' AND end_date >= NOW() ORDER BY id DESC LIMIT 5 ");
		if($db->numRows($result_last) != 0){
			while($item = $db->fetchArray($result_last)){
				$this->displayItemFromList($item);
			}
		}else{
			$this->displayInfo('no_item');
		}
	}

	private function displayCategory($category=null,$current_page){
		global $db, $core, $page;
		if(!isset($page)) $page = new Page();

		if(isset($category) && $category != ''){
			
			$row = $db->fetchArray($db->query("SELECT name FROM trade_category WHERE id='".$category."' "));
			$page->moduleHeader('','');
			echo '<p>Catégorie : '.stripslashes($row['name']).'</p>';
			$page->moduleFooter();
			
			if(!isset($current_page) || $current_page == ''){
				$current_page = 1;
			}
			//pages management
			$start_row = ($current_page - 1) * ($this->config['items_per_page']);
		
			$request_select = "SELECT id, id_category, id_trader, trade_type, title, description, price, main_photo, contact_firstname, contact_lastname FROM trade_item ";
			$request_where = " WHERE id_category IN ( SELECT id FROM trade_category WHERE father_id='".$category."' ) OR id_category='".$category."' AND moderated='N' AND end_date >= NOW() ";
			$reques_end = " ORDER BY id DESC";
			
			$result_category = $db->query($request_select.$request_where.$request_end." LIMIT ".$start_row.",".$this->config['items_per_page']);
			$count = $db->result($db->query("SELECT COUNT(id) FROM trade_item ".$request_where), 0);
			
			if($db->numRows($result_category) != 0){
			
				//displays pages
				echo '<p>';
				$page->htmlBlock->pagesList((int)$count, $this->config['items_per_page'], $current_page, 'href', $core->makeUrl($this->path.'/index.php').'?part=category&content='.$category.'&option=[#]');
				echo '</p>';

				while($item = $db->fetchArray($result_category)){
					$this->displayItemFromList($item);
				}
				
				//displays pages
				echo '<p>';
				$page->htmlBlock->pagesList((int)$count, $this->config['items_per_page'], $current_page, 'href', $core->makeUrl($this->path.'/index.php').'?part=category&option=[#]');
				echo '</p>';
				
			}else{
				$this->displayInfo('no_item');
			}
			
		}else{
			$this->displayError('action');
		}
	
	}

	private function displayItemFromList($item){
		global $core, $db, $page;
		$core->loadClass('fileManager');
		$fm = new FileManager();
		if(!isset($page)) $page = new Page();

		$trader = 'Auteur : ';
		if($item['trade_type']=='selling'){
			$trade_type = 'Offre : ';
			$module_class = 'selling';
		}else{
			$trade_type = 'Demande : ';
			$module_class = 'buying';
		}
	
		$cat = $db->fetchArray($db->query("SELECT tcf.id AS cat_id, tc.id AS subcat_id, tcf.name AS cat_name, tc.name AS subcat_name FROM trade_category tc LEFT JOIN trade_category tcf ON tc.father_id = tcf.id WHERE tc.id='".$item['id_category']."' "));
		
		//this div allows to apply a css on the module without defining the css as module.style.css but in trade.style.css
		echo '<div class="'.$module_class.'">';
		
		$page->moduleHeader($trade_type.htmlentities(stripslashes($item['title']), ENT_QUOTES, 'UTF-8'),'');
		
		echo '<div class="item-category">';
		if($cat['cat_name'] != ''){
			echo '<a class="category"  href="'.$core->makeUrl($this->path.'/index.php').'?part=category&content='.$cat['cat_id'].'">'.htmlentities(stripslashes($cat['cat_name']), ENT_QUOTES, 'UTF-8').'</a>';
			echo ' &gt; <a class="sub_category" href="'.$core->makeUrl($this->path.'/index.php').'?part=category&content='.$cat['subcat_id'].'">'.htmlentities(stripslashes($cat['subcat_name']), ENT_QUOTES, 'UTF-8').'</a>';
		}else{
			echo '<a class="category"  href="'.$core->makeUrl($this->path.'/index.php').'?part=category&content='.$cat['subcat_id'].'">'.htmlentities(stripslashes($cat['subcat_name']), ENT_QUOTES, 'UTF-8').'</a>';
		}
			
			
		echo '</div>';
		
		
		echo '<div class="item-description">
			<div class="item-photo">';
			if( isset($item['main_photo']) && $item['main_photo']!='' ){
				echo $fm->preview($item['main_photo'], 1, true);
			}

		$desc = htmlentities(stripslashes($item['description']), ENT_QUOTES, 'UTF-8');
		$cut_desc = substr($desc, 0, 200);
		if($cut_desc != $desc){
			$cut_desc .= '...';
		}
		
		echo '</div>
			<p class="price">'.($item['price'] != 0 ? 'Prix : <span>'.$item['price'].' €</span>' : '').'</p>
			<br /><br />
			<span class="text-indent"> &nbsp; &nbsp; </span>
			<span class="description_text">'.$cut_desc.'</span>
		</div>';
		$trader_infos_content = htmlentities( $this->cutText( stripslashes($item['contact_firstname'].' '.$item['contact_lastname']), 15), ENT_QUOTES, 'UTF-8');

		echo '<div class="item-actions">
			<div class="trader_infos"><p>'.$trader.' <span onmouseover="activateToolTips(this,\''.$page->htmlBlock->escapeTipContent(stripslashes($item['contact_firstname']).' '.$item['contact_lastname']).'\');">'.$trader_infos_content.'</span></p></div>';

		//for later use
		//	<div class="trader-evaluation"><p>Note : <span>***</span></p></div>
		echo '<ul class="panel">';
		
		if($item['id_trader'] != $core->getUserId()){
			if($this->is_trombi){
				$core->loadClass('trombi');
				$trombi = new Trombi();
				echo '<li><a onclick="'.$trombi->openCard($item['id_trader']).'">Voir la fiche de l\'annonceur</a></li>';
			}
		
			echo '<li><a '.
				' onmouseover="activateToolTips(this,\'L\\\'espace de l\\\'utilisateur vous renseigne sur lui-m&ecirc;me, ses annonces, sa note, etc...\')" '.
				' onclick="hide(toolTips)" '.
				' href="'.$core->makeUrl($this->path.'/index.php').'?part=myspace&content='.$item['id_trader'].'&option=false" '.
				'>Voir son espace</a></li>';
		}
		
		echo '<li><a href="'.$core->makeUrl('applications/trade/index.php').'?part=item&content='.$item['id'].'" >Voir l\'annonce</a></li>
			</ul>
		</div>';
		$page->moduleFooter();

		echo '</div>';
	}

	private function displayItem($item_id=null){
		global $core, $db, $page;
		if(!isset($page)) $page = new Page();

		if(!isset($item_id)){
			$this->displayError('action');
		}

		$item_result = $db->query("SELECT id, id_category, id_trader, trade_type, title, description, price, contact_firstname, contact_lastname, contact_address, contact_phone, contact_mobile, contact_email FROM trade_item WHERE id='".$item_id."'");

		if($db->numRows($item_result) != 1){
			$this->displayError('action');
		}else{
			$item = $db->fetchArray($item_result);
			
			$subcat = $db->fetchArray($db->query("SELECT name, id FROM trade_category WHERE id='".$item['id_category']."' "));
			$cat = $db->fetchArray($db->query("SELECT name, id FROM trade_category WHERE id=(SELECT father_id FROM trade_category WHERE id='".$item['id_category']."' ) "));
		
			if($item['trade_type']=='selling'){
				$trade_type = 'Offre : ';
				$trader = 'Offreur : ';
				$module_class = 'selling';
			}else{
				$trade_type = 'Demande : ';
				$trader = 'Demandeur : ';
				$module_class = 'buying';
			}
		
			echo '<div id="item_photo"></div>';
		
			//this div allows to apply a css on the module without defining the css as module.style.css but in trade.style.css
			echo '<div class="'.$module_class.' see_trade_details">';
			
			$page->moduleHeader($trade_type.stripslashes($item['title']),'');
			
			echo '<div class="item-category">'.
				(($cat['name'] != '') ? '<a class="category" href="'.$core->makeUrl($this->path.'/index.php').'?part=category&content='.$cat['id'].'">'.
				stripslashes($cat['name']).'</a> &gt; ' : '' ).'<a class="sub_category" href="'.$core->makeUrl($this->path.'/index.php').'?part=category&content='.$subcat['id'].'">'.stripslashes($subcat['name']).'</a>
						</div>';
		
			echo '<div class="item-description">
			<p>
					Description de l\'objet :
			</p>
			
			<p class="description_text">'.stripslashes($item['description']).'</p>
			
			<div class="clear"></div>
		
			<p class="right">'.($item['price'] != 0 ? 'Prix : <span>'.stripslashes($item['price']).'€</span>' : '').'</p>
			</div>
			<div class="clear"></div>

			<p class="contact_trader"><span class="titre_menu"><span>J</span>oindre l\'auteur de l\'annonce :</span></p>';


			echo '<div class="item-description">';

			if ($item['contact_firstname'] != '' || $item['contact_lastname'] != '') {
				echo'
				<p>
					<span>'.stripslashes($item['contact_firstname']).'</span>
					<span>'.stripslashes($item['contact_lastname']).'</span>
				</p>';
			}
			
			if ($item['contact_adress'] != '') {
				echo '<p>Adresse : <span>'.stripslashes($item['contact_adress']).'</span></p>';
			}
			if ($item['contact_phone'] != '') {
				echo '<p>Téléphone fixe : <span>'.stripslashes($item['contact_phone']).'</span></p>';
			}
			if ($item['contact_mobile'] != '') {
				echo '<p>Téléphone mobile : <span>'.stripslashes($item['contact_mobile']).'</span></p>';
			}
			if ($item['contact_email'] != '') {
				echo '<p>Email : <span><a href="mailto:'.stripslashes($item['contact_email']).'">'.stripslashes($item['contact_email']).'</a></span></p>';
			}

			echo '</div>';

			$page->moduleFooter();

			echo '</div>';
			
		}
	}


//** End of Methods to navigate within items **//


	public function displayPhoto($photo=null){
		global $core, $page;
		$core->loadClass('fileManager');
		$fm = new FileManager();
		if(!isset($page)) $page = new Page();

		if(!isset($photo)){
		  $this->displayError('action');
		}
		
		$page->moduleHeader('','');
			echo '<div class="photo">
				<a href="'.$core->makeUrl('include/engine/file/fileManagerPreview.php').'?id='.$photo.'&level=4">';
			echo $fm->preview($photo, 3, true);
			echo '</a>
			</div>';
		$page->moduleFooter();
	}


//** Methods displaying space of an user **//

	public function displayMyspace($user_id, $myspace = false){
		global $db;
		$myspace = ($myspace == 'true') ? true : false;
		
		$trader = $db->fetchArray($db->query("SELECT id_user, lastname, firstname FROM trombi_trombi WHERE id_user='".$user_id."' "));
		
		//$this->displayTraderProfile($trader, $myspace);
		echo '<div id="items_'.$user_id.'">';
		$this->displayTraderItems($trader, $myspace);
		echo '</div>';
	}

	private function displayTraderProfile($trader, $myspace = false){
		global $core, $db, $page;
		if(!isset($page)) $page = new Page();
		
		if($myspace){
			$header = 'Mon profil :';
		}else{
			$header = 'Profil : '.stripslashes($trader['firstname'].' '.$trader['lastname']);
		}
		$page->moduleHeader($header, '');
		
		$page->moduleFooter();
	}

	public function displayTraderItems($trader, $myspace = false){
		global $core, $db, $page;
		if(!isset($page)) $page = new Page();
		$core->loadClass('filemanager');
		$box = new Box();
		$fm = new FileManager();
		
		if($myspace){
			$header = 'Mes annonces :';
		}else{
			$header = stripslashes($trader['firstname'].' '.$trader['lastname']);
		}
	
		$items = Array();
		$result = $db->query("SELECT ".
			" ti.id, ".
			" tsc.id AS subcat_id, ".
			" tsc.father_id AS cat_id, ".
			" tsc.name AS subcat_name, ".
			" tc.name AS cat_name, ".
			" ti.main_photo, ".
			" ti.price, ".
			" ti.trade_type, ".
			" ti.title, ".
			" ti.description ".
		" FROM trade_item ti ".
			" INNER JOIN ( ".
				" trade_category tsc LEFT JOIN trade_category tc ON tsc.father_id = tc.id ".
			" ) ON ti.id_category = tsc.id ".
		" WHERE ti.id_trader='".$trader['id_user']."' ".
			" AND ti.moderated='N' ".
			" AND ti.end_date > NOW() "
		);
		
		while($item = $db->fetchArray($result)){
			$items[] = $item;
		}
	
		$page->moduleHeader($header, '');
		
		echo '<p class="titles selling"><span class="titre_menu"><span>O</span>ffres :</span></p>';
		echo '<div class="created_trades selling">';
	

		$i = 0;
		foreach($items as $item) {
			$class = 'line_a';
			if ($i%2) {
				$class = 'line_b';
			}

			$desc = htmlentities(stripslashes($item['description']), ENT_QUOTES, 'UTF-8');
			$cut_desc = substr($desc, 0, 200);
			if($cut_desc != $desc){
				$cut_desc .= '...';
			}

			$tooltip_start = $page->htmlBlock->escapeTipContent(
				'<div class="item-description">');
			
			
			$tooltip_end = $page->htmlBlock->escapeTipContent(
					( ($item['trade_type'] == 'selling') ? '<p class="price">'.($item['price'] != 0 ? 'Prix : <span>'.$item['price'].' €</span>' : '').'</p>' : '' ).
					'<p></p>
					<span class="text-indent"> &nbsp; &nbsp; </span>
					<span class="description_text">'.$cut_desc.'</span>
				</div>'
			);
			
			if($item['trade_type'] == 'selling'){
			
		
				echo '<div class="'.$class.'">';
				
				echo '<div class="trade_name_block left"><a class="trade_name"'.
					' href="'.$core->makeUrl($this->path.'/index.php').'?part=item&amp;content='.$item['id'].'" '.
					' onmouseover="activateToolTips(this, \''.$tooltip_start;
					
				if( isset($item['main_photo']) && $item['main_photo']!='' && $item['main_photo']!=0){
					echo $page->htmlBlock->escapeTipContent('<div class="item-photo">'.
						$fm->preview($item['main_photo'], 1, true).
					'</div>');
				}
				
				echo $tooltip_end.'\')" >'.$this->cutText(stripslashes($item['title']), 60).'</a>
							</div>';
							
				echo '<div class="item_category_block left">'.
					($item['cat_name'] != '' ? '<a class="category" href="'.$core->makeUrl($this->path.'/index.php').'?part=category&content='.$item['cat_id'].'">'.stripslashes($item['cat_name']).'</a> &gt; ' : '').
					'<a class="sub_category" href="'.$core->makeUrl($this->path.'/index.php').'?part=category&content='.$item['subcat_id'].'">'.stripslashes($item['subcat_name']).'</a>'.'
				</div>';

					
				if($myspace) {
				
					echo '<div class="see_trade_block left"><a class="see_trade"'.
						' onclick="hide(toolTips);AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=update_item&amp;item='.$item['id'].'\',\'items_'.$trader['id_user'].'\')" '.
						' >Modifier l\'annonce</a>
								</div>';
						
					echo '<a class="left remove_trade" onclick="'.$box->confirm(
								'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=delete_item&amp;item='.$item['id'].'\', \'items_'.$trader['id_user'].'\')',
								'Etes-vous sûr de vouloir supprimer l\'annonce : "'.stripslashes($item['title']).'" ?').
					'" ><span style="display: none">Retirer l\'annonce</span></a>';
				}
				
				echo '</div>';
				$i++;
			}
		}
		echo '</div>';
		
		echo '<p class="titles buying"><span class="titre_menu"><span>D</span>emandes :</span></p>';
		echo '<div class="created_trades buying">';

		$j = 0;
		foreach($items as $item) {
			$class = 'line_a';
			if ($j%2) {
				$class = 'line_b';
			}
			if($item['trade_type'] == 'buying'){
				echo '<div class="'.$class.'">';
				
				echo '<div class="trade_name_block left"><a class="trade_name left"'.
					' href="'.$core->makeUrl($this->path.'/index.php').'?part=item&amp;content='.$item['id'].'" '.
					' onmouseover="activateToolTips(this, \''.$tooltip_start;


				if( isset($item['main_photo']) && $item['main_photo']!='' && $item['main_photo']!=0){
					echo $page->htmlBlock->escapeTipContent('<div class="item-photo">'.
						$fm->preview($item['main_photo'], 1, true).
					'</div>');
				}

				echo $tooltip_end.'\')" >'.$this->cutText(stripslashes($item['title']), 60).'</a>
						</div>';
						
				echo '<div class="item_category_block left">'.
					($item['cat_name'] != '' ? '<a class="category" href="'.$core->makeUrl($this->path.'/index.php').'?part=category&content='.$item['cat_id'].'">'.stripslashes($item['cat_name']).'</a> &gt; ' : '').
					'<a class="sub_category" href="'.$core->makeUrl($this->path.'/index.php').'?part=category&content='.$item['subcat_id'].'">'.stripslashes($item['subcat_name']).'</a>'.'
				</div>';

					
				if($myspace){
					echo '<div class="see_trade_block left"><a class="left see_trade"'.
						' onclick="hide(toolTips);AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=update_item&amp;item='.$item['id'].'\',\'items_'.$trader['id_user'].'\')" '.
						' >Modifier l\'annonce</a>
							</div>';
						
					echo '<a class="left remove_trade" onclick="'.$box->confirm(
								'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=delete_item&amp;item='.$item['id'].'&amp;trader='.$trader['id_user'].'\', \'items_'.$trader['id_user'].'\')',
								'Etes-vous sûr de vouloir supprimer l\'annonce : "'.stripslashes($item['title']).'" ?').
					'" ><span style="display: none">Retirer l\'annonce</span></a>';
				}
				
				echo '</div>';
				$j++;
			}
		}
		echo '</div>';
	
		$page->moduleFooter();

	}
	
	public function displayUpdateItem($item_id){
		global $core, $db, $page;
		$core->loadClass('fileManager');
		$core->loadClass('tabs');
		$fm = new FileManager();
		if(!isset($page)) $page = new Page();
		$tabs = new Tabs(Array('description' => 'Description', 'photos' => 'Photographies', 'contact' => 'Contact'));

		$item = $db->fetchArray($db->query("SELECT id_trader, trade_type, main_photo, second_photo, price, contact_firstname, contact_lastname, contact_address, contact_phone, contact_mobile, contact_email, title, description FROM trade_item WHERE id='".$item_id."' "));
		
		do{
			$fm_id = rand();
		}while(isset($_SESSION['file_manager_'.$fm_id]));
		
		
	
    echo '<div id="update_item_feedback" style="display:none" >';
		$this->displayInfo('sending');
		echo '</div>';

    echo '<div id="update_missing_alert" style="display:none" >';
		$this->displayError('create_missing');
		echo '</div>';

		$page->moduleHeader('Votre annonce :','');
		
		$tabs->display();
	
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade.php').'" onsubmit="'.
				'if( !trade_verifUpdate(this) ){'.
						'show(gE(\'update_missing_alert\'));'.
						'document.documentElement.scrollTop = 0;'.
						$tabs->onclickGotoTab('description').
					'}else{'.
						'hide(gE(\'update_missing_alert\'));'.
						'show(gE(\'update_item_feedback\'));'.
						'AJAX.submitAndCall(this, true, '.
							'function(){'.
								'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=display_myspace&amp;trader='.$item['id_trader'].'&amp;myspace='.(($core->getUserId() == $item['id_trader']) ? 'true' : 'false').'\',\'trade_content\');'.
							  'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=display_menu_myspace&amp;trader='.$item['id_tader'].'&amp;myspace='.(($core->getUserId() == $item['id_trader']) ? 'true' : 'false').'\',\'trade_menu\');'.
							'}'.
						');'.
					'}'.
					'return false;'.
			'">';
		echo '<div>
				<input type="hidden" name="id_trade" value="'.$this->id.'" />
				<input type="hidden" name="fm_id" value="'.$fm_id.'" />
				<input type="hidden" name="geniutt_action" value="save_item" />
				<input type="hidden" name="save_case" value="update" />
				<input type="hidden" name="user_id" value="'.$core->getUserId().'" />
				<input type="hidden" name="id_item" value="'.$item_id.'" />
			</div>';

		$tabs->separatorStart('description');
		
		echo '
			<p class="form-input">
				<label><span>I</span>ntitulé de l\'annonce * :</label>
				<input type="text" size="40" name="title" onblur="validateInput(this,\'SOMETHING\')" value="'.stripslashes($item['title']).'" />
			</p>
			<p class="form-input">
				<label><span>C</span>ontenu de votre annonce * :</label>
				<textarea name="content" cols="37" onblur="validateInput(this,\'SOMETHING\')" >'.stripslashes($item['description']).'</textarea>
			</p>';
			
		if($item['trade_type'] == 'selling'){
			echo '
				<p class="form-input">
					<label><span>P</span>rix :</label>
					<input type="text" name="price" size="10" value="'.$item['price'].'" /><span> €</span>
				</p>
			';
		}

		echo '<p class="form-submit">
			<input type="submit" value="Modifier" />
		</p>';
		
		$tabs->separatorStop('description');
		$tabs->separatorStart('photos');
	
		if($item['main_photo'] != '' && $item['main_photo'] != 0){
		  echo $fm->preview($item['main_photo'], 1);
			echo '<p class="form-input"><label>Supprimer la photo</label><input type="checkbox" value="Y" name="del_photo1" /></p>';
		}					
		$fm->uploadOrChoose(true, 'image', 'Photo principale :', true, 'photo1');
		if($item['second_photo'] != '' && $item['second_photo'] != 0){
		  echo $fm->preview($item['second_photo'], 1);
			echo '<p class="form-input"><label>Supprimer la photo</label><input type="checkbox" value="Y" name="del_photo2" /></p>';
		}
		$fm->uploadOrChoose(true, 'image', 'Photo supplémentaire :', true, 'photo2');
		
		echo '<p class="form-submit">
			<input type="submit" value="Modifier" />
		</p>';
		
		$tabs->separatorStop('photos');
		$tabs->separatorStart('contact');
		
		echo '<p class="form-input information">Entrez ici les coordonnées de la personne à contacter pour cette annonce (probablement vous) :</p>
			<p class="form-input">
				<label><span>N</span>om * :</label>
				<input type="text" name="contact_lastname" size="20" onblur="validateInput(this, \'SOMETHING\')" value="'.stripslashes($item['contact_lastname']).'" />
			</p>
			<p class="form-input">
				<label><span>P</span>renom * :</label>
				<input type="text" name="contact_firstname" size="20" onblur="validateInput(this, \'SOMETHING\')" value="'.stripslashes($item['contact_firstname']).'" />
			</p>
			<p class="form-input information">Veuillez entrer au moins un moyen de contacter cette personne * :</p>
			<p class="form-input">
				<label><span>A</span>dresse :</label>
				<textarea name="contact_address" cols="37" rows="3">'.stripslashes($item['contact_address']).'</textarea>
			</p>
			<p class="form-input">
				<label><span>T</span>éléphone fixe :</label>
				<input type="text" name="contact_phone" size="14" max_length="14" value="'.stripslashes($item['contact_phone']).'" />
			</p>
			<p class="form-input">
				<label><span>T</span>éléphone mobile :</label>
				<input type="text" name="contact_mobile" size="14" max_length="14" value="'.stripslashes($item['contact_mobile']).'" />
			</p>
			<p class="form-input">
				<label><span>E</span>mail :</label>
				<input type="text" name="contact_email" size="40" max_length="50" value="'.stripslashes($item['contact_email']).'" />
			</p>
		<p class="form-input information">* Les champs marqués d\'une étoile sont obligatoires.</p>
		<p class="form-submit">
			<input type="submit" value="Modifier" />
		</p>';
		$tabs->separatorStop('contact');
		echo '</form>';
	
		$_SESSION['file_manager_'.$fm_id] = serialize($fm);
		$page->moduleFooter();

	}

/*	public function displayUpdateItem($item_id){
		global $core, $db, $page;
		$core->loadClass('magicform');
		if(!isset($page))$page = new Page();
		
		$item = $db->fetchArray($db->query("SELECT id_trader, title, trade_type FROM trade_item WHERE id='".$item_id."' "));
		
		if($item['trade_type'] == 'selling'){
			$only = Array('title', 'description', 'price', 'end_date');
		}else{
			$only = Array('title', 'description', 'end_date');
		}
		
		$mf_options = Array(
			'update' => true,
			'rowToUpdate' => Array('field' => 'id', 'pattern' => $item_id),
			'name' => '',
			'button' => 'Modifier',
			'feedback' => 'info',
			'message' => 'Envoi en cours, veuillez patienter...',
			'once' => true,
			'only' => $only,
			'legend' => Array('title'=>'Titre', 'description'=>'Description', 'price'=>'Prix', 'end_date' => 'Date de péremption' ),
			'onsubmit' => 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&geniutt_action=display_myspace&trader='.$item['id_trader'].'&myspace='.( ($core->getUserId() == $item['id_trader']) ? 'true' : 'false').'\', \'trade_content\');'.
				'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&amp;geniutt_action=display_menu_myspace&amp;trader='.$item['id_tader'].'&amp;myspace='.(($core->getUserId() == $item['id_trader']) ? 'true' : 'false').'\',\'trade_menu\');'
			,
			'describe' => Array(
				'id' =>  Array("type"=>"int(10) unsigned","vide"=>"NO","key"=>"1","default"=>"","extra"=>"auto_increment"),
				'id_category' =>  Array("type"=>"tinyint(3) unsigned","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'id_trader' =>  Array("type"=>"int(10) unsigned","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'moderated' =>  Array("type"=>"enum('Y','N')","vide"=>"NO","key"=>"","default"=>"N","extra"=>""),
				'trade_type' =>  Array("type"=>"enum('selling','buying')","vide"=>"NO","key"=>"","default"=>"selling","extra"=>""),
				'title' =>  Array("type"=>"varchar(100)","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'description' =>  Array("type"=>"text","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'price' =>  Array("type"=>"float unsigned","vide"=>"NO","key"=>"","default"=>"0","extra"=>""),
				'main_photo' =>  Array("type"=>"int(10) unsigned","vide"=>"YES","key"=>"","default"=>"","extra"=>""),
				'second_photo' =>  Array("type"=>"int(10) unsigned","vide"=>"YES","key"=>"","default"=>"","extra"=>""),
				'creation_date' =>  Array("type"=>"date","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'end_date' =>  Array("type"=>"date","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'contact_firstname' =>  Array("type"=>"varchar(20)","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'contact_lastname' =>  Array("type"=>"varchar(30)","vide"=>"NO","key"=>"","default"=>"","extra"=>""),
				'contact_address' =>  Array("type"=>"varchar(250)","vide"=>"YES","key"=>"","default"=>"","extra"=>""),
				'contact_phone' =>  Array("type"=>"varchar(20)","vide"=>"YES","key"=>"","default"=>"","extra"=>""),
				'contact_mobile' =>  Array("type"=>"varchar(20)","vide"=>"YES","key"=>"","default"=>"","extra"=>""),
				'contact_email' =>  Array("type"=>"varchar(50)","vide"=>"YES","key"=>"","default"=>"","extra"=>"")
			),
			'forceType' => Array('price' => Array('type' => 'float')),
			'forceSize' => Array(
				'title' => Array('size' => '50'),
				'price' => Array('size' => '10'),
			),
			'analyse' => false
		);

		$mf = new MagicForm('trade_item', $mf_options);

		$page->moduleHeader('Modification de : '.stripslashes($item['title']),'');
		$mf->display();
		$page->moduleFooter();
	}
	*/

	public function deleteItem($item){
		global $db, $core;
		$trader = $db->result($db->query("SELECT id_trader FROM trade_item WHERE id='".$item."' "), 0);
		if( $core->getUserId() == $trader || $core->verifDroits($this->config['admin_rights']) ){
			$db->query("DELETE FROM trade_item WHERE id='".$item."' ");
			if($core->getUserId() == $trader){
				$this->displayTraderItems($trader, 'true');
			}else{
				$this->displayTraderItems($trader, 'false');
			}
		}else{
			$this->displayError('action');
		}
	}

//** End of methods displaying space of an user **//


//** Methods to add an item **//


	private function displayCreate(){
		global $core, $db, $page;
		$core->loadClass('fileManager');
		$core->loadClass('tabs');
		$fm = new FileManager();
		if(!isset($page)) $page = new Page();
		$tabs = new Tabs(Array('description' => '1. Description', 'photos' => '2. Photographies', 'contact' => '3. Contact'));

		do{
			$fm_id = rand();
		}while(isset($_SESSION['file_manager_'.$fm_id]));
		
		
		if($this->is_trombi){
			$user = $db->fetchArray($db->query("SELECT lastname, firstname, address, address2, postal_code, city, phone, email FROM trombi_trombi WHERE id_user='".$core->getUserId()."' "));
		}

	
    echo '<div id="add_item_feedback" style="display:none" >';
		$this->displayInfo('sending');
		echo '</div>';

    echo '<div id="create_missing_alert" style="display:none" >';
		$this->displayError('create_missing');
		echo '</div>';

		$page->moduleHeader('Votre annonce :','');
		
		$tabs->display();
	
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade.php').'" method="post" enctype="multipart/form-data" onsubmit="'.
				'if( !trade_verifCreate(this) ){'.
						'show(gE(\'create_missing_alert\'));'.
						'document.documentElement.scrollTop = 0;'.
						$tabs->onclickGotoTab('description').';'.
					'}else{'.
						'hide(gE(\'create_missing_alert\'));'.
						'show(gE(\'add_item_feedback\'));'.
						'AJAX.submitAndUpdate(this, true, \'trade_content\');'.
						'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade.php').'?id_trade='.$this->id.'&geniutt_action=display_search_module\',\'trade_menu\');'.
					'}'.
					'return false;'.
			'">';
		echo '<div>
				<input type="hidden" name="id_trade" value="'.$this->id.'" />
				<input type="hidden" name="fm_id" value="'.$fm_id.'" />
				<input type="hidden" name="geniutt_action" value="save_item" />
				<input type="hidden" name="save_case" value="add" />
				<input type="hidden" name="user_id" value="'.$core->getUserId().'" />
			</div>';

		$tabs->separatorStart('description');
		
		echo '<p class="form-input">
				<label><span>T</span>ype d\'annonce * :</label>
				<select name="type" style="z-index: -1;">
					<option value="buying" onclick="hide(gE(\'create_price\'))">Demande</option>
					<option value="selling" onclick="show(gE(\'create_price\'))">Offre</option>
				</select>
			</p>
			';
			$this->displayCreateCategories();
			echo '<p class="form-input">
				<label><span>I</span>ntitulé de l\'annonce * :</label>
				<input type="text" size="40" name="title" onblur="validateInput(this,\'SOMETHING\')" />
			</p>
			<p class="form-input">
				<label><span>C</span>ontenu de votre annonce * :</label>
				<textarea name="content" cols="37" onblur="validateInput(this,\'SOMETHING\')" ></textarea>
			</p>
			<div id="create_price" style="display: none;">';
		echo '
				<p class="form-input">
					<label><span>P</span>rix :</label>
					<input type="text" name="price" size="10" /><span> €</span>
				</p>
			';
		echo '</div>
			<p class="form-input" onmouseover="activateToolTips(this,\'Votre annonce sera valable au maximum '.$this->config['max_duration'].' jours. Si cette annonce ne doit pas rester en ligne aussi longtemps, entrez sa durée de validité (en jours).\')">
				<label><span>D</span>urée de validité :</label>
				<input type="text" name="end_date" max_lentgh="2" size="2" />
				<span>jour(s)</span>
			</p>
			<p class="form-submit">
				<input type="button" onclick="'.$tabs->onclickGotoTab('photos').'" value="Etape suivante" />
			</p>';
		
		$tabs->separatorStop('description');
		$tabs->separatorStart('photos');
		
		echo '<p class="form-input information">Vous pouvez ici joindre des photographies à votre annonce.</p>';
		
		$fm->uploadOrChoose(true, 'image', 'Photo principale :', true, 'photo1');
		$fm->uploadOrChoose(true, 'image', 'Photo supplémentaire :', true, 'photo2');
		
		echo '<p class="form-submit">
				<input type="button" onclick="'.$tabs->onclickGotoTab('description').'" value="Etape précédente" />
				<input type="button" onclick="'.$tabs->onclickGotoTab('contact').'" value="Etape suivante" />
			</p>';
		
		$tabs->separatorStop('photos');
		$tabs->separatorStart('contact');
		
		echo '<p class="form-input information">Entrez ici les coordonnées de la personne à contacter pour cette annonce (probablement vous) :</p>
			<p class="form-input">
				<label><span>N</span>om * :</label>
				<input type="text" name="contact_lastname" size="20" onblur="validateInput(this, \'SOMETHING\')" '.((isset($user))?'value="'.stripslashes($user['lastname']).'"':'').' />
			</p>
			<p class="form-input">
				<label><span>P</span>renom * :</label>
				<input type="text" name="contact_firstname" size="20" onblur="validateInput(this, \'SOMETHING\')" '.((isset($user))?'value="'.stripslashes($user['firstname']).'"':'').' />
			</p>
			<p class="form-input information">Veuillez entrer au moins un moyen de contacter cette personne * :</p>
			<p class="form-input">
				<label><span>A</span>dresse :</label>
				<textarea name="contact_address" cols="37" rows="3">'.((isset($user))?stripslashes($user['address'].'
'.$user['postal_code'].' '.$user['city']):'').'</textarea>
			</p>
			<p class="form-input">
				<label><span>T</span>éléphone fixe :</label>
				<input type="text" name="contact_phone" size="14" max_length="14" '.((isset($user))?'value="'.stripslashes($user['phone']).'"':'').' />
			</p>
			<p class="form-input">
				<label><span>T</span>éléphone mobile :</label>
				<input type="text" name="contact_mobile" size="14" max_length="14"  />
			</p>
			<p class="form-input">
				<label><span>E</span>mail :</label>
				<input type="text" name="contact_email" size="40" max_length="50" '.((isset($user))?'value="'.stripslashes($user['email']).'"':'').' />
			</p>
		<p class="form-input information">* Les champs marqués d\'une étoile sont obligatoires.</p>
		<p class="form-submit">
			<input type="button" onclick="'.$tabs->onclickGotoTab('photos').'" value="Etape précédente" />
			<input type="submit" value="Ajouter" />
		</p>';
		$tabs->separatorStop('contact');
		echo '</form>';
	
		$_SESSION['file_manager_'.$fm_id] = serialize($fm);
		$page->moduleFooter();

	}
	
	private function displayCreateSubCategory($category){
		global $core, $page;
		if(!isset($page)) $page = new Page();
		$form = new Form();
		$box = new Box();
		
		echo '
			<select name="subcategory" style="z-index: -1;" id="create_subcat_select" disabled="disabled" onchange="'.
				'gE(\'new_subcategory\').value = \'\';'.
				'if(this.selectedIndex == this.options.length -1){'.
					$box->prompt('text', 'new_subcategory', 'Nouvelle sous-catégorie :', 'Nom :', "trade_addCatOption(\'create_subcat_select\');").
				'}'.
			'">
		';
		$form->selectFromDb('trade_category', 'name', " WHERE father_id='".$category."' ", 'Sélectionnez une sous-catégorie', '', 'id');
		echo '
				<option value="new">Nouvelle sous-catégorie</option>
			</select>
		';
	}

	private function displayCreateCategories(){
		global $core, $page;
		if(!isset($page)) $page = new Page();
		$form = new Form();
		$box = new Box();

		echo '
			<p class="form-input">
				<label><span>C</span>atégorie * :</label>
				<select id="create_cat_select" style="z-index: -1;" name="category" onchange="'.
					'gE(\'new_category\').value = \'\';'.
					'gE(\'new_subcategory\').value = \'\';'.
					'if(this.selectedIndex == this.options.length -1){'.
						$box->prompt('text', 'new_category', 'Nouvelle catégorie :', 'Nom :', "trade_addCatOption(\'create_cat_select\');").
					'}'.
					'trade_createSubCateg(this, \'create_subcat_select\', \''.$this->str_cat.'\');'.
				'">
		';
		$form->selectFromDb('trade_category', 'name', ' WHERE father_id = 0 ', 'Sélectionnez une catégorie', '', 'id');
		echo '
					<option value="new">Nouvelle catégorie</option>
				</select>
			</p>
			<p class="form-input">
				<label><span>S</span>ous-catégorie :</label>
				<span id="create_subcategory">
				';
		$this->displayCreateSubCategory('none');
		echo '
				</span>
				<span>Optionnel</span>
			</p>
			
			<div>
				<input id="new_category" type="hidden" name="new_category" />
				<input id="new_subcategory" type="hidden" name="new_subcategory" />
			</div>';
	}

	public function saveItem($post){
		global $core, $db;
		$core->loadClass('form');
		$core->loadClass('date');
		$core->loadClass('mail');
		$core->loadClass('fileManager');
		$form = new Form();
		$mail = new Mail();
		
		$fm = unserialize($_SESSION['file_manager_'.$post['fm_id']]);
		
		$date = new Date();
		$end_date = new Date();
		$end_date = $end_date->returnDate($end_date->addDays( $this->config['max_duration'] ));
		
		$errors = Array();
		
		if( $post['price'] != '' && ( $post['type'] == 'selling' || $post['type'] == '' ) ){
			$price = $post['price'];
		}else{
			$price = '';
		}
		
		$photo_ids = $fm->saveOrGet('image', 'log', 'log');

		if($post['save_case'] == 'update'){
			$result = $db->query("UPDATE trade_item SET ".
				" title='".$db->escapeString($post['title'])."', ".
				" description='".$db->escapeString($post['content'])."', ".
				( (isset($photo_ids['photo1']) && $photo_ids['photo1'] != '') ? " main_photo='".$photo_ids['photo1']."', " : (($post['del_photo1'] == 'Y')?" main_photo='', ":'') ).
				( (isset($photo_ids['photo2']) && $photo_ids['photo2'] != '') ? " second_photo='".$photo_ids['photo2']."', " : (($post['del_photo2'] == 'Y')?" second_photo='', ":'') ).
				" price='".$price."', ".
				" contact_firstname='".$db->escapeString($post['contact_firstname'])."', ".
				" contact_lastname='".$db->escapeString($post['contact_lastname'])."', ".
				" contact_address='".$db->escapeString($post['contact_address'])."', ".
				" contact_phone='".$db->escapeString($post['contact_phone'])."', ".
				" contact_mobile='".$db->escapeString($post['contact_mobile'])."', ".
				" contact_email='".$db->escapeString($post['contact_email'])."' ".
				" WHERE id='".$post['id_item']."' ");
		}
		
		if($post['save_case'] == 'add'){
			$cat_id = $categ_id = $subcat_id = '';
		
			if( ( $post['category'] == '' || $post['category'] == 'NULL' || $post['category'] == 'new' || $post['category'] == '0') && $post['new_category'] != '' ){
				if(!$db->query("INSERT INTO trade_category (name, father_id) VALUES ( '".$db->escapeString($post['new_category'])."', 0 ) ")){
					$error = 'failed_query';
				}
				$categ_id = $db->insertId();
			}
			elseif( ($post['category'] == '' || $post['category'] == 'NULL' || $post['category'] == 'new' || $post['category'] == '0') && $post['new_category'] == '' ){
				$errors[] = 'no_categ';
			}else{
				$cat_id = $categ_id = $post['category'];
			}

			if( !isset($error) && !isset($errors[0]) && ( $post['subcategory'] == '' || $post['subcategory'] == 'NULL' || $post['subcategory'] == 'new' || $post['subcategory'] == '0') && $post['new_subcategory'] != '' ){
				if(!$db->query("INSERT INTO trade_category (name, father_id) VALUES ('".$db->escapeString($post['new_subcategory'])."', '".$categ_id."') ")){
					$error = 'failed_query';
				}
				$categ_id = $db->insertId();
			}elseif( !isset($error) && !isset($errors[0]) && $post['subcategory'] != '' && $post['subcategory'] != 'NULL' && $post['subcategory'] != 'new' && $post['subcategory'] != '0'){
				$subcat_id = $categ_id = $post['subcategory'];
			}

			if( $post['end_date'] != '' && $form->verifForm($post['end_date'], 'int') ){
				$end_date = $date->returnDate($date->addDays( $post['end_date'] ));
			}

			if( $post['title'] == ''){
				$errors[] = 'no_title';
			}

			if( $post['content'] == ''){
				$errors[] = 'no_content';
			}

			if( $post['contact_firstname'] == '' || $post['contact_lastname'] == ''){
				$errors[] = 'no_contact_name';
			}
			
			if( $post['contact_address'] == '' && $post['contact_phone'] == '' && $post['contact_mobile'] == '' && $post['contact_email'] == ''){
				$errors[] = 'no_contact_way';
			}
	
			if( count($errors) == 0 && !isset($error) ){
			
				$result = $db->query("INSERT INTO trade_item ".
				" (id_category, id_trader, moderated, trade_type, title, description, price, creation_date, end_date, contact_firstname, contact_lastname, contact_address, contact_phone, contact_mobile, contact_email, main_photo, second_photo) VALUES ( ".
				" '".$categ_id."', ".
				" '".$core->getUserId()."', ".
				" '".( ($this->config['auto_valid']) ? 'N' : 'Y' )."', ".
				" '".$post['type']."', ".
				" '".$db->escapeString($post['title'])."', ".
				" '".$db->escapeString($post['content'])."', ".
				" '".$price."', ".
				" NOW(), ".
				" '".$end_date."', ".
				" '".$db->escapeString($post['contact_firstname'])."', ".
				" '".$db->escapeString($post['contact_lastname'])."', ".
				" '".$db->escapeString($post['contact_address'])."', ".
				" '".$db->escapeString($post['contact_phone'])."', ".
				" '".$db->escapeString($post['contact_mobile'])."', ".
				" '".$db->escapeString($post['contact_email'])."', ".
				" '".$photo_ids['photo1']."', ".
				" '".$photo_ids['photo2']."' ".
				" )");

				$item_id = $db->insertId();
				
				if($result){
					$this->displayInfo('item_added');
					
					//follow-up of categories by mail
					$categ_names = $db->query("SELECT name FROM trade_category WHERE id='".$cat_id."' OR id='".$subcat_id."' ORDER BY id ASC");
					$row = $db->fetchArray($categ_names);
					$categ_name = stripslashes($row['name']);
					if($db->numRows($categ_names) > 1){
						$row = $db->fetchArray($categ_names);
						$subcateg_name = stripslashes($row['name']);
					}
				
					$from = $core->getConfig('mailWebmaster');
					$subject = 'Nouvelle annonce';
					$message = 'Une nouvelle annonce a été créée dans la catégorie : '.$categ_name.( (isset($subcateg_name)) ? ' > '.$subcateg_name.'.' : '.').'
'.'Vous pouvez y accèder en cliquant ici : '.$core->getConfig('baseUrl').'/'.$this->path.'/index.php?part=item&content='.$item_id.'


Annonce :

Intitulé : '.( ($post['type'] == 'selling') ? 'Vend :' : 'Achète :' ).' '.stripslashes($post['title']).'
Description : '.$post['content'].'
'.( ($post['type'] == 'selling' && $post['price'] != 0) ? 'Prix : '.stripslashes($post['price']).'€' : '' );
				
					$users_to_alert = $db->query("SELECT lastname, firstname, email FROM trombi_trombi WHERE id_user IN (SELECT DISTINCT id_trader FROM trade_alert WHERE id_category='".$categ_id."' AND mail='Y' ) ");
					while($user = $db->fetchArray($users_to_alert)){
						$to = stripslashes($user['email']);
						$mail->send($from, $to, $message, $subject);
					}
				}
			
			}else{
				$this->displayError($error);
				$this->displayError($errors);
			}
		}
		
		if(!$result){
			$this->displayError('failed_query');
		}

		//update of categories in form selects
		$this->loadCategories();

		$this->displayLastItems();
		
	}

//** End of methods to add an item **//

/**** End of methods displaying content of div : "trade_content" ****/



/**** Here are methods used to notice user about all you want to ****/


	private function displayError($error=''){
		global $page;
		if(!isset($page)) $page = new Page();
		
		if(is_array($error)){
		
			$msg = '';
			foreach($error as $er){
				switch($er){
					case 'no_categ':
						$msg .= '<li>aucune catégorie n\'a été renseignée</li>';
						break;
					case 'no_title':
						$msg .= '<li>aucun titre n\'a été renseigné</li>';
						break;
					case 'no_content':
						$msg .= '<li>aucune description n\'a été renseignée</li>';
						break;
					case 'no_contact_name':
						$msg .= '<li>le contact fourni n\'a pas de nom ou prénom</li>';
						break;
					case 'no_contact_way':
						$msg .= '<li>aucun moyen de joindre le contact n\'a été fourni</li>';
						break;
					default:
						//empty
						break;
				}
			}

			$page->msgError('Des erreurs ont été commises :<ul>'.$msg.'</ul>');
			
		}else{
	
			switch($error){
				case 'action':
					$page->msgError('Vous avez probablement tenté d\'effectuer une action non permise. Veuillez utiliser les liens du site pour naviguer.');
					break;
				case 'empty_field':
					$page->msgError('Veuillez remplir au moins un champ.');
					break;
				case 'create_missing':
					$page->msgError('Veuillez renseigner les champs marqués d\'une étoile.');
					break;
				case 'failed_query';
					$page->msgError('L\'envoi de la requête a échoué.');
					break;
				default:
					$page->msgError('Une erreur est apparue.');
					break;
			}
		}
	}

	private function displayWarning($warning=''){
		global $page;
		if(!isset($page)) $page = new Page();
		
		if(is_array($warning)){
			
			$msg = '';
			foreach($warning as $warn){
				switch($warn){
					case 'price':
						$msg .= '<li>Le prix entré n\'est pas valide. La recherche a été effectuée sans en tenir compte. Exemples de prix valides : 1423.6 - 768,90 - 4 - 5,61</li>';
						break;
					default:
						//empty
						break;
				}
			}

			$page->msgWarning('Des erreurs ont été commises :<ul>'.$msg.'</ul>');
		}
		
	}

	private function displayInfo($info=''){
		global $page;
		if(!isset($page)) $page = new Page();

		switch($info){
			case 'no_result':
				$page->msgInfo("Aucune annonce ne correspond à votre requête.");
				break;
			case 'no_item':
				$page->msgInfo("Il n'y a aucune annonce actuellement.");
				break;
			case 'sending':
				$page->msgInfo('Envoi en cours, veuillez patienter...');
				break;
			case 'item_added':
				$msg = 'Annonce ajoutée.';
				if(!$this->config['auto_valid']){
					$msg .= ' Elle sera visible après modération.';
				}
				$page->msgInfo($msg);
				break;
			default:
				//empty
				break;
		}

	}


/**** End of methods used to notice user about all you want to ****/

}//end of Trade class
?>
