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


/******************* module_sommaire_admin.class **********************************
Definition pour l'admin du sommaire
**********************************************************************/
class Module_sommaire_admin {
		
	private $return_url;
		
	public final function __construct()
	{
		$this->id = rand(1000000,9999999);
		$_SESSION['admin_sommaire_'.$this->id] = serialize($this);
	}
						
	public function displayContent() {
		global $core, $page;
		if(!isset($page)){
			$page = new Page();
		}
    $page->moduleHeader('Module Sommaire','');

		//div pour ajax
		echo '<div id="admin_module_sommaire_content">';
		$this->displayTitles();
		echo '</div>';
		$page->moduleFooter();
	}

	public function displayMenu(){
		global $core, $page, $db;
		$form = new Form();

		if(!isset($page)){
			$page = new Page();
		}
		$page->moduleHeader('Module Sommaire','');
		echo '<ul class="panel">
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=display_titles\',\'admin_module_sommaire_content\')">Voir les titres</a></li>
						<li class="link_menu"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=add_title\',\'admin_module_sommaire_content\')">Ajouter un titre</a></li>
					</ul>';
		
		$page->moduleFooter();
	}

	public function displayTitles(){
		global $db;

		echo '<table width="95%" align="center" class="data_table">
			<tr>
				<th width="30%">Nom</th>
				<th width="50%">URL</th>
				<th width="10%">Droits</th>
				<th width="5%">Caché</th>
				<th width="5%"></th>
			</tr>';
		$result = $db->query("SELECT id,name,url,father_id,is_hidden,rights,category FROM sommaire_title WHERE category = 'a' ORDER BY name ASC ");
		while($row = $db->fetchArray($result)){
			$this->displayRow($row);
			$res = $db->query("SELECT id,name,url,father_id,is_hidden,rights,category FROM sommaire_title WHERE category = 'b' AND father_id = '".$row['id']."' ORDER BY name ASC ");
			
			while($ro = $db->fetchArray($res)){
				$this->displayRow($ro);
				$re = $db->query("SELECT id,name,url,father_id,is_hidden,rights,category FROM sommaire_title WHERE category = 'c' AND father_id = '".$ro['id']."' ORDER BY name ASC ");
				
				while($r = $db->fetchArray($re)){
					$this->displayRow($r);
				}
				
			}
			
		}
		echo '</table>';
	}

	
	private function displayRow($row){
		global $core;
		$core->loadClass('htmlblock');
		$box = new Box();
		$hb = new HtmlBlock();
		
			$class='';
			if($row['is_hidden']=='true'){
				$class=' class="no_active" ';
			}

			//Test to know the place of the current title in the hierarchy
			if ($row['category'] == 'a') {
				$title_font_style = 'big';
			}
			elseif ($row['category'] == 'b') {
				$title_font_style = 'normal';
			}
			else {
				$title_font_style = 'small';
			}

			$name = stripslashes($row['name']);
			$cutName = substr($name, 0, 30);
			if($cutName != $name){
				$cutName .= '...';
			}
			
			$url = stripslashes($row['url']);
			$cutUrl = substr($url, 0, 50);
			if($cutUrl != $url){
				$cutUrl .= '...';
			}
		
			echo '<tr '.$class.' >
				<td><a onclick="hide(toolTips);AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update&title='.$row['id'].'\',\'admin_module_sommaire_content\')"><span class="'.$title_font_style.'" '.( $cutName != $name ? ' onmouseover="'.$hb->tooltip($name).'" ' : '' ).' >'.$cutName.'</span></a></td>
				<td><a onclick="hide(toolTips);AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update&title='.$row['id'].'\',\'admin_module_sommaire_content\')" '.( $cutUrl != $url ? ' onmouseover="'.$hb->tooltip($url).'" ' : '' ).' >'.$cutUrl.'</a></td>
				<td><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=update&title='.$row['id'].'\',\'admin_module_sommaire_content\')">'.stripslashes($row['rights']).'</a></td>
				<td align="center"><a onclick="AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=hide_title&title='.$row['id'].'&hidden='.(($row['is_hidden']=='false')?'true':'false').'\',\'admin_module_sommaire_content\')">'.(($row['is_hidden']=='false')?'N':'Y').'</a></td>
				<td align="center"><a class="trash" onclick="'.$box->confirm('AJAX.getAndUpdate(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=save&case=delete&title_to_del='.$row['id'].'\',\'admin_module_sommaire_content\')','Etes-vous sûr de vouloir supprimer "'.$name.'" ?').'"></a></td>
			</tr>';
			
	}


	//After this line : old admin is recycled.
	
	private function titleInfos($id)
  {
	  global $db;
	  $data = $db->query('SELECT * FROM sommaire_title WHERE sommaire_title.id="'.$id.'"');
    while ($row =$db->fetchArray($data)){
      $rights=explode(' ',stripslashes($row['rights']));
			$infos=Array('id'=>$row['id'],'name'=>stripslashes($row['name']),'url'=>stripslashes($row['url']),'category'=>$row['category'],'father_id'=>$row['father_id'],'is_hidden'=>$row['is_hidden'],'rights'=>$rights);
    }
    return $infos;
  }
																		
	public function fatherSelect($category='',$id='',$father_id=''){
		global $core;
		$core->loadClass('form');
		$form = new Form();

    echo '<select name="title_father_id" id="title_father_id" >';
		if($father_id=='' || father_id==0){
			echo '<option value="NULL" >Selectionnez un titre</option>';
		}
		
		if($id!=''){
			$cat = " WHERE id!='".$id."' ";
		}else{
			$cat = " WHERE 1";
		}
		
		switch($category){
			case 'b':
				$cat .= " AND category='a' ";
				$form->selectFromDb('sommaire_title','name',$cat,'',$father_id,'id');
				break;
			case 'c':
				$cat .= " AND category='b' ";
				$form->selectFromDb('sommaire_title','name',$cat,'',$father_id,'id');
				break;
			default:
				break;
		}

	  echo '</select>';
	}
  
	/*
	 * Affiche un formulaire
	 * @case =create/update : formulaire de création ou de mise à jour de titre
	 * @selected : titre à mettre à jour
	 */
	public function displayForm($case,$selected='')
	{
		global $db,$core;
		$core->loadClass('form');
		$form = new Form();
		
		if($case=='update'){
			//récupération de toute les informations du titre sélctionné
			$infos_selected = $this->titleInfos($selected);
		}
		
		$tab_attributes=array(
			'form'=>array('id'=>'','method'=>'post','action'=>''),
			'legend'=>'',
			'title'=>array('name'=>'title_name','id'=>'title_name','value'=>''),
			'category'=>array('name'=>'title_category','id'=>'title_category','onchange'=>''),
			'father'=>array('name'=>'title_father_id','id'=>'title_father_id','onchange'=>'','disabled'=>''),
			'url'=>array('name'=>'title_url','id'=>'title_url','value'=>''),
			'rights'=>array('name'=>'rights[]','id'=>'rights','value'=>''),
			'hidden_true'=>array('name'=>'title_is_hidden','id'=>'is_hidden_true'),
			'hidden_false'=>array('name'=>'title_is_hidden','id'=>'is_hidden_false'),
			'submit'=>array('onclick'=>'','value'=>'')
		);
		
		switch($case){
			case 'create':
				$tab_attributes['form']['id']='form_title_create';
				$tab_attributes['form']['save_params']='?id_admin='.$this->id.'&content=save&case=create';
				$tab_attributes['legend']='Ajouter un titre au sommaire';
				$tab_attributes['submit']['onclick']='verifCreate(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=save&case=create\')';
				$tab_attributes['submit']['value']='Créer le titre';
				$tab_attributes['category']['onchange'].='enableFatherId(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=category\')';
				break;
			case 'update':
				$tab_attributes['form']['id']='form_title_update';
				$tab_attributes['form']['save_params']='?id_admin='.$this->id.'&content=save&case=update';
				$tab_attributes['legend']='Modifier un titre du sommaire';
				$tab_attributes['submit']['onclick']='verifUpdate(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=save&case=update\')';
				$tab_attributes['submit']['value']='Modifier le titre';
				$tab_attributes['title_to_alt']['value']=$infos_selected['id'];
				$tab_attributes['title']['value']=$infos_selected['name'];
				$tab_attributes['url']['value']=$infos_selected['url'];
				$tab_attributes['category']['onchange'].='enableFatherId(\''.$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').'?id_admin='.$this->id.'&content=category&id='.$infos_selected['id'].'\')';
				break;
			default:
				//void
				break;
		}
		
	
		//beginning of form
		echo '<form id="'.$tab_attributes['form']['id'].'" action="'.
					$core->makeUrl('modules/module_sommaire/admin/ajax/content.php').$tab_attributes['form']['save_params'].'" '.
					'method="'.$tab_attributes['form']['method'].'">
				<fieldset><legend><b>'.$tab_attributes['legend'].'</b></legend>';
		
		if($case=='update'){
			echo '<input type="hidden" name="title_to_alt_id" id="title_to_alt_id" value="'.$tab_attributes['title_to_alt']['value'].'" />';
		}
		
		echo '<p class="form-input"><label>Titre :</label>
					<input type="text" name="'.$tab_attributes['title']['name'].'" id="'.$tab_attributes['title']['id'].'" value="'.$tab_attributes['title']['value'].'" /></p>
					
					<p class="form-input"><label>Ce titre est un :</label>
					<select name="'.$tab_attributes['category']['name'].'" id="'.$tab_attributes['category']['id'].'" onchange="'.$tab_attributes['category']['onchange'].'">';
		
		$cat=(isset($infos_selected["category"]))?$infos_selected["category"]:'';
		switch($cat){
			case 'b':
				echo '<option value="a">titre principal</option>
					<option value="b" selected="selected">titre intermédiaire</option>
					<option value="c">sous-titre</option>';
				break;
			case 'c':
				echo '<option value="a">titre principal</option>
					<option value="b">titre intermédiaire</option>
					<option value="c" selected="selected">sous-titre</option>';
				break;
			default:
				echo '<option value="a" selected="selected">titre principal</option>
					<option value="b">titre intermédiaire</option>
					<option value="c">sous-titre</option>';
				break;
		}

			echo '</select></p><p class="form-input">
				<label>ayant pour titre supérieur :</label>';

			//select changes with title category. Managed with JavaScript
			echo '<span id="selectContent">';
			$father_id=(isset($infos_selected['father_id']))?$infos_selected['father_id']:'';
			$category=(isset($infos_selected['category']))?$infos_selected['category']:'';
			$id=(isset($infos_selected['id']))?$infos_selected['id']:'';
			$this->fatherSelect($category,$id,$father_id);
			echo '</span></p>';

			echo '<p class="form-input"><label>URL du titre :</label>

				<input type="text" name="'.$tab_attributes['url']['name'].'" id="'.$tab_attributes['url']['id'].'" value="'.$tab_attributes['url']['value'].'" /></p>
				<p class="form-input"><label>Groupe(s) autorisé(s) à voir le titre : </label>
				<select name="'.$tab_attributes['rights']['name'].'" id="'.$tab_attributes['rights']['id'].'" size="4" multiple="multiple">';
				
			if($case=='create'){
				$form->selectFromDb('listes','liste','','','','');
			}

			if($case=='update'){
				$data = $db->query("SELECT listes.liste FROM listes ;");
		
		 		while ($row =$db->fetchArray($data)){
					$sel='';
		 			if(in_array($row['liste'],$infos_selected['rights'])){
		 				$sel=' selected="selected" ';
		 			}
		 	 		echo '<option value="'.stripslashes($row['liste']).'" '.$sel.' >'.stripslashes($row['liste']).'</option>';
		 		}
			}

			echo '</select></p>
				<label>Ce titre est caché pour l\'instant :</label>';
			
			$is_hidden = (isset($infos_selected['is_hidden']))?$infos_selected['is_hidden']:'';
			if($is_hidden=='true'){
			
				echo '<input type="radio" name="'.$tab_attributes['hidden_true']['name'].'" value="true" id="'.$tab_attributes['hidden_true']['id'].'" checked="checked" /><label>Oui</label>
					<input type="radio" name="'.$tab_attributes['hidden_false']['name'].'" value="false" id="'.$tab_attributes['hidden_false']['id'].'" /><label>Non</label>';
					
			}else{
			
				echo '<input type="radio" name="'.$tab_attributes['hidden_true']['name'].'" value="true" id="'.$tab_attributes['hidden_true']['id'].'" /><label>Oui</label>
					<input type="radio" name="'.$tab_attributes['hidden_false']['name'].'" value="false" id="'.$tab_attributes['hidden_false']['id'].'" checked="checked"/><label>Non</label>';
					
			}
	
			echo '<p class="form-submit"><input type="button" value="'.$tab_attributes['submit']['value'].'" onclick="'.$tab_attributes['submit']['onclick'].'" /></p>
			</fieldset></form>';

	//end of displayAdminForm()
	}
  
	public function hideTitle($title,$hidden){
		global $db;
		$db->query("UPDATE sommaire_title SET is_hidden='".$hidden."' WHERE id='".$title."'");
	}
	
	public function saveForm($get){
		global $db,$page;
		if(!isset($page)){
			$page = new Page();
		}
		
		$action = $get['case'];
	
		if( !isset($get['title_father_id']) || $get['title_father_id'] == '' || $get['title_father_id'] == 0){
			$fatherId = 'NULL';
		}else{
			$fatherId = "'".$get['title_father_id']."'";
		}
		
		switch($action){
		
			case 'create':
				$request = "INSERT INTO sommaire_title (name,url,category,father_id,is_hidden,rights) VALUES (
					'".$db->escapeString($get['title_name'])."', 
					'".$db->escapeString($get['title_url'])."', 
					'".$get['title_category']."', 
					$fatherId, 
					'".$get['title_is_hidden']."', 
					'".$db->escapeString($get['rights'])."' 
				)";
				break;
				
			case 'update':
				$request = "UPDATE sommaire_title SET 
					name = '".$db->escapeString($get['title_name'])."', 
					url = '".$db->escapeString($get['title_url'])."', 
					father_id = $fatherId, 
					is_hidden = '".$get['title_is_hidden']."', 
					rights = '".$db->escapeString($get['rights'])."', 
					category = '".$get['title_category']."' 
				WHERE id = '".$get['title_to_alt_id']."' ";
				break;
				
			case 'delete':
				$request = "DELETE FROM sommaire_title WHERE id = '".$get['title_to_del']."' ";
				break;
				
			default:
				//void
				break;
		}
	
		$resp = $db->query($request);
	}
}
?>
