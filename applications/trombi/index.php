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



function afficheNbPage($href, $num_page, $nb_row) {
	$locale = Locale::getLocale();
	global $limit,$core;
	$core->loadClass('htmlblock');
	$hb = new HtmlBlock();
	$hb->pagesList((int)$nb_row, (int)$limit, (int)$num_page, 'href', $href, $locale->display('page','kernel'));
}


function getValue($name) {
	if( isset($_POST[$name]) ){
		return mysql_escape_string($_POST[$name]);
	} else {
		return "";
	}	
}


function isCardMine($id_card){
	global $core, $db;
	
	$myCardsResult = $db->query("SELECT id FROM trombi_trombi WHERE id_user = '".$core->getUserId()."' ");
	if( isset($myCardsResult) ){
	  while( $row = $db->fetchArray($myCardsResult) ){
			if( $row['id'] == $id_card ){
				return true;
			}
		}
	}

	return false;
}

function logCardVisit($id_card){
	global $core, $db;
	$ret = false;


	$result = $db->query("SELECT COUNT(*) FROM trombi_visit WHERE id_card = '".$id_card."' AND id_user = '".$core->getUserId()."' ");
	if( isset($result) ){
		if( $db->result($result, 0) == 1 ){
			if($db->query("UPDATE trombi_visit SET visits_count = visits_count + 1 WHERE id_card = '".$id_card."' AND id_user = '".$core->getUserId()."' ")){
				$ret = true;
			}
		}else{
			if($db->query("INSERT trombi_visit (id_card, id_user) VALUES ('".$id_card."', '".$core->getUserId()."') ")){
				$ret = true;
			}
		}
	}
	
	return $ret;
	
}

$SECU = "log";
$LOCALE = "trombi";

include ("../../include/main.inc");
include (dirname(__FILE__).'/config_trombi.inc');

$ARIANEWIRE = Array (
	"Accueil" => $core->makeUrl('index.php'),
	"Trombi" => "index.php"
);

$locale = Locale::getLocale();
$page = new Page();
$page->header($locale->display('trombi','kernel'), 'trombi');

$core->loadClass('filemanager');
$core->loadClass('trombi');
$core->loadClass('link');
$core->loadClass('tabs');
$core->loadClass('htmlblock');

$box=new Box();
$fm = new FileManager();
$trombi = new Trombi();
$link = new Link();
$hb = new HtmlBlock();

$secret="fad63c9ae53d8db4f88224d892bffe4ab0bafda6";

$limit= $core->user->getMyPref('trombi_limit');
$limit= ($limit < 10) ? 10 : $limit;

$photo= $core->user->getMyPref('trombi_photo');

$page->adminLink('user', 'default', $config_trombi['admin_rights']);
?>
<div class="clear"></div>
<div class="colomn1">
<?php
$page->moduleHeader($locale->display('seek','kernel'),'');
$menu_search= new Tabs(Array('simple'=>$locale->display('simple_seek','trombi'),'advanced'=>$locale->display('advanced_seek','trombi')));
$menu_search->display();
$menu_search->separatorStart("simple");
?>
<form action="" method="POST" id="fasesearchForm">
	<div class="input-hidden"><input type="hidden" name="ga" value="fs" /></div>
	<p><input type="text" size="15" id="fastsearch" name="fastsearch" value="<?=getValue("fastsearch");?>">
  <input type="submit" value="<?=$locale->display('seek','kernel')?>"></p>
  <p><a class="link_button" href="javascript:gE('fastsearch').value='';gE('fasesearchForm').submit();void(0);" ><?=$locale->display('see_all','trombi')?></a></p>
  <div class="autocomplete_container" id="trombi_autocomplete"></div> 
</form>
<script type="text/javascript"> 
	var phpFile = "./ajax/fastsearch.php";
	var schema = ["results", "value","data"]; 
	var trombi_datasource = new YAHOO.widget.DS_XHR(phpFile, schema); 
	var trombiAutoComp = new YAHOO.widget.AutoComplete("fastsearch","trombi_autocomplete", trombi_datasource); 
	trombiAutoComp.minQueryLength = 3;
	trombiAutoComp.animVert = false;
	trombiAutoComp.animHoriz = false;
	YAHOO.widget.AutoComplete.prototype.formatResult = function(aResultItem, sQuery) { 
	    var sResult = aResultItem[0]; 
	    if(sResult) { 
	        return sResult+aResultItem[1]; 
	    } 
	    else { 
	        return ""; 
	    } 
	}; 
</script> 
<?php
$menu_search->separatorStop("simple");
$menu_search->separatorStart("advanced");
?>
<div id="advance_search">
	<div class="trombi_options_supp">
	  	<form action="" method="POST">
				<div class="input-hidden"><input type="hidden" name="ga" value="s" /></div>
				<fieldset >
<legend class="titre_menu">Informations générales</legend>
<p class="form-input">
    <label for="id_nickname">Pseudonyme :</label>
	<input id="id_nickname" size="15"  maxlength="30" type="text" name="nickname" value="<?=getValue("nickname");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_lastname">Nom :</label>
	<input id="id_lastname" size="15"  maxlength="30" type="text" name="lastname" value="<?=getValue("lastname");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_firstname">Prénom :</label>
	<input id="id_firstname" size="15"  maxlength="30" type="text" name="firstname" value="<?=getValue("firstname");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_position">Poste occupé :</label>
	<input id="id_position" size="15"  maxlength="60" type="text" name="position" value="<?=getValue("position");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_birthdate">Date de naissance :</label>
		<input id="id_birthdate" size="10"  maxlength="10" type="text" name="birthdate" value="<?=getValue("birthdate");?>" />
		<span>jj/mm/aaaa</span>
		
	
	
</p>

</fieldset>
<p></p>
<input type="submit" name="search" value="<?=$locale->display("seek","kernel")?>">
<p></p>
<fieldset >
<legend class="titre_menu">Coordonnées personnelles</legend>
<p class="form-input">
    <label for="id_address">Rue :</label>
	<input id="id_address" size="15"  maxlength="60" type="text" name="address" value="<?=getValue("address");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_postal_code">Code Postal :</label>
	<input id="id_postal_code" size="15"  maxlength="6" type="text" name="postal_code" value="<?=getValue("postal_code");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_city">Ville :</label>
	<input id="id_city" size="15"  maxlength="60" type="text" name="city" value="<?=getValue("city");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_phone">Téléphone :</label>
	<input id="id_phone" size="15"  maxlength="15" type="text" name="phone" value="<?=getValue("phone");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_email">Email Principal :</label>
	<input id="id_email" size="15"  maxlength="150" type="text" name="email" value="<?=getValue("email");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_email2">Email secondaire :</label>
	<input id="id_email2" size="15"  maxlength="150" type="text" name="email2" value="<?=getValue("email2");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_msn">Compte Msn :</label>
	<input id="id_msn" size="15"  maxlength="60" type="text" name="msn" value="<?=getValue("msn");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_jabber">Compte Jabber/GoogleTalk :</label>
	<input id="id_jabber" size="15"  maxlength="60" type="text" name="jabber" value="<?=getValue("jabber");?>" />
	<span></span>
</p>
<p class="form-input">
    <label for="id_yahoo">Compte Yahoo Messenger :</label>
	<input id="id_yahoo" size="15"  maxlength="60" type="text" name="yahoo" value="<?=getValue("yahoo");?>" />
	<span></span>
</p>

</fieldset>
<p></p>
<input type="submit" name="search" value="<?=$locale->display("seek","kernel")?>">
<p></p>

		</form>
	</div>
</div>
<?php
$menu_search->separatorStop("advanced");
$page->moduleFooter();
$page->moduleHeader($locale->display('options','kernel'), 'libre');

$infos_header = $locale->display('infos_perso','trombi');
echo "<ul class='panel'>
	<li><a href='".$core->makeUrl("/user/index.php?part=info")."'><span>".substr($infos_header, 0, 1)."</span>".substr($infos_header, 1)."</a></li>
</ul>";
$page->moduleFooter();
?>
</div>
<div class="colomn2">
<?php



$query= "SELECT COUNT(*) FROM trombi_trombi LEFT JOIN usersInfo ON trombi_trombi.id_user = usersInfo.id WHERE ";
$url="index.php?page=[#]";
$num_page=1;

if ( isset ($_POST["fastsearch"]) ) {
	$my_query = "LOWER(usersInfo.login) LIKE CONCAT('%',LOWER('".$db->escapeString(getValue('fastsearch'))."'),'%') 
		OR LOWER(CONCAT(firstname, ' ',lastname)) LIKE CONCAT('%',LOWER('".$db->escapeString(getValue('fastsearch'))."'),'%') 
		OR LOWER(CONCAT(lastname, ' ',firstname)) LIKE CONCAT('%',LOWER('".$db->escapeString(getValue('fastsearch'))."'),'%')  OR LOWER(nickname) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(lastname) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(firstname) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%') ".(
    			preg_match("/^([0-1]?[0-9])\/([0-3]?[0-9])\/([2,1][0,9][0-9][0-9])/",getValue("fastsearch"), $temp)?
					" OR birthdate='".$temp[3]."-".$temp[2]."-".$temp[1]."'"
					:"")." OR LOWER(phone) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(email) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(email2) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  OR LOWER(site) LIKE CONCAT('%',LOWER('".(getValue('fastsearch'))."'),'%')  ";
	$query .= $my_query;
	$sha = sha1($my_query.$secret);
	$_SESSION['trombi_query_'.$sha] = $my_query;
	$url .= "&amp;query=1&amp;sha=".$sha;
	
} elseif ( isset ($_POST["search"]) ) {

	$my_query = "1 ".(getValue('nickname')==null?"":" AND LOWER(nickname) LIKE CONCAT('%',LOWER('".(getValue('nickname'))."'),'%')")."".(getValue('lastname')==null?"":" AND LOWER(lastname) LIKE CONCAT('%',LOWER('".(getValue('lastname'))."'),'%')")."".(getValue('firstname')==null?"":" AND LOWER(firstname) LIKE CONCAT('%',LOWER('".(getValue('firstname'))."'),'%')")."".(getValue('position')==null?"":" AND LOWER(position) LIKE CONCAT('%',LOWER('".(getValue('position'))."'),'%')")."".((getValue("birthdate"))==null?"":(
    			preg_match("/^([0-1]?[0-9])\/([0-3]?[0-9])\/([2,1][0,9][0-9][0-9])/",getValue("birthdate"), $temp)?
					" AND birthdate='".$temp[3]."-".$temp[2]."-".$temp[1]."'"
					:""))."".(getValue('address')==null?"":" AND LOWER(address) LIKE CONCAT('%',LOWER('".(getValue('address'))."'),'%')")."".(getValue('postal_code')==null?"":" AND LOWER(postal_code) LIKE CONCAT('%',LOWER('".(getValue('postal_code'))."'),'%')")."".(getValue('city')==null?"":" AND LOWER(city) LIKE CONCAT('%',LOWER('".(getValue('city'))."'),'%')")."".(getValue('phone')==null?"":" AND LOWER(phone) LIKE CONCAT('%',LOWER('".(getValue('phone'))."'),'%')")."".(getValue('email')==null?"":" AND LOWER(email) LIKE CONCAT('%',LOWER('".(getValue('email'))."'),'%')")."".(getValue('email2')==null?"":" AND LOWER(email2) LIKE CONCAT('%',LOWER('".(getValue('email2'))."'),'%')")."".(getValue('msn')==null?"":" AND LOWER(msn) LIKE CONCAT('%',LOWER('".(getValue('msn'))."'),'%')")."".(getValue('jabber')==null?"":" AND LOWER(jabber) LIKE CONCAT('%',LOWER('".(getValue('jabber'))."'),'%')")."".(getValue('yahoo')==null?"":" AND LOWER(yahoo) LIKE CONCAT('%',LOWER('".(getValue('yahoo'))."'),'%')")."";
	$query .= $my_query;
	$sha = sha1($my_query.$secret);
	$_SESSION['trombi_query_'.$sha] = $my_query;
	$url .= "&amp;query=1&amp;sha=".$sha;
	
} elseif( isset($_GET["query"]) && $_GET["query"]=='1' ) {

	if ( isset($_GET['sha']) && $_GET['sha'] == sha1($_SESSION['trombi_query_'.$_GET['sha']].$secret) ) {
		$my_query = $_SESSION['trombi_query_'.$_GET['sha']];
		$query .= $my_query;
		$url .= "&amp;query=1&amp;sha=".$_GET['sha'];
		$num_page = intval($_GET["page"]);
	} else {
		$query = null;
	}
	
} else {

	$query = null;
	
}

$action = isset($_POST['ga']) ? $_POST['ga'] : (isset($_GET['ga']) ? $_GET['ga'] : '');

//'cd' = 'display card', 's' = 'search'
if( $action != 'dc' ){
	if ($query != null) {
		$result = $db->query($query);
		$nb_result = $db->result($result, 0);
	}

	if ($query == null){
		$members_nb = $db->result($db->query('SELECT COUNT(*) FROM usersInfo WHERE id <> 1 '), 0);
		$query = "SELECT trombi_trombi.* FROM trombi_trombi LEFT JOIN usersInfo ON trombi_trombi.id_user = usersInfo.id WHERE trombi_trombi.id > ( SELECT CAST(Max(id)-5 as signed) FROM trombi_trombi ) ORDER BY usersInfo.id DESC ";
		$result = $db->query($query);
		$page->moduleHeader($locale->display('last_added', 'trombi'), 'droite');
		echo '<div class="members_nb">Aujourd\'hui : '.$members_nb.' membres inscrits sur le site.</div>';
		echo '<div class="user-list">';
		while ($value= $db->fetchArray($result, MYSQL_ASSOC)) {
?>
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

<?php

		}
		echo "</div>";
		$page->moduleFooter();

	} elseif  ($nb_result == 0) {

		$page->msgInfo(locale :: display('no_result', 'trombi'));
	
	} else {

		$query = "SELECT trombi_trombi.* FROM trombi_trombi LEFT JOIN usersInfo ON trombi_trombi.id_user = usersInfo.id WHERE (";
		$query .= $my_query.") ORDER BY trombi_trombi.lastname ASC LIMIT ".(($num_page - 1)*$limit).",".($limit);
		$result = $db->query($query);
		$page->moduleHeader($locale->display('search_result', 'trombi').' : '.$nb_result.' '.$locale->display('results', 'trombi'), 'droite');
		$url .= '&amp;ga=s';
		afficheNbPage($url, $num_page, $nb_result);
		echo '<dl class="user-list">';
	
		while ($value= $db->fetchArray($result, MYSQL_ASSOC)) {
?>
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

<?php

		}
		echo "</dl>";
		afficheNbPage($url, $num_page, $nb_result);
		$page->moduleFooter();
	
	}
	
}else{

	if( isset($_GET['c']) ){

		if( !isCardMine($_GET['c']) ){
			logCardVisit($_GET['c']);
		}
		
		$query = "SELECT * FROM trombi_trombi WHERE trombi_trombi.id = '".$_GET['c']."' ";
		$result = $db->query($query);
		$value = $db->fetchArray($result);
		
		$strName = ucwords(strtolower(stripslashes($value['firstname'].' '.$value['lastname'])));
		$page->moduleHeader(ucwords($locale->display('trombi_card', 'trombi')).' : '.$strName, 'droite');
		
		echo '';
		
		echo '
		<div class="trombi_card">'
				.'<div class="header_block">'
						.'<div class="search_icon_block">'
								.'<span class="icon_search">&nbsp;</span>'
								.'<a class="search" alt="search icon" href="'.str_replace('[#]', $num_page, $url).'&amp;ga=s#result_'.$value['id'].'">Retourner à la recherche</a>'
						.'</div>'
						.'<div class="clear"></div>'
						.'<div class="user_photos_block">'
								.$trombi->displayUserPhotos($value['id_user'])
						.'</div>'
						.'<div class="clear"></div>';
		
		//preloads photo
		$photo = $fm->preview($value['photo'], 3, true);
  	echo '<span style="display: none;">'.$photo.'</span>';
		
	  echo '
			<div class="email_block">'.$link->mailto(stripslashes($value['email']), 'Contacter', 'mini_mail').'</div>
			<div class="photo_block"><div class="picture_block"><a class="avatar"';

		if( isset($value['photo']) && $value['photo'] != '' ){
			echo ' onclick="dialBox.draw(\''.
				$page->htmlBlock->escapeTipContent('<div class="close_avatar_prev"><a class="link_button" onclick="dialBox.erase()">Fermer</a></div><div>'.$photo.'</div>').
				'\', \''.$strName.'\', true);" ';
		}
		
		echo ' ><span></span>'.$fm->preview($value['photo'], 1, true).'</a>
			</div></div>';
		
		echo '
			<div class="summary_block">
				<div class="name_line"><h3>'.$strName.'</h3>';

				if( isset($value['signal_social']) && $value['signal_social'] != '' ){
				  echo '<span class="social_status">'.stripslashes($value['signal_social']).'</span>';
				}
		
				echo '
				</div>
				<div class="summary_infos">';
				if( isset($value['company_name']) && $value['company_name'] != '' ){
					echo ($value['position'] != '' ? '<h4>'.stripslashes($value['position']).'</h4>' : '').
					'<h4>'.stripslashes($value['company_name']).'</h4>';
				}
				echo '
					<p>'.stripslashes($value['city']).'</p>
				</div>
			</div>
			<div class="clear"></div>
		</div>
		<div class="clear"></div>
			
			<div class="main_block">
		';
		
		if( isset($config_trombi['cv']) && $config_trombi['cv'] ){
			$cv = true;
			$core->loadClass('tabs');
			$tabs = new Tabs( Array('main' => 'Contact', 'cv' => 'CV') );
			$tabs->display();
			$tabs->separatorStart('main');
		}else{
			$cv = false;
		}
		
?>
		<h3>Informations générales</h3>
<dl>
<?php
if( $value['nickname'] != ''){
	echo "
	<dt>Pseudonyme :</dt><dd class=\"small\">".stripslashes($value['nickname'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['lastname'] != ''){
	echo "
	<dt>Nom :</dt><dd class=\"small\">".stripslashes($value['lastname'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['firstname'] != ''){
	echo "
	<dt>Prénom :</dt><dd class=\"small\">".stripslashes($value['firstname'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['position'] != ''){
	echo "
	<dt>Poste occupé :</dt><dd class=\"small\">".stripslashes($value['position'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['birthdate'] != '0000-00-00' && $value['birthdate'] != '00-00-0000' ){
	echo "
<dt>Date de naissance :</dt>
<dd class=\"small\">".( preg_match("/^([0-9]{4})\-([0-9]{2})\-([0-9]{2})/",$value['birthdate'], $temp) ? $temp[3]."/".$temp[2]."/".$temp[1] : $value['birthdate'])."</dd>
<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['signal_social'] != ''){
	echo "
	<dt>Signal social :</dt><dd class=\"small\">".stripslashes($value['signal_social'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['photo'] != '' && $value['photo'] != 0 ) {
	$photo_photoUrl=$value['photo'];
?>
<dt>Photo :</dt>
<dd class="small">
<?php
	echo $fm->preview($photo_photoUrl, 1);
?>
</dd>
</dl>
<dl>
<div class="clear"></div>

<?php	
}
?>
<?php
if( $value['hobbies'] != ''){
	echo "
	<dt>Vos passions :</dt><dd class=\"big\"><div class=\"clear\"></div><p>".stripslashes($value['hobbies'])."</p></dd>
	<div class=\"clear\"></div>
	";
}
?>

</dl>
<h3>Coordonnées personnelles</h3>
<dl>
<?php
if( $value['address'] != ''){
	echo "
	<dt>Rue :</dt><dd class=\"small\">".stripslashes($value['address'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['address2'] != ''){
	echo "
	<dt>Complément adresse (appartement, étage...) :</dt><dd class=\"small\">".stripslashes($value['address2'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['postal_code'] != ''){
	echo "
	<dt>Code Postal :</dt><dd class=\"small\">".stripslashes($value['postal_code'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['city'] != ''){
	echo "
	<dt>Ville :</dt><dd class=\"small\">".stripslashes($value['city'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['phone'] != ''){
	echo "
	<dt>Téléphone :</dt><dd class=\"small\">".stripslashes($value['phone'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['email'] != ''){
	echo "
	<dt>Email Principal :</dt><dd class=\"small\">".stripslashes($value['email'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['email2'] != ''){
	echo "
	<dt>Email secondaire :</dt><dd class=\"small\">".stripslashes($value['email2'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['site'] != ''){
	echo "
	<dt>Site Web :</dt><dd class=\"small\">".stripslashes($value['site'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['msn'] != ''){
	echo "
	<dt>Compte Msn :</dt><dd class=\"small\">".stripslashes($value['msn'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['jabber'] != ''){
	echo "
	<dt>Compte Jabber/GoogleTalk :</dt><dd class=\"small\">".stripslashes($value['jabber'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>
<?php
if( $value['yahoo'] != ''){
	echo "
	<dt>Compte Yahoo Messenger :</dt><dd class=\"small\">".stripslashes($value['yahoo'])."</dd>
	<div class=\"clear\"></div>
	";
}
?>

</dl>

<?php

		if( $cv ){
			$tabs->separatorStop('main');
			$tabs->separatorStart('cv');
?>
		<h3>Formation</h3>
		<div><?=stripslashes($value['studies'])?></div>
		<h3>Expérience</h3>
		<div><?=stripslashes($value['professional_history'])?></div>
		<h3>Langues et informatique</h3>
		<div><?=stripslashes($value['languages'])?></div>
		<h3>Informations complémentaires</h3>
		<div><?=stripslashes($value['history_freespace'])?></div>
<?php
			$tabs->separatorStop('cv');
		}
		
		echo '
			</div>
		</div>';
	
		$page->moduleFooter();
	}
	
}

?>
</div>
<?

$page->footer();
?>
