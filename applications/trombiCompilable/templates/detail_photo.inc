<?php
if( $values['/*id*/'] != '' && $values['/*id*/'] != 0 ) {
	$/*id*/_photoUrl=$values['/*id*/'];
?>
<dt>/*text*/</dt>
<dd>
<?php
	echo $fm->preview($/*id*/_photoUrl, 1);
?>
</dd>
</dl>
<dl>
<div class="clear"></div>

<?php	
}
?>
