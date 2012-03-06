<div id="<?=sha1("/*nom*/");?>">
<?php 
$page->moduleHeader('/*nom*/','');
?>
/*chieldform*/

<p class="form-submit">
	<input type="reset" value="<?=$locale->display('del_change','kernel');?>" />
	<input type="submit" name="save" value="<?=$locale->display('save','kernel');?>" />
</p>
<?php 
$page->moduleFooter();
?>
</div>
