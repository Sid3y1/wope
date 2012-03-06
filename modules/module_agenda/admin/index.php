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



  $SECU = 'admin';       //Niveaux de sécurité (voir $session->authPage())
  //Obligatoire
  include('../../../include/main.inc');

  $vignette_largeur = 130;
  $vignette_hauteur = 175;  
  
  $page = new Page();
  $page->header(locale::display('edit_event','events'),'agenda');
  
    $core->loadClass('date');
    $date = new Date();
?>
<h1><?=locale::display('edit_event','events')?></h1>
<?php    
  echo '<p class="menu_admin_ev" align="center"><a href="'.$core->makeUrl('modules/module_agenda/new.php').'" title="'.locale::display('add_event','events').'"> '.locale::display('add_event','events').' </a>| <a href="../index.php">'.locale::display('event_list','events').'</a>| <a href="edit_type.php">'.locale::display('edit_type','faq').'</a></p>';
  
?>



<?php

$frm=$_REQUEST;

if(isset($frm['action'])){
	switch ($frm['action']) {
	   case 'delete':
	      $db->query("DELETE FROM eventsListe WHERE id='".$frm['id']."'");

	      $image = "affiches/".$frm['id'].".jpg";
	      if (is_file($image)) unlink($image);

	      break;

	   case 'del_picture':
	      $image = "affiches/".$frm['id'].".jpg";
	      if (is_file($image)) unlink($image);
	      break;

	   case 'display':
	      $db->query("UPDATE eventsListe SET actif='".$frm['status']."' WHERE id='".$frm['id']."'");
	      break;

	   case 'save_modif':
	      $db->query("UPDATE eventsListe SET titre='".$frm['titre']."', date='".$date->reverseDate($frm['date'])."', heure='".$frm['heure']."', lieu='".$frm['lieu']."', auteur='".$frm['auteur']."', type='".$frm['type']."',description='".$frm['description']."' WHERE id='".$frm['id']."'");

	      if(isset($_FILES['frm_image'])){
		 $url = 'affiches/'.$_POST['id'].'.jpg';
		 switch($_FILES['frm_image']['error']) {
		    case 0:
		       //On ne veut que des images jpeg
		       $size = getimagesize($_FILES['frm_image']['tmp_name']);
		       if ($size[2] >= 1 && $size[2] <= 3) {
			  //On enregistre l'image
			  if(move_uploaded_file($_FILES['frm_image']['tmp_name'], $url)){
			  //On redimensionne l'image grace à imagemagik
			  //exec("/usr/bin/convert -compress JPEG -thumbnail 131x175 $url $url");
	             
	             //On redimensionne l'image grace à la classe file
	            $core->loadClass('file');
	            $file = new file();
	            if($file->image_resize($url,$vignette_largeur,$vignette_hauteur) == false)$page->msgError(locale::display('img_error','event'));
			  }
		     }	
		    break;
		 }
	      }


	      break;

	   case 'add_new':
	      $db->query("INSERT INTO eventsListe (titre) VALUES ('".$frm['titre']."')");
	      $id = $db->insertId();
	      $affichage='frm_modif';
	      break;

	}
}

if(!isset($affichage))$affichage='';

switch($frm['affichage']) {

   default:
        echo '
        <form action="'.$_SERVER['PHP_SELF'].'" method="post">
        <input type="hidden" name="action" value="add_new">
        <table align="center" cellpadding="1" cellspacing="0" class="dataTable">
        <tr>
         <th>#</th>
         <th>'.locale::display('date','kernel').'</th>
         <th>'.locale::display('title','kernel').'</th>
         <th>'.locale::display('type','kernel').'</th>
         <th align=center>'.locale::display('visible','kernel').'</th>
         <th align=center>'.locale::display('delete','kernel').'</th>
        </tr>
        ';
        $result = $db->query("SELECT eventsListe.id AS id, eventsListe.titre AS titre, eventsListe.date AS date, eventsType.imageMini AS imageMini, eventsListe.actif AS actif FROM eventsListe LEFT OUTER JOIN eventsType ON eventsType.id = eventsListe.type ORDER BY date DESC");
        while ($row = $db->fetchArray($result)) {
           if ($row['actif']=='N') {
              $color='#FF0000';
           } else {
              $color='';
           }
           echo '<tr bgcolor="'.$color.'">
            <td align="center"><a href="'.$_SERVER['PHP_SELF'].'?affichage=frm_modif&id='.$row['id'].'">'.$row['id'].'</a></td>
            <td><a href="'.$_SERVER['PHP_SELF'].'?affichage=frm_modif&id='.$row['id'].'">'.$row['date'].'</a></td>
	    

	    
            <td><a href="'.$_SERVER['PHP_SELF'].'?affichage=frm_modif&id='.$row['id'].'">'.$row['titre'].'</a></td>
            <td align="center"><img src="images/'.$row['imageMini'].'"></td>
            <td align="center">';

           if ($row['actif']=='N') {
              echo '<a href="'.$_SERVER['PHP_SELF'].'?action=display&status=Y&id='.$row['id'].'">'.locale::display('validate','kernel').'</a>';
           } else {
              echo '<a href="'.$_SERVER['PHP_SELF'].'?action=display&status=N&id='.$row['id'].'">'.locale::display('cancel','kernel').'</a>';
           }

            echo '</td>
            <td align="center"><a href="'.$_SERVER['PHP_SELF'].'?action=delete&id='.$row['id'].'">'.locale::display('erase','kernel').'</a></td>

  </tr>';
        }
        echo '
        <tr>
         <td>New</td>
         <td colspan="3"><input type="text" size="40" name="titre" value="" maxlength="40"></td>
         <td align="center"><input type="submit" value="'.locale::display('add','kernel').'"></td>
        </tr>
        </table>
        </form>';
   break;

   case 'frm_modif':
      $row = $db->fetchArray($db->query("SELECT * FROM eventsListe WHERE id='".$frm['id']."'"));
   ?>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post" enctype="multipart/form-data">
 <input type="hidden" name="id" value="<?=$frm['id']?>">
 <input type="hidden" name="action" value="save_modif">
 <p class="form-ligne">
	<label for="login_auteur"><?=locale::display('author','kernel')?> (<?=locale::display('login','kernel')?>)</label>
	<input type="text" style="background:transparent;" onFocus="this.blur();" id="login_auteur" value="<?=$core->getLogin()?>"/>
  </p>
  <p class="form-ligne">
	<label for="frm[titre]"><?=locale::display('title','kernel')?></label>
	<input type="text" size="20" maxlength="40" name="titre" id="titre" value="<?=$row['titre']?>"/>
  </p>
  <p class="form-ligne">
	<label for="date"><?=locale::display('date','kernel')?></label>
	<input type="text" size="10" maxlength="10" name="date" id="date" value="<?=$date->formatDate($row['date'])?>"/>
	<a href="javascript:calendar.init('date');">
	<img src="<?=$core->makeUrl('images/pics/calendar.gif')?>" border="0" alt="<?=locale::display('calendar','kernel')?>"></a> (jj/mm/aaaa) 
  </p>
  <p class="form-ligne">
	<label for="heure"><?=locale::display('hour','kernel')?></label>
	<input type="text" size="5" maxlength="5" name="heure" id="heure" value="<?=$row['heure']?>"/>
  </p>
  <p class="form-ligne">
	<label for="lieu"><?=locale::display('place','kernel')?></label>
	<input type="text" size="20" maxlength="20" name="lieu" id="lieu" value="<?=$row['lieu']?>"/>
  </p>
  <p class="form-ligne">
	<label for="auteur"><?=locale::display('author','kernel')?></label>
	<input type="text" size="20" maxlength="20" name="auteur" id="auteur" value="<?=$row['auteur']?>"/>
  </p>
  <p class="form-ligne">
	<label for="type"><?=locale::display('type','kernel')?></label>
	<select name="type" id="type">
	    <?php
	    $result2 = $db->query("SELECT id,titre FROM eventsType ORDER BY titre ASC");
	    while ($row2 = $db->fetchArray($result2)) {
	       if ($row2['id'] == $row['type']) $selected = 'selected="selected"'; else $selected='';
	       echo '<option value="'.$row2['id'].'" '.$selected.'>'.$row2['titre'].'</option>';
	    }

	    ?>
	</select>
  </p>
  <p class="form-ligne">
	<label for="description"><?=locale::display('description','kernel')?></label>
	<textarea cols="30" rows="7" name="description" id="description"><?=$row['description']?></textarea>
  </p>
  <p class="form-ligne">
	<label for="frm_image"><?=locale::display('image_jpg','events')?></label>
   <?php
   $image = 'affiches/'.$row['id'].'.jpg';
   if (is_file($image)) {
      echo '<img src="'.$frm['image'].'" alt="" id="frm_image"><a href="index.php?affichage=frm_modif&action=del_picture&id='.$frm['id'].'">'.locale::display('erase','kernel').'</a>';
   } else {
      echo '<input type="file" name="frm_image" id="frm_image" size="20">';
   }
   ?>
  </p>
  <p class="form-ligne">
	<input type="submit" value="<?=locale::display('save','kernel')?>">   
    </p>
</form>
   <?php
   break;
}
?>

<?php
   $page->footer();
?>
