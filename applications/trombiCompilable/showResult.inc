<a name="result_<?=$value['id']?>"></a>
<div class="user_name">
<?php
	$strName = ucwords(strtolower(stripslashes($value['firstname'].' '.$value['lastname'])));

	echo $strName;

  if (isset($value['signal_social']) && $value['signal_social'] != '') {
		echo '<br /><span>'.stripslashes($value['signal_social']).'</span>';
	}

?>
</div>
<div class="clear"></div>

<?php
	//preloads photo
	$photo = $fm->preview($value['photo'], 3, true);
	echo '<span style="display: none;">'.$photo.'</span>';
	echo '<div class="picture_block"><a class="avatar"';

	if( isset($value['photo']) && $value['photo'] != '' ){
		echo ' onclick="dialBox.draw(\'';
		echo $page->htmlBlock->escapeTipContent(
			'<div class="close_avatar_prev"><a class="link_button" onclick="dialBox.erase()">Fermer</a></div><div>'.$photo.'</div>'
		);
		echo '\', \''.$strName.'\', true);" ';
	}
	echo ' ><span></span>'.$fm->preview($value['photo'], 1, true).'</a></div>';

	//cuts strings
	$position = html_entity_decode(stripslashes($value['position']), ENT_QUOTES, 'UTF-8');
	if( strlen($position) > 25 ){
		$cutPosition = htmlentities(substr($position, 0, 22), ENT_QUOTES, 'UTF-8').'...';
		$onMouseOverPosition = ' onmouseover="'.$hb->tooltip($position).'" ';
	}else{
		$cutPosition = htmlentities($position, ENT_QUOTES, 'UTF-8');
		$onMouseOverPosition = '';
	}
	
	$city = html_entity_decode(stripslashes($value['city']), ENT_QUOTES, 'UTF-8');
	if( strlen($city) > 25 ){
		$cutCity = htmlentities(substr($city, 0, 22), ENT_QUOTES, 'UTF-8').'...';
		$onMouseOverCity = ' onmouseover="'.$hb->tooltip($city).'" ';
	}else{
		$cutCity = htmlentities($city, ENT_QUOTES, 'UTF-8');
		$onMouseOverCity = '';
	}
	
	$email = stripslashes($value['email']);
	if( strlen($email) > 25 ){
		$cutEmail = substr($email, 0, 22).'...';
		$onMouseOverEmail = ' onmouseover="activateToolTips(this, \''.$email.'\');" ';
	}else{
		$cutEmail = $email;
		$onMouseOverEmail = '';
	}
?>

<div class="details_block">

			<ul class="infos_block">
					<li>
						<span class="item_title">Poste : </span>
						<span<?=$onMouseOverPosition?>><?=$cutPosition?></span>
					</li>
					<li>
						<span class="item_title">Adresse : </span>
						<span<?=$onMouseOverCity?>><?=$cutCity?></span>
					</li>
					<li>
						<?=$link->mailto($email, $cutEmail, 'mini_mail', $onMouseOverEmail)?>
					</li>
					<li>
						<span class="item_title">Tel. : </span>
						<span><?=$value['phone']?></span>
					</li>
			</ul>

			<ul class="panel">
					<li><a href="<?=str_replace('[#]', $num_page, $url)?>&amp;ga=dc&amp;c=<?=$value['id']?>"><span>V</span>oir sa fiche</a></li>
					<li><a href="http://www.viamichelin.fr/viamichelin/fra/dyn/controller/mapPerformPage?strAddress=<?=urlencode(stripslashes($value['address']))?>&amp;strCP=<?=urlencode($value['postal_code'])?>&amp;strLocation=<?=urlencode(stripslashes($value['city']))?>&amp;strCountry=1424"><span>P</span>lan d'accès</a></li>
<!--					<li><a href=""><span>E</span>ntrer dans son réseau</a></li>
					<li><a onclick="AJAX.getAndUpdate('ajax/anniv.php?addAnniv=1&loginAnniv=3','anniv_3')"><span>M</span>e notifier son anniversaire</a></li>-->
			</ul>		
	
	<div class="clear"></div>
	
</div>

<div class="user_sep clear"></div>
