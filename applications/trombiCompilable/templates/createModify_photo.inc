<p class="form-input">
	<label for="id_/*id*/">/*text*/</label>	
<?php

//
//if($values['/*id*/']!="" and !$falsevalid) {
//	if (!isset($/*id*/_photoUrl)) {
//		$/*id*/_photoUrl=getValue($values,'/*id*/');	
//	}
//
//	echo '<img src="'.$core->makeUrl("$/*id*/_photoDir$/*id*/_photoUrl").'" />';
//		echo '<p class="form-input">
//			<label for="id_/*id*/">'.$locale->display('delete','kernel').'</label>
//			<input type="CHECKBOX" id="id_/*id*/" name="/*id*/" value="delete">
//		</p>';
//
//}else{
//	echo '<p class="form-input">
//			<input type="hidden" name="MAX_FILE_SIZE" value="/*maxSize*/" />
//			<label for="id_/*id*/">/*text*/</label>
//			<input id="id_/*id*/" name="/*id*/" type="file" />
//			<label for="id_/*id*/">'.getError($error,"/*id*/").'</label>
//	</p>';
//}
//

$val_/*id*/ = getValue($values,"/*id*/");
if($val_/*id*/ != "") {
	echo $fm->preview($val_/*id*/, 1);
	echo '</p>';
	
?>
<div class="input-hidden">
	<input type="hidden" id="id_/*id*/" name="/*id*/" value="<?=$val_/*id*/;?>" />
</div>
<p class="form-input">
	<label for="delete_/*id*/"><?=$locale->display('delete','kernel');?></label>
	<input type="CHECKBOX" id="delete_/*id*/" name="delete_/*id*/" value="delete">
</p>
<p class="form-input">
<?php

}

$fm->uploadOrChoose(true, 'image', '', true, '/*id*/');

?>
	<label for="id_/*id*/"><?=getError($error,"/*id*/");?></label>
</p>

