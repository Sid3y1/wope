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


class magicForm {

	/*Option du formulaire*/
	private $option;

	/*Champs du formulaire => description complete*/
	private $field;

	/*La table en jeu*/
	private $table;
	
	/*Le tableau des champs (nom)*/
	private $champ;

	/*Le tableau des valeurs */
	private $value;

	/*Indicateur pour savoir si on est en auto ou en analyse de requette */
	private $auto;

	/*Tableau de valeur avec les valeurs !!! */
	private $valueMore;


	/**
		* __construct Constructeur de magicForm, il va analyser le truc !
		* 
		* @param mixed $table 
		* @param mixed $option 
		* @access public
		* @return void
		*/
	public function __construct($table,$option = Array()){
		global $db,$core;
		//L'id permet de faire plusieurs Formulaires par pages, tres utile, il va sauvegarder l'objet dans une variable de session, raison de CQ evidentes !
		$this->id = rand(1000000,9999999);
		//Si on trouve un espace dans la variable $table c'est que ce n'est pas un nom de table mais une requete !
		if(strpos($table," ") !== false){
			$req = $table;
			/*
			 * VK : Autre version du code ci dessous
			 * je ne sais pas si ce code est utilisé donc je n'ai pas testé
			 * cette expression régulière mais elle devrait être plus efficace
			 * (une seule expression pour remplir les 3 champs)
			 */ 
			//preg_match("/^insert +into +(.+) ?\((.+)\) +values +\((.+)\)/i",$req,$temp);
			//$table=$temp[1];
			//$champ=explode(',', $temp[2]);
			//$value=explode(',', $temp[3]);
			
			//On recupere les infos une a une !
			$table = preg_replace("/[iI][nN][sS][eE][rR][tT][\s]*[iI][nN][tT][oO][\s]*(.*) .*/","\\1",$req);
			$champ = preg_replace("/[iI][nN][sS][eE][rR][tT][\s]*[iI][nN][tT][oO].*\((.*)\)[vV][aA][lL][uU][eE][sS].*/","\\1",$req);
			$value = preg_replace("/[iI][nN][sS][eE][rR][tT] [iI][nN][tT][oO] .* \(.*\)[vV][aA][lL][uU][eE][sS]\((.*)\)/","\\1",$req);
			$champ = explode(',',$champ);
			$value = explode(',',$value);
			//On ne va montrer que les champ qui ont comme valeur #
			foreach($value as $i => $n){
				if($n == '?'){
					$option['only'][]=$champ[$i];
				}
			}
			$this->champ = $champ;
			$this->value = $value;
			$this->auto = true;
				
		}else{
			$this->auto = false;
		}
		
		$option['description'] = (isset($option['description']))?$option['description']:'';
		//Soit on recupere les informations 
		if (isset($option['describe']) && is_array($option['description']) ){
			$this->field = $option['describe'];
		} else {
			$res = $db->query("describe `$table`"); 
			while($don = $db->fetchArray($res)){
				$key =( $don['Key'] == 'PRI');
				
				$this->field[$don['Field']] = Array("type"=>$don['Type'],"vide"=>$don['Null'],"key"=>$key,"default"=>$don['Default'],"extra"=>$don['Extra']);
			}
			if(isset($option['analyse']) && $option['analyse'] == true){
				echo '<textarea class="analyse">';
				echo '\'describe\' => Array('."\n";
				$indice = 0;
				foreach($this->field as $name => $don){
					if($indice!=0){
						echo ','."\n";
					}
					$indice ++;
					echo '\''.$name.'\' =>  Array("type"=>"'.$don['type'].'","vide"=>"'.$don['vide'].'","key"=>"'.$don['key'].'","default"=>"'.$don['default'].'","extra"=>"'.$don['extra'].'")';
				}
				echo ')';
				echo '</textarea>';
			}
		}	
		$this->table = $table;
		$this->option = $option;
		$this->forceType = (isset($option['forceType']) && is_array($option['forceType']))?$option['forceType']:Array();
		$this->forceValue = (isset($option['forceValue']) && is_array($option['forceValue']))?$option['forceValue']:Array();
		$this->forceSize = (isset($option['forceSize']) && is_array($option['forceSize']))?$option['forceSize']:Array();

		//Sauvegarde du bordel !
		$_SESSION['magicForm_'.$this->id] = serialize($this);
	}


						public function display(){
							global $core,$db;
							//Si on est en manuel on initialise les valeurs en tableau !
							if(!$this->auto){
								$this->champ = Array();
								$this->value = Array();
							}
							$option = $this->option;
						
							//Initialisation des variables de gestion du cas "update"
							$update=false;
							$dataToUpdate = '';
							$option['update'] = (isset($option['update']))?$option['update']:false;
							$option['rowToUpdate'] = (isset($option['rowToUpdate']))?$option['rowToUpdate']:Array('field'=>'','pattern'=>'');
							//Peut-on faire un update ?
							if( $option['update'] && is_array($option['rowToUpdate']) ){
								//On verifie si l'enregistrement existe et si oui on charge les donnees de l'enregistrement
								//A ameliorer pour ne selectionner que le necessaire
								$dataTU = $db->query("SELECT * FROM ".$this->table." WHERE ".$option['rowToUpdate']['field']."='".$option['rowToUpdate']['pattern']."' ");
								if($db->numRows($dataTU) == 1){
									$update = true;
									while($dataTUrow=$db->fetchArray($dataTU)){
										$dataToUpdate = $dataTUrow;
									}
								}
							}
							
							echo '<div id="form_msg_'.$this->id.'"></div>';
							
							if($update){
								echo '<form action="'.$core->makeUrl('include/ajax/framework.php?action=magicformUpdate&id='.$this->id).'" method="post" onsubmit="formMakeAndCall(this,function(){';
							}
							else{
								echo '<form action="'.$core->makeUrl('include/ajax/framework.php?action=magicform').'" method="post" onsubmit="formMakeAndCall(this,function(){';
							}
							if(!isset($option['feadback']) || $option['feadback'] != 'fixe'){
								switch($option['feedback']){
									case 'popup':
										echo 'dialBox.draw(this.responseText);';
										break;
									case 'info':
										echo 'gE(\'form_msg_'.$this->id.'\').innerHTML = this.responseText; ';
										break;
									default:
										//void
										break;
								}
								if(!$option['update']){
									if(!isset($this->option['once']) || $option['once']){
										//Si il le shouaite on efface le formulaire 
										echo 'remove(gE(\'MagicForm_'.$this->id.'\')); ';
									}else{
										//On reset le formulaire sinon ca a pas de gueule !
										echo 'gE(\'MagicForm_'.$this->id.'\').reset(); ';
									}
								}

							}else{
								echo 'gE(\'MagicForm_'.$this->id.'\').innerHTML = this.responseText; ';

							}
							if (isset($this->option['onsubmit'])) {
								echo $this->option['onsubmit'];
							}
							echo '});';
							echo 'return false;" id="MagicForm_'.$this->id.'">';
							echo '<input type="hidden" name="id" value="'.$this->id.'" />';
							echo '<fieldset><legend>'.(isset($option['name'])?$option['name']:ucfirst($this->table)).'</legend>';	
							//on passe les parametres de la requete d'update
							echo '<input type="hidden" name="field" value="'.$option['rowToUpdate']['field'].'" />';
							echo '<input type="hidden" name="pattern" value="'.$option['rowToUpdate']['pattern'].'" />';
							$position=0;
							foreach($this->field as $name => $data){
								if(!isset($this->forceType[$name])){
									$type = ereg_replace("()\(.*\)","\\1",$data['type']);
									$tmp	= explode(" ",$type);
									$type = $tmp[0];
								}else{
 									$type = $this->forceType[$name]['type'];
								}
								if(isset($option['only']) && !in_array($name,$option['only']) ){
									continue;
								}
								if(!$this->auto){
									$this->champ[] = $name;
									$this->value[] = '?';
								}
								//empeche d'ecraser $_POST['login'] de /include/main.inc lors de la gestion des sessions
								if($name=='login'){
									$name='mf_login';
									if(isset($option['legend']['login']))$option['legend'][$name]=$option['legend']['login'];
									if(isset($this->forceType['login'])){
										$this->forceType[$name]=$this->forceType['login'];
										$type = $this->forceType['mf_login']['type'];
									}
									if(isset($this->forceValue['login'])){
										$this->forceValue[$name]=$this->forceValue['login'];
									}
									if($update)$dataToUpdate[$name]=$dataToUpdate['login'];
								}
								if(isset($this->forceValue[$name])&& $this->forceValue[$name]['value']!=''){
									$data['default'] = $this->forceValue[$name]['value'];
								}
								
								//this div is used by the calendar
								echo '<div id="cal_container"></div>';
								
								switch ($type){
									case 'varchar':
										$nbr = ereg_replace(".*\(([0-9]*)\)","\\1",$data['type']);
										echo '<p class="form-input"><label for="'.$name.'">'.( isset($option['legend'][$name]) ? $option['legend'][$name] : ucfirst($name) ).'</label>';
										echo  '<input type="text" '.
											' maxlength="'.$nbr.'" '.
											( isset($this->forceSize[$name]['size']) ? ' size="'.$this->forceSize[$name]['size'].'" ' : ' ' ).
											' name="'.$name.'" '.
											' value="'.( ($update) ? stripslashes($dataToUpdate[$name]) : $data['default'] ).'" '.
											( ($data['vide']=='NO') ? ' onblur="validateInput(this,\'SOMETHING\');" ' : '' ).' />'.
										'</p>';
										break;
									case 'int':
										if($data['extra'] != "auto_increment"){
											$nbr = ereg_replace(".*\(([0-9]*)\)","\\1",$data['type']);
											echo '<p class="form-input"><label for="'.$name.'">'.( isset($option['legend'][$name]) ? $option['legend'][$name] : ucfirst($name) ).'</label>';
											echo '<input type="text" '.
												' maxlength="'.$nbr.'" '.
												( isset($this->forceSize[$name]['size']) ? ' size="'.$this->forceSize[$name]['size'].'" ' : ' ' ).
												' name="'.$name.'" '.
												' value="'.( ($update) ? $dataToUpdate[$name] : $data['default'] ).'" '.
												' onblur="validateInput(this,\'INT\');" />'.
											'</p>';
										}
										break;
									case 'float':
										echo '<p class="form-input"><label for="'.$name.'">'.( isset($option['legend'][$name]) ? $option['legend'][$name] : ucfirst($name) ).'</label>';
										echo '<input type="text" '.
											( isset($this->forceSize[$name]['size']) ? ' size="'.$this->forceSize[$name]['size'].'" ' : ' ' ).
											' name="'.$name.'" '.
											' value="'.( ($update) ? $dataToUpdate[$name] : $data['default'] ).'" '.
											' onblur="validateInput(this,\'FLOAT\');" /></p>';
										break;
									case 'text':
										echo '<p class="form-textarea"><label for="'.$name.'">'.( isset($option['legend'][$name]) ? $option['legend'][$name] : ucfirst($name) ).'</label>';
										echo '<textarea name="'.$name.'" '.
											( isset($this->forceSize[$name]['rows']) ? ' rows="'.$this->forceSize[$name]['rows'].'" ' : ' rows="5" ' ).' '.
											( isset($this->forceSize[$name]['cols']) ? ' cols="'.$this->forceSize[$name]['cols'].'" ' : ' cols="50" ' ).' '.
											( ($data['vide']=='NO') ? ' onblur="validateInput(this,\'SOMETHING\');" ' : '' ).
										' >'.( ($update) ? stripslashes($dataToUpdate[$name]) : $data['default'] ).'</textarea></p>';
										//todo:AJouter un dico sur ce champ !!!
										break;
								case 'hidden':
									echo  '<input type="hidden" '.
										' name="'.$name.'" '.
										' value="'.(  ($update)  ?  stripslashes($dataToUpdate[$name])  :  ( isset($this->forceType[$name]['value']) ? stripslashes($this->forceType[$name]['value']) : $data['default'] )  ).'" '.
									' />';
									break;
								case 'password':
									echo '<p class="form-input"><label for="'.$name.'">'.( isset($option['legend'][$name]) ? $option['legend'][$name] : ucfirst($name) ).'</label>';
                  echo  '<input type="password" '.
										' name="'.$name.'" '.
										' value="'.( ($update) ? stripslashes($dataToUpdate[$name]) : $data['default'] ).'" '.
										( ($data['vide']=='NO') ? ' onblur="validateInput(this,\'SOMETHING\');" ' : ' ' ).
									' /></p>';
									break;
								case 'user_id':
								case 'login':
									echo  '<input type="hidden" name="'.$name.'" value="'.$core->getUserId().'" />';
									break;
								case 'date':
										$id='date_'.rand();
										echo '<p class="form-date"><label for="'.$name.'">'.( isset($option['legend'][$name]) ? $option['legend'][$name] : ucfirst($name) ).'</label>';
										$core->loadClass('date');
										if($update){
											$date = new Date($dataToUpdate[$name]);
											$dataToUpdate[$name] = $date->formatDate();
										}
										else{
											$date = new Date($data['default']);
											$data['default'] = $date->formatDate();
										}
										echo '<input id="date_field_'.$name.'" type="text" '.
											' id="'.$id.'" '.
											' maxlength="10" '.
											' size="'.( isset($this->forceSize[$name]['size']) ? $this->forceSize[$name]['size'] : '10' ).'" '.
											' name="'.$name.'" '.
											' value="'.( ($update) ? $dataToUpdate[$name] : $data['default']).'" '.
											' onblur="validateInput(this,\'DATE_FR\');" '.
										' />
											<a title="'.locale::display('calendar', 'kernel').'" class="calendar_button" onclick="selectDate(\'cal_container\', '.
										    'function(date){'.
											   ' gE(\'date_field_'.$name.'\').value = date; '.
											  '}'.
											', true, \'FR\');">&nbsp;</a>
										</p>';
										break;
									case 'select':
										echo '<p class="form-input"><label for="'.$name.'">'.( isset($option['legend'][$name]) ? $option['legend'][$name] : ucfirst($name) ).'</label>';
										echo '<select name="'.$name.'">';
										foreach($this->forceType[$name]['options'] As $value => $label){
											//selection par defaut de l'ancienne valeur
											$sel = '';
											if( ($update) && (stripslashes($dataToUpdate[$name]) == $value) ){
												$sel = 'selected="selected"';
											}
											echo '<option value="'.$value.'" '.$sel.' >'.$label.'</option>';
										}
										echo '</select></p>';

									 break;
									case 'radio':
										echo '<p><label for="'.$name.'">'.( isset($option['legend'][$name]) ? $option['legend'][$name] : ucfirst($name)).'</label>';
										foreach($this->forceType[$name]['options'] As $value => $label){
											$sel = '';
											if( ($update) && (stripslashes($dataToUpdate[$name]) == $value) ){
                        $sel = 'checked="checked"';
                      }
											if( !$update && $value == $this->forceType[$name]['default']){
												$sel = 'checked="checked"';
											}
										  echo '<input type="radio" name="'.$name.'" value="'.$value.'" '.$sel.' /><span>'.$label.'</span>';
										}
										echo '</p>';
										break;
									//pas encore utilisable, il faut faire un implode lors de la recuperation des donnees
/*									case 'multiple_select':
										if($update)$dataArray = explode(' ',$dataToUpdate[$name]);
										echo '<p class="form-input"><label for="'.$name.'">'.(isset($option['legend'][$name])?$option['legend'][$name]:ucfirst($name)).'</label>';
										echo '<select name="'.$name.'" multiple="multiple" size="5">';
										foreach($this->forceType[$name]['options'] As $value => $label){
											//selection par defaut de l'ancienne valeur
											$sel = '';
											if(($update) && in_array($value,$dataArray) ){
												$sel = 'selected="selected"';
											}
											echo '<option value="'.$value.'" '.$sel.' >'.$label.'</option>';
										}
										echo '</select></p>';
									 
									 break;*/
									 
									 
								}
								
								// VK : Ajout gestion des catégories
								if (isset($this->option["category"]) && 
										$this->option["category"]["position"]==++$position) {
								 	$login=(int)$core->getUserId();
									 	
								 	if ($update) {
									 	$keyName = "";
									 	foreach (array_keys($this->field) as $fieldName) {
									 		if ($this->field[$fieldName]['key']) {
									 			// The key must be unique to be able to use the tag frontend
									 			// please modify your database !!
									 			assert($keyName=="");
									 			$keyName=$fieldName;
									 		}
									 	}
									 	// The key must exist to be able to use the tag frontend
							 			// please modify your database !!
							 			assert($keyName<>"");
									 	$key_value=htmlentities($dataToUpdate[$keyName],ENT_QUOTES,"UTF-8");
									 	$key_query="info.tableOrig='".$this->table."'
															AND info.idOrig='".$key_value."'";
									 	$result = $db->query("
												SELECT 
													categ.idCategory as id,
													categ.name as name,
													IF(info.id IS NULL, 'false', 'true') as selected
												FROM 
													kernel_tag_category_user_set categ
													LEFT JOIN (
														kernel_tag_information info 
															INNER JOIN
																kernel_tag_information_category info_categ
														) ON (categ.idCategory = info_categ.idCategory
															AND info.tableOrig='".$this->table."'
															AND info.idOrig='".$key_value."'
															AND info.id=info_categ.idInformation
														)
												WHERE categ.login='".$login."'");
								 	} else {
								 		$result = $db->query("
												SELECT 
													categ.idCategory as id,
													categ.name as name,
													'false' as selected
												FROM 
													kernel_tag_category_user_set categ
												WHERE categ.login='".$login."'");
								 	}
								 	
						 			
								 	echo '<p class="form-input"><label for="category">'.(isset($this->option["category"]['legend'])?$this->option["category"]['legend']:"Category").'</label>';
									echo '<select name="category[]" multiple>';
									while ($row = $db->fetchArray($result)) {
										$value=$row['id'];
										$label=stripslashes($row['name']);
										if(($update) and $row['selected']=="true" ){
											$sel = 'selected="selected"';
										} else {
											$sel ='';
										}
										echo '<option value="'.$value.'" '.$sel.' >'.$label.'</option>';
									}
									echo '</select></p>';
								}
								//Sauvegarde du bordel !
								$_SESSION['magicForm_'.$this->id] = serialize($this);

							}

							echo '<p class="form-submit"><input type="submit" value="'.(isset($option['button'])?$option['button']:'Envoyer').'" /></p>';	
							echo '</fieldset></form>';
						}

						public function verifForm($send_data){
							if(!isset($send_data)){
								return false;
							}
							
							return $send_data;
						}

						public function undo(){
							global $page,$db;
							$request = 'DELETE FROM '.$this->table.' WHERE 1';
							$requestMore = "";
							foreach($this->champ as $id => $name){
								$requestMore .= ' AND '.$name.'='.$this->valueMore[$id].' ';
							}
							if($requestMore == ""){
								echo Locale::display('fail','kernel');
							}else{
								$res = $db->query($request.$requestMore.' LIMIT 1',1);
								$aR = $db->affectedRows();
								echo $aR;
								if($aR != 1){
									echo Locale::display('fail','kernel');
								}else{
									echo Locale::display('undo_success','kernel');
								}
							}
						}

						//cree un nouvel enregistrement
						public function saveData($post){
							global $db,$page,$core;
							$core->loadClass('date');
							$date = new Date();

							if($post==false){
								return $post;
							}
							
							$undo ='';
							if(!isset($this->option['undo']) || $this->option['undo'] == true){
								$undo = ' <a class="undo" onclick="AJAX.getAndUpdate(\''.$core->makeUrl('include/ajax/framework.php?action=magicformUndo&id='.$this->id).'\', \'MagicForm_'.$this->id.'\');" >'.Locale::display('undo','kernel').'</a>';
								//On sauvegarde les donnée envoyées en post pour le Undo
							}
							$requestStart = 'INSERT INTO '.$this->table.' (';
									$requestMiddle = ')VALUES(';
										$requestEnd = ')';

							$init = 0;
							$this->valueMore = $this->value;
							foreach($this->champ as $id => $name){
								$skip=false;
								if($name=='login'){
									$mf_name='mf_login';												
								} else{												
									$mf_name=$name;
								}
								
								$requestStart .= (($init==0)?'':',').$name;
								if($this->valueMore[$id] == '?'){
									$this->valueMore[$id] = (isset($post[$mf_name])?"'".$db->escapeString($post[$mf_name])."'":"''");
								}
								//astuce crade pour les dates en attendant la fonction verifForm()
								$requestMiddle .= (($init==0)?'':',').$date->reverseDate($this->valueMore[$id]);
								
								$init++;
							}
							
							$request = $requestStart.$requestMiddle.$requestEnd;
							$db->query($request);
							
														
							if (isset($this->option['category'])) {
								$core->loadClass("tag");
								$categories = isset($post['category']) ? $post['category'] : Array() ;
								$keyName = "";
							 	foreach (array_keys($this->field) as $fieldName) {
							 		if ($this->field[$fieldName]['key']) {
							 			// The key must be unique to be able to use the tag frontend
							 			// please modify your database !!
							 			assert($keyName=="");
							 			$keyName=$fieldName;
							 		}
							 	}
								$tag = new Tag(array($this->table=>$keyName),$core,$db);
								$last_insert_id=$db->insertId();
								$idInformation=$tag->getIdInformation($this->table,$last_insert_id);
								$tag->setInformationCategory($idInformation, $categories);
							}
																	
							if(isset($this->option['message']) && $this->option['message'] != false && $this->option['message'] != ''){
								if($this->option['feedback'] == 'popup'){
									echo $this->option['message'].$undo;
								}else{
									$page->msgInfo($this->option['message'].$undo);
								}
							}elseif(!isset($this->option['message'])){
								if(isset($this->option['feedback']) && $this->option['feedback']=='popup'){
									echo Locale::display('add_success','kernel').$undo;
								}else{
									$page->msgInfo(Locale::display('add_success','kernel').$undo);
								}
							}
							if(isset($this->option['once']) && $this->option['once']==false && (isset($this->option['feedback']) && $this->option['feedback'] == 'fixe')){
								$this->display();
							}


							//Sauvegarde du bordel !
							$_SESSION['magicForm_'.$this->id] = serialize($this);

							return true;

						}

						//met a jour l'enregistrement considere
						public function updateData($post){
							global $db,$page,$core;
							$core->loadClass('date');
							$date = new Date();
							
							if($post==false){
								return $post;
							}
							
							$request = 'UPDATE '.$this->table.' SET ';
							$init = 0;
							$this->valueMore = $this->value;
							
							$categories=array();
							foreach($this->champ as $id => $name){
								if($name=='login'){
									$mf_name='mf_login';
								}else{
									$mf_name=$name;
								}
								
								$request .= (($init==0)?'':',').$name.'=';
								if($this->valueMore[$id] == '?'){
									$this->valueMore[$id] = (isset($post[$mf_name])?"'".$db->escapeString($post[$mf_name])."'":"''");
								}
								//astuce crade pour les dates en attendant la fonction verifForm()
								$request .= $date->reverseDate($this->valueMore[$id]);
								$init++;
							}

							$request .= " WHERE ".$_POST['field']."='".$_POST['pattern']."' ";
							$db->query($request);
							
							if (isset($this->option['category'])) {
								$core->loadClass("tag");
								$categories=$post['category'];
								$keyName = "";
							 	foreach (array_keys($this->field) as $fieldName) {
							 		if ($this->field[$fieldName]['key']) {
							 			// The key must be unique to be able to use the tag frontend
							 			// please modify your database !!
							 			assert($keyName=="");
							 			$keyName=$fieldName;
							 		}
							 	}
								$tag = new Tag(array($this->table=>$keyName),$core,$db);
								$id=$_POST['pattern'];
								$idInformation=$tag->getIdInformation($this->table,$id);
								$tag->setInformationCategory($idInformation, $categories);
							}
							
							if(isset($this->option['message']) && $this->option['message'] != false && $this->option['message'] != ''){
								if($this->option['feedback']=='popup'){
									echo $this->option['message'];
								}else{
									$page->msgInfo($this->option['message']);
								}
							}elseif(!isset($this->option['message'])){
								if($this->option['feedback']=='popup'){
									echo Locale::display('update_success','kernel');
								}else{
									$page->msgInfo(Locale::display('update_success','kernel'));
								}
							}
							
							if(isset($this->option['once']) && $this->option['once']==false && (isset($this->option['feedback']) && $this->option['feedback'] == 'fixe')){
								$this->display();
							}

							//Sauvegarde du bordel !
							$_SESSION['magicForm_'.$this->id] = serialize($this);

							return true;
						}
}
