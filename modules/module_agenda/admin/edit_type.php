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
  include ('../../../include/main.inc');

  $page = new Page();
  $page->header(locale::display('edit_type_event','events'),'agenda');
  
  $affichage=isset($_POST['affichage'])?$_POST['affichage']:(isset($_GET['affichage'])?$_GET['affichage']:"");
  $action=isset($_POST['action'])?$_POST['action']:(isset($_GET['action'])?$_GET['action']:"");
  $id=isset($_POST['id'])?$_POST['id']:(isset($_GET['id'])?$_GET['id']:"");  
?>

<h1><?=locale::display('edit_type_event','events')?></H1>

<?php

  echo '<p>';
  echo '<a href="../new.php" title="'.locale::display('add_event','events').'">'.locale::display('add_event','events') .' </a> | <a href="../index.php">'.locale::display('event_list','events').'</a>';
  if ($core->verifDroits('admin')) {
     echo ' | <a href="index.php">'.locale::display('administration','kernel').'</a>';
  }
  echo "</p>";
if(isset($action)){
	switch ($action) {
	   case 'delete':
	   	   	 if($image=$db->fetchArray($db->query("SELECT image, imageMini FROM eventsType WHERE id='$id'"))){
		      	$img = '../img_types/'.$image['image'];
			      if (is_file($img))
							unlink($img);
		      	$img_mini = '../img_types/'.$image['imageMini'];
		      	if (is_file($img_mini))
							unlink($img_mini);
	   	  }
	      $db->query("DELETE FROM eventsType WHERE id=$id");
	      break;
		
	   case 'del_picture':
	   	  
	   	  if($image=$db->fetchArray($db->query("SELECT image, imageMini FROM eventsType WHERE id='$id'"))){
		      $img = '../img_types/'.$image['image'];
		      if (is_file($img))
						unlink($img);
		      $img_mini = '../img_types/'.$image['imageMini'];
		      if (is_file($img_mini))
						unlink($img_mini);
	   	  }
	      break;
	      
	   case 'save_modif':
	   	 $img_id=rand(); 
	   	  	   
	     $db->query("UPDATE eventsType SET titre='".$_POST['frm']['titre']."', image='".$img_id.'_50x50.jpg'."', imageMini='".$img_id.'_22x22.jpg'."' WHERE id=$id");
	      
	     if(isset($_FILES['frm_image'])){
		 		$url = '../img_types/'.$img_id.'_50x50.jpg';
		 		$url_mini = '../img_types/'.$img_id.'_22x22.jpg';
		 		switch($_FILES['frm_image']['error']) {
		    	case 0:
		      	$size = getimagesize($_FILES['frm_image']['tmp_name']);
						//type d'image : gif, jpeg, png acceptes
		     		if ($size[2] >= 1 && $size[2] <= 3) {
			  			//On enregistre l'image
			  			if(move_uploaded_file($_FILES['frm_image']['tmp_name'], $url) && copy($url, $url_mini)){
	            	//On redimensionne l'image grace à la classe file
	            	$core->loadClass('file');
	            	$file = new file();
	            	if($file->image_resize($url,50,50) == false || $file->image_resize($url_mini,22,22) == false)
									$page->msgError(locale::display('img_error','event'));
			  			}
		     		}	
		    		break;
					default:
						//void
						break;
		 		}
			}
	      
	   	break;

	  case 'add_new':
	    $db->query("INSERT INTO eventsType (titre) VALUES ('".$_POST['frm']['titre']."')");
	    $id = $db->insertId();
	    $affichage='frm_modif';
	    break;

	}
}

if(!(isset($affichage)))$affichage='';

switch($affichage) {

   default:
        echo '
        <form action="'.$_SERVER['PHP_SELF'].'" method="post">
        <input type="hidden" name="action" value="add_new">
        <table align="center" cellpadding="1" cellspacing="0" class="dataTable">
        <tr>
         <th>#</th>
         <th>'.locale::display('title','kernel').'</th>
         <th>'.locale::display('image','kernel').'</th>
         <th>'.locale::display('image_mini','events').'</th>
         <th>'.locale::display('action','kernel').'</th>
        </tr>
        ';
        $result = $db->query("SELECT * FROM eventsType ORDER BY titre ASC");
        while ($row = $db->fetchArray($result)) {
           echo '<tr align=center>
            <td align="center"><a href="'.$_SERVER['PHP_SELF'].'?affichage=frm_modif&id='.$row['id'].'">'.$row['id'].'</a></td>
            <td><a href="'.$_SERVER['PHP_SELF'].'?affichage=frm_modif&id='.$row['id'].'">'.$row['titre'].'</a></td>
            <td><img src="../img_types/'.$row['image'].'" alt="'.$row['image'].'"/></td>
            <td><img src="../img_types/'.$row['imageMini'].'" alt="'.$row['imageMini'].'"/></td>
            <td align="center"><a href="'.$_SERVER['PHP_SELF'].'?action=delete&id='.$row['id'].'">'.locale::display('erase','kernel').'</a></td>
          </tr>';
        }
        echo '
        <tr>
         <td colspan="2">'.locale::display('add_type','events').'</td>
         <td colspan="2" align="center"><input type="text" size="30" name="frm[titre]" value="" maxlength="30"></td>
         <td><input type="submit" value="'.locale::display('new','kernel').'"></td>
        </tr>
        </table>
        </form>';
   break;

   case 'frm_modif':
      $row = $db->fetchArray($db->query("SELECT * FROM eventsType WHERE id='$id'"));
   ?>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post"  enctype="multipart/form-data">
 <input type="hidden" name="id" value="<?=$id?>">
 <input type="hidden" name="action" value="save_modif">
 <table align="center">
  <tr>
   <td><?=locale::display('title','kernel')?></td>
   <td><input type="text" size="20" maxlength="30" name="frm[titre]" value="<?=$row['titre']?>"></td>
  </tr>
  <tr>
   <td><?=locale::display('image','kernel')?></td>
   <td>
   <?php
   if (is_file('img_types/'.$row['image'])) {
      echo '<img src="../img_types/'.$row['image'].'" alt="'.$row['image'].'"/><a href="edit_type.php?affichage=frm_modif&action=del_picture&id='.$id.'">'.locale::display('erase','kernel').'</a>';
   } else {
      echo '<input type="file" name="frm_image" id="frm_image" size="20">';
   }
   ?>
   </td>
  </tr>
  <tr>
   <td colspan="2" align="right"><input type="submit" value="<?=locale::display('save','kernel')?>"></td>
  </tr>



 </table>
</form>
   <?php
   break;
}
?>

<?php
   $page->footer();
?>
