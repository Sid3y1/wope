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


/******************* sommaire.class ***********************************

	définition pour le sommaire

**********************************************************************/
class Module_sommaire extends Module
{

	public function display()
	{
	  global $db,$core,$page;
	
	  $result = $db->query("SELECT * FROM sommaire_title WHERE is_hidden='false' AND category='a' ORDER BY name Asc");
	  echo '<ul class="list_block">';
	  
	  while ($categoryA = $db->fetchArray($result))
	  {
	
	    if ($core->verifDroits($categoryA['rights']))
	    {

	      $nameA = stripslashes($categoryA['name']);
	      $idA = $categoryA['id'];
	      $urlA ='';
	      if (isset($categoryA['url']) && ($categoryA['url']!=''))
	      	$urlA = ' href="'.stripslashes($categoryA['url']).'" ';
	      	

	      echo '<li class="titleA"><h3>';
	      if($categoryA['url']!=''){
	      	echo "<a $urlA>";
				}
	      	
	      echo '
					<span class="icon"></span>
					<span class="text">'.$nameA.'</span>
					<span class="clear"></span>
				';
				
		  	if($categoryA['url']!=''){
					echo '</a>';
				}

				echo '</h3>';

	      $result2 = $db -> query("SELECT * FROM sommaire_title WHERE father_id='$idA' AND is_hidden='false' AND category='b' ORDER BY name Asc");

	      $mediumTitle = '</li><ul>';
	

	      while ($categoryB = $db->fetchArray($result2))
	      {
	        if ($core->verifDroits($categoryB['rights']))
	        {
	          $urlB ='';
	          if (isset($categoryB['url'])&&($categoryB['url']!='')){
	          	$urlB = ' href="'.stripslashes($categoryB['url']).'" ';
						}
	          	
	          $nameB = stripslashes($categoryB['name']);
	          $idB = $categoryB['id'];

	          $mediumTitle .= '<li class="titleB"><h4>';
	          if($categoryB['url']!=''){
	      	  	$mediumTitle .= "<a $urlB>";
						}
			  		$mediumTitle .= $nameB;
	          if($categoryB['url']!=''){
			  			$mediumTitle.='</a>';
						}
			  		$mediumTitle.='</h4>';
	          
	          $result3 = $db -> query("SELECT * FROM sommaire_title WHERE father_id='$idB' AND is_hidden='false' AND category='c' ORDER BY name Asc");
	          
	          $mediumTitle .= '</li><ul>';
	          
	          while($categoryC = $db->fetchArray($result3)){
	          	if($core->verifDroits($categoryC['rights'])){
	          		$urlC ='';
	          		if (isset($categoryC['url'])&&($categoryC['url']!='')){
	          			$urlC = ' href="'.stripslashes($categoryC['url']).'" ';
								}
	          			
	          		$nameC = stripslashes($categoryC['name']);
			        	$idC = $categoryC['id'];
			        
			        	$mediumTitle .= '<li class="titleC">';
			        	if($categoryC['url']!=''){
			        		$mediumTitle .= "<a $urlC>";
								}
								$mediumTitle .= $nameC;
								if($categoryC['url']!=''){
									$mediumTitle .= '</a>';
								}
								$mediumTitle .= '</li>';
	          	}
	          }
	          $mediumTitle .= '</ul>';
	         }

	       }
	       echo $mediumTitle.'</ul>';
	     }
	   }
	   
	   echo '</ul>';
	   
	     $page->adminLink('module','module_sommaire','admin');
	 }	
    
}
?>
