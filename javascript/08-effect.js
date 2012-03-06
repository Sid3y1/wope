/**
 * display is a Method of HTMLElement to hide|show element
 *
 * @param params       a Object, content optional parameters
 * @return             a Boolean, <code>true</code> if element is visible else <code>false</code>
 * @need               HTMLElement#show()
 * @need               HTMLElement#hide()
 */
function display(el,params)
{
  params = params || new Object();
  var displayMode = params.displayMode || '',
  onshow = params.onshow || function(){},
  onhide = params.onhide || function(){};
  if(window.getComputedStyle(el, '').display == 'none')
  {
    show(el,displayMode);
    onshow();
    return true;
  }
  else
  {
    hide(el);    
    onhide();
  }
  return false;
};

/**
 * show is a Method of HTMLElement to show element
 *
 * @param displayMode  a String, CSS value to force the mode of display (Optional)
 * @return             Void
 */
function show(el, displayMode)
{
  displayMode = displayMode || '';
  el.style.display = displayMode;
};

/**
 * show is a Method of HTMLElement to show element
 *
 * @return             Void
 */
function hide(el)
{
  el.style.display = 'none';
};
