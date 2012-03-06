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


class TextEditor{

	private $name;

	private $options;
	
	public final function __construct($name = '', $options = Array()){
		$this->name = $name;
		$this->options = $options;
	}

	public function display(){
		echo '
		<textarea name="'.$this->name.'" rows="'.(isset($this->options['rows']) ? $this->options['rows'] : '10').'" cols="'.(isset($this->options['cols']) ? $this->options['cols'] : '55').'">'.( isset($this->options['value']) ? $this->options['value'] : '').'</textarea>
		<script type="text/javascript">tinyMCE.execCommand("mceAddControl",true,"'.$this->name.'");</script>
		';
	}

	public function loadScripts(){
		global $core;
		
		echo '<script type="text/javascript" src="'.$core->makeUrl('javascript/tiny_mce/tiny_mce.js').'"></script>';

		// This allows to save editor content even if in an div loaded in the "AJAX" way.
		// However, this is a bad solution : editor content isn't cleaned up,
		// which entraine some problems with HTML entities
		echo '<script type="text/javascript">
			saveEditorContentOnChange = function(){
				tinyMCE.selectedInstance.formElement.value = tinyMCE.getContent();
			};
		</script>';
		
		echo '<script type="text/javascript">
							tinyMCE.init({
								mode : "exact",
								theme : "advanced",
								language : "fr",
								auto_reset_designmode : true,
								onchange_callback : "saveEditorContentOnChange",
								plugins : "preview,spellchecker,contextmenu,paste",
								theme_advanced_layout_manager : "SimpleLayout",
								theme_advanced_disable : "underline,strikethrough,formatselect,styleselect,anchor,image,cleanup,help,code,hr,visualaid,justifyleft,justifycenter,justifyright,justifyfull,separator,bold,italic,sub,sup,removeformat,charmap",
								theme_advanced_buttons1_add : "formatselect,justifyleft,justifycenter,justifyright,justifyfull,separator,bold,italic,separator,forecolor,backcolor",
								theme_advanced_buttons2_add_before: "cut,copy,paste",
								theme_advanced_buttons2_add : "separator,spellchecker,charmap,preview",
								theme_advanced_blockformats : "p,h1,h2,h3,h4,h5,h6",
								theme_advanced_toolbar_location : "top",
								theme_advanced_toolbar_align : "left",
								spellchecker_languages : "+English=en,French=fr"								
							});
						</script>';
	}
	
}
?>
