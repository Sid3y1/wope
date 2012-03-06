/**
 * Object AJAX contends method to make HTTPTransactions
 */
//TODO: support XML, JSON
var AJAX = {
  /**
   * Method makeHTTPTransaction to initialise a new HTTPRequest and send it with values defined in parameters
   * @params params  An Object, with each properties are content value : 
   *                 url, datas, httpMode, headers, asyncMode, user, pass, event
   *                 Only url is required.
   * @return         Void
   * @need           isNull()
   */
  makeHTTPTransaction : function(params)
  {
		if(params.hud === true){
	    HUD.activate('Chargement ...', 'loading');
		}
    if(params.url)
      var url = params.url;
    else
      return;
    var datas = params.datas || '',
    httpMode = params.httpMode || 'GET';
    if(httpMode == 'GET')
    {
      var curUrl = url.split('?');
      url = curUrl[0];
      if(!isNull(datas) && curUrl.length >= 2)
        datas = curUrl[1] + '&' + datas;
      else if(curUrl.length >= 2)
      {
        datas = curUrl[1];
      }
      url = new URL(url, datas);
    }
    //TODO: accept customs header
    //var headers = params.headers || new Array();    
    var asyncMode = params.asyncMode || true;// true : wait server response    
    var request = new XMLHttpRequest();
    request.onreadystatechange = (function()
    {
      return function()
      {
        (function()
        {
          switch(this.readyState)
          {
            /*
            0 = non initialisé ;
            1 = ouverture. La méthode open() a été appelée avec succès ;
            2 = envoyé. La méthode send() a été appelée avec succès ;
            3 = en train de recevoir. Des données sont en train d'être transférées, mais le transfert n'est pas terminé ;
            4 = terminé. Les données sont chargées.
            */
            case 0 : //Uninitialized
              if(typeof params.onuninitialize != 'undefined')
                params.onuninitialize.apply(this);
              break;
            case 1 : //Open
              if(typeof params.onopen != 'undefined')
                params.onopen.apply(this);
              break;
            case 2 : //Sent
              if(typeof params.onsend != 'undefined')
                params.onsend.apply(this);
              break;
            case 3 : //Receiving
              if(typeof params.onreceive != 'undefined')
                params.onreceive.apply(this);
              break;
            case 4 : //Loaded
              if(typeof params.onload != 'undefined')
                params.onload.apply(this);
							/*2xx Success // Status == 0 if it's local*/
              if(this.status == 0 || this.status >= 200 && this.status < 300 && typeof params.onsuccess != 'undefined'){
								params.onsuccess.apply(this);
							}
							/*3xx Redirection, 4xx Client Error, 5xx Server Error*/
              if(this.status >= 300){
								if(typeof params.onfailure != 'undefined'){
									params.onfailure.apply(this);
								}else{
									switch(this.status){
										case 401:
											dialBox.draw('<p>Vous n\'avez pas les droits requis pour acc&eacute;der &agrave; la page demand&eacute;e. Peut-&ecirc;tre &ecirc;tes-vous d&eacute;connect&eacute;.</p><form action=""><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + this.status, false);
											break;
										/*other errors are here for debugging purpose*/
										case 400:
											dialBox.draw('<p>Erreur de syntaxe dans l\'adresse du document.</p><form onsubmit="dialBox.erase();return false;"><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + this.status, false);
											break;
										case 404:
											dialBox.draw('<p>La page demand&eacute;e n\'existe pas.</p><form onsubmit="dialBox.erase();return false;"><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + this.status, false);
											break;
										case 414:
											dialBox.draw('<p>L\'URL de la requ&ecirc;te est trop longue.</p><form onsubmit="dialBox.erase();return false;"><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + this.status, false);
											break;
										default:
											dialBox.draw('<p>Une erreur est survenue, l\'action &agrave; &eacute;chou&eacute;.</p><form onsubmit="dialBox.erase();return false;"><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + this.status, false);
											break;
									}
								}
							}
							if(params.hud === true){
	              hide(HUD);
							}
							break;
						default:
							//void
							break;
          }
        }).apply(request);
      }
    })();
    
    var user = params.user || null,
    pass = params.pass || null;
    if(isNull(user) && isNull(pass))
      request.open(httpMode, url, asyncMode);
    else
      request.open(httpMode, url, asyncMode, user, pass);
    // Opération de transmision des données en fonction du mode HTTP ['GET'|'POST'|...]
    //TODO: support adding headers
    switch(httpMode)
    {
      case 'get':
  		case 'GET':
        request.send(null);
        break;
  		case 'post':
  		case 'POST':
        request.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
        request.send(datas);
  		  break;
      default:
      //case 'PUT':
      //case 'DELETE':
  		//case 'HEAD':
        break;
  	}
    return;
  },
	
	
  /**
   * getAndCall  makes an HTTP transaction and 
	 *   calls the "callback" function when request is complete
	 *
   * @params url      a String or URL, the location where to go
   * @param callback  a Function or FuncLink, the Function called when request is complete
   *                  and status is in (200 to 299)(onsuccess)
	 * @param hud				a Boolean, activate HUD or not
   * @return          Void
   * @need            #makeHTTPTransaction()
   */
  getAndCall : function(url, callback, hud)
  {
		hud = hud === false ? false : true;
		if(!callback){
    	this.makeHTTPTransaction({
      	url : url,
				hud : hud
    	});
		}else{
    	this.makeHTTPTransaction({
      	onsuccess : callback,
      	url : url,
				hud : hud
    	});
		}
  },
	
  
  /**
   * getAndUpdate  makes an HTTP transaction and 
	 *   refreshes the content of an element with the response
	 *
   * @params url      a String or URL, the location for make HTTP transaction
   * @param element   a DOMObject or String, the element or id of element will be update
	 * @param hud				a Boolean, activate HUD or not
   * @return          Void
   * @need            #getAndCall()
   * @need            gE() | document.getElement()
   */
  getAndUpdate : function(url, element, hud)
  {
		hud = hud === false ? false : true;
    var callback = function()
    {
      arguments.callee.element.innerHTML = this.responseText;
    };
    callback.element = gE(element);
    this.getAndCall(url, callback, hud);
  },
  
	
  /**
   * getAndEval  makes an HTTP transaction and evaluates the response
	 *
   * @params url      a String or URL, the location for make HTTP transaction
	 * @param hud				a Boolean, activate HUD or not
   * @return          void
   * @need            #getAndCall()
   */
  getAndEval : function(url, hud)
  {
		hud = hud === false ? false : true;
    this.getAndCall(url, function(){eval(this.responseText)}, hud);
  },
	
	
 /**
   * getAndEvalUpdate  makes an HTTP transaction,
	 *   refreshes the content of an element with the response
	 *   and evaluates javascript present in the response
	 *
   * @params url      a String or URL, the location for make HTTP transaction
   * @param element   a DOMObject or String, the element or id of element will be update
	 * @param hud				a Boolean, activate HUD or not
   * @return          void
   * @need            #getAndCall()
   * @need            gE() | document.getElement()
   */
  getAndEvalUpdate : function(url, element, hud)
  {
		hud = hud === false ? false : true;
		
    var callback = function()
    {
			var el = arguments.callee.element;
			
      el.innerHTML = this.responseText;

			var all = el.getElementsByTagName("*");
			var al = all.length;
			for (var i = 0; i < al; i++) {
			  all[i].id = all[i].getAttribute("id");
			  all[i].name = all[i].getAttribute("name");
				// LET THE NEXT LINE COMMENTED : IT DESTROYS THE STYLE OF THE PAGE
			  //all[i].className = all[i].getAttribute("class");
			}
			
			var allscript = el.getElementsByTagName("script");
			var asl = allscript.length;
			for(var i = 0; i < asl; i++){
				window.eval(allscript[i].text);
			}

    };
		
    callback.element = gE(element);
    this.getAndCall(url, callback, hud);
  },


	/**
		* treatErrors  treats the possible errors of an HTTP transaction
		* 
		* @param  o  a response object from YAHOO! HTTP transaction
		*/
	treatErrors : function(o)
	{
  		if(o.status !== undefined){
				switch(o.status){
					case 401:
						dialBox.draw('<p>Vous n\'avez pas les droits requis pour acc&eacute;der &agrave; la page demand&eacute;e. Peut-&ecirc;tre &ecirc;tes-vous d&eacute;connect&eacute;.</p><form action=""><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + o.status, false);
						break;
					/*other errors are here for debugging purpose*/
					case 400:
						dialBox.draw('<p>Erreur de syntaxe dans l\'adresse du document.</p><form onsubmit="dialBox.erase();return false;"><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + o.status, false);
						break;
					case 404:
						dialBox.draw('<p>La page demand&eacute;e n\'existe pas.</p><form onsubmit="dialBox.erase();return false;"><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + o.status, false);
						break;
					case 414:
						dialBox.draw('<p>L\'URL de la requ&ecirc;te est trop longue.</p><form onsubmit="dialBox.erase();return false;"><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + o.status, false);
						break;
					default:
						dialBox.draw('<p>Une erreur est survenue, l\'action &agrave; &eacute;chou&eacute;.</p><form onsubmit="dialBox.erase();return false;"><p align="center"><input type="submit" value="OK" /></p></form>', 'Error ' + o.status, false);
						break;
				}
			}
	},
	

	/**
	* submitAndCall() send form datas with ajax and call a function back
	* @param  form        form object
	* @param  is_file        file upload (boolean)
	* @param  callback    callback function
	* @return void  
	**/
	submitAndCall : function(form, is_file, callback, hud)
	{
		hud = hud === false ? false : true;

		if(hud){
	    HUD.activate('Chargement ...', 'loading');
		}
		
		var handleSuccess =  function(o){
			if(callback != undefined){
				callback.call(o);
			}
			if(hud){
				hide(HUD);
			}
		}

		var handleFailure = function(o){
  		if(o.status !== undefined){
			  this.treatErrors(o);
			}
			if(hud){
				hide(HUD);
			}
		}

		var handleUpload = function(o){
			//Since file uploading occurs through an iframe, 
			//traditional response data such as HTTP status codes 
			//are not directly available, and connection manager
			//cannot accurately discern success or failure.
			//Instead, the callback's upload handler will receive
			//a response object containing the body of the iframe 
			//document, as a string, when the transaction is complete.
	  	if(o.responseText !== undefined && callback != undefined){
				callback.call(o);
			}
			if(hud){
				hide(HUD);
			}
		}

		var sendCallback ={ 
			success: handleSuccess,
	  	failure: handleFailure, 
			upload: handleUpload
		};
		
		YAHOO.util.Connect.setForm(form, is_file);
		YAHOO.util.Connect.asyncRequest('POST', form.action, sendCallback);
	},

	/**
	* submitAndUpdate() send form datas with ajax and update a HTMLElement content back
	* @param  form        form object
	* @param  is_file        file upload (boolean)
	* @param  element    HTMLElement to update
	* @return void  
	**/
	submitAndUpdate : function(form, is_file, element, hud)
	{
		hud = hud === false ? false : true;
		
		if(hud){
	    HUD.activate('Chargement ...', 'loading');
		}
		
    var el = gE(element);

		var handleSuccess =  function(o){
			if(o.responseText !== undefined){
      	el.innerHTML = o.responseText;
			}
			if(hud){
				hide(HUD);
			}
		}
		
		var handleFailure = function(o){
  		if(o.status !== undefined){
				this.treatErrors(o);
			}
			if(hud){
				hide(HUD);
			}
		}

		var sendCallback ={ 
			success: handleSuccess,
	  	failure: handleFailure, 
			upload: handleSuccess
		};

		YAHOO.util.Connect.setForm(form, is_file);
		YAHOO.util.Connect.asyncRequest('POST', form.action, sendCallback);

	},

	/**
	* submitAndEvalUpdate() sends form datas with ajax, updates a HTMLElement content back and executes its JS code
	* @param  form        form object
	* @param  is_file        file upload (boolean)
	* @param  element    HTMLElement to update
	* @return void  
	**/
	submitAndEvalUpdate : function(form, is_file, element, hud)
	{
		hud = hud === false ? false : true;
		
		if(hud){
	    HUD.activate('Chargement ...', 'loading');
		}
		
    var el = gE(element);

		var handleSuccess =  function(o){
			if(o.responseText !== undefined){
      	el.innerHTML = o.responseText;

				var all = el.getElementsByTagName("*");
				var al = all.length;
				for (var i = 0; i < al; i++) {
				  all[i].id = all[i].getAttribute("id");
				  all[i].name = all[i].getAttribute("name");
				  all[i].className = all[i].getAttribute("class");
				}
				
				var allscript = el.getElementsByTagName("script");
				var asl = allscript.length;
				for(var i = 0; i < asl; i++){
					window.eval(allscript[i].text);
				}

			}
			
			if(hud){
				hide(HUD);
			}
		}
		
		var handleFailure = function(o){
  		if(o.status !== undefined){
				this.treatErrors(o);
			}
			if(hud){
				hide(HUD);
			}
		}

		var sendCallback ={ 
			success: handleSuccess,
	  	failure: handleFailure, 
			upload: handleSuccess
		};

		YAHOO.util.Connect.setForm(form, is_file);
		YAHOO.util.Connect.asyncRequest('POST', form.action, sendCallback);

	}

	
};
