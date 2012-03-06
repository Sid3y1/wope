
  <table>
    <tr>
      <td>/*text*/</td>
      <td>
      	<table>  
<?php
	/*extraphp*/
	$query="/*query*/";
	$result=$db->query($query);
	while ($row=$db->fetchArray($result,MYSQL_ASSOC)) {
		$selected="";
		if (is_array(getValue($values,'/*id*/'))) {
			if (in_array($row['id'],getValue($values,'/*id*/'))) {
				$selected="checked";
			}
		} else {
			if ($row['id']==getValue($values,'/*id*/')) {
				$selected="checked";
			}
		}
?>
<tr>
	<td><label for="id_/*id*/_<?=$row['id']?>"><?=$row['value']?></label></td>
	<td><input id="id_/*id*/_<?=$row['id']?>" type="/*type*/" name="/*name*/" value="<?=$row['id']?>" <?=$selected;?> /></td>
</tr>
<?php 
	}
	?>
			</table>
		</td>
    </tr>    
  </table>