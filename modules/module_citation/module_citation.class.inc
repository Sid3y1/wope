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


class Module_citation extends Module {

/*
	*
	* Fonction afficheCitation
	* Affiche le HTML d'une citation
	*
	* @param $quote, $author
	* @return void
	*
	*/

	public function afficheCitation($quote, $author = '')
	{
		echo '<p class="quote"><q>'. $quote .'</q><cite title="Author"> - '. $author .'</cite><div class="clear"></div></p>';
	}


  public function display() {
		global $db, $core, $page;

		if($db->result($db->query("SELECT count(id) FROM citation WHERE date = now()"),0)==0)
		{
			//Met la dernière citation affichée en citation du jour
			$db->query("UPDATE citation SET date = now() ORDER BY date ASC LIMIT 1 ");
		}
		
		if($core->verifDroits('admin'))
		{
			//Affiche la prochaine citation en citation du jour
			$don = $db->fetchArray($db->query("SELECT texte,auteur FROM citation ORDER BY date ASC LIMIT 1"));
			$don['texte'] .= "[admin]";
		}
		else
		{
			$don = $db->fetchArray($db->query("SELECT texte,auteur FROM citation WHERE date = now()"));
		}
//		$quote = utf8_decode($don['texte']); Le texte est déjà en utf8 donc je vois pas pourquoi il faut faire ça [Rémi]
		$quote = $don['texte']; 

		$author = $don['auteur'];
		$this->afficheCitation($quote, $author);                
  }
}



//todo je sais plus mais faut todo qq chose
?>
