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

	private $input = '';
	
	private $output = '';
	
	private $pos = 0;
	
	private $inputLength = 0;
	
	private $protect = false;
	

	/*
	 * tags array containing open tags
	 */
	private $tags = Array();

	public final function __construct(){
		$this->input = '';
		$this->output = '';
		$this->pos = 0;
		$this->inputLength = 0;
		$this->tags = Array();
	}

	/*
	 * BBParse is the method to call to parse text
	 *
	 * @param  string  text to parse
	 * 
	 * @return parsed text
	 */
	public function parse($text = ''){
		$this->input = $text;
		$this->output = '';
		$this->pos = (int)0;
		$this->tags = Array();
		
		$this->inputLength = (int)strlen($text);

		if( $this->inputLength > 0){
			$this->parseBB();
			$this->addMissingTags();
		}
		
		return $this->output;
	}
	


	private function parseBB(){
		
		while( $this->pos < $this->inputLength ){

			$char = $this->input[$this->pos];
		
			switch($char){
				
				case '[':
					
					if( $this->pos+1 < $this->inputLength ){
						
						//open
						if( $this->input[$this->pos+1] != '/' ){
	
							$tag = '';
							$this->pos++;
							while( ($this->pos < $this->inputLength) && $this->input[$this->pos] != ']' && $this->input[$this->pos] != '='){
								$tag .= $this->input[$this->pos];
								$this->pos++;
							}
							if( $this->pos < $this->inputLength ){
								if($this->input[$this->pos] == '='){
									$opt = '';
									$this->pos++;
									
									while( $this->pos < $this->inputLength && $this->input[$this->pos] != ']' ){
										$opt .= $this->input[$this->pos];
										$this->pos++;
									}
									if( $this->pos < $this->inputLength && $this->input[$this->pos] == ']' ){
										$this->openBBTag($tag, $opt);
									}else{
										$this->output .= '['.$tag.'='.$opt;
									}
								}else{
									$this->openBBTag($tag);
								}
							}else{
								$this->output .= '['.$tag;
							}
							
						//close
						}else{
							
							$tag = '';
							$this->pos += 2;
							while( $this->pos < $this->inputLength && $this->input[$this->pos] != ']' ){
								$tag .= $this->input[$this->pos];
								$this->pos++;
							}
							
							if( $this->pos < $this->inputLength && $this->input[$this->pos] == ']' ){
								$this->closeBBTag($tag);
							}else{
								$this->output .= '[/'.$tag;
							}
							
						}
					}
					break;

				//unix & windows
				case "\n":
					$this->output .= '<br />';
					break;

				//mac
				case "\r":
					//windows writes "\r\n"
					if( $this->pos+1 < $this->inputLength && $this->input[$this->pos+1] != "\n" ){
						$this->output .= '<br />';
					}
					break;

				default:
					$this->output .= $this->protect && $char == '"' ? htmlentities($char, ENT_QUOTES, 'UTF-8') : $char;
					break;
				
			}
			
			$this->pos++;
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
	private function openBBTag($tag, $opt = null){
		
		$isOpt = isset($opt) && $opt != '' ? true : false;
		
		//"img" is a critical HTML tag
		if( $this->removeHTMLTag('img') ){
			$this->output .= '" />';
		}

		//"a" is a critical HTML tag
		if( $tag != 'img' && $this->removeHTMLTag('a') ){
			$this->output .= '</a>';
		}
		
		switch( $tag ){
		
			case 'g':
			case 'b':
				$this->addHTMLTag('strong');
				$this->output .= '<strong>';
				break;
				
			case 'i':
				$this->addHTMLTag('em');
				$this->output .= '<em>';
				break;
				
			case 's':
			case 'u':
				$this->addHTMLTag('span');
				$this->output .= '<span class="bb_underline">';
				break;
				
			case 'img':
				$this->addHTMLTag('img');
				$this->output .= '<img ';
				if($isOpt){
					$this->output .= ' alt="'.str_replace(Array("\r", "\n", '"'), '', $opt).'" title="'.str_replace(Array("\r", "\n", '"'), '', $opt).'" src="';
				}else{
					$this->output .= ' src="';
				}
				$this->protect = true;
				break;
				
			case 'citer':
			case 'quote':
				$this->addHTMLTag('div', 2);
				$this->output .= '<div class="bb_quote">';
				if($isOpt){
					$this->output .= '<div class="bb_quote_author">'.$opt.' '.Locale::display('wrote', 'bbcode').' :</div>'.
						'<div class="bb_quote_text">';
				}else{  
					$this->output .= '<div class="bb_quote_author">'.Locale::display('Quotation', 'bbcode').' :</div>'.
						'<div class="bb_quote_text">';
				}
				break;
				
			case 'signer':
			case 'sign':
				$this->addHTMLTag('div');
				$this->output .= '<div class="bb_sign">';
				break;

			case 'mail':
			case 'url':
				$this->addHTMLTag('a');
				$content = '';
				$this->pos++;
				while( $this->pos < $this->inputLength && $this->input[$this->pos] != '['){
					$content .= $this->input[$this->pos];
					$this->pos++;
				}
				
				if( $this->pos < $this->inputLength ){
					$url = $isOpt ? $opt : $content;
					
					if( $tag == 'mail' || ereg('^[[:alnum:]_\.-]+@[[:alnum:]_\.-]+\.[[:alnum:]_\.-]+$', $url) ){
						$this->output .= '<a href="mailto:'.str_replace(Array("\r", "\n", '"'), '', $url).'">'.$content;
					}else{
						$this->output .= '<a href="'.str_replace(Array("\r", "\n", '"'), '', $url).'">'.$content;
					}

					if( $this->input[$this->pos] == '[' ){
						$this->pos--;
					}
				}else{
					$this->output .= $url;
				}
				break;
				
			default:
				$this->output .= '['.$tag.']';
				break;
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
	private function closeBBTag($tag){
		
		//"img" is a critical HTML tag
		if( $tag != 'img' && $this->removeHTMLTag('img') ){
				$this->output .= '" />';
		}

		//"a" is a critical HTML tag
		if( $tag != 'img' && $tag != 'url' && $tag != 'mail' && $this->removeHTMLTag('a') ){
			$this->output .= '</a>';
		}
	
		switch($tag){
			case 'url':
			case 'mail':
				if( $this->removeHTMLTag('a') ){
					$this->output .= '</a>';
				}
				break;
				
			case 'g':
			case 'b':
				if( $this->removeHTMLTag('strong') ){
					$this->output .= '</strong>';
				}
				break;
				
			case 's':
			case 'u':
				if( $this->removeHTMLTag('span') ){
					$this->output .= '</span>';
				}
				break;
				
			case 'i':
				if( $this->removeHTMLTag('em') ){
					$this->output .= '</em>';
				}
				break;
				
			case 'img':
				if( $this->removeHTMLTag('img') ){
					$this->protect = false;
					$this->output .= '" />';
				}
				break;
				
			case 'citer':
			case 'quote':
				if( $this->removeHTMLTag('div', 2) ){
					$this->output .= '</div></div>';
				}
				break;
				
			case 'signer':
			case 'sign':
				if( $this->removeHTMLTag('div') ){
					$this->output .= '</div>';
				}
				break;
				
			default:
				$this->output .= '[/'.$tag.']';
				break;
		}
	}
	
	
	private function addHTMLTag($tag, $nb = 1){
		for( $i = 0; $i < $nb; $i++ ){
			$this->tags[] = $tag;
		}
	}
	
	

	private function removeHTMLTag($tag, $nb = 1){
		//$result = true;
		$idx = count($this->tags) -1;
		$tagIndexes = Array();
		
		while( $nb > 0 && $idx >= 0 ){
			if( $this->tags[$idx] == $tag ){
				$tagIndexes[] = $idx;
				$nb--;
			}
			$idx--;
		}
		
		if( $nb == 0 ){
			$result = true;
			foreach( $tagIndexes as $idx ){
				array_splice($this->tags, $idx, 1);
			}
		}else{
			$result = false;
		}
		
		/*for( $i = 0; $i < $nb; $i++ ){
			$tagIdx = array_search($tag, $this->tags);
			if( $tagIdx !== false ){
				array_splice($this->tags, $tagIdx, 1);
			}else{
				$result = false;
			}
		}
		*/
		return $result;
	}


	/*
	 * addMissingTags  try to repair user's errors, adding missing tags
	 *
	 * @param  string  text  text to repair
	 *
	 * @return repaired text
	 */
	private function addMissingTags(){

		//"img" is a critical tag
		if( $idx = array_search('img', $this->tags) ){
			array_splice($this->tags, $idx, 1);
			$this->output .= '" />';
		}
		
		//"a" is a critical tag
		if( $idx = array_search('a', $this->tags) ){
			array_splice($this->tags, $idx, 1);
			$this->output .= '</a>';
		}

		$nb = count($this->tags) - 1;
		
		for($i = $nb; $i >= 0; $i--){
			$this->closeHTMLTag($this->tags[$i]);
		}

	}

	private function closeHTMLTag($tag = ''){
		
		switch($tag){
			
			case 'img':
				$this->output .= '" />';
				break;
				
			default:
				$this->output .= '</'.$tag.'>';
				break;
				
		}
		
	}

	
}//end of BBCode class
?>
