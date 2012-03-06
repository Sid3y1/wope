
var Cookie = function () {	
	this.messages=new Array();
	this.listUsers=null;
	this.chatLogin=this._readCookie("chatLogin");
	this.json_data=this._readCookie("chat_json");
	if (this.json_data) {
		this.json_data=decodeURIComponent(this.json_data);
		//this.messages=this.json_data.parseJSON();
	}
	this.listUsers=this._readCookie("listUsers");
	if (this.listUsers) {
		this.listUsers=decodeURIComponent(this.listUsers);
	}
	if (this.chatLogin) {
		if (decodeURIComponent(this.chatLogin)!=myChat.login) {
			this._purgeCookie();
		}
	} else {
		this.chatLogin=encodeURIComponent(myChat.login);
	}
};

Cookie.prototype._purgeCookie = function() {
	this.messages=new Array();
	this.listUsers=null;
	this.chatLogin=encodeURIComponent(myChat.login);
	this.save();
}

Cookie.prototype.update = function() {
	this.json_data=this._readCookie("chat_json");
	if (this.json_data) {
		this.json_data=decodeURIComponent(this.json_data);
		this.messages=this.json_data.parseJSON();
	}
	this.listUsers=this._readCookie("listUsers");
	if (this.listUsers) {
		this.listUsers=decodeURIComponent(this.listUsers);
	}
}

Cookie.prototype.getSemaphore = function() {
	var semaphore=myChat.cookie._readCookie("semaphore");
	var now=new Date();
	if (semaphore) {
		if (now.getTime() < (parseInt(semaphore) + 2000)) {
			return false;
		} else {
			document.cookie = "semaphore="+now.getTime()+" ; path=/ ";
			return true;
		}
	} else {
		document.cookie = "semaphore="+now.getTime()+" ; path=/ ";
		return true;
	}
}

// See http://www.quirksmode.org/js/cookies.html
Cookie.prototype._readCookie = function(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)===' ') {	
			c = c.substring(1,c.length);
		}
		if (c.indexOf(nameEQ) === 0) {
			return c.substring(nameEQ.length,c.length);	
		}
	}
	return null;
};

Cookie.prototype.save = function() {
	if (this.messages) {
		//document.cookie = "chat_json="+this.messages.toJSONString()+" ; path=/ ";
		document.cookie = "chat_json="+encodeURIComponent(this.messages.toJSONString())+" ; path=/ ";
	}
	if (this.listUsers) {
		//document.cookie = "listUsers="+encodeURIComponent(this.listUsers)+" ; path=/ ";
		document.cookie = "listUsers="+this.listUsers+" ; path=/ ";
	}
	if (this.chatLogin) {
		document.cookie = "chatLogin="+this.chatLogin+" ; path=/ ";
	}
};


var Debug = function(){
	this.debug=false;
	this.debugMessage=null;
	this.debugDiv=null;
};

Debug.prototype.add = function(string) {
	if (typeof(string)=="object") {
		this.debugMessage=string.toJSONString();
	} else if (typeof(string)=="object") {
		this.debugMessage=string.toJSONString();
	} else {
		this.debugMessage=string;
	}
	
	if (this.debug) {
		if (!this.debugDiv) {
			this.debugDiv=document.getElementById("debug");
		}
		if (this.debugDiv) {
			this.debugDiv.innerHTML=this.debug.innerHTML+"<br>"+this.debugMessage;
		} else {
			alert(this.debugMessage);
		}
	}
};


var callbackRefresh = {
	success: function(o) {
		var result=o.responseText.parseJSON();
		if (result!==false) {
			myChat.parseDataRefresh(result);
		}
	},
	failure: function(o) {
		myChat.unavailable();
	}, 
	timeout: 2000
};

var callbackMessage = {
	success: function(o) {
		
		var result=o.responseText.parseJSON();
		if (result!==false) {
			myChat.parseDataMessage(result);
		}
	},
	failure: function(o) {
		myChat.unavailable();
	}, 
	timeout: 2000
};

var callbackInit = {
	success: function(o) {
		var result=o.responseText.parseJSON();
		if (result!==false) {
			myChat.parseDataInit(result);
		}
	},
	failure: function(o) {
		myChat.unavailable();
	}, 
	timeout: 2000
};

var callbackRegisterUser = {
	success: function(o) {
		var result=o.responseText.parseJSON();
		if (result!==false) {
			myChat.parseDataRegisterUser(result);
		}
	},
	failure: function(o) {
		myChat.unavailable();
	}, 
	timeout: 2000
};

var callbackAddUser = {
	success: function(o) {
		var result=o.responseText.parseJSON();
		if (result!==false) {
			myChat.parseDataAddUser(result);
		}
	},
	failure: function(o) {
		myChat.unavailable();
	}, 
	timeout: 2000
};

var ListUsers = function (){
};	

ListUsers.prototype.init= function () {
	var listeUser=document.getElementById("listeUser");
	if (listeUser) {
		listeUser.innerHTML="Please Wait...";
	}
	var chat_input_adduser=document.getElementById("chat_input_adduser");
	if (chat_input_adduser) {
		chat_input_adduser.disabled=false;		
		chat_input_adduser.onkeypress=function(e) {var key; if(e.keyCode){key=e.keyCode;}else{key=e.which;} if (key==13){myChat.listUsers.addUser();}};
	}
	var chat_input_adduser_valid=document.getElementById("chat_input_adduser_valid");
	if (chat_input_adduser_valid) {
		chat_input_adduser_valid.disabled=false;	
	}
};

ListUsers.prototype.unavailable= function () {
	var listeUser=document.getElementById("listeUser");
	if (listeUser) {
		listeUser.innerHTML="Reconnecting...";
	}
	
	var chat_input_adduser=document.getElementById("chat_input_adduser");
	if (chat_input_adduser) {
		chat_input_adduser.disabled=true;
	}
	
	var chat_input_adduser_valid=document.getElementById("chat_input_adduser_valid");
	if (chat_input_adduser_valid) {
		chat_input_adduser_valid.disabled=true;	
	}
};

ListUsers.prototype.updateListe = function (users) {
	var listeUser=document.getElementById("listeUser");
	if (listeUser) {
		//empty the list
		listeUser.innerHTML="";
		//this._createForm();
		for(var i=0; i < users.length; i++) {
			this._createUserDiv(users[i],listeUser);
		}
	}
};

ListUsers.prototype.getListUsersFromCookie = function () {
	var listUsersJSON=myChat.cookie.listUsers;
	if (listUsersJSON) {
		var listUsers=listUsersJSON.parseJSON();
		if (listUsers) {
			this.updateListe(listUsers);
		}
	}
};
ListUsers.prototype._createUserDiv= function (user,listeUser) {
	var div=document.createElement("div");
	if (user.name && user.name!="") {
		var txt = document.createTextNode(user.name);	
	} else {
		var txt = document.createTextNode(user.user);
	}
	var a=document.createElement("a");
	var className="";
	if (!user.presence) {
		className="listUserNotAvailable";
	} else if (user.presence=="available") {
		className="listUserAvailable";
	} else if (user.presence=="away") {
		className="listUserAway";
	} else if (user.presence=="xa") {
		className="listUserXa";
	} else if (user.presence=="dnd") {
		className="listUserDnd";
	} else if (user.presence=="chat") {
		className="listUserChat";
	} else {
		className="listUserNotAvailable";
	}
	div.setAttribute("class",className);
	div.setAttribute("className",className);	
	a.setAttribute("class",className);
	a.setAttribute("className",className);
	
	if (className!="listUserNotAvailable") {
		div.appendChild(a);
		a.appendChild(txt);
	} else {
		div.appendChild(txt);
	}
	
	a.onclick=function() {myChat.messageWindow.openChatWindow(user.user,user.ressource);};
	listeUser.appendChild(div);
};

ListUsers.prototype.addUser = function() {
	myChat.debug.add("Appel de addUser");
	inputField=document.getElementById("chat_input_adduser");
	if (inputField) {
		inputField.focus();
		user=inputField.value;
		myChat.debug.add("inputFielValue = "+user);
		username=encodeURIComponent(user);
		YAHOO.util.Connect.asyncRequest('GET',
			myChat.url+"?action=requestSubscription&username="+username, callbackAddUser);
	}
};

ListUsers.prototype.registerUser = function(user) {
	if (window.confirm("Authorisez vous '"+user+"' à vous voir connecter ?")) {
		acceptRequestFrom=encodeURIComponent(user);
		YAHOO.util.Connect.asyncRequest('GET', 
			myChat.url+"?action=acceptRequest&acceptRequestFrom="+acceptRequestFrom, callbackRegisterUser);
	}
};

var MessageWindow = function(){
};

MessageWindow.prototype.init = function() {
	var cookie=myChat.cookie;
	input_chat=document.getElementById("input_chat");
	if (input_chat) {
		input_chat.disabled=false;
	}
	input_chat_valid=document.getElementById("input_chat_valid");
	if (input_chat_valid) {
		input_chat_valid.disabled=false;
	}
	
	if(cookie.messages) {
		for (var i=myChat.lastMessageReadFromCookie; i< cookie.messages.length; i++) {
			this.newMessage(cookie.messages[i],true);
		}
		myChat.lastMessageReadFromCookie=cookie.messages.length;
	}
	
	if (cookie.messages[cookie.messages.length-1]) {
		if (cookie.messages[cookie.messages.length-1].to) {
			this._giveFocus(cookie.messages[cookie.messages
				.length-1].to,cookie.messages[cookie.messages.length-1].ressource);
		} else {
			this._giveFocus(cookie.messages[cookie.messages.length-1]
				.from,cookie.messages[cookie.messages.length-1].ressource);
		}
	}
};

MessageWindow.prototype.getMessageFromCookie = function() {
	myChat.cookie.update();
	var cookie=myChat.cookie;
	if (cookie.messages) {
		//alert("Read :" + myChat.lastMessageReadFromCookie+" On:"+cookie.messages.length);
		for (var i=myChat.lastMessageReadFromCookie; i< cookie.messages.length; i++) {
			this.newMessage(cookie.messages[i]);
		}
		myChat.lastMessageReadFromCookie=cookie.messages.length;
	}
};


MessageWindow.prototype.unavailable = function() {
	input_chat=document.getElementById("input_chat");
	if (input_chat) {
		input_chat.disabled=true;
	}
	input_chat_valid=document.getElementById("input_chat_valid");
	if (input_chat_valid) {
		input_chat_valid.disabled=true;	
	}
};

MessageWindow.prototype.newMessage = function(message,init) {
	if (message.to && message.from && message.data) {
		var div=document.getElementById("chat_"+message.to+"@"+message.ressource);
		if (!div) {
			div=this._createChatDiv(message.to,message.ressource);
		}
		this._addTextToChat(div,message.from,message.data);
	} else if (message.from && message.data) {
		div=document.getElementById("chat_"+message.from+"@"+message.ressource);
		if (!div) {
			div=this._createChatDiv(message.from,message.ressource);
		}
		if (!init) {
			if (message.from!=myChat.chatWindow.login || message.ressource!=myChat.chatWindow.ressource) {
				div.previousSibling.setAttribute("class","chat_header_new_message");
				div.previousSibling.setAttribute("className","chat_header_new_message");
			}
		}
		this._addTextToChat(div,message.from,message.data);
	}
};

MessageWindow.prototype.openChatWindow = function(login,ressource) {
	if (!document.getElementById("chat_"+login+"@"+ressource)){
		this._createChatDiv(login,ressource);
	}
	this._giveFocus(login,ressource);
};

MessageWindow.prototype.closeChatWindow = function(login,ressource) {
	//Remove the chat_window and the chat header from the page 
	var listeWindow=document.getElementsByTagName("div");
	for (var i=0; i< listeWindow.length;i++) {
		if (listeWindow[i].getAttribute("name")=="chat_window") {
			if (listeWindow[i].getAttribute("id")==="chat_"+login+"@"+ressource) {
				var chat_header=listeWindow[i].previousSibling;
				var chat_window=listeWindow[i];
				var parent_div=listeWindow[i].parentNode;
				parent_div.removeChild(chat_header);
				parent_div.removeChild(chat_window);
			}
		}
	}
	
	//check if we still have a chat window
	var nbChatWindow=0;
	var listeWindow=document.getElementsByTagName("div");
	for (var i=0; i< listeWindow.length;i++) {
		if (listeWindow[i].getAttribute("name")=="chat_window") {
			nbChatWindow++;
		}
	}
	// No more chat widow we empty the chat div
	if (nbChatWindow==0) {
		var chatDiv=document.getElementById("chat_text");
		chatDiv.innerHTML="";
	}
	
	// Empty the message from the cookie that correspond to this chat window
	var newCookie = new Array();
	var message;
	for (var j=0;j<myChat.cookie.messages.length;j++) {
		message=myChat.cookie.messages[j];
		if (message.ressource==ressource) {
			if (message.to) {
				if (message.to!=login) {
					newCookie[newCookie.length]=message;
				}
			} else {
				if (message.from!=login) {
					newCookie[newCookie.length]=message;
				}
			}
		} else {
			newCookie[newCookie.length]=message;
		}
	}
	myChat.cookie.messages=newCookie;
	myChat.cookie.save();
};

MessageWindow.prototype.toggleOpenClose = function(login,ressource) {
	var hideMe=false;
	if(myChat.chatWindow) {
		if (myChat.chatWindow.login==login && myChat.chatWindow.ressource==ressource) {
			hideMe=true;
			myChat.chatWindow=new ChatWindow();
		} else {
			myChat.chatWindow=new ChatWindow(login,ressource);
		}
	} else {
		myChat.chatWindow=new ChatWindow(login,ressource);
	}
	// Hide all the other
	var listeWindow=document.getElementsByTagName("div");
	for (var i=0; i< listeWindow.length;i++) {
		if (listeWindow[i].getAttribute("name")=="chat_window") {
			if (listeWindow[i].getAttribute("id")==="chat_"+login+"@"+ressource && !hideMe) {
				listeWindow[i].setAttribute("class","chat_window");
				listeWindow[i].setAttribute("className","chat_window");
				listeWindow[i].previousSibling.setAttribute("class","chat_header_normal");
				listeWindow[i].previousSibling.setAttribute("className","chat_header_normal");
			} else {
				listeWindow[i].setAttribute("class","hide");
				listeWindow[i].setAttribute("className","hide");
			}
		}
	}
};

MessageWindow.prototype._giveFocus = function(login,ressource) {
	var hideMe=false;
	if(myChat.chatWindow) {
		myChat.chatWindow=new ChatWindow(login,ressource);
	} else {
		myChat.chatWindow=new ChatWindow(login,ressource);
	}
	// Hide all the other
	var listeWindow=document.getElementsByTagName("div");
	for (var i=0; i< listeWindow.length;i++) {
		if (listeWindow[i].getAttribute("name")=="chat_window") {
			if (listeWindow[i].getAttribute("id")==="chat_"+login+"@"+ressource && !hideMe) {
				listeWindow[i].setAttribute("class","chat_window");
				listeWindow[i].setAttribute("className","chat_window");
				listeWindow[i].previousSibling.setAttribute("class","chat_header_normal");
				listeWindow[i].previousSibling.setAttribute("className","chat_header_normal");
			} else {
				listeWindow[i].setAttribute("class","hide");
				listeWindow[i].setAttribute("className","hide");
			}
		}
	}
};


MessageWindow.prototype._createChatDiv = function (login,ressource) {
	var outputDiv;
	var inputField,buttonField;
	var chatdiv=document.getElementById("chat_text");
	var div = document.createElement("div");
	var a = document.createElement("a");
	var txt = document.createTextNode(login);
	var form= document.getElementById("form_chat");
	a.setAttribute("class","chat_header");
	a.setAttribute("className","chat_header");
	a.onclick=function() {myChat.messageWindow.toggleOpenClose(login,ressource);};
	a.appendChild(txt);
	//create the header
	div.setAttribute("class","chat_header_normal");
	div.setAttribute("className","chat_header_normal");
	div.appendChild(a);
	a = document.createElement("a");
	a.setAttribute("class","chat_header_close");
	a.setAttribute("className","chat_header_close");
	a.onclick=function() {myChat.messageWindow.closeChatWindow(login,ressource);};
	txt = document.createTextNode("X");
	a.appendChild(txt);
	div.appendChild(a);
	if (form) {
		chatdiv.insertBefore(div,form);
	} else {
		chatdiv.appendChild(div);
	}
	//create the window
	div = document.createElement("div");
	div.setAttribute("id","chat_"+login+"@"+ressource);
	div.setAttribute("name","chat_window");
	div.setAttribute("className","chat_window");
	div.setAttribute("class","chat_window");
	if (form) {
		chatdiv.insertBefore(div,form);
	} else {
		chatdiv.appendChild(div);
	}
	outputDiv=div;
	this._giveFocus(login,ressource);
	this._createForm(login,ressource);
	return outputDiv;
};

MessageWindow.prototype._createForm = function(login,ressource) {
	var outputDiv;
	var inputField,buttonField;
	var chatdiv=document.getElementById("chat_text");
	var div=document.getElementById("form_chat");
	if (!div) {
		div= document.createElement("div");
		//create the form windowdiv
		div.setAttribute("id","form_chat");
		div.setAttribute("class","form_window");
		div.setAttribute("className","form_window");
		chatdiv.appendChild(div);
		//create the text field
		inputField = document.createElement("input");
		inputField.setAttribute("type","text");
		inputField.setAttribute("id","input_chat");
		inputField.setAttribute("class","form_window");
		inputField.setAttribute("className","form_window");
		inputField.setAttribute("value","");
		inputField.onkeypress=function(e) {var key; if(e && e.keyCode){key=e.keyCode;}else if (e){ key=e.which;} if (key && key==13){myChat.messageWindow.sendData();}};
		div.appendChild(inputField);
		//create the button field
		buttonField = document.createElement("input");
		buttonField.setAttribute("type","submit");
		buttonField.setAttribute("id","input_chat_valid");
		buttonField.setAttribute("class","form_window");
		buttonField.setAttribute("className","form_window");
		buttonField.setAttribute("value","Ok");
		buttonField.onclick=function() {myChat.messageWindow.sendData();};
		div.appendChild(buttonField);
	}
};

MessageWindow.prototype._wrapText = function(text) {
	var reg;
	var left="";
	var temp;
	var retText="";
	var right=text;
	while (right.length>24) {
		reg=/^(.{24})(.*)$/;
		temp=reg.exec(right);
		left=temp[1];
		right=temp[2];
		reg=/^(.*) (.*?)$/;
		temp=reg.exec(left);
		if (temp) {
			if (temp[1].length>12) {
				left=temp[1];
				right=temp[2]+right;
			}
		}
		retText=retText+left+" ";
	}
	retText=retText+right;
	return retText;
}

MessageWindow.prototype._addTextToChat = function (chatdiv,login,texte) {
	var p;
	var txt;
	var div;
	var listeChat=chatdiv.childNodes;
	var lastChatLogin;
	var chat_a_bottom;
	var createLoginField=true;
	if (listeChat) {
		for (var i=0;i<listeChat.length;i++) {
			if(listeChat[i].getAttribute("class")=="chat_login") {
				lastChatLogin=listeChat[i];
			} else if(listeChat[i].getAttribute("class")=="chat_a_bottom") {
				chat_a_bottom=listeChat[i];
			}
		}
	}
	if (!chat_a_bottom) {
		chat_a_bottom= document.createElement("a");
		chat_a_bottom.setAttribute("class","chat_a_bottom");
		chat_a_bottom.setAttribute("className","chat_a_bottom");
		chat_a_bottom.setAttribute("name","chat_a_bottom");
	}
	//Look if the previous chat login is not similar
	if (lastChatLogin) {
		if (lastChatLogin.innerHTML==login+" :") {
			createLoginField=false;
		}
	} 
	if (createLoginField) {
		//creation of the login : field
		div = document.createElement("div");
		txt = document.createTextNode(login+" :");
		div.setAttribute("class","chat_login");
		div.setAttribute("className","chat_login");
		div.appendChild(txt);
		chatdiv.appendChild(div);
	}
	//creation of the text field
	div = document.createElement("div");
	texte=this._wrapText(texte);
	txt = document.createTextNode(texte);
	div.setAttribute("class","chat_window_text");
	//bug IE
	div.setAttribute("className","chat_window_text");
	div.appendChild(txt);
	chatdiv.appendChild(div);
	chatdiv.appendChild(chat_a_bottom);
	div.scrollIntoView(false);
};

MessageWindow.prototype.sendData = function () {
	var inputField;
	var chatDiv;
	var texte;
	var url;
	var myData;
	var myConnect;
	var login = myChat.chatWindow.login;
	var ressource = myChat.chatWindow.ressource;
	if (login && ressource) {
		chatDiv=document.getElementById("chat_"+login+"@"+ressource);
		inputField=document.getElementById("input_chat");
		inputField.focus();
		texte=inputField.value;
		inputField.value="";
		if (texte!=="") {
			var myMessage=new Message();
			myMessage.to=login;
			myMessage.from=myChat.login;
			myMessage.data=texte;
			
			myChat.waitMessages.push(myMessage);
			myData="action=message&data=";
			myData+=encodeURIComponent(texte);
			myData+="&to=";
			myData+=encodeURIComponent(login+"@"+ressource);
			YAHOO.util.Connect.asyncRequest('GET', myChat.url+"?"+myData+"&user="+myChat.login, callbackMessage);
		}
	}
};

var Chat = function (){
	this.login = "";
	this.password="";
	this.seed="";
	this.url ="ajax/update.php";
	this.connected = false;
	this.messageWindow=null;
	this.listUsers=null;
	this.debug=new Debug();
	this.cookie=null;
	this.chatWindow=null;
	this.lastMessageReadFromCookie=0;
	this.waitMessages=new Array();
};

Chat.prototype.init= function() {
	window.location="#chat_a_bottom";
	this.cookie=new Cookie();
	this.messageWindow = new MessageWindow();
	this.messageWindow.init();
	this.listUsers = new ListUsers();
	this.listUsers.init();
	myChat.debug.add("Launch init");
	YAHOO.util.Connect.asyncRequest('GET',myChat.url+"?action=init&login="+this.login+"&password="+this.password+"&seed="+this.seed, callbackInit);
};

Chat.prototype.loadData= function() {
	if(myChat.cookie.getSemaphore()==true) {
		YAHOO.util.Connect.asyncRequest('GET', myChat.url+"?action=refresh&user="+myChat.login, callbackRefresh);
	} else {
		if (myChat.connected) {
			myChat.messageWindow.getMessageFromCookie();
			myChat.listUsers.getListUsersFromCookie();
		}
		window.setTimeout("myChat.loadData()",2000);
	}
};

Chat.prototype.parseDataRefresh= function(result) {
	if (result.status && result.status=="ok") {
		// The refresh succeed so we prepare an other refresh
		myChat.debug.add(result);
		window.setTimeout("myChat.loadData()",2000);
	} else {
		// disable the chat and try to login again in 30s
		this.unavailable();
		myChat.debug.add(result);
	}
	
	if (result.users) {
		this.listUsers.updateListe(result.users);
		this.cookie.listUsers=result.users.toJSONString();
		this.cookie.save();
	}
	if (result.messages) {
		for (var i=0;i<result.messages.length;i++) {
			var message=new Message(result.messages[i]);
			this.messageWindow.newMessage(message);
			if (this.cookie.messages){
				this.cookie.messages.push(message);
				this.lastMessageReadFromCookie++;
			}
		}
		this.cookie.save();
	}
	if (result.subscriptionRequests) {
		for (var i=0;i<result.subscriptionRequests.length;i++) {
			myChat.debug.add(result.subscriptionRequests);
			var subscriptionRequest=subscriptionRequests[i];
			this.listUsers.registerUser(subscriptionRequest.from);
		}
	}
};

Chat.prototype.parseDataInit= function (result) {
	if (result.status && result.status=="ok") {
		myChat.debug.add(result);
		myChat.connected=true;
		window.setTimeout("myChat.loadData()",20);
	} else {
		myChat.debug.add(result);
		myChat.unavailable();
	}
};

Chat.prototype.parseRegisterUser= function (result) {
	if (result.status && result.status=="ok") {
		myChat.debug.add(result);
	} else {
		myChat.debug.add(result);
	}
};

Chat.prototype.parseDataAddUser= function (result) {
	if (result.status && result.status=="ok") {
		myChat.debug.add(result);
	} else {
		myChat.debug.add(result);
	}
};

Chat.prototype.parseDataMessage= function (result) {
	if (result.status && result.status=="ok") {
		myChat.debug.add(result);
		myMessage = myChat.waitMessages.pop();
		myChat.messageWindow.newMessage(myMessage);
		myChat.lastMessageReadFromCookie++;
		myChat.cookie.messages.push(myMessage);
		myChat.cookie.save();
	} else {
		//This message didn't succeed remove it from the stack
		myMessage = myChat.waitMessages.pop();
		myChat.debug.add(result);
	}
};

Chat.prototype.unavailable = function() {
	myChat.debug.add("Unavailable");
	// Retry in 30s
	myChat.messageWindow.unavailable();
	myChat.listUsers.unavailable();
	window.setTimeout("myChat.init()",30000);
};

var ChatWindow = function (login,ressource) {
	myChat.connected=false;
	this.login=login;
	this.ressource=ressource;
};

var Message = function(message){
	if (message) {
		this.from=message.from;
		this.to=message.to;
		this.data=message.data;
		var reg = /(.*)\//;
		var res=reg.exec(message.ressource);
		this.ressource=res[1];
	} else {
		this.from = "";
		this.to="";
		this.data="";
		this.ressource=myChat.chatWindow.ressource;
	}
};


var myChat = new Chat();

function d() {
	myChat.debug.debug=!myChat.debug.debug;
}


