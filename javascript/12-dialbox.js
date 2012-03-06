/**
 * Object dialBox contend methods to make a in popup
 */
var dialBox = {
  mask : null,
  box : null,
  /**
   * draw is a method to draw the in popup
   *
   * @param content   a String, content of the in popup
   * @return          a DOMObject, return the link of created object
   */
  draw : function(content, titleContent, closeOnClick)
  {
		if(closeOnClick !== false){closeOnClick = true;};
		
    //Ajout du masque
    var mask = document.createElement('DIV');
    mask.style.visibility = 'hidden';
    mask.className = 'dialbox-mask';
    this.mask = document.body.appendChild(mask);
    //addEventListener(mask, 'click', function(){dialBox.erase();}, false);//IE Bugs
		
		if(closeOnClick){
	    mask.onclick = function()
  	  {
    	  dialBox.erase();
   		};
		}
    
    //TODO: add an annimation    
    
    var box = document.createElement('DIV');//create parent
    box.style.visibility = 'hidden';
    box.className = 'dialbox';
    this.box = document.body.appendChild(box);
    this.box.appendChild(document.createElement('H2')).innerHTML = titleContent || 'Information :';
    this.box = this.box.appendChild(document.createElement('DIV'));
        
    if(typeof content === 'string')
      this.box.innerHTML = content;
    else
      this.box.appendChild(content);
      
    var boxStyle = box.style;
	
		if(isIE && !isIE7){
			boxStyle.left = ( getViewportWidth() - box.offsetWidth ) / 2 + document.documentElement.scrollLeft + 'px';
			boxStyle.top = ( getViewportHeight() - box.offsetHeight ) / 2 + document.documentElement.scrollTop + 'px';
		}else{
	    boxStyle.top = ( mask.offsetHeight - box.offsetHeight ) /2 + 'px';
  	  boxStyle.left = ( mask.offsetWidth - box.offsetWidth ) /2 + 'px';
		}

    mask.style.visibility = 'visible';
    boxStyle.visibility = 'visible';
		
		if(isIE){
			swapSelects(box);
		}
		
    return box;
  },
  
  /**
   * erase is a method to remove the in popup
   *
   * @return     Void
   */
  erase : function()
  {
    //TODO: ajouter une annimation de disparition
    //Supression du masque

		if(isIE){
			this.box.style.visibility = 'hidden';
			swapSelects(this.box);
		}
				
    remove(this.mask);
    this.mask = null;
    remove(this.box.parentNode);
    this.box = null;
  },
  
  /**
   * drawAJAXContent is a method to make a HTTPTransaction and draw a in popup
   *
   * @params params  An Object, with each properties are content value : 
   *                 url, datas, httpMode, headers, asyncMode, user, pass, events
   *                 (Optional)
   * @return           Void
   */
  drawAJAXContent : function(params)
  {
    AJAX.makeHttpRequest(params);
  }
};
