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


//******************************************************************************
//FORUM DISTANT

$that=array();
//labels provenant de affiche_thread.php
$that['forum_thread']='Forum : thread';
$that['deactivate_follow_mail']='Désactiver le suivi mail';
$that['activate_follow_mail']='Activer le suivi mail';
$that['deactivate_follow_site']='Désactiver le suivi site';
$that['activate_follow_site']='Activer le suivi site';
$that['thread_list']='Liste des threads';
$that['reply_quoting_msg']='Répondre de suite en citant ce message';
$that['quote_msg']='Citer ce message dans ma future réponse';
$that['abuse_msg']='Signaler un abus sur ce message';
$that['forum_abuse_manage']='Liste des abus signalés';
$that['msg_concerned_info']='Message concerné';
$that['no_thread']='Thread inexistant !';
$that['message']='Message';
$that['said']='a dit';
$that['censured_avatar']='Votre avatar est pour l\'instant censuré !';
$that['censured_picture']='Image censurée';
$that['sign_message']='Signer mon message';
$that['see_abuse_msg']='Voir les abus signalés';
$that['edit_forum']='Editer le salon';
$that['add_room']='Ajouter un salon';
$that['last_active_subjects']='Derniers sujets actifs';
$that['want_delete_thread']='Etes-vous sûr de vouloir effacer ce sujet ?';
$that['del_error']='Erreur lors de l\'effacement';
//attention, les textes ci dessous sont affichées dans les infos bulles, il faut donc faire gaffe au '
//mettre le texte entre double " et mettre un \' devant les '
$that['modo_will_check']="Merci, un moderateur va controler ce message au plus tôt";
$that['bold_selected_text']="Sélectionner un texte et cliquer sur ce bouton pour le metre en gras";
$that['italic_selected_text']="Sélectionner un texte et cliquer sur ce bouton pour le metre en italique";
$that['underline_selected_text']="Sélectionner un texte et cliquer sur ce bouton pour souligner le texte";
$that['quote_selected_text']="Sélectionner un texte et cliquer sur ce bouton pour metre le texte en mode &lt; citer &gt;";
$that['link_selected_text']="Sélectionner un texte et cliquer sur ce bouton pour le metre en &lt; lien &gt;";
$that['mail_selected_text']="Sélectionner un texte et cliquer sur ce bouton pour le metre en &lt; e-mail &gt;";
$that['smiley_list']="Cliquer pour acceder a la liste des smileys";
$that['preview_selected_text']="Cliquer ici pour voir un apercu de votre message !";
$that['bold_text']="texte en gras";
$that['italic_text']="texte en italique";
$that['underline_text']="texte souligné";
$that['quote_text']="texte cité";
$that['link_text']="lien";
$that['link_url']="adresse";
$that['mail_text']="lien";
$that['mail_address']="adresse mail";
$that['quote_author']="auteur";

//labels provenant de all_abus.php
$that['abuse_manage']='Forum : Gestion des abus et des censures';
$that['warning_info']='Infos avertissement';
$that['info_concerned_msg']='Infos message visé';
$that['bound_msg']='Lien message';
$that['seen']='Vu ?';
$that['no_abuse']='Aucun abus signalé non lu.';
$that['see_abuse_not_filed']='Voir les abus non archivés';
$that['see_filed_abuse']='Voir les archives';

//labels provenant de edit_message.php
$that['forum_edit_msg']='Forum : edition d\'un message';
$that['censure']='Censurer';
$that['blocked']='Bloqué';
$that['authorized']='Autorisé';
$that['no_avatar']='Pas d\'avatar pour ce message ou avatar indisponible !';
$that['author']='Auteur';
$that['edit_coment']='Commentaire sur l\'édition';
$that['edit_hist']='Historique des éditions';
$that['no_change']='Aucune modification enregistrée';
$that['cancel']='Annuler';
$that['preview']='Aperçu';
$that['save_draft']='Sauver le brouillon';
$that['post']='Valider';

//labels provenant de edit_salon.php
$that['forum_edit_room']='Forum : Edition d\'un salon';
$that['right_read']='Droits pour lecture';
$that['right_write']='Droits pour écriture';
$that['right_attach']='Droits pour attachement';
$that['right_mod']='Droits pour modération';


//labels provenant de edit_thread.php
$that['delete_thread']='Effacer ce thread';

//labels provenant de modo.php
$that['announce_title'] = "Signalement d'abus";
$that['your_comment'] = "Votre commentaire";
$that['announce_modo']='Vous allez signaler à un moderateur un abus sur le message de ';
$that['announce_modo_2']=', veuillez commenter votre action.';
$that['forum_new_thread']='Forum : nouveau thread';

//labels provenant de recherche.php
$that['forum_seek_msg']='Forum : recherche d\'un message"';
$that['result_seek']='Il y a <strong>[1]</strong> résultat(s) à votre recherche.';
$that['result_from']='Résultats de [1] à [2]';
$that['seek_simple']='Recherche Simple';
$that['seek_advanced']='Recherche avancée';
$that['show']='Salon';
$that['thread']='Thread';

//labels provenant de thread.php
$that['thread_not_readable']='Ce salon ne vous est pas accessible';

//labels provenant de salon.php
$that['forum_not_seen']='Forum non visité';
$that['new_msg']='Nouveau(x) message(s) depuis la dernière visite';
$that['no_msg']='Pas de nouveau message';

//labels provenant de stats.php
$that['forum_stats']='Forum : stats';

//labels provenant de thread.php
$that['forum_msg']='Forum : messages';

$label['forum'] = $that;
?>
