/*
 +-----------------------------------------------------------------------+
 | RoundCube Webmail Client Script                                       |
 |                                                                       |
 | This file is part of the RoundCube Webmail client                     |
 | Copyright (C) 2005, RoundCube Dev, - Switzerland                      |
 | Licensed under the GNU GPL                                            |
 |                                                                       |
 | Modified: 2005/10/21 (roundcube)                                      |
 |                                                                       |
 +-----------------------------------------------------------------------+
 | Author: Thomas Bruederli <roundcube@gmail.com>                        |
 +-----------------------------------------------------------------------+
*/

var idInput;
var separator;

// default browsercheck
function roundcube_browser()
  {
  this.ver = parseFloat(navigator.appVersion);
  this.appver = navigator.appVersion;
  this.agent = navigator.userAgent;
  this.name = navigator.appName;
  this.vendor = navigator.vendor ? navigator.vendor : '';
  this.vendver = navigator.vendorSub ? parseFloat(navigator.vendorSub) : 0;
  this.product = navigator.product ? navigator.product : '';
  this.platform = String(navigator.platform).toLowerCase();
  this.lang = (navigator.language) ? navigator.language.substring(0,2) :
              (navigator.browserLanguage) ? navigator.browserLanguage.substring(0,2) :
              (navigator.systemLanguage) ? navigator.systemLanguage.substring(0,2) : 'en';

  this.win = (this.platform.indexOf('win')>=0) ? true : false;
  this.mac = (this.platform.indexOf('mac')>=0) ? true : false;
  this.linux = (this.platform.indexOf('linux')>=0) ? true : false;
  this.unix = (this.platform.indexOf('unix')>=0) ? true : false;

  this.dom = document.getElementById ? true : false;
  this.dom2 = (document.addEventListener && document.removeEventListener);

  this.ie = (document.all) ? true : false;
  this.ie4 = (this.ie && !this.dom);
  this.ie5 = (this.dom && this.appver.indexOf('MSIE 5')>0);
  this.ie6 = (this.dom && this.appver.indexOf('MSIE 6')>0);

  this.mz = (this.dom && this.ver>=5);  // (this.dom && this.product=='Gecko')
  this.ns = ((this.ver<5 && this.name=='Netscape') || (this.ver>=5 && this.vendor.indexOf('Netscape')>=0));
  this.ns4 = (this.ns && parseInt(this.ver)==4);
  this.ns6 = (this.ns && parseInt(this.vendver)==6);  // (this.mz && this.ns) ? true : false;
  this.ns7 = (this.ns && parseInt(this.vendver)==7);  // this.agent.indexOf('Netscape/7')>0);
  this.safari = (this.agent.toLowerCase().indexOf('safari')>0 || this.agent.toLowerCase().indexOf('applewebkit')>0);
  this.konq   = (this.agent.toLowerCase().indexOf('konqueror')>0);

  this.opera = (window.opera) ? true : false;
  this.opera5 = (this.opera5 && this.agent.indexOf('Opera 5')>0) ? true : false;
  this.opera6 = (this.opera && this.agent.indexOf('Opera 6')>0) ? true : false;
  this.opera7 = (this.opera && this.agent.indexOf('Opera 7')>0) ? true : false;

  if(this.opera && window.RegExp)
    this.vendver = (/opera(\s|\/)([0-9\.]+)/i.test(navigator.userAgent)) ? parseFloat(RegExp.$2) : -1;
  else if(!this.vendver && this.safari)
    this.vendver = (/(safari|applewebkit)\/([0-9]+)/i.test(this.agent)) ? parseInt(RegExp.$2) : 0;
  else if((!this.vendver && this.mz) || this.agent.indexOf('Camino')>0)
    this.vendver = (/rv:([0-9\.]+)/.test(this.agent)) ? parseFloat(RegExp.$1) : 0;
  else if(this.ie && window.RegExp)
    this.vendver = (/msie\s+([0-9\.]+)/i.test(this.agent)) ? parseFloat(RegExp.$1) : 0;
  else if(this.konq && window.RegExp)
    this.vendver = (/khtml\/([0-9\.]+)/i.test(this.agent)) ? parseFloat(RegExp.$1) : 0;


  // get real language out of safari's user agent
  if(this.safari && (/;\s+([a-z]{2})-[a-z]{2}\)/i.test(this.agent)))
    this.lang = RegExp.$1;

  this.dhtml = ((this.ie4 && this.win) || this.ie5 || this.ie6 || this.ns4 || this.mz);
  this.layers = this.ns4;  // (document.layers);
  this.div = (this.ie4 || this.dom);
  this.vml = (this.win && this.ie && this.dom && !this.opera);
  this.linkborder = (this.ie || this.mz);
  this.rollover = (this.ver>=4 || (this.ns && this.ver>=3));  // (document.images) ? true : false;
  this.pngalpha = (this.mz || (this.opera && this.vendver>=6) || (this.ie && this.mac && this.vendver>=5) ||
                   (this.ie && this.win && this.vendver>=5.5) || this.safari);
  this.opacity = (this.mz || (this.ie && this.vendver>=5.5 && !this.opera) || (this.safari && this.vendver>=100));
  this.cookies = navigator.cookieEnabled;
  
  // test for XMLHTTP support
  this.xmlhttp_test = function()
    {
    var activeX_test = new Function("try{var o=new ActiveXObject('Microsoft.XMLHTTP');return true;}catch(err){return false;}");
    this.xmlhttp = (window.XMLHttpRequest || (window.ActiveXObject && activeX_test())) ? true : false;
    return this.xmlhttp;
    }
  }




var rcube_layer_objects = new Array();

function rcube_layer(id, attributes)
  {
  this.name = id;
  
  // create a new layer in the current document
  this.create = function(arg)
    {
    var l = (arg.x) ? arg.x : 0;
    var t = (arg.y) ? arg.y : 0;
    var w = arg.width;
    var h = arg.height;
    var z = arg.zindex;
    var vis = arg.vis;
    var parent = arg.parent;
    var obj;

    obj = document.createElement('DIV');
    with(obj)
      {
      id = this.name;
      with(style)
        {
        position = 'absolute';
        visibility = (vis) ? (vis==2) ? 'inherit' : 'visible' : 'hidden';
        left = l+'px';
        top = t+'px';
        if(w) width = w+'px';
        if(h) height = h+'px';
        if(z) zIndex = z;
        }
      }
      
    if(parent) parent.appendChild(obj);
    else document.body.appendChild(obj);

    this.elm = obj;
    };


  // create new layer
  if(attributes!=null)
    {
    this.create(attributes);
    this.name = this.elm.id;
    }
  else  // just refer to the object
    this.elm = document.getElementById(id);


  if(!this.elm)
    return false;


  // ********* layer object properties *********

  this.css = this.elm.style;
  this.event = this.elm;
  this.width = this.elm.offsetWidth;
  this.height = this.elm.offsetHeight;
  this.x = parseInt(this.elm.offsetLeft);
  this.y = parseInt(this.elm.offsetTop);
  this.visible = (this.css.visibility=='visible' || this.css.visibility=='show' || this.css.visibility=='inherit') ? true : false;

  this.id = rcube_layer_objects.length;
  this.obj = 'rcube_layer_objects['+this.id+']';
  rcube_layer_objects[this.id] = this;


  // ********* layer object methods *********


  // move the layer to a specific position
  this.move = function(x, y)
    {
    this.x = x;
    this.y = y;
    this.css.left = Math.round(this.x)+'px';
    this.css.top = Math.round(this.y)+'px';
    }


  // move the layer for a specific step
  this.shift = function(x,y)
    {
    x = Math.round(x*100)/100;
    y = Math.round(y*100)/100;
    this.move(this.x+x, this.y+y);
    }


  // change the layers width and height
  this.resize = function(w,h)
    {
    this.css.width  = w+'px';
    this.css.height = h+'px';
    this.width = w;
    this.height = h;
    }


  // cut the layer (top,width,height,left)
  this.clip = function(t,w,h,l)
    {
    this.css.clip='rect('+t+' '+w+' '+h+' '+l+')';
    this.clip_height = h;
    this.clip_width = w;
    }


  // show or hide the layer
  this.show = function(a)
    {
    if(a==1)
      {
      this.css.visibility = 'visible';
      this.visible = true;
      }
    else if(a==2)
      {
      this.css.visibility = 'inherit';
      this.visible = true;
      }
    else
      {
      this.css.visibility = 'hidden';
      this.visible = false;
      }
    }


  // write new content into a Layer
  this.write = function(cont)
    {
    this.elm.innerHTML = cont;
    }


  // set the given color to the layer background
  this.set_bgcolor = function(c)
    {
    if(!c || c=='#')
      c = 'transparent';

    this.css.backgroundColor = c;
    }


  // set the opacity of a layer to the given ammount (in %)
  this.set_opacity = function(v)
    {
    if(!bw.opacity)
      return;

    var op = v<=1 ? Math.round(v*100) : parseInt(v);

    if(bw.ie)
      this.css.filter = 'alpha(opacity:'+op+')';
    else if(bw.safari)
      {
      this.css.opacity = op/100;
      this.css.KhtmlOpacity = op/100;
      }
    else if(bw.mz)
      this.css.MozOpacity = op/100;
    }
  }



// find a value in a specific array and returns the index
function find_in_array()
  {
  var args = find_in_array.arguments;
  if(!args.length) return -1;

  var haystack = typeof(args[0])=='object' ? args[0] : args.length>1 && typeof(args[1])=='object' ? args[1] : new Array();
  var needle = typeof(args[0])!='object' ? args[0] : args.length>1 && typeof(args[1])!='object' ? args[1] : '';
  var nocase = args.length==3 ? args[2] : false;

  if(!haystack.length) return -1;

  for(var i=0; i<haystack.length; i++)
    if(nocase && haystack[i].toLowerCase()==needle.toLowerCase())
      return i;
    else if(haystack[i]==needle)
      return i;

  return -1;
  }


// get any type of html objects by id/name
function rcube_find_object(id, d)
  {
  var n, f, obj, e;
  if(!d) d = document;

  if(d.getElementsByName && (e = d.getElementsByName(id)))
    obj = e[0];
  if(!obj && d.getElementById)
    obj = d.getElementById(id);
  if(!obj && d.all)
    obj = d.all[id];

  if(!obj && d.images.length)
    obj = d.images[id];

  if(!obj && d.forms.length)
    for(f=0; f<d.forms.length; f++)
      {
      if(d.forms[f].name == id)
        obj = d.forms[f];
      else if(d.forms[f].elements[id])
        obj = d.forms[f].elements[id];
      }

  if(!obj && d.layers)
    {
    if(d.layers[id]) obj = d.layers[id];
    for(n=0; !obj && n<d.layers.length; n++)
      obj = nex_get_object(id, d.layers[n].document);
    }

  return obj;
  }


// return the absolute position of an object within the document
function rcube_get_object_pos(obj)
  {
  if(typeof(obj)=='string')
    obj = nex_get_object(obj);

  if(!obj) return {x:0, y:0};

  var iX = (bw.layers) ? obj.x : obj.offsetLeft;
  var iY = (bw.layers) ? obj.y : obj.offsetTop;

  if(bw.ie || bw.mz)
    {
    var elm = obj.offsetParent;
    while(elm && elm!=null)
      {
      iX += elm.offsetLeft;
      iY += elm.offsetTop;
      elm = elm.offsetParent;
      }
    }

  if(bw.mac && bw.ie5) iX += document.body.leftMargin;
  if(bw.mac && bw.ie5) iY += document.body.topMargin;

  return {x:iX, y:iY};
  }

var bw = new roundcube_browser();





/* PAGE END */
/* PAGE START */


var rcube_webmail_client;


function rcube_webmail()
  {
  this.env = new Object();
  this.gui_objects = new Object();
  this.commands = new Object();
  this.selection = new Array();
  this.separator = '';
  // create public reference to myself
  rcube_webmail_client = this;
  this.ref = 'rcube_webmail_client';
 
  // webmail client settings
  this.dblclick_time = 600;
  this.message_time = 5000;
  this.request_timeout = 120000;
  this.mbox_expression = new RegExp('[^0-9a-z\-_]', 'gi');
  this.env.blank_img = 'skins/default/images/blank.gif';
  
  // mimetypes supported by the browser (default settings)
  this.mimetypes = new Array('text/plain', 'text/html', 'text/xml',
                             'image/jpeg', 'image/gif', 'image/png',
                             'application/x-javascript', 'application/pdf',
                             'application/x-shockwave-flash');


  // set environment variable
  this.set_env = function(name, value)
    {
    //if (!this.busy)
      this.env[name] = value;    
    };

  // add a button to the button list
    // register a specific gui object
  this.gui_object = function(name, id)
    {
    this.gui_objects[name] = id;
    };


  // initialize webmail client
  this.init = function()
    {
    this.task = this.env.task;
    
    // check browser
    if (!bw.dom || !bw.xmlhttp_test())
      {
      location.href = this.env.comm_path+'&_action=error&_code=0x199';
      return;
      }
    
    // find all registered gui objects
    for (var n in this.gui_objects)
      this.gui_objects[n] = rcube_find_object(this.gui_objects[n]);
      
    // tell parent window that this frame is loaded
    if (this.env.framed && parent.rcmail && parent.rcmail.set_busy)
      parent.rcmail.set_busy(false);

    // enable general commands
    this.enable_command('logout', 'mail', 'addressbook', 'settings', true);
    
    switch (this.task)
      {
      case 'mail':
        var msg_list = this.gui_objects.messagelist;
        if (msg_list)
          {
          this.init_messagelist(msg_list);
          this.enable_command('markread', true);
          }

        // enable mail commands
        this.enable_command('list', 'compose', 'add-contact', true);
        
        if (this.env.action=='show')
          {
          this.enable_command('show', 'reply', 'forward', 'moveto', 'delete', 'viewsource', 'print', 'load-attachment', true);
          if (this.env.next_uid)
            this.enable_command('nextmessage', true);
          if (this.env.prev_uid)
            this.enable_command('previousmessage', true);
          }

        if (this.env.action=='show' && this.env.blockedobjects)
          {
          if (this.gui_objects.remoteobjectsmsg)
            this.gui_objects.remoteobjectsmsg.style.display = 'block';
          this.enable_command('load-images', true);
          }  

        if (this.env.action=='compose')
          this.enable_command('add-attachment', 'send-attachment', 'send', true);
          
        if (this.env.messagecount)
          this.enable_command('select-all', 'select-none', true);

        this.set_page_buttons();

        // focus this window
        window.focus();

        // init message compose form
        if (this.env.action=='compose')
          this.init_messageform();

        // show printing dialog
        if (this.env.action=='print')
          window.print();
/*SID ajout pour la redimention de la frame*/
/*parent.frameSize((parseInt(document.getElementById("messagelist").offsetHeight) + 200) +'px','javascript');
  */      break;


      case 'addressbook':
        var contacts_list = this.gui_objects.contactslist;
        if (contacts_list)
          this.init_contactslist(contacts_list);
      
        this.set_page_buttons();
          
        if (this.env.cid)
          this.enable_command('show', 'edit', true);

        if ((this.env.action=='add' || this.env.action=='edit') && this.gui_objects.editform)
          this.enable_command('save', true);
      
        this.enable_command('list', 'add', true);
        break;


      case 'settings':
        this.enable_command('preferences', 'identities', 'save', 'folders', true);
        
        if (this.env.action=='identities' || this.env.action=='edit-identity' || this.env.action=='add-identity')
          this.enable_command('edit', 'add', 'delete', true);

        if (this.env.action=='edit-identity' || this.env.action=='add-identity')
          this.enable_command('save', true);
          
        if (this.env.action=='folders')
          this.enable_command('subscribe', 'unsubscribe', 'create-folder', 'delete-folder', true);
          
        var identities_list = this.gui_objects.identitieslist;
        if (identities_list)
          this.init_identitieslist(identities_list);

        break;

      case 'login':
        var input_user = rcube_find_object('_user');
        var input_pass = rcube_find_object('_pass');
        if (input_user && input_user.value=='')
          input_user.focus();
        else if (input_pass)
          input_pass.focus();
          
        this.enable_command('login', true);
        break;
      
      default:
        break;
      }


    // enable basic commands
    this.enable_command('logout', true);

    // disable browser's contextmenus
    //document.oncontextmenu = function(){ return false; }

    // flag object as complete
    this.loaded = true;
          
    // show message
    if (this.pending_message)
      this.display_message(this.pending_message[0], this.pending_message[1]);
    };


  // get all message rows from HTML table and init each row
  this.init_messagelist = function(msg_list)
    {
    if (msg_list && msg_list.tBodies[0])
      {
      this.message_rows = new Array();

      var row;
      for(var r=0; r<msg_list.tBodies[0].childNodes.length; r++)
        {
        row = msg_list.tBodies[0].childNodes[r];
        //row = msg_list.tBodies[0].rows[r];
        this.init_message_row(row);
        }
      }
      
    // alias to common rows array
    this.list_rows = this.message_rows;
    };
    
    
  // make references in internal array and set event handlers
  this.init_message_row = function(row)
    {
    var uid, msg_icon;
    
    if (String(row.id).match(/rcmrow([0-9]+)/))
      {
      uid = RegExp.$1;
      row.uid = uid;
              
      this.message_rows[uid] = {id:row.id, obj:row,
                                classname:row.className,
                                unread:this.env.messages[uid] ? this.env.messages[uid].unread : null,
                                replied:this.env.messages[uid] ? this.env.messages[uid].replied : null};
              
      // set eventhandlers to table row
      row.onmousedown = function(e){ return rcube_webmail_client.drag_row(e, this.uid); };
      row.onmouseup = function(e){ return rcube_webmail_client.click_row(e, this.uid); };
              
      // set eventhandler to message icon
      if ((msg_icon = row.cells[0].childNodes[0]) && row.cells[0].childNodes[0].nodeName=='IMG')
        {                
        msg_icon.id = 'msgicn_'+uid;
        msg_icon._row = row;
        msg_icon.onmousedown = function(e) { rcube_webmail_client.command('markread', this); };
                
        // get message icon and save original icon src
        this.message_rows[uid].icon = msg_icon;
        }
      }
    };


  // init message compose form: set focus and eventhandlers
  this.init_messageform = function()
    {
    if (!this.gui_objects.messageform)
      return false;
    
    //this.messageform = this.gui_objects.messageform;
  /*  var input_to = rcube_find_object('_to');
    var input_cc = rcube_find_object('_cc');
    var input_bcc = rcube_find_object('_bcc');
    var input_replyto = rcube_find_object('_replyto');
    var input_subject = rcube_find_object('_subject');
    var input_message = rcube_find_object('_message');
    
    // init live search events
    if (input_to)
      this.init_address_input_events(input_to);
    if (input_cc)
      this.init_address_input_events(input_cc);
    if (input_bcc)
      this.init_address_input_events(input_bcc);
    if (input_to && input_to.value=='')
      input_to.focus();
    else if (input_subject && input_subject.value=='')
      input_subject.focus();
    else if (input_message)
      this.set_caret2start(input_message); // input_message.focus();
*/
    };


  this.init_address_input_events = function(obj)
    {
    var handler = function(e){ return rcube_webmail_client.ksearch_keypress(e,this); };
    var handler2 = function(e){ return rcube_webmail_client.ksearch_blur(e,this); };
	
    if (bw.safari)
      {
      obj.addEventListener('keydown', handler, false);
      // obj.addEventListener('blur', handler2, false);
      }
    else if (bw.mz)
      {
      obj.addEventListener('keypress', handler, false);
      obj.addEventListener('blur', handler2, false);
      }
    else if (bw.ie)
      {
      obj.onkeydown = handler;
      //obj.attachEvent('onkeydown', handler);
      // obj.attachEvent('onblur', handler2, false);
      }
	
    obj.setAttribute('autocomplete', 'off');       
    };



  // get all contact rows from HTML table and init each row
  this.init_contactslist = function(contacts_list)
    {
    if (contacts_list && contacts_list.tBodies[0])
      {
      this.contact_rows = new Array();

      var row;
      for(var r=0; r<contacts_list.tBodies[0].childNodes.length; r++)
        {
        row = contacts_list.tBodies[0].childNodes[r];
        this.init_table_row(row, 'contact_rows');
        }
      }

    // alias to common rows array
    this.list_rows = this.contact_rows;
    
    if (this.env.cid)
      this.select(this.env.cid);
    };


  // make references in internal array and set event handlers
  this.init_table_row = function(row, array_name)
    {
    var cid;
    
    if (String(row.id).match(/rcmrow([0-9]+)/))
      {
      cid = RegExp.$1;
      row.cid = cid;

      this[array_name][cid] = {id:row.id,
                               obj:row,
                               classname:row.className};

      // set eventhandlers to table row
      row.onmousedown = function(e) { rcube_webmail_client.in_selection_before=this.cid; return false; };  // fake for drag handler
      row.onmouseup = function(e){ return rcube_webmail_client.click_row(e, this.cid); };
      }
    };


  // get all contact rows from HTML table and init each row
  this.init_identitieslist = function(identities_list)
    {
    if (identities_list && identities_list.tBodies[0])
      {
      this.identity_rows = new Array();

      var row;
      for(var r=0; r<identities_list.tBodies[0].childNodes.length; r++)
        {
        row = identities_list.tBodies[0].childNodes[r];
        this.init_table_row(row, 'identity_rows');
        }
      }

    // alias to common rows array
    this.list_rows = this.identity_rows;
    
    if (this.env.iid)
      this.select(this.env.iid);    
    };
    


  /*********************************************************/
  /*********       client command interface        *********/
  /*********************************************************/


  // execute a specific command on the web client
  this.command = function(command, props, obj)
    {
    if (obj && obj.blur)
      obj.blur();

    if (this.busy)
      return false;

    // command not supported or allowed
    if (!this.commands[command])
      {
      // pass command to parent window
      if (this.env.framed && parent.rcmail && parent.rcmail.command)
        parent.rcmail.command(command, props);

      return false;
      }

    // process command
    switch (command)
      {
      case 'login':
        if (this.gui_objects.loginform)
          this.gui_objects.loginform.submit();
        break;

      case 'logout':
        location.href = this.env.comm_path+'&_action=logout';
        break;      

      // commands to switch task
      case 'mail':
      case 'addressbook':
      case 'settings':
        this.switch_task(command);
        break;


      // misc list commands
      case 'list':
        if (this.task=='mail')
          this.list_mailbox(props);
        else if (this.task=='addressbook')
          this.list_contacts();
        break;

      case 'nextpage':
        this.list_page('next');
        break;

      case 'previouspage':
        this.list_page('prev');
        break;


      // common commands used in multiple tasks
      case 'show':
        if (this.task=='mail')
          {
          var uid = this.get_single_uid();
          if (uid && (!this.env.uid || uid != this.env.uid))
            this.show_message(uid);
          }
        else if (this.task=='addressbook')
          {
          var cid = props ? props : this.get_single_cid();
          if (cid && !(this.env.action=='show' && cid==this.env.cid))
            this.load_contact(cid, 'show');
          }
        break;

      case 'add':
        if (this.task=='addressbook')
          this.load_contact(0, 'add');
        else if (this.task=='settings')
          {
          this.clear_selection();
          this.load_identity(0, 'add-identity');
          }
        break;

      case 'edit':
        var cid;
        if (this.task=='addressbook' && (cid = this.get_single_cid()))
          this.load_contact(cid, 'edit');
        else if (this.task=='settings' && props)
          this.load_identity(props, 'edit-identity');
        break;

      case 'save-identity':
      case 'save':
        if (this.gui_objects.editform)
          this.gui_objects.editform.submit();
        break;

      case 'delete':
        // mail task
        if (this.task=='mail' && this.env.trash_mailbox && String(this.env.mailbox).toLowerCase()!=String(this.env.trash_mailbox).toLowerCase())
          this.move_messages(this.env.trash_mailbox);
        else if (this.task=='mail')
          this.delete_messages();
        // addressbook task
        else if (this.task=='addressbook')
          this.delete_contacts();
        // user settings task
        else if (this.task=='settings')
          this.delete_identity();
        break;


      // mail task commands
      case 'move':
      case 'moveto':
        this.move_messages(props);
        break;
        
      case 'markread':
        if (props && !props._row)
          break;
        
        var uid;
        var flag = 'read';
        
        if (props._row.uid)
          {
          uid = props._row.uid;
          this.dont_select = true;
          
          // toggle read/unread
          if (!this.message_rows[uid].unread)
            flag = 'unread';
          }
          
        this.mark_message(flag, uid);
        break;
        
      case 'load-images':
        if (this.env.uid)
          this.show_message(this.env.uid, true);
        break;

      case 'load-attachment':
        var url = this.env.comm_path+'&_action=get&_mbox='+this.env.mailbox+'&_uid='+this.env.uid+'&_part='+props.part;
        
        // open attachment in frame if it's of a supported mimetype
        if (this.env.uid && props.mimetype && find_in_array(props.mimetype, this.mimetypes)>=0)
          {
          this.attachment_win = window.open(url+'&_frame=1', 'rcubemailattachment');
          if (this.attachment_win)
            {
            setTimeout(this.ref+'.attachment_win.focus()', 10);
            break;
            }
          }

        location.href = url;
        break;
        
      case 'select-all':
        this.select_all(props);
        break;

      case 'select-none':
        this.clear_selection();
        break;

      case 'nextmessage':
        if (this.env.next_uid)
          this.show_message(this.env.next_uid);
          //location.href = this.env.comm_path+'&_action=show&_uid='+this.env.next_uid+'&_mbox='+this.env.mailbox;
        break;

      case 'previousmessage':
        if (this.env.prev_uid)
          this.show_message(this.env.prev_uid);
          //location.href = this.env.comm_path+'&_action=show&_uid='+this.env.prev_uid+'&_mbox='+this.env.mailbox;
        break;

      case 'compose':
        var url = this.env.comm_path+'&_action=compose';
        
        // modify url if we're in addressbook
        if (this.task=='addressbook')
          {
          url = this.get_task_url('mail', url);            
          var a_cids = new Array();
          
          // use contact_id passed as command parameter
          if (props)
            a_cids[a_cids.length] = props;
            
          // get selected contacts
          else
            {
            for (var n=0; n<this.selection.length; n++)
              a_cids[a_cids.length] = this.selection[n];
            }

          if (a_cids.length)
            url += '&_to='+a_cids.join(',');
          else
            break;
          }
        else if (props)
           url += '&_to='+props;

        this.set_busy(true);
        location.href = url;
        break;      

      case 'send':
        if (!this.gui_objects.messageform)
          break;
          
        // check input fields
        var input_to = rcube_find_object('_to');
        var input_subject = rcube_find_object('_subject');
        var input_message = rcube_find_object('_message');
        
        if (input_to.value!='' && input_message.value!='')
          {
          this.set_busy(true, 'sendingmessage');
          var form = this.gui_objects.messageform;
          form.submit();
          }
          
        break;

      case 'add-attachment':
        this.show_attachment_form(true);
        
      case 'send-attachment':
        this.upload_file(props)      
        break;

      case 'reply':
        var uid;
        if (uid = this.get_single_uid())
          {
          this.set_busy(true);
          location.href = this.env.comm_path+'&_action=compose&_reply_uid='+uid+'&_mbox='+escape(this.env.mailbox);
          }
        break;      

      case 'forward':
        var uid;
        if (uid = this.get_single_uid())
          {
          this.set_busy(true);
          location.href = this.env.comm_path+'&_action=compose&_forward_uid='+uid+'&_mbox='+escape(this.env.mailbox);
          }
        break;
        
      case 'print':
        var uid;
        if (uid = this.get_single_uid())
          {
          this.printwin = window.open(this.env.comm_path+'&_action=print&_uid='+uid+'&_mbox='+escape(this.env.mailbox)+(this.env.safemode ? '&_safe=1' : ''));
          if (this.printwin)
            setTimeout(this.ref+'.printwin.focus()', 20);
          }
        break;

      case 'viewsource':
        var uid;
        if (uid = this.get_single_uid())
          {          
          this.sourcewin = window.open(this.env.comm_path+'&_action=viewsource&_uid='+this.env.uid+'&_mbox='+escape(this.env.mailbox));
          if (this.sourcewin)
            setTimeout(this.ref+'.sourcewin.focus()', 20);
          }
        break;

      case 'add-contact':
        this.add_contact(props);
        break;


      // user settings commands
      case 'preferences':
        location.href = this.env.comm_path;
        break;

      case 'identities':
        location.href = this.env.comm_path+'&_action=identities';
        break;
          
      case 'delete-identity':
        this.delete_identity();
        
      case 'folders':
        location.href = this.env.comm_path+'&_action=folders';
        break;

      case 'subscribe':
        this.subscribe_folder(props);
        break;

      case 'unsubscribe':
        this.unsubscribe_folder(props);
        break;
        
      case 'create-folder':
        this.create_folder(props);
        break;

      case 'delete-folder':
        if (confirm('Do you really want to delete this folder?'))
          this.delete_folder(props);
        break;

      }

    return obj ? false : true;
    };


  // set command enabled or disabled
  this.enable_command = function()
    {
    var args = this.enable_command.arguments;
    if(!args.length) return -1;

    var command;
    var enable = args[args.length-1];
   
    };

  this.get_task_url = function(task, url)
    {
    if (!url)
      url = this.env.comm_path;

    return url.replace(/_task=[a-z]+/, '_task='+task);
    };
    
  /*********************************************************/
  /*********     keyboard live-search methods      *********/
  /*********************************************************/


  // handler for keyboard events on address-fields
  this.ksearch_keypress = function(e, obj)
    {
    if (typeof(this.env.contacts)!='object' || !this.env.contacts.length)
      return true;

    if (this.ksearch_timer)
      clearTimeout(this.ksearch_timer);

    if (!e)
      e = window.event;
      
    var highlight;
    var key = e.keyCode ? e.keyCode : e.which;

    switch (key)
      {
      case 38:  // key up
      case 40:  // key down
        if (!this.ksearch_pane)
          break;
          
        var dir = key==38 ? 1 : 0;
        var next;
        
        highlight = document.getElementById('rcmksearchSelected');
        if (!highlight)
          highlight = this.ksearch_pane.ul.firstChild;
        
        if (highlight && (next = dir ? highlight.previousSibling : highlight.nextSibling))
          {
          highlight.removeAttribute('id');
          //highlight.removeAttribute('class');
          this.set_classname(highlight, 'selected', false);
          }

        if (next)
          {
          next.setAttribute('id', 'rcmksearchSelected');
          this.set_classname(next, 'selected', true);
          this.ksearch_selected = next._rcm_id;
          }

        if (e.preventDefault)
          e.preventDefault();
        return false;

      case 9:  // tab
        if(e.shiftKey)
          break;

      case 13:  // enter     
        if (this.ksearch_selected===null || !this.ksearch_input || !this.ksearch_value)
          break;

        // get cursor pos
        var inp_value = this.ksearch_input.value.toLowerCase();
        var cpos = this.get_caret_pos(this.ksearch_input);
        var p = inp_value.lastIndexOf(this.ksearch_value, cpos);
        
        // replace search string with full address
        var pre = this.ksearch_input.value.substring(0, p);
        var end = this.ksearch_input.value.substring(p+this.ksearch_value.length, this.ksearch_input.value.length);
        var insert = this.env.contacts[this.ksearch_selected]+this.separator;
        this.ksearch_input.value = pre + insert + end;
        
        //this.ksearch_input.value = this.ksearch_input.value.substring(0, p)+insert;
        
        // set caret to insert pos
        cpos = p+insert.length;
        if (this.ksearch_input.setSelectionRange)
          this.ksearch_input.setSelectionRange(cpos, cpos);
        
        // hide ksearch pane
        this.ksearch_hide();
      
        if (e.preventDefault)
          e.preventDefault();
        return false;

      case 27:  // escape
        this.ksearch_hide();
        break;

      }

    // start timer
    this.ksearch_timer = setTimeout(this.ref+'.ksearch_get_results()', 200);      
    this.ksearch_input = obj;
    
    return true;
    };

  this.reselect = function(id){ 	
   highlight = document.getElementById('rcmksearchSelected');
 		obj = document.getElementById('span'+id).parentNode;
   this.set_classname(highlight, 'selected', false);
   this.set_classname(obj, 'selected', true);
   highlight.removeAttribute('id');
			this.ksearch_selected = obj._rcm_id;
   obj.setAttribute('id', 'rcmksearchSelected');	
		}

this.thisone = function(id){
 		obj = document.getElementById('span'+id).parentNode;
// get cursor pos
        var inp_value = this.ksearch_input.value.toLowerCase();
        var cpos = this.get_caret_pos(this.ksearch_input);
        var p = inp_value.lastIndexOf(this.ksearch_value, cpos);
        
        // replace search string with full address
        var pre = this.ksearch_input.value.substring(0, p);
        var end = this.ksearch_input.value.substring(p+this.ksearch_value.length, this.ksearch_input.value.length);
        var insert = this.env.contacts[this.ksearch_selected]+this.separator;
        this.ksearch_input.value = pre + insert + end;
        
        //this.ksearch_input.value = this.ksearch_input.value.substring(0, p)+insert;
        
        // set caret to insert pos
        cpos = p+insert.length;
        if (this.ksearch_input.setSelectionRange)
          this.ksearch_input.setSelectionRange(cpos, cpos);
        
        // hide ksearch pane
        this.ksearch_hide();

								
}

		
  // address search processor
  this.ksearch_get_results = function()
    {
    var inp_value = this.ksearch_input ? this.ksearch_input.value : null;
    if (inp_value===null)
      return;

    // get string from current cursor pos to last comma
    var cpos = this.get_caret_pos(this.ksearch_input);
    var p = inp_value.lastIndexOf(this.separator, cpos-1);
    var q = inp_value.substring(p+1, cpos);

    // trim query string
    q = q.replace(/(^\s+|\s+$)/g, '').toLowerCase();

    if (!q.length || q==this.ksearch_value)
      {
      if (!q.length && this.ksearch_pane && this.ksearch_pane.visible)
        this.ksearch_pane.show(0);

      return;
      }

    this.ksearch_value = q;
    
    // start searching the contact list
    var a_results = new Array();
    var a_result_ids = new Array();
    var c=0;
    for (var i=0; i<this.env.contacts.length; i++)
      {
      if (this.env.contacts[i].toLowerCase().indexOf(q)>=0)
        {
        a_results[c] = this.env.contacts[i];
        a_result_ids[c++] = i;
        
        if (c==15)  // limit search results
          break;
        }
      }

    // display search results
    if (c && a_results.length)
      {
      var p, ul, li;
      
      // create results pane if not present
      if (!this.ksearch_pane)
        {
        ul = document.createElement('UL');
        this.ksearch_pane = new rcube_layer('rcmKSearchpane', {vis:0, zindex:30000});
        this.ksearch_pane.elm.appendChild(ul);
        this.ksearch_pane.ul = ul;
        }
      else
        ul = this.ksearch_pane.ul;

      // remove all search results
      ul.innerHTML = '';
            
      // add each result line to list
      for (i=0; i<a_results.length; i++)
        {
        li = document.createElement('LI');
        li.innerHTML = '<span id="span'+a_result_ids[i]+'" onmousedown="suggest.thisone('+a_result_ids[i]+')" onmouseover="suggest.reselect('+a_result_ids[i]+');">'+a_results[i].replace(/</, '&lt;').replace(/>/, '&gt;')+'</span>';
        li._rcm_id = a_result_ids[i];
        ul.appendChild(li);
        }

      // check if last selected item is still in result list
      if (this.ksearch_selected!==null)
        {
        p = find_in_array(this.ksearch_selected, a_result_ids);
        if (p>=0 && ul.childNodes)
          {
          ul.childNodes[p].setAttribute('id', 'rcmksearchSelected');
          this.set_classname(ul.childNodes[p], 'selected', true);
          }
        else
          this.ksearch_selected = null;
        }
      
      // if no item selected, select the first one
      if (this.ksearch_selected===null)
        {
        ul.firstChild.setAttribute('id', 'rcmksearchSelected');
        this.set_classname(ul.firstChild, 'selected', true);
        this.ksearch_selected = a_result_ids[0];
        }

      // resize the containing layer to fit the list
      //this.ksearch_pane.resize(ul.offsetWidth, ul.offsetHeight);
    
      // move the results pane right under the input box and make it visible
      var pos = rcube_get_object_pos(this.ksearch_input);
      this.ksearch_pane.move(pos.x, pos.y+this.ksearch_input.offsetHeight);
      this.ksearch_pane.show(1); 
      }
    // hide results pane
    else
      this.ksearch_hide();
    };


  this.ksearch_blur = function(e, obj)
    {
    if (this.ksearch_timer)
      clearTimeout(this.ksearch_timer);

    this.ksearch_value = '';      
    this.ksearch_input = null;
    
    this.ksearch_hide();
    };


  this.ksearch_hide = function()
    {
    this.ksearch_selected = null;
    
    if (this.ksearch_pane)
      this.ksearch_pane.show(0);    

this.ksearch_input.focus();
    };



  /*********************************************************/
  /*********         address book methods          *********/
  /*********************************************************/


  this.list_contacts = function(page)
    {
    var add_url = '';
    var target = window;
    
    if (page && this.current_page==page)
      return false;

    // load contacts remotely
    if (this.gui_objects.contactslist)
      {
      this.list_contacts_remote(page);
      return;
      }

    if (this.env.contentframe && window.frames && window.frames[this.env.contentframe])
      {
      target = window.frames[this.env.contentframe];
      add_url = '&_framed=1';
      }

    this.set_busy(true, 'loading');
    location.href = this.env.comm_path+(page ? '&_page='+page : '')+add_url;
    };


  // send remote request to load contacts list
  this.list_contacts_remote = function(page)
    {
    // clear list
    var table = this.gui_objects.contactslist;
    var tbody = document.createElement('TBODY');
    table.insertBefore(tbody, table.tBodies[0]);
    table.tBodies[1].style.display = 'none';
    
    this.contact_rows = new Array();
    this.list_rows = this.contact_rows;

    // send request to server
    var url = page ? '&_page='+page : '';
    this.set_busy(true, 'loading');
    this.http_request('list', url);
    };


  // load contact record
  this.load_contact = function(cid, action, framed)
    {
    var add_url = '';
    var target = window;
    if (this.env.contentframe && window.frames && window.frames[this.env.contentframe])
      {
      add_url = '&_framed=1';
      target = window.frames[this.env.contentframe];
      document.getElementById(this.env.contentframe).style.visibility = 'inherit';
      }
    else if (framed)
      return false;
      
    //if (this.env.framed && add_url=='')
    //  add_url = '&_framed=1';
    
    if (action && (cid || action=='add'))
      {
      this.set_busy(true);
      target.location.href = this.env.comm_path+'&_action='+action+'&_cid='+cid+add_url;
      }
    };


  this.delete_contacts = function()
    {
    // exit if no mailbox specified or if selection is empty
    if (!(this.selection.length || this.env.cid))
      return;
    
    var a_cids = new Array();

    if (this.env.cid)
      a_cids[a_cids.length] = this.env.cid;
    else
      {
      var id;
      for (var n=0; n<this.selection.length; n++)
        {
        id = this.selection[n];
        a_cids[a_cids.length] = id;
      
        // 'remove' row from list (just hide it)
        if (this.contact_rows[id].obj)
          this.contact_rows[id].obj.style.display = 'none';
        }

      // hide content frame if we delete the currently displayed contact
      if (this.selection.length==1 && this.env.contentframe)
        {
        var elm = document.getElementById(this.env.contentframe);
        elm.style.visibility = 'hidden';
        }
      }

    // send request to server
    this.http_request('delete', '_cid='+a_cids.join(',')+'&_from='+(this.env.action ? this.env.action : ''));
    };


  // update a contact record in the list
  this.update_contact_row = function(cid, cols_arr)
    {
    if (!this.contact_rows[cid] || !this.contact_rows[cid].obj)
      return false;
      
    var row = this.contact_rows[cid].obj;
    for (var c=0; c<cols_arr.length; c++)
      if (row.cells[c])
        row.cells[c].innerHTML = cols_arr[c];

    };
  


  /*********************************************************/
  /*********        user settings methods          *********/
  /*********************************************************/


  // load contact record
  this.load_identity = function(id, action)
    {
    if (action=='edit-identity' && (!id || id==this.env.iid))
      return;

    var add_url = '';
    var target = window;
    if (this.env.contentframe && window.frames && window.frames[this.env.contentframe])
      {
      add_url = '&_framed=1';
      target = window.frames[this.env.contentframe];
      document.getElementById(this.env.contentframe).style.visibility = 'inherit';
      }

    if (action && (id || action=='add-identity'))
      {
      this.set_busy(true);
      target.location.href = this.env.comm_path+'&_action='+action+'&_iid='+id+add_url;
      }
    };



  this.delete_identity = function(id)
    {
    // exit if no mailbox specified or if selection is empty
    if (!(this.selection.length || this.env.iid))
      return;
    
    if (!id)
      id = this.env.iid ? this.env.iid : this.selection[0];

/*
    // 'remove' row from list (just hide it)
    if (this.identity_rows && this.identity_rows[id].obj)
      {
      this.clear_selection();
      this.identity_rows[id].obj.style.display = 'none';
      }
*/

    // if (this.env.framed && id)
      this.set_busy(true);
      location.href = this.env.comm_path+'&_action=delete-identity&_iid='+id;
    // else if (id)
    //  this.http_request('delete-identity', '_iid='+id);
    };


  this.create_folder = function(name)
    {
    var form;
    if ((form = this.gui_objects.editform) && form.elements['_folder_name'])
      name = form.elements['_folder_name'].value;

    if (name)
      this.http_request('create-folder', '_name='+escape(name));
    else if (form.elements['_folder_name'])
      form.elements['_folder_name'].focus();
    };


  this.delete_folder = function(folder)
    {
    if (folder)
      {
      for (var id in this.env.subscriptionrows)
        if (this.env.subscriptionrows[id]==folder)
          break;
          
      var row;
      if (id && (row = document.getElementById(id)))
        row.style.display = 'none';

      this.http_request('delete-folder', '_mboxes='+escape(folder));
      }
    };


  this.subscribe_folder = function(folder)
    {
    var form;
    if ((form = this.gui_objects.editform) && form.elements['_unsubscribed'])
      this.change_subscription('_unsubscribed', '_subscribed', 'subscribe');
    else if (folder)
      this.http_request('subscribe', '_mboxes='+escape(folder));
    };


  this.unsubscribe_folder = function(folder)
    {
    var form;
    if ((form = this.gui_objects.editform) && form.elements['_subscribed'])
      this.change_subscription('_subscribed', '_unsubscribed', 'unsubscribe');
    else if (folder)
      this.http_request('unsubscribe', '_mboxes='+escape(folder));
    };
    

  this.change_subscription = function(from, to, action)
    {
    var form;
    if (form = this.gui_objects.editform)
      {
      var a_folders = new Array();
      var list_from = form.elements[from];

      for (var i=0; list_from && i<list_from.options.length; i++)
        {
        if (list_from.options[i] && list_from.options[i].selected)
          {
          a_folders[a_folders.length] = list_from.options[i].value;
          list_from[i] = null;
          i--;
          }
        }

      // yes, we have some folders selected
      if (a_folders.length)
        {
        var list_to = form.elements[to];
        var index;
        
        for (var n=0; n<a_folders.length; n++)
          {
          index = list_to.options.length;
          list_to[index] = new Option(a_folders[n]);
          }
          
        this.http_request(action, '_mboxes='+escape(a_folders.join(',')));
        }
      }
      
    };


   // add a new folder to the subscription list by cloning a folder row
   this.add_folder_row = function(name)
     {
     if (!this.gui_objects.subscriptionlist)
       return false;

     var tbody = this.gui_objects.subscriptionlist.tBodies[0];
     var id = tbody.childNodes.length+1;
     
     // clone a table row
     var row = this.clone_table_row(tbody.rows[0]);
     row.id = 'rcmrow'+id;
     tbody.appendChild(row);

     // add to folder/row-ID map
     this.env.subscriptionrows[row.id] = name;

     // set folder name
     row.cells[0].innerHTML = name;
     if (row.cells[1].firstChild.tagName=='INPUT')
       {
       row.cells[1].firstChild.value = name;
       row.cells[1].firstChild.checked = true;
       }
     if (row.cells[2].firstChild.tagName=='A')
       row.cells[2].firstChild.onclick = new Function(this.ref+".command('delete-folder','"+name+"')");

    var form;
    if ((form = this.gui_objects.editform) && form.elements['_folder_name'])
      form.elements['_folder_name'].value = '';
     };


  // duplicate a specific table row
  this.clone_table_row = function(row)
    {
    var cell, td;
    var new_row = document.createElement('TR');
    for(var n=0; n<row.childNodes.length; n++)
      {
      cell = row.childNodes[n];
      td = document.createElement('TD');

      if (cell.className)
        td.className = cell.className;
      if (cell.align)
        td.setAttribute('align', cell.align);
        
      td.innerHTML = cell.innerHTML;
      new_row.appendChild(td);
      }
    
    return new_row;
    };


  /*********************************************************/
  /*********           GUI functionality           *********/
  /*********************************************************/


  // eable/disable buttons for page shifting
  this.set_page_buttons = function()
    {
    this.enable_command('nextpage', (this.env.pagecount > this.env.current_page));
    this.enable_command('previouspage', (this.env.current_page > 1));
    }


  // set button to a specific state
 
  // set/unset a specific class name
  this.set_classname = function(obj, classname, set)
    {
    var reg = new RegExp('\s*'+classname, 'i');
    if (!set && obj.className.match(reg))
      obj.className = obj.className.replace(reg, '');
    else if (set && !obj.className.match(reg))
      obj.className += ' '+classname;
    };


  // display a specific alttext
  this.alttext = function(text)
    {
    
    };


  // display a system message
  this.display_message = function(msg, type, hold)
    {
    if (!this.loaded)  // save message in order to display after page loaded
      {
      this.pending_message = new Array(msg, type);
      return true;
      }
    
    if (!this.gui_objects.message)
      return false;
      
    if (this.message_timer)
      clearTimeout(this.message_timer);
    
    var cont = msg;
    if (type)
      cont = '<div class="'+type+'">'+cont+'</div>';

    this.gui_objects.message._rcube = this;
    this.gui_objects.message.innerHTML = cont;
    this.gui_objects.message.style.display = 'block';
    
    if (type!='loading')
      this.gui_objects.message.onmousedown = function(){ this._rcube.hide_message(); return true; };
    
    if (!hold)
      this.message_timer = setTimeout(this.ref+'.hide_message()', this.message_time);
    };


  // make a message row disapear
  this.hide_message = function()
    {
    if (this.gui_objects.message)
      {
      this.gui_objects.message.style.display = 'none';
      this.gui_objects.message.onmousedown = null;
      }
    };


  // mark a mailbox as selected and set environment variable
  this.select_mailbox = function(mbox)
    {
    if (this.gui_objects.mailboxlist)
      {
      var item, reg, text_obj;
      var s_mbox = String(mbox).toLowerCase().replace(this.mbox_expression, '');
      var s_current = this.env.mailbox.toLowerCase().replace(this.mbox_expression, '');
      var nodes = this.gui_objects.mailboxlist.getElementsByTagName('LI');
      
      for (var n=0; n<nodes.length; n++)
        {
        item = nodes[n];
        if (item.className && item.className.indexOf('mailbox '+s_mbox+' ')>=0)
          this.set_classname(item, 'selected', true);
        else if (item.className && item.className.indexOf('mailbox '+s_current)>=0)
          this.set_classname(item, 'selected', false);          
        }
      }
    
    this.env.mailbox = mbox;
    };


  // create a table row in the message list
  this.add_message_row = function(uid, cols, flags, attachment)
    {
    if (!this.gui_objects.messagelist || !this.gui_objects.messagelist.tBodies[0])
      return false;
    
    var tbody = this.gui_objects.messagelist.tBodies[0];
    var rowcount = tbody.rows.length;
    var even = rowcount%2;
    
    this.env.messages[uid] = {replied:flags.replied?1:0,
                              unread:flags.unread?1:0};
    
    var row = document.createElement('TR');
    row.id = 'rcmrow'+uid;
    row.className = 'message '+(even ? 'even' : 'odd')+(flags.unread ? ' unread' : '');

    if (this.in_selection(uid))
      row.className += ' selected';

    var icon = flags.unread && this.env.unreadicon ? this.env.unreadicon :
               (flags.replied && this.env.repliedicon ? this.env.repliedicon : this.env.messageicon);

    var col = document.createElement('TD');
    col.className = 'icon';
    col.innerHTML = icon ? '<img src="'+icon+'" alt="" border="0" />' : '';
    row.appendChild(col);

    // add each submitted col
    for (var c in cols)
      {
      col = document.createElement('TD');
      col.className = String(c).toLowerCase();
      col.innerHTML = cols[c];
      row.appendChild(col);
      }

    col = document.createElement('TD');
    col.className = 'icon';
    col.innerHTML = attachment && this.env.attachmenticon ? '<img src="'+this.env.attachmenticon+'" alt="" border="0" />' : '';
    row.appendChild(col);
    
    tbody.appendChild(row);
    this.init_message_row(row);
    };


  // replace content of row count display
  this.set_rowcount = function(text)
    {
    if (this.gui_objects.countdisplay)
      this.gui_objects.countdisplay.innerHTML = text;

    // update page navigation buttons
    this.set_page_buttons();
    };


  // update the mailboxlist
  this.set_unread_count = function(mbox, count)
    {
    if (!this.gui_objects.mailboxlist)
      return false;
      
    mbox = String(mbox).toLowerCase().replace(this.mbox_expression, '');
    
    var item, reg, text_obj;
    for (var n=0; n<this.gui_objects.mailboxlist.childNodes.length; n++)
      {
      item = this.gui_objects.mailboxlist.childNodes[n];

      if (item.className && item.className.indexOf('mailbox '+mbox)>=0)
        {
        // set new text
        text_obj = item.firstChild;
        reg = /\s+\([0-9]+\)$/i;

        if (count && text_obj.innerHTML.match(reg))
          text_obj.innerHTML = text_obj.innerHTML.replace(reg, ' ('+count+')');
        else if (count)
          text_obj.innerHTML += ' ('+count+')';
        else
          text_obj.innerHTML = text_obj.innerHTML.replace(reg, '');
          
        // set the right classes
        this.set_classname(item, 'unread', count>0 ? true : false);        
        break;
        }
      }
    };


  // add row to contacts list
  this.add_contact_row = function(cid, cols)
    {
    if (!this.gui_objects.contactslist || !this.gui_objects.contactslist.tBodies[0])
      return false;
    
    var tbody = this.gui_objects.contactslist.tBodies[0];
    var rowcount = tbody.rows.length;
    var even = rowcount%2;
    
    var row = document.createElement('TR');
    row.id = 'rcmrow'+cid;
    row.className = 'contact '+(even ? 'even' : 'odd');
    
    if (this.in_selection(cid))
      row.className += ' selected';

    // add each submitted col
    for (var c in cols)
      {
      col = document.createElement('TD');
      col.className = String(c).toLowerCase();
      col.innerHTML = cols[c];
      row.appendChild(col);
      }
    
    tbody.appendChild(row);
    this.init_table_row(row, 'contact_rows');
    };



  /********************************************************/
  /*********          drag & drop methods         *********/
  /********************************************************/


  this.drag_mouse_move = function(e)
    {
    if (this.drag_start)
      {
      if (!this.draglayer)
        this.draglayer = new rcube_layer('rcmdraglayer', {x:0, y:0, width:300, vis:0, zindex:2000});
      
      // get subjects of selectedd messages
      var names = '';
      var c, subject, obj;
      for(var n=0; n<this.selection.length; n++)
        {
        if (n>12)  // only show 12 lines
          {
          names += '...';
          break;
          }

        if (this.message_rows[this.selection[n]].obj)
          {
          obj = this.message_rows[this.selection[n]].obj;
          subject = '';

          for(c=0; c<obj.childNodes.length; c++)
            if (!subject && obj.childNodes[c].nodeName=='TD' && obj.childNodes[c].firstChild && obj.childNodes[c].firstChild.nodeType==3)
              {
              subject = obj.childNodes[c].firstChild.data;
              names += (subject.length > 50 ? subject.substring(0, 50)+'...' : subject) + '<br />';
              }
          }
        }
        
      this.draglayer.write(names);
      this.draglayer.show(1);
      }

    var pos = this.get_mouse_pos(e);
    this.draglayer.move(pos.x+20, pos.y-5);
    
    this.drag_start = false;
    this.drag_active = true;
    
    return false;
    };


  this.drag_mouse_up = function()
    {
    document.onmousemove = null;
    
    if (this.draglayer && this.draglayer.visible)
      this.draglayer.show(0);
      
    this.drag_active = false;
    
    return false;
    };



  /********************************************************/
  /*********        remote request methods        *********/
  /********************************************************/


  // send a http request to the server
  this.http_request = function(action, querystring)
    {
    if (window.XMLHttpRequest)
      this.request_obj = new XMLHttpRequest();
    else if (window.ActiveXObject)
      this.request_obj = new ActiveXObject("Microsoft.XMLHTTP");
    else
      {
      
      }

    querystring += '&_remote=1';
    
    // add timestamp to request url to avoid cacheing problems in Safari
    if (bw.safari)
      querystring += '&_ts='+(new Date().getTime());

    // send request
    if (this.request_obj)
      {
      // prompt('request', this.env.comm_path+'&_action='+escape(action)+'&'+querystring);
      console('HTTP request: '+this.env.comm_path+'&_action='+escape(action)+'&'+querystring);
      this.set_busy(true);
      this.request_action = action;
      this.request_obj.onreadystatechange = function(){ rcube_webmail_client.http_response();
						
						parent.frameSize((parseInt(document.getElementById("messagelist").offsetHeight) + 140) +'px','javascript');
						
						};
      this.request_obj.open('GET', this.env.comm_path+'&_action='+escape(action)+'&'+querystring);
      this.request_obj.send(null);
      }
    };


  // handle http response
  this.http_response = function()
    {
    if (this.request_obj.readyState == 4) // || this.request_obj.readyState == 2)
      {
      var ctype = this.request_obj.getResponseHeader('Content-Type');
      if (ctype)
        ctype = String(ctype).toLowerCase();

      this.set_busy(false);

  console(this.request_obj.responseText);

      // if we get javascript code from server -> execute it
      if (this.request_obj.responseText && (ctype=='text/javascript' || ctype=='application/x-javascript'))
        eval(this.request_obj.responseText);

      // process the response data according to the sent action
      switch (this.request_action)
        {
        case 'delete':
        case 'moveto':
          if (this.env.action=='show')
            this.command('list');
          break;
          
        case 'list':
          this.enable_command('select-all', 'select-none', this.env.messagecount ? true : false);
          break;
        }
      }
    };


  /********************************************************/
  /*********            helper methods            *********/
  /********************************************************/
  
  // check if we're in show mode or if we have a unique selection
  // and return the message uid
  this.get_single_uid = function()
    {
    return this.env.uid ? this.env.uid : (this.selection.length==1 ? this.selection[0] : null);
    };

  // same as above but for contacts
  this.get_single_cid = function()
    {
    return this.env.cid ? this.env.cid : (this.selection.length==1 ? this.selection[0] : null);
    };


  // check if Shift-key is pressed on event
  this.check_shiftkey = function(e)
    {
    if(!e && window.event)
      e = window.event;

    if(bw.linux && bw.ns4 && e.modifiers)
      return true;
    else if((bw.ns4 && e.modifiers & Event.SHIFT_MASK) || (e && e.shiftKey))
      return true;
    else
      return false;
    }


  this.get_mouse_pos = function(e)
    {
    if(!e) e = window.event;
    var mX = (e.pageX) ? e.pageX : e.clientX;
    var mY = (e.pageY) ? e.pageY : e.clientY;

    if(document.body && document.all)
      {
      mX += document.body.scrollLeft;
      mY += document.body.scrollTop;
      }

    return { x:mX, y:mY };
    };
    
  
  this.get_caret_pos = function(obj)
    {
    if (typeof(obj.selectionEnd)!='undefined')
      return obj.selectionEnd;

    else if (document.selection && document.selection.createRange)
      {
      var range = document.selection.createRange();
      if (range.parentElement()!=obj)
        return 0;

      var gm = range.duplicate();
      if (obj.tagName=='TEXTAREA')
        gm.moveToElementText(obj);
      else
        gm.expand('textedit');
      
      gm.setEndPoint('EndToStart', range);
      var p = gm.text.length;

      return p<=obj.value.length ? p : -1;
      }

    else
      return obj.value.length;
    };


  this.set_caret2start = function(obj)
    {
    if (obj.createTextRange)
      {
      var range = obj.createTextRange();
      range.collapse(true);
      range.select();
      }
    else if (obj.setSelectionRange)
      obj.setSelectionRange(0,0);

    obj.focus();
    };


  // set all fields of a form disabled
  this.lock_form = function(form, lock)
    {
    if (!form || !form.elements)
      return;
    
    var type;
    for (var n=0; n<form.elements.length; n++)
      {
      type = form.elements[n];
      if (type=='hidden')
        continue;
        
      form.elements[n].disabled = lock;
      }
    };
    
  }  // end object rcube_webmail




function console(str)
  {
  if (document.debugform && document.debugform.console)
    document.debugform.console.value += str+'\n--------------------------------------\n';
  }


// set onload handler
window.onload = function(e)
  {
  if (window.rcube_webmail_client)
    rcube_webmail_client.init();
  };



var suggest = new rcube_webmail();
suggest.set_env('task', 'mail');
suggest.set_env('action', 'compose');
suggest.gui_object('messageform', 'form');
function suggestInit(liste,id,separator){
 suggest.set_env('contacts',liste);
	idInput = id;
	suggest.separator = separator;
	obj = document.getElementById(id);
 suggest.init_address_input_events(obj);

}
