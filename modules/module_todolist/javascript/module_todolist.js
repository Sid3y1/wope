		//\r\n is Windows
		//\r is Mac
		//\n is Unix
		function convertTextToHTML(s){
			s = s.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/\r\n/g,"<br />").replace(/\n/g,"<br />").replace(/\r/g,"<br />").replace(/  /g,"&nbsp; ");
			return s;
		}
	
		//<BR> is IE
		//<BR/> is Opera
		//<br> is Firefox
		function convertHTMLToText(s){
			s = s.replace(/&amp;/g,"&").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/<br>/g,"\n").replace(/<br \/>/g,"\n").replace(/<BR>/g,"\n").replace(/<BR \/>/g,"\n").replace(/<br\/>/g,"\n").replace(/<BR\/>/g,"\n").replace(/&nbsp;/g," ");
			return s;
		}
	
		function countLines(s){
			var lines = s.split("<br>").length + s.split("<br />").length + s.split("<BR>").length + s.split("<BR />").length + s.split("<br/>").length + s.split("<BR/>").length;
			return lines;
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
 * editAndSaveTarget is a method of HTMLElements to transform a Text to a text input (TEXTAREA), containing the content of the target element
 * 
 * @param source       a HTML element containing the source of the input
 * @param url          a String or URL, location to send datas
 * @param inputName    a String, name of hidden input use
 * @param limitLines   a Number, limit number of rows
 * @return             void
 * @see                #edit()
 * @need               isNull()
 * @need               AJAX#makeHTTPTransaction()
 */
 
editAndSaveTarget = function(source, type, el, url, inputName, limitLines, callback)
{
  var text = source.innerHTML, input,
	size = source.innerHTML.length,
	parent = el.parentNode;
  var input = document.createElement('INPUT');
	var nb_lines = countLines(source.innerHTML);
	//on ajuste le textarea au texte en tenant compte de la limite
	if(nb_lines <= limitLines){
		input.rows = nb_lines;
	}
	else{
		input.rows = limitLines;
	}
  input.className = 'magicText';
	input.onkeyup = (function(){
		return function()
		{
			var nb = countLinesArea(input.value);
			if((nb <= limitLines) && (nb>input.rows)){
				input.rows = nb;
			}
		}
	}
	)();
	//quand on quitte le textarea
  input.onblur = (function()
    {
      return function()
      {
				parent.replaceChild(el,input);
        if(!isNull(input.value.trim()) && input.value != text)
        {
					if (type == 'text') {
	          AJAX.makeHTTPTransaction({
              url : url,
 	            datas : inputName + '=' + encodeURIComponent(convertTextToHTML(input.value)) ,
   	          asyncMode : false,
     	        httpMode : 'POST'
      			});
					} 
					
					if (type == 'date') {
	          params = {
							callback : (callback) ? callback : function(){;},
 		          url : url,
     		      datas : inputName + '=' + encodeURIComponent(input.value),
         		  asyncMode : false,
         			httpMode : 'POST'
   					};
						AJAX.makeHTTPTransaction(params);
					}
        
					if ((convertTextToHTML(input.value)).length > 60) {
						var end = '...';
					} else {
							var end = '';
						}

          el.innerHTML = (convertTextToHTML(input.value)).substring(0,55) + end;
					source.innerHTML = convertTextToHTML(input.value);
					
        }
				//si plus aucun texte
				if(isNull(input.value)){
					var msg = '';
  				AJAX.makeHTTPTransaction({
                url : url,
                datas : inputName + '=' + msg,
                asyncMode : false,
                httpMode : 'POST'
              });
          el.innerHTML = msg;
				}
      }
		}
    )();
  input.value = convertHTMLToText(source.innerHTML);
  el.parentNode.replaceChild(input, el);
  input.focus();
};
