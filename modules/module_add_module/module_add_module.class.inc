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


class Module_add_module extends Module {

	public function display() {
		global $db, $core, $session, $page;

		if( property_exists($this, 'dynamicTitle') ){
	
			$newTabColumn = $db->result($db->query("SELECT `column` FROM kernel_module_user_module_final WHERE idUser = '".$core->getUserId()."' AND tab = '".$this->getTab()."' AND inUserColumn = 'Y' ORDER BY `column` ASC LIMIT 1"), 0);

			$result = $db->query("
				SELECT id, title, instantiable
				FROM kernel_module_user_module_available 
				WHERE idUser = '".$core->getUserId()."'
			");

			while( $row = $db->fetchArray($result) ){
				$modules[] = $row;
			}

			?>
			
			<script type="text/javascript">
			function moduleAction(action, mId){
				var newModule = document.createElement("DIV");
				moduleColumn = gE("column_<?=$newTabColumn?>");
				if( moduleColumn ){
					moduleColumn.insertBefore(newModule, moduleColumn.firstChild);
					AJAX.getAndUpdate("<?=$core->makeUrl('include/engine/page/module/ajax/module.php')?>?ga=" + action + "&mid=" + mId + "&cid=<?=$newTabColumn?>", newModule);
				}else{
					alert("There is no column where to place the module...");
				}
			}

			function addModuleInPage(form){
				var selected = form.modules.options[form.modules.selectedIndex];
				if( selected.className == "N" ){
					remove(selected);
				}
				moduleAction('add', selected.value);
			}
			</script>	
		
		<?php

		$is_module = false;
		
		echo '<form id="add_modules_form" class="add_modules_form" action="" onsubmit="addModuleInPage(this);return false">
			<span><select name="modules" id="addable_modules_list">';
				
			foreach( $modules as $module ){
			
				echo '<option class="'.$module['instantiable'].'" value="'.$module['id'].'">'.stripslashes($module['title']).'</option>';
						
				$is_module = true;
				
			}
	
			echo '</select><a class="add_modules_button" href="javascript:addModuleInPage(gE(\'add_modules_form\'));void(0);" >Insérer</a></span>
		</form>';

		}else{
		
			//verifies there isn't default modules already loaded in column 1
			$page->verifUsersModule(1);

			//On construit la liste des modules de l'utilisateur
			//et le tableau associatif nom => id, solution temporaire pour gerer la reselection d'un module supprime
			$arrModules = Array();
			$result = $db->query("SELECT module, id FROM usersmodule WHERE login='".$core->getUserId()."'");
			while ($row = $db->fetchArray($result)) {
				$arrModules[] = stripslashes($row['module']);
			}

			//Selection de tous les modules
			$modulequery = $db->query("SELECT nom ,
				class AS module, 
				titre,
				droits 
				FROM moduleListe ml
				LEFT OUTER JOIN droits_module dm ON ml.nom=dm.module
				WHERE dispo='Y'
				AND user_module='Y'
				AND ml.nom NOT IN (SELECT module AS nom 
					FROM usersmodule
					WHERE login='".$core->getUserId()."')
				ORDER BY titre");


			//mode liste deroulante
			?>
		
			<script type="text/javascript">
			function module_pref(action,name){
				var newModule = document.createElement("DIV");
				moduleColumn = gE("module_content_box");
				moduleColumn.insertBefore(newModule, moduleColumn.firstChild);
				AJAX.getAndUpdate("include/ajax/module_pref.php?action="+action+"&name="+name,newModule);
			}

			function addModuleInPage(form){
				var selected = form.modules.options[form.modules.selectedIndex];
				remove(selected);
				module_pref('add', selected.value);
			}
			</script>
			<?php

		$is_module = false;
/*		echo '<ul id="addable_modules_list">';
		
			while($module = $db->fetchArray($modulequery))
			{
				if($core->verifDroits($module['droits']))
				{
					//On affiche pas si il a deja ce module
					if(!in_array($module['module'], $arrModules))
					{
						echo '<li><a onclick="'.
							'if(gE(\'addable_modules_list\').childNodes.length == 1){'.
								'show(gE(\'empty_module_list_alert\'));'.
							'}'.
							'remove(this.parentNode);'.
							'module_pref(\'add\',\''.$page->htmlBlock->escapeTipContent(stripslashes($module['module'])).'\');'.
						'">'.stripslashes($module['titre']).'</a></li>';
						
						$is_module = true;
					}
				}
			}
		
		echo '</ul>';
		
		$display = '';
		if($is_module){
			$display = ' style="display: none;" ';
		}
		echo '<div id="empty_module_list_alert" '.$display.' ><i>Il n\'y a plus de modules à insérer.</i></div>';
*/
			echo '<form id="add_modules_form" class="add_modules_form" action="" onsubmit="addModuleInPage(this);return false">
				<span><select name="modules" id="addable_modules_list">';
				
			while($module = $db->fetchArray($modulequery))
			{
				if($core->verifDroits($module['droits']))
				{
					//On affiche pas si il a deja ce module
					if(!in_array($module['module'], $arrModules))
					{
						echo '<option value="'.$page->htmlBlock->escapeTipContent(stripslashes($module['module'])).'">'.stripslashes($module['titre']).'</option>';
						
						$is_module = true;
					}
				}
			}

			echo '</select><a class="add_modules_button" href="javascript:addModuleInPage(gE(\'add_modules_form\'));void(0);" >Insérer</a></span>
			</form>';

			
		}//end if property
		
	} // end of member function module_add_module

}
?>
