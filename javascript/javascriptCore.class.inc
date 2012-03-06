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
 * JavascriptCore 
 * 
 * @package 
 * @version 1.001
 * @copyright Copyright
 * @author Sid 
 * @license PHP Version 5.0 {@link http://www.php.net/license/5_0.txt}
 */
class JavascriptCore
{

	/**
	 * __construct Constructeur vide
	 * 
	 * @access public
	 * @return void
	 */
	public function __construct(){
//Void
	}

	/**
	 * loadScripts Charge les scripts javascripts
	 * 
	 * @access public
	 * @return void
	 */
	public function loadScripts(){
		global $OPT,$core,$config;
		$dir = opendir($core->getConfig('baseServer').'/javascript/'); 
		//C'est pour IE sinon tout va buguer :
		echo "<!--[if ie]><script>var behavior = '". $core->makeUrl('javascript/behavior.htc') . "', skinFolder = '".$core->makeUrl('skins/'.$core->user->getSkin())."/';</script><![endif]-->";
		/* //linkage vers tous les fichiers *.js
		while($file= readdir($dir)){
			if(basename($file,'.js') != basename($file)){
				echo '<script type="text/javascript" src="'.$core->makeUrl('javascripts/'.$file).'"></script>
					';
			}
		}
		*/
		
		if(isset($OPT) && isset($OPT['pre_scripts'])){
			foreach($OPT['pre_scripts'] as $script){
				switch($script){
					case 'editor':
						$core->loadClass('texteditor');
						$te = new TextEditor();
						$te->loadScripts();
						break;
					default:
						//empty
						break;
				}
			}
		}
		
		echo '<script type="text/javascript" src="'.$core->makeUrl('javascript/javascript_core.js').'"></script>';
	
	}


	/**
	 * loadContextScripts Loads scripts used with elements loaded after the page
	 * 
	 * @param  $context
	 * @access public
	 * @return void
	 */
	public function loadContextScripts($context='',$path='.',$file=''){
		global $core,$db;
		if($path=='')$path='.';
		switch($context){
			case 'index':
				$userId = $core->getUserId();
				$modules = $db->query("
					SELECT mm.class FROM kernel_module_module mm INNER JOIN kernel_module_available ma ON mm.id = ma.id_module
    			WHERE ( ma.strict= 'Y' AND '".$userId."' IN ( SELECT login FROM kernel_right_user_get WHERE idRight = ma.right ) )
		        OR
						( ma.strict= 'N' AND '".$userId."' IN ( SELECT login  FROM kernel_right_user WHERE idRight = ma.right ) )
				");
				while($mod=$db->fetchArray($modules)){
					if(file_exists($path.'/modules/'.$mod['class'].'/javascript/'.$mod['class'].'.js')){
						echo '<script type="text/javascript" src="'.$core->makeUrl('modules/'.$mod['class'].'/javascript/'.$mod['class'].'.js').'"></script>';
					}
				}
				break;
			case 'admin':
	//			echo '<script type="text/javascript" src="'.$core->makeUrl('applications/admin/javascript/admin.js').'"></script>';
				$modules=$db->query("SELECT class FROM kernel_module_module WHERE admin_available = 'Y' ");
				while($mod=$db->fetchArray($modules)){
					if(file_exists($path.'/modules/'.$mod['class'].'/admin/javascript/'.$mod['class'].'_admin.js')){
						echo '<script type="text/javascript" src="'.$core->makeUrl('modules/'.$mod['class'].'/admin/javascript/'.$mod['class'].'_admin.js').'"></script>';
					}
				}
				break;
			case 'user_admin':
				echo '<script type="text/javascript" src="'.$core->makeUrl('applications/admin/javascript/admin.js').'"></script>';
				break;
			case 'editor':
				//RTE:
				//echo '<script type="text/javascript" src="'.$core->makeUrl('javascript/rte/html2xhtml.js').'"></script>';
				//echo '<script type="text/javascript" src="'.$core->makeUrl('javascript/rte/richtext_compressed.js').'"></script>';
				
				//Dojo with its editor:
				//echo '<script type="text/javascript" src="'.$core->makeUrl('javascript/dojo/dojo.js').'"></script>';
				
				//TinyMCE:
				//needs to be loaded first
				//echo '<script language="javascript" type="text/javascript" src="'.$core->makeUrl('javascript/tiny_mce/tiny_mce.js').'"></script>';
				break;
			default:
				if( file_exists($path.'/'.$context.'/javascript/'.$file.'.js') ){
					echo '<script type="text/javascript" src="'.$core->makeUrl($context.'/javascript/'.$file.'.js').'"></script>';
				}
				echo $core->makeUrl($context.'/javascript/'.$file.'.js');
				break;
		}
	}
	

  public function array2js($arr, $type='')
{
	if (!$type)
		$type = 'mixed';

	if (is_array($arr))
	{
		// no items in array
		if (!sizeof($arr))
			return 'new Array()';
		else
		{
			$a_pairs = array();
			$keys_arr = array_keys($arr);
			$is_assoc = $have_numeric = 0;

			for ($i=0; $i<sizeof($keys_arr); ++$i)
			{
				if(is_numeric($keys_arr[$i]))
					$have_numeric = 1;
				if (!is_numeric($keys_arr[$i]) || $keys_arr[$i]!=$i)
					$is_assoc = 1;
				if($is_assoc && $have_numeric)
					break;
			}

			$previous_was_array = false;
			while (list($key, $value) = each($arr))
			{
				// enclose key with quotes if it is not variable-name conform
				if (!ereg("^[_a-zA-Z]{1}[_a-zA-Z0-9]*$", $key) /* || is_js_reserved_word($key) */)
					$key = "'$key'";

				if (!is_array($value))
				{
					$value = str_replace("\r\n", '\n', $value);
					$value = str_replace("\n", '\n', $value);
				}

				$is_string = false;
				if (!is_array($value))
				{
					if ($type=='string')
						$is_string = true;
					else if ((($type=='mixed' && is_numeric($value)) || $type=='int') && strlen($value)<16)   // js interprets numbers with digits >15 as ...e+... 
						$is_string = FALSE;
					else
						$is_string = TRUE;
				}
			}
		}
	}
	}
}
?>
