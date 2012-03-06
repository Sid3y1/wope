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
 * Date 
	* Classe de type string
 * 
 * @package 
 * @version $id$
 * @copyright Copyright
 * @author Sid 
 * @license PHP Version 5.0 {@link http://www.php.net/license/5_0.txt}
 */
class Date
{

	/**
	 * month 
	 * Mois de la date
	 * @var mixed
	 * @access public
	 */
	public $month;
	
	/**
	 * year 
	 * Année 
	 * @var mixed
	 * @access public
	 */
	public $year;
	
	/**
	 * day 
	 * Jour 
	 * @var mixed
	 * @access public
	 */
	public $day;
	
	/**
	 * arrayDow 
	 * Tableau des jours de la semmaine (DayOfWeek)
	 * @var mixed
	 * @access public
	 */
	public $arrayDow;

	/**
	 * arrayMonth 
	 * Tableau des Mois
	 * @var mixed
	 * @access public
	 */
	public $arrayMonth;

	/**
		* construct Constructeur de la date, il enregistre la date courante !
		* 
		* @param string $date 
		* @final
		* @access public
		* @return void
		*/
	public final function __construct($date=''){
		if( $this->isDate($date) ){
			$date = preg_match('/^([0-9]{4})-((0[1-9])|(1[0-2]))-((0[1-9])|([1-2][0-9])|(3[01]))$/', $date, $reg);
			$time = mktime(0, 0, 0, $reg[2], $reg[5], $reg[1]);
			$this->month = date('m',$time);
			$this->year  = date('Y',$time);
			$this->day   = date('d',$time);
		}else{
			$this->month = date('m');
			$this->year  = date('Y');
			$this->day   = date('d');
		}
		$this->arrayDow = array ( 0 => 'Dimanche', 1 => 'Lundi', 2 => 'Mardi', 3 => 'Mercredi', 4 => 'Jeudi', 5 => 'Vendredi', 6 => 'Samedi');
		$this->arrayMonth = array ( 1 => 'Janvier', 2 => 'Février', 3 => 'Mars', 4 => 'Avril', 5 => 'Mai' , 6 => 'Juin', 7 => 'Juillet', 8 => 'Août', 9 => 'Septembre', 10 => 'Octobre', 11 => 'Novembre', 12 => 'Décembre');
	}

	/**
		* display Pour afficher la date !
		* 
		* @access public
		* @return void
		*/
	public function display($separator = ' '){
		return $this->day.$separator.$this->month.$separator.$this->year;
	}
	
	/**
	  * returnDate() returns date in standard format
		*
		*	@return string
		*/
	public function returnDate(){
		return $this->year.'-'.$this->month.'-'.$this->day;
	}


	public function getDate(){
		return getdate(mktime(0, 0, 0, $this->month, $this->day, $this->year));
	}
	
	/**
		* Fonction formatDate
		* Retourne une date au format specifie
		*
		* @param $date date au format "YYYY-MM-DD" ou "YYYY-MM-DD HH:MM:SS"
		* @param $format format du retour (les parametres sont les memes que pour la fonction date()
		* @return date au format specifie
		*/

	function formatDate ($date='', $format = 'd/m/Y') {
	if($date==''){
  $date = $this->year.'-'.$this->month.'-'.$this->day;
	}
		if($date == '0000-00-00' && $format == 'd/m/Y') return '  /  /    ';
		if (ereg( "^([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})$", $date, $regs )) {
			return (date($format,mktime (0,0,0,$regs[2],$regs[3],$regs[1])));
		} else if (preg_match("/^([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})$/",$date, $regs) ) {
			return (date($format,
						mktime ($regs[4],$regs[5],$regs[6],
							$regs[2],$regs[3],$regs[1])));
		} elseif (preg_match("/^([0-9]{4})\-([0-9]{2})\-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})/",$date,$regs)){
			return (date($format,
						mktime ($regs[4],$regs[5],$regs[6],
							$regs[2],$regs[3],$regs[1])));

		} else {
			return $date;
		}
	}

	/**
		* Fonction reverseDate
		* Retourne une date au format SQL
		*
		* @param $date date au format "DD/MM/YYYY"
		* @return date au format SQL : "YYYY-MM-DD"
		*/

	public function reverseDate ($date) {
		//french usual format -> standard format
		if (ereg( "^([0-9]{1,2})[-/]([0-9]{1,2})[-/]([0-9]{4})$", $date, $regs )) {
			return (date("Y-m-d",mktime (0,0,0,$regs[2],$regs[1],$regs[3])));
		//this case is for magicForm
		}elseif (ereg( "^'?([0-9]{1,2})[-/]([0-9]{1,2})[-/]([0-9]{4})'?$", $date, $regs )) {
			return ("'".date("Y-m-d",mktime (0,0,0,$regs[2],$regs[1],$regs[3]))."'");
		//standard format -> french usual format
		}elseif(ereg( "^([0-9]{4})[-/]([0-9]{1,2})[-/]([0-9]{1,2})$", $date, $regs )){
			return (date("d/m/Y",mktime (0,0,0,$regs[2],$regs[3],$regs[1])));
		//not recognized format
		}else{
			return $date;
		}
	}

	
	/**
		* Fonction semestreEnCours
		* Retourne le semestre en cours
		*
		* @param $date Date au format Y-m-d dont on veut le semestre
		* @return ($saison, $annee) Liste avec (printemps|automne) + annee
		*/

		public function semestreEnCours($date) {
		$limite1 = 45; // Jour de l'annee oa? l'on passe en printemps (15 fev)
		$limite2 = 212; // Jour de l'annee oa? l'on passe en automne (1er Aout)
		$semestre = Array();
		if (ereg( "^([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})$", $date, $regs )) {
			$mkdate = mktime (0,0,0,$regs[2],$regs[3],$regs[1]);
			$dayOfYear = date("z", $mkdate);

			if ($dayOfYear < $limite1) {
				$semestre['saison'] = 'automne';
				$semestre['annee'] = date("Y",$mkdate) - 1;
			} elseif ($dayOfYear >= $limite1 and $dayOfYear < $limite2) {
				$semestre['saison'] = 'printemps';
				$semestre['annee'] = date("Y",$mkdate);
			} else {
				$semestre['saison'] = 'automne';
				$semestre['annee'] = date("Y",$mkdate);
			}

		}
		return $semestre;

	} // end of member function semestreEnCours

	
	/**
		* add() adds a number of days to this date
		*
		* @param   $nb_day  number of days to add
		* @return  date (format : YYYY-mm-dd)
		*/
	public function addDays($nb_day=''){
		if(ereg('^[[:digit:]]+$',$nb_day)){
			$nb_sec = $nb_day * 24 * 60 * 60;
			$new_time = mktime(0, 0, 0, $this->month, $this->day, $this->year) + $nb_sec;
			$this->year = date('Y',$new_time);
			$this->month = date('m',$new_time);
			$this->day = date('d',$new_time);
		}
		return $this->year.'-'.$this->month.'-'.$this->day;
	}

	/**
	  * compare() compare this date with an other (one date argument) ,
		* 	or compare first date argument with second date argument
		*
		* @param  string  $operator  operator to use for comparison
		* @param  string  $date1     date to compare this date with, or date to be compared to second date argument
		* @param  string  $date2     (optional) date to compare with first date argument
		* 		//dates have to be in standard format
		*
		* @return boolean
		*/

	public function compare($operator, $date1, $date2 = null){
		if(isset($date1) && $this->isDate($date1)){
		
			if(isset($date2) && $this->isDate($date2)){
				$time1 = $this->getTimestamp($date1);
				$time2 = $this->getTimestamp($date2);
			}else{
				$time1 = $this->getTimestamp();
				$time2 = $this->getTimestamp($date1);
			}
			
			switch($operator){
				case '<':
					$result = ( $time1 < $time2 );
					break;
				case '>':
					$result = ( $time1 > $time2 );
					break;
				case '<=':
					$result = ( $time1 <= $time2 );
					break;
				case '>=':
					$result = ( $time1 >= $time2 );
					break;
				case '==':
					$result = ( $time1 == $time2 );
					break;
				case '!=':
					$result = ( $time1 != $time2 );
					break;
				default:
					$result = false;
					break;
			}
		}
		else{
			$result = false;
		}
		
		return $result;
	}

	/**
	  * isDate() tests if passed argument is a date
		*
		* @param  string  $date  a date in standard format
		*
		* @return boolean
		*/
	public function isDate($date, $case=''){
		switch($case){
			case 'FR':
				return ereg('^((0[1-9])|([1-2][0-9])|(3[01]))[/ -]((0[1-9])|(1[0-2]))[/ -]([0-9]{4})$',$date);
			default:
				return ereg('^([0-9]{4})-((0[1-9])|(1[0-2]))-((0[1-9])|([1-2][0-9])|(3[01]))$',$date);
		}
	}
	
	/**
	  *  getTimestamp() returns timestamp of this date or of date passed in argument
		*
		*  @param  string  (optional) $date  a date in standard format
		*
		*  @return int  timestamp
		*/
	public function getTimestamp($date = null){
		if(isset($date) && $this->isDate($date)){
			$date = new Date($date);
			$timestamp = mktime(0, 0, 0, $date->month, $date->day, $date->year);
		}else{
			$timestamp = mktime(0, 0, 0, $this->month, $this->day, $this->year);
		}
		return $timestamp;
	}

} // end of Date Object
?>
