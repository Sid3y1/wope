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


class BBCode {

	/*
	 * tags array containing numbers of each open tags
	 */
	private $tags = Array('strong' => 0, 'em' => 0, 'span' => 0, 'img' => 0, 'div' => 0, 'a' => 0);

	public final function __construct(){
	}

	/*
	 * BBParse is the method to call to parse text
	 *
	 * @param  string  text to parse
	 * 
	 * @return parsed text
	 */
	public function BBParse($text){
		//resets $tags
		$this->tags = Array('strong' => 0, 'em' => 0, 'span' => 0, 'img' => 0, 'div' => 0, 'a' => 0);

		//debug
		//echo '
		//
		//<div>';
		
		$text = $this->BBRead($text);

		//debug
		//print_r($this->tags);
		//echo '<br />
		//
		//';
		$text = $this->addMissingTags($text);

		//debug
		//print_r($this->tags);
		//echo '<br />
		//
		//';
		//echo '</div>
		//
		//';
		
		return $text;
	}
	

	/*
	 * BBRead  reads text to parse character by character
	 *
	 * @param  string  text  text to read
	 * @param  string  case  action to perform (open tag, close tag, etc...)
	 *
	 * @return altered text
	 */
	private function BBRead($text, $case = ''){
		$char = $text[0];
	
		switch($case){
		
			case 'tag':
				if($char == '/'){
					return $this->BBRead(substr($text, 1), 'close');
				}else{
					return $this->BBRead($text, 'open');
				}
				
			case 'open':
				$tag = '';
				while($text[0] != ']' && $text[0] != '='){
					$tag .= $text[0];
					$text = substr($text, 1);
				}
				return $this->BBOpenTag($text, $tag);

			case 'close':
				$tag = '';
				while($text[0] != ']'){
					$tag .= $text[0];
					$text = substr($text, 1);
				}
				return $this->BBCloseTag($tag).$this->BBRead(substr($text, 1));
				
			default:
			
				switch($char){
					case '[':
						return $this->BBRead(substr($text,1), 'tag');
					case '':
						return '';
					case "\n":
						return '<br />'.$this->BBRead(substr($text,1));
					default:
						return $char.$this->BBRead(substr($text,1));
				}
				
		}
	}


	/*
	 * BBOpenTag  replaces BB tags by HTML tags
	 *
	 * @param  string  text  text to parse
	 * @param  string  tag   tag to open
	 *
	 * @return altered text
	 */
	private function BBOpenTag($text, $tag){
		$opt = false;
		$option = '';
		
		if($text[0] == '='){
			$opt = true;
			$text = substr($text, 1);
			while(isset($text[0]) && $text[0] != ']'){
				$option .= $text[0];
				$text = substr($text, 1);
			}
		}
		
		$missing = '';
		//"img" is a critical tag
		if($this->tags['img'] > 0){
			$missing .= $this->closeTag('img');
		}
		
		//"a" is a critical tag
		if($this->tags['a'] > 0 && $tag != 'img'){
			$missing .= $this->closeTag('a');
		}
		
		switch($tag){
		
			case 'g':
			case 'b':
				$this->tags['strong']++;
				return $missing.'<strong>'.$this->BBRead(substr($text, 1));
				
			case 'i':
				$this->tags['em']++;
				return $missing.'<em>'.$this->BBRead(substr($text, 1));
				
			case 's':
			case 'u':
				$this->tags['span']++;
				return $missing.'<span class="bb_underline">'.$this->BBRead(substr($text, 1));
				
			case 'img':
				$this->tags['img']++;
				$result = '<img ';
				if($opt){
					$result .= ' alt="'.str_replace("\n", '', $option).'" title="'.str_replace("\n", '', $option).'" src="'.$this->BBRead(substr($text, 1));
				}else{
					$result .= ' src="'.$this->BBRead(substr($text, 1));
				}
				return $result;
				
			case 'citer':
			case 'quote':
				$this->tags['div'] += 2;
				$result = $missing.'
					<div class="bb_quote">';
				if($opt){
					$result .= '
						<div class="bb_quote_author">'.$option.' '.locale::display('wrote', 'bbcode').' :</div>
						<div class="bb_quote_text">'.$this->BBRead(substr($text, 1));
				}else{  
					$result .= '
						<div class="bb_quote_author">'.locale::display('Quotation', 'bbcode').' :</div>
						<div class="bb_quote_text">'.$this->BBRead(substr($text, 1));
				}
				return $result;
				
			case 'signer':
			case 'sign':
				$this->tags['div']++;
				return $missing.'
					<div class="bb_sign">'.$this->BBRead(substr($text, 1));

			case 'url':
				$this->tags['a']++;
				$url = '';
				$mail = false;
				$text = substr($text, 1);
				while(isset($text[0]) && $text[0] != '['){
					$url .= $text[0];
					$text = substr($text, 1);
				}
				if(ereg('^[[:alnum:]_\.-]+@[[:alnum:]_\.-]+\.[[:alnum:]_\.-]+$', $url)){
					$mail = true;
				}
				return $missing.'<a href="'.( ($mail) ? 'mailto:' : '' ).str_replace("\n", '', $url).'">'.( ($opt) ? $option : $url).$this->BBRead($text);
				
			case 'mail':
				$this->tags['a']++;
				$mail = '';
				$text = substr($text, 1);
				while(isset($text[0]) && $text[0] != '['){
					$mail .= $text[0];
					$text = substr($text, 1);
				}
				return $missing.'<a href="mailto:'.str_replace("\n", '', $mail).'">'.( ($opt) ? $option : $mail).$this->BBRead($text);
				
			default:
				return $this->BBRead(substr($text, 1));
		}
	}
	
	
	/*
	 * BBCloseTag  replaces BB tags by HTML tags
	 *
	 * @param  string  text  text to parse
	 * @param  string  tag   tag to close
	 *
	 * @return altered text
	 */
	private function BBCloseTag($tag){
	
		switch($tag){
			case 'url':
			case 'mail':
				if($this->tags['a'] > 0){
					$this->tags['a']--;
					return '</a>
					';
				}else{
					return '';
				}
				
			case 'g':
			case 'b':
				$this->tags['strong']--;
				return '</strong>
				';
				
			case 's':
			case 'u':
				$this->tags['span']--;
				return '</span>
				';
				
			case 'i':
				$this->tags['em']--;
				return '</em>
				';
				
			case 'img':
				if($this->tags['img'] > 0){
					$this->tags['img']--;
					return '" />
					';
				}else{
					return '';
				}
				
			case 'citer':
			case 'quote':
				$this->tags['div'] -= 2;
				return '
					</div>
				</div>
				';
				
			case 'signer':
			case 'sign':
				$this->tags['div']--;
				return '
					</div>
				';
				
			default:
				return '';
		}
	}


	/*
	 * addMissingTags  try to repair user's errors, adding missing tags
	 *
	 * @param  string  text  text to repair
	 *
	 * @return repaired text
	 */
	private function addMissingTags($text){
		//debug
		//echo $this->tags['a'];


		//"a" is a critical tag
		if($this->tags['a'] > 0){
			$text .= $this->closeTag('a');
		}

		foreach($this->tags as $tag => $nb){
			if($nb > 0){
				for($i=0; $i<$nb; $i++){
					$text .= $this->closeTag($tag);
				}
			}
			if($nb < 0){
				for($i=0; $i<abs($nb); $i++){
					$text = $this->openTag($tag).$text;
				}
			}
		}

		return $text;
	}

	private function closeTag($tag){
		//debug
		//echo 'close '.$tag.'<br />
		//
		//';
		
		$this->tags[$tag]--;
		switch($tag){
			case 'img':
				return '" />';
			default:
				return '</'.$tag.'>';
		}
	}

	private function openTag($tag){
		//debug
		//echo 'open '.$tag.'<br />
		//
		//';
		
		$this->tags[$tag]++;
		switch($tag){
			case 'img':
				return '<img "';
			default:
				return '<'.$tag.'>';
		}
	}
	
}//end of BBCode class
?>
