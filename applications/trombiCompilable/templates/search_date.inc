<p class="form-input">
    <label for="id_/*id*/">/*text*/</label>
		<input id="id_/*id*/" size="10"  maxlength="10" type="text" name="/*id*/" value="<?=getValue("/*id*/");?>" />
		<span>/*extraText*/</span>
		
	/*calendar*/
		<a title="<?=locale::display('calendar', 'kernel')?>" class="calendar_button" onclick="selectDate('cal_container', function(date){gE('id_/*id*/').value = date;}, true, 'FR');">&nbsp;</a>
		<div id="cal_container"></div>
	/*calendar*/
	
</p>
