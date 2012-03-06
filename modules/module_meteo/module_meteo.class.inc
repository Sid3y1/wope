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


class Module_meteo extends Module {

	private $cityCode;

	private $config;
	
	public final function __construct( $informations = Array() ) {
		parent::__construct($informations);
		global $core;
		
		$this->cityCode = $this->getCityCode($core->user->getMyPref('modules_meteo_ville'));
	}


	public function getTitle() {
		global $core;
		return $core->user->getMyPref('modules_meteo_ville');
	}
	
  	public function display() {
    global $db, $core, $chronos;
		
		try{
			$cache = new User_cache('meteo','1',$this, 'displayContent');
			$cache->displayCache();
		}
		catch (FileException $e){
			echo $e->getFullDebugMessage();
			return false;
		}
  }

	public function displayContent() {
		$locale = Locale::getLocale();
		if( !$meteo = $this->getData($this->cityCode) ){
			echo "<h4>Service indisponible pour l'instant veuillez réessayer ultérieurement</h4>";
			return false;
		}
				
		echo '<div class="day_block">'
					.'<div class="picture i_'.$meteo['currentCondition']['code'].'"></div>'
					.'<div class="details">'
							.'<div class="temperature">'.$meteo['currentCondition']['temperature'].'°C</div>'
							.'<div class="weather">'.$locale->display(strtolower((string)$meteo['currentCondition']['sky']),'meteo').'</div>'
					.'</div>'
			 .'</div>';
	
		echo '<div class="next_days">';

		$target_day = false;
		foreach ($meteo['forecast'] as $day => $forecast) {
			$target_day = !$target_day;
			$class = (($target_day) ? 'prev_today' : 'prev_tomorrow' );
			$day_title = (($target_day) ? 'Pour la journée :' : 'Demain :' );
		/* '.$locale->display(strtolower(date('l',strtotime($day))),'kernel').' */
			echo '<div class="'.$class.'">'
						.'<div class="picture i_'.$forecast['code'].'"></div>'
						.'<dl class="prev_temp">'
							.'<dt>'.$day_title.'</dt>'
							.'<dd>'.$forecast['minTemperature'].'°C / '.$forecast['maxTemperature'].'°C</dd>'
							.'<dd>'.$locale->display(strtolower((string)$forecast['sky']),'meteo').'</dd>'
						.'</ul>'
					.'</div>';
		}
		echo '</div><div class="clear"></div>';

	}



		

/**
	*	This function parse the rss feed and return an array which contain every data we need
	* 
	* @return $meteo array
	*
	*
	*/

	private function getData($cityCode) {
		global $core;
	
		$core->loadClass('rss');	
		try{
			$rss = new ParseRss('http://weather.yahooapis.com/forecastrss?p=FRXX'.$cityCode.'&u=c',1);
		}catch(FileException $e){
			echo $e->getFullDebugMessage();
			return false;
		}
		
		$meteo = Array();
		$items = $rss->getItems();
		foreach ($items as $item){
			foreach ($item->children("http://xml.weather.yahoo.com/ns/rss/1.0") as $itemChildrenKey => $itemChildrenValue){
				if ($itemChildrenKey == "condition") {
					$attribut = $itemChildrenValue->attributes();
				    $meteo['currentCondition']['sky'] = $attribut['text'];
					$meteo['currentCondition']['temperature'] = $attribut['temp'];
					$meteo['currentCondition']['time'] = date('G:i',strtotime($attribut['date']));
					$meteo['currentCondition']['code'] = $attribut['code'];
				}
				if ($itemChildrenKey == "forecast") {
				    $attribut = $itemChildrenValue->attributes();
					$meteo['forecast'][date('Y-m-d',strtotime($attribut['date']))] = Array();
					$meteo['forecast'][date('Y-m-d',strtotime($attribut['date']))]['sky'] = $attribut['text'];
					$meteo['forecast'][date('Y-m-d',strtotime($attribut['date']))]['minTemperature'] = $attribut['low'];
					$meteo['forecast'][date('Y-m-d',strtotime($attribut['date']))]['maxTemperature'] = $attribut['high'];
					$meteo['forecast'][date('Y-m-d',strtotime($attribut['date']))]['code'] = $attribut['code'];
				}
			}
		}
		return $meteo;
	}

/**
	* Association between city name and its code on weather.com
	*
	* @return $code int
	*/

	private function getCityCode($city){
    switch ($city){
		
      case 'Troyes':
      $code="0101";
      break;
			
			case 'Belfort':
			$code="0012";
			break;
			
      case 'Paris':
      $code="0076";
      break;

      case 'Lyon':
      $code="0055";
      break;

      case 'Marseille':
      $code="0059";
      break;

      case 'Bordeaux':
      $code="0016";
      break;

      case 'Lille':
      $code="0052";
      break;

      case 'Nantes':
      $code="0072";
      break;

      case 'Toulouse':
      $code="0099";
      break;

      case 'Orléans':
      $code="0074";
      break;

      case 'Brest':
      $code="0019";
      break;

      case 'Rouen':
      $code="0085";
      break;

      case 'Tours':
      $code="0100";
      break;

      case 'Strasbourg':
      $code="0095";
      break;

      case 'Dijon':
      $code="0036";
      break;

      case 'Amiens':
      $code="0269";
      break;

      case 'Châteauroux':
      $code="0030";
      break;

      case 'Dax':
      $code="0149";
      break;

      case 'Lens':
      $code="0049";
      break;

      case 'Nice':
      $code="0073";
      break;

      case 'Niort':
      $code="0166";
      break;

      case 'Montpellier':
      $code="0068";
      break;

      case 'Rodez':
      $code="0171";
      break;

      case 'Reims':
      $code="0080";
      break;

      case 'Grenoble':
      $code="0153";
      break;

      case 'Auxerre':
      $code="0008";
      break;

      case 'La Rochelle':
      $code="0044";
      break;

      case 'Le Mans':
      $code="0048";
      break;

      case 'Clermont Ferrand':
      $code="0120";
      break;

      case 'Saint-Etienne':
      $code="0274";
      break;

      case 'Metz':
      $code="0064";
      break;

      case 'Chamonix':
      $code="0212";
      break;

      case 'Rennes':
      $code="0114";
      break;

      case 'Bourges':
      $code="0018";
      break;

      case 'Le Havre':
      $code="0047";
      break;

      case 'Caen':
      $code="0020";
      break;

      case 'Saint-Quentin':
      $code="0276";
      break;

      case 'Poitiers':
      $code="0170";
      break;

      case 'Biarritz':
      $code="0125";
      break;

      case 'Dunkerque':
      $code="0151";
      break;

      default:
      $code="0101";
			break;
    }

		return $code;
	}

	
} // End of class

?>
