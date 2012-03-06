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


class Module_trombi extends Module {
	
	public final function __construct($informations = Array()) {
		parent::__construct($informations);
	}
												
  public function display()
  {
    global $db, $core, $themecolor;

		$core->loadClass('FileManager');
		$fm = new FileManager();		
		$core->loadClass('Trombi');
		$trombi = new Trombi();
            
?>
		<form action="<?=$core->makeUrl('applications/trombi/index.php') ?>" method="POST">
	  	<div class="autocomplete_container" id="trombi_autocomplete"></div> 	
			<script type="text/javascript"> 
				var phpFile = "./trombi/ajax/fastsearch.php";
				var schema = ["results", "value","data"]; 
				var trombi_datasource = new YAHOO.widget.DS_XHR(phpFile, schema); 
				var trombiAutoComp = new YAHOO.widget.AutoComplete("fastsearch","trombi_autocomplete", trombi_datasource); 
				trombiAutoComp.minQueryLength = 3;
				trombiAutoComp.animVert = false;
				trombiAutoComp.animHoriz = false;
				YAHOO.widget.AutoComplete.prototype.formatResult = function(aResultItem, sQuery) { 
				    var sResult = aResultItem[0]; 
				    if(sResult) { 
				        return sResult+aResultItem[1]; 
				    } 
				    else { 
				        return ""; 
				    } 
				}; 
			</script>
			
			<div class="left_column">
					<p class="form-input">
							<input type="text" size="15" id="fastsearch" name="fastsearch" value="">
					  	<input type="submit" value="<?=locale::display('seek','kernel')?>">
					</p>

		      <a class="link_button" href="<?=$core->makeUrl('user/index.php') ?>?part=info">Modifier ma fiche</a>
<?php
/*
		        	<li><a class="link_menu" href="<?=$core->makeUrl('applications/trombi/notifications.php') ?>">Notifications d'anniversaire</a></li>
		        	<li><a class="link_menu" href="<?=$core->makeUrl('applications/trombi/anniversaires.php') ?>">Tous les anniversaires</a></li>  
*/
?>
		    	
		    	<ul class="panel">
	    <?php
    	
    	/*$fete = $db->result($db->query("SELECT fete FROM fetes WHERE mois=".date("m")." and jour=".date("d")." "),0);
			echo '<div class="colonne_droite_module_trombi"><ul><li class="fete_du_jour">Fête(s) du jour :';
			if($db->numRows($result)==0){
					echo 'Pas de fêtes aujourd\'hui';
    	}
			echo '<a href="'.$core->makeUrl('applications/trombi/index.php').'?type=simple&action=search&toSearch='.$fete.'">'.$fete.'</a></li>';
			*/
    	$result = $db->query("SELECT login, lastname, firstname, YEAR(CURDATE())-YEAR(birthdate) AS age  FROM trombi_trombi, usersInfo WHERE trombi_trombi.id_user=usersInfo.id and month(birthdate) = ".date("m")." and dayofmonth(birthdate) = ".date("d")." ORDER BY lastname ASC");
			switch($db->numRows($result)){
				case 0:
					echo '<li class="annivs_jour">Le(s) anniversaire(s) du jour:</li>';
					break;
				case 1:
    			echo '<li class="annivs_jour">L\'anniversaire du jour :</li>';
					break;
				default:
    			echo '<li class="annivs_jour">Les anniversaires du jour :</li>';
					break;
			}
			if($db->numRows($result)!=0){
    		while ($row = $db->fetchArray($result))
    		{
    	  	echo '<li>' . $row['lastname'] . ' ' . $row['firstname'] . ' : '.$row['age'].' ans</li>';
    		}
			}else{
				echo '<li>Aucun anniversaire aujourd\'hui</li>';
			}

			$result2 = $db->query("SELECT id_user, lastname, firstname, photo FROM trombi_trombi ORDER BY id DESC LIMIT 2");
			$last_suscribers = '';
			$i=1;
			while ($last_suscribers = $db->fetchArray($result2)) {
				$suscribers[$i]['id']        = $last_suscribers['id_user'];
				$suscribers[$i]['firstname'] = $last_suscribers['firstname'];
				$suscribers[$i]['lastname']  = $last_suscribers['lastname'];
				$suscribers[$i]['photo']     = $last_suscribers['photo'];
				$i++;
			}
			
			for ($i = 1; $i <= 2; $i++) {
				$last_suscribers .= '<div class="last_users_block">'
																.'<a href="#nogo" onclick="'.$trombi->openCard($suscribers[$i]['id']).'">'.$fm->preview($suscribers[$i]['photo'], 1).'</a>'
																.'<p class="users_infos_block">'.$suscribers[$i]['firstname'].' '.$suscribers[$i]['lastname'].'</p>'
													 .'</div>';
			}
		
    	echo '</ul></div>'
			
					.'<div class="right_column">'
							.'<p class="last_users_title">Les derniers inscrits : </p>'
							.$last_suscribers
					.'</div>'
					.'<div class="clear"></div>'
			
			
			
			.'</form>';
  	}
}
?>
