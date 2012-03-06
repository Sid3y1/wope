/**
	* Object NEWMAIL contend methods to make a tooltip appearing at the bottom
	*/
var NEWMAIL = {

  activate : function(message){
	
		if(message != undefined && message != ''){
	  
			if(document.getElementById('NEWMAIL')){//gE() bug avec IE
  	  	var newmail = gE('NEWMAIL');
			}else{
				newmail = document.body.appendChild(document.createElement('DIV'));
			}
		
			newmail.innerHTML = message;
		
			show(newmail);
			
			newmail.style.top =  getViewportHeight() - newmail.offsetHeight + 'px';
			newmail.style.left =  getViewportWidth() - newmail.offsetWidth + 'px';
			
			button_left = newmail.offsetWidth - 20;
			
			newmail.innerHTML = '<div><a onclick="NEWMAIL.hide();" style="position:absolute;left:' + button_left + 'px;" class="close">&nbsp;</a></div>' + newmail.innerHTML;
		}
	},

	hide : function(){
		newmail = gE('NEWMAIL');
		hide(newmail);
	}

}

/**
 * Open a popup window.
 *
 * @param string $url               The URL to open in the popup window.
 * @param optional integer $width   The width of the popup window. (Default:
 *                                  600 px)
 * @param optional integer $height  The height of the popup window. (Default:
 *                                  500 px)
 * @param optional string $args     Any additional args to pass to the script.
 *                                  (Default: no args)
 */
function popup_imp(url, width, height, args)
{
    if (!width) {
        width = 600;
    }
    var screen_width = screen.width;
    if (width > (screen_width - 75)) {
        width = screen_width - 75;
    }

    if (!height) {
        height = 500;
    }
    var screen_width = screen.width;
    if (width > (screen_width - 75)) {
        width = screen_width - 75;
    }

    var now = new Date();
    var name = now.getTime();

    if (url.indexOf('?') == -1) {
        var glue = '?';
    } else {
        var glue = '&';
    }

    if (args != "") {
        url = url + glue + args + "&uniq=" + name;
    } else {
        url = url + glue + "uniq=" + name;
    }

    param = "toolbar=no,location=no,status=yes,scrollbars=yes,resizable=yes,width=" + width + ",height=" + height + ",left=0,top=0";
    win = window.open(url, name, param);
    if (!win) {
        alert("The image save window can not be opened. Perhaps you have set your browser to block popup windows ?");
    } else {
        if (!eval("win.opener")) {
            win.opener = self;
        }
        win.focus();
    }
}
