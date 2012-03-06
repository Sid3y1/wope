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


require_once (dirname(__FILE__)."/tag.class.inc");
   		
class tag_interface {
	
	private $tag=null;
	private $tableName;
	private $tableNames;
	private $core;
	private $js_array_set;
	private $cloudCache;

    public function __construct($tableName,$idName) {
    	global $core,$db;
    	$this->js_array_set=false;
   		$this->tableNames=array($tableName=>$idName);
   		$this->tableName=$tableName;
   		$this->tag = new Tag($this->tableNames, $core, $db);
   		$this->core=$core;
   		$this->cloudCache=array();
    }
    
    public function displayStar($idInformation) {
    	$on=$this->tag->isStarred($idInformation);
    	$ret='<a id="star_'.$idInformation.'" class="star_'.($on?"on":"off").'"' 
					.'onclick="'
						.'star=gE(\'star_'.$idInformation.'\');'
						.'if (star.getAttribute(\'class\')==\'star_on\') {'
							.'star.setAttribute(\'class\', \'star_off\');'
							.'AJAX.getAndEval(\''
								.$this->ajaxAction("action=unstar_information&id_information=$idInformation").'\');'
						.'} else {'
							.'star.setAttribute(\'class\', \'star_on\');'
							.'AJAX.getAndEval(\''
								.$this->ajaxAction("action=star_information&id_information=$idInformation").'\');'
						.'}'
						.'"></a>';	
    	return $ret;
    }
		
		
    public function displayShared($idInformation) {
    	$on=$this->tag->isShared($idInformation);
    	$ret='<a id="share_'.$idInformation.'" class="action_icon share sharing_'.($on?"on":"off").'"' 
					.'onclick="'
						.'share=gE(\'share_'.$idInformation.'\');'
						.'if (share.getAttribute(\'class\')==\'action_icon share sharing_on\') {'
							.'share.setAttribute(\'class\', \'action_icon share sharing_off\');'
							.'AJAX.getAndEval(\''
								.$this->ajaxAction("action=unshare_information&id_information=$idInformation").'\');'
						.'} else {'
							.'share.setAttribute(\'class\', \'action_icon share sharing_on\');'
							.'AJAX.getAndEval(\''
								.$this->ajaxAction("action=share_information&id_information=$idInformation").'\');'
						.'}" '
						.'onmouseover="show(gE(\'sharing_popup_'.$idInformation.'\'));" ' 
						.'onmouseout="hide(gE(\'sharing_popup_'.$idInformation.'\'));" >'
								.'<a id="sharing_popup_'.$idInformation.'" class="action_title" style="z-index: 900; display: none;">Partager</a>'
						.'</a>';	
						
    	return $ret;
    }
   
    
		public function displayDigg($idInformation) {
    	
		$nbDigg=$this->tag->getNbDigg($idInformation);
    	
    	return "<div class=\"image_digg\">
				<span class=\"score_digg\">
					<strong id=\"score_digg_$idInformation\">$nbDigg</strong> diggs
				</span>
			</div>";
    }
    
    public function displayAddDigg($idInformation) {
    	if ($this->tag->isDigg($idInformation)) {
    		return "";
		} else {
			return "<div class=\"image_add_digg\" id=\"image_add_digg_$idInformation\">
					<a class=\"add_digg\" onclick=\"
						AJAX.getAndUpdate('".$this->ajaxAction("action=digg_information&id_information=$idInformation")."','score_digg_$idInformation');gE('image_add_digg_$idInformation').innerHTML='';\">digg + 1</a>
				</div>";
		}
    }
    
    private function ajaxAction($action) {
    	$core=$this->core;
    	$ret=$core->makeUrl("/tag/ajax/tag.php");
    	$action="table_name=".urlencode($this->tableName)
    		."&table_name_id=".urlencode($this->tableNames[$this->tableName])
    		."&".$action;
    	$sha=sha1($action."tag");
    	$ret.="?action=".urlencode($action)."&hash=".$sha;
    	return $ret;
    }
    
    public function displayTags($idInformation) {
    	$action= /* "window.location='".$this->core->makeUrl('/tag/search.php?tagid=$tagId')."'"; */ '';
    	return $this->displayTagsList($idInformation, $action);
    }
    
    private function displayTagsList($idInformation, $action,$inForm=false) {
    	if (!isset($this->cloudCache[$idInformation])) {
    		$this->cloudCache[$idInformation]=$this->tag->getCloudTag($idInformation);	
    	}
    	$tags=$this->cloudCache[$idInformation];
		if (isset($tags[0])) {
			$min=$tags[0]['tagCount'];
		} else {
			$min=0;
		}
		$max=0;
		foreach($tags as $tag) {
			if ($min>$tag['tagCount']) {
				$min=$tag['tagCount'];
			}
			if ($max<$tag['tagCount']) {
				$max=$tag['tagCount'];
			}
		}
		if ($inForm) {
			$id='cloud_tags_form_'.($idInformation);
			$class='mon_tagging';
			$style ="";
		} else {
			$id='cloud_of_tags_'.($idInformation);
			$class='cloud';
			$style ="display: block; display: none";
		}
		$ret='<div id="'.$id.'" class="'.$class.'" style="'.$style.'">';
		$i=0;
		if ($inForm) {
			$ret.='Tag existant :<br />';	
		}
		foreach ($tags as $tag) {
			$tagId='tag_'.($idInformation).$id.'_'.($i++);
			// Get the tag level (0..10)
			if ($max-$min>0) {
				$tagClass='tag_level_'.(intval(($tag['tagCount']-$min)*10/($max-$min)));
			} else {
				$tagClass='tag_level_3';
			}
			$action_i=str_replace('$tagName',urlencode($tag['tagName']),$action);
			$action_i=str_replace('$tagId',$tag['idTagName'],$action_i);
			if ($i>1) {
				$ret.=', ';	
			}
			$ret.='<a id="'.$tagId.'" class="'.$tagClass.'" onclick="'.$action_i.'">'.$tag['tagName'].'</a>';	
		}
		
		$ret.='</div>';
		
		return $ret;
	}

	public function displayComms($idInformation) {
		$ret='<div class="modele_comm" id="modele_comm_'.($idInformation).'">';
			
		$ret.= $this->displayCommsIdInformation($idInformation);
		$ret.="</div>";
		return $ret;
	}
	
	public function displayCommsIdInformation($idInformation) {
		$comments=$this->tag->getComments($idInformation,0,20);
		
		$this->core->loadClass("date");
		$date=new Date();
		$ret ="";
		foreach ($comments as $comment) {
			$ret.= '<div class="info_comm">'
								.'<span class="auteur_comm">'
										.'<strong>'.ucfirst($comment['author']{0}).'</strong>'.strtolower(substr($comment['author'],1))
								.'</span>'
								
								.' a dit : <span class="comm_date">('.$date->formatDate($comment['date'],"d/m/y à h:i").')</span>'
					  .'</div>'
						.'<div class="comm_texte">'.nl2br(htmlentities($comment['texte'],ENT_QUOTES,"UTF-8")).'</div>';			
		}
		return $ret;
	}
	
	public function displayFormAddComms($idInformation) {
		
		$ret = '<form id="form_mon_comm_'.$idInformation.'">'
						.'<textarea name=texte class="mon_comm" type="text" wrap="physical" rows="4" cols="94"></textarea>'
						.'<input type="button" onclick="AJAX.getAndUpdate(\''
								.$this->ajaxAction("action=add_comment&id_information=$idInformation")
								.'&texte=\'+encodeURIComponent(gE(\'form_mon_comm_'.$idInformation.'\').texte.value)'
								.',\'modele_comm_'.($idInformation).'\');gE(\'form_mon_comm_'.$idInformation.'\').texte.value=\'\';" class="poster_comm" value="poster mon commentaire" />
			  </form>';

		return $ret;
	}
	
	public function displayTagUser($idInformation) {
		$listTagsUser=$this->tag->getTagNamesUser($idInformation);
		$listTags='Mes tags : <br />'.' '.' '; /** THE TWO SPACES ARE HERE TO BE CUTTED BY THE "substr($listTags,0,-2)" IN CASE THERE IS NO TAGS **/
		foreach ($listTagsUser as $tagname) {
			$action='AJAX.getAndUpdate(\''
					.$this->ajaxAction("action=remove_tag&id_information=$idInformation")
					.'&tag_name='.urlencode($tagname['name']).'\''
					.',\'mon_tagging_'.($idInformation).'\');';
			$listTags.='<a onclick="'.$action.'">'.$tagname['name'].'</a>, ';
		}
		$listTags=substr($listTags,0,-2);
		
		return $listTags;
	}
	
	public function displayTagForm($idInformation) {
		
		
		$ret='<div id="cloud_form_'.$idInformation.'" style="display: none;">';
		$action='AJAX.getAndUpdate(\''
					.$this->ajaxAction("action=add_tag&id_information=$idInformation")
					.'&tag_name=$tagName\''
					.',\'mon_tagging_'.($idInformation).'\');';
		$ret.=$this->displayTagsList($idInformation, $action,true);			
		
		$ret.='		<div id="mon_tagging_'.$idInformation.'" class="mon_tagging">';
		$ret.= $this->displayTagUser($idInformation);
		$ret.='		</div>
					<div class="clear"></div>';
		
		$action='AJAX.getAndUpdate(\''
					.$this->ajaxAction("action=add_tag&id_information=$idInformation")
					.'&tag_name=\'+encodeURIComponent(gE(\'mon_tagging_form_'.$idInformation.'\').'.'mon_tagging_input_'.$idInformation.'.value)'
					.',\'mon_tagging_'.($idInformation).'\'); gE(\'mon_tagging_form_'.$idInformation.'\').'.'mon_tagging_input_'.$idInformation.'.value=\'\';';
						
		$ret.='<form class="formta" id="mon_tagging_form_'.$idInformation.'" onsubmit="'.$action.';return false;">';
		$ret.='		<div class="bloc_ajout_tag">
									<label for="mon_tagging_input_'.$idInformation.'" class="champ_ajouter_tag left">Ajouter un tag : </label>
									<input class="mon_tagging_input left" type="text" id="mon_tagging_input_'.$idInformation.'" value="" />
									<div class="mon_tagging_container" id="mon_tagging_'.$idInformation.'_container"></div>
									<a class="bouton_ajout_tag left" onclick="'.$action.'"></a>
							</div>			
					 </form>
					 <div class="clear"></div>
					 
			</div>
			<div class="clear"></div>';
		$ret.="\n\n\n";
		$ret.="<script type=\"text/javascript\">";
		if (!$this->js_array_set) {
			$this->js_array_set=true;
			$tagNames=$this->tag->getTagNames();
			$listTags="";
			foreach ($tagNames as $tagname) {
				$listTags.='"'.$tagname['name'].'",';
			}
			$listTags=substr($listTags,0,-1);
			
			$ret.='var listTags = ['.$listTags.'];';
			$ret.='var listTagsDS = new YAHOO.widget.DS_JSArray(listTags);';
		}
		
		$ret.='var mon_tagging_'.$idInformation.'_autoComp = new YAHOO.widget.AutoComplete("mon_tagging_input_'.$idInformation.'","mon_tagging_'.$idInformation.'_container", listTagsDS);';
		$ret.='mon_tagging_'.$idInformation.'_autoComp.animVert = false; ';
		$ret.='mon_tagging_'.$idInformation.'_autoComp.animHoriz = false; ';
		$ret.='mon_tagging_'.$idInformation.'_autoComp.maxResultsDisplayed = 20; ';
		$ret.='mon_tagging_'.$idInformation.'_autoComp.minQueryLength = 0; ';
		$ret.='mon_tagging_'.$idInformation.'_autoComp.queryDelay = 0; ';
		$ret.='mon_tagging_'.$idInformation.'_autoComp.typeAhead = false; ';
		$ret.="</script>";
		$ret.="\n\n\n";
		return $ret;
	}
	
	public function tagHeader($id) {
		$idInformation = $this->tag->getIdInformation($this->tableName,$id);
		$ret='<div class="col_g">';
		$ret.=$this->displayDigg($idInformation);
		$ret.=$this->displayAddDigg($idInformation);
		$ret.='<div class="specialclear2"></div>';
		$ret.=$this->displayStar($idInformation);
		$ret.='</div>';
		$ret.='<div class="col_d">';
		return $ret;
	}
	
	public function tagFooter($id) {
		$idInformation = $this->tag->getIdInformation($this->tableName,$id);
		
		$ret =$this->displayTags($idInformation);
		$ret.='<div class="clear"></div>';
		$ret.=$this->displayTagForm($idInformation);
		$ret.='
					<div class="bloc_see_all_comms">
						<a class="see_all_comms" onclick="display(gE(\'les_comms_'.$idInformation.'\')); display(gE(\'masquer_comms_'.$idInformation.'\')); display(gE(\'voir_comms_'.$idInformation.'\'))"><span id="voir_comms_'.$idInformation.'">Voir</span><span id="masquer_comms_'.$idInformation.'" style="display: none">Masquer</span> les commentaires ... </a>
					</div>
					
					<div class="sep_blocs"></div>
					
					<div class="bloc_see_tags">
						<a class="see_tags" onclick="display(gE(\'cloud_of_tags_'.$idInformation.'\')); display(gE(\'masquer_'.$idInformation.'\')); display(gE(\'voir_'.$idInformation.'\'))"><span id="voir_'.$idInformation.'">Voir</span><span id="masquer_'.$idInformation.'" style="display: none">Masquer</span> les tags</a>
					</div>
					
					<div class="sep_blocs"></div>
					
					<div class="bloc_add_tag">
						<a class="add_tag" onclick="display(gE(\'cloud_form_'.$idInformation.'\'));">Tagger l\'information</a>
					</div>	
					<div class="clear"></div>
				';
		
		$ret.='<div id="les_comms_'.($idInformation).'" style="display: block; display: none;">';
		$ret.=$this->displayComms($idInformation);	
		$ret.='<div class="specialclear"></div>';
		$ret.=$this->displayFormAddComms($idInformation);
		$ret.='</div>';
		return $ret;
	}

	public function photoTag($id) {
		$idInformation = $this->tag->getIdInformation($this->tableName,$id);
		$ret['id'] = $idInformation;
		
		$ret['display'] =$this->displayTags($idInformation);
		$ret['display'].='<div class="clear"></div>';
		$ret['display'].=$this->displayTagForm($idInformation);
		$ret['display'].='<div class="clear"></div>';
		$ret['display'].='<div id="les_comms_'.($idInformation).'">';
		$ret['display'].=$this->displayComms($idInformation);	
		$ret['display'].='<div class="specialclear"></div>';
		$ret['display'].=$this->displayFormAddComms($idInformation);
		$ret['display'].='</div>';
		return $ret;
	}
	
	
}
?>
