
  <dl>
    <dt>/*text*/</dt>
    <dd>
      	<dl>  
<?php
	/*extraphp*/
	$query="/*query*/";
	$result=$db->query($query);
	while ($row=$db->fetchArray($result,MYSQL_ASSOC)) {
		$selected=false;
		if (is_array(getValue('/*id*/'))) {
			if (in_array($row['id'],$values['/*id*/'])) {
				$selected=true;
			}
		} else {
			if ($row['id']==$values['/*id*/']) {
				$selected=true;
			}
		}
		if ($selected) {
?>
	<dt><?=$row['value']?></dt>
<?php 
		}
	}
	?>
		</dl>
    </dd>    
  </dl>