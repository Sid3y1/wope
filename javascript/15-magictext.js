		//\r\n is Windows
		//\r is Mac
		//\n is Unix
		function convertTextToHTML(s){
			return s.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/\r\n/g,"<br />").replace(/\n/g,"<br />").replace(/\r/g,"<br />").replace(/  /g,"&nbsp; ");
		}
	
		//<BR> is IE
		//<BR/> is Opera
		//<br> is Firefox
		function convertHTMLToText(s){
			return s.replace(/&amp;/g,"&").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/<br>/g,"\n").replace(/<br \/>/g,"\n").replace(/<BR>/g,"\n").replace(/<BR \/>/g,"\n").replace(/<br\/>/g,"\n").replace(/<BR\/>/g,"\n").replace(/&nbsp;/g," ");
		}

		function countLines(s){
			return s.split("<br>").length + s.split("<br />").length + s.split("<BR>").length + s.split("<BR />").length + s.split("<br/>").length + s.split("<BR/>").length;
		}

		function countLinesArea(s){
			var linesRN = s.split("\r\n").length;
			//linesRN has already counted \n and \r which are part of \r\n
			var linesN = s.split("\n").length - linesRN;
			var linesR = s.split("\r").length - linesRN;
			var lines = linesRN + linesN + linesR;
			return lines;
		}


/**
 * edit is a method of HTMLElements to transform a Text to a text input (tag INPUT or TEXTAREA)
 * and when blur input retransform to Text and save the modification in hidden input.
 *
 * @param el           object   element to edit
 * @param inputName    string   name of hidden input use
 * @param limitLength  integer  length before displaying a textarea (Optional)
 * @param maxLength    integer  max length of text (Optionnal)
 * @return             void
 * @need               isNull()
 * @need               String#trim()
 */
function edit(el, inputName, limitLength, maxLength)
{  
  limitLength = limitLength || 30;
  var text = el.innerHTML, input,
  size = el.innerHTML.length,
	parent = el.parentNode;
  if(size <= limitLength)
  {
    var input = document.createElement('INPUT');
    input.type = 'text';
    input.size = size;
		if(maxLength && !isNaN(maxLength)){
			input.maxLength = maxLength;
		}
    input.onkeydown = function()
    {
      input.size = input.value.length +1;
    };
    input.onkeyup = function()
    {
      input.size = input.value.length;
    };
  }
  else
    var input = document.createElement('TEXTAREA');
  input.className = 'magicText';
  input.onblur = (function()
    {
      return function()
      {
        if(!isNull(input.value.trim()) && input.value != text)
        {
          if(typeof el.form[inputName] == 'undefined')
          {
            var input = document.createElement('INPUT');
            input.name = inputName;
            input.type = 'hidden';
            el.form.appendChild(input);
          }
          else
            var input = el.form[inputName];
          	input.value = el.value;
        }
        parent.replaceChild(el, input);
				el.innerHTML = convertTextToHTML(input.value);
      }
    }
    )();
  input.value = convertHTMLToText(el.innerHTML);
  el.parentNode.replaceChild(input, el);
  input.focus();
};



/**
 * like edit, editAndSend is a method of HTMLElements to transform a Text to a text input (tag INPUT or TEXTAREA)
 * and when blur input retransform to Text. Dont save the modification, but send (in POST method) to specified location (the url).
 *
 * @param el           element to edit
 * @param url          a String or URL, location to send datas
 * @param inputName    a String, name of hidden input use
 * @param drawResult   a Boolean, if draw result (in dialBox) (Optional)
 * @param limitLength  a Number, length of limit (Optional)
 * @param maxLength    integer  max length of text (Optionnal)
 * @return             void
 * @see                #edit()
 * @need               isNull()
 * @need               AJAX#makeHTTPTransaction()
 * @need               String#trim()
 */
function editAndSend(el, url, inputName, drawResult, limitLength, maxLength)
{  
  limitLength = limitLength || 30;
  var text = el.innerHTML, 
	input,
  size = el.innerHTML.length,
	parent = el.parentNode;
  if(size <= limitLength)
  {
    var input = document.createElement('INPUT');
    input.type = 'text';
    input.size = size;
		if(maxLength && !isNaN(maxLength)){
			input.maxLength = maxLength;
		}
    input.onkeydown = function()
    {
      input.size = input.value.length +1;
    };
    input.onkeyup = function()
    {
      input.size = input.value.length;
    };
  }
  else{
    var input = document.createElement('TEXTAREA');
	}
  input.className = 'magicText';
  input.onblur = (function()
    {
      return function()
      {
        parent.replaceChild(el, input);
        if(!isNull(input.value.trim()) && input.value != text)
        {
          if(!drawResult){
            AJAX.makeHTTPTransaction({
                url : url,
                datas : inputName + '=' + input.value,
                asyncMode : false,
                httpMode : 'POST'
              });
					}else{
            AJAX.makeHTTPTransaction({
                url : url,
                datas : inputName + '=' + input.value,
                httpMode : 'POST',
                onsuccess : function(){dialBox.draw(this.responseText)}
              });
					}
          el.innerHTML = convertTextToHTML(input.value);
        }
      }
    }
    )();
  input.value = convertHTMLToText(el.innerHTML);
  el.parentNode.replaceChild(input, el);
  input.focus();
};
