/**
*		Copyright	: (C) 2006 Geniutt SARL
*		Licence		: All Rights Reserved
*		Email     : contact@geniutt.fr
*		Version		: 8.0
*
*		Notes : Contrairement aux autres parties du site, cette partie du code
*		source n'est pas libre. Elle est la propriétée de la SARL Geniutt, toute
*		utilisation, copie, distribution, modification est strictement interdite
*		sans autorisation contraire.
*
**/

var mainMenu = document.getElementById('mainMenu');

mainMenu.center = function()
{
  mainMenu.style.marginLeft = (this.parentNode.offsetWidth - this.offsetWidth) /2 + 'px';
  /*@cc_on @*/
  /*@if (@_win32)
  mainMenu.style.marginLeft = (this.parentNode.offsetWidth - this.offsetWidth) /4 + 'px';
  /*@end @*/
};
mainMenu.center();
mainMenu.img = new Array();
mainMenu.over = null;
var element = mainMenu.childNodes[0], img, num = 0;
while(element)
{
  img = element.childNodes[0].childNodes[0];// LI > A > IMG

  img.onmouseover = function()
  {
		mainMenu.over = this;
    mainMenu.calculate();
  };
  /*
  img.onmouseout = function()
  {
    mainMenu.over = null;
    //mainMenu.calculate();
  };
  */
  
  /*@cc_on @*/
  /*@if (@_win32)
  img.style.filter = 'progid:DXImageTransform.Microsoft.AlphaImageLoader(src=\'' + img.src +'\', sizingMethod=\'scale\')';
  img.src = img.src.slice(0, img.src.lastIndexOf('/') +1) + 'blank.png';
  /*@end @*/
  mainMenu.img[num] = img;
  element = element.nextSibling;
  num++;
}

mainMenu.conf = {
  maxSize : 100,
  minSize : 60
}
mainMenu.conf.diffSize = mainMenu.conf.maxSize - mainMenu.conf.minSize;

//Preload images

mainMenu.calculate = function()
{
  if(!isNull(this.over))
  {
    elementIndex = this.img.indexOf(this.over);
    var curElement, curSize, curIndex, minSize = this.conf.minSize, maxSize = this.conf.maxSize, diffSize = this.conf.diffSize;
    
    curElement = this.img[elementIndex];
    
    // [-1 1]
    var curPosImage = -(curElement.offsetLeft + curElement.offsetWidth /2 - mouse.x) / curElement.offsetWidth *2;
    curPosImage = curPosImage > 1 ? 1 : (curPosImage < -1 ? -1 : curPosImage);
    //this.reset();
    
    //-3
    curIndex = elementIndex -3;
    if(0 <= curIndex && curIndex < this.img.length)
    {
      curElement = this.img[curIndex];
      curSize = parseInt(minSize + 0.07 * diffSize - 0.07 * diffSize * curPosImage);
      curElement.style.marginTop = '-' + parseInt(curSize - minSize) + 'px';
      mainMenu.setSize(curElement, curSize);
      mainMenu.setBigImage(curElement);
    }
    
    //-2
    curIndex = elementIndex -2;
    if(0 <= curIndex && curIndex < this.img.length)
    {
      curElement = this.img[curIndex];
      curSize = parseInt(minSize + 0.28 * diffSize - 0.14 * diffSize * curPosImage);
      curElement.style.marginTop = '-' + parseInt(curSize - minSize) + 'px';
      mainMenu.setSize(curElement, curSize);
      mainMenu.setBigImage(curElement);
    }
    
    // -1
    curIndex = elementIndex -1;
    if(0 <= curIndex && curIndex < this.img.length)
    {
      curElement = this.img[curIndex];
      curSize = parseInt(minSize + 0.71 * diffSize - 0.28 * diffSize * curPosImage);
      curElement.style.marginTop = '-' + parseInt(curSize - minSize) + 'px';
      mainMenu.setSize(curElement, curSize);
      mainMenu.setBigImage(curElement);
    }
    
    //0
    curElement = this.img[elementIndex];
    curSize = maxSize;
    curElement.style.marginTop = '-' + parseInt(curSize - minSize) + 'px';
    mainMenu.setSize(curElement, maxSize);
    mainMenu.setBigImage(curElement);
    //curElement.nextSibling.innerHTML = curPosImage;
    
    // +1
    curIndex = elementIndex +1;
    if(0 <= curIndex && curIndex < this.img.length)
    {
      curElement = this.img[curIndex];
      curSize = parseInt(minSize + 0.71 * diffSize + 0.28 * diffSize * curPosImage);
      curElement.style.marginTop = '-' + parseInt(curSize - minSize) + 'px';
      mainMenu.setSize(curElement, curSize);
      mainMenu.setBigImage(curElement);
    }
    
    // +2
    curIndex = elementIndex +2;
    if(0 <= curIndex && curIndex < this.img.length)
    {
      curElement = this.img[curIndex];
      curSize = parseInt(minSize + 0.28 * diffSize + 0.14 * diffSize * curPosImage);
      curElement.style.marginTop = '-' + parseInt(curSize - minSize) + 'px';
      mainMenu.setSize(curElement, curSize);
      mainMenu.setBigImage(curElement);
    }
    
    // +3
    curIndex = elementIndex +3;
    if(0 <= curIndex && curIndex < this.img.length)
    {
      curElement = this.img[curIndex];
      curSize = parseInt(minSize + 0.07 * diffSize + 0.07 * diffSize * curPosImage);
      curElement.style.marginTop = '-' + parseInt(curSize - minSize) + 'px';
      mainMenu.setSize(curElement, curSize);
      mainMenu.setBigImage(curElement);
    }
    
    //Center
    this.center();
  }
};
mainMenu.onmouseout = function()
  {
    this.over = null;
    this.reset();
    this.center();
  };
mainMenu.lastFrame = 0;
mainMenu.onmousemove = function()
{
  var curFrame = (new Date).getTime();
  if(this.lastFrame <= curFrame - 20)//Default value : 40 (= 25fps)
  {
    this.calculate();
    this.lastFrame = curFrame;
  }
};
mainMenu.reset = function()
{
  this.img.forEach(function(element)
  {
    element.style.marginTop = '';
    mainMenu.setImage(element);
    mainMenu.setSize(element, mainMenu.conf.minSize);
  });
};

mainMenu.setBigImage = function(element)//element IMG
{
  /*@cc_on @*/
  /*@if (@_win32)
  img = element;
  element = {src : ''};
  /*@end @*/
  element.src = element.src.replace('small_', 'full_');
  /*@cc_on @*/
  /*@if (@_win32)
  img.style.filter = img.style.filter.replace('small_', 'full_');
  /*@end @*/
}
mainMenu.setImage = function(element)
{
  /*@cc_on @*/
  /*@if (@_win32)
  img = element;
  element = {src : ''};
  /*@end @*/
  element.src = element.src.replace('full_', 'small_');
  /*@cc_on @*/
  /*@if (@_win32)
  img.style.filter = img.style.filter.replace('full_', 'small_');
  /*@end @*/
}
mainMenu.setSize = function(element)
{
  element.style.height = element.style.width = arguments.length <= 2 ? arguments[1] + 'px' : '';
  /*
  if(arguments.length <= 3)
    document.body.innerHTML += arguments[1] + ' ';
  */
}
