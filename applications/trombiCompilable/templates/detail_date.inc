<?php
if( $values['/*id*/'] != '0000-00-00' && $values['/*id*/'] != '00-00-0000' ){
	echo "
<dt>/*text*/</dt>
<dd>".( preg_match("/^([0-9]{4})\-([0-9]{2})\-([0-9]{2})/",$values['/*id*/'], $temp) ? $temp[3]."/".$temp[2]."/".$temp[1] : $values['/*id*/'])."</dd>
<div class=\"clear\"></div>
	";
}
?>
