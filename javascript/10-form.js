/*getInputs = function(form, typeName, name)
{
  form = $(form);
  var inputs = form.getElementsByTagName('input');
  
  if (!typeName && !name)
    return inputs;
    
  var matchingInputs = new Array();
  for (var i = 0; i < inputs.length; i++) {
    var input = inputs[i];
    if ((typeName && input.type != typeName) ||
        (name && input.name != name)) 
      continue;
    matchingInputs.push(input);
  }

  return matchingInputs;
};*/

//TODO: check problem with Select in IE


/**
 * toString is method to transform an Object to String. Example : <code>alert(Object);</code>
 * Here the method return datas of all FORM's INPUTs, SELECTs and TEXTAREAs.
 * All datas are seperated by a & sign
 * Datas are writen like this : name of input + equal sign + value or only value if name is undefined.
 *
 * @return    a String, form datas
 * @need      isNull()
 * @need      String#trim()
 */
function formToString(frm)
{
  var elements = frm.elements, values = new Array(), tmp;
  for(var i = 0, length = elements.length, element; i < length; i++)
  {
    tmp = inputToString(elements[i]);
    if(!isNull(tmp.trim()))
      values.push(tmp);      
  }
  return values.join('&');
};


/**
 * toString is method to transform an Object to String. Example : <code>alert(Object);</code>
 * Here the method return datas for all type of INPUTs (text, hidden, password, radio, checkbox, ...) and TEXTAREA
 * Datas are writen like this : name of input + equal sign + value or only value if name is undefined.
 *
 * @return    a String, input,textarea datas
 * @need      isNull()
 * @need      String#trim()
 */
function inputToString(input)
{
  var name = input.name,
  value = input.value,
  tmp = new String();
	tn = input.tagName;
	switch(tn.toLowerCase())
  {
  	case 'input':
			switch(input.type)
  		{
    		case 'text' :
    		case 'hidden' :
    		case 'password' :
    		case 'textarea' :
      		tmp = !isNull(name) && !isNull(value) ? input.name + '=' + input.value : (!isNull(value) ? value : '');
      		break;
    		case 'radio' :
    		case 'checkbox' :
      		if(input.checked)        
        		tmp = !isNull(name) && !isNull(value) ? input.name + '=' + input.value : (!isNull(value) ? value : '');
      		break;
				default:
					//void
					break;
			}
			break;
		case 'select':
  		var name = input.name,
  		value, option,
  		values = new Array(),
  		optionsCollection = input.options;
  		if(!isNull(name))
  		{
    		for(var i = 0, length = optionsCollection.length; i < length; i++)
    		{
      		option = optionsCollection[i];
      		value = option.value;
      		if(option.selected && !isNull(value.trim()))
      		{
        		values.push(name + '=' + value);
      		}
    		}
  		}
  		else
  		{
    		for(var i = 0, length = optionsCollection.length; i < length; i++)
    		{
      		option = optionsCollection[i];
      		value = option.value;
      		if(option.selected && !isNull(value.trim()))
      		{
        		values.push(value);
      		}
    		}
  		}
  		tmp = values.join('&');
			break;
		case 'textarea' :
      tmp = !isNull(name) && !isNull(value) ? input.name + '=' + input.value : (!isNull(value) ? value : '');
			break;
	
  }
  return tmp;
};



/**
 * formMakeHTTPTransaction is a FORM's method to send FORM's values (url, datas, httpMode)
 *
 * @params params  An Object, with each properties are content value : 
 *                 url, datas, httpMode, headers, asyncMode, user, pass, events
 *                 (Optional)
 * @return         Void
 * @need           AJAX#makeHTTPTransaction()
 */
function formMakeHTTPTransaction(frm, params)
{
  params = params || new Object();
  params.url = params.url || frm.action;
  params.datas = params.datas || formToString(frm);
  params.httpMode = params.httpMode || frm.method;
  AJAX.makeHTTPTransaction(params);
};

/**
 * formMakeAndCall is a FORM's method to send FORM's values (url, datas, httpMode)
 * and call function when request is completed and status is in (200 to 299)(onsuccess)
 *
 * @params callback  An Function or FuncLink, a callback function 
 * @return           Void
 * @need             HTMLFormElement#makeHTTPTransaction()
 */
function formMakeAndCall(frm, callback)
{
  formMakeHTTPTransaction(frm, {onsuccess : callback});
};


/**
 * validateInput is a method to verify if the content is valid.
 *
 * @param        a RegExp or String, regular expression or name of predefined regular expression Example : <code>RegExp.EMAIL</code> or <code>'EMAIL'</code>
 * @param  boolean  empty_auth  Can the input value be empty ?
 *
 * @return       <code>true</code> if is validated else <code>false</code>.
 */
function validateInput(input)
{
  var state = false;
	tn = input.tagName;
	switch(tn.toLowerCase())
	{
		case 'input':
  		switch(input.type)
  		{
    		case 'text' :
    		case 'password' :
    		case 'textarea' :
      		if(!input.pattern)
      		{
        		var regexp = typeof arguments[1] === 'string' ? RegExp[arguments[1]] : arguments[1];
						input.pattern = regexp;
      		}
					else{
        		var regexp = input.pattern;
					}

					if(!input.empty_auth){
						var empty_auth = (arguments[2] && typeof arguments[2] === 'boolean') ? arguments[2] : false ;
						input.empty_auth = empty_auth;
					}
					else{
						var empty_auth = input.empty_auth;
					}
					
					if(empty_auth && input.value == ''){
						state = true;
					}
					else if(regexp && regexp.test){
	      		state = regexp.test(input.value.trim());
					}
					else{
						state = false;
					}
					
      		if(typeof input.onkeyup != 'function')
      		{
        		input.onblur = 
        		input.onkeydown = 
        		input.onkeyup = function()
        		{
          		validateInput(this);
        		};
        		input.onfocus = function()
        		{
          		return validateInput(this);
        		};
      		}
      		if(!state)
        		addClassName(input.parentNode,'error');
      		else    
        		removeClassName(input.parentNode,'error');
      		break;
    		case 'checkbox' :
      		state = arguments.length >= 2 ? input.checked : !input.checked;
      		//state = unchecked(false) -> click (callfunction and save state) -> state = checked(true)
      		if(!state)
        		addClassName(input.parentNode, 'error');
      		else    
        		removeClassName(input.parentNode, 'error');
      		break;    
    		case 'radio' :
      		var group = input.form[input.name];
      		for(var i = 0, l = group.length, element; i < l; i++)
      		{
        		element = group[i];
        		if(arguments.length >= 2 || element != input)
          		state = element.checked ? true : state;
        		else if(!element.checked)
          		state = !input.checked ? true : state;
        		else
          		state = element.checked ? true : state;
        		if(typeof element.onmouseup != 'function')
          		element.onmouseup = function(){return validateInput(input);};
      		}
      		//alert(state);
      		if(!state)
        		for(var i = 0, l = group.length; i < l; i++)
          		addClassName(group[i].parentNode,'error');
      		else
        		for(var i = 0, l = group.length; i < l; i++)
          		removeClassName(group[i].parentNode,'error');
      		break;
				default:
					//void
					break;
			}
			break;
		case 'select' :
  		var state = false;
  		if(typeof input.onkeyup != 'function')
  		{
    		input.onmouseup =
    		input.onkeyup = function()
    		{
      		validateInput(this);
    		};
  		}
  		//return state;
  		state = !isNull(inputToString(input));
  		if(!state)
    		addClassName(input.parentNode,'error');
  		else    
    		removeClassName(input.parentNode,'error');  
			break;
		case 'textarea' :
    	if(!input.pattern)
    	{    
     		var regexp = typeof arguments[1] === 'string' ? RegExp[arguments[1]] : arguments[1];
     		input.pattern = regexp;
    	}
    	else{
     		var regexp = input.pattern;
			}
			
			if(!input.empty_auth){
				var empty_auth = (arguments[2] && typeof arguments[2] === 'boolean') ? arguments[2] : false ;
				input.empty_auth = empty_auth;
			}
			else{
				var empty_auth = input.empty_auth;
			}
		
			if(empty_auth && input.value == ''){
				state = true;
			}
			else if(regexp && regexp.test){
	    	state = regexp.test(input.value.trim());
			}
			else{
				state = false;
			}
			
    	if(typeof input.onkeyup != 'function')
    	{
     		input.onblur = 
     		input.onkeydown = 
     		input.onkeyup = function()
     		{
       		validateInput(this);
     		};
     		input.onfocus = function()
     		{
       		return validateInput(this);
     		};
    	}
    	if(!state)
     		addClassName(input.parentNode,'error');
    	else    
     		removeClassName(input.parentNode,'error');
    	break;
		default:
			//void
			break;
  }
  return state;
};

/**
 * validateForm is a method to verify if all FORM's INPUT', SELECT' and TEXTAREA' content is valid
 *
 * @return    <code>true</code> if is valid else <code>false</code>.
 */
function validateForm(frm)
{
  var state = true, elementCollection = frm.elements;
  for(var i = 0, length = elementCollection.length, validation; i < length; i++)
  {
    var elmt = elementCollection[i];
    if(typeof elmt.onfocus == 'function')//Inputs(type text, password) and TextAreas and Selects
    {
      //fireevent
      //elmt.dispatchEvent(document.createEvent('HTMLEvents').initEvent('focus', true, true));
      //state = elmt.validate() ? state : false;//Can't do that because we dont know the pattern gived
      state = elmt.onfocus() ? state : false;
    }
    else if(typeof elmt.onmouseup == 'function')//Inputs(type checkbox and radio)
    {
      state = validateInput(elmt,'flag') ? state : false;
    }
  }
  return state;
};

var calendars = new Object();

/**
 * selectDate() dispalays a calendar to select a date
 *
 * @param  string    container_id  id of the div which will contain the calendar
 * @param  function  callback      function to execute after a date is selected
 * @param  boolean   mobile        does the calendar appear near the mouse ?
 * @param  string    locale        locale (FR|US)
 *
 * @return void
 */
function selectDate(container_id, callback, mobile, locale) 
{
	
	if(calendars['"'+container_id+'"']){
		calendars['"'+container_id+'"'].hide();
		calendars['"'+container_id+'"'] = undefined;
	}

		calendars['"'+container_id+'"'] = new YAHOO.widget.Calendar("cal", container_id, {close: true});
		var cal = calendars['"'+container_id+'"'];
		cal.locale = locale || 'FR';
		cal.container = gE(container_id);
	
		cal.cfg.setProperty("MONTHS_SHORT", ["Jan", "Fév", "Mar", "Avr", "Mai", "Jui", "Jui", "Aoû", "Sep", "Oct", "Nov", "Déc"]);
		cal.cfg.setProperty("MONTHS_LONG", ["Janvier", "Février", "Mars", "Avril", "Mai", "Juin", "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"]);
		cal.cfg.setProperty("WEEKDAYS_1CHAR", ["S", "M", "D", "M", "D", "F", "S"]);
		cal.cfg.setProperty("WEEKDAYS_SHORT", ["Di", "Lu", "Ma", "Me", "Je", "Ve", "Sa"]);
		cal.cfg.setProperty("WEEKDAYS_MEDIUM", ["Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam"]);
		cal.cfg.setProperty("WEEKDAYS_LONG", ["Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"]);
		cal.cfg.setProperty("START_WEEKDAY", 1);

		cal.container.style.display = 'none';
	
	if(mobile){
		cal.container.style.position = 'absolute';
		cal.container.style.left = ((mouse.x >= 100) ? mouse.x - 100 : 0 ) + 'px';
		cal.container.style.top = mouse.y + 'px';
	}
	
	  cal.selectEvent.subscribe(function(){
			cal.hide();
//			removeEventListener(document, 'click', handleClickOutCal(container_id), false);

			callback( cal.toWopeDate(cal.getSelectedDates(), locale) );
 	 });

	

	  cal.render();
	
		calendars['"'+container_id+'"'].show();
		
//		addEventListener(document, 'click', handleClickOutCal(container_id), false);
	
}

function handleClickOutCal(id){
		if(gE(id).style.display != 'none' && (mouse.x < calendars['"'+id+'"'].container.offsetLeft || mouse.x > calendars['"'+id+'"'].container.offsetLeft + calendars['"'+id+'"'].container.offsetWidth || mouse.y < calendars['"'+id+'"'].container.offsetTop || mouse.y > calendars['"'+id+'"'].container.offsetTop + calendars['"'+id+'"'].container.offsetHeight )){
			calendars['"'+id+'"'].hide();
			calendars['"'+id+'"'] = undefined;
		}

}
  
	
	
/**
 * toWopeDate() is a method to create a calendar in your web page  
 *
 * @author : Sylvain@Wope ~ 11/2006
 *
 * @param : date you have selected in your calendar
 * @param : style you want to use to display the selected date
 *
 * @return : the date formatted as you wanted
 */
	 
	 YAHOO.widget.Calendar_Core.prototype.toWopeDate = function(date, locale) 
	 {
		 var returnDate = new Array();
		 var style = style;
     if (date instanceof Date) {
		   returnDate = [[date.getFullYear(), date.getMonth()+1, date.getDate()]];
		 } else if (typeof date == 'string') {
			   returnDate = this._parseDates(date);
			 } else if (date instanceof Array) {
			 	  for (var i=0;i<date.length;++i) {
					  var d = date[i];
					  var calYear, calMonth, calDate = new String();
					  calYear = d.getFullYear();
						calMonth = d.getMonth()+1;
            if (calMonth.toString().length == 1) {
	            calMonth = '0'+calMonth;
	          }
            calDate = d.getDate();
	          if (calDate.toString().length == 1) {
	            calDate = '0'+calDate;
	          }
					}
        }
				
				if (locale == 'US') {
			  	return calYear+'-'+calMonth+'-'+calDate;
				}
				if (locale == 'FR') {
					return calDate+'/'+calMonth+'/'+calYear;
				}
		};


