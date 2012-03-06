<?php
class Trade_admin {
	
	private $id;

	private $config;

	private $path;

	private $str_cat;

	public final function __construct(){
		global $core;
		
		$this->path = 'applications/trade/admin';
		
		include($core->getConfig('baseServer').'/'.$this->path.'/config_admin_trade.inc');
		include($core->getConfig('baseServer').'/'.$this->path.'/../config_trade.inc');

		//pay attention to the order, possible identical keys are set to the second value
		$this->config = array_merge($config_trade, $config_admin_trade);
		
		$this->loadCategories();
		$this->is_trombi = $core->isTrombi();
		
		$this->id = rand(1000000,9999999);
		$_SESSION['trade_admin_'.$this->id] = serialize($this);
	}

	public function displayContent($item = null){
		global $page;
		if(!isset($page))$page = new Page();

		echo '<div id="trade_admin_content">';
		if( $item != null ){
			$this->displayUpdateItem($item);	
		}else{
			$this->displayItems(1);
		}
		echo '</div>';
	}
	
	public function displayMenu(){
		global $page,$core;
		if(!isset($page))$page = new Page();

		$page->moduleHeader($this->config['title'],'');
		echo '<ul class="panel">
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_items&amp;cur_page=1\',\'trade_admin_content\')">Liste des annonces</a></li>
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_add_item\',\'trade_admin_content\')">Ajouter une annonce</a></li>
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_search_item\',\'trade_admin_content\')">Rechercher une annonce</a></li>
			<li>--</li>
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_categories\',\'trade_admin_content\')">Liste des catégories et sous-catégories</a></li>
			<li><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_add_category\',\'trade_admin_content\')">Ajouter une catégorie</a></li>
		</ul>';
		$page->moduleFooter();
	}

	
	public function displaySummary($items_nb, $period){
		global $page, $core, $db;
		if(!isset($page))$page = new Page();

		$result = $db->query("SELECT id, title FROM trade_item WHERE moderated='Y' AND end_date > NOW() AND DATEDIFF(NOW(), creation_date ) <= 7 ORDER BY creation_date ASC LIMIT ".$items_nb." ");
		$nb = $db->numRows($result);

		$page->moduleHeader(ucfirst($this->config['title']).' à modérer ( '.$nb.' )', '');
		echo '<h4>Dernières annonces à modérer :</h4>';
		
		if($db->numRows($result) != 0){
			$i = 0;
			$line_style = '';
			while($row = $db->fetchArray($result)) {
				if ($i%2) {
					$line_style = 'line_1';
				} else {
						$line_style = 'line_2';
				}
				echo '<div class="admin_home_item_list '.$line_style.'"><a href="'.$core->makeUrl('admin/index.php').'?part=app&amp;content=trade&amp;option='.$row['id'].'">'.$row['title'].'</a></div>';
				$i++;
			}
		}else{
			echo '<div><i>Aucune annonce à modérer ces '.$period.' derniers jours.</i></div>';
		}
		
		echo '<div class="end_list_clear"></div><div>';
		$page->adminLink('app', 'trade', $this->config['admin_rights']);
		echo '</div>';
		
		$page->moduleFooter();
	}


	public function displaySearchItem(){
		global $page,$core;
		if(!isset($page))$page = new Page();
		$form = new Form();
		
		$page->moduleHeader('Recherche','');
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade_admin.php').'" onsubmit="'.
      	'AJAX.submitAndUpdate(this, false, \'trade_admin_content\');'.
    	  'return false;'.
  	  '">
				<div>
 					<input type="hidden" name="id_admin" value="'.$this->id.'" />
   		 	  <input type="hidden" name="geniutt_action" value="search_item" />
		  	</div>
	      <p class="form-input">
					<label><span>T</span>ype d\'annonce :</label>
					<select name="type">
					  <option value="all" selected="selected">Tous les types</option>
						<option value="selling">Offre</option>
						<option value="buying">Demande</option>
					</select>
				</p>
				<p class="form-input">
					<label><span>C</span>atégorie :</label>
				  <select name="category" onchange="trade_searchSubCateg(this, \'cat_select\',\''.$this->str_cat.'\')">
				    <option value="all" selected="selected">Toutes les catégories</option>
		';
    $form->selectFromDb('trade_category', 'name', " WHERE father_id = 0 ", '', '', 'id');
		echo '
		      </select>
		    </p>
				<p class="form-input" id="search_subcategory">
		';
		$this->displaySearchSubCategory('none');
		echo '
    	  </p>
			  <p class="form-input">
					<label><span>L</span>\'intitulé contient :</label>
					<input type="text" name="title" />
				</p>
				<p class="form-input">
					<label><span>L</span>\'annonce contient :</label>
					<input type="text" name="description" />
				</p>
				<p class="form-input">
					<label><span>P</span>rix inférieur à :</label></p>
					<input type="text" name="price" /><span>&nbsp;€</span>
				</p>
				<p class="form-input">
					<label><span>L</span>\'annonce est :</label></p>
					<input type="radio" name="moderated" value="Y" /><span>modérée</span>
					<input type="radio" name="moderated" value="N" /><span>non modérée</span>
					<input type="radio" name="moderated" value="all" checked="checked" /><span>indifférent</span>
				</p>
				<p class="form-submit">
					<input type="submit" value="Chercher" />
				</p>
			</form>';
																																							
		$page->moduleFooter();
	}

	private function displaySearchSubCategory($category){
    global $core, $page;
	  if(!isset($page)) $page = new Page();
	  $form = new Form();

	  echo '<label><span>S</span>ous-catégorie :</label>
		<select name="subcategory" id="cat_select">
	    <option value="all" selected="selected">Toutes les sous-catégories</option>
		';
		$form->selectFromDb('trade_category', 'name', " WHERE father_id='".$category."' ", '', '', 'id');
		echo '
		</select>
		';
	}
																											

	public function displayItems($cur_page=1){
		global $page, $core, $db;
		if(!isset($page))$page = new Page();

		$page->moduleHeader('Annonces actuelles','');
		//pages management
    $start_row = ($cur_page - 1) * ($this->config['items_per_page']);
		
		$request = "SELECT ti.id, ti.id_trader, ti.main_photo, ti.moderated, tr.firstname, tr.lastname, ti.title, ti.description FROM trade_item ti INNER JOIN trombi_trombi tr ON ti.id_trader=tr.id_user WHERE ti.end_date > NOW() ORDER BY id DESC ";
		$result = $db->query($request." LIMIT ".$start_row.",".$this->config['items_per_page']);
		
		echo '<table class="data_table" width="95%">
			<tr>
				<th width="80%">Annonce</th>
				<th width="10%">Modérée</th>
				<th width="10%">Supprimer</th>
			</tr>';
		while($item = $db->fetchArray($result)){
			$this->displayItem($item, 'list',$cur_page);
		}
		echo '</table>';
	
		echo '<p>';
		$page->htmlBlock->pagesList($request, $this->config['items_per_page'], $cur_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_items&amp;cur_page=[#]\',\'trade_admin_content\')');
		echo '</p>';
		$page->moduleFooter();
	}

	private function displayItem($item, $case, $cur_page){
		global $core;
		$core->loadClass('box');
		$core->loadClass('htmlblock');
		$core->loadClass('filemanager');
		$box = new Box();
		$hb = new HtmlBlock();
		$fm = new FileManager();

		$class = '';
		if($item['moderated'] == 'Y'){
			$class = 'class="no_valid"';
		}
		
		echo '<tr '.$class.'>
			<td><a '.
				' onclick="hide(toolTips);AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_update_item&amp;item='.$item['id'].'&amp;case='.$case.'&amp;cur_page='.$cur_page.'\',\'trade_admin_content\');"'.
				' onmouseover="activateToolTips(this, \''.htmlentities('<div>');
		echo $hb->escapeTipContent($fm->preview($item['main_photo'], 1, true).'</div><div>'.stripslashes($item['description'])).'</div>\')" '.
			'>'.stripslashes($item['title']).'</a></td>
			<td align="center"><a onclick="'.
				'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=moderate_item&amp;item='.$item['id'].'&amp;moderated='.(($item['moderated']=='Y')?'N':'Y').'&amp;case='.$case.'&amp;cur_page='.$cur_page.'\',\'trade_admin_content\');'.
			'">'.(($item['moderated']=='Y')?'Y':'N').'</a></td>
			<td align="center"><a class="delete" onclick="'.$box->confirm(
				'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=delete_item&amp;item='.$item['id'].'&amp;case='.$case.'&amp;cur_page='.$cur_page.'\',\'trade_admin_content\');',
				'Etes-vous sûr de vouloir supprimer l\'annonce : '.$item['title'].' ?').
			'">Supprimer</a></td>
		</tr>';
	}

	public function displayQueryResult($data, $current_page){
    global $db, $core, $page;
	  if(!isset($page)) $page = new Page();
	  $form = new Form();

	  //this array will be useful to notice several errors to the user
	  $warning = Array();

		//changeing of page, search is recovered from session
		if($data == 'session'){
		  $data = unserialize($_SESSION['trade_admin_'.$this->id.'_search']);
		//first page, search comes from formulary, we save search criteria
		}else{
		  $_SESSION['trade_admin_'.$this->id.'_search']= serialize($data);
		}
		
		//pages management
    $start_row = ($current_page - 1) * ($this->config['items_per_page']);

		//a non-empty field is required
    if( ($data['type'] != 'all') || ($data['category'] != 'all') || ($data['subcategory'] != 'all') || ($data['title'] != '') || ($data['description'] != '') || ($data['price'] != '') || ($date['moderated'] != 'all' ) ){

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
			    $price_where = " AND price < '".$data['price']."' ";
			  }else{
			    $warning[] = 'price';
			  }
			}
			
			$moderated_where = '';
      if( $data['moderated'] != 'all' ){
			  $moderated_where = " AND moderated = '".$data['moderated']."' ";
			}
		
			$where = $cat_where.
      	$type_where.
				"AND title LIKE '%".$data['title']."%' ".
				"AND description LIKE '%".$data['content']."%' ".
				$price_where.
				$moderated_where;

		//all fields are empty
		}else{
		  $where = false;
		}

		if($where){

    	//only valid items are displayed
			$where .= " AND end_date > NOW() ";
			$request = "SELECT ti.id, ti.id_trader, ti.main_photo, ti.moderated, tr.firstname, tr.lastname, ti.title, ti.description FROM trade_item ti INNER JOIN trombi_trombi tr ON ti.id_trader=tr.id_user WHERE 1 ".$where;

			$result_search = $db->query($request." ORDER BY id DESC LIMIT ".$start_row.",".$this->config['items_per_page']);

			if($db->numRows($result_search) != 0){
			  //if there is non critical errors, displays these errors
			  if(isset($warning[0]))$this->displayWarning($warning);

					$page->moduleHeader('Résultats de la recherche :', '');
					
					echo '<table class="data_table" width="95%">
						<tr>
							<th>Annonce</th>
							<th>Modérée</th>
							<th></th>
						</tr>';
	        while($item = $db->fetchArray($result_search)){
	          $this->displayItem($item, 'search', $current_page);
	        }
					echo '</table>';
					
					//displays pages
					echo '<p>';
          $page->htmlBlock->pagesList($request, $this->config['items_per_page'], $current_page, 'onclick', 'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&geniutt_action=search_item&criteria_place=session&cur_page=[#]\',\'trade_admin_content\')');
					echo '</p>';
				
					$page->moduleFooter();

			}else{
			  $this->displayInfo('no_result');
			}
		}else{
		  $this->displayError('empty_field');
		}
	}
																																																													

	public function displayCategories(){
		global $page, $core, $db;
		if(!isset($page))$page = new Page();

		$cat = $subcat = Array();
		$result = $db->query("SELECT id, name, father_id FROM trade_category WHERE 1 ORDER BY name ASC");
		while($row = $db->fetchArray($result)){
			if($row['father_id'] == '' || $row['father_id'] == '0'){
				$cat[$row['id']] = Array('name'=>$row['name']);
			}else{
				$subcat[$row['id']] = Array('name'=>$row['name'], 'father_id'=>$row['father_id']);
			}
		}
		
		$page->moduleHeader('Catégories','');
		echo '<div class="categories_list_block">';
		foreach($cat as $id => $att){
			echo '
				<div class="category_line">
						<div class="category_block left">
							<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_update_category&amp;cat='.$id.'&amp;main=true\',\'trade_admin_content\')">'.$att['name'].'</a>
						</div>

						<a class="delete left" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_delete_category&amp;cat='.$id.'&amp;name='.$att['name'].'&amp;main=true\',\'trade_admin_content\')">Supprimer</a>
							
						<div class="clear"></div>
					
				</div>
				<div class="clear"></div>';
			$sub_cat = 1;
			foreach($subcat as $sid => $satt){
				if($satt['father_id'] == $id) {

					if ($sub_cat%2) {
						$cat_color = '1';
					} else {
							$cat_color = '2';
						}

					echo '<div class="sub_category_line color'.$cat_color.'">
									<div class="sub_category_block left">
											<a onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_update_category&amp;cat='.$sid.'&amp;main=false\',\'trade_admin_content\')">'.$satt['name'].'</a>
									</div>
											
									<a class="delete left" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_delete_category&amp;cat='.$sid.'&amp;name='.$satt['name'].'&amp;main=false\',\'trade_admin_content\')">Supprimer</a>

									<div class="clear"></div>

								</div>
					<div class="clear"></div>';
					$sub_cat++;
				}
			}
		}
		echo '</div>';
		$page->moduleFooter();
	}

	public function displayUpdateCategory($cat, $main){
		global $db, $core, $page;
		if(!isset($page))$page = new Page();
		$form = new Form();

		$main_cat = ($main == 'true') ? true : false;

		$cat = $db->fetchArray($db->query("SELECT id, name, father_id FROM trade_category WHERE id='".$cat."' "));

		$page->moduleHeader('Modification de la '.(($main_cat)?'':'sous-').'catégorie : '.$cat['name'], '');
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade_admin.php').'" '.
			' onsubmit="AJAX.submitAndUpdate(this, false, \'trade_admin_content\');return false;" '.
		'>
			<div>
				<input type="hidden" name="id_admin" value="'.$this->id.'" />
				<input type="hidden" name="geniutt_action" value="update_category" />
				<input type="hidden" name="id_cat" value="'.$cat['id'].'" />
			</div>
			<p class="form-input">
				<label>Intitulé :</label>
				<input type="text" name="cat_name" value="'.$cat['name'].'" />
			</p>
			';
		if(!$main_cat){
			echo '
			<p class="form-input">
				<label>Sous-catégorie de :</label>
				<select name="cat_category">
					<option value="NULL" >N\'est pas une sous-catégorie</option>
				';
			$form->selectFromDb('trade_category','name'," WHERE father_id = 0 ",'',$cat['father_id'],'id');
			echo '
				</select>
			</p>
			';
		}
		echo '
			<p class="form-submit">
				<input type="submit" value="Modifier" />
			</p>
		</form>';
	}
	
	public function displayDeleteCategory($cat_id, $cat_name, $main){
		global $db, $core, $page;
		if(!isset($page))$page = new Page();

		$main_cat = ($main == 'true') ? true : false;
		
		$page->moduleHeader('Suppression de la '.(($main_cat)?'':'sous-').'catégorie : '.$cat_name, '');
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade_admin.php').'" '.
			' onsubmit="AJAX.submitAndUpdate(this, false, \'trade_admin_content\');return false;" '.
		'>
			<div>
				<input type="hidden" name="id_admin" value="'.$this->id.'" />
				<input type="hidden" name="geniutt_action" value="delete_category" />
				<input type="hidden" name="id_cat" value="'.$cat_id.'" />
			</div>
			';
			
		if($main_cat){
			echo '
			<p class="form-input">Veuillez créer ou choisir une catégorie dans laquelle placer les annonces et les sous_catégorie de la catégorie "'.$cat_name.'".<br />Si vous n\'en choisissez aucune, les annonces seront modérées jusqu\'à ce que vous les replaciez une à une.</p>
				';
		}else{
			echo '
			<p class="form-input">Veuillez créer ou choisir une catégorie dans laquelle placer les annonces de la catégorie "'.$cat_name.'".<br />Si vous n\'en choisissez aucune, les annonces seront modérées jusqu\'à ce que vous les replaciez une à une.</p>
				';
		}
		
		$this->displayCreateCategory($cat_id, $main_cat);
		echo '
			<p class="form-submit">
				<input type="submit" value="Supprimer" />
				<input type="button" value="Annuler" onclick="AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/trade_admin.php').'?id_admin='.$this->id.'&amp;geniutt_action=display_categories\',\'trade_admin_content\')" />
			</p>
		</form>';
	}

	private function displayCreateCategory($cat_id, $main_cat){
		global $core;
		$core->loadClass('form');
		$form = new Form();
		
		echo '
		<p class="form-input" id="create_select_category">
			<label><span>C</span>atégorie :</label>
			<select name="category">
				<option value="NULL" >Aucune catégorie</option>
				';
		if($main_cat){
			$form->selectFromDb('trade_category', 'name', " WHERE father_id = 0 AND id!='".$cat_id."' ", '', '', 'id');
		}else{
			$form->selectFromDb('trade_category', 'name', " WHERE id!='".$cat_id."' ", '', '', 'id');
		}
		echo '
				<option value="new" onclick="'.
					'show(gE(\'create_new_category\'));'.
					'hide(gE(\'create_select_category\'));'.
					'this.parentNode.selectedIndex=\'\';'.
				'">Nouvelle catégorie</option>
			</select>
		</p>
		
		<div id="create_new_category" style="display: none;">
			<p class="form-input">
				<label><span>N</span>ouvelle catégorie :</label>
				<input type="text" size="40" name="new_category" onblur="validateInput(this, \'SOMETHING\')" />
				<span><a onclick="'.
					'show(gE(\'create_select_category\'));'.
					'hide(gE(\'create_new_category\'));'.
					'this.parentNode.selectedIndex=\'\';'.
				'">Retour à la liste</a></span>
			</p>
			';
		if(!$main_cat){
			echo '<p class="form-input">
				<label>Est une sous-catégorie de :</label>
				<select name="father_category">
					<option value="NULL" >N\'est pas une sous-catégorie</option>
				';
			$form->selectFromDb('trade_category', 'name', " WHERE father_id = 0 ", '', '', 'id');
			echo '
				</select>
			</p>';
		}
		echo '
		</div>';

	}

	public function displayAddCategory(){
		global $db, $core, $page;
		if(!isset($page))$page = new Page();
		$form = new Form();

		$page->moduleHeader('Nouvelle catégorie :', '');
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade_admin.php').'" '.
			' onsubmit="AJAX.submitAndUpdate(this, false, \'trade_admin_content\');return false;" '.
		'>
			<div>
				<input type="hidden" name="id_admin" value="'.$this->id.'" />
				<input type="hidden" name="geniutt_action" value="add_category" />
			</div>
			<p class="form-input">
				<label>Intitulé :</label>
				<input type="text" name="cat_name" />
			</p>
			<p class="form-input" id="create_select_category">
				<label>Est une sous-catégorie de :</label>
				<select name="cat_category">
					<option value="NULL" >N\'est pas une sous-catégorie</option>
			';
		$form->selectFromDb('trade_category', 'name', " WHERE father_id = 0 ", '', '', 'id');
		echo '
					<option value="new" onclick="'.
						'show(gE(\'create_new_category\'));'.
						'hide(gE(\'create_select_category\'));'.
						'this.parentNode.selectedIndex=0;'.
					'">Nouvelle catégorie</option>
				</select>
			</p>
			<p class="form-input" id="create_new_category" style="display: none;">
				<label>Est-une sous-catégorie de :</label>
				<input id="new_category" type="text" size="40" name="new_category" onblur="validateInput(this, \'SOMETHING\')" />
				<span><a onclick="'.
					'show(gE(\'create_select_category\'));'.
					'hide(gE(\'create_new_category\'));'.
					'gE(\'new_category\').value=\'\';'.
				'">Retour à la liste</a></span>
			</p>
			<p class="form-submit">
				<input type="submit" value="Créer" />
			</p>
		';
		
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

		$_SESSION['trade_admin_'.$this->id] = serialize($this);
	}


	public function deleteItem($item, $case, $cur_page){
		global $db;
		$db->query("DELETE FROM trade_item WHERE id='".$item."' ");

		switch($case){
			case 'list':
				$this->displayItems($cur_page);
				break;
			case 'search':
				$this->displayQueryResult('session', $cur_page);
				break;
			default:
				//empty
				break;
		}
	}
	
	public function deleteCategory($post){
		global $db;

		$moderated = '';
		if($post['category'] == '' && $post['new_category'] != '' ){
			$father_category = " 0 ";
			if($post['father_category'] != 'NULL' && $post['father_category'] != '' && $post['father_category'] != '0'){
				$father_category = " '".$post['father_category']."' ";
			}
			
			if(!$db->query("INSERT INTO trade_category (name, father_id) VALUES ('".$db->escapeString($post['new_category'])."', ".$father_category." )")){
				$error = 'failed_query';
			}
			
			$categ_id = " '".$db->insertId()."' ";
			
		//"new" case can't appear, but prudence doesn't cost anything
		}elseif($post['category'] != 'NULL' && $post['category'] != 'new' && $post['category'] != '0'){
			$categ_id = "  = '".$post['category']."' ";
		}else{
			$categ_id = " = 0 ";
			$moderated = ", moderated='Y' ";
		}
	
		if(!isset($error) || $error == ''){
			//all subcategories depending on this category are moved in an other category chosen by the webmaster
			if(!$db->query("UPDATE trade_category SET father_id ".$categ_id." WHERE father_id='".$post['id_cat']."' ")){
				$error = 'failed_query';
			}
		}
		
		if(!isset($error) || $error == ''){
			//all items depending on this category are moved in an other category chosen by the webmaster
			if(!$db->query("UPDATE trade_item SET id_category ".$categ_id." ".$moderated." WHERE id_category='".$post['id_cat']."' ")){
				$error = 'failed_query';
			}
		}
		
		if(!isset($error) || $error == ''){
			//category is deleted
			if(!$db->query("DELETE FROM trade_category WHERE id='".$post['id_cat']."' ")){
				$error = 'failed_query';
			}
		}
	
		if(isset($error))$this->displayError($error);
		$this->displayCategories();
	}
	
	public function moderateItem($item, $moderate, $case, $cur_page){
		global $db;
		$db->query("UPDATE trade_item SET moderated='".$moderate."' WHERE id='".$item."' ");
	
		switch($case){
			case 'list':
				$this->displayItems($cur_page);
				break;
			case 'search':
				$this->displayQueryResult('session', $cur_page);
				break;
			default:
				//empty
				break;
		}
	}

	public function displayUpdateItem($id_item, $case, $cur_page){
		global $core, $db, $page;
		
		$core->loadClass('fileManager');
		$core->loadClass('date');
		$core->loadClass('tabs');
		
		if(!isset($page))$page = new Page();
		
		$tabs = new Tabs(Array('description' => '1. Description', 'photos' => '2. Photographies', 'contact' => '3. Contact'));
		$fm = new FileManager();
		$date = new Date();
		
		do{
			$fm_id = rand();
		}while(isset($_SESSION['file_manager_'.$fm_id]));
		
		$item = $db->fetchArray($db->query("SELECT tc.father_id AS cat, ti.id_category AS subcat, ti.id_trader, ti.moderated, ti.trade_type, ti.price, ti.main_photo, ti.second_photo, ti.end_date, ti.contact_firstname, ti.contact_lastname, ti.contact_address, ti.contact_phone, ti.contact_mobile, ti.contact_email, ti.title, ti.description FROM trade_item ti, trade_category tc  WHERE ti.id='".$id_item."' AND (ti.id_category=tc.id OR ti.id_category = 0) "));

		
    echo '<div id="update_item_feedback" style="display:none" >';
		$this->displayInfo('sending');
		echo '</div>';

    echo '<div id="update_missing_alert" style="display:none" >';
		$this->displayError('create_missing');
		echo '</div>';

		$page->moduleHeader('Modification de l\'annonce : '.$item['title'],'');
		
		$tabs->display();
		
		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade_admin.php').'" onsubmit="'.
				'if( !trade_verifCreate(this) ){'.
						'show(gE(\'update_missing_alert\'));'.
					'}else{'.
						'hide(gE(\'update_missing_alert\'));'.
						'show(gE(\'update_item_feedback\'));'.
						'AJAX.submitAndUpdate(this, true, \'trade_admin_content\');'.
					'}'.
					'return false;'.
			'">';
		echo '<div>
				<input type="hidden" name="id_admin" value="'.$this->id.'" />
				<input type="hidden" name="fm_id" value="'.$fm_id.'" />
				<input type="hidden" name="geniutt_action" value="save_item" />
				<input type="hidden" name="display_case" value="'.$case.'" />
				<input type="hidden" name="save_case" value="update" />
				<input type="hidden" name="cur_page" value="'.$cur_page.'" />
				<input type="hidden" name="id_item" value="'.$id_item.'" />
			</div>
			';

		$tabs->separatorStart('description');

		echo '
			<p class="form-input">
				<label><span>T</span>ype d\'annonce * :</label>
				<select name="type">
				';
		if($item['trade_type'] == 'selling'){
			echo '<option value="buying" onclick="hide(gE(\'create_price\'))">Demande</option>
					<option value="selling" onclick="show(gE(\'create_price\'))" selected="selected">Offre</option>';
		}else{
			echo '<option value="buying" onclick="hide(gE(\'create_price\'))" selected="selected">Demande</option>
					<option value="selling" onclick="show(gE(\'create_price\'))">Offre</option>';
		}
		echo '
				</select>
			</p>
			';
			$this->displayCreateCategories($item['cat'] != '0' ? $item['cat'] : $item['subcat'], $item['cat'] != '0' ? $item['subcat']: '');
			echo '<p class="form-input">
				<label><span>I</span>ntitulé de l\'annonce * :</label>
				<input type="text" size="40" name="title" onblur="validateInput(this,\'SOMETHING\')" value="'.stripslashes($item['title']).'" />
			</p>
			<p class="form-input">
				<label><span>C</span>ontenu de l\'annonce * :</label>
				<textarea name="content" cols="37" onblur="validateInput(this,\'SOMETHING\')" >'.stripslashes($item['description']).'</textarea>
			</p>';
		$dis_price = '';
		if($item['trade_type'] == 'buying'){
			$dis_price = ' style="display: none;" ';
		}
		echo '<div id="create_price" '.$dis_price.'>
				<p class="form-input">
					<label><span>P</span>rix :</label>
					<input type="text" name="price" size="10" value="'.$item['price'].'" /><span> €</span>
				</p>
			</div>
			<p class="form-input">
				<label><span>D</span>ate de péremption :</label>
				<input type="text" name="end_date" max_lentgh="10" size="10" value="'.$date->reverseDate($item['end_date']).'" />
			</p>
			<p class="form-input">
				<label><span>A</span>nnonce temporairement modérée :</label>
			';
		if($item['moderated'] == 'Y'){
			echo '<input type="checkbox" name="moderated" value="Y" checked="checked" />';
		}else{
			echo '<input type="checkbox" name="moderated" value="Y" />';
		}
		echo '</p>
			<p class="form-submit">
		  	<input type="button" onclick="'.$tabs->onclickGotoTab('photos').'" value="Etape suivante" />
			</p>';

		$tabs->separatorStop('description');
		$tabs->separatorStart('photos');
			
		if($item['main_photo'] != ''){
			echo $fm->preview($item['main_photo'], 1);
		}
		
		$fm->uploadOrChoose(false,'image', 'Photographie principale :', true, 'photo1');
		if($item['second_photo'] != ''){
			echo $fm->preview($item['second_photo'], 1);
		}
		$fm->uploadOrChoose(false,'image', 'Photographie secondaire :', true, 'photo2');
		
		echo '<p class="form-submit">
		  <input type="button" onclick="'.$tabs->onclickGotoTab('description').'" value="Etape précédente" />
			<input type="button" onclick="'.$tabs->onclickGotoTab('contact').'" value="Etape suivante" />
		</p>';
													
		
		$tabs->separatorStop('photos');
		$tabs->separatorStart('contact');
				
		echo '
			<p class="form-input information">Entrez ici les coordonnées de la personne à contacter pour cette annonce :</p>
			<p class="form-input">
				<label><span>N</span>om * :</label>
				<input type="text" name="contact_lastname" size="20" onblur="validateInput(this, \'SOMETHING\')" value="'.stripslashes($item['contact_lastname']).'" />
			</p>
			<p class="form-input">
				<label><span>P</span>renom * :</label>
				<input type="text" name="contact_firstname" size="20" onblur="validateInput(this, \'SOMETHING\')" value="'.stripslashes($item['contact_firstname']).'" />
			</p>
			<p class="form-input information">Veuillez entrer au moins un moyen de contacter cette personne :</p>
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
		</fieldset>
		<p class="form-input information">* Les champs marqués d\'une étoile sont obligatoires.</p>
		<p class="form-submit">
			<input type="button" onclick="'.$tabs->onclickGotoTab('photos').'" value="Etape précédente" />
			<input type="submit" value="Modifier" />
		</p>';
		$tabs->separatorStop('contact');
		echo '</form>';
	
		$_SESSION['file_manager_'.$fm_id] = serialize($fm);
		$page->moduleFooter();
	}
	
	public function updateItem($post){
		global $db;
/*		$db->query("UPDATE trade_item SET ".
		"".
		""
		);
*/		
		switch($post['case']){
			case 'list':
				$this->displayItems($post['cur_page']);
				break;
			case 'search':
				$this->displayQueryResult('session', $post['cur_page']);
				break;
			default:
				//empty
				break;
		}
	}

	public function addCategory($post){
		global $db;
		
		$father = " 0 ";
		
		if( $post['new_category'] != '' ){
			$db->query("INSERT INTO trade_category (name, father_id) VALUES ( '".$db->escapeString($post['new_category'])."', 0 ) ");
			$father = " '".$db->insertId()."' ";
		}elseif($post['cat_category'] != 'NULL' && $post['cat_category'] != 'new' && $post['cat_category'] != '' && $post['cat_category'] != '0'){
			$father = " '".$post['cat_category']."' ";
		}
	
		
		if($post['cat_name'] != ''){
			if(!$db->query("INSERT INTO trade_category (name, father_id) VALUES ('".$db->escapeString($post['cat_name'])."', ".$father." ) ")){
				$error = 'failed_query';
			}
		}else{
			$error[] = 'no_cat_name';
		}
	
		if(isset($error))$this->displayError($error);
		
		$this->loadCategories();
		
		$this->displayCategories();
	}

	public function updateCategory($post){
		global $db;

		$father = ", father_id=0 ";
		if($post['cat_category'] != 'NULL' && $post['cat_category'] != ''){
			$father = ", father_id='".$post['cat_category']."' ";
		}
		
		if($post['cat_name'] != ''){
			if(!$db->query("UPDATE trade_category SET name='".$db->escapeString($post['cat_name'])."' ".$father." WHERE id='".$post['id_cat']."' ")){
				$error = 'failed_query';
			}
		}else{
			$error[] = 'no_cat_name';
		}
	
		if(isset($error))$this->displayError($error);
		$this->displayCategories();
	}

	public function displayAddItem(){
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
			$user = $db->fetchArray($db->query("SELECT firstname, lastname, address, address2, city, postal_code, phone, email FROM trombi_trombi WHERE id_user='".$core->getUserId()."' "));
		}

	
    echo '<div id="add_item_feedback" style="display:none" >';
		$this->displayInfo('sending');
		echo '</div>';

    echo '<div id="create_missing_alert" style="display:none" >';
		$this->displayError('create_missing');
		echo '</div>';

		$page->moduleHeader('Votre annonce :','');
		
		$tabs->display();

		echo '<form action="'.$core->makeUrl($this->path.'/ajax/trade_admin.php').'" onsubmit="'.
				'if( !trade_verifCreate(this) ){'.
						'show(gE(\'create_missing_alert\'));'.
					'}else{'.
						'hide(gE(\'create_missing_alert\'));'.
						'show(gE(\'add_item_feedback\'));'.
						'AJAX.submitAndUpdate(this, true, \'trade_admin_content\');'.
					'}'.
					'return false;'.
			'">';
		echo '<div>
				<input type="hidden" name="id_admin" value="'.$this->id.'" />
				<input type="hidden" name="fm_id" value="'.$fm_id.'" />
				<input type="hidden" name="geniutt_action" value="save_item" />
				<input type="hidden" name="save_case" value="add" />
				<input type="hidden" name="user_id" value="'.$core->getUserId().'" />
			</div>';

		$tabs->separatorStart('description');
		
		echo '<p class="form-input">
				<label><span>T</span>ype d\'annonce * :</label>
				<select name="type">
					<option value="buying" onclick="hide(gE(\'create_price\'))">Demande</option>
					<option value="selling" onclick="show(gE(\'create_price\'))">Offre</option>
				</select>
			</p>';
			
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
		
		$fm->uploadOrChoose(false,'image','Photographie principale :', true, 'photo1');
		$fm->uploadOrChoose(false,'image','Photographie secondaire :', true, 'photo2');
		
		echo '<p class="form-submit">
		  <input type="button" onclick="'.$tabs->onclickGotoTab('description').'" value="Etape précédente" />
		  <input type="button" onclick="'.$tabs->onclickGotoTab('contact').'" value="Etape suivante" />
		</p>';

		$tabs->separatorStop('photos');
		$tabs->separatorStart('contact');
		
		echo '
			<p class="form-input information">Entrez ici les coordonnées de la personne à contacter pour cette annonce :</p>
			<p class="form-input">
				<label><span>N</span>om * :</label>
				<input type="text" name="contact_lastname" size="20" onblur="validateInput(this, \'SOMETHING\')" '.((isset($user))?'value="'.stripslashes($user['lastname']).'"':'').' />
			</p>
			<p class="form-input">
				<label><span>P</span>renom * :</label>
				<input type="text" name="contact_firstname" size="20" onblur="validateInput(this, \'SOMETHING\')" '.((isset($user))?'value="'.stripslashes($user['firstname']).'"':'').' />
			</p>
			<p class="form-input information">Veuillez entrer au moins un moyen de contacter cette personne :</p>
			<p class="form-input">
				<label><span>A</span>dresse :</label>
				<textarea name="contact_address" cols="37" rows="3">'.((isset($user))?stripslashes($user['address'].$user['address2'].'
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
	
	private function displayCreateSubCategory($category, $subcat=''){
		global $core, $page;
		if(!isset($page)) $page = new Page();
		$form = new Form();
		
		$dis = ($subcat == '') ? ' disabled="disabled" ' : '';
		
		echo '
			<select name="subcategory" id="subcat_select" '.$dis.'>
		';
		if($subcat == ''){
			$form->selectFromDb('trade_category', 'name', " WHERE father_id='".$category."' ", 'Sélectionnez une sous-catégorie', '', 'id');
		}else{
			echo '<option value="NULL">Sélectionnez une catégorie</option>';
			$form->selectFromDb('trade_category', 'name', " WHERE father_id='".$category."' ", '', $subcat, 'id');
		}
		echo '
				<option value="new" onclick="'.
					'show(gE(\'create_new_subcategory\'));'.
					'show(gE(\'new_subcategory_back\'));'.
					'hide(gE(\'create_select_subcategory\'));'.
					'this.parentNode.selectedIndex=0;'.
				'" >Nouvelle sous-catégorie</option>
			</select>
		';
	}

	private function displayCreateCategories($cat='', $subcat=''){
		global $core, $page;
		if(!isset($page)) $page = new Page();
		$form = new Form();

		echo '
			<p class="form-input" id="create_select_category">
				<label><span>C</span>atégorie * :</label>
				<select name="category" onchange="hide(gE(\'create_new_subcategory\'));show(gE(\'create_select_subcategory\'));trade_createSubCateg(this, \'subcat_select\', \''.$this->str_cat.'\')">
		';
		if($cat == '' && $subcat == ''){
			$form->selectFromDb('trade_category', 'name', ' WHERE father_id = 0 ', 'Sélectionnez une catégorie', '', 'id');
		}elseif($cat != ''){
			echo '<option value="NULL">Sélectionnez une catégorie</option>';
			$form->selectFromDb('trade_category', 'name', ' WHERE father_id = 0 ', '', $cat, 'id');
		}else{
			echo '<option value="NULL">Sélectionnez une catégorie</option>';
			$form->selectFromDb('trade_category', 'name', ' WHERE father_id = 0 ', '', $subcat, 'id');
		}
		echo '
					<option value="new" onclick="'.
						'show(gE(\'create_new_category\'));'.
						'show(gE(\'create_new_subcategory\'));'.
						'show(gE(\'new_subcategory_option\'));'.
						'hide(gE(\'create_select_category\'));'.
						'hide(gE(\'create_select_subcategory\'));'.
						'hide(gE(\'new_subcategory_back\'));'.
						'this.parentNode.selectedIndex=0;'.
						'gE(\'subcat_select\').disabled = true;'.
					'">Nouvelle catégorie</option>
				</select>
			</p>
			<p class="form-input" id="create_select_subcategory">
				<label><span>S</span>ous-catégorie :</label>
				<span id="create_subcategory">
				';
		if($cat != '' && $subcat != ''){
			$this->displayCreateSubCategory($cat, $subcat);
		}else{
			$this->displayCreateSubCategory('none');
		}
		echo '
				</span>
			</p>
			
			<p class="form-input" id="create_new_category" style="display: none;">
				<label><span>N</span>ouvelle catégorie * :</label>
				<input id="new_category" type="text" size="40" name="new_category" onblur="validateInput(this, \'SOMETHING\')" />
				<span><a onclick="'.
						'show(gE(\'create_select_category\'));'.
						'show(gE(\'create_select_subcategory\'));'.
						'hide(gE(\'create_new_category\'));'.
						'hide(gE(\'create_new_subcategory\'));'.
						'hide(gE(\'new_subcategory_option\'));'.
						'gE(\'new_category\').value=\'\';'.
						'gE(\'new_subcategory\').value=\'\';'.
				'">Retour à la liste</a></span>
			</p>
			<p class="form-input" id="create_new_subcategory" style="display: none;">
				<label><span>N</span>ouvelle sous-catégorie :</label>
				<input id="new_subcategory" type="text" size="40" name="new_subcategory" />
				<span id="new_subcategory_back" style="display: none;"><a onclick="'.
					'show(gE(\'create_select_subcategory\'));'.
					'hide(gE(\'create_new_subcategory\'));'.
					'hide(gE(\'new_subcategory_back\'));'.
					'gE(\'new_subcategory\').value=\'\';'.
				'">Retour à la liste</a></span>
				<span id="new_subcategory_option" style="display: none;">Optionnel</span>
			</p>';
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

		$cat_id = $categ_id = $subcat_id = '';
		
		if( ($post['category'] == '' || $post['category'] == 'NULL' || $post['category'] == 'new' || $post['category'] == '0') && $post['new_category'] != '' ){
			if(!$db->query("INSERT INTO trade_category (name, father_id) VALUES ( '".$db->escapeString($post['new_category'])."', 0 ) ")){
				$errors = 'failed_query';
			}
			$categ_id = $db->insertId();
		}
		elseif( ($post['category'] == '' || $post['category'] == 'NULL' || $post['category'] == 'new'  || $post['category'] == '0') && $post['new_category'] == '' ){
			$errors[] = 'no_categ';
		}else{
			$cat_id = $categ_id = $post['category'];
		}

		if( !isset($errors[0]) && ($post['subcategory'] == '' || $post['subcategory'] == 'NULL' || $post['subcategory'] == 'new' || $post['subcategory'] == '0') && $post['new_subcategory'] != '' ){
			if(!$db->query("INSERT INTO trade_category (name, father_id) VALUES ('".$db->escapeString($post['new_subcategory'])."', '".$categ_id."') ")){
				$errors = 'failed_query';
			}
			$categ_id = $db->insertId();
		}elseif( !isset($errors[0]) && $post['subcategory'] != '' && $post['subcategory'] != 'NULL' && $post['subcategory'] != 'new' && $post['subcategory'] != '0' ){
			$subcat_id = $categ_id = $post['subcategory'];
		}

		
		if( $post['price'] != '' && $post['type'] == 'selling'){
			$price = $post['price'];
		}else{
			$price = '';
		}
		
		if($post['save_case'] == 'add'){
			if( $post['end_date'] != '' && $form->verifForm($post['end_date'], 'int') ){
				$end_date = $date->returnDate($date->addDays( $post['end_date'] ));
			}
		}else{
			$end_date = $date->reverseDate($post['end_date']);
			if(!$date->isDate($end_date) || $date->compare('>=', $end_date) ){
				$errors[] = 'date';
			}
		}
	
		if( !isset($errors[0]) ){
		
			$photo_ids = $fm->saveOrGet('image');
			
			//add case
			if($post['save_case'] == 'add'){
			
				$result = $db->query("INSERT INTO trade_item ".
				" (id_category, id_trader, moderated, trade_type, title, description, price, creation_date, end_date, contact_firstname, contact_lastname, contact_address, contact_phone, contact_mobile, contact_email, main_photo, second_photo) VALUES ( ".
				" '".$categ_id."', ".
				" '".$core->getUserId()."', ".
				" 'Y', ".
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
				
				//follow-up of categories by mail
				$categ_names = $db->query("SELECT name FROM trade_category WHERE id='".$cat_id."' OR id='".$subcat_id."' ORDER BY id ASC");
				$row = $db->fetchArray($categ_names);
				$categ_name = $row['name'];
				if($db->numRows($categ_names) > 1){
					$row = $db->fetchArray($categ_names);
					$subcateg_name = $row['name'];
				}
				
				$from = $core->getConfig('mailWebmaster');
				$subject = 'Nouvelle annonce';
				$message = 'Une nouvelle annonce a été créée dans la catégorie : '.$categ_name.( (isset($subcateg_name)) ? ' > '.$subcateg_name.'.' : '.').'
'.'Vous pouvez y accèder en cliquant ici : '.$core->getConfig('baseUrl').'/'.$this->path.'/index.php?part=item&content='.$item_id.'


Annonce :

Intitulé : '.( ($post['type'] == 'selling') ? 'Vend :' : 'Achète :' ).' '.$post['title'].'
Description : '.$post['content'].'
'.( ($post['type'] == 'selling') ? 'Prix : '.$post['price'].'€' : '' );
				
				$users_to_alert = $db->query("SELECT firstname, lastname, email FROM trombi_trombi WHERE id_user IN (SELECT DISTINCT id_trader FROM trade_alert WHERE id_category='".$categ_id."' AND mail='Y' ) ");
				while($user = $db->fetchArray($users_to_alert)){
					$to = $user['email'];
					$mail->send($from, $to, $message, $subject);
				}
				
			//update case
			}else{
			
				$result = $db->query("UPDATE trade_item SET ".
				" id_category='".$categ_id."', ".
				" id_trader='".$core->getUserId()."', ".
				" moderated='".(($post['moderated'] == 'Y') ? 'Y' : 'N' )."', ".
				" trade_type='".$post['type']."', ".
				" title='".$db->escapeString($post['title'])."', ".
				" description='".$db->escapeString($post['content'])."', ".
				" price='".$price."', ".
				" end_date='".$end_date."', ".
				" contact_firstname='".$db->escapeString($post['contact_firstname'])."', ".
				" contact_lastname='".$db->escapeString($post['contact_lastname'])."', ".
				" contact_address='".$db->escapeString($post['contact_address'])."', ".
				" contact_phone='".$db->escapeString($post['contact_phone'])."', ".
				" contact_mobile='".$db->escapeString($post['contact_mobile'])."', ".
				" contact_email='".$db->escapeString($post['contact_email'])."' ".
				( (isset($photo_ids['photo1']) && $photo_ids['photo1'] != '') ? " , main_photo='".$photo_ids['photo1']."' " : '').
				( (isset($photo_ids['photo2']) && $photo_ids['photo2'] != '') ? " , second_photo='".$photo_ids['photo2']."' " : '').
				" WHERE id='".$post['id_item']."'");
			
			}
			
			if(!$result){
				$this->displayError('failed_query');
			}
		}else{
			$this->displayError($errors);
		}

		//refresh categories in form selects
		$this->loadCategories();

		$this->displayItems(isset($post['cur_page']) ? $post['cur_page'] : 1 );
	}



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
					case 'date':
						$msg .= '<li>la date fournie n\'est pas valide ou appartient au passé</li>';
						break;
					case 'no_cat_name':
						$msg .= '<li>la catégorie n\'a pas de nom</li>';
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
						$msg .= '<li>le prix entré n\'est pas valide. La recherche a été effectuée sans en tenir compte.<br />Exemples de prix valides : 1423.6 - 768,90 - 4 - 5,61</li>';
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
			default:
				//empty
				break;
		}

	}


/**** End of methods used to notice user about all you want to ****/

}//end of Trade_Adminclass
?>
