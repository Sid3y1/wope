<?php
if( $value['/*id*/'] != '0000-00-00' && $value['/*id*/'] != '00-00-0000' ){
	echo "
<dt>/*text*/</dt>
<dd class=\"small\">".( preg_match("/^([0-9]{4})\-([0-9]{2})\-([0-9]{2})/",$value['/*id*/'], $temp) ? $temp[3]."/".$temp[2]."/".$temp[1] : $value['/*id*/'])."</dd>
<div class=\"clear\"></div>
	";
}
?>
