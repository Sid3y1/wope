/**
 * Here found compatibility with differents methods, classes, functions
 * for respect standarts of ECMAscript and Document Model Object (if is possible)
 * Many is for IE
 */

//TODO: make compatibility to load XML document


// browser detection
var ua = navigator.userAgent.toLowerCase(),
  isOpera = (ua.indexOf('opera') > -1),
  isSafari = (ua.indexOf('safari') > -1),
	isGecko = (!isOpera && !isSafari && ua.indexOf('gecko') > -1),
	isIE = (!isOpera && ua.indexOf('msie') > -1),
	isIE6 = (!isOpera && ua.indexOf('msie 6.0') > -1),
	isIE7 = (!isOpera && ua.indexOf('msie 7.0') > -1);

/**
 * Method addEventListener if not already exist. Add callback on event
 * @param String name of event
 * @param Function callback function called when event is fired
 * @param Boolean if the event capture is bubbled (here not work)
 */
 function addEventListener(el, eventName, callback, captureBubble)
 {
 	switch(eventName){
		case 'DOMContentLoaded':
			if(el.addEventListener){
				YAHOO.util.Event.addListener(el, eventName, callback);
			}else{//else is IE
				window.attachEvent('onload', callback);
			}
			break;
		default:
		 	YAHOO.util.Event.addListener(el, eventName, callback);
			break;
	}
/*    if(el.addEventListener)
*    {
*      el.addEventListener(eventName, callback, captureBubble);
*    }
*    else
*    //else is IE
*    {
*      switch(eventName)
*      {
*        //only on document.addEventListener()
*        case 'DOMContentLoaded' :
*    		  window.attachEvent('onload', callback);
*          break;
*        default:
*          el.attachEvent('on' + eventName, (function(){
*              return function()
*              {
*                callback.apply(el, new Array(event));
*              };
*          })());
*					break;
*      }
*    }
*/
	}


/**
 * Method removeEventListener if not already exist. Remove callback on event
 * @param String name of event
 * @param Function callback function called when event is fired
 * @param Boolean if the event capture is bubbled (here not work)
 */
  function removeEventListener(el, eventName, callback, captureBubble)
  {
 		YAHOO.util.Event.removeListener(el, eventName, callback);
/*    if(el.removeEventListener)
    {
      el.removeEventListener(eventName, callback, captureBubble);
    }
    else
    //else is IE
    {
      switch(eventName)
      {
        case 'DOMContentLoaded' :        
          break;
        default:
          el.detachEvent('on' + eventName, callback);
      }
    }
*/
  }

/**
 * Class XMLHttpRequest if not already exist. For making HTTPRequests
 * @note is impossible to add any properties or method to this object (or in general to ActiveXObject Class)
 */
if(!XMLHttpRequest)
{
  var XMLHttpRequest = function()
  {
    try
    {
      return new ActiveXObject('Msxml2.XMLHTTP');
    }
    catch(e)
    {
      try
      {
         return new ActiveXObject('Microsoft.XMLHTTP');
      }
      catch(E)
      {
        
      }
    }
    return null;
  }
}

	/**
 		* Method getComputedStyle if not already exist in window object
 		* @param Object element to get it style's object
 		* @param String CSS pseudoClass (:hover, :first-child,...) but here can't be use (IE alternate method hasen't got equivalent)
 		* @return CSSStyleDeclaration
 		*/
	if(!window.getComputedStyle)
	{
 		window.getComputedStyle = function(element, pseudoClass)
  	{
    	if(element.currentStyle)
      	return element.currentStyle;
  	};
	}

	/**
		* swapSelect() allows to hide SELECT elements when an element is over (IE's bug)
		*
		* @param Object el element to display over select elements
		* 
		* @return void
		*/
	function swapSelects(el){ 
		var el_x  = getX(el);
		var el_y  = getY(el); 
		var el_width  = el.offsetWidth;
		var el_height  = el.offsetHeight;
	
		var arSelects = document.getElementsByTagName('SELECT');
		var sel_length = arSelects.length;
		
		for (var i = 0; i < sel_length; i++) { 
			var select = arSelects[i];
			var select_x = getX(select);
			var select_y = getY(select); 
			var select_width = select.offsetWidth;
			var select_height = select.offsetHeight;
			
			var crossWidth = false; 
			if( el_x > (select_x - el_width) && el_x < (select_x + select_width) ){ 
				crossWidth = true; 
			}
			
			var crossHeight = false; 
			if ( el_y > (select_y - el_height) && el_y < (select_y + select_height) ){ 
				crossHeight = true; 
			}
			
			if (crossWidth && crossHeight) { 
				var vis = (el.style.visibility != 'hidden' && el.style.display != 'none') ? 'hidden' : 'visible'; 
				if (select.style.visibility != vis) {
					select.style.visibility = vis;
				}
			} else { 
				if (select.style.visibility != 'visible') {
					select.style.visibility = 'visible';
				} 
			} 
		} 
	}
