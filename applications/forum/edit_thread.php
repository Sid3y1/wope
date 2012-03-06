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


   //Le include doit deja contenir a fonction de connexion à Mysql
   //Obligatoire
   $relativePath = '../..';       //Chemin relatif mpour accéder à la racine de l'arborecense
   $SECU = "moderateur";       //Niveaux de sécurité (voir $session->authPage())
   include ($relativePath . '/include/main.inc');
   
	 //Initialisation du forum
	 $core->localLoadClass('afficheforumbasic');
	 $affiche_forum = new afficheforumBasic();

   $id_thread=$_GET['id_thread'];
	 $db_name = $_GET['db_name'];
   $thread = $affiche_forum->forum->getThreadData($id_thread);
   $id_salon = $thread['id_salon'];
   $salon_tmp = $affiche_forum->forum->getSalonData($id_salon);
	 $salon_name = $salon_tmp['nom'];
	 
	 $ARIANEWIRE = Array(
	 	'Accueil' => $core->makeUrl('index.php'),
		'Forum' => $core->makeUrl('applications/forum'),
		$salon_name => $core->makeUrl('applications/forum/thread.php').'?id_salon='.$id_salon.'&db_name='.$db_name ,
		$thread['nom_thread'] => $core->makeUrl('applications/forum/affiche_thread.php').'?id_thread='.$id_thread.'&db_name='.$db_name
	 );
	 
   //Propre à la page
   $page = new Page('Forum :','');
	 $page->header(locale::display('forum_edit_room','forum'), 'forum');

   //Démarrage des classes de gestion des fichiers statique
   //include (dirname(__FILE__)."/genStatic.class.inc");
   //$genStatic=new genStatic(dirname(__FILE__).'/static/');

   if (!$affiche_forum->forum->isModerateur($id_thread))
      {
      exit();
      }
      

	$page->moduleHeader($thread['nom_thread'],'');
	if ($affiche_forum->forum->modo)
  {
		$affiche_forum->displayDeleteThread($id_thread);
		echo '<div class="clear"></div>';

   if($affiche_forum->forum->modo && isset($_GET['action']) && $_GET['action']=='delete')
      {
       ?>
        <div align="center">
          <?=locale::display('want_delete_thread','forum')?> :
          <br />
          <i>
            <?=$thread['nom_thread']?>
          </i>
          <br />
          <br />
          <a href='edit_thread.php?id_thread=<?=$id_thread?>&action=do_delete&db_name=<?=$db_name?>'>
            <?=locale::display('yes','kernel')?>
          </a> |
          <a href='edit_thread.php?id_thread=<?=$id_thread?>&db_name=<?=$db_name?>'>
            <?=locale::display('no','kernel')?>
          </a>
        </div>
<?php
}
elseif ($affiche_forum->forum->modo && isset($_GET['action']) && $_GET['action']=='do_delete')
{
      $id_thread=(int)$id_thread;
      $query ="DELETE FROM forum_thread WHERE id='$id_thread'";
      $db->query($query);
      $query ="DELETE FROM forum_message WHERE id_thread='$id_thread'";
      $db->query($query);
      $query="DELETE FROM forum_login WHERE thread='$id_thread'";
      $db->query($query);
      $core->url->redirect("forum/thread.php?id_salon=$id_salon&db_name=$db_name");
}
else
{
?>
<form class="edit_form" action="thread.php?id_salon=<?=$id_salon?>" method="post" id='post_message'>
	<div>
		<input type='hidden' name='edit' value='true' />
		<input type='hidden' name='db_name' value="<?=$db_name?>" />
  	<input type='hidden' name='id_thread' value='<?=$id_thread?>' />
	</div>
  <p class="form-input">
		<label>Etat :</label>
    <select name='etat' style="font-size:8pt;">
			<option value='normal' <?=($thread['sticky']=='false' && $thread['locked']=='false' && $thread['drop']=='false')?"selected=\"selected\"":""?>>Normal</option>
			<option value='drop' <?=($thread['drop']=='true')?"selected=\"selected\"":""?>>Caché (invisible pour les utilisateurs hors équipe de modération)</option>
			<option value='sticky' <?=($thread['sticky']=='true' && $thread['locked']=='false')?"selected=\"selected\"":""?>>Post-it (toujours affiché en haut)</option>
      <option value='locked' <?=($thread['sticky']=='false' && $thread['locked']=='true')?"selected=\"selected\"":""?>>Vérouillé (plus de réponses possibles)</option>
      <option value='sticky-locked' <?=($thread['sticky']=='true' && $thread['locked']=='true')?"selected=\"selected\"":""?>>Annonce (Post-it vérouillé)</option>
    </select>
  </p>
	<p class="form-input">
		<label>Titre :</label>
    <input type="text" name="nom" size="75" class="forum_form" maxlength="70" value="<?=$thread['nom_thread']?>" />
	</p>
	<p class="form-input">
  	<label>Emplacement :</label>
    <select name="id_salon" style="font-size:8pt;">
<?php
      //on recupère les id du salon
      $tab_id_salon=array();
      $query="SELECT id,droit_lecture FROM forum_salon ORDER BY nom ASC";
      $result=$db->query($query);
      while ($row=$db->fetchArray($result))
      {
        $droit_lecture=$row["droit_lecture"];
        if ($droit_lecture == "all")
        {
          $tab_id_salon[]=$row['id'];
        }
				elseif ($core->verifDroits($droit_lecture))
        {
        	$tab_id_salon[]=$row['id'];
        }
      }
      foreach ($tab_id_salon as $id_salon)
      {
        $salon=$affiche_forum->forum->getSalonData($id_salon);
        echo '<option value="'.$salon['id'].'" ';
        if ($salon['id']==$thread['id_salon'])
        {
        	echo ' selected="selected" ';
        }
       	echo ">
        	".$salon['nom']."
        	</option>";
      }
?>
    </select>
	</p>
	<p class="form-submit">
    <input type="submit" value="Ok" name="edit" />
	</p>
</form>
<?php
  }
}
$page->moduleFooter();
$page->footer();
?>
