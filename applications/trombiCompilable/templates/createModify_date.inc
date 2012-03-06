<p class="form-input">
    <p class="form-input">
    <label for="id_/*id*/">/*text*/</label>
	/*input-text*/
		<input id="id_/*id*/" size="10"  maxlength="10" type="text" name="/*id*/" value="<?=preg_match("/^([0-9]{4})\-([0-9]{2})\-([0-9]{2})/",getValue($values,"/*id*/"), $temp)?$temp[3]."/".$temp[2]."/".$temp[1]:getValue($values,"/*id*/");?>" />
	/*input-text*/
	/*input-selects*/
		<select name="day_/*id*/" id="id_day_/*id*/">
<?php
	if(preg_match("/^([0-9]{4})\-([0-9]{2})\-([0-9]{2})/",getValue($values,"/*id*/"), $temp)){
		$day = $temp[3];
		$month = $temp[2];
		$year = $temp[1];
	}else{
		$day = $month = $year = '';
	}
	
	for($i = 1; $i <= 31; $i++){
	  $d = $i < 10 ? '0'.$i : $i;
    $sel = '';
    if( $day == $d ){
      $sel = ' selected="selected" ';
    }
    echo '<option value="'.$d.'" '.$sel.' >'.$d.'</option>';
  }
?>
		</select>
		<select name="month_/*id*/" id="id_month_/*id*/">
<?php
  for($i = 1; $i <= 12; $i++){
    $m = $i < 10 ? '0'.$i : $i;
    $sel = '';
    if( $month == $m ){
      $sel = ' selected="selected" ';
    }
    echo '<option value="'.$m.'" '.$sel.' >'.$m.'</option>';
  }
?>
		</select>
		<select name="year_/*id*/" id="id_year_/*id*/">
<?php
	$now = getdate();
  $cur_year = $now['year'];
	$start_year = $cur_year - 100;
	$end_year = $cur_year - 5;
	
	for($y = $start_year; $y <= $end_year; $y++){
    $sel = '';
    if( $year == $y ){
			$sel = ' selected="selected" ';
		}
		echo '<option value="'.$y.'" '.$sel.' >'.$y.'</option>';
	}
?>
		</select>
	/*input-selects*/
	
		<span>/*extraText*/</span>
	
	/*calendar*/
		<a title="<?=locale::display('calendar', 'kernel')?>" class="calendar_button" onclick="selectDate('cal_container', function(date){gE('id_/*id*/').value = date;}, true, 'FR');">&nbsp;</a>
	/*calendar*/
	
	<label for="id_/*id*/"><?=getError($error,"/*id*/");?></label>
	<div id="cal_container"></div>
</p>
