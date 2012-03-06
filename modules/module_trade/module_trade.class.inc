<?php
class Module_Trade extends Module {

	private $config;
	
  public final function __construct($informations = Array() ) {
		parent::__construct($informations);
	
		include('config_module_trade.inc');
		$this->config = $config;
  }


  public function display() {
		global $db, $core, $page;
		$core->loadClass('fileManager');
		$fm = new FileManager();
		
		echo '<div class="my_title_block">
						<span class="titre_menu">
							<a onclick="display(gE(\'module_trade_my_items\'));"><span>M</span>es annonces... </a>
						</span>
					</div>
					
					<div class="clear"></div>
					<div class="my_block" id="module_trade_my_items" style="display: none;">';

		$result = $db->query("SELECT ti.id AS id_item, ti.moderated, ti.main_photo, ti.title, ti.description, ti.trade_type, tc.name AS subcat_name, tc.id AS id_subcat, tcf.id AS id_cat, tcf.name AS cat_name ".
      " FROM trade_item ti INNER JOIN (trade_category tc LEFT JOIN trade_category tcf ON tc.father_id=tcf.id) ON ti.id_category=tc.id ".
			" WHERE ti.id_trader = '".$core->getUserId()."' ".
				" AND  ti.end_date > NOW() ".
			" ORDER BY ti.creation_date DESC");
		
		if($db->numRows($result) > 0){

			$darkness = true;
		
			while($item = $db->fetchArray($result)){
				//style of this item
				$darkness = !$darkness;
				$class = '_'.(($darkness) ? 'a' : 'b' );

				$tooltip = $page->htmlBlock->escapeTipContent(
					'<div class="item_tooltip"><div class="item_photo">'.
					$fm->preview($item['main_photo'], 1, true).
					'<div class="item_desc">'.
					$this->cutText(stripslashes($item['description']), 200).
					'</div></div>'
				);
				echo '<div class="item_line my'.$class.'">
					<div class="item_title left" onmouseover="activateToolTips(this,\''.$tooltip.'\')" '.
					'>
						<a href="'.$core->makeUrl('applications/trade/index.php').'?part=item&amp;content='.$item['id_item'].'" >';
					
				echo htmlentities($this->cutText(stripslashes($item['title']), 60), ENT_QUOTES, 'UTF-8').'</a></div>'.
					'<div class="item_subcategory left">
						<a href="'.$core->makeUrl('applications/trade/index.php').'?part=category&amp;content='.$item['id_cat'].'" >'.
							stripslashes($item['cat_name']).
						'</a> &gt; '.
						'<a href="'.$core->makeUrl('applications/trade/index.php').'?part=category&amp;content='.$item['id_subcat'].'" >'.
							stripslashes($item['subcat_name']).
						'</a>
					</div>
					<span id="item_moderated">'.( ($item['moderated'] == 'N') ? 'Active' : 'Validation en cours' ).'</span>
					<div class="clear"></div>
				</div>';		
			}
	
		}else{
			echo '<p><i>Vous n\'avez aucune annonce en cours.</i></p>';
		}
		
		echo '</div><div class="clear"></div>';
			
	
		$result = $db->query("SELECT ti.id AS id_item, ti.main_photo, ti.title, ti.description, ti.trade_type, tc.name AS subcat_name, tc.id AS id_subcat, tcf.id AS id_cat, tcf.name AS cat_name ".
			"FROM trade_item ti INNER JOIN (trade_category tc LEFT JOIN trade_category tcf ON tc.father_id=tcf.id) ON ti.id_category=tc.id ".
			" WHERE ti.id_category IN (SELECT ta.id_category FROM trade_alert ta WHERE ta.id_trader='".$core->getUserId()."' AND ta.site='Y' ) AND ti.end_date > NOW() AND ti.moderated='N' ".
			" AND ti.id_trader != '".$core->getUserId()."' ".
			" ORDER BY trade_type, cat_name ASC, subcat_name ASC, id_item DESC");

		$items = Array();
		while($item = $db->fetchArray($result)) {
			$items[] = Array(
				'id_item' => $item['id_item'],
				'main_photo' => $item['main_photo'],
				'id_cat' => $item['id_cat'], 
				'id_subcat' => $item['id_subcat'], 
				'subcat_name' => htmlentities(stripslashes($item['subcat_name']), ENT_QUOTES, 'UTF-8'), 
				'cat_name' => htmlentities(stripslashes($item['cat_name']), ENT_QUOTES, 'UTF-8'), 
				'title' => stripslashes($item['title']), 
				'description' => stripslashes($item['description']), 
				'trade_type' => $item['trade_type']
			);
		}
		
		echo '<div>';
		if($db->numRows($result) == 0){
			echo '<div><p><i>Actuellement, il n\'y a aucune annonce dans les catégories que vous suivez.</i></p>';
		}
		
		$old_cat = $old_subcat = '';
		$i = 0;
		$color = $darkness = true;	
		$nb_items = $core->user->getPref($core->getUserId(), 'modules_trade_followup');
		$is_buying = false;
		$first_item = true;
		
		foreach($items as $attr){
			if($first_item && $attr['trade_type'] == 'selling'){
				echo '<div class="selling_block"><p class="selling_title_block"><span class="titre_menu"><span>O</span>ffre :</span></p>';
				$first_item = false;
			}
			
			if ($is_buying == false && $attr['trade_type'] == 'buying') {
				if(!$first_item){
					echo '</div>';
				}
				echo '<div class="buying_block"><p class="buying_title_block"><span class="titre_menu"><span>D</span>emande :</span></p>';
				$is_buying = true;
			}
			$i = 0;

			if($i <= $nb_items){
				//style of this item
				$darkness = !$darkness;
				$class = '_'.(($darkness) ? 'a' : 'b' );
				
				if($attr['trade_type'] == 'selling') {
					$color = 'selling';
				} else {
						$color = 'buying';
					}

				if( ( $attr['id_cat'] != $old_cat && $attr['id_cat'] != '' ) ) {
					echo '<p class="items_category"><a '.
					' href="'.$core->makeUrl('applications/trade/index.php').'?part=category&amp;content='.$attr['id_cat'].'" '.
					'>'.$attr['cat_name'].'</a></p>';
				}
		
				$tooltip = $page->htmlBlock->escapeTipContent(
					'<div class="item_tooltip"><div class="item_photo">'.
					$fm->preview($attr['main_photo'], 1, true).
					'<div class="item_desc">'.
					$this->cutText($attr['description'], 200).
					'</div></div>'
				);
				echo '<div class="item_line '.$color.$class.'">
					<div class="item_title left" onmouseover="activateToolTips(this,\''.$tooltip.'\')" '.
					'>
						<a href="'.$core->makeUrl('applications/trade/index.php').'?part=item&amp;content='.$attr['id_item'].'" >';
					
					echo htmlentities($this->cutText($attr['title'], 60), ENT_QUOTES, 'UTF-8').'</a>
					</div>
					<div class="item_subcategory left"><a '.
						' href="'.$core->makeUrl('applications/trade/index.php').'?part=category&amp;content='.$attr['id_subcat'].'" '.
						'>'.$attr['subcat_name'].'</a>
					</div>
					<div class="clear"></div>
				</div>';
			}

			$old_cat = $attr['id_cat'];
			$old_subcat = $attr['id_subcat'];
			$i++;
		}
		
		echo '</div></div>';

		echo '<p class="comment">Pour suivre une catégorie d\'annonces, utilisez <a href="'.$core->makeUrl('applications/trade/index.php').'?part=myspace&content='.$core->getUserId().'&option=true">votre espace</a>...</p><br />';
		
		$page->adminLink('app', 'trade', $this->config['admin_rights']);
  }
    
	private function cutText ($text,$max_length=LG_MAX_TITRE) {
    $text=preg_replace("/(.{".($max_length-3)."}).+/","\\1...",$text);
	  return $text;
	}
					
	
}
?>
