//moved to javascript/15-magictext.js
/*
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
*/

	/**
 	* like edit or editAndSend, editAndSaveNotes is a method of HTMLElements to transform a Text to a text input (TEXTAREA)
 	* and when blur input retransform to Text. Dont save the modification, but send (in POST method) to specified location (the url).
 	*
 	* @param url          a String or URL, location to send datas
 	* @param inputName    a String, name of hidden input use
 	* @param limitLines   a Number, limit number of rows
 	* @return             void
 	* @see                #edit()
 	* @need               AJAX#makeHTTPTransaction()
 	*/
 
	function editAndSaveNotes(el, url, inputName, limitLines){
	  var text = el.innerHTML, input,
		size = el.innerHTML.length;
		var input = document.createElement('TEXTAREA');
		var nb_lines = countLines(el.innerHTML);
		
		var explaination = document.createElement('P');
		
		el.parentNode.insertBefore(explaination, el);
		if (text != '' && text == 'Cliquez sur ce texte pour le modifier') {
			explaination.innerHTML = 'Cliquez en dehors du champ de texte pour le sauvegarder';
		}
		
		//on ajuste le textarea au texte en tenant compte de la limite
		if(nb_lines <= limitLines){
			input.rows = nb_lines;
		}
		else{
			input.rows = limitLines;
		}
  	input.className = 'magicText';
		input.onkeyup = (function(){
			return function(){
				var nb = countLinesArea(input.value);
				if((nb <= limitLines) && (nb>input.rows)){
					input.rows = nb;
				}
			}
		})();
	
		//quand on quitte le textarea
  	input.onblur = (function(){
      return function(){
        if(!isNull(input.value.trim()) && input.value != text)
        {
          AJAX.makeHTTPTransaction({
                url : url,
                datas : inputName + '=' + encodeURIComponent(input.value),
                asyncMode : false,
                httpMode : 'POST'
              });
          el.innerHTML = convertTextToHTML(input.value);
        }
				
				//si plus aucun texte
				if(isNull(input.value)){
					var msg = 'Cliquez sur ce texte pour le modifier';
  				AJAX.makeHTTPTransaction({
                url : url,
                datas : inputName + '=' + msg,
                asyncMode : false,
                httpMode : 'POST'
              });
          el.innerHTML = msg;
				}
				this.parentNode.removeChild(explaination);
				this.parentNode.replaceChild(el,this);
      }
		})();
	  input.value = convertHTMLToText(el.innerHTML);
  	el.parentNode.replaceChild(input, el);
  	input.focus();
	}

