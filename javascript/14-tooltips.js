/**
 * Object dialBox contend methods to make a tooltips
 */
var toolTips;
//Initalize at document loaded
addEventListener(document, 'DOMContentLoaded', function()
  {
    toolTips = document.body.appendChild(document.createElement('DIV'));
    hide(toolTips);
    toolTips.style.display = 'none';
	toolTips.style.visibility = 'visible'; 
    toolTips.className = 'tooltips';
    toolTips.setPos = function(x, y)
    {
      this.style.left = x + 10 + 'px';
      this.style.top = y + 10 + 'px';
    };
    toolTips.activate = function(element, content)
    {
      if(content)
      element.toolTips.content = content;
      element.onmouseover = null;
      addEventListener(element, 'mouseover', function()
        {
          show(toolTips);
//too slow
/*  				if(isIE){
						swapSelects(toolTips);
					}*/
        }, false);
      addEventListener(element, 'mouseout', function()
        {
          hide(toolTips);
//too slow
/*					if(isIE){
						swapSelects(toolTips);
					}*/
        }, false);
    };
  }, false);


/**
 * activateToolTips is a Method of HTMLElement to activate tooltips on a element
 *
 * @param content      a String, content of tooltips or indicate if content is get by HTTPRequest
 * @return             Void
 */
function activateToolTips(el,content)
{
  if(content == 'AJAX_MODE')
  {
    var url = arguments[2];
    //TODO : finish this mode
  }
  else
  {
    el.toolTipsContent = content;
    el.onmouseover = function()
    {
      toolTips.innerHTML = el.toolTipsContent;
      show(toolTips);
//too slow
/*  		if(isIE){
				swapSelects(toolTips);
			}*/
  	}
    el.onmouseover();
    el.onmouseout = function()
    {
      hide(toolTips);

//too slow
/*			if(isIE){
				swapSelects(toolTips);
			}*/
    }
    el.onmousemove = function()
    {
      toolTips.setPos(mouse.x, mouse.y);

//too slow
/*			if(isIE){
				swapSelects(toolTips);
			}*/
    };
  }
};
