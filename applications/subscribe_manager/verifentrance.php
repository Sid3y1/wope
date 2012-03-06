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



   //Le include doit deja contenir la fonction de connexion a Mysql
   $idPage = 133;
   //Obligatoire
   $relativePath = '../..';       //Chemin relatif mpour acceder a� la racine de l'arborecense
   //$niveauSecu = array('etu','ext');       //Niveaux de securite (voir $session->authPage())
   $niveauSecu='all';
   include ($relativePath . '/include/main.inc');
   //Propre a la page
   $page = new Page();
   $page->header("SubscribeManager");
			echo '<script type="text/javascript" src="suggest.js"></script>';
$id_event = $_GET['id_event'];
if($db->result($db->query("SELECT login_admin FROM subscribeManager_event WHERE id_event='".$id_event."'"),0)!=$core->getLogin()){
 echo 'Vous n\'etes pas l\'admin !!!';
 exit();
}
?>
<style>
#rcmKSearchpane
{
  background-color: #F9F9F9;
  border: 1px solid #CCCCCC;
}

#rcmKSearchpane ul
{
  margin: 0px;
  padding: 2px;
  list-style-image: none;
  list-style-type: none;
}

#rcmKSearchpane ul li
{
  height: 16px;
  font-size: 11px;
  padding-left: 8px;
  padding-top: 2px;
  padding-right: 8px;
  white-space: nowrap;  
}

#rcmKSearchpane ul li span
{
 cursor:pointer;
  height: 16px;
  font-size: 11px;
  padding-left: 8px;
  padding-top: 2px;
  padding-right: 8px;
  white-space: nowrap;  
}

#rcmKSearchpane ul li.selected
{
  color: #ffffff;
  background-color: #CC3333;
}
</style>

<script type="text/javascript" src="suggest2.js"></script>
<?php
/*
$res = $db->query("SELECT exterieur FROM usersInfo WHERE exterieur != ''");

$logins = $db->fetchArray($res);
$logins = "'".$logins[0]."'";
while($don = $db->fetchArray($res)){
$logins .= ",'".str_replace("","",str_replace("'","",$don[0]))."'";
}
*/
?>
<script type="text/javascript" src="../include/xmlhttp/ajax2.js"></script>
<div style="float:left">
<form action="addothers.php" method="post" onsubmit="return false;">
<p><label for="codebarre" >Code:</label><input onkeydown="check();" onfocus="check()" onchange="check();" type="text" size="20" name="codebarre" id="codebarre" style="border:solid 2px blue;"/></p>
</form>
<span id="deb"></span></div>
<div id="who" style="float:right"></div>

<script>
var checked = 0;
function check(){

 who = document.getElementById("who");
	debug = document.getElementById('deb');
 code = document.getElementById('codebarre').value;
	if(code!=checked){
  if(code==''){
/*   who.innerHTML = "VIDE";*/
  }else{
	  if(code.length=='14'){
	 	 checked = code;
    who.innerHTML = "Page en chargement !";
    ajaxId('ajaxwho.php?verif&id_event=<?=$_GET['id_event']?>&code='+code,'who');
    document.getElementById('codebarre').value = "";
				document.getElementById('codebarre').focus();

setTimeout("suggestInit([<?=$logins?>],'coresLogin',' ');",1000);
setTimeout("document.getElementById('coresLogin').focus();",1000);

   }else{
			checked = 0;
    document.getElementById('who').innerHTML = "En attente de Code valide !";
			}
	 }
	}
	setTimeout("check();",100);
}
setTimeout("check();",10);
</script>
