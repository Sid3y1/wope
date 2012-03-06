<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>RoundCube|Mail :: Composer un nouveau message</title>
<link rel="stylesheet" type="text/css" href="skins/default/common.css" />
<link rel="stylesheet" type="text/css" href="skins/default/mail.css" />
<script type="text/javascript">
<!--

function rcmail_toggle_display(id)
{
	var row, disp;
	if (row = document.getElementById(id))
	{
		disp = (!row.style.display || row.style.display=='none') ? (document.all ? 'block' : 'table-row') : 'none';
		row.style.display = disp;
	}

	return false;
}

//-->
</script>
<script type="text/javascript" src="common.js"></script>
<script type="text/javascript" src="suggest2.js"></script>
<script type="text/javascript">
<!--

var rcmail = new rcube_webmail();
rcmail.set_env('contacts', ['toto','prout','coucou','super','genial']);
rcmail.set_env('task', 'mail');
rcmail.set_env('action', 'compose');
rcmail.set_env('mailbox', 'INBOX');
rcmail.set_env('trash_mailbox', 'Mail-conf/Trash');
rcmail.register_button('list', 'rcmbtn100', 'link', '', '', '');
rcmail.register_button('addressbook', 'rcmbtn101', 'link', '', '', '');
rcmail.register_button('settings', 'rcmbtn102', 'link', '', '', '');
rcmail.register_button('logout', 'rcmbtn103', 'link', '', '', '');
rcmail.register_button('list', 'rcmbtn104', 'image', '', '', '');
rcmail.gui_object('message', 'message');
rcmail.register_button('list', 'rcmbtn105', 'image', '', '', '');
rcmail.register_button('send', 'rcmbtn106', 'image', 'skins/default/images/buttons/send_act.png', '', '');
rcmail.register_button('contacts', 'rcmbtn107', 'image', 'skins/default/images/buttons/contacts_act.png', '', '');
rcmail.register_button('add-attachment', 'rcmbtn108', 'image', 'skins/default/images/buttons/attach_act.png', '', '');
rcmail.gui_object('messageform', 'form');
rcmail.register_button('send', 'rcmbtn109', 'image', 'skins/default/images/buttons/send_act.png', '', '');
rcmail.gui_object('attachmentlist', 'rcmAttachmentList');
rcmail.register_button('add-attachment', 'rcmbtn110', 'image', 'skins/default/images/buttons/add_act.png', '', '');
rcmail.gui_object('uploadbox', 'attachment-form');

//-->
</script>

</head>

<body>

<div id="taskbar">
<a href="#" onclick="return rcmail.command('list','',this)" id="rcmbtn100" class="button-mail">e-Mail</a>
<a href="#" onclick="return rcmail.command('addressbook','',this)" id="rcmbtn101" class="button-addressbook">Carnet d'adresses</a>
<a href="#" onclick="return rcmail.command('settings','',this)" id="rcmbtn102" class="button-settings">Pr&Atilde;&copy;f&Atilde;&copy;rences</a>
<a href="#" onclick="return rcmail.command('logout','',this)" id="rcmbtn103" class="button-logout" style="display:none">Quitter</a>
</div>

<div id="header" style="display:none">
<a href="#" onclick="return rcmail.command('list','',this)"><img src="skins/default/images/roundcube_logo.png" id="rcmbtn104" width="165" height="55" border="0" alt="[RoundCube Webmail]" /></a></div>

<div id="message"></div>


<form name="form" method="post">

<div id="messagetoolbar">
<a href="#" onclick="return rcmail.command('list','',this)" title="Retourner &Atilde;&nbsp; la liste des messages"><img src="skins/default/images/buttons/back_act.png" id="rcmbtn105" width="32" height="32" border="0" alt="" /></a>
<a href="#" onclick="return rcmail.command('send','',this)" title="Envoyer le message maintenant"><img src="skins/default/images/buttons/send_pas.png" id="rcmbtn106" width="32" height="32" border="0" alt="" /></a>
<a href="#" onclick="return rcmail.command('contacts','',this)" title="Carnet d'adresses"><img src="skins/default/images/buttons/contacts_pas.png" id="rcmbtn107" width="32" height="32" border="0" alt="" /></a>
<a href="#" onclick="return rcmail.command('add-attachment','',this)" title="Joindre un fichier"><img src="skins/default/images/buttons/attach_pas.png" id="rcmbtn108" width="32" height="32" border="0" alt="" /></a>

<div id="priority-selector">
Priorit&Atilde;&copy;:&nbsp;
<input type="hidden" name="_auth" value="59532569007740635ca1439d0de636316f634dbc" />

<input name="_task" value="mail" type="hidden" />
<input name="_action" value="send" type="hidden" />

<select name="_priority">
<option value="5">Tr&Atilde;&uml;s basse</option>
<option value="4">Basse</option>
<option value="0" selected>Normale</option>
<option value="2">Elev&Atilde;&copy;e</option>
<option value="1">Tr&Atilde;&uml;s &Atilde;&copy;lev&Atilde;&copy;e</option>

</select>
</div>

</div>

<div id="compose-container">
<table border="0" cellspacing="0" cellpadding="1" style="width:100%; height:100%;" summary=""><tr>
<td>

<table border="0" cellspacing="0" cellpadding="1" id="compose-headers" summary=""><tr>

<td class="title">De</td>
<td><select name="_from">
<option value="2">CHATEAU Mathieu &lt;mathieu.chateau@utt.fr&gt;</option>

<option value="1">chateaum &lt;chateaum@utt.fr&gt;</option>
</select></td>

</tr><tr>

<td class="title top">A</td>
<td><textarea name="_to" cols="80" rows="2"></textarea></td>
<td class="add-button"><a href="#" onclick="return rcmail_toggle_display('compose-cc')">[Cc]</a><br />
<a href="#" onclick="return rcmail_toggle_display('compose-bcc')">[Bcc]</a><br /></td>

</tr><tr id="compose-cc">

<td class="title top">Cc</td>
<td><textarea name="_cc" cols="80" rows="2"></textarea></td>

</tr><tr id="compose-bcc">

<td class="title top">Cci</td>
<td><textarea name="_bcc" cols="80" rows="2"></textarea></td>

</tr><tr id="compose-replyto">

<td class="title top">R&Atilde;&copy;pondre &Atilde;&nbsp;</td>
<td><input name="_replyto" size="80" type="text" /></td>

<!-- </tr><tr>

<td><input type="file" name="_attachments[]" size="20" /></td> -->

</tr><tr>

<td class="title">Sujet</td>
<td><input name="_subject" id="compose-subject" value="" type="text" /></td>

</tr></table>

</td>

</tr><tr>

<td style="width:100%; height:100%;">
<textarea name="_message" id="compose-body" cols="80" rows="20" warp="virtual"></textarea>
<div style="float:right">

<h2>Envoyer :<a href="#" onclick="return rcmail.command('send','',this)" title="Envoyer le message maintenant"><img src="skins/default/images/buttons/send_pas.png" id="rcmbtn109" width="32" height="32" border="0" alt="" /></a>
<h2></div>
</td>

</tr></table>

</div>

<div id="compose-attachments">
<div>Attachments</div>
<ul id="rcmAttachmentList">
</ul>
<p><a href="#" onclick="return rcmail.command('add-attachment','',this)" title="Joindre un fichier"><img src="skins/default/images/buttons/add_pas.png" id="rcmbtn110" width="23" height="18" border="0" alt="" /></a>
</div>

</form>

<div id="attachment-form">
<form action="./" method="post" enctype="multipart/form-data">
<input type="hidden" name="_auth" value="59532569007740635ca1439d0de636316f634dbc" />
<input type="file" name="_attachments[]" /><br />
<input type="button" value="Fermer" class="button" onclick="document.getElementById('attachment-form').style.visibility='hidden'" />
<input type="button" value="Joindre" class="button" onclick="rcmail.command('send-attachment', this.form)" />
</form>
</div>



<!--
<div id="attachment-form">
<input type="file" name="_attachments[]" size="20" /><br />
<input type="button" value="" onclick="rcmail_show_attachment_form(0)" />
<input type="button" value="" onclick="rcmail.command('send-attachment', this)" />
</div>
-->


</body>

</html>
