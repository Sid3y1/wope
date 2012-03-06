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
 * class String
 */
class String
{
  /**
		* Fonction encrypt
		* Crypte une chaine en fonction d'une cle
		*
		* @param $key         cle de cryptage
		* @param $plain_text  texte a crypter
		* @return texte crypte
		*/
	public function encrypt($key, $plain_text) {
		// returns encrypted text
		// incoming: should be the $key that was encrypt
		// with and the $plain_text that wants to be encrypted

		$plain_text = trim($plain_text);
		/* Quoting Mcrypt:
					"You must (in CFB and OFB mode) or can (in CBC mode)
					supply an initialization vector (IV) to the respective
					cipher function. The IV must be unique and must be the
					same when decrypting/encrypting."
					Meaning, we need a way to generate a _unique_ initialization vector
					but at the same time, be able to know how to gather our IV at both
					encrypt/decrypt stage.  My personal recommendation would be
					(if you are working with files) is to get the md5() of the file.
					In this example, however, I want more of a broader scope, so I chose
					to md5() the key, which should be the same both times. Note that the IV
					needs to be the size of our algorithm, hence us using substr.
			*/
		$iv = substr(md5($key), 0,mcrypt_get_iv_size (MCRYPT_CAST_256,MCRYPT_MODE_CFB));
		$c_t = mcrypt_cfb (MCRYPT_CAST_256, $key, $plain_text, MCRYPT_ENCRYPT, $iv);
		return trim(chop(base64_encode($c_t)));
	} // end of member function encrypt

  /**
		* Fonction decrypt
		* Decrypte une chaine en fonction d'une cle
		*
		* @param $key         cle de cryptage
		* @param $crypt_text  texte a decrypter
		* @return texte decrypte
		*/
	public function decrypt($key, $crypt_text) {
		// incoming: should be the $key that you encrypted
		// with and the $c_t (encrypted text)
		// returns plain text
		// decode it first :)
		$c_t =  trim(chop(base64_decode($crypt_text)));
		$iv = substr(md5($key), 0,mcrypt_get_iv_size (MCRYPT_CAST_256,MCRYPT_MODE_CFB));
		$p_t = mcrypt_cfb (MCRYPT_CAST_256, $key, $c_t, MCRYPT_DECRYPT, $iv);
		return trim(chop($p_t));
	} // end of member function decrypt

  /**
		* Fonction wordwrap3
		* Decoupage d'un text long
		*
		* @param $str Texte a formatter
		* @param $cols Nombre de caractere par ligne
		* @param $cut
		* @return str formatte
		*/
  public function wordwrap3($str,$cols,$cut="") {

		$tag_open = '<';
		$tag_close = '>';
		$white_space = ' ';
		$count = 0;
		$in_tag = 0;
		$str_len = strlen($str);
		$segment_width = 0;
		$last_white_space = 0;
		$begin_of_line = 0;

		for ($i=0 ; $i<=$str_len ; $i++){
			if ((isset($str[$i])) && ($str[$i] == $tag_open)) {
				$in_tag++;
			} else {
				if (isset($str[$i]) && $str[$i] == $tag_close) {

					if ($in_tag > 0) {
						$in_tag--;
					}

				} else {
					if ($in_tag == 0) {

						if(isset($str[$i]) && $str[$i] !=$white_space)
							$segment_width++;


						if ($segment_width > $cols) {

							if (isset($str[$i]) && $str[$i] == " ") {
								$str = substr($str,0,$i).$cut.substr($str,$i+1,$str_len-1);
								$i += strlen($cut);
								$str_len = strlen($str);
								$segment_width = 0;
								$begin_of_line = $i;
								$last_white_space = $i;

							} else {

								if ($last_white_space != $begin_of_line) {
									$i = $last_white_space;
								}

								$str = substr($str,0,$i+1).$cut.substr($str,$i+1,$str_len-1);
								$i += strlen($cut);
								$str_len = strlen($str);
								$segment_width = 0;
								$begin_of_line = $i;
								$last_white_space = $i;
							}
						} else {
							if (isset($str[$i]) && $str[$i] == " ") {
								$last_white_space = $i;
							}
						}
					}
				}
			}
		}
		return $str;
	} // end of member function wordwrap3

  /**
   *
   * @param string text    * @return bool
   * @access public
   */
  public function isUTF8 ($text) {
		// From http://w3.org/International/questions/qa-forms-utf-8.html
		return (mb_detect_encoding($text,"UTF-8, ISO-8859-1, ASCII")==="UTF-8");

	} // end of member function isUTF8

  /**
   *
   * @param string text    * @return string
   * @access public
   */
  public function convertUTF8 ($text) {
	$text=(mb_convert_encoding($text,"UTF-8"));
	return $text;
 } // end of member function convertUTF8

  /**
   *
   * @param string text    * @return string
   * @access public
   */
  public function unconvertUTF8 ($text) {
	$text=(mb_convert_encoding($text,"ISO-8859-1","UTF-8"));
	return $text;
 } // end of member function unconvertUTF8

	public function htmlToText($text){
		$text = html_entity_decode($text, ENT_QUOTES, 'UTF-8');
		$patterns = Array (
			"/<br \/>/si",
			// removes spaces
			"/([\n\r])[\s]+/",
			// removes javascript
			"/<script[?>]*?>.*?<\/script>/si",
			// removes HTML tags
			"/<[\/\!]*?[^<>]*?>/si"
		);

		$replacements = Array (
			"\n",
			"\\1",
			"",
			""
		);
		
		$text = preg_replace ($patterns, $replacements, $text);
		return $text;
	}

} // end of String
?>
