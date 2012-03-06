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

function getValue($values, $nom) {
	if (isset($_POST[$nom])){
		if (get_magic_quotes_gpc()) {
			return stripslashes($_POST[$nom]);
		} else {
			return $_POST[$nom];
		}
	} elseif (isset($values[$nom])) {
		return stripslashes(html_entity_decode($values[$nom], ENT_QUOTES, 'UTF-8'));
	} else {
		return "";
	}	
}

function getError($error,$id) {
	if (isset($error[$id])) {
		return $error[$id];
	} else {
		return "";
	}
}


$core->loadClass('filemanager');
$locale=Locale::getLocale();


$error = Array();
$values = Array();
$displayErrors = false;
	
if( isset($idUser) && $idUser != "" ){
	$idUser = (int)$_GET["id_user"];
	if( !$core->verifDroits('admin') ){
		echo $locale->display('no_page_right','kernel');
		$page->footer();
		exit();
	}
} else {
	$idUser = $core->getUserId();	
}


$query="SELECT count(id) as nb FROM trombi_trombi WHERE id_user='$idUser'";
$result=$db->query($query);

if ($db->result($result,"nb")==1) {
	$query="SELECT * FROM trombi_trombi WHERE id_user='$idUser'";
	$result=$db->query($query);
	$values = $db->fetchArray($result,MYSQL_ASSOC);
	$idTrombi=$values['id'];
	$mode="UPDATE";
} else {
	$mode="INSERT";
}


if (isset($_POST['save'])){

	//recovers the files manager in the session
	if( isset($_POST['fm_id']) && isset($_SESSION['file_manager_'.$_POST['fm_id']]) ){
		$fm = unserialize($_SESSION['file_manager_'.$_POST['fm_id']]);
		$save_files = true;
	//or creates it
	}else{
		$fm = new FileManager();
		$save_files = false;
	}
	
	$valid=true;
	$falsevalid=false;
	
	//saves files if existing
	if($save_files){
		$photo_ids = $fm->saveOrGet('image', 'log', 'log');
	}
	
	
	if (!preg_match("/^.+$/",$_POST["nickname"])) {
$valid=false;
$error["nickname"]="Ce champ doit être rempli !";
};
if (!preg_match("/^.+$/",$_POST["lastname"])) {
$valid=false;
$error["lastname"]="Ce champ doit être rempli";
};
if (!preg_match("/^.+$/",$_POST["firstname"])) {
$valid=false;
$error["firstname"]="Ce champ doit être rempli";
};
if( $_POST["day_birthdate"]!="" && $_POST["month_birthdate"]!="" && $_POST["year_birthdate"]!="" ){
$post_birthdate= $_POST["birthdate"] = $_POST["year_birthdate"]."-".$_POST["month_birthdate"]."-".$_POST["day_birthdate"];
} else { 
$post_["birthdate"]="0000-00-00";
} 

			if( isset($photo_ids) && isset($photo_ids['photo']) && $photo_ids['photo'] != ''){
				$values["photo"] = $_POST["photo"] = $photo_ids['photo'];
			}elseif( isset($_POST["delete_photo"]) && $_POST["delete_photo"] == "delete" ){
				$_POST["photo"] = $values["photo"] = "";
			}
		if (!preg_match("/^([0-9]+)?$/",$_POST["postal_code"])) {
$valid=false;
$error["postal_code"]="Code Postal invalide";
};
if (!preg_match("/^([\+0-9]+)?$/",$_POST["phone"])) {
$valid=false;
$error["phone"]="Telephone invalide";
};
if (!preg_match("/^([a-zA-Z0-9_\-\.]+@([a-zA-Z0-9_\-]+\.)+[a-zA-Z]+)?$/",$_POST["email"])) {
$valid=false;
$error["email"]="Email invalide";
};
if (!preg_match("/^([a-zA-Z0-9_\-\.]+@([a-zA-Z0-9_\-]+\.)+[a-zA-Z]+)?$/",$_POST["email2"])) {
$valid=false;
$error["email2"]="Email invalide";
};



	if( $valid ){
		$query = $mode." trombi_trombi SET id_user='".$core->getUserId()."', nickname='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['nickname']), ENT_QUOTES, "UTF-8")))."', lastname='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['lastname']), ENT_QUOTES, "UTF-8")))."', firstname='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['firstname']), ENT_QUOTES, "UTF-8")))."', birthdate='$post_birthdate', signal_social='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['signal_social']), ENT_QUOTES, "UTF-8")))."', position='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['position']), ENT_QUOTES, "UTF-8")))."', photo='".mysql_real_escape_string(stripslashes($_POST['photo']))."', hobbies='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['hobbies']), ENT_QUOTES, "UTF-8")))."', address='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['address']), ENT_QUOTES, "UTF-8")))."', address2='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['address2']), ENT_QUOTES, "UTF-8")))."', postal_code='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['postal_code']), ENT_QUOTES, "UTF-8")))."', city='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['city']), ENT_QUOTES, "UTF-8")))."', phone='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['phone']), ENT_QUOTES, "UTF-8")))."', email='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['email']), ENT_QUOTES, "UTF-8")))."', email2='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['email2']), ENT_QUOTES, "UTF-8")))."', site='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['site']), ENT_QUOTES, "UTF-8")))."', msn='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['msn']), ENT_QUOTES, "UTF-8")))."', jabber='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['jabber']), ENT_QUOTES, "UTF-8")))."', yahoo='".mysql_real_escape_string(str_replace(Array("\n", "  "), Array("<br />", "&nbsp;&nbsp;"), htmlentities(stripslashes($_POST['yahoo']), ENT_QUOTES, "UTF-8")))."' ";
		if( $mode == "UPDATE" ){
			$query .= "WHERE id='$idTrombi'";	
		} else {
			$query .= "";		
		}
		if( $db->query($query) ){
			$page->msgInfo($locale->display('card_saved','trombi'));
			if( !$falsevalid ){
				//$core->url->redirect("/*relRep*/");
			}	
		} else {
			$page->msgError($locale->display('data_error','trombi'));	
		}
	} else {
		$displayErrors = true;
	}
	
//first posting of the form
}else{
	$fm = new FileManager();
}


if (isset($_GET['id_user'])) {
	$action = "?id_user=".urlencode($_GET['id_user']);
} else {
	$action = "";
}


$tabs_array['Informations générales']='Informations générales';
$tabs_array['Coordonnées personnelles']='Coordonnées personnelles';



//menu
echo '<div class="admin_menu1">';
$page->moduleHeader('Actions','');

echo '<script type="text/javascript">';
echo 'function hideAllPersonnalInfo() {';
foreach ($tabs_array as $tab) {
	echo 'hide(document.getElementById(\''.sha1($tab).'\'));';
}
echo '}';
echo '</script>';
echo '<ul class="panel">';

foreach ($tabs_array as $tab) {
	echo '<li><a onclick="javscript:hideAllPersonnalInfo();show(document.getElementById(\''.sha1($tab).'\'))">'.$tab.'</a></li>';
}
$page->moduleFooter();
echo '</div>';


//content : forms
echo '<div id="admin_info_content" class="admin_menu2">';

if( $displayErrors ){
	$err_msg = '<div><dl>';

if( isset($error["nickname"]) ){
$err_msg .= "<dt>Pseudonyme :</dt><dd>".$error["nickname"]."</dd>";
}
if( isset($error["lastname"]) ){
$err_msg .= "<dt>Nom :</dt><dd>".$error["lastname"]."</dd>";
}
if( isset($error["firstname"]) ){
$err_msg .= "<dt>Prénom :</dt><dd>".$error["firstname"]."</dd>";
}
if( isset($error["birthdate"]) ){
$err_msg .= "<dt>Date de naissance : </dt><dd>".$error["birthdate"]."</dd>";
}
if( isset($error["signal_social"]) ){
$err_msg .= "<dt>Signal social :</dt><dd>".$error["signal_social"]."</dd>";
}
if( isset($error["position"]) ){
$err_msg .= "<dt>Poste occupé :</dt><dd>".$error["position"]."</dd>";
}
if( isset($error["photo"]) ){
$err_msg .= "<dt>Photo : </dt><dd>".$error["photo"]."</dd>";
}
if( isset($error["hobbies"]) ){
$err_msg .= "<dt>Vos passions :</dt><dd>".$error["hobbies"]."</dd>";
}
if( isset($error["address"]) ){
$err_msg .= "<dt>Rue :</dt><dd>".$error["address"]."</dd>";
}
if( isset($error["address2"]) ){
$err_msg .= "<dt>Complément adresse (appartement, étage...) :</dt><dd>".$error["address2"]."</dd>";
}
if( isset($error["postal_code"]) ){
$err_msg .= "<dt>Code Postal :</dt><dd>".$error["postal_code"]."</dd>";
}
if( isset($error["city"]) ){
$err_msg .= "<dt>Ville :</dt><dd>".$error["city"]."</dd>";
}
if( isset($error["phone"]) ){
$err_msg .= "<dt>Téléphone :</dt><dd>".$error["phone"]."</dd>";
}
if( isset($error["email"]) ){
$err_msg .= "<dt>Courriel :</dt><dd>".$error["email"]."</dd>";
}
if( isset($error["email2"]) ){
$err_msg .= "<dt>Courriel secondaire :</dt><dd>".$error["email2"]."</dd>";
}
if( isset($error["site"]) ){
$err_msg .= "<dt>Site Web :</dt><dd>".$error["site"]."</dd>";
}
if( isset($error["msn"]) ){
$err_msg .= "<dt>Compte Msn</dt><dd>".$error["msn"]."</dd>";
}
if( isset($error["jabber"]) ){
$err_msg .= "<dt>Compte GoogleTalk/Jabber</dt><dd>".$error["jabber"]."</dd>";
}
if( isset($error["yahoo"]) ){
$err_msg .= "<dt>Compte Yahoo</dt><dd>".$error["yahoo"]."</dd>";
}


	$err_msg .= '</dl></div>';
	$page->msgError('<div>'.$locale->display('data_error','trombi').'</div>'.$err_msg);
}


do{
  $fm_id=rand(1000000, 9999999);
}while( isset($_SESSION['file_manager_'.$fm_id]) );

?>
<form action="<?=$action?>" method="post" enctype="multipart/form-data">
	<div class="input-hidden">
		<input type="hidden" name="fm_id" value="<?=$fm_id?>" />
	</div>

<div id="<?=sha1("Informations générales");?>">
<?php 
$page->moduleHeader('Informations générales','');
?>
<p class="form-input">
    <label for="id_nickname">Pseudonyme :</label>
	<input id="id_nickname" maxlength="30" type="text" name="nickname" value="<?=getValue($values,"nickname");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_nickname"><?=getError($error,"nickname");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_lastname">Nom :</label>
	<input id="id_lastname" maxlength="30" type="text" name="lastname" value="<?=getValue($values,"lastname");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_lastname"><?=getError($error,"lastname");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_firstname">Prénom :</label>
	<input id="id_firstname" maxlength="30" type="text" name="firstname" value="<?=getValue($values,"firstname");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_firstname"><?=getError($error,"firstname");?></label>
	</div>
</p>
<p class="form-input">
    <p class="form-input">
    <label for="id_birthdate">Date de naissance :</label>
	
	
		<select name="day_birthdate" id="id_day_birthdate">
<?php
	if(preg_match("/^([0-9]{4})\-([0-9]{2})\-([0-9]{2})/",getValue($values,"birthdate"), $temp)){
		$day = $temp[3];
		$month = $temp[2];
		$year = $temp[1];
	}else{
		$day = $month = $year = '';
	}
	
	for($i = 1; $i <= 31; $i++){
	  $d = $i < 10 ? '0'.$i : $i;
    $sel = '';
    if( $day == $d ){
      $sel = ' selected="selected" ';
    }
    echo '<option value="'.$d.'" '.$sel.' >'.$d.'</option>';
  }
?>
		</select>
		<select name="month_birthdate" id="id_month_birthdate">
<?php
  for($i = 1; $i <= 12; $i++){
    $m = $i < 10 ? '0'.$i : $i;
    $sel = '';
    if( $month == $m ){
      $sel = ' selected="selected" ';
    }
    echo '<option value="'.$m.'" '.$sel.' >'.$m.'</option>';
  }
?>
		</select>
		<select name="year_birthdate" id="id_year_birthdate">
<?php
	$now = getdate();
  $cur_year = $now['year'];
	$start_year = $cur_year - 100;
	$end_year = $cur_year - 5;
	
	for($y = $start_year; $y <= $end_year; $y++){
    $sel = '';
    if( $year == $y ){
			$sel = ' selected="selected" ';
		}
		echo '<option value="'.$y.'" '.$sel.' >'.$y.'</option>';
	}
?>
		</select>
	
	
		<span>jj/mm/aaaa</span>
	
	
	
	<label for="id_birthdate"><?=getError($error,"birthdate");?></label>
	<div id="cal_container"></div>
</p>
<p class="form-input">
    <label for="id_signal_social">Signal social :</label>
	<input id="id_signal_social" maxlength="50" type="text" name="signal_social" value="<?=getValue($values,"signal_social");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_signal_social"><?=getError($error,"signal_social");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_position">Poste occupé :</label>
	<input id="id_position" maxlength="50" type="text" name="position" value="<?=getValue($values,"position");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_position"><?=getError($error,"position");?></label>
	</div>
</p>
<p class="form-input">
	<label for="id_photo">Photo :</label>	
<?php

//
//if($values['photo']!="" and !$falsevalid) {
//	if (!isset($photo_photoUrl)) {
//		$photo_photoUrl=getValue($values,'photo');	
//	}
//
//	echo '<img src="'.$core->makeUrl("$photo_photoDir$photo_photoUrl").'" />';
//		echo '<p class="form-input">
//			<label for="id_photo">'.$locale->display('delete','kernel').'</label>
//			<input type="CHECKBOX" id="id_photo" name="photo" value="delete">
//		</p>';
//
//}else{
//	echo '<p class="form-input">
//			<input type="hidden" name="MAX_FILE_SIZE" value="2000000" />
//			<label for="id_photo">Photo :</label>
//			<input id="id_photo" name="photo" type="file" />
//			<label for="id_photo">'.getError($error,"photo").'</label>
//	</p>';
//}
//

$val_photo = getValue($values,"photo");
if($val_photo != "") {
	echo $fm->preview($val_photo, 1);
	echo '</p>';
	
?>
<div class="input-hidden">
	<input type="hidden" id="id_photo" name="photo" value="<?=$val_photo;?>" />
</div>
<p class="form-input">
	<label for="delete_photo"><?=$locale->display('delete','kernel');?></label>
	<input type="CHECKBOX" id="delete_photo" name="delete_photo" value="delete">
</p>
<p class="form-input">
<?php

}

$fm->uploadOrChoose(true, 'image', '', true, 'photo');

?>
	<label for="id_photo"><?=getError($error,"photo");?></label>
</p>

<?
//TODO Refaire la partie javascript
?>

<p class="form-textarea">
	<label for="id_hobbies">Vos passions :</label>
	<textarea name="hobbies" id="id_hobbies" 
			onBlur="Calcul_hobbies('aff_hobbies','id_hobbies',400);"
			onChange="Calcul_hobbies('aff_hobbies','id_hobbies',400);" 
			onFocus="Calcul_hobbies('aff_hobbies','id_hobbies',400);"
			onKeydown="Calcul_hobbies('aff_hobbies','id_hobbies',400);" 
			onKeyup="Calcul_hobbies('aff_hobbies','id_hobbies',400);"
			rows="9"
			cols="45"><?=str_replace(Array("<br />", "&nbsp;"), Array("\n", " "), getValue($values,"hobbies"));?></textarea>
	<span></span>
	<label for="id_hobbies"><?=getError($error,"hobbies");?></label>
</p>
<p>
	<input type="text" size="3" name="aff_hobbies" id="aff_hobbies" value="" readonly="readonly" />
   <script  type="text/javascript">
  function Calcul_hobbies(id_affich,id_compte,max_carac)
  {
    max_carac = max_carac || 550
    var iLongueurRestante,
	  input_comptage = document.getElementById(id_compte),
    iLongueur = input_comptage.value.length;
    if(iLongueur>max_carac)
    {
      input_comptage.value = input_comptage.value.substring(0,max_carac);
      iLongueurRestante = 0;
    }
    else
      iLongueurRestante = max_carac - iLongueur;
    document.getElementById(id_affich).value = iLongueurRestante;
  }
  Calcul_hobbies('aff_hobbies','id_hobbies',400);
	</script>
</p>


<p class="form-submit">
	<input type="reset" value="<?=$locale->display('del_change','kernel');?>" />
	<input type="submit" name="save" value="<?=$locale->display('save','kernel');?>" />
</p>
<?php 
$page->moduleFooter();
?>
</div>
<div id="<?=sha1("Coordonnées personnelles");?>">
<?php 
$page->moduleHeader('Coordonnées personnelles','');
?>
<p class="form-input">
    <label for="id_address">Rue :</label>
	<input id="id_address" maxlength="60" type="text" name="address" value="<?=getValue($values,"address");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_address"><?=getError($error,"address");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_address2">Complément adresse (appartement, étage...) :</label>
	<input id="id_address2" maxlength="60" type="text" name="address2" value="<?=getValue($values,"address2");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_address2"><?=getError($error,"address2");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_postal_code">Code Postal :</label>
	<input id="id_postal_code" maxlength="6" type="text" name="postal_code" value="<?=getValue($values,"postal_code");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_postal_code"><?=getError($error,"postal_code");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_city">Ville :</label>
	<input id="id_city" maxlength="60" type="text" name="city" value="<?=getValue($values,"city");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_city"><?=getError($error,"city");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_phone">Téléphone :</label>
	<input id="id_phone" maxlength="15" type="text" name="phone" value="<?=getValue($values,"phone");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_phone"><?=getError($error,"phone");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_email">Courriel :</label>
	<input id="id_email" maxlength="150" type="text" name="email" value="<?=getValue($values,"email");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_email"><?=getError($error,"email");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_email2">Courriel secondaire :</label>
	<input id="id_email2" maxlength="150" type="text" name="email2" value="<?=getValue($values,"email2");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_email2"><?=getError($error,"email2");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_site">Site Web :</label>
	<input id="id_site" maxlength="30" type="text" name="site" value="<?=getValue($values,"site");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_site"><?=getError($error,"site");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_msn">Compte Msn</label>
	<input id="id_msn" maxlength="60" type="text" name="msn" value="<?=getValue($values,"msn");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_msn"><?=getError($error,"msn");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_jabber">Compte GoogleTalk/Jabber</label>
	<input id="id_jabber" maxlength="60" type="text" name="jabber" value="<?=getValue($values,"jabber");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_jabber"><?=getError($error,"jabber");?></label>
	</div>
</p>
<p class="form-input">
    <label for="id_yahoo">Compte Yahoo</label>
	<input id="id_yahoo" maxlength="60" type="text" name="yahoo" value="<?=getValue($values,"yahoo");?>" />
	<span></span>
	<div class="error_block">
		<label for="id_yahoo"><?=getError($error,"yahoo");?></label>
	</div>
</p>


<p class="form-submit">
	<input type="reset" value="<?=$locale->display('del_change','kernel');?>" />
	<input type="submit" name="save" value="<?=$locale->display('save','kernel');?>" />
</p>
<?php 
$page->moduleFooter();
?>
</div>


</form>
<?

echo '</div>';


echo '<script type="text/javascript">';
echo 'hideAllPersonnalInfo();';
$tab = array_shift($tabs_array);
echo 'show(document.getElementById(\''.sha1($tab).'\'));';
echo '</script>';


//saves file manager in session
$_SESSION['file_manager_'.$fm_id] = serialize($fm);

?>
