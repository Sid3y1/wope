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
  $page->header();

	$core->loadClass('form');
	$class_form = new Form();
       
    $core->loadClass('mail') ;
    $mail = new Mail();
    
  $erreur = Array();
  if (isset($_POST['action'])) {
  	
  	 $form=$_POST['form'];
  
     $dataOk = true;

        	
     //On regarde si le login est conforme
     if (!$class_form->verifForm($form['login'], 'login')) {
        $erreur[] = "Ce pseudo n'est pas valide : seuls les lettres et les chiffres sont autorisés (de 4 à 8 caractères).";
        $dataOk = false;
     }

     //On regarde si le login est déjà pris
     $result = $db->query("SELECT ui.login, l.login FROM usersInfo ui, login l WHERE ui.login='".$form['login']."' OR l.login='".$form['login']."'");
     if ($db->result ($result, 0) > 0 && $form['login']!='defaut') {
        $erreur[] = "Cet identifiant existe déjà, veuillez en choisir un autre.";
        $dataOk = false;
     }

     //On vérifie le mot de passe (non nul et confirmé)
     if ($form['pass'] == '') {
        $erreur[] = "Vous n'avez pas saisi de mot de passe.";
        $dataOk = false;
     }
     if ($form['pass'] != $form['passconf']) {
        $erreur[] = "Les mots de passe saisis diffèrent.";
        $dataOk = false;
     }


     //On vérifié l'email
     if (!$class_form->verifForm($form['email'], 'email')) {
        $erreur[] = "Votre email n'est pas valide";
        $dataOk = false;
     }




     if ($dataOk) {
        $code = substr(md5($form['login']),2,8);


        //On crée le compte

$msg_text = "Votre compte vient d'être créé. votre code d'activation est : ".$code."\n\r\n\r
Pour rappel, les informations que vous avez saisies sont :\n\r
Identifiant (Login): ".$form['login']."\n\r
Mot de passe : ".$form['pass']."\n\r
Email : ".$form['email']."\n\r
\n\r
Vous pouvez activer votre compte en vous rendant ici :\n\r
".$core->getConfig('baseUrl')."/user/activation.php?login=".$form['login']."&code=".$code." \n\r";
    
$mail->send($core->getConfig('mailWebmaster'), $form['email'], $msg_text, 'Création de compte') ;


//envois d'un mail à l'admin

$msg_text = "Un nouvel utilisateur vient de se créer un compte sur le site ".$core->getConfig('site_name')." ( ".$core->getConfig('baseUrl')." )\n\r
Identifiant (Login): ".$form['login']."\n\r
Email : ".$form['email']."\n\r
\n\r
Il serait bon de lui donner les droits appropriés.\n\r
";

$mail->send($core->getConfig('mailWebmaster'), $core->getConfig('mailWebmaster'), $msg_text, 'Création de compte') ;


        $db->query("INSERT INTO usersInfo (login,prenom,nom,pass,grp, email, dateCreation,userType) VALUES ('".$form['login']."','".$form['prenom']."','".$form['nom']."',PASSWORD('".$form['pass']."'),'ext','".$form['email']."', NOW(),'In')");


        $toAffiche = 'confirm';
     } else {
        $toAffiche = 'form';
     }

  } else {
     $form = Array('login'=>'','pass'=>'','passconf'=>'','email'=>'','nom'=>'','prenom'=>'');
     $toAffiche = 'form';
  }


  switch ($toAffiche) {
  	case 'form':

  		if (count($erreur) > 0) {
    	 $rapport_err = 'Erreur'.((count($erreur)>1)?"s":"").' dans votre saisie !<ul>';
     		foreach($erreur as $err) {
        	$rapport_err .= '<li style="padding-left:30px;">' . $err . '</li>';
     		}
     		$rapport_err .= '</ul>';
     		$page->msgError($rapport_err);
  		}
			
			$page->msgInfo('En créant votre compte, vous allez recevoir un code d\'activation par mail.<br/>'.
	      'Vous ne pourrez vous identifier qu\'une fois ce code saisi sur le site.');
			$page->moduleHeader('Inscription','');
  ?>
    <form method="post" action="<?=$core->makeUrl('user/new_user.php')?>">
    <div><input type="hidden" name="action" value="saveAccount" /></div>
    <p class="form-input"><label>Prénom :</label>
 	  <input type="text" name="form[prenom]" size="50" maxlength="60" value="<?=stripslashes($form['prenom'])?>" /></p>
    <p class="form-input"><label>Nom :</label>
    <input type="text" name="form[nom]" size="50" maxlength="60" value="<?=stripslashes($form['nom'])?>" /></p>
	  <p class="form-input"><label>Identifiant (login) * :</label>
    <input type="text" name="form[login]" size="50" maxlength="50" value="<?=stripslashes($form['login'])?>" /></p>
	  <p class="form-input"><label>Mot de passe *</label>
    <input type="password" name="form[pass]" size="20" maxlength="20" /></p>
	  <p class="form-input"><label>Confirmation du mot de passe * :</label>
	  <input type="password" name="form[passconf]" size="20" maxlength="20" /></p>
	  <p class="form-input"><label>Email * :</label>
    <input type="text" name="form[email]" size="50" maxlength="60" value="<?=stripslashes($form['email'])?>" /></p>
    <p><em>* Champs obligatoires</em></p>
    <p class="form-submit"><input type="submit" value="Créer mon compte" /></p>
    </form>
     <p>La création d'un compte utilisateur sur ce site vous permettra d'accéder à des rubriques supplémentaires. 
    Afin d'obtenir davantage de droits, veuillez contacter <a href="mailto:<?=$core->getConfig('mailWebmaster')?>">l'administrateur du site</a></p>

<?php
			$page->moduleFooter();
     break;
     case 'confirm':
			$page->moduleHeader('Inscription','');
?>
     <p>Vous venez de créer votre compte sur notre portail.</p>
     <p>Votre compte n'est pas encore activé, vous allez recevoir votre code d'activation par mail.</p>
     <a href="<?=$core->makeUrl('user/activation.php')?>">Saisir le code de confirmation</a>
<?php
			$page->moduleFooter();
     break;
  }
  $page->footer();
?>


