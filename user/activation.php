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


  //Obligatoire
  $SECU = 'all';       //Niveaux de sécurité (voir $session->authPage())

  include ('../include/main.inc');

  //Propre à la page
  $core->loadClass('user') ;

  $page = new Page();
  $page->header('','index');


  $erreur = array();
  if (isset($HTTP_POST_VARS['action'])) {
     $authOk = true;
     $form = $_POST['form'];
     if ($form['code'] != substr(md5($form['login']),2,8)) {
        $authOk = false;
        $erreur[] = "Code d'activation incorrect ";
     }
     $result = $db->query("SELECT COUNT(*) AS nb FROM usersInfo WHERE login='".$form['login']."' AND pass=PASSWORD('".$form['pass']."')");
     if ($db->result($result, 'nb') != 1) {
        $authOk = false;
        $erreur[] = "Mot de passe incorrect";
     }
     if ($authOk) {
        $db->query("INSERT INTO login (login, type) VALUES ('$form[login]', 'ext')");

        $db->query("UPDATE usersInfo SET actif='Y' WHERE login='".$form['login']."'");
        $toAffiche = 'confirm';
     } else {
        $toAffiche = 'form';
     }

  } else {
     $form = Array('login'=>'','code'=>'');
     if (isset($HTTP_GET_VARS['login'])) {
        $form['login'] = $_GET['login'];
        $form['code'] = $_GET['code'];
     }

     $toAffiche = 'form';
  }


  switch ($toAffiche) {
  case 'form':
  if (count($erreur) > 0) {
     $text = 'Erreurs dans votre saisie !<ul>';
     foreach($erreur as $err) {
        $text .=  '<li>' . $err . '</li>';
     }
     $text .=  '</ul>';

     $page->msgError($text);
  }
	
$page->moduleHeader('Activation de votre compte','');
  ?>
    <form method="post" action="<?=$core->makeUrl('user/activation.php')?>">
     <input type="hidden" name="action" value="saveAccount">
       <p class="form-input"><label>Identifiant :</label>
       <input type="text" name="form[login]" value="<?=$form['login']?>" /></p>
       <p  class="form-input"><label>Mot de passe :</label>
       <input type="password" name="form[pass]" /></p>
       <p class="form-input"><label>Code d'activation</label>
       <input type="text" name="form[code]" value="<?=$form['code']?>" /></p>
       <p class="form-submit"><input type="submit" name="form[submit]" value="Activer mon compte"></p>
    </form>

<?php
$page->moduleFooter();
     break;
    case 'confirm':
		 	echo '<div class="column0">';
			$page->displayModuleName('module_identification');
			echo '</div>';
	
		 	echo '<div class="column1">';
      $page->msgInfo('Votre compte est à présent activé.<br/>Vous pouvez maintenant accéder au site.<br/>'.
					'Afin d\'obtenir davantage de droits, veuillez contacter <a href="mailto:'.$core->getConfig('mailResponsable').'">l\'administrateur du site</a>.');
      $nextPage='/index.php';

			echo '</div>';

	echo '
	<script type="text/javascript">
		document.formAuth.elements["user"].value="'.$form['login'].'";
		document.formAuth.elements["pass"].value="'.$form['pass'].'";
		document.formAuth.elements["redirect"].value="'.$core->makeUrl('').'";
	</script>
	';
     break;
  }

  $page->footer();
?>
