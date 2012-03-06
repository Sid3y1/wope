/**
	* Object HUD contend methods to make a tooltips
	*/
var HUD = {
	//activate : function(plip,plop){}
  activate : function(message, className){
		className = className || '';
		HUD = document.body.appendChild(document.createElement('DIV'));
		if(!isNull(className))
			HUD.className = className;
		HUD.innerHTML = message;
	}
}

addEventListener(document,'DOMContentLoaded', function()
	{
	  if(document.getElementById('HUD'))//gE() bug avec IE
  	{
  	  HUD = gE('HUD');
   	  hide(HUD);
  	}
  	else
	  {
	    HUD = document.body.appendChild(document.createElement('DIV'));
   	  hide(HUD);
	    HUD.id = 'HUD';
	  }
	  HUD.activate = function(message, className)
	  {
	    this.innerHTML = message;
	    this.className = className || '';
	    show(this);
			
			if(HUD.attachEvent){
				HUD.style.top = document.documentElement.scrollTop + 'px';
			}
	  }
	});

/*//IE doesn't understand "position: fixed"
if(HUD.attachEvent){
	addEventListener(element, 'scroll', function(){
		HUD.style.top = document.documentElement.scrollTop + 'px';
	}, false);													
}*/
