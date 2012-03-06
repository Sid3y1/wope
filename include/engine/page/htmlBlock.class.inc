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


/**
	* HtmlBlock 
	* 
	* @package 
	* @version $id$
	* @author Sid 
	* @license PHP Version 5.0 {@link http://www.php.net/license/5_0.txt}
	*/
class HtmlBlock
{



	public $menu;

	public $form;

	public $box;

	/**
		* __construct Constructeur qui instancie surtout les classes filles 
		* 
		* @final
		* @access public
		* @return void
		*/
	public final function __construct (){
		$this->menu = new Menu();
		$this->form = new Form();
		$this->box = new Box();
	}


	/**
	  * pagesList() displays a list of page numbers in order to navigate between several pages
		*
		* @param  req          SQL request
		* @param  nb_per_page  number of rows to display per page
		* @param  cur_page     current displayed page (0 => special case)
		* @param  method       method the link has to use to act : onclick || href
		* @param  action       content of the onclick or href attribute
		*
		* @return void
		*/
	public function pagesList($req, $nb_per_page, $cur_page, $method, $action, $pg_label = ''){
	  global $db;
	  
	  //VK : temporary solution
	  if (is_int($req)) {
	  	$nb_row = $req;
	  } else {
   		$res = $db->query($req);
			$nb_row = $db->numRows($res);
	  }
		
		if( !isset($nb_per_page) || $nb_per_page == ''){
			$nb_per_page = (int)$nb_row;
		}

		//number of pages to display
		$nb_page = ceil((int)$nb_row / (int)$nb_per_page);

		//displays pages list only if more than one page
		if($nb_page != 1){
			
			echo '<div class="clear"></div><div id="pages_list" class="pages_list">'.$pg_label;
			$points_before = $points_after = false;

			//displays less-than signs if current page isn't the first
			if($cur_page != 1 && $cur_page != 0){
				echo '<a '.$method.'="'.str_replace('[#]',1,$action).'" class="first">&lt;&lt;</a>';
			}
			
			for ($i=1;$i<=$nb_page;$i++){
			
				//for a great number of pages, occults some of them
				if( $nb_page > 10 && $i != $nb_page && $i != 1){

					//occults page numbers inferior to current page number of more than 2 and puts points instead
					if( ($cur_page - $i) > 2 ){
						if($points_before == false){
							echo ' .. ';
							$points_before = true;
						}
					}
					
					//occults page numbers superior to current page number of more than 2 and puts points instead
					if( ($i - $cur_page) > 2 ){
						if($points_after == false){
							echo ' .. ';
							$points_after = true;
						}
					}
				}
			
				//in other cases, displays page number
				if( $nb_page<10 || (abs($cur_page - $i)) <= 2 || $i==1 || $i==$nb_page){
					//current page isn't clickable
					if($cur_page==$i){
						echo '<span class="selected_page"> '.$i.' </span>';
					}else{
						echo '<a '.$method.'="'.str_replace('[#]',$i,$action).'">'.$i.'</a>';
					}
				}
				
			
			}

			//displays greater-than signs if current page isn't the last
			if($cur_page != $nb_page && $cur_page != 0){
				echo '<a '.$method.'="'.str_replace('[#]',$nb_page,$action).'" class="last">&gt;&gt;</a>';
			}
			
			echo '</div><div class="clear"></div>';
		}
	}
	
  public function escapeTipContent($text = ''){
		$text = addslashes($text);
		$text = preg_replace("/\\r\\n|\\n|\\r/","<br />",$text); //better than nl2br() -> adds "<br />" AND removes line breaks
		$text = htmlentities($text,ENT_QUOTES,'UTF-8');
		return $text;
	}
	
	public function tooltip($text = null){
		return isset($text) ? 'activateToolTips(this, \''.$this->escapeTipContent($text).'\');' : false;
	}
	
	public function showTrombi($userId) {
		$div_id=sha1(microtime());
		return $this->box->info("<div id=".$div_id."><div>").";AJAX.getAndUpdate('".$core->makeUrl("/user/ajax/user_infos.php?id=$userId")."','".$div_id."')";
	}
} // end of HtmlBlock

?>
