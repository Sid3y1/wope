/****
Last Modified: 21/03/06 10:12:54

 GoogieSpell
   Google spell checker for your own web-apps :)
   Copyright Amir Salihefendic 2006
 LICENSE
  GPL (see gpl.txt for more information)
  This basically means that you can't use this script with/in proprietary software!
  There is another license that permits you to use this script with proprietary software. Check out:... for more info.
  AUTHOR
   4mir Salihefendic (http://amix.dk) - amix@amix.dk
 VERSION
	 3.0
****/
var GOOGIE_CUR_LANG = "fr";

function GoogieSpell() {
 img_dir = img_dir;
	server_url = server_url;
  var cookie_value;
  var lang;
  cookie_value = getCookie('language');

  if(cookie_value != null)
    GOOGIE_CUR_LANG = cookie_value;

  this.img_dir = img_dir;
  this.server_url = server_url;

  this.lang_to_word = {"da": "Dansk", "de": "Deutsch", "en": "English",
                       "es": "Espa&#241;ol", "fr": "Fran&#231;ais", "it": "Italiano", 
                       "nl": "Nederlands", "pl": "Polski", "pt": "Portugu&#234;s",
                       "fi": "Suomi", "sv": "Svenska"};
  this.langlist_codes = AJS.keys(this.lang_to_word);

  this.show_change_lang_pic = true;

  this.language_status_div = null;

  this.spelling_state_observer = null;

  this.request = null;
  this.error_window = null;
  this.language_window = null;
  this.edit_layer = null;
  this.orginal_text = null;
  this.results = null;
  this.text_area = null;
  this.gselm = null;
  this.cur_lang_span = null;
  this.ta_scroll_top = 0;
  this.el_scroll_top = 0;
}

GoogieSpell.prototype.setStateChanged = function(current_state) {
  if(this.spelling_state_observer != null)
    this.spelling_state_observer(current_state);
}

GoogieSpell.item_onmouseover = function(e) {
  var elm = GoogieSpell.getEventElm(e);
  if(elm.className != "googie_list_close" && elm.className != "googie_list_revert")
    elm.className = "googie_list_onhover";
  else
    elm.parentNode.className = "googie_list_onhover";
}

GoogieSpell.item_onmouseout = function(e) {
  var elm = GoogieSpell.getEventElm(e);
  if(elm.className != "googie_list_close" && elm.className != "googie_list_revert")
    elm.className = "googie_list_onout";
  else
    elm.parentNode.className = "googie_list_onout";
}

GoogieSpell.prototype.getGoogleUrl = function() {
  return this.server_url + GOOGIE_CUR_LANG;
}

GoogieSpell.prototype.spellCheck = function(elm, name) {
  TA_SCROLL_TOP = this.text_area.scrollTop;
  this.appendIndicator(elm);

  try {
    this.hideLangWindow();
  }
  catch(e) {}
  
  this.gselm = elm;

  this.createEditLayer(this.text_area.offsetWidth, this.text_area.offsetHeight);

  this.createErrorWindow();
  AJS.getBody().appendChild(this.error_window);

  try { netscape.security.PrivilegeManager.enablePrivilege("UniversalBrowserRead"); } 
  catch (e) { }

  this.gselm.onclick = null;

  this.orginal_text = this.text_area.value;
  var me = this;

  //Create request
  var d = AJS.getRequest(this.getGoogleUrl());
  var reqdone = function(req) {
    var r_text = req.responseText;
    if(r_text.match(/<c.*>/) != null) {
      var results = GoogieSpell.parseResult(r_text);
      //Before parsing be sure that errors were found
      me.results = results;
      me.showErrorsInIframe(results);

      me.resumeEditingState();
    }
    else {
      me.flashNoSpellingErrorState();
    }
    me.removeIndicator();
  };
  d.addCallback(reqdone);

  var req_text = GoogieSpell.escapeSepcial(this.orginal_text);
  d.sendReq(GoogieSpell.createXMLReq(req_text));
}

GoogieSpell.escapeSepcial = function(val) {
  return val.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
}

GoogieSpell.createXMLReq = function (text) {
  return '<?xml version="1.0" encoding="utf-8" ?><spellrequest textalreadyclipped="0" ignoredups="0" ignoredigits="1" ignoreallcaps="1"><text>' + text + '</text></spellrequest>';
}

//Retunrs an array
//result[item] -> ['attrs']
//                ['suggestions']
GoogieSpell.parseResult = function(r_text) {
  var re_split_attr_c = /\w="\d+"/g;
  var re_split_text = /\t/g;

  var matched_c = r_text.match(/<c[^>]*>[^<]*<\/c>/g);
  var results = new Array();
  
  for(var i=0; i < matched_c.length; i++) {
    var item = new Array();

    //Get attributes
    item['attrs'] = new Array();
    var split_c = matched_c[i].match(re_split_attr_c);
    for(var j=0; j < split_c.length; j++) {
      var c_attr = split_c[j].split(/=/);
      item['attrs'][c_attr[0]] = parseInt(c_attr[1].replace('"', ''));
    }

    //Get suggestions
    item['suggestions'] = new Array();
    var only_text = matched_c[i].replace(/<[^>]*>/g, "");
    var split_t = only_text.split(re_split_text);
    for(var k=0; k < split_t.length; k++) {
    if(split_t[k] != "")
      item['suggestions'].push(split_t[k]);
    }
    results.push(item);
  }
  return results;
}

/****
 Error window (the drop-down window)
****/
GoogieSpell.prototype.createErrorWindow = function() {
  this.error_window = AJS.DIV();
  this.error_window.className = "googie_window";
}

GoogieSpell.prototype.hideErrorWindow = function() {
  this.error_window.style.visibility = "hidden";
}

GoogieSpell.prototype.updateOrginalText = function(offset, old_value, new_value, id) {
  var part_1 = this.orginal_text.substring(0, offset);
  var part_2 = this.orginal_text.substring(offset+old_value.length);
  this.orginal_text = part_1 + new_value + part_2;
  var add_2_offset = new_value.length - old_value.length;
  for(var j=0; j < this.results.length; j++) {
    //Don't edit the offset of the current item
    if(j != id && j > id){
      this.results[j]['attrs']['o'] += add_2_offset;
    }
  }
}

GoogieSpell.prototype.saveOldValue = function (id, old_value) {
  this.results[id]['is_changed'] = true;
  this.results[id]['old_value'] = old_value;
}

GoogieSpell.prototype.showErrorWindow = function(elm, id) {
  var me = this;

  var abs_pos = GoogieSpell.absolutePosition(elm);
  abs_pos.y -= this.edit_layer.scrollTop;
  this.error_window.style.visibility = "visible";
  this.error_window.style.top = (abs_pos.y+20) + "px";
  this.error_window.style.left = (abs_pos.x) + "px";
  this.error_window.innerHTML = "";

  //Build up the result list
  var table = AJS.TABLE({'class': 'googie_list'});
  var list = AJS.TBODY();

  var suggestions = this.results[id]['suggestions'];
  var offset = this.results[id]['attrs']['o'];
  var len = this.results[id]['attrs']['l'];

  if(suggestions.length == 0) {
    var row = AJS.TR();
    var item = AJS.TD();
    var dummy = AJS.SPAN();
    item.appendChild(AJS.TN("Pas de suggestion :("));
    row.appendChild(item);
    list.appendChild(row);
  }

  for(i=0; i < suggestions.length; i++) {
    var row = AJS.TR();
    var item = AJS.TD();
    var dummy = AJS.SPAN();
    dummy.innerHTML = suggestions[i];
    item.appendChild(AJS.TN(dummy.innerHTML));
    
    item.onclick = function(e) {
      var l_elm = GoogieSpell.getEventElm(e);
      var old_value = elm.innerHTML;
      var new_value = l_elm.innerHTML;

      elm.style.color = "green";
      elm.innerHTML = l_elm.innerHTML;
      me.hideErrorWindow();

      me.updateOrginalText(offset, old_value, new_value, id);

      //Update to the new length
      me.results[id]['attrs']['l'] = new_value.length;
      me.saveOldValue(id, old_value);
    };
    item.onmouseover = GoogieSpell.item_onmouseover;
    item.onmouseout = GoogieSpell.item_onmouseout;
    row.appendChild(item);
    list.appendChild(row);
  }
  
  //The element is changed, append the revert
  if(this.results[id]['is_changed']) {
    var old_value = this.results[id]['old_value'];
    var offset = this.results[id]['attrs']['o'];
    var revert_row = AJS.TR();
    var revert = AJS.TD();

    revert.onmouseover = GoogieSpell.item_onmouseover;
    revert.onmouseout = GoogieSpell.item_onmouseout;
    revert.appendChild(AJS.SPAN({'class': 'googie_list_revert'}, "Revert to " + old_value));

    revert.onclick = function(e) { 
      me.updateOrginalText(offset, elm.innerHTML, old_value, id);
      elm.style.color = "#b91414";
      elm.innerHTML = old_value;
      me.hideErrorWindow();
    };

    revert_row.appendChild(revert);
    list.appendChild(revert_row);
  }

  //Append the edit box
  var edit_row = AJS.TR();
  var edit = AJS.TD();

  var edit_input = AJS.INPUT({'style': 'width: 110px;'});

  var onsub = function () {
    me.saveOldValue(id, elm.innerHTML);
    me.updateOrginalText(offset, elm.innerHTML, edit_input.value, id);
    elm.style.color = "green"
    elm.innerHTML = edit_input.value;
    
    me.hideErrorWindow();
    return false;
  };
  
  var ok_pic = AJS.IMG({'src': this.img_dir + "ok.gif"});
  var edit_form = AJS.FORM({'style': 'margin: 0; padding: 0'}, edit_input, " ", ok_pic);
  ok_pic.onclick = onsub;
  edit_form.onsubmit = onsub;
  
  edit.appendChild(edit_form);
  edit_row.appendChild(edit);
  list.appendChild(edit_row);

  //Close button
  var close_row = AJS.TR();
  var close = AJS.TD();
  close.onmouseover = GoogieSpell.item_onmouseover;
  close.onmouseout = GoogieSpell.item_onmouseout;
  close.appendChild(AJS.SPAN({'class': 'googie_list_close'}, "Fermer"));
  close.onclick = function() { me.hideErrorWindow()};
  close_row.appendChild(close);
  list.appendChild(close_row);

  table.appendChild(list);
  this.error_window.appendChild(table);
}


/****
  Edit layer (the layer where the suggestions are stored)
****/
GoogieSpell.prototype.createEditLayer = function(width, height) {
  this.edit_layer = AJS.DIV({'class': 'googie_edit_layer'});
  
  //Set the style so it looks like edit areas
  this.edit_layer.className = this.text_area.className;
  this.edit_layer.style.border = "1px solid #999";
  this.edit_layer.style.overflow = "auto";
  this.edit_layer.style.backgroundColor = "#F1EDFE";
  this.edit_layer.style.padding = "3px";

  this.edit_layer.style.width = (width-8) + "px";
  this.edit_layer.style.height = height + "px";
}

GoogieSpell.prototype.resumeEditing = function(e, me) {
  this.setStateChanged("check_spelling");
  me.switch_lan_pic.style.display = "inline";

  me.el_scroll_top = me.edit_layer.scrollTop;
  var elm = GoogieSpell.getEventElm(e);
  AJS.replaceChildNodes(elm, this.createSpellDiv());

  elm.onclick = function(e) {
    me.spellCheck(elm, me.text_area.id);
  };
  me.hideErrorWindow();

  //Remove the EDIT_LAYER
  me.edit_layer.parentNode.removeChild(me.edit_layer);

  me.text_area.value = me.orginal_text;
  AJS.showElement(me.text_area);
  me.gselm.className = "googie_no_style";

  me.text_area.scrollTop = me.ta_scroll_top;

  elm.onmouseout = null;
}

GoogieSpell.prototype.createErrorLink = function(text, id) {
  var elm = AJS.SPAN({'class': 'googie_link'});
  var me = this;
  elm.onclick = function () {
    me.showErrorWindow(elm, id);
  };
  elm.innerHTML = text;
  return elm;
}

GoogieSpell.prototype.showErrorsInIframe = function(results) {
  var output = AJS.DIV();
  output.style.textAlign = "left";
  var is_safari = (navigator.userAgent.toLowerCase().indexOf("safari") != -1);
  var pointer = 0;
  for(var i=0; i < results.length; i++) {
    var offset = results[i]['attrs']['o'];
    var len = results[i]['attrs']['l'];
    
    var part_1_text = GoogieSpell.escapeSepcial(this.orginal_text.substring(pointer, offset)).replace(/\n/g, "<BR>");
    var part_1 = AJS.SPAN();
    part_1.innerHTML = part_1_text;
    if(part_1_text.substring(0, 1) == " ")
      output.appendChild(AJS.TN(" "));
    output.appendChild(part_1);
    if(part_1_text.substr(part_1_text.length-1, 1) == " " && is_safari)
      output.appendChild(AJS.TN(" "));
    pointer += offset - pointer;
    
    //If the last child was an error, then insert some space
    output.appendChild(this.createErrorLink(this.orginal_text.substr(offset, len), i));
    pointer += len;
  }
  //Insert the rest of the orginal text
  var part_2 = AJS.SPAN();
  part_2.innerHTML = GoogieSpell.escapeSepcial(this.orginal_text.substr(pointer, this.orginal_text.length)).replace(/\n/g, "<BR>");
  output.appendChild(part_2);

  this.edit_layer.appendChild(output);

  //Hide text area
  AJS.hideElement(this.text_area);
  this.text_area.parentNode.insertBefore(this.edit_layer, this.text_area.nextSibling);
}

GoogieSpell.Position = function(x, y) {
  this.x = x;
  this.y = y;
}	

//Get the absolute position of menu_slide
GoogieSpell.absolutePosition = function(element) {
  //Create a new object that has elements y and x pos...
  var posObj = new GoogieSpell.Position(element.offsetLeft, element.offsetTop);

  //Check if the element has an offsetParent - if it has .. loop until it has not
  if(element.offsetParent) {
    var temp_pos =	GoogieSpell.absolutePosition(element.offsetParent);
    posObj.x += temp_pos.x;
    posObj.y += temp_pos.y;
  }
  return posObj;
}

GoogieSpell.getEventElm = function(e) {
	var targ;
	if (!e) var e = window.event;
	if (e.target) targ = e.target;
	else if (e.srcElement) targ = e.srcElement;
	if (targ.nodeType == 3) // defeat Safari bug
		targ = targ.parentNode;
  return targ;
}

GoogieSpell.prototype.removeIndicator = function(elm) {
  AJS.removeElement(this.indicator);
}

GoogieSpell.prototype.appendIndicator = function(elm) {
  var img = AJS.IMG({'src': this.img_dir + 'indicator.gif', 'style': 'padding-right: 5px;'});
  img.style.width = "16px";
  img.style.height = "16px";
  this.indicator = img;
  img.style.textDecoration = "none";
  AJS.insertBefore(img, elm);
}

/****
 Choose language
****/
GoogieSpell.prototype.createLangWindow = function() {
  this.language_window = AJS.DIV({'class': 'googie_window'});
  this.language_window.style.width = "100px";

  //Build up the result list
  var table = AJS.TABLE({'class': 'googie_list'});
  var list = AJS.TBODY();

  this.lang_elms = new Array();

  for(i=0; i < this.langlist_codes.length; i++) {
    var row = AJS.TR();
    var item = AJS.TD();
    item.googieId = this.langlist_codes[i];
    this.lang_elms.push(item);
    var lang_span = AJS.SPAN();
    lang_span.innerHTML = this.lang_to_word[this.langlist_codes[i]];
    item.appendChild(AJS.TN(lang_span.innerHTML));

    var me = this;
    
    item.onclick = function(e) {
      var elm = GoogieSpell.getEventElm(e);
      me.deHighlightCurSel();

      GOOGIE_CUR_LANG = elm.googieId;
      var to_word = me.lang_to_word[elm.googieId];

      if(me.language_status_div != null)
        me.language_status_div.innerHTML = to_word;

      try {
        CUR_LANG_SPAN.innerHTML = to_word;
      }
      catch(e) {}

      //Set cookie
      var now = new Date();
      now.setTime(now.getTime() + 365 * 24 * 60 * 60 * 1000);
      setCookie('language', GOOGIE_CUR_LANG, now);

      me.highlightCurSel();
      me.hideLangWindow();
    };

    item.onmouseover = function(e) { 
      var i_it = GoogieSpell.getEventElm(e);
      if(i_it.className != "googie_list_selected")
        i_it.className = "googie_list_onhover";
    };
    item.onmouseout = function(e) { 
      var i_it = GoogieSpell.getEventElm(e);
      if(i_it.className != "googie_list_selected")
        i_it.className = "googie_list_onout"; 
    };

    row.appendChild(item);
    list.appendChild(row);
  }

  this.highlightCurSel();

  //Close button
  var close_row = AJS.TR();
  var close = AJS.TD();
  close.onmouseover = GoogieSpell.item_onmouseover;
  close.onmouseout = GoogieSpell.item_onmouseout;
  close.appendChild(AJS.SPAN({'class': 'googie_list_close'}, "Fermer"));
  var me = this;
  close.onclick = function(e) {
    me.hideLangWindow(); GoogieSpell.item_onmouseout(e);
  };
  close_row.appendChild(close);
  list.appendChild(close_row);

  table.appendChild(list);
  this.language_window.appendChild(table);
}

GoogieSpell.prototype.hideLangWindow = function() {
  this.language_window.style.visibility = "hidden";
  this.switch_lan_pic.className = "googie_lang_3d_on";
}

GoogieSpell.prototype.deHighlightCurSel = function() {
  this.lang_cur_elm.className = "googie_list_onout";
}

GoogieSpell.prototype.highlightCurSel = function() {
  for(var i=0; i < this.lang_elms.length; i++) {
    if(this.lang_elms[i].googieId == GOOGIE_CUR_LANG) {
      this.lang_elms[i].className = "googie_list_selected";
      this.lang_cur_elm = this.lang_elms[i];
    }
    else {
      this.lang_elms[i].className = "googie_list_onout";
    }
  }
}

GoogieSpell.prototype.showLangWindow = function(elm, ofst_top, ofst_left) {
  if(!AJS.isDefined(ofst_top))
    ofst_top = 20;
  if(!AJS.isDefined(ofst_left))
    ofst_left = 50;

  this.createLangWindow();
  AJS.getBody().appendChild(this.language_window);

  var abs_pos = GoogieSpell.absolutePosition(elm);
  AJS.showElement(this.language_window);
  this.language_window.style.top = (abs_pos.y+ofst_top) + "px";
  this.language_window.style.left = (abs_pos.x+ofst_left-this.language_window.offsetWidth) + "px";
  this.highlightCurSel();
  this.language_window.style.visibility = "visible";
}

GoogieSpell.prototype.flashNoSpellingErrorState = function() {
  this.setStateChanged("no_error_found");
  var me = this;
  AJS.hideElement(this.switch_lan_pic);
  this.gselm.innerHTML = "Pas de fautes trouvées !";
  this.gselm.className = "googie_check_spelling_ok";
  this.gselm.style.textDecoration = "none";
  this.gselm.style.cursor = "default";
  var fu = function() {
    AJS.removeElement(me.gselm);
    me.checkSpellingState();
  };
  setTimeout(fu, 1000);
}

GoogieSpell.prototype.resumeEditingState = function() {
  this.setStateChanged("resume_editing");
  var me = this;
  AJS.hideElement(me.switch_lan_pic);

  //Change link text to resume
  me.gselm.innerHTML = "Fermer le correcteur !";
  me.gselm.onclick = function(e) {
    me.resumeEditing(e, me);
  }
  me.gselm.className = "googie_check_spelling_ok";
  me.edit_layer.scrollTop = me.el_scroll_top;
}

GoogieSpell.prototype.createChangeLangPic = function() {
  var switch_lan = AJS.A({'class': 'googie_lang_3d_on', 'style': 'padding-left: 6px;'}, AJS.IMG({'src': this.img_dir + 'change_lang.gif', 'alt': "Change language"}));
  switch_lan.onmouseover = function() {
    if(this.className != "googie_lang_3d_click")
      this.className = "googie_lang_3d_on";
  }

  var me = this;
  switch_lan.onclick = function() {
    if(this.className == "googie_lang_3d_click") {
      me.hideLangWindow();
    }
    else {
      me.showLangWindow(switch_lan);
      this.className = "googie_lang_3d_click";
    }
  }
  return switch_lan;
}

GoogieSpell.prototype.createSpellDiv = function() {
  return AJS.DIV({'style': 'display: inline'}, AJS.IMG({'src': this.img_dir + "spellc.gif"}), " ", AJS.SPAN({'class': 'googie_check_spelling_link'}, "Orthographe"));
}

GoogieSpell.prototype.checkSpellingState = function() {
  this.setStateChanged("check_spelling");
  var me = this;
  if(this.show_change_lang_pic)
    this.switch_lan_pic = this.createChangeLangPic();
  else
    this.switch_lan_pic = AJS.SPAN();

  var span_chck = this.createSpellDiv();
  span_chck.onclick = function() {
    me.spellCheck(span_chck);
  }
  AJS.appendChildNodes(this.spell_container, span_chck, this.switch_lan_pic);
}

GoogieSpell.prototype.setLanguages = function(lang_dict) {
  this.lang_to_word = lang_dict;
  this.langlist_codes = AJS.keys(lang_dict);
}

GoogieSpell.prototype.decorateTextarea = function(id, /*optional*/spell_container_id, force_width) {
  var me = this;
  
  if(typeof(id) == "string")
    this.text_area = AJS.getElement(id);
  else
    this.text_area = id;

  var r_width;

  if(this.text_area != null) {
    if(AJS.isDefined(spell_container_id)) {
      if(typeof(spell_container_id) == "string")
        this.spell_container = AJS.getElement(spell_container_id);
      else
        this.spell_container = spell_container_id;
    }
    else {
      var table = AJS.TABLE();
      var tbody = AJS.TBODY();
      var tr = AJS.TR();
      if(AJS.isDefined(force_width)) {
        r_width = force_width;
      }
      else {
        r_width = this.text_area.offsetWidth + "px";
      }

      var spell_container = AJS.TD();
      this.spell_container = spell_container;

      tr.appendChild(spell_container);

      tbody.appendChild(tr);
      table.appendChild(tbody);

      AJS.insertBefore(table, this.text_area);

      //Set width
      table.style.width = r_width;
      spell_container.style.width = r_width;
      spell_container.style.textAlign = "right";
    }

    this.checkSpellingState();
  }
  else {
    alert("Text area not found");
  }
}
