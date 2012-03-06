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


class File_Browser{

	private $config;

	private $path;

	private $id;

	private $files;

	public final function __construct(){
		include('config_file_browser.inc');
		$this->config = $config_file_browser;
		$this->path = $this->config['path'];

		do{
			$this->id = rand(1000000, 9999999);
		}while( isset($_SESSION['file_browser_'.$this->id]) );
		
		$_SESSION['file_browser_'.$this->id] = serialize($this);
	}

	public function display($case = 'user', $content = 'user', $option = ''){
		$method = 'displayFiles'.ucfirst(strtolower($case));
		$this->$method($content, $option);
	}
	
	public function displayFilesUser($user = 'user', $type = ''){
		global $db, $core, $page;
	
		if($user == 'user'){
			$this->saveFile();
		}
		
		$condition = ' ( 0 ';
		
		foreach($core->user->droits as $right){
		  $condition .= " OR right_r = '$right' ";
		}

		$condition .= ' ) ';
		
		if($user == 'user'){
		  $more = " AND login='".$core->getUserId()."' ";
		}else{
			$more = " AND login='".$user."' ";
			$user_info = $db->fetchArray($db->query("SELECT firstname, lastname FROM trombi_trombi WHERE id_user = '".$user."' "));
		}
		
		$result = $db->query("SELECT id, name, size, date, filetype FROM kernel_file_manager_files WHERE $condition AND searchable=true $more AND filetype LIKE '%$type%' ORDER BY name ASC ");

		while($row = $db->fetchArray($result)){
			$this->files[] = $row;
		}

		$header = ($user == 'user') ? 'Mes fichiers' : 'Fichiers de '.$user_info['firstname'].' '.$user_info['lastname'];
		
		$page->moduleHeader($header, '');
		echo '<div id="files_table_div">';
		$this->displayFilesTable($user);
		echo '</div>';

		if($user == 'user'){
			$this->displayAddFile();
		}
		
		$page->moduleFooter();

		$_SESSION['file_browser_'.$this->id] = serialize($this);
	}

	private function saveFile(){
		global $core, $page;
		$core->loadClass('filemanager');
		
		if( isset($_POST['new_file']) && isset($_SESSION['file_manager_'.$_POST['fm_id']]) ){
			$fm = unserialize($_SESSION['file_manager_'.$_POST['fm_id']]);
			$result = $fm->saveOrGet('', 'log', 'log');
			unset($_SESSION['file_manager_'.$_POST['fm_id']]);
		}
		
}

	private function displayAddFile(){
		global $core;
		$core->loadClass('filemanager');
		$fm = new FileManager();
		
		do{
			$id = rand(1000000, 9999999);
		}while(isset($_SESSION['file_manager_'.$id]));
		
		echo '<div class="clear"></div>
		<div>
			<form action="index.php" method="post" enctype="multipart/form-data">
				<div class="form-hidden">
					<input type="hidden" name="fm_id" value="'.$id.'" />
				</div>';
			
		$fm->upload('Ajouter un fichier :', 'my_new_file');
		
		echo '
				<p class="form-submit">
					<input type="submit" name="new_file" value="Ajouter" />
				</p>
			</form>
		</div>
		';

		$_SESSION['file_manager_'.$id] = serialize($fm);
	}

	public function displayFilesTable($user = 'user'){
		global $core, $db;
		$core->loadClass('date');
		$core->loadClass('box');
		$core->loadClass('filemanager');
		$core->loadClass('htmlblock');
		$fm = new FileManager();
		$box = new Box();
		$hb = new HtmlBlock();

		$files_nb = (int)count($this->files);
		$cols_nb = ((int)$this->config['files_per_column'] <= 3 ) ? (int)$this->config['files_per_column'] : 3;
		$rows_nb = (int)( intval($files_nb / $cols_nb) + ( ($files_nb % $cols_nb != 0) ? 1 : 0 ) );

		$files_window_width = $cols_nb * 200;
		$files_window_height = ($rows_nb < 3) ? $rows_nb * 175 : 3 * 150;

		echo '<div class="files_menu">
		';
		
		echo '<ul class="panel">
			<li><a href="javascript:if(window.selected_file){window.location.href = \''.$core->makeUrl('include/engine/file/fileManagerPreview.php').'?id=\' + window.selected_file + \'&amp;level=4\';}"><span>V</span>oir</a></li>
			<li><a href="javascript:if(window.selected_file){window.location.href = \''.$core->makeUrl('include/engine/file/fileManagerDownload.php').'?id=\' + window.selected_file;}"><span>T</span>élécharger</a></li>'.
/*			'<li><a href="javascript:function(){if(window.selected_file){'.
					$box->confirm('AJAX.getAndCall('.
								'\''.$core->makeUrl($this->path.'/ajax/file_browser.php').'?id_fb='.$this->id.'&amp;wope_action=remove_f&amp;f=\' + window.selected_file, '.
								'function(){'.
									'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/file_browser.php').'?id_fb='.$this->id.'&amp;wope_action=display_ft\',\'files_table_div\');'.
									'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/file_browser.php').'?id_fb='.$this->id.'&amp;wope_action=display_quotas\',\'files_quotas\');'.
								'}'.
							');',
							'Etes-vous sûr de vouloir supprimer ce fichier ?').
			'}}"><span>S</span>upprimer</a></li>'.*/
		'</ul>';
		
		echo '<div id="files_quotas">'.$fm->displayQuotas($user).'</div>';
		
		echo '</div>
		';
	
		echo '<div class="files_table_window left" style="height: '.$files_window_height.'px; width: '.$files_window_width.'px; overflow: auto;" >';

		echo '<table class="files_table">';
		
		for ($i = 0; $i < $rows_nb; $i++){
			echo '<tr>';
			for($j = 0; $j < $cols_nb; $j++){
				$el = ($cols_nb * $i) + $j;
				if(isset($this->files[$el])){
					$don = $this->files[$el];
					$date = new Date($don['date']);
					$name = stripslashes($don['name']);

					echo '<td class="file" id="td_'.$don['id'].'" width="'.($files_window_width / $cols_nb).'" height="'.($files_window_height / $rows_nb).'"><a onclick="'.

						'if(window.selected_file){'.
							'removeClassName(gE(\'td_\' + window.selected_file), \'selected\');'.
						'}'.
						'window.selected_file = \''.$don['id'].'\';'.
						'addClassName(this.parentNode, \'selected\');'.
					
					'">';
					echo '<span>'.$fm->getIcon($don['id'], stripslashes($don['filetype'])).'</span><br />';
					echo '<span id="file_manager_id'.$don['id'].'" class="file_manager_name">'.( (strlen($name) > 25) ? substr($name, 0, 22).'...' : $name ).'</span><br />
					<span>'.$fm->humanSize($don['size']).' '.$date->display('/').'</span></a><br />
					<span><a onclick="'.
						$box->confirm(
							'AJAX.getAndCall('.
								'\''.$core->makeUrl($this->path.'/ajax/file_browser.php').'?id_fb='.$this->id.'&amp;wope_action=remove_f&amp;f='.$don['id'].'\', '.
								'function(){'.
									'gE(\'td_'.$don['id'].'\').parentNode.removeChild(gE(\'td_'.$don['id'].'\'));'.
									'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/file_browser.php').'?id_fb='.$this->id.'&amp;wope_action=display_quotas\',\'files_quotas\');'.
								//	'AJAX.getAndUpdate(\''.$core->makeUrl($this->path.'/ajax/file_browser.php').'?id_fb='.$this->id.'&amp;wope_action=display_ft\',\'files_table_div\');'.
								'}'.
							')',
							'Etes-vous sûr de vouloir supprimer le fichier : '.$name
						).'" class="delete left" onmouseover="activateToolTips(this, \''.$hb->escapeTipContent('Supprimer le fichier').'\')"></a></span>';
					
					echo '</td>';
				}else{
					echo '<td></td>';
				}
			}
			echo '</tr>';
		}
			
		echo'
				</table>
		</div>';
	}

	public function removeFile($file_id){
		$i = 0;
		$row = current($this->files);
		while( $row['id'] != $file_id ){
			next($this->files);
			$row = current($this->files);
			$i++;
		}
		array_splice($this->files, $i, 1);
	}

}

?>
