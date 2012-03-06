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


class File {

public $image;


	public function __construct(){
	}
	
  public function image_resize($filename, $newwidth,$newheight,$type_img_out=""){
  
    
    list($width,$height,$ext_img) = getimagesize($filename);
    
    if(!empty($type_img_out))$ext_img=$type_img_out;
    
    if($height<$width) $newheight = $newheight * $height/$width;
    elseif($height>$width) $newwidth = $newwidth * $width/$height;
        
    // chargement
    $thumb = imagecreatetruecolor($newwidth, $newheight);
    switch($ext_img){
      // rappel 
      // 1 -> GIF
      // 2 -> JPG
      // 3 -> PNG
      case 1:
        $source = imagecreatefromgif($filename);
        break;
      case 2:
        $source = imagecreatefromjpeg($filename);
        break;
      case 3:
        $source = imagecreatefrompng($filename);
        break;
      default:
        return false;
    } 

    // Redimensionnement
    if(imagecopyresized($thumb, $source, 0, 0, 0, 0, $newwidth, $newheight, $width, $height)){
    
        switch($ext_img){

          case 1: return $source = imagegif($thumb,$filename);
          case 2: return $source = imagejpeg($thumb,$filename);
          case 3: return $source = imagepng($thumb,$filename);
          default:
            return false;
        } 
      }
 	 
  }


	/*
	 * contrairement a image_resize, cette fonction redimensionne
	 * l'image en conservant ses proportions de maniere a ce
	 * qu'elle ne depasse pas les dimensions passees en parametre
	 */
	public function image_max_size($filename, $max_width, $max_height, $type_img_out=""){
    list($image_width,$image_height,$image_type,$image_att) = getimagesize($filename);
		$old_width=$image_width;
		$old_height=$image_height;
		if($image_width > $max_width){
		  $image_height = $max_width * $image_height / $image_width;
		  $image_width = $max_width;
		}
		if($image_height > $max_height){
		  $image_width = $max_height * $image_width / $image_height;
		  $image_height = $max_height;
		}
	
    if(!empty($type_img_out))$image_type=$type_img_out;
	
	  $thumb = imagecreatetruecolor($image_width, $image_height);
    switch($image_type){
      // rappel 
      // 1 -> GIF
      // 2 -> JPG
      // 3 -> PNG
      case 1:
        $source = imagecreatefromgif($filename);
        break;
      case 2:
        $source = imagecreatefromjpeg($filename);
        break;
      case 3:
        $source = imagecreatefrompng($filename);
        break;
      default:
        return false;
    } 

    // Redimensionnement
    if(imagecopyresized($thumb, $source, 0, 0, 0, 0, $image_width, $image_height, $old_width,$old_height)){
    
        switch($image_type){

          case 1: return $source = imagegif($thumb,$filename);
          case 2: return $source = imagejpeg($thumb,$filename);
          case 3: return $source = imagepng($thumb,$filename);
          default:
            return false;
        } 
      }
 																																																																				
	}
}
?>
