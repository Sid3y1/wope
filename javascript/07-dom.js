/**
 * getElements|gE is a Function to get element
 *
 * @param       a DOMObject or String, (Optional)
 * @return      a Array with all DOMObject targeted
 */
var gE =
document.getElements = function()
{
  var elements = new Array();

  for (var i = 0; i < arguments.length; i++)
  {
    var element = arguments[i];
    if (typeof element === 'string')
      element = document.getElementById(element) || null;
    if (arguments.length == 1)
    {
      if(element === null)
        return undefined;
      return element;
    }
    if(element != null)
      elements.push(element);
  }
  return elements;
};

/**
 * remove is a Method of HTMLElement to remove element
 *
 * @param       a DOMObject or String, (Optional)
 * @return      a DOMObject, return element removed
 */
function remove(el)
{
  return el.parentNode.removeChild(el);
};

/**
 * getNodeIndex is a Method of HTMLElement to remove element
 *
 * @return      a Number, return index of parent's childNodes
 */
function getNodeIndex(el)
{
   var tmp = new Number();
   for (var i = 0; i < el.parentNode.childNodes.length; i++)
   {
     if(el.parentNode.childNodes[i] == el)
       tmp = i;
   }
   return tmp
};

/**
 * accessToString is a Method of HTMLElement get path to access to the DOMObject
 * example : <code>document.childNodes[1].childNodes[0].childNodes[0]</code>
 *
 * @return      a String, return the path
 */
function accessToString(el)
{
  var tmp = new String();
  var tmpObj = el;
  while(tmpObj.parentNode != null && typeof getNodeIndex(tmpObj) != 'undefined')
  {
    tmp = '.childNodes[' + getNodeIndex(tmpObj) + ']' + tmp;
    tmpObj = tmpObj.parentNode;
  }
  return 'document' + tmp;
};

/* TODO: clear content element
HTMLElement.prototype.clear = function()
{
  
}
*/

/**
 * addClassName is a Method of HTMLElement to add a class name
 *
 * @param classNames  a String or Array
 * @return            Void
 */
function addClassName(el, classNames)
{
  var curClassName = el.className.split(' ');
  if(typeof classNames === 'string')
    classNames = classNames.split(' ');
  //TODO: Array.forEach
  for(var i = 0, l = classNames.length, index; i < l; i++)
  {
    index = curClassName.indexOf(classNames[i]);
    if(index == -1 && !isNull(classNames[i]))
      curClassName.push(classNames[i]);
  }
  el.className = curClassName.join(' ');
};

/**
 * removeClassName is a Method of HTMLElement to remove a class name
 *
 * @param classNames  a String or Array
 * @return            Void
 */
function removeClassName(el, classNames)
{
  var curClassName = el.className.split(' ');
  if(typeof classNames === 'string')
    classNames = classNames.split(' ');
  //TODO: Array.forEach
  for(var i = 0, l = classNames.length, index; i < l; i++)
  {
    index = curClassName.indexOf(classNames[i]);
    if(index != -1)
      curClassName.remove(index);
  }
  el.className = curClassName.join(' ');
};

/*
 * isClassName checks if some classes are associated to an element
 *
 * @param  HTMLElement   el  the element to check
 * @param  String|Array  classNames  the classes to check for
 *
 * @return boolean
 */
function isClassName(el, classNames){
	var curClassName = el.className.split(' ');
	if(typeof classNames === 'string'){
	  classNames = classNames.split(' ');
	}

	for(var i = 0, l = classNames.length, index; i < l; i++){
		index = curClassName.indexOf(classNames[i]);
		if(index == -1){
		  return false;
		}
	}
	
	return true;
			
};


/*
 * replaceClassName replaces a class associated to an element (if existing) by another
 *
 * @param  HTMLElement   el  the element to check
 * @param  String  class1  the class to replace
 * @param  String  class2  the new class
 *
 * @return boolean
 */
function replaceClassName(el, class1, class2){
	var isClass = isClassName(el, class1);
	
	if(isClass){
		removeClassName(el, class1);
		addClassName(el, class2);
	}
	
	return isClass;
}


/*
 * switchClassName switches a class associated to an element between two possible values
 *
 * @param  HTMLElement   el  the element to check
 * @param  String  class1
 * @param  String  class2
 *
 * @return new class name
 */
function switchClassName(el, class1, class2){
	var newClass;
	
	if(isClassName(el, class1)){
		removeClassName(el, class1);
    addClassName(el, class2);
		newClass = class2;
	}else if(isClassName(el, class2)){
		removeClassName(el, class2);
    addClassName(el, class1);
		newClass = class1;
	}else if(class2 == ''){
		addClassName(el, class1);
		newClass = class1;
	}else{
		newClass = false;
	}

	return newClass;
}

//TODO: with CSS property : visiblity


/*
 * getX returns element X position (takes account of scrollbars)
 */
function getX(element){
	var x = 0;
	for(var e = element; e; e = e.offsetParent){
		x += e.offsetLeft;
	}

	for(e = element.parentNode; e && e != document.body; e = e.parentNode){
		if(e.scrollLeft){
			x -= e.scrollLeft;
		}
	}

	return x;
}

/*
 * getY returns element Y position (takes account of scrollbars)
 */
function getY(element){
	var y = 0;
	for(var e = element; e; e = e.offsetParent){
		y += e.offsetTop;
	}

	for(e = element.parentNode; e && e != document.body; e = e.parentNode){
		if(e.scrollTop){
			y -= e.scrollTop;
		}
	}

	return y;
}

function getViewportWidth(){
	return document.documentElement.clientWidth;
	//return document.documentElement.clientWidth || document.body.clientWidth || self.innerWidth;
}

function getViewportHeight(){
	return document.documentElement.clientHeight;
	//return document.documentElement.clientHeight || document.body.clientHeight || self.innerHeight;
}

