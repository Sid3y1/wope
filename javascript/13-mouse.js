/*
 * Object mouse contend x and y position of cursor
 */
var mouse = {
  x : null,
  y : null,
  toString : function()
  {
    return this.x + ' ' + this.y;
  }/*,
  getCursorStyle : function()
  {
    
  };*/
};

//initialize capture of x and y mouse pos
addEventListener(document, 'mousemove',function(e){
	mouse.x = e.pageX ? e.pageX : e.clientX + document.documentElement.scrollLeft;
	mouse.y = e.pageY ? e.pageY : e.clientY + document.documentElement.scrollTop;
}, false);
