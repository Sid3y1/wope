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



$SECU = 'all';

include ('../include/main.inc');

$core->loadClass('mail');
	
$page = new Page();
$page->header('Mot de passe égaré', '');

$action = isset($_POST['ga']) ? $_POST['ga'] : ( isset($_GET['ga']) ? $_GET['ga'] : '' );

switch( $action ){

	case 'cp':
    $login = stripslashes($_POST['user_login']);
		$code = $_POST['user_code'];
		$p = $_POST['p'];

		$lostPassResult = $db->query("SELECT login, code FROM user_lost_pass WHERE id='".$db->escapeString($p)."' ");
		
		$procedureExists = ( $db->numRows($lostPassResult) == 1 ) ;

		if( $procedureExists ){
			$row = $db->fetchArray($lostPassResult);
			$infosOk = ( $login == stripslashes($row['login']) && $code == $row['code'] );
		}
		
		if( $procedureExists && $infosOk ){
		
	    $result = $db->query("SELECT ui.login, tt.email, tt.email2 FROM usersInfo ui INNER JOIN trombi_trombi tt ON ui.id = tt.id_user WHERE ui.login='".$db->escapeString($login)."' LIMIT 1");

			if( $db->numRows($result) == 1 ){
		 
		    $row = $db->fetchArray($result);

		    if( isset($row['login']) && $row['login'] != '' ){
			
					if( stripslashes($row['email']) == stripslashes($row['login']).'@'.$core->getConfig('local') ){
						$to = stripslashes($row['email2']);
					}else{
				 		$to = stripslashes($row['email']);
					}
		 
			 		$form = new Form();
				
				 	if( $to != '' && $form->verifForm($to, 'email') ){
	
						$mdp = substr(md5($to . date("YmdHis")),8,5);
					
	   	  	  if ( $db->query("UPDATE usersInfo SET pass=PASSWORD('" . $mdp . "') WHERE login='" . $db->escapeString($login) . "'") ){
							$db->query("DELETE FROM user_lost_pass WHERE id = '".$db->escapeString($p)."' ");
					
							//exec('sh '.$core->getConfig('baseServer').'/include/scripts/change_mail_password.sh '.$login.' '.$core->getConfig('local').' '.$mdp);
					
							$mail = new Mail ();
					
							$msg = '	Bonjour,
'.
					    '	Voici votre nouveau mot de passe pour accéder à '.$core->getConfig('site_name').': '.$mdp.'
'.
					    '	Vous pouvez dès à présent vous logger ici : '.$core->getConfig('baseUrl').'
'.
							'	Rappel de votre identifiant : '.$login.'

'.
							'	Cordialement,
'.
			  		  '	l\'équipe de '.$core->getConfig('site_name');
						
  	  	  	  if( $mail->send($core->getConfig('mailWebmaster'), $to, $msg, 'Votre nouveau mot de passe') ){
								$page->msgInfo('Votre nouveau mot de passe vous a été envoyé à cette adresse : '.$to.'.');
							}else{
									$page->msgError('L\'envoi du mail de confirmation a échoué. Veuillez contacter l\'administrateur à l\'adresse suivante : <a href="mailto:'.$core->getConfig('mailWebmaster').'">'.$core->getConfig('mailWebmaster').'</a>');
							}

						}else{
							$page->msgError('Une erreur est survenue durant la procédure de récupération. Veuillez contacter l\'administrateur à l\'adresse suivante : <a href="mailto:'.$core->getConfig('mailWebmaster').'">'.$core->getConfig('mailWebmaster').'</a>');
						}
					
 					}else{
						$page->msgError('Vous n\'aviez pas renseigné d\'email valide dans votre profil. Veuillez contacter l\'administrateur à l\'adresse suivante : <a href="mailto:'.$core->getConfig('mailWebmaster').'">'.$core->getConfig('mailWebmaster').'</a>'); 
					}
				
 			  }else{
     	  	$page->msgError('Ce login n\'existe pas.');
    		}
		 
			}
			
		}else{
			$page->msgWarning('Il semble qu\'aucune procédure de récupération de mot de passe n\'ait été démarrée pour ce login. Si vous avez perdu votre mot de passe, veuillez commencer la procédure en cliquant <a href="'.$core->makeUrl('user/recover_pass.php').'">ici</a>.');
		}
  	break;

	case 'ap':
    $login = stripslashes($_POST['user_login']);
    $result = $db->query("SELECT ui.login, tt.email, tt.email2 FROM usersInfo ui INNER JOIN trombi_trombi tt ON ui.id = tt.id_user WHERE ui.login='".$db->escapeString($login)."' LIMIT 1");

		if( $db->numRows($result) == 1 ){
		 
	    $row = $db->fetchArray($result);

	    if( isset($row['login']) && $row['login'] != '' ){
			
				if( stripslashes($row['email']) == stripslashes($row['login']).'@'.$core->getConfig('local') ){
					$to = stripslashes($row['email2']);
				}else{
			 		$to = stripslashes($row['email']);
				}
		 
		 		$form = new Form();
				
			 	if( $to != '' && $form->verifForm($to, 'email') ){

					$code = rand(100000, 999999);

					if( $db->query("INSERT INTO user_lost_pass (login, code) VALUES ('".$db->escapeString($login)."', '".$db->escapeString($code)."') ") ){
						$p = $db->insertId();
					
						$mail = new Mail ();
					
						$msg = '	Bonjour,

'.
				    '	Une demande de changement de mot de passe a été enregistrée pour votre compte.
'.
				    '	Si vous n\'êtes pas à l\'origine de cette demande ou si vous avez retrouvé votre mot de passe, cliquez sur le lien ci-dessous pour annuler la procédure :
'.
						$core->getConfig('baseUrl').'/user/recover_pass.php?ga=cl&p='.$p.'

'.
						'	Sinon, veuillez confirmer votre demande à cette adresse : 
'.
						$core->getConfig('baseUrl').'/user/recover_pass.php?ga=cf&p='.$p.'

'.
						'	en remplissant le formulaire avec les informations suivantes :
'.
						'		Identifiant : '.$login.'
'.
						'		Code de confirmation : '.$code.'

'.
						'	Cordialement,
'.
			  	  '	l\'équipe de '.$core->getConfig('site_name');

  	    	  if( $mail->send($core->getConfig('mailWebmaster'), $to, $msg, 'Demande d\'un nouveau mot de passe : confirmation') ){
							$page->msgInfo('Une demande de confirmation vous a été envoyée à cette adresse : '.$to.'.');
						}else{
							$page->msgError('L\'envoi du mail de confirmation a échoué. Veuillez contacter l\'administrateur à l\'adresse suivante : <a href="mailto:'.$core->getConfig('mailWebmaster').'">'.$core->getConfig('mailWebmaster').'</a>');
						}

					}else{
						$page->msgError('Une erreur est survenue durant la procédure de récupération. Veuillez contacter l\'administrateur à l\'adresse suivante : <a href="mailto:'.$core->getConfig('mailWebmaster').'">'.$core->getConfig('mailWebmaster').'</a>');
					}
					
 				}else{
					$page->msgError('Vous n\'aviez pas renseigné d\'email valide dans votre profil. Veuillez contacter l\'administrateur à l\'adresse suivante : <a href="mailto:'.$core->getConfig('mailWebmaster').'">'.$core->getConfig('mailWebmaster').'</a>'); 
				}
				
 		  }else{
        $page->msgError('Ce login n\'existe pas.');
    	}
		 
		}

		break;

	case 'clp':
		if( $db->query("DELETE FROM user_lost_pass WHERE id= '".$db->escapeString($_POST['p'])."' AND login='".$db->escapeString($_POST['user_login'])."' AND code='".$db->escapeString($_POST['user_code'])."' ") ){
			$page->msgInfo('La procédure de création de mot de passe a été annulée');
		}else{
			//$page->msgError('Une erreur est survenue durant la procédure d\'annulation.');
		}
		break;

	case 'cl':
		$page->moduleHeader('Annulation','');
?>

	<p>Veuillez entrer votre identifiant de connexion et le code que vous avez reçu par mail.</p>
	
	<form action="recover_pass.php" method="post">
  	<div class="input-hidden">
			<input type="hidden" name="ga" value="clp" />
			<input type="hidden" name="p" value="<?=$_GET['p']?>" />
		</div>
  	<p class="form-input">
			<label>Identifiant :</label>
  		<input type="text" name="user_login" />
		</p>
  	<p class="form-input">
			<label>Code reçu :</label>
  		<input type="password" name="user_code" />
		</p>
  	<p class="form-submit">
			<input type="submit" value="Soumettre" />
		</p>
 	</form>
 
<?php
		$page->moduleFooter();
		break;
		
	case 'cf':
		$page->moduleHeader('Confirmation','');
?>

	<p>Veuillez entrer votre identifiant de connexion et le code que vous avez reçu par mail.</p>
	
	<form action="recover_pass.php" method="post">
  	<div class="input-hidden">
			<input type="hidden" name="ga" value="cp" />
			<input type="hidden" name="p" value="<?=$_GET['p']?>" />
		</div>
  	<p class="form-input">
			<label>Identifiant :</label>
  		<input type="text" name="user_login" />
		</p>
  	<p class="form-input">
			<label>Code reçu :</label>
  		<input type="password" name="user_code" />
		</p>
  	<p class="form-submit">
			<input type="submit" value="Soumettre" />
		</p>
 	</form>
 
<?php
		$page->moduleFooter();
		break;
		
	default:
		$page->moduleHeader('Demande d\'un nouveau mot de passe','');
?>

	<p>Veuillez fournir votre identifiant de connexion. Un demande de confirmation sera envoyée par mail.</p>
	
	<form action="recover_pass.php" method="post">
  	<div class="input-hidden">
			<input type="hidden" name="ga" value="ap" />
		</div>
  	<p class="form-input">
			<label>Identifiant :</label>
  		<input type="text" name="user_login" />
		</p>
  	<p class="form-submit">
			<input type="submit" value="Soumettre" />
		</p>
 	</form>
 
<?php
		$page->moduleFooter();
		break;	
}
	
$page->footer();
?>
