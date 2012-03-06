<?php
if( $value['/*id*/'] != '' && $value['/*id*/'] != 0 ) {
	$/*id*/_photoUrl=$value['/*id*/'];
?>
<dt>/*text*/</dt>
<dd class="small">
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
