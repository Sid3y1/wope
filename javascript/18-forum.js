function putSmiley(oInput, icon){
  oInput.value += " " + icon + " ";
}

function quoteMessage(url, id_input, goto_reply){
	var text_input = gE(id_input);
	var message = '';
	
	var handleFailure = function(o){
			if(o.responseText != undefined){
				alert(o.responseText);
			}
	}

	var handleSuccess = function(o){
		if(o.responseText != undefined){
			message = o.responseText;
			if(goto_reply){
				window.location.href = window.location.href.replace(/&page=[0-9]*/, "").replace(/#.*/, "") + '#reply';
			}
			if(text_input){
				text_input.value = message;
			}
		}
	}

	var callback ={
  	success: handleSuccess,
		failure: handleFailure
	}
	
	var con = YAHOO.util.Connect.asyncRequest('GET', url, callback);
	
}

function removeSlashes(text){
	return text.replace(/\\/g, "\\").replace(/\'/g, "'").replace(/\"/g, '"');
}

function applyBBStyle(oInput, start_tag, end_tag, msg, val){
  var text_input = oInput;
	scrolling = false;
  if(text_input){
		if(text_input.scrollTop){
			scrolling = true;
			scrollLevel = text_input.scrollTop;
		}
		//focus on text_input
    text_input.focus();
    if(typeof text_input.selectionStart != "undefined"){
      //cursor position
      var start_pos = text_input.selectionStart;
      var end_pos = text_input.selectionEnd;
      
      var str = text_input.value;
      var start_str = str.substring( 0 , start_pos);
      var end_str = str.substring( end_pos, text_input.textLength );
			var selected_text = str.substring( start_pos, end_pos);
			var sup_length = 0;
		 	if(selected_text == ''){
				if(val != undefined){
					start_tag = start_tag + '=' + val;
					sup_length = val.length +1 ;
				}
				selected_text = msg;
			}
      //insert new text
      text_input.value = start_str + '[' + start_tag + ']' + selected_text + '[' + end_tag + ']' + end_str;
      //restore cursor and focus
      text_input.setSelectionRange(start_str.length + start_tag.length + selected_text.length + end_tag.length + 4 + sup_length, start_str.length + start_tag.length + selected_text.length + end_tag.length + 4 + sup_length );
      text_input.focus();

			if(scrolling){
				text_input.scrollTop = scrollLevel;
			}
    }
		//IE
    else if(document.selection){
			var selected_text = window.document.selection.createRange().text;
			//if text is selected
			if( selected_text.length > 0){
				var str = document.selection.createRange();
				str.text = '[' + start_tag + ']' + selected_text + '[' + end_tag + ']';
				str.collapse();
				str.select();
			}
			else{
				var str = text_input.value;
				var str_point = "#~#~#";
				//insert a mark where cursor is
				var tmp = document.selection.createRange().duplicate();
				tmp.text = str_point;
				//search of point position
				var start_pos = text_input.value.search(str_point);
				var start_str = str.substring( 0 , start_pos);
				var end_str = str.substring(start_pos, text_input.textLength);
				//insert text
				var sup_length = 0;
				if(val != undefined){
				  start_tag = start_tag + '=' + val;
				  sup_length = val.length +1 ;
				}
																		
				text_input.value = start_str + '[' + start_tag + ']' + msg + '[' + end_tag + ']' + end_str;
				//place cursor after new text
				start_pos += msg.length;
				str = text_input.createTextRange();
				str.moveStart("character", start_pos + start_tag.length + sup_length + msg.length + end_tag.length + 4);
				str.collapse(true);
				str.select();

      	text_input.focus();
				
				if(scrolling){
					text_input.scrollTop = scrollLevel;
				}
			}
		}
	}
}

function increaseAreaSize(oInput, max, step){
	if(oInput.rows < max){
		oInput.rows += step;
	}
}

function decreaseAreaSize(oInput, min, step){
	if(oInput.rows > min){
		oInput.rows -= step;
	}
}

