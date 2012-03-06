/* Copyright (c) 2006, Yahoo! Inc. All rights reserved.Code licensed under the BSD License:http://developer.yahoo.net/yui/license.txt version: 0.12.0 */ if(typeof YAHOO=="undefined"){var YAHOO={};}YAHOO.namespace=function(){var a=arguments,o=null,i,j,d;for(i=0;i<a.length;++i){d=a[i].split(".");o=YAHOO;for(j=(d[0]=="YAHOO")?1:0;j<d.length;++j){o[d[j]]=o[d[j]]||{};o=o[d[j]];}}return o;};YAHOO.log=function(_2,_3,_4){var l=YAHOO.widget.Logger;if(l&&l.log){return l.log(_2,_3,_4);}else{return false;}};YAHOO.extend=function(_6,_7,_8){var F=function(){};F.prototype=_7.prototype;_6.prototype=new F();_6.prototype.constructor=_6;_6.superclass=_7.prototype;if(_7.prototype.constructor==Object.prototype.constructor){_7.prototype.constructor=_7;}if(_8){for(var i in _8){_6.prototype[i]=_8[i];}}};YAHOO.augment=function(r,s){var rp=r.prototype,sp=s.prototype,a=arguments,i,p;if(a[2]){for(i=2;i<a.length;++i){rp[a[i]]=sp[a[i]];}}else{for(p in sp){if(!rp[p]){rp[p]=sp[p];}}}};YAHOO.namespace("util","widget","example");
/* Copyright (c) 2006, Yahoo! Inc. All rights reserved.  Code licensed under the BSD License: http://developer.yahoo.net/yui/license.txt Version: 0.11.3 */ YAHOO.util.CustomEvent=function(_1,_2,_3){this.type=_1;this.scope=_2||window;this.silent=_3;this.subscribers=[];if(!this.silent){}};YAHOO.util.CustomEvent.prototype={subscribe:function(fn,_5,_6){this.subscribers.push(new YAHOO.util.Subscriber(fn,_5,_6));},unsubscribe:function(fn,_7){var _8=false;for(var i=0,len=this.subscribers.length;i<len;++i){var s=this.subscribers[i];if(s&&s.contains(fn,_7)){this._delete(i);_8=true;}}return _8;},fire:function(){var len=this.subscribers.length;if(!len&&this.silent){return;}var _12=[];for(var i=0;i<arguments.length;++i){_12.push(arguments[i]);}if(!this.silent){}for(i=0;i<len;++i){var s=this.subscribers[i];if(s){if(!this.silent){}var _13=(s.override)?s.obj:this.scope;s.fn.call(_13,this.type,_12,s.obj);}}},unsubscribeAll:function(){for(var i=0,len=this.subscribers.length;i<len;++i){this._delete(len-1-i);}},_delete:function(_14){var s=this.subscribers[_14];if(s){delete s.fn;delete s.obj;}this.subscribers.splice(_14,1);},toString:function(){return "CustomEvent: "+"'"+this.type+"', "+"scope: "+this.scope;}};YAHOO.util.Subscriber=function(fn,obj,_16){this.fn=fn;this.obj=obj||null;this.override=(_16);};YAHOO.util.Subscriber.prototype.contains=function(fn,obj){return (this.fn==fn&&this.obj==obj);};YAHOO.util.Subscriber.prototype.toString=function(){return "Subscriber { obj: "+(this.obj||"")+", override: "+(this.override||"no")+" }";};if(!YAHOO.util.Event){YAHOO.util.Event=function(){var _17=false;var _18=[];var _19=[];var _20=[];var _21=[];var _22=[];var _23=0;var _24=[];var _25=[];var _26=0;return {POLL_RETRYS:200,POLL_INTERVAL:50,EL:0,TYPE:1,FN:2,WFN:3,SCOPE:3,ADJ_SCOPE:4,isSafari:(/Safari|Konqueror|KHTML/gi).test(navigator.userAgent),isIE:(!this.isSafari&&!navigator.userAgent.match(/opera/gi)&&navigator.userAgent.match(/msie/gi)),addDelayedListener:function(el,_28,fn,_29,_30){_19[_19.length]=[el,_28,fn,_29,_30];if(_17){_23=this.POLL_RETRYS;this.startTimeout(0);}},startTimeout:function(_31){var i=(_31||_31===0)?_31:this.POLL_INTERVAL;var _32=this;var _33=function(){_32._tryPreloadAttach();};this.timeout=setTimeout(_33,i);},onAvailable:function(_34,_35,_36,_37){_24.push({id:_34,fn:_35,obj:_36,override:_37});_23=this.POLL_RETRYS;this.startTimeout(0);},addListener:function(el,_38,fn,_39,_40){if(!fn||!fn.call){return false;}if(this._isValidCollection(el)){var ok=true;for(var i=0,len=el.length;i<len;++i){ok=(this.on(el[i],_38,fn,_39,_40)&&ok);}return ok;}else{if(typeof el=="string"){var oEl=this.getEl(el);if(_17&&oEl){el=oEl;}else{this.addDelayedListener(el,_38,fn,_39,_40);return true;}}}if(!el){return false;}if("unload"==_38&&_39!==this){_20[_20.length]=[el,_38,fn,_39,_40];return true;}var _43=(_40)?_39:el;var _44=function(e){return fn.call(_43,YAHOO.util.Event.getEvent(e),_39);};var li=[el,_38,fn,_44,_43];var _47=_18.length;_18[_47]=li;if(this.useLegacyEvent(el,_38)){var _48=this.getLegacyIndex(el,_38);if(_48==-1||el!=_21[_48][0]){_48=_21.length;_25[el.id+_38]=_48;_21[_48]=[el,_38,el["on"+_38]];_22[_48]=[];el["on"+_38]=function(e){YAHOO.util.Event.fireLegacyEvent(YAHOO.util.Event.getEvent(e),_48);};}_22[_48].push(_47);}else{if(el.addEventListener){el.addEventListener(_38,_44,false);}else{if(el.attachEvent){el.attachEvent("on"+_38,_44);}}}return true;},fireLegacyEvent:function(e,_49){var ok=true;var le=_22[_49];for(var i=0,len=le.length;i<len;++i){var _51=le[i];if(_51){var li=_18[_51];if(li&&li[this.WFN]){var _52=li[this.ADJ_SCOPE];var ret=li[this.WFN].call(_52,e);ok=(ok&&ret);}else{delete le[i];}}}return ok;},getLegacyIndex:function(el,_54){var key=this.generateId(el)+_54;if(typeof _25[key]=="undefined"){return -1;}else{return _25[key];}},useLegacyEvent:function(el,_56){if(!el.addEventListener&&!el.attachEvent){return true;}else{if(this.isSafari){if("click"==_56||"dblclick"==_56){return true;}}}return false;},removeListener:function(el,_57,fn,_58){if(!fn||!fn.call){return false;}if(typeof el=="string"){el=this.getEl(el);}else{if(this._isValidCollection(el)){var ok=true;for(var i=0,len=el.length;i<len;++i){ok=(this.removeListener(el[i],_57,fn)&&ok);}return ok;}}if("unload"==_57){for(i=0,len=_20.length;i<len;i++){var li=_20[i];if(li&&li[0]==el&&li[1]==_57&&li[2]==fn){_20.splice(i,1);return true;}}return false;}var _59=null;if("undefined"==typeof _58){_58=this._getCacheIndex(el,_57,fn);}if(_58>=0){_59=_18[_58];}if(!el||!_59){return false;}if(el.removeEventListener){el.removeEventListener(_57,_59[this.WFN],false);}else{if(el.detachEvent){el.detachEvent("on"+_57,_59[this.WFN]);}}delete _18[_58][this.WFN];delete _18[_58][this.FN];_18.splice(_58,1);return true;},getTarget:function(ev,_61){var t=ev.target||ev.srcElement;return this.resolveTextNode(t);},resolveTextNode:function(_63){if(_63&&_63.nodeName&&"#TEXT"==_63.nodeName.toUpperCase()){return _63.parentNode;}else{return _63;}},getPageX:function(ev){var x=ev.pageX;if(!x&&0!==x){x=ev.clientX||0;if(this.isIE){x+=this._getScrollLeft();}}return x;},getPageY:function(ev){var y=ev.pageY;if(!y&&0!==y){y=ev.clientY||0;if(this.isIE){y+=this._getScrollTop();}}return y;},getXY:function(ev){return [this.getPageX(ev),this.getPageY(ev)];},getRelatedTarget:function(ev){var t=ev.relatedTarget;if(!t){if(ev.type=="mouseout"){t=ev.toElement;}else{if(ev.type=="mouseover"){t=ev.fromElement;}}}return this.resolveTextNode(t);},getTime:function(ev){if(!ev.time){var t=new Date().getTime();try{ev.time=t;}catch(e){return t;}}return ev.time;},stopEvent:function(ev){this.stopPropagation(ev);this.preventDefault(ev);},stopPropagation:function(ev){if(ev.stopPropagation){ev.stopPropagation();}else{ev.cancelBubble=true;}},preventDefault:function(ev){if(ev.preventDefault){ev.preventDefault();}else{ev.returnValue=false;}},getEvent:function(e){var ev=e||window.event;if(!ev){var c=this.getEvent.caller;while(c){ev=c.arguments[0];if(ev&&Event==ev.constructor){break;}c=c.caller;}}return ev;},getCharCode:function(ev){return ev.charCode||((ev.type=="keypress")?ev.keyCode:0);},_getCacheIndex:function(el,_67,fn){for(var i=0,len=_18.length;i<len;++i){var li=_18[i];if(li&&li[this.FN]==fn&&li[this.EL]==el&&li[this.TYPE]==_67){return i;}}return -1;},generateId:function(el){var id=el.id;if(!id){id="yuievtautoid-"+_26;++_26;el.id=id;}return id;},_isValidCollection:function(o){return (o&&o.length&&typeof o!="string"&&!o.tagName&&!o.alert&&typeof o[0]!="undefined");},elCache:{},getEl:function(id){return document.getElementById(id);},clearCache:function(){},_load:function(e){_17=true;},_tryPreloadAttach:function(){if(this.locked){return false;}this.locked=true;var _70=!_17;if(!_70){_70=(_23>0);}var _71=[];for(var i=0,len=_19.length;i<len;++i){var d=_19[i];if(d){var el=this.getEl(d[this.EL]);if(el){this.on(el,d[this.TYPE],d[this.FN],d[this.SCOPE],d[this.ADJ_SCOPE]);delete _19[i];}else{_71.push(d);}}}_19=_71;var _73=[];for(i=0,len=_24.length;i<len;++i){var _74=_24[i];if(_74){el=this.getEl(_74.id);if(el){var _75=(_74.override)?_74.obj:el;_74.fn.call(_75,_74.obj);delete _24[i];}else{_73.push(_74);}}}_23=(_71.length===0&&_73.length===0)?0:_23-1;if(_70){this.startTimeout();}this.locked=false;return true;},purgeElement:function(el,_76,_77){var _78=this.getListeners(el,_77);if(_78){for(var i=0,len=_78.length;i<len;++i){var l=_78[i];this.removeListener(el,l.type,l.fn);}}if(_76&&el&&el.childNodes){for(i=0,len=el.childNodes.length;i<len;++i){this.purgeElement(el.childNodes[i],_76,_77);}}},getListeners:function(el,_80){var _81=[];if(_18&&_18.length>0){for(var i=0,len=_18.length;i<len;++i){var l=_18[i];if(l&&l[this.EL]===el&&(!_80||_80===l[this.TYPE])){_81.push({type:l[this.TYPE],fn:l[this.FN],obj:l[this.SCOPE],adjust:l[this.ADJ_SCOPE],index:i});}}}return (_81.length)?_81:null;},_unload:function(e,me){for(var i=0,len=_20.length;i<len;++i){var l=_20[i];if(l){var _83=(l[this.ADJ_SCOPE])?l[this.SCOPE]:window;l[this.FN].call(_83,this.getEvent(e),l[this.SCOPE]);}}if(_18&&_18.length>0){var j=_18.length;while(j){var _85=j-1;l=_18[_85];if(l){this.removeListener(l[this.EL],l[this.TYPE],l[this.FN],_85);}j=j-1;}this.clearCache();}for(i=0,len=_21.length;i<len;++i){delete _21[i][0];delete _21[i];}},_getScrollLeft:function(){return this._getScroll()[1];},_getScrollTop:function(){return this._getScroll()[0];},_getScroll:function(){var dd=document.documentElement,db=document.body;if(dd&&dd.scrollTop){return [dd.scrollTop,dd.scrollLeft];}else{if(db){return [db.scrollTop,db.scrollLeft];}else{return [0,0];}}}};}();YAHOO.util.Event.on=YAHOO.util.Event.addListener;if(document&&document.body){YAHOO.util.Event._load();}else{YAHOO.util.Event.on(window,"load",YAHOO.util.Event._load,YAHOO.util.Event,true);}YAHOO.util.Event.on(window,"unload",YAHOO.util.Event._unload,YAHOO.util.Event,true);YAHOO.util.Event._tryPreloadAttach();}
/* Copyright (c) 2006, Yahoo! Inc. All rights reserved.Code licensed under the BSD License:http://developer.yahoo.net/yui/license.txt version: 0.12.0 */
YAHOO.util.Connect={_msxml_progid:['MSXML2.XMLHTTP.3.0','MSXML2.XMLHTTP','Microsoft.XMLHTTP'],_http_header:{},_has_http_headers:false,_use_default_post_header:true,_default_post_header:'application/x-www-form-urlencoded',_isFormSubmit:false,_isFileUpload:false,_formNode:null,_sFormData:null,_poll:{},_timeOut:{},_polling_interval:50,_transaction_id:0,setProgId:function(id)
{this._msxml_progid.unshift(id);},setDefaultPostHeader:function(b)
{this._use_default_post_header=b;},setPollingInterval:function(i)
{if(typeof i=='number'&&isFinite(i)){this._polling_interval=i;}},createXhrObject:function(transactionId)
{var obj,http;try
{http=new XMLHttpRequest();obj={conn:http,tId:transactionId};}
catch(e)
{for(var i=0;i<this._msxml_progid.length;++i){try
{http=new ActiveXObject(this._msxml_progid[i]);obj={conn:http,tId:transactionId};break;}
catch(e){}}}
finally
{return obj;}},getConnectionObject:function()
{var o;var tId=this._transaction_id;try
{o=this.createXhrObject(tId);if(o){this._transaction_id++;}}
catch(e){}
finally
{return o;}},asyncRequest:function(method,uri,callback,postData)
{var o=this.getConnectionObject();if(!o){return null;}
else{if(this._isFormSubmit){if(this._isFileUpload){this.uploadFile(o.tId,callback,uri,postData);this.releaseObject(o);return;}
if(method=='GET'){if(this._sFormData.length!=0){uri+=((uri.indexOf('?')==-1)?'?':'&')+this._sFormData;}
else{uri+="?"+this._sFormData;}}
else if(method=='POST'){postData=postData?this._sFormData+"&"+postData:this._sFormData;}}
o.conn.open(method,uri,true);if(this._isFormSubmit||(postData&&this._use_default_post_header)){this.initHeader('Content-Type',this._default_post_header);if(this._isFormSubmit){this.resetFormState();}}
if(this._has_http_headers){this.setHeader(o);}
this.handleReadyState(o,callback);o.conn.send(postData||null);return o;}},handleReadyState:function(o,callback)
{var oConn=this;if(callback&&callback.timeout){this._timeOut[o.tId]=window.setTimeout(function(){oConn.abort(o,callback,true);},callback.timeout);}
this._poll[o.tId]=window.setInterval(function(){if(o.conn&&o.conn.readyState==4){window.clearInterval(oConn._poll[o.tId]);delete oConn._poll[o.tId];if(callback&&callback.timeout){delete oConn._timeOut[o.tId];}
oConn.handleTransactionResponse(o,callback);}},this._polling_interval);},handleTransactionResponse:function(o,callback,isAbort)
{if(!callback){this.releaseObject(o);return;}
var httpStatus,responseObject;try
{if(o.conn.status!==undefined&&o.conn.status!=0){httpStatus=o.conn.status;}
else{httpStatus=13030;}}
catch(e){httpStatus=13030;}
if(httpStatus>=200&&httpStatus<300){try
{responseObject=this.createResponseObject(o,callback.argument);if(callback.success){if(!callback.scope){callback.success(responseObject);}
else{callback.success.apply(callback.scope,[responseObject]);}}}
catch(e){}}
else{try
{switch(httpStatus){case 12002:case 12029:case 12030:case 12031:case 12152:case 13030:responseObject=this.createExceptionObject(o.tId,callback.argument,(isAbort?isAbort:false));if(callback.failure){if(!callback.scope){callback.failure(responseObject);}
else{callback.failure.apply(callback.scope,[responseObject]);}}
break;default:responseObject=this.createResponseObject(o,callback.argument);if(callback.failure){if(!callback.scope){callback.failure(responseObject);}
else{callback.failure.apply(callback.scope,[responseObject]);}}}}
catch(e){}}
this.releaseObject(o);responseObject=null;},createResponseObject:function(o,callbackArg)
{var obj={};var headerObj={};try
{var headerStr=o.conn.getAllResponseHeaders();var header=headerStr.split('\n');for(var i=0;i<header.length;i++){var delimitPos=header[i].indexOf(':');if(delimitPos!=-1){headerObj[header[i].substring(0,delimitPos)]=header[i].substring(delimitPos+2);}}}
catch(e){}
obj.tId=o.tId;obj.status=o.conn.status;obj.statusText=o.conn.statusText;obj.getResponseHeader=headerObj;obj.getAllResponseHeaders=headerStr;obj.responseText=o.conn.responseText;obj.responseXML=o.conn.responseXML;if(typeof callbackArg!==undefined){obj.argument=callbackArg;}
return obj;},createExceptionObject:function(tId,callbackArg,isAbort)
{var COMM_CODE=0;var COMM_ERROR='communication failure';var ABORT_CODE=-1;var ABORT_ERROR='transaction aborted';var obj={};obj.tId=tId;if(isAbort){obj.status=ABORT_CODE;obj.statusText=ABORT_ERROR;}
else{obj.status=COMM_CODE;obj.statusText=COMM_ERROR;}
if(callbackArg){obj.argument=callbackArg;}
return obj;},initHeader:function(label,value)
{if(this._http_header[label]===undefined){this._http_header[label]=value;}
else{this._http_header[label]=value+","+this._http_header[label];}
this._has_http_headers=true;},setHeader:function(o)
{for(var prop in this._http_header){if(this._http_header.hasOwnProperty(prop)){o.conn.setRequestHeader(prop,this._http_header[prop]);}}
delete this._http_header;this._http_header={};this._has_http_headers=false;},setForm:function(formId,isUpload,secureUri)
{this.resetFormState();var oForm;if(typeof formId=='string'){oForm=(document.getElementById(formId)||document.forms[formId]);}
else if(typeof formId=='object'){oForm=formId;}
else{return;}
if(isUpload){this.createFrame(secureUri?secureUri:null);this._isFormSubmit=true;this._isFileUpload=true;this._formNode=oForm;return;}
var oElement,oName,oValue,oDisabled;var hasSubmit=false;for(var i=0;i<oForm.elements.length;i++){oElement=oForm.elements[i];oDisabled=oForm.elements[i].disabled;oName=oForm.elements[i].name;oValue=oForm.elements[i].value;if(!oDisabled&&oName)
{switch(oElement.type)
{case'select-one':case'select-multiple':for(var j=0;j<oElement.options.length;j++){if(oElement.options[j].selected){if(window.ActiveXObject){this._sFormData+=encodeURIComponent(oName)+'='+encodeURIComponent(oElement.options[j].attributes['value'].specified?oElement.options[j].value:oElement.options[j].text)+'&';}
else{this._sFormData+=encodeURIComponent(oName)+'='+encodeURIComponent(oElement.options[j].hasAttribute('value')?oElement.options[j].value:oElement.options[j].text)+'&';}}}
break;case'radio':case'checkbox':if(oElement.checked){this._sFormData+=encodeURIComponent(oName)+'='+encodeURIComponent(oValue)+'&';}
break;case'file':case undefined:case'reset':case'button':break;case'submit':if(hasSubmit==false){this._sFormData+=encodeURIComponent(oName)+'='+encodeURIComponent(oValue)+'&';hasSubmit=true;}
break;default:this._sFormData+=encodeURIComponent(oName)+'='+encodeURIComponent(oValue)+'&';break;}}}
this._isFormSubmit=true;this._sFormData=this._sFormData.substr(0,this._sFormData.length-1);return this._sFormData;},resetFormState:function(){this._isFormSubmit=false;this._isFileUpload=false;this._formNode=null;this._sFormData="";},createFrame:function(secureUri){var frameId='yuiIO'+this._transaction_id;if(window.ActiveXObject){var io=document.createElement('<iframe id="'+frameId+'" name="'+frameId+'" />');if(typeof secureUri=='boolean'){io.src='javascript:false';}
else if(typeof secureURI=='string'){io.src=secureUri;}}
else{var io=document.createElement('iframe');io.id=frameId;io.name=frameId;}
io.style.position='absolute';io.style.top='-1000px';io.style.left='-1000px';document.body.appendChild(io);},appendPostData:function(postData)
{var formElements=new Array();var postMessage=postData.split('&');for(var i=0;i<postMessage.length;i++){var delimitPos=postMessage[i].indexOf('=');if(delimitPos!=-1){formElements[i]=document.createElement('input');formElements[i].type='hidden';formElements[i].name=postMessage[i].substring(0,delimitPos);formElements[i].value=postMessage[i].substring(delimitPos+1);this._formNode.appendChild(formElements[i]);}}
return formElements;},uploadFile:function(id,callback,uri,postData){var frameId='yuiIO'+id;var io=document.getElementById(frameId);this._formNode.action=uri;this._formNode.method='POST';this._formNode.target=frameId;if(this._formNode.encoding){this._formNode.encoding='multipart/form-data';}
else{this._formNode.enctype='multipart/form-data';}
if(postData){var oElements=this.appendPostData(postData);}
this._formNode.submit();if(oElements&&oElements.length>0){try
{for(var i=0;i<oElements.length;i++){this._formNode.removeChild(oElements[i]);}}
catch(e){}}
this.resetFormState();var uploadCallback=function()
{var obj={};obj.tId=id;obj.argument=callback.argument;try
{obj.responseText=io.contentWindow.document.body?io.contentWindow.document.body.innerHTML:null;obj.responseXML=io.contentWindow.document.XMLDocument?io.contentWindow.document.XMLDocument:io.contentWindow.document;}
catch(e){}
if(callback.upload){if(!callback.scope){callback.upload(obj);}
else{callback.upload.apply(callback.scope,[obj]);}}
if(YAHOO.util.Event){YAHOO.util.Event.removeListener(io,"load",uploadCallback);}
else if(window.detachEvent){io.detachEvent('onload',uploadCallback);}
else{io.removeEventListener('load',uploadCallback,false);}
setTimeout(function(){document.body.removeChild(io);},100);};if(YAHOO.util.Event){YAHOO.util.Event.addListener(io,"load",uploadCallback);}
else if(window.attachEvent){io.attachEvent('onload',uploadCallback);}
else{io.addEventListener('load',uploadCallback,false);}},abort:function(o,callback,isTimeout)
{if(this.isCallInProgress(o)){o.conn.abort();window.clearInterval(this._poll[o.tId]);delete this._poll[o.tId];if(isTimeout){delete this._timeOut[o.tId];}
this.handleTransactionResponse(o,callback,true);return true;}
else{return false;}},isCallInProgress:function(o)
{if(o.conn){return o.conn.readyState!=4&&o.conn.readyState!=0;}
else{return false;}},releaseObject:function(o)
{o.conn=null;o=null;}};
/* Copyright (c) 2006, Yahoo! Inc. All rights reserved.Code licensed under the BSD License:http://developer.yahoo.net/yui/license.txt version: 0.12.0 */(function(){var Y=YAHOO.util,getStyle,setStyle,id_counter=0,propertyCache={};var ua=navigator.userAgent.toLowerCase(),isOpera=(ua.indexOf('opera')>-1),isSafari=(ua.indexOf('safari')>-1),isGecko=(!isOpera&&!isSafari&&ua.indexOf('gecko')>-1),isIE=(!isOpera&&ua.indexOf('msie')>-1);var patterns={HYPHEN:/(-[a-z])/i};var toCamel=function(property){if(!patterns.HYPHEN.test(property)){return property;}if(propertyCache[property]){return propertyCache[property];}while(patterns.HYPHEN.exec(property)){property=property.replace(RegExp.$1,RegExp.$1.substr(1).toUpperCase());}propertyCache[property]=property;return property;};if(document.defaultView&&document.defaultView.getComputedStyle){getStyle=function(el,property){var value=null;var computed=document.defaultView.getComputedStyle(el,'');if(computed){value=computed[toCamel(property)];}return el.style[property]||value;};}else if(document.documentElement.currentStyle&&isIE){getStyle=function(el,property){switch(toCamel(property)){case'opacity':var val=100;try{val=el.filters['DXImageTransform.Microsoft.Alpha'].opacity;}catch(e){try{val=el.filters('alpha').opacity;}catch(e){}}return val/100;break;default:var value=el.currentStyle?el.currentStyle[property]:null;return(el.style[property]||value);}};}else{getStyle=function(el,property){return el.style[property];};}if(isIE){setStyle=function(el,property,val){switch(property){case'opacity':if(typeof el.style.filter=='string'){el.style.filter='alpha(opacity='+val*100+')';if(!el.currentStyle||!el.currentStyle.hasLayout){el.style.zoom=1;}}break;default:el.style[property]=val;}};}else{setStyle=function(el,property,val){el.style[property]=val;};}YAHOO.util.Dom={get:function(el){if(!el){return null;}if(typeof el!='string'&&!(el instanceof Array)){return el;}if(typeof el=='string'){return document.getElementById(el);}else{var collection=[];for(var i=0,len=el.length;i<len;++i){collection[collection.length]=Y.Dom.get(el[i]);}return collection;}return null;},getStyle:function(el,property){property=toCamel(property);var f=function(element){return getStyle(element,property);};return Y.Dom.batch(el,f,Y.Dom,true);},setStyle:function(el,property,val){property=toCamel(property);var f=function(element){setStyle(element,property,val);};Y.Dom.batch(el,f,Y.Dom,true);},getXY:function(el){var f=function(el){if(el.parentNode===null||el.offsetParent===null||this.getStyle(el,'display')=='none'){return false;}var parentNode=null;var pos=[];var box;if(el.getBoundingClientRect){box=el.getBoundingClientRect();var doc=document;if(!this.inDocument(el)&&parent.document!=document){doc=parent.document;if(!this.isAncestor(doc.documentElement,el)){return false;}}var scrollTop=Math.max(doc.documentElement.scrollTop,doc.body.scrollTop);var scrollLeft=Math.max(doc.documentElement.scrollLeft,doc.body.scrollLeft);return[box.left+scrollLeft,box.top+scrollTop];}else{pos=[el.offsetLeft,el.offsetTop];parentNode=el.offsetParent;if(parentNode!=el){while(parentNode){pos[0]+=parentNode.offsetLeft;pos[1]+=parentNode.offsetTop;parentNode=parentNode.offsetParent;}}if(isSafari&&this.getStyle(el,'position')=='absolute'){pos[0]-=document.body.offsetLeft;pos[1]-=document.body.offsetTop;}}if(el.parentNode){parentNode=el.parentNode;}else{parentNode=null;}while(parentNode&&parentNode.tagName.toUpperCase()!='BODY'&&parentNode.tagName.toUpperCase()!='HTML'){if(Y.Dom.getStyle(parentNode,'display')!='inline'){pos[0]-=parentNode.scrollLeft;pos[1]-=parentNode.scrollTop;}if(parentNode.parentNode){parentNode=parentNode.parentNode;}else{parentNode=null;}}return pos;};return Y.Dom.batch(el,f,Y.Dom,true);},getX:function(el){var f=function(el){return Y.Dom.getXY(el)[0];};return Y.Dom.batch(el,f,Y.Dom,true);},getY:function(el){var f=function(el){return Y.Dom.getXY(el)[1];};return Y.Dom.batch(el,f,Y.Dom,true);},setXY:function(el,pos,noRetry){var f=function(el){var style_pos=this.getStyle(el,'position');if(style_pos=='static'){this.setStyle(el,'position','relative');style_pos='relative';}var pageXY=this.getXY(el);if(pageXY===false){return false;}var delta=[parseInt(this.getStyle(el,'left'),10),parseInt(this.getStyle(el,'top'),10)];if(isNaN(delta[0])){delta[0]=(style_pos=='relative')?0:el.offsetLeft;}if(isNaN(delta[1])){delta[1]=(style_pos=='relative')?0:el.offsetTop;}if(pos[0]!==null){el.style.left=pos[0]-pageXY[0]+delta[0]+'px';}if(pos[1]!==null){el.style.top=pos[1]-pageXY[1]+delta[1]+'px';}var newXY=this.getXY(el);if(!noRetry&&(newXY[0]!=pos[0]||newXY[1]!=pos[1])){this.setXY(el,pos,true);}};Y.Dom.batch(el,f,Y.Dom,true);},setX:function(el,x){Y.Dom.setXY(el,[x,null]);},setY:function(el,y){Y.Dom.setXY(el,[null,y]);},getRegion:function(el){var f=function(el){var region=new Y.Region.getRegion(el);return region;};return Y.Dom.batch(el,f,Y.Dom,true);},getClientWidth:function(){return Y.Dom.getViewportWidth();},getClientHeight:function(){return Y.Dom.getViewportHeight();},getElementsByClassName:function(className,tag,root){var method=function(el){return Y.Dom.hasClass(el,className);};return Y.Dom.getElementsBy(method,tag,root);},hasClass:function(el,className){var re=new RegExp('(?:^|\\s+)'+className+'(?:\\s+|$)');var f=function(el){return re.test(el['className']);};return Y.Dom.batch(el,f,Y.Dom,true);},addClass:function(el,className){var f=function(el){if(this.hasClass(el,className)){return;}el['className']=[el['className'],className].join(' ');};Y.Dom.batch(el,f,Y.Dom,true);},removeClass:function(el,className){var re=new RegExp('(?:^|\\s+)'+className+'(?:\\s+|$)','g');var f=function(el){if(!this.hasClass(el,className)){return;}var c=el['className'];el['className']=c.replace(re,' ');if(this.hasClass(el,className)){this.removeClass(el,className);}};Y.Dom.batch(el,f,Y.Dom,true);},replaceClass:function(el,oldClassName,newClassName){if(oldClassName===newClassName){return false;}var re=new RegExp('(?:^|\\s+)'+oldClassName+'(?:\\s+|$)','g');var f=function(el){if(!this.hasClass(el,oldClassName)){this.addClass(el,newClassName);return;}el['className']=el['className'].replace(re,' '+newClassName+' ');if(this.hasClass(el,oldClassName)){this.replaceClass(el,oldClassName,newClassName);}};Y.Dom.batch(el,f,Y.Dom,true);},generateId:function(el,prefix){prefix=prefix||'yui-gen';el=el||{};var f=function(el){if(el){el=Y.Dom.get(el);}else{el={};}if(!el.id){el.id=prefix+id_counter++;}return el.id;};return Y.Dom.batch(el,f,Y.Dom,true);},isAncestor:function(haystack,needle){haystack=Y.Dom.get(haystack);if(!haystack||!needle){return false;}var f=function(needle){if(haystack.contains&&!isSafari){return haystack.contains(needle);}else if(haystack.compareDocumentPosition){return!!(haystack.compareDocumentPosition(needle)&16);}else{var parent=needle.parentNode;while(parent){if(parent==haystack){return true;}else if(!parent.tagName||parent.tagName.toUpperCase()=='HTML'){return false;}parent=parent.parentNode;}return false;}};return Y.Dom.batch(needle,f,Y.Dom,true);},inDocument:function(el){var f=function(el){return this.isAncestor(document.documentElement,el);};return Y.Dom.batch(el,f,Y.Dom,true);},getElementsBy:function(method,tag,root){tag=tag||'*';root=Y.Dom.get(root)||document;var nodes=[];var elements=root.getElementsByTagName(tag);if(!elements.length&&(tag=='*'&&root.all)){elements=root.all;}for(var i=0,len=elements.length;i<len;++i){if(method(elements[i])){nodes[nodes.length]=elements[i];}}return nodes;},batch:function(el,method,o,override){var id=el;el=Y.Dom.get(el);var scope=(override)?o:window;if(!el||el.tagName||!el.length){if(!el){return false;}return method.call(scope,el,o);}var collection=[];for(var i=0,len=el.length;i<len;++i){if(!el[i]){id=el[i];}collection[collection.length]=method.call(scope,el[i],o);}return collection;},getDocumentHeight:function(){var scrollHeight=(document.compatMode!='CSS1Compat')?document.body.scrollHeight:document.documentElement.scrollHeight;var h=Math.max(scrollHeight,Y.Dom.getViewportHeight());return h;},getDocumentWidth:function(){var scrollWidth=(document.compatMode!='CSS1Compat')?document.body.scrollWidth:document.documentElement.scrollWidth;var w=Math.max(scrollWidth,Y.Dom.getViewportWidth());return w;},getViewportHeight:function(){var height=self.innerHeight;var mode=document.compatMode;if((mode||isIE)&&!isOpera){height=(mode=='CSS1Compat')?document.documentElement.clientHeight:document.body.clientHeight;}return height;},getViewportWidth:function(){var width=self.innerWidth;var mode=document.compatMode;if(mode||isIE){width=(mode=='CSS1Compat')?document.documentElement.clientWidth:document.body.clientWidth;}return width;}};})();YAHOO.util.Region=function(t,r,b,l){this.top=t;this[1]=t;this.right=r;this.bottom=b;this.left=l;this[0]=l;};YAHOO.util.Region.prototype.contains=function(region){return(region.left>=this.left&&region.right<=this.right&&region.top>=this.top&&region.bottom<=this.bottom);};YAHOO.util.Region.prototype.getArea=function(){return((this.bottom-this.top)*(this.right-this.left));};YAHOO.util.Region.prototype.intersect=function(region){var t=Math.max(this.top,region.top);var r=Math.min(this.right,region.right);var b=Math.min(this.bottom,region.bottom);var l=Math.max(this.left,region.left);if(b>=t&&r>=l){return new YAHOO.util.Region(t,r,b,l);}else{return null;}};YAHOO.util.Region.prototype.union=function(region){var t=Math.min(this.top,region.top);var r=Math.max(this.right,region.right);var b=Math.max(this.bottom,region.bottom);var l=Math.min(this.left,region.left);return new YAHOO.util.Region(t,r,b,l);};YAHOO.util.Region.prototype.toString=function(){return("Region {"+"top: "+this.top+", right: "+this.right+", bottom: "+this.bottom+", left: "+this.left+"}");};YAHOO.util.Region.getRegion=function(el){var p=YAHOO.util.Dom.getXY(el);var t=p[1];var r=p[0]+el.offsetWidth;var b=p[1]+el.offsetHeight;var l=p[0];return new YAHOO.util.Region(t,r,b,l);};YAHOO.util.Point=function(x,y){if(x instanceof Array){y=x[1];x=x[0];}this.x=this.right=this.left=this[0]=x;this.y=this.top=this.bottom=this[1]=y;};YAHOO.util.Point.prototype=new YAHOO.util.Region();
/* Copyright (c) 2006, Yahoo! Inc. All rights reserved. Code licensed under the BSD License: http://developer.yahoo.net/yui/license.txt version 0.12.1 */
YAHOO.util.Config=function(owner){if(owner){this.init(owner);}};YAHOO.util.Config.prototype={owner:null,queueInProgress:false,checkBoolean:function(val){if(typeof val=='boolean'){return true;}else{return false;}},checkNumber:function(val){if(isNaN(val)){return false;}else{return true;}}};YAHOO.util.Config.prototype.init=function(owner){this.owner=owner;this.configChangedEvent=new YAHOO.util.CustomEvent("configChanged");this.queueInProgress=false;var config={};var initialConfig={};var eventQueue=[];var fireEvent=function(key,value){key=key.toLowerCase();var property=config[key];if(typeof property!='undefined'&&property.event){property.event.fire(value);}};this.addProperty=function(key,propertyObject){key=key.toLowerCase();config[key]=propertyObject;propertyObject.event=new YAHOO.util.CustomEvent(key);propertyObject.key=key;if(propertyObject.handler){propertyObject.event.subscribe(propertyObject.handler,this.owner,true);}
this.setProperty(key,propertyObject.value,true);if(!propertyObject.suppressEvent){this.queueProperty(key,propertyObject.value);}};this.getConfig=function(){var cfg={};for(var prop in config){var property=config[prop];if(typeof property!='undefined'&&property.event){cfg[prop]=property.value;}}
return cfg;};this.getProperty=function(key){key=key.toLowerCase();var property=config[key];if(typeof property!='undefined'&&property.event){return property.value;}else{return undefined;}};this.resetProperty=function(key){key=key.toLowerCase();var property=config[key];if(typeof property!='undefined'&&property.event){if(initialConfig[key]&&initialConfig[key]!='undefined'){this.setProperty(key,initialConfig[key]);}
return true;}else{return false;}};this.setProperty=function(key,value,silent){key=key.toLowerCase();if(this.queueInProgress&&!silent){this.queueProperty(key,value);return true;}else{var property=config[key];if(typeof property!='undefined'&&property.event){if(property.validator&&!property.validator(value)){return false;}else{property.value=value;if(!silent){fireEvent(key,value);this.configChangedEvent.fire([key,value]);}
return true;}}else{return false;}}};this.queueProperty=function(key,value){key=key.toLowerCase();var property=config[key];if(typeof property!='undefined'&&property.event){if(typeof value!='undefined'&&property.validator&&!property.validator(value)){return false;}else{if(typeof value!='undefined'){property.value=value;}else{value=property.value;}
var foundDuplicate=false;for(var i=0;i<eventQueue.length;i++){var queueItem=eventQueue[i];if(queueItem){var queueItemKey=queueItem[0];var queueItemValue=queueItem[1];if(queueItemKey.toLowerCase()==key){eventQueue[i]=null;eventQueue.push([key,(typeof value!='undefined'?value:queueItemValue)]);foundDuplicate=true;break;}}}
if(!foundDuplicate&&typeof value!='undefined'){eventQueue.push([key,value]);}}
if(property.supercedes){for(var s=0;s<property.supercedes.length;s++){var supercedesCheck=property.supercedes[s];for(var q=0;q<eventQueue.length;q++){var queueItemCheck=eventQueue[q];if(queueItemCheck){var queueItemCheckKey=queueItemCheck[0];var queueItemCheckValue=queueItemCheck[1];if(queueItemCheckKey.toLowerCase()==supercedesCheck.toLowerCase()){eventQueue.push([queueItemCheckKey,queueItemCheckValue]);eventQueue[q]=null;break;}}}}}
return true;}else{return false;}};this.refireEvent=function(key){key=key.toLowerCase();var property=config[key];if(typeof property!='undefined'&&property.event&&typeof property.value!='undefined'){if(this.queueInProgress){this.queueProperty(key);}else{fireEvent(key,property.value);}}};this.applyConfig=function(userConfig,init){if(init){initialConfig=userConfig;}
for(var prop in userConfig){this.queueProperty(prop,userConfig[prop]);}};this.refresh=function(){for(var prop in config){this.refireEvent(prop);}};this.fireQueue=function(){this.queueInProgress=true;for(var i=0;i<eventQueue.length;i++){var queueItem=eventQueue[i];if(queueItem){var key=queueItem[0];var value=queueItem[1];var property=config[key];property.value=value;fireEvent(key,value);}}
this.queueInProgress=false;eventQueue=[];};this.subscribeToConfigEvent=function(key,handler,obj,override){key=key.toLowerCase();var property=config[key];if(typeof property!='undefined'&&property.event){if(!YAHOO.util.Config.alreadySubscribed(property.event,handler,obj)){property.event.subscribe(handler,obj,override);}
return true;}else{return false;}};this.unsubscribeFromConfigEvent=function(key,handler,obj){key=key.toLowerCase();var property=config[key];if(typeof property!='undefined'&&property.event){return property.event.unsubscribe(handler,obj);}else{return false;}};this.toString=function(){var output="Config";if(this.owner){output+=" ["+this.owner.toString()+"]";}
return output;};this.outputEventQueue=function(){var output="";for(var q=0;q<eventQueue.length;q++){var queueItem=eventQueue[q];if(queueItem){output+=queueItem[0]+"="+queueItem[1]+", ";}}
return output;};};YAHOO.util.Config.alreadySubscribed=function(evt,fn,obj){for(var e=0;e<evt.subscribers.length;e++){var subsc=evt.subscribers[e];if(subsc&&subsc.obj==obj&&subsc.fn==fn){return true;}}
return false;};YAHOO.widget.DateMath={DAY:"D",WEEK:"W",YEAR:"Y",MONTH:"M",ONE_DAY_MS:1000*60*60*24,add:function(date,field,amount){var d=new Date(date.getTime());switch(field){case this.MONTH:var newMonth=date.getMonth()+amount;var years=0;if(newMonth<0){while(newMonth<0){newMonth+=12;years-=1;}}else if(newMonth>11){while(newMonth>11){newMonth-=12;years+=1;}}
d.setMonth(newMonth);d.setFullYear(date.getFullYear()+years);break;case this.DAY:d.setDate(date.getDate()+amount);break;case this.YEAR:d.setFullYear(date.getFullYear()+amount);break;case this.WEEK:d.setDate(date.getDate()+(amount*7));break;}
return d;},subtract:function(date,field,amount){return this.add(date,field,(amount*-1));},before:function(date,compareTo){var ms=compareTo.getTime();if(date.getTime()<ms){return true;}else{return false;}},after:function(date,compareTo){var ms=compareTo.getTime();if(date.getTime()>ms){return true;}else{return false;}},between:function(date,dateBegin,dateEnd){if(this.after(date,dateBegin)&&this.before(date,dateEnd)){return true;}else{return false;}},getJan1:function(calendarYear){return new Date(calendarYear,0,1);},getDayOffset:function(date,calendarYear){var beginYear=this.getJan1(calendarYear);var dayOffset=Math.ceil((date.getTime()-beginYear.getTime())/this.ONE_DAY_MS);return dayOffset;},getWeekNumber:function(date,calendarYear){date=this.clearTime(date);var nearestThurs=new Date(date.getTime()+(4*this.ONE_DAY_MS)-((date.getDay())*this.ONE_DAY_MS));var jan1=new Date(nearestThurs.getFullYear(),0,1);var dayOfYear=((nearestThurs.getTime()-jan1.getTime())/this.ONE_DAY_MS)-1;var weekNum=Math.ceil((dayOfYear)/7);return weekNum;},isYearOverlapWeek:function(weekBeginDate){var overlaps=false;var nextWeek=this.add(weekBeginDate,this.DAY,6);if(nextWeek.getFullYear()!=weekBeginDate.getFullYear()){overlaps=true;}
return overlaps;},isMonthOverlapWeek:function(weekBeginDate){var overlaps=false;var nextWeek=this.add(weekBeginDate,this.DAY,6);if(nextWeek.getMonth()!=weekBeginDate.getMonth()){overlaps=true;}
return overlaps;},findMonthStart:function(date){var start=new Date(date.getFullYear(),date.getMonth(),1);return start;},findMonthEnd:function(date){var start=this.findMonthStart(date);var nextMonth=this.add(start,this.MONTH,1);var end=this.subtract(nextMonth,this.DAY,1);return end;},clearTime:function(date){date.setHours(12,0,0,0);return date;}};YAHOO.widget.Calendar=function(id,containerId,config){this.init(id,containerId,config);};YAHOO.widget.Calendar.IMG_ROOT=(window.location.href.toLowerCase().indexOf("https")===0?"https://a248.e.akamai.net/sec.yimg.com/i/":"http://us.i1.yimg.com/us.yimg.com/i/");YAHOO.widget.Calendar.DATE="D";YAHOO.widget.Calendar.MONTH_DAY="MD";YAHOO.widget.Calendar.WEEKDAY="WD";YAHOO.widget.Calendar.RANGE="R";YAHOO.widget.Calendar.MONTH="M";YAHOO.widget.Calendar.DISPLAY_DAYS=42;YAHOO.widget.Calendar.STOP_RENDER="S";YAHOO.widget.Calendar.prototype={Config:null,parent:null,index:-1,cells:null,cellDates:null,id:null,oDomContainer:null,today:null,renderStack:null,_renderStack:null,_pageDate:null,_selectedDates:null,domEventMap:null};YAHOO.widget.Calendar.prototype.init=function(id,containerId,config){this.initEvents();this.today=new Date();YAHOO.widget.DateMath.clearTime(this.today);this.id=id;this.oDomContainer=document.getElementById(containerId);this.cfg=new YAHOO.util.Config(this);this.Options={};this.Locale={};this.initStyles();YAHOO.util.Dom.addClass(this.oDomContainer,this.Style.CSS_CONTAINER);YAHOO.util.Dom.addClass(this.oDomContainer,this.Style.CSS_SINGLE);this.cellDates=[];this.cells=[];this.renderStack=[];this._renderStack=[];this.setupConfig();if(config){this.cfg.applyConfig(config,true);}
this.cfg.fireQueue();};YAHOO.widget.Calendar.prototype.configIframe=function(type,args,obj){var useIframe=args[0];if(YAHOO.util.Dom.inDocument(this.oDomContainer)){if(useIframe){var pos=YAHOO.util.Dom.getStyle(this.oDomContainer,"position");if(this.browser=="ie"&&(pos=="absolute"||pos=="relative")){if(!YAHOO.util.Dom.inDocument(this.iframe)){this.iframe=document.createElement("iframe");this.iframe.src="javascript:false;";YAHOO.util.Dom.setStyle(this.iframe,"opacity","0");this.oDomContainer.insertBefore(this.iframe,this.oDomContainer.firstChild);}}}else{if(this.iframe){if(this.iframe.parentNode){this.iframe.parentNode.removeChild(this.iframe);}
this.iframe=null;}}}};YAHOO.widget.Calendar.prototype.configTitle=function(type,args,obj){var title=args[0];var close=this.cfg.getProperty("close");var titleDiv;if(title&&title!==""){titleDiv=YAHOO.util.Dom.getElementsByClassName(YAHOO.widget.CalendarGroup.CSS_2UPTITLE,"div",this.oDomContainer)[0]||document.createElement("div");titleDiv.className=YAHOO.widget.CalendarGroup.CSS_2UPTITLE;titleDiv.innerHTML=title;this.oDomContainer.insertBefore(titleDiv,this.oDomContainer.firstChild);YAHOO.util.Dom.addClass(this.oDomContainer,"withtitle");}else{titleDiv=YAHOO.util.Dom.getElementsByClassName(YAHOO.widget.CalendarGroup.CSS_2UPTITLE,"div",this.oDomContainer)[0]||null;if(titleDiv){YAHOO.util.Event.purgeElement(titleDiv);this.oDomContainer.removeChild(titleDiv);}
if(!close){YAHOO.util.Dom.removeClass(this.oDomContainer,"withtitle");}}};YAHOO.widget.Calendar.prototype.configClose=function(type,args,obj){var close=args[0];var title=this.cfg.getProperty("title");var linkClose;if(close===true){linkClose=YAHOO.util.Dom.getElementsByClassName("link-close","a",this.oDomContainer)[0]||document.createElement("a");linkClose.href="javascript:void(null);";linkClose.className="link-close";YAHOO.util.Event.addListener(linkClose,"click",this.hide,this,true);var imgClose=document.createElement("img");imgClose.src=YAHOO.widget.Calendar.IMG_ROOT+"us/my/bn/x_d.gif";imgClose.className=YAHOO.widget.CalendarGroup.CSS_2UPCLOSE;linkClose.appendChild(imgClose);this.oDomContainer.appendChild(linkClose);YAHOO.util.Dom.addClass(this.oDomContainer,"withtitle");}else{linkClose=YAHOO.util.Dom.getElementsByClassName("link-close","a",this.oDomContainer)[0]||null;if(linkClose){YAHOO.util.Event.purgeElement(linkClose);this.oDomContainer.removeChild(linkClose);}
if(!title||title===""){YAHOO.util.Dom.removeClass(this.oDomContainer,"withtitle");}}};YAHOO.widget.Calendar.prototype.initEvents=function(){this.beforeSelectEvent=new YAHOO.util.CustomEvent("beforeSelect");this.selectEvent=new YAHOO.util.CustomEvent("select");this.beforeDeselectEvent=new YAHOO.util.CustomEvent("beforeDeselect");this.deselectEvent=new YAHOO.util.CustomEvent("deselect");this.changePageEvent=new YAHOO.util.CustomEvent("changePage");this.beforeRenderEvent=new YAHOO.util.CustomEvent("beforeRender");this.renderEvent=new YAHOO.util.CustomEvent("render");this.resetEvent=new YAHOO.util.CustomEvent("reset");this.clearEvent=new YAHOO.util.CustomEvent("clear");this.beforeSelectEvent.subscribe(this.onBeforeSelect,this,true);this.selectEvent.subscribe(this.onSelect,this,true);this.beforeDeselectEvent.subscribe(this.onBeforeDeselect,this,true);this.deselectEvent.subscribe(this.onDeselect,this,true);this.changePageEvent.subscribe(this.onChangePage,this,true);this.renderEvent.subscribe(this.onRender,this,true);this.resetEvent.subscribe(this.onReset,this,true);this.clearEvent.subscribe(this.onClear,this,true);};YAHOO.widget.Calendar.prototype.doSelectCell=function(e,cal){var target=YAHOO.util.Event.getTarget(e);var cell,index,d,date;while(target.tagName.toLowerCase()!="td"&&!YAHOO.util.Dom.hasClass(target,cal.Style.CSS_CELL_SELECTABLE)){target=target.parentNode;if(target.tagName.toLowerCase()=="html"){return;}}
cell=target;if(YAHOO.util.Dom.hasClass(cell,cal.Style.CSS_CELL_SELECTABLE)){index=cell.id.split("cell")[1];d=cal.cellDates[index];date=new Date(d[0],d[1]-1,d[2]);var link;if(cal.Options.MULTI_SELECT){link=cell.getElementsByTagName("a")[0];if(link){link.blur();}
var cellDate=cal.cellDates[index];var cellDateIndex=cal._indexOfSelectedFieldArray(cellDate);if(cellDateIndex>-1){cal.deselectCell(index);}else{cal.selectCell(index);}}else{link=cell.getElementsByTagName("a")[0];if(link){link.blur();}
cal.selectCell(index);}}};YAHOO.widget.Calendar.prototype.doCellMouseOver=function(e,cal){var target;if(e){target=YAHOO.util.Event.getTarget(e);}else{target=this;}
while(target.tagName.toLowerCase()!="td"){target=target.parentNode;if(target.tagName.toLowerCase()=="html"){return;}}
if(YAHOO.util.Dom.hasClass(target,cal.Style.CSS_CELL_SELECTABLE)){YAHOO.util.Dom.addClass(target,cal.Style.CSS_CELL_HOVER);}};YAHOO.widget.Calendar.prototype.doCellMouseOut=function(e,cal){var target;if(e){target=YAHOO.util.Event.getTarget(e);}else{target=this;}
while(target.tagName.toLowerCase()!="td"){target=target.parentNode;if(target.tagName.toLowerCase()=="html"){return;}}
if(YAHOO.util.Dom.hasClass(target,cal.Style.CSS_CELL_SELECTABLE)){YAHOO.util.Dom.removeClass(target,cal.Style.CSS_CELL_HOVER);}};YAHOO.widget.Calendar.prototype.setupConfig=function(){this.cfg.addProperty("pagedate",{value:new Date(),handler:this.configPageDate});this.cfg.addProperty("selected",{value:[],handler:this.configSelected});this.cfg.addProperty("title",{value:"",handler:this.configTitle});this.cfg.addProperty("close",{value:false,handler:this.configClose});this.cfg.addProperty("iframe",{value:true,handler:this.configIframe,validator:this.cfg.checkBoolean});this.cfg.addProperty("mindate",{value:null,handler:this.configMinDate});this.cfg.addProperty("maxdate",{value:null,handler:this.configMaxDate});this.cfg.addProperty("MULTI_SELECT",{value:false,handler:this.configOptions,validator:this.cfg.checkBoolean});this.cfg.addProperty("START_WEEKDAY",{value:0,handler:this.configOptions,validator:this.cfg.checkNumber});this.cfg.addProperty("SHOW_WEEKDAYS",{value:true,handler:this.configOptions,validator:this.cfg.checkBoolean});this.cfg.addProperty("SHOW_WEEK_HEADER",{value:false,handler:this.configOptions,validator:this.cfg.checkBoolean});this.cfg.addProperty("SHOW_WEEK_FOOTER",{value:false,handler:this.configOptions,validator:this.cfg.checkBoolean});this.cfg.addProperty("HIDE_BLANK_WEEKS",{value:false,handler:this.configOptions,validator:this.cfg.checkBoolean});this.cfg.addProperty("NAV_ARROW_LEFT",{value:YAHOO.widget.Calendar.IMG_ROOT+"us/tr/callt.gif",handler:this.configOptions});this.cfg.addProperty("NAV_ARROW_RIGHT",{value:YAHOO.widget.Calendar.IMG_ROOT+"us/tr/calrt.gif",handler:this.configOptions});this.cfg.addProperty("MONTHS_SHORT",{value:["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"],handler:this.configLocale});this.cfg.addProperty("MONTHS_LONG",{value:["January","February","March","April","May","June","July","August","September","October","November","December"],handler:this.configLocale});this.cfg.addProperty("WEEKDAYS_1CHAR",{value:["S","M","T","W","T","F","S"],handler:this.configLocale});this.cfg.addProperty("WEEKDAYS_SHORT",{value:["Su","Mo","Tu","We","Th","Fr","Sa"],handler:this.configLocale});this.cfg.addProperty("WEEKDAYS_MEDIUM",{value:["Sun","Mon","Tue","Wed","Thu","Fri","Sat"],handler:this.configLocale});this.cfg.addProperty("WEEKDAYS_LONG",{value:["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"],handler:this.configLocale});var refreshLocale=function(){this.cfg.refireEvent("LOCALE_MONTHS");this.cfg.refireEvent("LOCALE_WEEKDAYS");};this.cfg.subscribeToConfigEvent("START_WEEKDAY",refreshLocale,this,true);this.cfg.subscribeToConfigEvent("MONTHS_SHORT",refreshLocale,this,true);this.cfg.subscribeToConfigEvent("MONTHS_LONG",refreshLocale,this,true);this.cfg.subscribeToConfigEvent("WEEKDAYS_1CHAR",refreshLocale,this,true);this.cfg.subscribeToConfigEvent("WEEKDAYS_SHORT",refreshLocale,this,true);this.cfg.subscribeToConfigEvent("WEEKDAYS_MEDIUM",refreshLocale,this,true);this.cfg.subscribeToConfigEvent("WEEKDAYS_LONG",refreshLocale,this,true);this.cfg.addProperty("LOCALE_MONTHS",{value:"long",handler:this.configLocaleValues});this.cfg.addProperty("LOCALE_WEEKDAYS",{value:"short",handler:this.configLocaleValues});this.cfg.addProperty("DATE_DELIMITER",{value:",",handler:this.configLocale});this.cfg.addProperty("DATE_FIELD_DELIMITER",{value:"/",handler:this.configLocale});this.cfg.addProperty("DATE_RANGE_DELIMITER",{value:"-",handler:this.configLocale});this.cfg.addProperty("MY_MONTH_POSITION",{value:1,handler:this.configLocale,validator:this.cfg.checkNumber});this.cfg.addProperty("MY_YEAR_POSITION",{value:2,handler:this.configLocale,validator:this.cfg.checkNumber});this.cfg.addProperty("MD_MONTH_POSITION",{value:1,handler:this.configLocale,validator:this.cfg.checkNumber});this.cfg.addProperty("MD_DAY_POSITION",{value:2,handler:this.configLocale,validator:this.cfg.checkNumber});this.cfg.addProperty("MDY_MONTH_POSITION",{value:1,handler:this.configLocale,validator:this.cfg.checkNumber});this.cfg.addProperty("MDY_DAY_POSITION",{value:2,handler:this.configLocale,validator:this.cfg.checkNumber});this.cfg.addProperty("MDY_YEAR_POSITION",{value:3,handler:this.configLocale,validator:this.cfg.checkNumber});};YAHOO.widget.Calendar.prototype.configPageDate=function(type,args,obj){var val=args[0];var month,year,aMonthYear;if(val){if(val instanceof Date){val=YAHOO.widget.DateMath.findMonthStart(val);this.cfg.setProperty("pagedate",val,true);if(!this._pageDate){this._pageDate=this.cfg.getProperty("pagedate");}
return;}else{aMonthYear=val.split(this.cfg.getProperty("DATE_FIELD_DELIMITER"));month=parseInt(aMonthYear[this.cfg.getProperty("MY_MONTH_POSITION")-1],10)-1;year=parseInt(aMonthYear[this.cfg.getProperty("MY_YEAR_POSITION")-1],10);}}else{month=this.today.getMonth();year=this.today.getFullYear();}
this.cfg.setProperty("pagedate",new Date(year,month,1),true);if(!this._pageDate){this._pageDate=this.cfg.getProperty("pagedate");}};YAHOO.widget.Calendar.prototype.configMinDate=function(type,args,obj){var val=args[0];if(typeof val=='string'){val=this._parseDate(val);this.cfg.setProperty("mindate",new Date(val[0],(val[1]-1),val[2]));}};YAHOO.widget.Calendar.prototype.configMaxDate=function(type,args,obj){var val=args[0];if(typeof val=='string'){val=this._parseDate(val);this.cfg.setProperty("maxdate",new Date(val[0],(val[1]-1),val[2]));}};YAHOO.widget.Calendar.prototype.configSelected=function(type,args,obj){var selected=args[0];if(selected){if(typeof selected=='string'){this.cfg.setProperty("selected",this._parseDates(selected),true);}}
if(!this._selectedDates){this._selectedDates=this.cfg.getProperty("selected");}};YAHOO.widget.Calendar.prototype.configOptions=function(type,args,obj){type=type.toUpperCase();var val=args[0];this.Options[type]=val;};YAHOO.widget.Calendar.prototype.configLocale=function(type,args,obj){type=type.toUpperCase();var val=args[0];this.Locale[type]=val;this.cfg.refireEvent("LOCALE_MONTHS");this.cfg.refireEvent("LOCALE_WEEKDAYS");};YAHOO.widget.Calendar.prototype.configLocaleValues=function(type,args,obj){type=type.toUpperCase();var val=args[0];switch(type){case"LOCALE_MONTHS":switch(val){case"short":this.Locale.LOCALE_MONTHS=this.cfg.getProperty("MONTHS_SHORT").concat();break;case"long":this.Locale.LOCALE_MONTHS=this.cfg.getProperty("MONTHS_LONG").concat();break;}
break;case"LOCALE_WEEKDAYS":switch(val){case"1char":this.Locale.LOCALE_WEEKDAYS=this.cfg.getProperty("WEEKDAYS_1CHAR").concat();break;case"short":this.Locale.LOCALE_WEEKDAYS=this.cfg.getProperty("WEEKDAYS_SHORT").concat();break;case"medium":this.Locale.LOCALE_WEEKDAYS=this.cfg.getProperty("WEEKDAYS_MEDIUM").concat();break;case"long":this.Locale.LOCALE_WEEKDAYS=this.cfg.getProperty("WEEKDAYS_LONG").concat();break;}
var START_WEEKDAY=this.cfg.getProperty("START_WEEKDAY");if(START_WEEKDAY>0){for(var w=0;w<START_WEEKDAY;++w){this.Locale.LOCALE_WEEKDAYS.push(this.Locale.LOCALE_WEEKDAYS.shift());}}
break;}};YAHOO.widget.Calendar.prototype.initStyles=function(){this.Style={CSS_ROW_HEADER:"calrowhead",CSS_ROW_FOOTER:"calrowfoot",CSS_CELL:"calcell",CSS_CELL_SELECTED:"selected",CSS_CELL_SELECTABLE:"selectable",CSS_CELL_RESTRICTED:"restricted",CSS_CELL_TODAY:"today",CSS_CELL_OOM:"oom",CSS_CELL_OOB:"previous",CSS_HEADER:"calheader",CSS_HEADER_TEXT:"calhead",CSS_WEEKDAY_CELL:"calweekdaycell",CSS_WEEKDAY_ROW:"calweekdayrow",CSS_FOOTER:"calfoot",CSS_CALENDAR:"yui-calendar",CSS_SINGLE:"single",CSS_CONTAINER:"yui-calcontainer",CSS_NAV_LEFT:"calnavleft",CSS_NAV_RIGHT:"calnavright",CSS_CELL_TOP:"calcelltop",CSS_CELL_LEFT:"calcellleft",CSS_CELL_RIGHT:"calcellright",CSS_CELL_BOTTOM:"calcellbottom",CSS_CELL_HOVER:"calcellhover",CSS_CELL_HIGHLIGHT1:"highlight1",CSS_CELL_HIGHLIGHT2:"highlight2",CSS_CELL_HIGHLIGHT3:"highlight3",CSS_CELL_HIGHLIGHT4:"highlight4"};};YAHOO.widget.Calendar.prototype.buildMonthLabel=function(){var text=this.Locale.LOCALE_MONTHS[this.cfg.getProperty("pagedate").getMonth()]+" "+this.cfg.getProperty("pagedate").getFullYear();return text;};YAHOO.widget.Calendar.prototype.buildDayLabel=function(workingDate){var day=workingDate.getDate();return day;};YAHOO.widget.Calendar.prototype.renderHeader=function(html){var colSpan=7;if(this.cfg.getProperty("SHOW_WEEK_HEADER")){colSpan+=1;}
if(this.cfg.getProperty("SHOW_WEEK_FOOTER")){colSpan+=1;}
html[html.length]="<thead>";html[html.length]="<tr>";html[html.length]='<th colspan="'+colSpan+'" class="'+this.Style.CSS_HEADER_TEXT+'">';html[html.length]='<div class="'+this.Style.CSS_HEADER+'">';var renderLeft,renderRight=false;if(this.parent){if(this.index===0){renderLeft=true;}
if(this.index==(this.parent.cfg.getProperty("pages")-1)){renderRight=true;}}else{renderLeft=true;renderRight=true;}
var cal=this.parent||this;if(renderLeft){html[html.length]='<a class="'+this.Style.CSS_NAV_LEFT+'" style="background-image:url('+this.cfg.getProperty("NAV_ARROW_LEFT")+')">&#160;</a>';}
html[html.length]=this.buildMonthLabel();if(renderRight){html[html.length]='<a class="'+this.Style.CSS_NAV_RIGHT+'" style="background-image:url('+this.cfg.getProperty("NAV_ARROW_RIGHT")+')">&#160;</a>';}
html[html.length]='</div>';html[html.length]='</th>';html[html.length]='</tr>';if(this.cfg.getProperty("SHOW_WEEKDAYS")){html=this.buildWeekdays(html);}
html[html.length]='</thead>';return html;};YAHOO.widget.Calendar.prototype.buildWeekdays=function(html){html[html.length]='<tr class="'+this.Style.CSS_WEEKDAY_ROW+'">';if(this.cfg.getProperty("SHOW_WEEK_HEADER")){html[html.length]='<th>&#160;</th>';}
for(var i=0;i<this.Locale.LOCALE_WEEKDAYS.length;++i){html[html.length]='<th class="calweekdaycell">'+this.Locale.LOCALE_WEEKDAYS[i]+'</th>';}
if(this.cfg.getProperty("SHOW_WEEK_FOOTER")){html[html.length]='<th>&#160;</th>';}
html[html.length]='</tr>';return html;};YAHOO.widget.Calendar.prototype.renderBody=function(workingDate,html){var startDay=this.cfg.getProperty("START_WEEKDAY");this.preMonthDays=workingDate.getDay();if(startDay>0){this.preMonthDays-=startDay;}
if(this.preMonthDays<0){this.preMonthDays+=7;}
this.monthDays=YAHOO.widget.DateMath.findMonthEnd(workingDate).getDate();this.postMonthDays=YAHOO.widget.Calendar.DISPLAY_DAYS-this.preMonthDays-this.monthDays;workingDate=YAHOO.widget.DateMath.subtract(workingDate,YAHOO.widget.DateMath.DAY,this.preMonthDays);var useDate,weekNum,weekClass;useDate=this.cfg.getProperty("pagedate");html[html.length]='<tbody class="m'+(useDate.getMonth()+1)+'">';var i=0;var tempDiv=document.createElement("div");var cell=document.createElement("td");tempDiv.appendChild(cell);var jan1=new Date(useDate.getFullYear(),0,1);var cal=this.parent||this;for(var r=0;r<6;r++){weekNum=YAHOO.widget.DateMath.getWeekNumber(workingDate,useDate.getFullYear(),startDay);weekClass="w"+weekNum;if(r!==0&&this.isDateOOM(workingDate)&&this.cfg.getProperty("HIDE_BLANK_WEEKS")===true){break;}else{html[html.length]='<tr class="'+weekClass+'">';if(this.cfg.getProperty("SHOW_WEEK_HEADER")){html=this.renderRowHeader(weekNum,html);}
for(var d=0;d<7;d++){var cellRenderers=[];this.clearElement(cell);YAHOO.util.Dom.addClass(cell,"calcell");cell.id=this.id+"_cell"+i;cell.innerHTML=i;var renderer=null;if(workingDate.getFullYear()==this.today.getFullYear()&&workingDate.getMonth()==this.today.getMonth()&&workingDate.getDate()==this.today.getDate()){cellRenderers[cellRenderers.length]=cal.renderCellStyleToday;}
this.cellDates[this.cellDates.length]=[workingDate.getFullYear(),workingDate.getMonth()+1,workingDate.getDate()];if(this.isDateOOM(workingDate)){cellRenderers[cellRenderers.length]=cal.renderCellNotThisMonth;}else{YAHOO.util.Dom.addClass(cell,"wd"+workingDate.getDay());YAHOO.util.Dom.addClass(cell,"d"+workingDate.getDate());for(var s=0;s<this.renderStack.length;++s){var rArray=this.renderStack[s];var type=rArray[0];var month;var day;var year;switch(type){case YAHOO.widget.Calendar.DATE:month=rArray[1][1];day=rArray[1][2];year=rArray[1][0];if(workingDate.getMonth()+1==month&&workingDate.getDate()==day&&workingDate.getFullYear()==year){renderer=rArray[2];this.renderStack.splice(s,1);}
break;case YAHOO.widget.Calendar.MONTH_DAY:month=rArray[1][0];day=rArray[1][1];if(workingDate.getMonth()+1==month&&workingDate.getDate()==day){renderer=rArray[2];this.renderStack.splice(s,1);}
break;case YAHOO.widget.Calendar.RANGE:var date1=rArray[1][0];var date2=rArray[1][1];var d1month=date1[1];var d1day=date1[2];var d1year=date1[0];var d1=new Date(d1year,d1month-1,d1day);var d2month=date2[1];var d2day=date2[2];var d2year=date2[0];var d2=new Date(d2year,d2month-1,d2day);if(workingDate.getTime()>=d1.getTime()&&workingDate.getTime()<=d2.getTime()){renderer=rArray[2];if(workingDate.getTime()==d2.getTime()){this.renderStack.splice(s,1);}}
break;case YAHOO.widget.Calendar.WEEKDAY:var weekday=rArray[1][0];if(workingDate.getDay()+1==weekday){renderer=rArray[2];}
break;case YAHOO.widget.Calendar.MONTH:month=rArray[1][0];if(workingDate.getMonth()+1==month){renderer=rArray[2];}
break;}
if(renderer){cellRenderers[cellRenderers.length]=renderer;}}}
if(this._indexOfSelectedFieldArray([workingDate.getFullYear(),workingDate.getMonth()+1,workingDate.getDate()])>-1){cellRenderers[cellRenderers.length]=cal.renderCellStyleSelected;}
var mindate=this.cfg.getProperty("mindate");var maxdate=this.cfg.getProperty("maxdate");if(mindate){mindate=YAHOO.widget.DateMath.clearTime(mindate);}
if(maxdate){maxdate=YAHOO.widget.DateMath.clearTime(maxdate);}
if((mindate&&(workingDate.getTime()<mindate.getTime()))||(maxdate&&(workingDate.getTime()>maxdate.getTime()))){cellRenderers[cellRenderers.length]=cal.renderOutOfBoundsDate;}else{cellRenderers[cellRenderers.length]=cal.styleCellDefault;cellRenderers[cellRenderers.length]=cal.renderCellDefault;}
for(var x=0;x<cellRenderers.length;++x){var ren=cellRenderers[x];if(ren.call((this.parent||this),workingDate,cell)==YAHOO.widget.Calendar.STOP_RENDER){break;}}
workingDate.setTime(workingDate.getTime()+YAHOO.widget.DateMath.ONE_DAY_MS);if(i>=0&&i<=6){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_TOP);}
if((i%7)===0){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_LEFT);}
if(((i+1)%7)===0){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_RIGHT);}
var postDays=this.postMonthDays;if(postDays>=7&&this.cfg.getProperty("HIDE_BLANK_WEEKS")){var blankWeeks=Math.floor(postDays/7);for(var p=0;p<blankWeeks;++p){postDays-=7;}}
if(i>=((this.preMonthDays+postDays+this.monthDays)-7)){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_BOTTOM);}
html[html.length]=tempDiv.innerHTML;i++;}
if(this.cfg.getProperty("SHOW_WEEK_FOOTER")){html=this.renderRowFooter(weekNum,html);}
html[html.length]='</tr>';}}
html[html.length]='</tbody>';return html;};YAHOO.widget.Calendar.prototype.renderFooter=function(html){return html;};YAHOO.widget.Calendar.prototype.render=function(){this.beforeRenderEvent.fire();var workingDate=YAHOO.widget.DateMath.findMonthStart(this.cfg.getProperty("pagedate"));this.resetRenderers();this.cellDates.length=0;YAHOO.util.Event.purgeElement(this.oDomContainer,true);var html=[];html[html.length]='<table cellSpacing="0" class="'+this.Style.CSS_CALENDAR+' y'+workingDate.getFullYear()+'" id="'+this.id+'">';html=this.renderHeader(html);html=this.renderBody(workingDate,html);html=this.renderFooter(html);html[html.length]='</table>';this.oDomContainer.innerHTML=html.join("\n");this.applyListeners();this.cells=this.oDomContainer.getElementsByTagName("td");this.cfg.refireEvent("title");this.cfg.refireEvent("close");this.cfg.refireEvent("iframe");this.renderEvent.fire();};YAHOO.widget.Calendar.prototype.applyListeners=function(){var root=this.oDomContainer;var cal=this.parent||this;var linkLeft,linkRight;linkLeft=YAHOO.util.Dom.getElementsByClassName(this.Style.CSS_NAV_LEFT,"a",root);linkRight=YAHOO.util.Dom.getElementsByClassName(this.Style.CSS_NAV_RIGHT,"a",root);if(linkLeft){this.linkLeft=linkLeft[0];YAHOO.util.Event.addListener(this.linkLeft,"mousedown",cal.previousMonth,cal,true);}
if(linkRight){this.linkRight=linkRight[0];YAHOO.util.Event.addListener(this.linkRight,"mousedown",cal.nextMonth,cal,true);}
if(this.domEventMap){var el,elements;for(var cls in this.domEventMap){if(this.domEventMap.hasOwnProperty(cls)){var items=this.domEventMap[cls];if(!(items instanceof Array)){items=[items];}
for(var i=0;i<items.length;i++){var item=items[i];elements=YAHOO.util.Dom.getElementsByClassName(cls,item.tag,this.oDomContainer);for(var c=0;c<elements.length;c++){el=elements[c];YAHOO.util.Event.addListener(el,item.event,item.handler,item.scope,item.correct);}}}}}
YAHOO.util.Event.addListener(this.oDomContainer,"click",this.doSelectCell,this);YAHOO.util.Event.addListener(this.oDomContainer,"mouseover",this.doCellMouseOver,this);YAHOO.util.Event.addListener(this.oDomContainer,"mouseout",this.doCellMouseOut,this);};YAHOO.widget.Calendar.prototype.getDateByCellId=function(id){var date=this.getDateFieldsByCellId(id);return new Date(date[0],date[1]-1,date[2]);};YAHOO.widget.Calendar.prototype.getDateFieldsByCellId=function(id){id=id.toLowerCase().split("_cell")[1];id=parseInt(id,10);return this.cellDates[id];};YAHOO.widget.Calendar.prototype.renderOutOfBoundsDate=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_OOB);cell.innerHTML=workingDate.getDate();return YAHOO.widget.Calendar.STOP_RENDER;};YAHOO.widget.Calendar.prototype.renderRowHeader=function(weekNum,html){html[html.length]='<th class="calrowhead">'+weekNum+'</th>';return html;};YAHOO.widget.Calendar.prototype.renderRowFooter=function(weekNum,html){html[html.length]='<th class="calrowfoot">'+weekNum+'</th>';return html;};YAHOO.widget.Calendar.prototype.renderCellDefault=function(workingDate,cell){cell.innerHTML='<a href="javascript:void(null);" >'+this.buildDayLabel(workingDate)+"</a>";};YAHOO.widget.Calendar.prototype.styleCellDefault=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_SELECTABLE);};YAHOO.widget.Calendar.prototype.renderCellStyleHighlight1=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_HIGHLIGHT1);};YAHOO.widget.Calendar.prototype.renderCellStyleHighlight2=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_HIGHLIGHT2);};YAHOO.widget.Calendar.prototype.renderCellStyleHighlight3=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_HIGHLIGHT3);};YAHOO.widget.Calendar.prototype.renderCellStyleHighlight4=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_HIGHLIGHT4);};YAHOO.widget.Calendar.prototype.renderCellStyleToday=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_TODAY);};YAHOO.widget.Calendar.prototype.renderCellStyleSelected=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_SELECTED);};YAHOO.widget.Calendar.prototype.renderCellNotThisMonth=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_OOM);cell.innerHTML=workingDate.getDate();return YAHOO.widget.Calendar.STOP_RENDER;};YAHOO.widget.Calendar.prototype.renderBodyCellRestricted=function(workingDate,cell){YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL);YAHOO.util.Dom.addClass(cell,this.Style.CSS_CELL_RESTRICTED);cell.innerHTML=workingDate.getDate();return YAHOO.widget.Calendar.STOP_RENDER;};YAHOO.widget.Calendar.prototype.addMonths=function(count){this.cfg.setProperty("pagedate",YAHOO.widget.DateMath.add(this.cfg.getProperty("pagedate"),YAHOO.widget.DateMath.MONTH,count));this.resetRenderers();this.changePageEvent.fire();};YAHOO.widget.Calendar.prototype.subtractMonths=function(count){this.cfg.setProperty("pagedate",YAHOO.widget.DateMath.subtract(this.cfg.getProperty("pagedate"),YAHOO.widget.DateMath.MONTH,count));this.resetRenderers();this.changePageEvent.fire();};YAHOO.widget.Calendar.prototype.addYears=function(count){this.cfg.setProperty("pagedate",YAHOO.widget.DateMath.add(this.cfg.getProperty("pagedate"),YAHOO.widget.DateMath.YEAR,count));this.resetRenderers();this.changePageEvent.fire();};YAHOO.widget.Calendar.prototype.subtractYears=function(count){this.cfg.setProperty("pagedate",YAHOO.widget.DateMath.subtract(this.cfg.getProperty("pagedate"),YAHOO.widget.DateMath.YEAR,count));this.resetRenderers();this.changePageEvent.fire();};YAHOO.widget.Calendar.prototype.nextMonth=function(){this.addMonths(1);};YAHOO.widget.Calendar.prototype.previousMonth=function(){this.subtractMonths(1);};YAHOO.widget.Calendar.prototype.nextYear=function(){this.addYears(1);};YAHOO.widget.Calendar.prototype.previousYear=function(){this.subtractYears(1);};YAHOO.widget.Calendar.prototype.reset=function(){this.cfg.resetProperty("selected");this.cfg.resetProperty("pagedate");this.resetEvent.fire();};YAHOO.widget.Calendar.prototype.clear=function(){this.cfg.setProperty("selected",[]);this.cfg.setProperty("pagedate",new Date(this.today.getTime()));this.clearEvent.fire();};YAHOO.widget.Calendar.prototype.select=function(date){this.beforeSelectEvent.fire();var selected=this.cfg.getProperty("selected");var aToBeSelected=this._toFieldArray(date);for(var a=0;a<aToBeSelected.length;++a){var toSelect=aToBeSelected[a];if(this._indexOfSelectedFieldArray(toSelect)==-1){selected[selected.length]=toSelect;}}
if(this.parent){this.parent.cfg.setProperty("selected",selected);}else{this.cfg.setProperty("selected",selected);}
this.selectEvent.fire(aToBeSelected);return this.getSelectedDates();};YAHOO.widget.Calendar.prototype.selectCell=function(cellIndex){this.beforeSelectEvent.fire();var selected=this.cfg.getProperty("selected");var cell=this.cells[cellIndex];var cellDate=this.cellDates[cellIndex];var dCellDate=this._toDate(cellDate);var selectDate=cellDate.concat();selected[selected.length]=selectDate;if(this.parent){this.parent.cfg.setProperty("selected",selected);}else{this.cfg.setProperty("selected",selected);}
this.renderCellStyleSelected(dCellDate,cell);this.selectEvent.fire([selectDate]);this.doCellMouseOut.call(cell,null,this);return this.getSelectedDates();};YAHOO.widget.Calendar.prototype.deselect=function(date){this.beforeDeselectEvent.fire();var selected=this.cfg.getProperty("selected");var aToBeSelected=this._toFieldArray(date);for(var a=0;a<aToBeSelected.length;++a){var toSelect=aToBeSelected[a];var index=this._indexOfSelectedFieldArray(toSelect);if(index!=-1){selected.splice(index,1);}}
if(this.parent){this.parent.cfg.setProperty("selected",selected);}else{this.cfg.setProperty("selected",selected);}
this.deselectEvent.fire(aToBeSelected);return this.getSelectedDates();};YAHOO.widget.Calendar.prototype.deselectCell=function(i){this.beforeDeselectEvent.fire();var selected=this.cfg.getProperty("selected");var cell=this.cells[i];var cellDate=this.cellDates[i];var cellDateIndex=this._indexOfSelectedFieldArray(cellDate);var dCellDate=this._toDate(cellDate);var selectDate=cellDate.concat();if(cellDateIndex>-1){if(this.cfg.getProperty("pagedate").getMonth()==dCellDate.getMonth()&&this.cfg.getProperty("pagedate").getFullYear()==dCellDate.getFullYear()){YAHOO.util.Dom.removeClass(cell,this.Style.CSS_CELL_SELECTED);}
selected.splice(cellDateIndex,1);}
if(this.parent){this.parent.cfg.setProperty("selected",selected);}else{this.cfg.setProperty("selected",selected);}
this.deselectEvent.fire(selectDate);return this.getSelectedDates();};YAHOO.widget.Calendar.prototype.deselectAll=function(){this.beforeDeselectEvent.fire();var selected=this.cfg.getProperty("selected");var count=selected.length;var sel=selected.concat();if(this.parent){this.parent.cfg.setProperty("selected",[]);}else{this.cfg.setProperty("selected",[]);}
if(count>0){this.deselectEvent.fire(sel);}
return this.getSelectedDates();};YAHOO.widget.Calendar.prototype._toFieldArray=function(date){var returnDate=[];if(date instanceof Date){returnDate=[[date.getFullYear(),date.getMonth()+1,date.getDate()]];}else if(typeof date=='string'){returnDate=this._parseDates(date);}else if(date instanceof Array){for(var i=0;i<date.length;++i){var d=date[i];returnDate[returnDate.length]=[d.getFullYear(),d.getMonth()+1,d.getDate()];}}
return returnDate;};YAHOO.widget.Calendar.prototype._toDate=function(dateFieldArray){if(dateFieldArray instanceof Date){return dateFieldArray;}else{return new Date(dateFieldArray[0],dateFieldArray[1]-1,dateFieldArray[2]);}};YAHOO.widget.Calendar.prototype._fieldArraysAreEqual=function(array1,array2){var match=false;if(array1[0]==array2[0]&&array1[1]==array2[1]&&array1[2]==array2[2]){match=true;}
return match;};YAHOO.widget.Calendar.prototype._indexOfSelectedFieldArray=function(find){var selected=-1;var seldates=this.cfg.getProperty("selected");for(var s=0;s<seldates.length;++s){var sArray=seldates[s];if(find[0]==sArray[0]&&find[1]==sArray[1]&&find[2]==sArray[2]){selected=s;break;}}
return selected;};YAHOO.widget.Calendar.prototype.isDateOOM=function(date){var isOOM=false;if(date.getMonth()!=this.cfg.getProperty("pagedate").getMonth()){isOOM=true;}
return isOOM;};YAHOO.widget.Calendar.prototype.onBeforeSelect=function(){if(this.cfg.getProperty("MULTI_SELECT")===false){if(this.parent){this.parent.callChildFunction("clearAllBodyCellStyles",this.Style.CSS_CELL_SELECTED);this.parent.deselectAll();}else{this.clearAllBodyCellStyles(this.Style.CSS_CELL_SELECTED);this.deselectAll();}}};YAHOO.widget.Calendar.prototype.onSelect=function(selected){};YAHOO.widget.Calendar.prototype.onBeforeDeselect=function(){};YAHOO.widget.Calendar.prototype.onDeselect=function(deselected){};YAHOO.widget.Calendar.prototype.onChangePage=function(){this.render();};YAHOO.widget.Calendar.prototype.onRender=function(){};YAHOO.widget.Calendar.prototype.onReset=function(){this.render();};YAHOO.widget.Calendar.prototype.onClear=function(){this.render();};YAHOO.widget.Calendar.prototype.validate=function(){return true;};YAHOO.widget.Calendar.prototype._parseDate=function(sDate){var aDate=sDate.split(this.Locale.DATE_FIELD_DELIMITER);var rArray;if(aDate.length==2){rArray=[aDate[this.Locale.MD_MONTH_POSITION-1],aDate[this.Locale.MD_DAY_POSITION-1]];rArray.type=YAHOO.widget.Calendar.MONTH_DAY;}else{rArray=[aDate[this.Locale.MDY_YEAR_POSITION-1],aDate[this.Locale.MDY_MONTH_POSITION-1],aDate[this.Locale.MDY_DAY_POSITION-1]];rArray.type=YAHOO.widget.Calendar.DATE;}
for(var i=0;i<rArray.length;i++){rArray[i]=parseInt(rArray[i],10);}
return rArray;};YAHOO.widget.Calendar.prototype._parseDates=function(sDates){var aReturn=[];var aDates=sDates.split(this.Locale.DATE_DELIMITER);for(var d=0;d<aDates.length;++d){var sDate=aDates[d];if(sDate.indexOf(this.Locale.DATE_RANGE_DELIMITER)!=-1){var aRange=sDate.split(this.Locale.DATE_RANGE_DELIMITER);var dateStart=this._parseDate(aRange[0]);var dateEnd=this._parseDate(aRange[1]);var fullRange=this._parseRange(dateStart,dateEnd);aReturn=aReturn.concat(fullRange);}else{var aDate=this._parseDate(sDate);aReturn.push(aDate);}}
return aReturn;};YAHOO.widget.Calendar.prototype._parseRange=function(startDate,endDate){var dStart=new Date(startDate[0],startDate[1]-1,startDate[2]);var dCurrent=YAHOO.widget.DateMath.add(new Date(startDate[0],startDate[1]-1,startDate[2]),YAHOO.widget.DateMath.DAY,1);var dEnd=new Date(endDate[0],endDate[1]-1,endDate[2]);var results=[];results.push(startDate);while(dCurrent.getTime()<=dEnd.getTime()){results.push([dCurrent.getFullYear(),dCurrent.getMonth()+1,dCurrent.getDate()]);dCurrent=YAHOO.widget.DateMath.add(dCurrent,YAHOO.widget.DateMath.DAY,1);}
return results;};YAHOO.widget.Calendar.prototype.resetRenderers=function(){this.renderStack=this._renderStack.concat();};YAHOO.widget.Calendar.prototype.clearElement=function(cell){cell.innerHTML="&#160;";cell.className="";};YAHOO.widget.Calendar.prototype.addRenderer=function(sDates,fnRender){var aDates=this._parseDates(sDates);for(var i=0;i<aDates.length;++i){var aDate=aDates[i];if(aDate.length==2){if(aDate[0]instanceof Array){this._addRenderer(YAHOO.widget.Calendar.RANGE,aDate,fnRender);}else{this._addRenderer(YAHOO.widget.Calendar.MONTH_DAY,aDate,fnRender);}}else if(aDate.length==3){this._addRenderer(YAHOO.widget.Calendar.DATE,aDate,fnRender);}}};YAHOO.widget.Calendar.prototype._addRenderer=function(type,aDates,fnRender){var add=[type,aDates,fnRender];this.renderStack.unshift(add);this._renderStack=this.renderStack.concat();};YAHOO.widget.Calendar.prototype.addMonthRenderer=function(month,fnRender){this._addRenderer(YAHOO.widget.Calendar.MONTH,[month],fnRender);};YAHOO.widget.Calendar.prototype.addWeekdayRenderer=function(weekday,fnRender){this._addRenderer(YAHOO.widget.Calendar.WEEKDAY,[weekday],fnRender);};YAHOO.widget.Calendar.prototype.clearAllBodyCellStyles=function(style){for(var c=0;c<this.cells.length;++c){YAHOO.util.Dom.removeClass(this.cells[c],style);}};YAHOO.widget.Calendar.prototype.setMonth=function(month){var current=this.cfg.getProperty("pagedate");current.setMonth(parseInt(month,10));this.cfg.setProperty("pagedate",current);};YAHOO.widget.Calendar.prototype.setYear=function(year){var current=this.cfg.getProperty("pagedate");current.setFullYear(parseInt(year,10));this.cfg.setProperty("pagedate",current);};YAHOO.widget.Calendar.prototype.getSelectedDates=function(){var returnDates=[];var selected=this.cfg.getProperty("selected");for(var d=0;d<selected.length;++d){var dateArray=selected[d];var date=new Date(dateArray[0],dateArray[1]-1,dateArray[2]);returnDates.push(date);}
returnDates.sort(function(a,b){return a-b;});return returnDates;};YAHOO.widget.Calendar.prototype.hide=function(){this.oDomContainer.style.display="none";};YAHOO.widget.Calendar.prototype.show=function(){this.oDomContainer.style.display="block";};YAHOO.widget.Calendar.prototype.browser=function(){var ua=navigator.userAgent.toLowerCase();if(ua.indexOf('opera')!=-1){return'opera';}else if(ua.indexOf('msie 7')!=-1){return'ie7';}else if(ua.indexOf('msie')!=-1){return'ie';}else if(ua.indexOf('safari')!=-1){return'safari';}else if(ua.indexOf('gecko')!=-1){return'gecko';}else{return false;}}();YAHOO.widget.Calendar.prototype.toString=function(){return"Calendar "+this.id;};YAHOO.widget.Calendar_Core=YAHOO.widget.Calendar;YAHOO.widget.Cal_Core=YAHOO.widget.Calendar;YAHOO.widget.CalendarGroup=function(id,containerId,config){if(arguments.length>0){this.init(id,containerId,config);}};YAHOO.widget.CalendarGroup.prototype.init=function(id,containerId,config){this.initEvents();this.initStyles();this.pages=[];this.id=id;this.containerId=containerId;this.oDomContainer=document.getElementById(containerId);YAHOO.util.Dom.addClass(this.oDomContainer,YAHOO.widget.CalendarGroup.CSS_CONTAINER);YAHOO.util.Dom.addClass(this.oDomContainer,YAHOO.widget.CalendarGroup.CSS_MULTI_UP);this.cfg=new YAHOO.util.Config(this);this.Options={};this.Locale={};this.setupConfig();if(config){this.cfg.applyConfig(config,true);}
this.cfg.fireQueue();if(this.browser=="opera"){var fixWidth=function(){var startW=this.oDomContainer.offsetWidth;var w=0;for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];w+=cal.oDomContainer.offsetWidth;}
if(w>0){this.oDomContainer.style.width=w+"px";}};this.renderEvent.subscribe(fixWidth,this,true);}};YAHOO.widget.CalendarGroup.prototype.setupConfig=function(){this.cfg.addProperty("pages",{value:2,validator:this.cfg.checkNumber,handler:this.configPages});this.cfg.addProperty("pagedate",{value:new Date(),handler:this.configPageDate});this.cfg.addProperty("selected",{value:[],handler:this.delegateConfig});this.cfg.addProperty("title",{value:"",handler:this.configTitle});this.cfg.addProperty("close",{value:false,handler:this.configClose});this.cfg.addProperty("iframe",{value:true,handler:this.delegateConfig,validator:this.cfg.checkBoolean});this.cfg.addProperty("mindate",{value:null,handler:this.delegateConfig});this.cfg.addProperty("maxdate",{value:null,handler:this.delegateConfig});this.cfg.addProperty("MULTI_SELECT",{value:false,handler:this.delegateConfig,validator:this.cfg.checkBoolean});this.cfg.addProperty("START_WEEKDAY",{value:0,handler:this.delegateConfig,validator:this.cfg.checkNumber});this.cfg.addProperty("SHOW_WEEKDAYS",{value:true,handler:this.delegateConfig,validator:this.cfg.checkBoolean});this.cfg.addProperty("SHOW_WEEK_HEADER",{value:false,handler:this.delegateConfig,validator:this.cfg.checkBoolean});this.cfg.addProperty("SHOW_WEEK_FOOTER",{value:false,handler:this.delegateConfig,validator:this.cfg.checkBoolean});this.cfg.addProperty("HIDE_BLANK_WEEKS",{value:false,handler:this.delegateConfig,validator:this.cfg.checkBoolean});this.cfg.addProperty("NAV_ARROW_LEFT",{value:YAHOO.widget.Calendar.IMG_ROOT+"us/tr/callt.gif",handler:this.delegateConfig});this.cfg.addProperty("NAV_ARROW_RIGHT",{value:YAHOO.widget.Calendar.IMG_ROOT+"us/tr/calrt.gif",handler:this.delegateConfig});this.cfg.addProperty("MONTHS_SHORT",{value:["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"],handler:this.delegateConfig});this.cfg.addProperty("MONTHS_LONG",{value:["January","February","March","April","May","June","July","August","September","October","November","December"],handler:this.delegateConfig});this.cfg.addProperty("WEEKDAYS_1CHAR",{value:["S","M","T","W","T","F","S"],handler:this.delegateConfig});this.cfg.addProperty("WEEKDAYS_SHORT",{value:["Su","Mo","Tu","We","Th","Fr","Sa"],handler:this.delegateConfig});this.cfg.addProperty("WEEKDAYS_MEDIUM",{value:["Sun","Mon","Tue","Wed","Thu","Fri","Sat"],handler:this.delegateConfig});this.cfg.addProperty("WEEKDAYS_LONG",{value:["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"],handler:this.delegateConfig});this.cfg.addProperty("LOCALE_MONTHS",{value:"long",handler:this.delegateConfig});this.cfg.addProperty("LOCALE_WEEKDAYS",{value:"short",handler:this.delegateConfig});this.cfg.addProperty("DATE_DELIMITER",{value:",",handler:this.delegateConfig});this.cfg.addProperty("DATE_FIELD_DELIMITER",{value:"/",handler:this.delegateConfig});this.cfg.addProperty("DATE_RANGE_DELIMITER",{value:"-",handler:this.delegateConfig});this.cfg.addProperty("MY_MONTH_POSITION",{value:1,handler:this.delegateConfig,validator:this.cfg.checkNumber});this.cfg.addProperty("MY_YEAR_POSITION",{value:2,handler:this.delegateConfig,validator:this.cfg.checkNumber});this.cfg.addProperty("MD_MONTH_POSITION",{value:1,handler:this.delegateConfig,validator:this.cfg.checkNumber});this.cfg.addProperty("MD_DAY_POSITION",{value:2,handler:this.delegateConfig,validator:this.cfg.checkNumber});this.cfg.addProperty("MDY_MONTH_POSITION",{value:1,handler:this.delegateConfig,validator:this.cfg.checkNumber});this.cfg.addProperty("MDY_DAY_POSITION",{value:2,handler:this.delegateConfig,validator:this.cfg.checkNumber});this.cfg.addProperty("MDY_YEAR_POSITION",{value:3,handler:this.delegateConfig,validator:this.cfg.checkNumber});};YAHOO.widget.CalendarGroup.prototype.initEvents=function(){var me=this;var sub=function(fn,obj,bOverride){for(var p=0;p<me.pages.length;++p){var cal=me.pages[p];cal[this.type+"Event"].subscribe(fn,obj,bOverride);}};var unsub=function(fn,obj){for(var p=0;p<me.pages.length;++p){var cal=me.pages[p];cal[this.type+"Event"].unsubscribe(fn,obj);}};this.beforeSelectEvent=new YAHOO.util.CustomEvent("beforeSelect");this.beforeSelectEvent.subscribe=sub;this.beforeSelectEvent.unsubscribe=unsub;this.selectEvent=new YAHOO.util.CustomEvent("select");this.selectEvent.subscribe=sub;this.selectEvent.unsubscribe=unsub;this.beforeDeselectEvent=new YAHOO.util.CustomEvent("beforeDeselect");this.beforeDeselectEvent.subscribe=sub;this.beforeDeselectEvent.unsubscribe=unsub;this.deselectEvent=new YAHOO.util.CustomEvent("deselect");this.deselectEvent.subscribe=sub;this.deselectEvent.unsubscribe=unsub;this.changePageEvent=new YAHOO.util.CustomEvent("changePage");this.changePageEvent.subscribe=sub;this.changePageEvent.unsubscribe=unsub;this.beforeRenderEvent=new YAHOO.util.CustomEvent("beforeRender");this.beforeRenderEvent.subscribe=sub;this.beforeRenderEvent.unsubscribe=unsub;this.renderEvent=new YAHOO.util.CustomEvent("render");this.renderEvent.subscribe=sub;this.renderEvent.unsubscribe=unsub;this.resetEvent=new YAHOO.util.CustomEvent("reset");this.resetEvent.subscribe=sub;this.resetEvent.unsubscribe=unsub;this.clearEvent=new YAHOO.util.CustomEvent("clear");this.clearEvent.subscribe=sub;this.clearEvent.unsubscribe=unsub;};YAHOO.widget.CalendarGroup.prototype.configPages=function(type,args,obj){var pageCount=args[0];for(var p=0;p<pageCount;++p){var calId=this.id+"_"+p;var calContainerId=this.containerId+"_"+p;var childConfig=this.cfg.getConfig();childConfig.close=false;childConfig.title=false;var cal=this.constructChild(calId,calContainerId,childConfig);var caldate=cal.cfg.getProperty("pagedate");caldate.setMonth(caldate.getMonth()+p);cal.cfg.setProperty("pagedate",caldate);YAHOO.util.Dom.removeClass(cal.oDomContainer,this.Style.CSS_SINGLE);YAHOO.util.Dom.addClass(cal.oDomContainer,"groupcal");if(p===0){YAHOO.util.Dom.addClass(cal.oDomContainer,"first");}
if(p==(pageCount-1)){YAHOO.util.Dom.addClass(cal.oDomContainer,"last");}
cal.parent=this;cal.index=p;this.pages[this.pages.length]=cal;}};YAHOO.widget.CalendarGroup.prototype.configPageDate=function(type,args,obj){var val=args[0];for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.cfg.setProperty("pagedate",val);var calDate=cal.cfg.getProperty("pagedate");calDate.setMonth(calDate.getMonth()+p);}};YAHOO.widget.CalendarGroup.prototype.delegateConfig=function(type,args,obj){var val=args[0];var cal;for(var p=0;p<this.pages.length;p++){cal=this.pages[p];cal.cfg.setProperty(type,val);}};YAHOO.widget.CalendarGroup.prototype.setChildFunction=function(fnName,fn){var pageCount=this.cfg.getProperty("pages");for(var p=0;p<pageCount;++p){this.pages[p][fnName]=fn;}};YAHOO.widget.CalendarGroup.prototype.callChildFunction=function(fnName,args){var pageCount=this.cfg.getProperty("pages");for(var p=0;p<pageCount;++p){var page=this.pages[p];if(page[fnName]){var fn=page[fnName];fn.call(page,args);}}};YAHOO.widget.CalendarGroup.prototype.constructChild=function(id,containerId,config){var container=document.getElementById(containerId);if(!container){container=document.createElement("div");container.id=containerId;this.oDomContainer.appendChild(container);}
return new YAHOO.widget.Calendar(id,containerId,config);};YAHOO.widget.CalendarGroup.prototype.setMonth=function(month){month=parseInt(month,10);for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.setMonth(month+p);}};YAHOO.widget.CalendarGroup.prototype.setYear=function(year){year=parseInt(year,10);for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];var pageDate=cal.cfg.getProperty("pageDate");if((pageDate.getMonth()+1)==1&&p>0){year+=1;}
cal.setYear(year);}};YAHOO.widget.CalendarGroup.prototype.render=function(){this.renderHeader();for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.render();}
this.renderFooter();};YAHOO.widget.CalendarGroup.prototype.select=function(date){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.select(date);}
return this.getSelectedDates();};YAHOO.widget.CalendarGroup.prototype.selectCell=function(cellIndex){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.selectCell(cellIndex);}
return this.getSelectedDates();};YAHOO.widget.CalendarGroup.prototype.deselect=function(date){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.deselect(date);}
return this.getSelectedDates();};YAHOO.widget.CalendarGroup.prototype.deselectAll=function(){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.deselectAll();}
return this.getSelectedDates();};YAHOO.widget.CalendarGroup.prototype.deselectCell=function(cellIndex){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.deselectCell(cellIndex);}
return this.getSelectedDates();};YAHOO.widget.CalendarGroup.prototype.reset=function(){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.reset();}};YAHOO.widget.CalendarGroup.prototype.clear=function(){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.clear();}};YAHOO.widget.CalendarGroup.prototype.nextMonth=function(){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.nextMonth();}};YAHOO.widget.CalendarGroup.prototype.previousMonth=function(){for(var p=this.pages.length-1;p>=0;--p){var cal=this.pages[p];cal.previousMonth();}};YAHOO.widget.CalendarGroup.prototype.nextYear=function(){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.nextYear();}};YAHOO.widget.CalendarGroup.prototype.previousYear=function(){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.previousYear();}};YAHOO.widget.CalendarGroup.prototype.getSelectedDates=function(){var returnDates=[];var selected=this.cfg.getProperty("selected");for(var d=0;d<selected.length;++d){var dateArray=selected[d];var date=new Date(dateArray[0],dateArray[1]-1,dateArray[2]);returnDates.push(date);}
returnDates.sort(function(a,b){return a-b;});return returnDates;};YAHOO.widget.CalendarGroup.prototype.addRenderer=function(sDates,fnRender){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.addRenderer(sDates,fnRender);}};YAHOO.widget.CalendarGroup.prototype.addMonthRenderer=function(month,fnRender){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.addMonthRenderer(month,fnRender);}};YAHOO.widget.CalendarGroup.prototype.addWeekdayRenderer=function(weekday,fnRender){for(var p=0;p<this.pages.length;++p){var cal=this.pages[p];cal.addWeekdayRenderer(weekday,fnRender);}};YAHOO.widget.CalendarGroup.prototype.renderHeader=function(){};YAHOO.widget.CalendarGroup.prototype.renderFooter=function(){};YAHOO.widget.CalendarGroup.prototype.addMonths=function(count){this.callChildFunction("addMonths",count);};YAHOO.widget.CalendarGroup.prototype.subtractMonths=function(count){this.callChildFunction("subtractMonths",count);};YAHOO.widget.CalendarGroup.prototype.addYears=function(count){this.callChildFunction("addYears",count);};YAHOO.widget.CalendarGroup.prototype.subtractYears=function(count){this.callChildFunction("subtractYears",count);};YAHOO.widget.CalendarGroup.CSS_CONTAINER="yui-calcontainer";YAHOO.widget.CalendarGroup.CSS_MULTI_UP="multi";YAHOO.widget.CalendarGroup.CSS_2UPTITLE="title";YAHOO.widget.CalendarGroup.CSS_2UPCLOSE="close-icon";YAHOO.augment(YAHOO.widget.CalendarGroup,YAHOO.widget.Calendar,"buildDayLabel","buildMonthLabel","renderOutOfBoundsDate","renderRowHeader","renderRowFooter","renderCellDefault","styleCellDefault","renderCellStyleHighlight1","renderCellStyleHighlight2","renderCellStyleHighlight3","renderCellStyleHighlight4","renderCellStyleToday","renderCellStyleSelected","renderCellNotThisMonth","renderBodyCellRestricted","initStyles","configTitle","configClose","hide","show","browser");YAHOO.widget.CalendarGroup.prototype.toString=function(){return"CalendarGroup "+this.id;};YAHOO.widget.CalGrp=YAHOO.widget.CalendarGroup;YAHOO.widget.Calendar2up=function(id,containerId,config){this.init(id,containerId,config);};YAHOO.extend(YAHOO.widget.Calendar2up,YAHOO.widget.CalendarGroup);YAHOO.widget.Cal2up=YAHOO.widget.Calendar2up;
/* Copyright (c) 2006, Yahoo! Inc. All rights reserved.Code licensed under the BSD License:http://developer.yahoo.net/yui/license.txtversion: 0.12.0 */ YAHOO.util.Anim=function(el,attributes,duration,method){if(el){this.init(el,attributes,duration,method);}};YAHOO.util.Anim.prototype={toString:function(){var el=this.getEl();var id=el.id||el.tagName;return("Anim "+id);},patterns:{noNegatives:/width|height|opacity|padding/i,offsetAttribute:/^((width|height)|(top|left))$/,defaultUnit:/width|height|top$|bottom$|left$|right$/i,offsetUnit:/\d+(em|%|en|ex|pt|in|cm|mm|pc)$/i},doMethod:function(attr,start,end){return this.method(this.currentFrame,start,end-start,this.totalFrames);},setAttribute:function(attr,val,unit){if(this.patterns.noNegatives.test(attr)){val=(val>0)?val:0;}YAHOO.util.Dom.setStyle(this.getEl(),attr,val+unit);},getAttribute:function(attr){var el=this.getEl();var val=YAHOO.util.Dom.getStyle(el,attr);if(val!=='auto'&&!this.patterns.offsetUnit.test(val)){return parseFloat(val);}var a=this.patterns.offsetAttribute.exec(attr)||[];var pos=!!(a[3]);var box=!!(a[2]);if(box||(YAHOO.util.Dom.getStyle(el,'position')=='absolute'&&pos)){val=el['offset'+a[0].charAt(0).toUpperCase()+a[0].substr(1)];}else{val=0;}return val;},getDefaultUnit:function(attr){if(this.patterns.defaultUnit.test(attr)){return'px';}return'';},setRuntimeAttribute:function(attr){var start;var end;var attributes=this.attributes;this.runtimeAttributes[attr]={};var isset=function(prop){return(typeof prop!=='undefined');};if(!isset(attributes[attr]['to'])&&!isset(attributes[attr]['by'])){return false;}start=(isset(attributes[attr]['from']))?attributes[attr]['from']:this.getAttribute(attr);if(isset(attributes[attr]['to'])){end=attributes[attr]['to'];}else if(isset(attributes[attr]['by'])){if(start.constructor==Array){end=[];for(var i=0,len=start.length;i<len;++i){end[i]=start[i]+attributes[attr]['by'][i];}}else{end=start+attributes[attr]['by'];}}this.runtimeAttributes[attr].start=start;this.runtimeAttributes[attr].end=end;this.runtimeAttributes[attr].unit=(isset(attributes[attr].unit))?attributes[attr]['unit']:this.getDefaultUnit(attr);},init:function(el,attributes,duration,method){var isAnimated=false;var startTime=null;var actualFrames=0;el=YAHOO.util.Dom.get(el);this.attributes=attributes||{};this.duration=duration||1;this.method=method||YAHOO.util.Easing.easeNone;this.useSeconds=true;this.currentFrame=0;this.totalFrames=YAHOO.util.AnimMgr.fps;this.getEl=function(){return el;};this.isAnimated=function(){return isAnimated;};this.getStartTime=function(){return startTime;};this.runtimeAttributes={};this.animate=function(){if(this.isAnimated()){return false;}this.currentFrame=0;this.totalFrames=(this.useSeconds)?Math.ceil(YAHOO.util.AnimMgr.fps*this.duration):this.duration;YAHOO.util.AnimMgr.registerElement(this);};this.stop=function(finish){if(finish){this.currentFrame=this.totalFrames;this._onTween.fire();}YAHOO.util.AnimMgr.stop(this);};var onStart=function(){this.onStart.fire();this.runtimeAttributes={};for(var attr in this.attributes){this.setRuntimeAttribute(attr);}isAnimated=true;actualFrames=0;startTime=new Date();};var onTween=function(){var data={duration:new Date()-this.getStartTime(),currentFrame:this.currentFrame};data.toString=function(){return('duration: '+data.duration+', currentFrame: '+data.currentFrame);};this.onTween.fire(data);var runtimeAttributes=this.runtimeAttributes;for(var attr in runtimeAttributes){this.setAttribute(attr,this.doMethod(attr,runtimeAttributes[attr].start,runtimeAttributes[attr].end),runtimeAttributes[attr].unit);}actualFrames+=1;};var onComplete=function(){var actual_duration=(new Date()-startTime)/1000;var data={duration:actual_duration,frames:actualFrames,fps:actualFrames/actual_duration};data.toString=function(){return('duration: '+data.duration+', frames: '+data.frames+', fps: '+data.fps);};isAnimated=false;actualFrames=0;this.onComplete.fire(data);};this._onStart=new YAHOO.util.CustomEvent('_start',this,true);this.onStart=new YAHOO.util.CustomEvent('start',this);this.onTween=new YAHOO.util.CustomEvent('tween',this);this._onTween=new YAHOO.util.CustomEvent('_tween',this,true);this.onComplete=new YAHOO.util.CustomEvent('complete',this);this._onComplete=new YAHOO.util.CustomEvent('_complete',this,true);this._onStart.subscribe(onStart);this._onTween.subscribe(onTween);this._onComplete.subscribe(onComplete);}};YAHOO.util.AnimMgr=new function(){var thread=null;var queue=[];var tweenCount=0;this.fps=200;this.delay=1;this.registerElement=function(tween){queue[queue.length]=tween;tweenCount+=1;tween._onStart.fire();this.start();};this.unRegister=function(tween,index){tween._onComplete.fire();index=index||getIndex(tween);if(index!=-1){queue.splice(index,1);}tweenCount-=1;if(tweenCount<=0){this.stop();}};this.start=function(){if(thread===null){thread=setInterval(this.run,this.delay);}};this.stop=function(tween){if(!tween){clearInterval(thread);for(var i=0,len=queue.length;i<len;++i){if(queue[i].isAnimated()){this.unRegister(tween,i);}}queue=[];thread=null;tweenCount=0;}else{this.unRegister(tween);}};this.run=function(){for(var i=0,len=queue.length;i<len;++i){var tween=queue[i];if(!tween||!tween.isAnimated()){continue;}if(tween.currentFrame<tween.totalFrames||tween.totalFrames===null){tween.currentFrame+=1;if(tween.useSeconds){correctFrame(tween);}tween._onTween.fire();}else{YAHOO.util.AnimMgr.stop(tween,i);}}};var getIndex=function(anim){for(var i=0,len=queue.length;i<len;++i){if(queue[i]==anim){return i;}}return-1;};var correctFrame=function(tween){var frames=tween.totalFrames;var frame=tween.currentFrame;var expected=(tween.currentFrame*tween.duration*1000/tween.totalFrames);var elapsed=(new Date()-tween.getStartTime());var tweak=0;if(elapsed<tween.duration*1000){tweak=Math.round((elapsed/expected-1)*tween.currentFrame);}else{tweak=frames-(frame+1);}if(tweak>0&&isFinite(tweak)){if(tween.currentFrame+tweak>=frames){tweak=frames-(frame+1);}tween.currentFrame+=tweak;}};};YAHOO.util.Bezier=new function(){this.getPosition=function(points,t){var n=points.length;var tmp=[];for(var i=0;i<n;++i){tmp[i]=[points[i][0],points[i][1]];}for(var j=1;j<n;++j){for(i=0;i<n-j;++i){tmp[i][0]=(1-t)*tmp[i][0]+t*tmp[parseInt(i+1,10)][0];tmp[i][1]=(1-t)*tmp[i][1]+t*tmp[parseInt(i+1,10)][1];}}return[tmp[0][0],tmp[0][1]];};};(function(){YAHOO.util.ColorAnim=function(el,attributes,duration,method){YAHOO.util.ColorAnim.superclass.constructor.call(this,el,attributes,duration,method);};YAHOO.extend(YAHOO.util.ColorAnim,YAHOO.util.Anim);var Y=YAHOO.util;var superclass=Y.ColorAnim.superclass;var proto=Y.ColorAnim.prototype;proto.toString=function(){var el=this.getEl();var id=el.id||el.tagName;return("ColorAnim "+id);};proto.patterns.color=/color$/i;proto.patterns.rgb=/^rgb\(([0-9]+)\s*,\s*([0-9]+)\s*,\s*([0-9]+)\)$/i;proto.patterns.hex=/^#?([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{2})$/i;proto.patterns.hex3=/^#?([0-9A-F]{1})([0-9A-F]{1})([0-9A-F]{1})$/i;proto.patterns.transparent=/^transparent|rgba\(0, 0, 0, 0\)$/;proto.parseColor=function(s){if(s.length==3){return s;}var c=this.patterns.hex.exec(s);if(c&&c.length==4){return[parseInt(c[1],16),parseInt(c[2],16),parseInt(c[3],16)];}c=this.patterns.rgb.exec(s);if(c&&c.length==4){return[parseInt(c[1],10),parseInt(c[2],10),parseInt(c[3],10)];}c=this.patterns.hex3.exec(s);if(c&&c.length==4){return[parseInt(c[1]+c[1],16),parseInt(c[2]+c[2],16),parseInt(c[3]+c[3],16)];}return null;};proto.getAttribute=function(attr){var el=this.getEl();if(this.patterns.color.test(attr)){var val=YAHOO.util.Dom.getStyle(el,attr);if(this.patterns.transparent.test(val)){var parent=el.parentNode;val=Y.Dom.getStyle(parent,attr);while(parent&&this.patterns.transparent.test(val)){parent=parent.parentNode;val=Y.Dom.getStyle(parent,attr);if(parent.tagName.toUpperCase()=='HTML'){val='#fff';}}}}else{val=superclass.getAttribute.call(this,attr);}return val;};proto.doMethod=function(attr,start,end){var val;if(this.patterns.color.test(attr)){val=[];for(var i=0,len=start.length;i<len;++i){val[i]=superclass.doMethod.call(this,attr,start[i],end[i]);}val='rgb('+Math.floor(val[0])+','+Math.floor(val[1])+','+Math.floor(val[2])+')';}else{val=superclass.doMethod.call(this,attr,start,end);}return val;};proto.setRuntimeAttribute=function(attr){superclass.setRuntimeAttribute.call(this,attr);if(this.patterns.color.test(attr)){var attributes=this.attributes;var start=this.parseColor(this.runtimeAttributes[attr].start);var end=this.parseColor(this.runtimeAttributes[attr].end);if(typeof attributes[attr]['to']==='undefined'&&typeof attributes[attr]['by']!=='undefined'){end=this.parseColor(attributes[attr].by);for(var i=0,len=start.length;i<len;++i){end[i]=start[i]+end[i];}}this.runtimeAttributes[attr].start=start;this.runtimeAttributes[attr].end=end;}};})();YAHOO.util.Easing={easeNone:function(t,b,c,d){return c*t/d+b;},easeIn:function(t,b,c,d){return c*(t/=d)*t+b;},easeOut:function(t,b,c,d){return-c*(t/=d)*(t-2)+b;},easeBoth:function(t,b,c,d){if((t/=d/2)<1)return c/2*t*t+b;return-c/2*((--t)*(t-2)-1)+b;},easeInStrong:function(t,b,c,d){return c*(t/=d)*t*t*t+b;},easeOutStrong:function(t,b,c,d){return-c*((t=t/d-1)*t*t*t-1)+b;},easeBothStrong:function(t,b,c,d){if((t/=d/2)<1)return c/2*t*t*t*t+b;return-c/2*((t-=2)*t*t*t-2)+b;},elasticIn:function(t,b,c,d,a,p){if(t==0)return b;if((t/=d)==1)return b+c;if(!p)p=d*.3;if(!a||a<Math.abs(c)){a=c;var s=p/4;}else var s=p/(2*Math.PI)*Math.asin(c/a);return-(a*Math.pow(2,10*(t-=1))*Math.sin((t*d-s)*(2*Math.PI)/p))+b;},elasticOut:function(t,b,c,d,a,p){if(t==0)return b;if((t/=d)==1)return b+c;if(!p)p=d*.3;if(!a||a<Math.abs(c)){a=c;var s=p/4;}else var s=p/(2*Math.PI)*Math.asin(c/a);return a*Math.pow(2,-10*t)*Math.sin((t*d-s)*(2*Math.PI)/p)+c+b;},elasticBoth:function(t,b,c,d,a,p){if(t==0)return b;if((t/=d/2)==2)return b+c;if(!p)p=d*(.3*1.5);if(!a||a<Math.abs(c)){a=c;var s=p/4;}else var s=p/(2*Math.PI)*Math.asin(c/a);if(t<1)return-.5*(a*Math.pow(2,10*(t-=1))*Math.sin((t*d-s)*(2*Math.PI)/p))+b;return a*Math.pow(2,-10*(t-=1))*Math.sin((t*d-s)*(2*Math.PI)/p)*.5+c+b;},backIn:function(t,b,c,d,s){if(typeof s=='undefined')s=1.70158;return c*(t/=d)*t*((s+1)*t-s)+b;},backOut:function(t,b,c,d,s){if(typeof s=='undefined')s=1.70158;return c*((t=t/d-1)*t*((s+1)*t+s)+1)+b;},backBoth:function(t,b,c,d,s){if(typeof s=='undefined')s=1.70158;if((t/=d/2)<1)return c/2*(t*t*(((s*=(1.525))+1)*t-s))+b;return c/2*((t-=2)*t*(((s*=(1.525))+1)*t+s)+2)+b;},bounceIn:function(t,b,c,d){return c-YAHOO.util.Easing.bounceOut(d-t,0,c,d)+b;},bounceOut:function(t,b,c,d){if((t/=d)<(1/2.75)){return c*(7.5625*t*t)+b;}else if(t<(2/2.75)){return c*(7.5625*(t-=(1.5/2.75))*t+.75)+b;}else if(t<(2.5/2.75)){return c*(7.5625*(t-=(2.25/2.75))*t+.9375)+b;}else{return c*(7.5625*(t-=(2.625/2.75))*t+.984375)+b;}},bounceBoth:function(t,b,c,d){if(t<d/2)return YAHOO.util.Easing.bounceIn(t*2,0,c,d)*.5+b;return YAHOO.util.Easing.bounceOut(t*2-d,0,c,d)*.5+c*.5+b;}};(function(){YAHOO.util.Motion=function(el,attributes,duration,method){if(el){YAHOO.util.Motion.superclass.constructor.call(this,el,attributes,duration,method);}};YAHOO.extend(YAHOO.util.Motion,YAHOO.util.ColorAnim);var Y=YAHOO.util;var superclass=Y.Motion.superclass;var proto=Y.Motion.prototype;proto.toString=function(){var el=this.getEl();var id=el.id||el.tagName;return("Motion "+id);};proto.patterns.points=/^points$/i;proto.setAttribute=function(attr,val,unit){if(this.patterns.points.test(attr)){unit=unit||'px';superclass.setAttribute.call(this,'left',val[0],unit);superclass.setAttribute.call(this,'top',val[1],unit);}else{superclass.setAttribute.call(this,attr,val,unit);}};proto.getAttribute=function(attr){if(this.patterns.points.test(attr)){var val=[superclass.getAttribute.call(this,'left'),superclass.getAttribute.call(this,'top')];}else{val=superclass.getAttribute.call(this,attr);}return val;};proto.doMethod=function(attr,start,end){var val=null;if(this.patterns.points.test(attr)){var t=this.method(this.currentFrame,0,100,this.totalFrames)/100;val=Y.Bezier.getPosition(this.runtimeAttributes[attr],t);}else{val=superclass.doMethod.call(this,attr,start,end);}return val;};proto.setRuntimeAttribute=function(attr){if(this.patterns.points.test(attr)){var el=this.getEl();var attributes=this.attributes;var start;var control=attributes['points']['control']||[];var end;var i,len;if(control.length>0&&!(control[0]instanceof Array)){control=[control];}else{var tmp=[];for(i=0,len=control.length;i<len;++i){tmp[i]=control[i];}control=tmp;}if(Y.Dom.getStyle(el,'position')=='static'){Y.Dom.setStyle(el,'position','relative');}if(isset(attributes['points']['from'])){Y.Dom.setXY(el,attributes['points']['from']);}else{Y.Dom.setXY(el,Y.Dom.getXY(el));}start=this.getAttribute('points');if(isset(attributes['points']['to'])){end=translateValues.call(this,attributes['points']['to'],start);var pageXY=Y.Dom.getXY(this.getEl());for(i=0,len=control.length;i<len;++i){control[i]=translateValues.call(this,control[i],start);}}else if(isset(attributes['points']['by'])){end=[start[0]+attributes['points']['by'][0],start[1]+attributes['points']['by'][1]];for(i=0,len=control.length;i<len;++i){control[i]=[start[0]+control[i][0],start[1]+control[i][1]];}}this.runtimeAttributes[attr]=[start];if(control.length>0){this.runtimeAttributes[attr]=this.runtimeAttributes[attr].concat(control);}this.runtimeAttributes[attr][this.runtimeAttributes[attr].length]=end;}else{superclass.setRuntimeAttribute.call(this,attr);}};var translateValues=function(val,start){var pageXY=Y.Dom.getXY(this.getEl());val=[val[0]-pageXY[0]+start[0],val[1]-pageXY[1]+start[1]];return val;};var isset=function(prop){return(typeof prop!=='undefined');};})();(function(){YAHOO.util.Scroll=function(el,attributes,duration,method){if(el){YAHOO.util.Scroll.superclass.constructor.call(this,el,attributes,duration,method);}};YAHOO.extend(YAHOO.util.Scroll,YAHOO.util.ColorAnim);var Y=YAHOO.util;var superclass=Y.Scroll.superclass;var proto=Y.Scroll.prototype;proto.toString=function(){var el=this.getEl();var id=el.id||el.tagName;return("Scroll "+id);};proto.doMethod=function(attr,start,end){var val=null;if(attr=='scroll'){val=[this.method(this.currentFrame,start[0],end[0]-start[0],this.totalFrames),this.method(this.currentFrame,start[1],end[1]-start[1],this.totalFrames)];}else{val=superclass.doMethod.call(this,attr,start,end);}return val;};proto.getAttribute=function(attr){var val=null;var el=this.getEl();if(attr=='scroll'){val=[el.scrollLeft,el.scrollTop];}else{val=superclass.getAttribute.call(this,attr);}return val;};proto.setAttribute=function(attr,val,unit){var el=this.getEl();if(attr=='scroll'){el.scrollLeft=val[0];el.scrollTop=val[1];}else{superclass.setAttribute.call(this,attr,val,unit);}};})();
/* Copyright (c) 2006, Yahoo! Inc. All rights reserved.Code licensed under the BSD License:http://developer.yahoo.net/yui/license.txt version: 0.12.0 */
YAHOO.widget.AutoComplete=function(elInput,elContainer,oDataSource,oConfigs){if(elInput&&elContainer&&oDataSource){if(oDataSource&&(oDataSource instanceof YAHOO.widget.DataSource)){this.dataSource=oDataSource;}
else{return;}
if(YAHOO.util.Dom.inDocument(elInput)){if(typeof elInput=="string"){this._sName="instance"+YAHOO.widget.AutoComplete._nIndex+" "+elInput;this._oTextbox=document.getElementById(elInput);}
else{this._sName=(elInput.id)?"instance"+YAHOO.widget.AutoComplete._nIndex+" "+elInput.id:"instance"+YAHOO.widget.AutoComplete._nIndex;this._oTextbox=elInput;}}
else{return;}
if(YAHOO.util.Dom.inDocument(elContainer)){if(typeof elContainer=="string"){this._oContainer=document.getElementById(elContainer);}
else{this._oContainer=elContainer;}
if(this._oContainer.style.display=="none"){}}
else{return;}
if(typeof oConfigs=="object"){for(var sConfig in oConfigs){if(sConfig){this[sConfig]=oConfigs[sConfig];}}}
this._initContainer();this._initProps();this._initList();this._initContainerHelpers();var oSelf=this;var oTextbox=this._oTextbox;var oContent=this._oContainer._oContent;YAHOO.util.Event.addListener(oTextbox,"keyup",oSelf._onTextboxKeyUp,oSelf);YAHOO.util.Event.addListener(oTextbox,"keydown",oSelf._onTextboxKeyDown,oSelf);YAHOO.util.Event.addListener(oTextbox,"focus",oSelf._onTextboxFocus,oSelf);YAHOO.util.Event.addListener(oTextbox,"blur",oSelf._onTextboxBlur,oSelf);YAHOO.util.Event.addListener(oContent,"mouseover",oSelf._onContainerMouseover,oSelf);YAHOO.util.Event.addListener(oContent,"mouseout",oSelf._onContainerMouseout,oSelf);YAHOO.util.Event.addListener(oContent,"scroll",oSelf._onContainerScroll,oSelf);YAHOO.util.Event.addListener(oContent,"resize",oSelf._onContainerResize,oSelf);if(oTextbox.form){YAHOO.util.Event.addListener(oTextbox.form,"submit",oSelf._onFormSubmit,oSelf);}
YAHOO.util.Event.addListener(oTextbox,"keypress",oSelf._onTextboxKeyPress,oSelf);this.textboxFocusEvent=new YAHOO.util.CustomEvent("textboxFocus",this);this.textboxKeyEvent=new YAHOO.util.CustomEvent("textboxKey",this);this.dataRequestEvent=new YAHOO.util.CustomEvent("dataRequest",this);this.dataReturnEvent=new YAHOO.util.CustomEvent("dataReturn",this);this.dataErrorEvent=new YAHOO.util.CustomEvent("dataError",this);this.containerExpandEvent=new YAHOO.util.CustomEvent("containerExpand",this);this.typeAheadEvent=new YAHOO.util.CustomEvent("typeAhead",this);this.itemMouseOverEvent=new YAHOO.util.CustomEvent("itemMouseOver",this);this.itemMouseOutEvent=new YAHOO.util.CustomEvent("itemMouseOut",this);this.itemArrowToEvent=new YAHOO.util.CustomEvent("itemArrowTo",this);this.itemArrowFromEvent=new YAHOO.util.CustomEvent("itemArrowFrom",this);this.itemSelectEvent=new YAHOO.util.CustomEvent("itemSelect",this);this.unmatchedItemSelectEvent=new YAHOO.util.CustomEvent("unmatchedItemSelect",this);this.selectionEnforceEvent=new YAHOO.util.CustomEvent("selectionEnforce",this);this.containerCollapseEvent=new YAHOO.util.CustomEvent("containerCollapse",this);this.textboxBlurEvent=new YAHOO.util.CustomEvent("textboxBlur",this);oTextbox.setAttribute("autocomplete","off");YAHOO.widget.AutoComplete._nIndex++;}
else{}};YAHOO.widget.AutoComplete.prototype.dataSource=null;YAHOO.widget.AutoComplete.prototype.minQueryLength=1;YAHOO.widget.AutoComplete.prototype.maxResultsDisplayed=10;YAHOO.widget.AutoComplete.prototype.queryDelay=0.5;YAHOO.widget.AutoComplete.prototype.highlightClassName="yui-ac-highlight";YAHOO.widget.AutoComplete.prototype.prehighlightClassName=null;YAHOO.widget.AutoComplete.prototype.delimChar=null;YAHOO.widget.AutoComplete.prototype.autoHighlight=true;YAHOO.widget.AutoComplete.prototype.typeAhead=false;YAHOO.widget.AutoComplete.prototype.animHoriz=false;YAHOO.widget.AutoComplete.prototype.animVert=true;YAHOO.widget.AutoComplete.prototype.animSpeed=0.3;YAHOO.widget.AutoComplete.prototype.forceSelection=false;YAHOO.widget.AutoComplete.prototype.allowBrowserAutocomplete=true;YAHOO.widget.AutoComplete.prototype.alwaysShowContainer=false;YAHOO.widget.AutoComplete.prototype.useIFrame=false;YAHOO.widget.AutoComplete.prototype.useShadow=false;YAHOO.widget.AutoComplete.prototype.toString=function(){return"AutoComplete "+this._sName;};YAHOO.widget.AutoComplete.prototype.isContainerOpen=function(){return this._bContainerOpen;};YAHOO.widget.AutoComplete.prototype.getListItems=function(){return this._aListItems;};YAHOO.widget.AutoComplete.prototype.getListItemData=function(oListItem){if(oListItem._oResultData){return oListItem._oResultData;}
else{return false;}};YAHOO.widget.AutoComplete.prototype.setHeader=function(sHeader){if(sHeader){if(this._oContainer._oContent._oHeader){this._oContainer._oContent._oHeader.innerHTML=sHeader;this._oContainer._oContent._oHeader.style.display="block";}}
else{this._oContainer._oContent._oHeader.innerHTML="";this._oContainer._oContent._oHeader.style.display="none";}};YAHOO.widget.AutoComplete.prototype.setFooter=function(sFooter){if(sFooter){if(this._oContainer._oContent._oFooter){this._oContainer._oContent._oFooter.innerHTML=sFooter;this._oContainer._oContent._oFooter.style.display="block";}}
else{this._oContainer._oContent._oFooter.innerHTML="";this._oContainer._oContent._oFooter.style.display="none";}};YAHOO.widget.AutoComplete.prototype.setBody=function(sBody){if(sBody){if(this._oContainer._oContent._oBody){this._oContainer._oContent._oBody.innerHTML=sBody;this._oContainer._oContent._oBody.style.display="block";this._oContainer._oContent.style.display="block";}}
else{this._oContainer._oContent._oBody.innerHTML="";this._oContainer._oContent.style.display="none";}
this._maxResultsDisplayed=0;};YAHOO.widget.AutoComplete.prototype.formatResult=function(oResultItem,sQuery){var sResult=oResultItem[0];if(sResult){return sResult;}
else{return"";}};YAHOO.widget.AutoComplete.prototype.doBeforeExpandContainer=function(oResultItem,sQuery){return true;};YAHOO.widget.AutoComplete.prototype.sendQuery=function(sQuery){this._sendQuery(sQuery);};YAHOO.widget.AutoComplete.prototype.textboxFocusEvent=null;YAHOO.widget.AutoComplete.prototype.textboxKeyEvent=null;YAHOO.widget.AutoComplete.prototype.dataRequestEvent=null;YAHOO.widget.AutoComplete.prototype.dataReturnEvent=null;YAHOO.widget.AutoComplete.prototype.dataErrorEvent=null;YAHOO.widget.AutoComplete.prototype.containerExpandEvent=null;YAHOO.widget.AutoComplete.prototype.typeAheadEvent=null;YAHOO.widget.AutoComplete.prototype.itemMouseOverEvent=null;YAHOO.widget.AutoComplete.prototype.itemMouseOutEvent=null;YAHOO.widget.AutoComplete.prototype.itemArrowToEvent=null;YAHOO.widget.AutoComplete.prototype.itemArrowFromEvent=null;YAHOO.widget.AutoComplete.prototype.itemSelectEvent=null;YAHOO.widget.AutoComplete.prototype.unmatchedItemSelectEvent=null;YAHOO.widget.AutoComplete.prototype.selectionEnforceEvent=null;YAHOO.widget.AutoComplete.prototype.containerCollapseEvent=null;YAHOO.widget.AutoComplete.prototype.textboxBlurEvent=null;YAHOO.widget.AutoComplete._nIndex=0;YAHOO.widget.AutoComplete.prototype._sName=null;YAHOO.widget.AutoComplete.prototype._oTextbox=null;YAHOO.widget.AutoComplete.prototype._bFocused=true;YAHOO.widget.AutoComplete.prototype._oAnim=null;YAHOO.widget.AutoComplete.prototype._oContainer=null;YAHOO.widget.AutoComplete.prototype._bContainerOpen=false;YAHOO.widget.AutoComplete.prototype._bOverContainer=false;YAHOO.widget.AutoComplete.prototype._aListItems=null;YAHOO.widget.AutoComplete.prototype._nDisplayedItems=0;YAHOO.widget.AutoComplete.prototype._maxResultsDisplayed=0;YAHOO.widget.AutoComplete.prototype._sCurQuery=null;YAHOO.widget.AutoComplete.prototype._sSavedQuery=null;YAHOO.widget.AutoComplete.prototype._oCurItem=null;YAHOO.widget.AutoComplete.prototype._bItemSelected=false;YAHOO.widget.AutoComplete.prototype._nKeyCode=null;YAHOO.widget.AutoComplete.prototype._nDelayID=-1;YAHOO.widget.AutoComplete.prototype._iFrameSrc="javascript:false;";YAHOO.widget.AutoComplete.prototype._queryInterval=null;YAHOO.widget.AutoComplete.prototype._sLastTextboxValue=null;YAHOO.widget.AutoComplete.prototype._initProps=function(){var minQueryLength=this.minQueryLength;if(isNaN(minQueryLength)||(minQueryLength<1)){minQueryLength=1;}
var maxResultsDisplayed=this.maxResultsDisplayed;if(isNaN(this.maxResultsDisplayed)||(this.maxResultsDisplayed<1)){this.maxResultsDisplayed=10;}
var queryDelay=this.queryDelay;if(isNaN(this.queryDelay)||(this.queryDelay<0)){this.queryDelay=0.5;}
var aDelimChar=(this.delimChar)?this.delimChar:null;if(aDelimChar){if(typeof aDelimChar=="string"){this.delimChar=[aDelimChar];}
else if(aDelimChar.constructor!=Array){this.delimChar=null;}}
var animSpeed=this.animSpeed;if((this.animHoriz||this.animVert)&&YAHOO.util.Anim){if(isNaN(animSpeed)||(animSpeed<0)){animSpeed=0.3;}
if(!this._oAnim){oAnim=new YAHOO.util.Anim(this._oContainer._oContent,{},this.animSpeed);this._oAnim=oAnim;}
else{this._oAnim.duration=animSpeed;}}
if(this.forceSelection&&this.delimChar){}};YAHOO.widget.AutoComplete.prototype._initContainerHelpers=function(){if(this.useShadow&&!this._oContainer._oShadow){var oShadow=document.createElement("div");oShadow.className="yui-ac-shadow";this._oContainer._oShadow=this._oContainer.appendChild(oShadow);}
if(this.useIFrame&&!this._oContainer._oIFrame){var oIFrame=document.createElement("iframe");oIFrame.src=this._iFrameSrc;oIFrame.frameBorder=0;oIFrame.scrolling="no";oIFrame.style.position="absolute";oIFrame.style.width="100%";oIFrame.style.height="100%";oIFrame.tabIndex=-1;this._oContainer._oIFrame=this._oContainer.appendChild(oIFrame);}};YAHOO.widget.AutoComplete.prototype._initContainer=function(){if(!this._oContainer._oContent){var oContent=document.createElement("div");oContent.className="yui-ac-content";oContent.style.display="none";this._oContainer._oContent=this._oContainer.appendChild(oContent);var oHeader=document.createElement("div");oHeader.className="yui-ac-hd";oHeader.style.display="none";this._oContainer._oContent._oHeader=this._oContainer._oContent.appendChild(oHeader);var oBody=document.createElement("div");oBody.className="yui-ac-bd";this._oContainer._oContent._oBody=this._oContainer._oContent.appendChild(oBody);var oFooter=document.createElement("div");oFooter.className="yui-ac-ft";oFooter.style.display="none";this._oContainer._oContent._oFooter=this._oContainer._oContent.appendChild(oFooter);}
else{}};YAHOO.widget.AutoComplete.prototype._initList=function(){this._aListItems=[];while(this._oContainer._oContent._oBody.hasChildNodes()){var oldListItems=this.getListItems();if(oldListItems){for(var oldi=oldListItems.length-1;oldi>=0;i--){oldListItems[oldi]=null;}}
this._oContainer._oContent._oBody.innerHTML="";}
var oList=document.createElement("ul");oList=this._oContainer._oContent._oBody.appendChild(oList);for(var i=0;i<this.maxResultsDisplayed;i++){var oItem=document.createElement("li");oItem=oList.appendChild(oItem);this._aListItems[i]=oItem;this._initListItem(oItem,i);}
this._maxResultsDisplayed=this.maxResultsDisplayed;};YAHOO.widget.AutoComplete.prototype._initListItem=function(oItem,nItemIndex){var oSelf=this;oItem.style.display="none";oItem._nItemIndex=nItemIndex;oItem.mouseover=oItem.mouseout=oItem.onclick=null;YAHOO.util.Event.addListener(oItem,"mouseover",oSelf._onItemMouseover,oSelf);YAHOO.util.Event.addListener(oItem,"mouseout",oSelf._onItemMouseout,oSelf);YAHOO.util.Event.addListener(oItem,"click",oSelf._onItemMouseclick,oSelf);};YAHOO.widget.AutoComplete.prototype._onIMEDetected=function(oSelf){oSelf._enableIntervalDetection();};YAHOO.widget.AutoComplete.prototype._enableIntervalDetection=function(){var currValue=this._oTextbox.value;var lastValue=this._sLastTextboxValue;if(currValue!=lastValue){this._sLastTextboxValue=currValue;this._sendQuery(currValue);}};YAHOO.widget.AutoComplete.prototype._cancelIntervalDetection=function(oSelf){if(oSelf._queryInterval){clearInterval(oSelf._queryInterval);}};YAHOO.widget.AutoComplete.prototype._isIgnoreKey=function(nKeyCode){if((nKeyCode==9)||(nKeyCode==13)||(nKeyCode==16)||(nKeyCode==17)||(nKeyCode>=18&&nKeyCode<=20)||(nKeyCode==27)||(nKeyCode>=33&&nKeyCode<=35)||(nKeyCode>=36&&nKeyCode<=38)||(nKeyCode==40)||(nKeyCode>=44&&nKeyCode<=45)){return true;}
return false;};YAHOO.widget.AutoComplete.prototype._sendQuery=function(sQuery){if(this.minQueryLength==-1){this._toggleContainer(false);return;}
var aDelimChar=(this.delimChar)?this.delimChar:null;if(aDelimChar){var nDelimIndex=-1;for(var i=aDelimChar.length-1;i>=0;i--){var nNewIndex=sQuery.lastIndexOf(aDelimChar[i]);if(nNewIndex>nDelimIndex){nDelimIndex=nNewIndex;}}
if(aDelimChar[i]==" "){for(var j=aDelimChar.length-1;j>=0;j--){if(sQuery[nDelimIndex-1]==aDelimChar[j]){nDelimIndex--;break;}}}
if(nDelimIndex>-1){var nQueryStart=nDelimIndex+1;while(sQuery.charAt(nQueryStart)==" "){nQueryStart+=1;}
this._sSavedQuery=sQuery.substring(0,nQueryStart);sQuery=sQuery.substr(nQueryStart);}
else if(sQuery.indexOf(this._sSavedQuery)<0){this._sSavedQuery=null;}}
if(sQuery&&(sQuery.length<this.minQueryLength)||(!sQuery&&this.minQueryLength>0)){if(this._nDelayID!=-1){clearTimeout(this._nDelayID);}
this._toggleContainer(false);return;}
sQuery=encodeURIComponent(sQuery);this._nDelayID=-1;this.dataRequestEvent.fire(this,sQuery);this.dataSource.getResults(this._populateList,sQuery,this);};YAHOO.widget.AutoComplete.prototype._populateList=function(sQuery,aResults,oSelf){if(aResults===null){oSelf.dataErrorEvent.fire(oSelf,sQuery);}
if(!oSelf._bFocused||!aResults){return;}
var isOpera=(navigator.userAgent.toLowerCase().indexOf("opera")!=-1);var contentStyle=oSelf._oContainer._oContent.style;contentStyle.width=(!isOpera)?null:"";contentStyle.height=(!isOpera)?null:"";var sCurQuery=decodeURIComponent(sQuery);oSelf._sCurQuery=sCurQuery;oSelf._bItemSelected=false;if(oSelf._maxResultsDisplayed!=oSelf.maxResultsDisplayed){oSelf._initList();}
var nItems=Math.min(aResults.length,oSelf.maxResultsDisplayed);oSelf._nDisplayedItems=nItems;if(nItems>0){oSelf._initContainerHelpers();var aItems=oSelf._aListItems;for(var i=nItems-1;i>=0;i--){var oItemi=aItems[i];var oResultItemi=aResults[i];oItemi.innerHTML=oSelf.formatResult(oResultItemi,sCurQuery);oItemi.style.display="list-item";oItemi._sResultKey=oResultItemi[0];oItemi._oResultData=oResultItemi;}
for(var j=aItems.length-1;j>=nItems;j--){var oItemj=aItems[j];oItemj.innerHTML=null;oItemj.style.display="none";oItemj._sResultKey=null;oItemj._oResultData=null;}
if(oSelf.autoHighlight){var oFirstItem=aItems[0];oSelf._toggleHighlight(oFirstItem,"to");oSelf.itemArrowToEvent.fire(oSelf,oFirstItem);oSelf._typeAhead(oFirstItem,sQuery);}
else{oSelf._oCurItem=null;}
var ok=oSelf.doBeforeExpandContainer(oSelf._oTextbox,oSelf._oContainer,sQuery,aResults);oSelf._toggleContainer(ok);}
else{oSelf._toggleContainer(false);}
oSelf.dataReturnEvent.fire(oSelf,sQuery,aResults);};YAHOO.widget.AutoComplete.prototype._clearSelection=function(){var sValue=this._oTextbox.value;var sChar=(this.delimChar)?this.delimChar[0]:null;var nIndex=(sChar)?sValue.lastIndexOf(sChar,sValue.length-2):-1;if(nIndex>-1){this._oTextbox.value=sValue.substring(0,nIndex);}
else{this._oTextbox.value="";}
this._sSavedQuery=this._oTextbox.value;this.selectionEnforceEvent.fire(this);};YAHOO.widget.AutoComplete.prototype._textMatchesOption=function(){var foundMatch=false;for(var i=this._nDisplayedItems-1;i>=0;i--){var oItem=this._aListItems[i];var sMatch=oItem._sResultKey.toLowerCase();if(sMatch==this._sCurQuery.toLowerCase()){foundMatch=true;break;}}
return(foundMatch);};YAHOO.widget.AutoComplete.prototype._typeAhead=function(oItem,sQuery){if(!this.typeAhead){return;}
var oTextbox=this._oTextbox;var sValue=this._oTextbox.value;if(!oTextbox.setSelectionRange&&!oTextbox.createTextRange){return;}
var nStart=sValue.length;this._updateValue(oItem);var nEnd=oTextbox.value.length;this._selectText(oTextbox,nStart,nEnd);var sPrefill=oTextbox.value.substr(nStart,nEnd);this.typeAheadEvent.fire(this,sQuery,sPrefill);};YAHOO.widget.AutoComplete.prototype._selectText=function(oTextbox,nStart,nEnd){if(oTextbox.setSelectionRange){oTextbox.setSelectionRange(nStart,nEnd);}
else if(oTextbox.createTextRange){var oTextRange=oTextbox.createTextRange();oTextRange.moveStart("character",nStart);oTextRange.moveEnd("character",nEnd-oTextbox.value.length);oTextRange.select();}
else{oTextbox.select();}};YAHOO.widget.AutoComplete.prototype._toggleContainerHelpers=function(bShow){var bFireEvent=false;var width=this._oContainer._oContent.offsetWidth+"px";var height=this._oContainer._oContent.offsetHeight+"px";if(this.useIFrame&&this._oContainer._oIFrame){bFireEvent=true;if(bShow){this._oContainer._oIFrame.style.width=width;this._oContainer._oIFrame.style.height=height;}
else{this._oContainer._oIFrame.style.width=0;this._oContainer._oIFrame.style.height=0;}}
if(this.useShadow&&this._oContainer._oShadow){bFireEvent=true;if(bShow){this._oContainer._oShadow.style.width=width;this._oContainer._oShadow.style.height=height;}
else{this._oContainer._oShadow.style.width=0;this._oContainer._oShadow.style.height=0;}}};YAHOO.widget.AutoComplete.prototype._toggleContainer=function(bShow){var oContainer=this._oContainer;if(this.alwaysShowContainer&&this._bContainerOpen){return;}
if(!bShow){this._oContainer._oContent.scrollTop=0;var aItems=this._aListItems;if(aItems&&(aItems.length>0)){for(var i=aItems.length-1;i>=0;i--){aItems[i].style.display="none";}}
if(this._oCurItem){this._toggleHighlight(this._oCurItem,"from");}
this._oCurItem=null;this._nDisplayedItems=0;this._sCurQuery=null;}
if(!bShow&&!this._bContainerOpen){oContainer._oContent.style.display="none";return;}
var oAnim=this._oAnim;if(oAnim&&oAnim.getEl()&&(this.animHoriz||this.animVert)){if(!bShow){this._toggleContainerHelpers(bShow);}
if(oAnim.isAnimated()){oAnim.stop();}
var oClone=oContainer._oContent.cloneNode(true);oContainer.appendChild(oClone);oClone.style.top="-9000px";oClone.style.display="block";var wExp=oClone.offsetWidth;var hExp=oClone.offsetHeight;var wColl=(this.animHoriz)?0:wExp;var hColl=(this.animVert)?0:hExp;oAnim.attributes=(bShow)?{width:{to:wExp},height:{to:hExp}}:{width:{to:wColl},height:{to:hColl}};if(bShow&&!this._bContainerOpen){oContainer._oContent.style.width=wColl+"px";oContainer._oContent.style.height=hColl+"px";}
else{oContainer._oContent.style.width=wExp+"px";oContainer._oContent.style.height=hExp+"px";}
oContainer.removeChild(oClone);oClone=null;var oSelf=this;var onAnimComplete=function(){oAnim.onComplete.unsubscribeAll();if(bShow){oSelf.containerExpandEvent.fire(oSelf);}
else{oContainer._oContent.style.display="none";oSelf.containerCollapseEvent.fire(oSelf);}
oSelf._toggleContainerHelpers(bShow);};oContainer._oContent.style.display="block";oAnim.onComplete.subscribe(onAnimComplete);oAnim.animate();this._bContainerOpen=bShow;}
else{if(bShow){oContainer._oContent.style.display="block";this.containerExpandEvent.fire(this);}
else{oContainer._oContent.style.display="none";this.containerCollapseEvent.fire(this);}
this._toggleContainerHelpers(bShow);this._bContainerOpen=bShow;}};YAHOO.widget.AutoComplete.prototype._toggleHighlight=function(oNewItem,sType){var sHighlight=this.highlightClassName;if(this._oCurItem){YAHOO.util.Dom.removeClass(this._oCurItem,sHighlight);}
if((sType=="to")&&sHighlight){YAHOO.util.Dom.addClass(oNewItem,sHighlight);this._oCurItem=oNewItem;}};YAHOO.widget.AutoComplete.prototype._togglePrehighlight=function(oNewItem,sType){if(oNewItem==this._oCurItem){return;}
var sPrehighlight=this.prehighlightClassName;if((sType=="mouseover")&&sPrehighlight){YAHOO.util.Dom.addClass(oNewItem,sPrehighlight);}
else{YAHOO.util.Dom.removeClass(oNewItem,sPrehighlight);}};YAHOO.widget.AutoComplete.prototype._updateValue=function(oItem){var oTextbox=this._oTextbox;var sDelimChar=(this.delimChar)?(this.delimChar[0]||this.delimChar):null;var sSavedQuery=this._sSavedQuery;var sResultKey=oItem._sResultKey;oTextbox.focus();oTextbox.value="";if(sDelimChar){if(sSavedQuery){oTextbox.value=sSavedQuery;}
oTextbox.value+=sResultKey+sDelimChar;if(sDelimChar!=" "){oTextbox.value+=" ";}}
else{oTextbox.value=sResultKey;}
if(oTextbox.type=="textarea"){oTextbox.scrollTop=oTextbox.scrollHeight;}
var end=oTextbox.value.length;this._selectText(oTextbox,end,end);this._oCurItem=oItem;};YAHOO.widget.AutoComplete.prototype._selectItem=function(oItem){this._bItemSelected=true;this._updateValue(oItem);this._cancelIntervalDetection(this);this.itemSelectEvent.fire(this,oItem,oItem._oResultData);this._toggleContainer(false);};YAHOO.widget.AutoComplete.prototype._jumpSelection=function(){if(!this.typeAhead){return;}
else{this._toggleContainer(false);}};YAHOO.widget.AutoComplete.prototype._moveSelection=function(nKeyCode){if(this._bContainerOpen){var oCurItem=this._oCurItem;var nCurItemIndex=-1;if(oCurItem){nCurItemIndex=oCurItem._nItemIndex;}
var nNewItemIndex=(nKeyCode==40)?(nCurItemIndex+1):(nCurItemIndex-1);if(nNewItemIndex<-2||nNewItemIndex>=this._nDisplayedItems){return;}
if(oCurItem){this._toggleHighlight(oCurItem,"from");this.itemArrowFromEvent.fire(this,oCurItem);}
if(nNewItemIndex==-1){if(this.delimChar&&this._sSavedQuery){if(!this._textMatchesOption()){this._oTextbox.value=this._sSavedQuery;}
else{this._oTextbox.value=this._sSavedQuery+this._sCurQuery;}}
else{this._oTextbox.value=this._sCurQuery;}
this._oCurItem=null;return;}
if(nNewItemIndex==-2){this._toggleContainer(false);return;}
var oNewItem=this._aListItems[nNewItemIndex];var oContent=this._oContainer._oContent;var scrollOn=((YAHOO.util.Dom.getStyle(oContent,"overflow")=="auto")||(YAHOO.util.Dom.getStyle(oContent,"overflowY")=="auto"));if(scrollOn&&(nNewItemIndex>-1)&&(nNewItemIndex<this._nDisplayedItems)){if(nKeyCode==40){if((oNewItem.offsetTop+oNewItem.offsetHeight)>(oContent.scrollTop+oContent.offsetHeight)){oContent.scrollTop=(oNewItem.offsetTop+oNewItem.offsetHeight)-oContent.offsetHeight;}
else if((oNewItem.offsetTop+oNewItem.offsetHeight)<oContent.scrollTop){oContent.scrollTop=oNewItem.offsetTop;}}
else{if(oNewItem.offsetTop<oContent.scrollTop){this._oContainer._oContent.scrollTop=oNewItem.offsetTop;}
else if(oNewItem.offsetTop>(oContent.scrollTop+oContent.offsetHeight)){this._oContainer._oContent.scrollTop=(oNewItem.offsetTop+oNewItem.offsetHeight)-oContent.offsetHeight;}}}
this._toggleHighlight(oNewItem,"to");this.itemArrowToEvent.fire(this,oNewItem);if(this.typeAhead){this._updateValue(oNewItem);}}};YAHOO.widget.AutoComplete.prototype._onItemMouseover=function(v,oSelf){if(oSelf.prehighlightClassName){oSelf._togglePrehighlight(this,"mouseover");}
else{oSelf._toggleHighlight(this,"to");}
oSelf.itemMouseOverEvent.fire(oSelf,this);};YAHOO.widget.AutoComplete.prototype._onItemMouseout=function(v,oSelf){if(oSelf.prehighlightClassName){oSelf._togglePrehighlight(this,"mouseout");}
else{oSelf._toggleHighlight(this,"from");}
oSelf.itemMouseOutEvent.fire(oSelf,this);};YAHOO.widget.AutoComplete.prototype._onItemMouseclick=function(v,oSelf){oSelf._toggleHighlight(this,"to");oSelf._selectItem(this);};YAHOO.widget.AutoComplete.prototype._onContainerMouseover=function(v,oSelf){oSelf._bOverContainer=true;};YAHOO.widget.AutoComplete.prototype._onContainerMouseout=function(v,oSelf){oSelf._bOverContainer=false;if(oSelf._oCurItem){oSelf._toggleHighlight(oSelf._oCurItem,"to");}};YAHOO.widget.AutoComplete.prototype._onContainerScroll=function(v,oSelf){oSelf._oTextbox.focus();};YAHOO.widget.AutoComplete.prototype._onContainerResize=function(v,oSelf){oSelf._toggleContainerHelpers(oSelf._bContainerOpen);};YAHOO.widget.AutoComplete.prototype._onTextboxKeyDown=function(v,oSelf){var nKeyCode=v.keyCode;switch(nKeyCode){case 9:if(oSelf.delimChar&&(oSelf._nKeyCode!=nKeyCode)){if(oSelf._bContainerOpen){YAHOO.util.Event.stopEvent(v);}}
if(oSelf._oCurItem){oSelf._selectItem(oSelf._oCurItem);}
else{oSelf._toggleContainer(false);}
break;case 13:if(oSelf._nKeyCode!=nKeyCode){if(oSelf._bContainerOpen){YAHOO.util.Event.stopEvent(v);}}
if(oSelf._oCurItem){oSelf._selectItem(oSelf._oCurItem);}
else{oSelf._toggleContainer(false);}
break;case 27:oSelf._toggleContainer(false);return;case 39:oSelf._jumpSelection();break;case 38:YAHOO.util.Event.stopEvent(v);oSelf._moveSelection(nKeyCode);break;case 40:YAHOO.util.Event.stopEvent(v);oSelf._moveSelection(nKeyCode);break;default:break;}};YAHOO.widget.AutoComplete.prototype._onTextboxKeyPress=function(v,oSelf){var nKeyCode=v.keyCode;var isMac=(navigator.userAgent.toLowerCase().indexOf("mac")!=-1);if(isMac){switch(nKeyCode){case 9:if(oSelf.delimChar&&(oSelf._nKeyCode!=nKeyCode)){if(oSelf._bContainerOpen){YAHOO.util.Event.stopEvent(v);}}
break;case 13:if(oSelf._nKeyCode!=nKeyCode){if(oSelf._bContainerOpen){YAHOO.util.Event.stopEvent(v);}}
break;case 38:case 40:YAHOO.util.Event.stopEvent(v);break;default:break;}}
else if(nKeyCode==229){oSelf._queryInterval=setInterval(function(){oSelf._onIMEDetected(oSelf);},500);}};YAHOO.widget.AutoComplete.prototype._onTextboxKeyUp=function(v,oSelf){oSelf._initProps();var nKeyCode=v.keyCode;oSelf._nKeyCode=nKeyCode;var sText=this.value;if(oSelf._isIgnoreKey(nKeyCode)||(sText.toLowerCase()==oSelf._sCurQuery)){return;}
else{oSelf.textboxKeyEvent.fire(oSelf,nKeyCode);}
if(oSelf.queryDelay>0){var nDelayID=setTimeout(function(){oSelf._sendQuery(sText);},(oSelf.queryDelay*1000));if(oSelf._nDelayID!=-1){clearTimeout(oSelf._nDelayID);}
oSelf._nDelayID=nDelayID;}
else{oSelf._sendQuery(sText);}};YAHOO.widget.AutoComplete.prototype._onTextboxFocus=function(v,oSelf){oSelf._oTextbox.setAttribute("autocomplete","off");oSelf._bFocused=true;oSelf.textboxFocusEvent.fire(oSelf);};YAHOO.widget.AutoComplete.prototype._onTextboxBlur=function(v,oSelf){if(!oSelf._bOverContainer||(oSelf._nKeyCode==9)){if(!oSelf._bItemSelected){if(!oSelf._bContainerOpen||(oSelf._bContainerOpen&&!oSelf._textMatchesOption())){if(oSelf.forceSelection){oSelf._clearSelection();}
else{oSelf.unmatchedItemSelectEvent.fire(oSelf,oSelf._sCurQuery);}}}
if(oSelf._bContainerOpen){oSelf._toggleContainer(false);}
oSelf._cancelIntervalDetection(oSelf);oSelf._bFocused=false;oSelf.textboxBlurEvent.fire(oSelf);}};YAHOO.widget.AutoComplete.prototype._onFormSubmit=function(v,oSelf){if(oSelf.allowBrowserAutocomplete){oSelf._oTextbox.setAttribute("autocomplete","on");}
else{oSelf._oTextbox.setAttribute("autocomplete","off");}};YAHOO.widget.DataSource=function(){};YAHOO.widget.DataSource.ERROR_DATANULL="Response data was null";YAHOO.widget.DataSource.ERROR_DATAPARSE="Response data could not be parsed";YAHOO.widget.DataSource.prototype.maxCacheEntries=15;YAHOO.widget.DataSource.prototype.queryMatchContains=false;YAHOO.widget.DataSource.prototype.queryMatchSubset=false;YAHOO.widget.DataSource.prototype.queryMatchCase=false;YAHOO.widget.DataSource.prototype.toString=function(){return"DataSource "+this._sName;};YAHOO.widget.DataSource.prototype.getResults=function(oCallbackFn,sQuery,oParent){var aResults=this._doQueryCache(oCallbackFn,sQuery,oParent);if(aResults.length===0){this.queryEvent.fire(this,oParent,sQuery);this.doQuery(oCallbackFn,sQuery,oParent);}};YAHOO.widget.DataSource.prototype.doQuery=function(oCallbackFn,sQuery,oParent){};YAHOO.widget.DataSource.prototype.flushCache=function(){if(this._aCache){this._aCache=[];}
if(this._aCacheHelper){this._aCacheHelper=[];}
this.cacheFlushEvent.fire(this);};YAHOO.widget.DataSource.prototype.queryEvent=null;YAHOO.widget.DataSource.prototype.cacheQueryEvent=null;YAHOO.widget.DataSource.prototype.getResultsEvent=null;YAHOO.widget.DataSource.prototype.getCachedResultsEvent=null;YAHOO.widget.DataSource.prototype.dataErrorEvent=null;YAHOO.widget.DataSource.prototype.cacheFlushEvent=null;YAHOO.widget.DataSource._nIndex=0;YAHOO.widget.DataSource.prototype._sName=null;YAHOO.widget.DataSource.prototype._aCache=null;YAHOO.widget.DataSource.prototype._init=function(){var maxCacheEntries=this.maxCacheEntries;if(isNaN(maxCacheEntries)||(maxCacheEntries<0)){maxCacheEntries=0;}
if(maxCacheEntries>0&&!this._aCache){this._aCache=[];}
this._sName="instance"+YAHOO.widget.DataSource._nIndex;YAHOO.widget.DataSource._nIndex++;this.queryEvent=new YAHOO.util.CustomEvent("query",this);this.cacheQueryEvent=new YAHOO.util.CustomEvent("cacheQuery",this);this.getResultsEvent=new YAHOO.util.CustomEvent("getResults",this);this.getCachedResultsEvent=new YAHOO.util.CustomEvent("getCachedResults",this);this.dataErrorEvent=new YAHOO.util.CustomEvent("dataError",this);this.cacheFlushEvent=new YAHOO.util.CustomEvent("cacheFlush",this);};YAHOO.widget.DataSource.prototype._addCacheElem=function(oResult){var aCache=this._aCache;if(!aCache||!oResult||!oResult.query||!oResult.results){return;}
if(aCache.length>=this.maxCacheEntries){aCache.shift();}
aCache.push(oResult);};YAHOO.widget.DataSource.prototype._doQueryCache=function(oCallbackFn,sQuery,oParent){var aResults=[];var bMatchFound=false;var aCache=this._aCache;var nCacheLength=(aCache)?aCache.length:0;var bMatchContains=this.queryMatchContains;if((this.maxCacheEntries>0)&&aCache&&(nCacheLength>0)){this.cacheQueryEvent.fire(this,oParent,sQuery);if(!this.queryMatchCase){var sOrigQuery=sQuery;sQuery=sQuery.toLowerCase();}
for(var i=nCacheLength-1;i>=0;i--){var resultObj=aCache[i];var aAllResultItems=resultObj.results;var matchKey=(!this.queryMatchCase)?encodeURIComponent(resultObj.query).toLowerCase():encodeURIComponent(resultObj.query);if(matchKey==sQuery){bMatchFound=true;aResults=aAllResultItems;if(i!=nCacheLength-1){aCache.splice(i,1);this._addCacheElem(resultObj);}
break;}
else if(this.queryMatchSubset){for(var j=sQuery.length-1;j>=0;j--){var subQuery=sQuery.substr(0,j);if(matchKey==subQuery){bMatchFound=true;for(var k=aAllResultItems.length-1;k>=0;k--){var aRecord=aAllResultItems[k];var sKeyIndex=(this.queryMatchCase)?encodeURIComponent(aRecord[0]).indexOf(sQuery):encodeURIComponent(aRecord[0]).toLowerCase().indexOf(sQuery);if((!bMatchContains&&(sKeyIndex===0))||(bMatchContains&&(sKeyIndex>-1))){aResults.unshift(aRecord);}}
resultObj={};resultObj.query=sQuery;resultObj.results=aResults;this._addCacheElem(resultObj);break;}}
if(bMatchFound){break;}}}
if(bMatchFound){this.getCachedResultsEvent.fire(this,oParent,sOrigQuery,aResults);oCallbackFn(sOrigQuery,aResults,oParent);}}
return aResults;};YAHOO.widget.DS_XHR=function(sScriptURI,aSchema,oConfigs){if(typeof oConfigs=="object"){for(var sConfig in oConfigs){this[sConfig]=oConfigs[sConfig];}}
if(!aSchema||(aSchema.constructor!=Array)){return;}
else{this.schema=aSchema;}
this.scriptURI=sScriptURI;this._init();};YAHOO.widget.DS_XHR.prototype=new YAHOO.widget.DataSource();YAHOO.widget.DS_XHR.TYPE_JSON=0;YAHOO.widget.DS_XHR.TYPE_XML=1;YAHOO.widget.DS_XHR.TYPE_FLAT=2;YAHOO.widget.DS_XHR.ERROR_DATAXHR="XHR response failed";YAHOO.widget.DS_XHR.prototype.connMgr=YAHOO.util.Connect;YAHOO.widget.DS_XHR.prototype.connTimeout=0;YAHOO.widget.DS_XHR.prototype.scriptURI=null;YAHOO.widget.DS_XHR.prototype.scriptQueryParam="query";YAHOO.widget.DS_XHR.prototype.scriptQueryAppend="";YAHOO.widget.DS_XHR.prototype.responseType=YAHOO.widget.DS_XHR.TYPE_JSON;YAHOO.widget.DS_XHR.prototype.responseStripAfter="\n<!-";YAHOO.widget.DS_XHR.prototype.doQuery=function(oCallbackFn,sQuery,oParent){var isXML=(this.responseType==YAHOO.widget.DS_XHR.TYPE_XML);var sUri=this.scriptURI+"?"+this.scriptQueryParam+"="+sQuery;if(this.scriptQueryAppend.length>0){sUri+="&"+this.scriptQueryAppend;}
var oResponse=null;var oSelf=this;var responseSuccess=function(oResp){if(!oSelf._oConn||(oResp.tId!=oSelf._oConn.tId)){oSelf.dataErrorEvent.fire(oSelf,oParent,sQuery,YAHOO.widget.DataSource.ERROR_DATANULL);return;}
for(var foo in oResp){}
if(!isXML){oResp=oResp.responseText;}
else{oResp=oResp.responseXML;}
if(oResp===null){oSelf.dataErrorEvent.fire(oSelf,oParent,sQuery,YAHOO.widget.DataSource.ERROR_DATANULL);return;}
var aResults=oSelf.parseResponse(sQuery,oResp,oParent);var resultObj={};resultObj.query=decodeURIComponent(sQuery);resultObj.results=aResults;if(aResults===null){oSelf.dataErrorEvent.fire(oSelf,oParent,sQuery,YAHOO.widget.DataSource.ERROR_DATAPARSE);aResults=[];}
else{oSelf.getResultsEvent.fire(oSelf,oParent,sQuery,aResults);oSelf._addCacheElem(resultObj);}
oCallbackFn(sQuery,aResults,oParent);};var responseFailure=function(oResp){oSelf.dataErrorEvent.fire(oSelf,oParent,sQuery,YAHOO.widget.DS_XHR.ERROR_DATAXHR);return;};var oCallback={success:responseSuccess,failure:responseFailure};if(!isNaN(this.connTimeout)&&this.connTimeout>0){oCallback.timeout=this.connTimeout;}
if(this._oConn){this.connMgr.abort(this._oConn);}
oSelf._oConn=this.connMgr.asyncRequest("GET",sUri,oCallback,null);};YAHOO.widget.DS_XHR.prototype.parseResponse=function(sQuery,oResponse,oParent){var aSchema=this.schema;var aResults=[];var bError=false;var nEnd=((this.responseStripAfter!=="")&&(oResponse.indexOf))?oResponse.indexOf(this.responseStripAfter):-1;if(nEnd!=-1){oResponse=oResponse.substring(0,nEnd);}
switch(this.responseType){case YAHOO.widget.DS_XHR.TYPE_JSON:var jsonList;if(window.JSON&&(navigator.userAgent.toLowerCase().indexOf('khtml')==-1)){var jsonObjParsed=JSON.parse(oResponse);if(!jsonObjParsed){bError=true;break;}
else{try{jsonList=eval("jsonObjParsed."+aSchema[0]);}
catch(e){bError=true;break;}}}
else{try{while(oResponse.substring(0,1)==" "){oResponse=oResponse.substring(1,oResponse.length);}
if(oResponse.indexOf("{")<0){bError=true;break;}
if(oResponse.indexOf("{}")===0){break;}
var jsonObjRaw=eval("("+oResponse+")");if(!jsonObjRaw){bError=true;break;}
jsonList=eval("(jsonObjRaw."+aSchema[0]+")");}
catch(e){bError=true;break;}}
if(!jsonList){bError=true;break;}
if(jsonList.constructor!=Array){jsonList=[jsonList];}
for(var i=jsonList.length-1;i>=0;i--){var aResultItem=[];var jsonResult=jsonList[i];for(var j=aSchema.length-1;j>=1;j--){var dataFieldValue=jsonResult[aSchema[j]];if(!dataFieldValue){dataFieldValue="";}
aResultItem.unshift(dataFieldValue);}
if(aResultItem.length==1){aResultItem.push(jsonResult);}
aResults.unshift(aResultItem);}
break;case YAHOO.widget.DS_XHR.TYPE_XML:var xmlList=oResponse.getElementsByTagName(aSchema[0]);if(!xmlList){bError=true;break;}
for(var k=xmlList.length-1;k>=0;k--){var result=xmlList.item(k);var aFieldSet=[];for(var m=aSchema.length-1;m>=1;m--){var sValue=null;var xmlAttr=result.attributes.getNamedItem(aSchema[m]);if(xmlAttr){sValue=xmlAttr.value;}
else{var xmlNode=result.getElementsByTagName(aSchema[m]);if(xmlNode&&xmlNode.item(0)&&xmlNode.item(0).firstChild){sValue=xmlNode.item(0).firstChild.nodeValue;}
else{sValue="";}}
aFieldSet.unshift(sValue);}
aResults.unshift(aFieldSet);}
break;case YAHOO.widget.DS_XHR.TYPE_FLAT:if(oResponse.length>0){var newLength=oResponse.length-aSchema[0].length;if(oResponse.substr(newLength)==aSchema[0]){oResponse=oResponse.substr(0,newLength);}
var aRecords=oResponse.split(aSchema[0]);for(var n=aRecords.length-1;n>=0;n--){aResults[n]=aRecords[n].split(aSchema[1]);}}
break;default:break;}
sQuery=null;oResponse=null;oParent=null;if(bError){return null;}
else{return aResults;}};YAHOO.widget.DS_XHR.prototype._oConn=null;YAHOO.widget.DS_JSFunction=function(oFunction,oConfigs){if(typeof oConfigs=="object"){for(var sConfig in oConfigs){this[sConfig]=oConfigs[sConfig];}}
if(!oFunction||(oFunction.constructor!=Function)){return;}
else{this.dataFunction=oFunction;this._init();}};YAHOO.widget.DS_JSFunction.prototype=new YAHOO.widget.DataSource();YAHOO.widget.DS_JSFunction.prototype.dataFunction=null;YAHOO.widget.DS_JSFunction.prototype.doQuery=function(oCallbackFn,sQuery,oParent){var oFunction=this.dataFunction;var aResults=[];aResults=oFunction(sQuery);if(aResults===null){this.dataErrorEvent.fire(this,oParent,sQuery,YAHOO.widget.DataSource.ERROR_DATANULL);return;}
var resultObj={};resultObj.query=decodeURIComponent(sQuery);resultObj.results=aResults;this._addCacheElem(resultObj);this.getResultsEvent.fire(this,oParent,sQuery,aResults);oCallbackFn(sQuery,aResults,oParent);return;};YAHOO.widget.DS_JSArray=function(aData,oConfigs){if(typeof oConfigs=="object"){for(var sConfig in oConfigs){this[sConfig]=oConfigs[sConfig];}}
if(!aData||(aData.constructor!=Array)){return;}
else{this.data=aData;this._init();}};YAHOO.widget.DS_JSArray.prototype=new YAHOO.widget.DataSource();YAHOO.widget.DS_JSArray.prototype.data=null;YAHOO.widget.DS_JSArray.prototype.doQuery=function(oCallbackFn,sQuery,oParent){var aData=this.data;var aResults=[];var bMatchFound=false;var bMatchContains=this.queryMatchContains;if(sQuery){if(!this.queryMatchCase){sQuery=sQuery.toLowerCase();}
for(var i=aData.length-1;i>=0;i--){var aDataset=[];if(aData[i]){if(aData[i].constructor==String){aDataset[0]=aData[i];}
else if(aData[i].constructor==Array){aDataset=aData[i];}}
if(aDataset[0]&&(aDataset[0].constructor==String)){var sKeyIndex=(this.queryMatchCase)?encodeURIComponent(aDataset[0]).indexOf(sQuery):encodeURIComponent(aDataset[0]).toLowerCase().indexOf(sQuery);if((!bMatchContains&&(sKeyIndex===0))||(bMatchContains&&(sKeyIndex>-1))){aResults.unshift(aDataset);}}}}
this.getResultsEvent.fire(this,oParent,sQuery,aResults);oCallbackFn(sQuery,aResults,oParent);};
/**
 * Fonction permettant de connaitre si une variable est nulle
 *
 * @param mixed variable variable Ã  tester
 * @return boulean
 */
  
/**
 * Function isNull to verify if the value is null or is a empty String
 * @params value    a Object (Number, String, ...), value to compare
 * @return          a Boolean, <code>true</code> if is null, <code>false</code>
 * @need            #getAndCall()
 */
function isNull(value)
{
  return value === null || value === '';
}

/**
 * Pointer to HTML DOMObject like document.body
 */
document.html = document.documentElement;
  
/**
 * FuncLink is a Class to return Function for dont loose referencial of Object|Class's method.
 * Work too for anonymous and named Function.
 *
 * @param method  a (anonymous|named)Function or a String, a function or name of function or method
 * @param obj     a Object, if method is a method (Optional)
 * @return        the Function;
 */
var FuncLink = function(method, obj)
{
  obj = obj || null;
  if(typeof obj === 'object')//if using "obj.method"
  {
    //Use closure for using indirect call of obj's method,
    //to keep the referencial 'this'
    return function()
    {
      for(var i = 0, l = arguments.length, args = new Array(); i < l; i++)
      {
        args.push('arguments[' + i + ']');
      }
      eval('obj[method](' + args.join(',') + ')');
    };
  }
  else if(typeof method === 'function')//if is only a function (not a method)
  {
    return method;
  }
  else if(typeof method === 'string' && obj === null)//if the function is passed in String
  {
    return eval(method);
  }
  return function(){};
};
/**
 * forEach is a method of Array  to execute for each element of an Array
 *
 * @param func       a Function, accepte one argument, the array element
 * @return           Void;
 */
Array.prototype.forEach = function(func)
{
  for(var i = 0, l = this.length; i < l; i ++)
  {
    func(this[i]);
  }
};

/**
 * indexOf is a method of Array  to get the index of first element in the Array
 *
 * @param value       a Object (String, Number, ...), element to search index
 * @param begin       a Number, index where begin of search
 * @param strict      a Boolean, if search is strict (Optional)
 * @return            a Number, index of the first element that matches value if exist else -1
 */
Array.prototype.indexOf = function(value, begin, strict)
{
  for(var i = +begin || 0; i < this.length; i++)
  {
    if(this[i] === value || strict && this[i] == value)
    {
      return i;
    }
  }
  return -1;
};

/**
 * insert is a method of Array to insert value at index, without overwriting existing keys
 *
 * @param index       a Number, index where insert element
 * @param value       a Object (String, Number, ...), element to insert
 * @return            a Array, the array with the element inserted;
 */
Array.prototype.insert = function(index, value)
{
  if(index >= 0)
  {
    var a = this.slice(), b = a.splice(index);
    a[index] = value;
    return a.concat(b);
  }
};

/**
 * insert is a method of Array to get index of the last element that matches value
 *
 * @param value       a Object (String, Number, ...), element to search index
 * @param begin       a Number, index where begin of search
 * @param strict      a Boolean, if search is strict (Optional)
 * @return            a Array, the array with the element inserted;
 */
Array.prototype.lastIndexOf = function(value, begin, strict)
{
  begin = +begin || 0;
  var i = this.length;
  while(i-- > begin)
  {
    if(this[i] === value || strict && this[i] == value )
    {
      return i;
    }
  }
  return -1;
};

/**
 * remove is a method of Array to remove each elements index
 *
 * @param index     a Number, index of element to remove
 * @return          Void
 */
//TODO: support a Array argument
Array.prototype.remove = function()
{
  for(var i = 0; i < arguments.length; i++)
    this.splice(arguments[i] ,1);
};

/**
 * clear is a method of Array to remove all element of the Array
 *
 * @return   Void
 */
Array.prototype.clear = function()
{
  this.splice(0, this.length);
};

/**
 * trim is a Method of String to remove all before and after whitespace
 * @return     a String, result of fitering before and after whitespace
 */
String.prototype.trim = function()
{
	return this.replace(/^\s*|\s*$/g, '');
};
var URL = function(href)
{
  var url = href.split('?');
  this.href = url[0];
  this.datas = arguments[1] || url[1] || '';
};
URL.prototype.toString = function()
{
  var tmp = this.href;
  if(!isNull(this.datas))
    tmp += '?' + this.datas;
  return tmp;
};
URL.getHash = function()
{
  var hash = document.location.hash
  return hash.substring(1, hash.length);
};
/**
 * Here found compatibility with differents methods, classes, functions
 * for respect standarts of ECMAscript and Document Model Object (if is possible)
 * Many is for IE
 */

//TODO: make compatibility to load XML document


// browser detection
var ua = navigator.userAgent.toLowerCase(),
  isOpera = (ua.indexOf('opera') > -1),
  isSafari = (ua.indexOf('safari') > -1),
	isGecko = (!isOpera && !isSafari && ua.indexOf('gecko') > -1),
	isIE = (!isOpera && ua.indexOf('msie') > -1),
	isIE6 = (!isOpera && ua.indexOf('msie 6.0') > -1),
	isIE7 = (!isOpera && ua.indexOf('msie 7.0') > -1);

/**
 * Method addEventListener if not already exist. Add callback on event
 * @param String name of event
 * @param Function callback function called when event is fired
 * @param Boolean if the event capture is bubbled (here not work)
 */
 function addEventListener(el, eventName, callback, captureBubble)
 {
 	switch(eventName){
		case 'DOMContentLoaded':
			if(el.addEventListener){
				YAHOO.util.Event.addListener(el, eventName, callback);
			}else{//else is IE
				window.attachEvent('onload', callback);
			}
			break;
		default:
		 	YAHOO.util.Event.addListener(el, eventName, callback);
			break;
	}
/*    if(el.addEventListener)
*    {
*      el.addEventListener(eventName, callback, captureBubble);
*    }
*    else
*    //else is IE
*    {
*      switch(eventName)
*      {
*        //only on document.addEventListener()
*        case 'DOMContentLoaded' :
*    		  window.attachEvent('onload', callback);
*          break;
*        default:
*          el.attachEvent('on' + eventName, (function(){
*              return function()
*              {
*                callback.apply(el, new Array(event));
*              };
*          })());
*					break;
*      }
*    }
*/
	}


/**
 * Method removeEventListener if not already exist. Remove callback on event
 * @param String name of event
 * @param Function callback function called when event is fired
 * @param Boolean if the event capture is bubbled (here not work)
 */
  function removeEventListener(el, eventName, callback, captureBubble)
  {
 		YAHOO.util.Event.removeListener(el, eventName, callback);
/*    if(el.removeEventListener)
    {
      el.removeEventListener(eventName, callback, captureBubble);
    }
    else
    //else is IE
    {
      switch(eventName)
      {
        case 'DOMContentLoaded' :        
          break;
        default:
          el.detachEvent('on' + eventName, callback);
      }
    }
*/
  }

/**
 * Class XMLHttpRequest if not already exist. For making HTTPRequests
 * @note is impossible to add any properties or method to this object (or in general to ActiveXObject Class)
 */
if(!XMLHttpRequest)
{
  var XMLHttpRequest = function()
  {
    try
    {
      return new ActiveXObject('Msxml2.XMLHTTP');
    }
    catch(e)
    {
      try
      {
         return new ActiveXObject('Microsoft.XMLHTTP');
      }
      catch(E)
      {
        
      }
    }
    return null;
  }
}

	/**
 		* Method getComputedStyle if not already exist in window object
 		* @param Object element to get it style's object
 		* @param String CSS pseudoClass (:hover, :first-child,...) but here can't be use (IE alternate method hasen't got equivalent)
 		* @return CSSStyleDeclaration
 		*/
	if(!window.getComputedStyle)
	{
 		window.getComputedStyle = function(element, pseudoClass)
  	{
    	if(element.currentStyle)
      	return element.currentStyle;
  	};
	}

	/**
		* swapSelect() allows to hide SELECT elements when an element is over (IE's bug)
		*
		* @param Object el element to display over select elements
		* 
		* @return void
		*/
	function swapSelects(el){ 
		var el_x  = getX(el);
		var el_y  = getY(el); 
		var el_width  = el.offsetWidth;
		var el_height  = el.offsetHeight;
	
		var arSelects = document.getElementsByTagName('SELECT');
		var sel_length = arSelects.length;
		
		for (var i = 0; i < sel_length; i++) { 
			var select = arSelects[i];
			var select_x = getX(select);
			var select_y = getY(select); 
			var select_width = select.offsetWidth;
			var select_height = select.offsetHeight;
			
			var crossWidth = false; 
			if( el_x > (select_x - el_width) && el_x < (select_x + select_width) ){ 
				crossWidth = true; 
			}
			
			var crossHeight = false; 
			if ( el_y > (select_y - el_height) && el_y < (select_y + select_height) ){ 
				crossHeight = true; 
			}
			
			if (crossWidth && crossHeight) { 
				var vis = (el.style.visibility != 'hidden' && el.style.display != 'none') ? 'hidden' : 'visible'; 
				if (select.style.visibility != vis) {
					select.style.visibility = vis;
				}
			} else { 
				if (select.style.visibility != 'visible') {
					select.style.visibility = 'visible';
				} 
			} 
		} 
	}

/**
 * getElements|gE is a Function to get element
 *
 * @param       a DOMObject or String, (Optional)
 * @return      a Array with all DOMObject targeted
 */
var gE =
document.getElements = function()
{
  var elements = new Array();

  for (var i = 0; i < arguments.length; i++)
  {
    var element = arguments[i];
    if (typeof element === 'string')
      element = document.getElementById(element) || null;
    if (arguments.length == 1)
    {
      if(element === null)
        return undefined;
      return element;
    }
    if(element != null)
      elements.push(element);
  }
  return elements;
};

/**
 * remove is a Method of HTMLElement to remove element
 *
 * @param       a DOMObject or String, (Optional)
 * @return      a DOMObject, return element removed
 */
function remove(el)
{
  return el.parentNode.removeChild(el);
};

/**
 * getNodeIndex is a Method of HTMLElement to remove element
 *
 * @return      a Number, return index of parent's childNodes
 */
function getNodeIndex(el)
{
   var tmp = new Number();
   for (var i = 0; i < el.parentNode.childNodes.length; i++)
   {
     if(el.parentNode.childNodes[i] == el)
       tmp = i;
   }
   return tmp
};

/**
 * accessToString is a Method of HTMLElement get path to access to the DOMObject
 * example : <code>document.childNodes[1].childNodes[0].childNodes[0]</code>
 *
 * @return      a String, return the path
 */
function accessToString(el)
{
  var tmp = new String();
  var tmpObj = el;
  while(tmpObj.parentNode != null && typeof getNodeIndex(tmpObj) != 'undefined')
  {
    tmp = '.childNodes[' + getNodeIndex(tmpObj) + ']' + tmp;
    tmpObj = tmpObj.parentNode;
  }
  return 'document' + tmp;
};

/* TODO: clear content element
HTMLElement.prototype.clear = function()
{
  
}
*/

/**
 * addClassName is a Method of HTMLElement to add a class name
 *
 * @param classNames  a String or Array
 * @return            Void
 */
function addClassName(el, classNames)
{
  var curClassName = el.className.split(' ');
  if(typeof classNames === 'string')
    classNames = classNames.split(' ');
  //TODO: Array.forEach
  for(var i = 0, l = classNames.length, index; i < l; i++)
  {
    index = curClassName.indexOf(classNames[i]);
    if(index == -1 && !isNull(classNames[i]))
      curClassName.push(classNames[i]);
  }
  el.className = curClassName.join(' ');
};

/**
 * removeClassName is a Method of HTMLElement to remove a class name
 *
 * @param classNames  a String or Array
 * @return            Void
 */
function removeClassName(el, classNames)
{
  var curClassName = el.className.split(' ');
  if(typeof classNames === 'string')
    classNames = classNames.split(' ');
  //TODO: Array.forEach
  for(var i = 0, l = classNames.length, index; i < l; i++)
  {
    index = curClassName.indexOf(classNames[i]);
    if(index != -1)
      curClassName.remove(index);
  }
  el.className = curClassName.join(' ');
};

/*
 * isClassName checks if some classes are associated to an element
 *
 * @param  HTMLElement   el  the element to check
 * @param  String|Array  classNames  the classes to check for
 *
 * @return boolean
 */
function isClassName(el, classNames){
	var curClassName = el.className.split(' ');
	if(typeof classNames === 'string'){
	  classNames = classNames.split(' ');
	}

	for(var i = 0, l = classNames.length, index; i < l; i++){
		index = curClassName.indexOf(classNames[i]);
		if(index == -1){
		  return false;
		}
	}
	
	return true;
			
};


/*
 * replaceClassName replaces a class associated to an element (if existing) by another
 *
 * @param  HTMLElement   el  the element to check
 * @param  String  class1  the class to replace
 * @param  String  class2  the new class
 *
 * @return boolean
 */
function replaceClassName(el, class1, class2){
	var isClass = isClassName(el, class1);
	
	if(isClass){
		removeClassName(el, class1);
		addClassName(el, class2);
	}
	
	return isClass;
}


/*
 * switchClassName switches a class associated to an element between two possible values
 *
 * @param  HTMLElement   el  the element to check
 * @param  String  class1
 * @param  String  class2
 *
 * @return new class name
 */
function switchClassName(el, class1, class2){
	var newClass;
	
	if(isClassName(el, class1)){
		removeClassName(el, class1);
    addClassName(el, class2);
		newClass = class2;
	}else if(isClassName(el, class2)){
		removeClassName(el, class2);
    addClassName(el, class1);
		newClass = class1;
	}else if(class2 == ''){
		addClassName(el, class1);
		newClass = class1;
	}else{
		newClass = false;
	}

	return newClass;
}

//TODO: with CSS property : visiblity


/*
 * getX returns element X position (takes account of scrollbars)
 */
function getX(element){
	var x = 0;
	for(var e = element; e; e = e.offsetParent){
		x += e.offsetLeft;
	}

	for(e = element.parentNode; e && e != document.body; e = e.parentNode){
		if(e.scrollLeft){
			x -= e.scrollLeft;
		}
	}

	return x;
}

/*
 * getY returns element Y position (takes account of scrollbars)
 */
function getY(element){
	var y = 0;
	for(var e = element; e; e = e.offsetParent){
		y += e.offsetTop;
	}

	for(e = element.parentNode; e && e != document.body; e = e.parentNode){
		if(e.scrollTop){
			y -= e.scrollTop;
		}
	}

	return y;
}

function getViewportWidth(){
	return document.documentElement.clientWidth;
	//return document.documentElement.clientWidth || document.body.clientWidth || self.innerWidth;
}

function getViewportHeight(){
	return document.documentElement.clientHeight;
	//return document.documentElement.clientHeight || document.body.clientHeight || self.innerHeight;
}


/**
 * display is a Method of HTMLElement to hide|show element
 *
 * @param params       a Object, content optional parameters
 * @return             a Boolean, <code>true</code> if element is visible else <code>false</code>
 * @need               HTMLElement#show()
 * @need               HTMLElement#hide()
 */
function display(el,params)
{
  params = params || new Object();
  var displayMode = params.displayMode || '',
  onshow = params.onshow || function(){},
  onhide = params.onhide || function(){};
  if(window.getComputedStyle(el, '').display == 'none')
  {
    show(el,displayMode);
    onshow();
    return true;
  }
  else
  {
    hide(el);    
    onhide();
  }
  return false;
};

/**
 * show is a Method of HTMLElement to show element
 *
 * @param displayMode  a String, CSS value to force the mode of display (Optional)
 * @return             Void
 */
function show(el, displayMode)
{
  displayMode = displayMode || '';
  el.style.display = displayMode;
};

/**
 * show is a Method of HTMLElement to show element
 *
 * @return             Void
 */
function hide(el)
{
  el.style.display = 'none';
};

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

/*getInputs = function(form, typeName, name)
{
  form = $(form);
  var inputs = form.getElementsByTagName('input');
  
  if (!typeName && !name)
    return inputs;
    
  var matchingInputs = new Array();
  for (var i = 0; i < inputs.length; i++) {
    var input = inputs[i];
    if ((typeName && input.type != typeName) ||
        (name && input.name != name)) 
      continue;
    matchingInputs.push(input);
  }

  return matchingInputs;
};*/

//TODO: check problem with Select in IE


/**
 * toString is method to transform an Object to String. Example : <code>alert(Object);</code>
 * Here the method return datas of all FORM's INPUTs, SELECTs and TEXTAREAs.
 * All datas are seperated by a & sign
 * Datas are writen like this : name of input + equal sign + value or only value if name is undefined.
 *
 * @return    a String, form datas
 * @need      isNull()
 * @need      String#trim()
 */
function formToString(frm)
{
  var elements = frm.elements, values = new Array(), tmp;
  for(var i = 0, length = elements.length, element; i < length; i++)
  {
    tmp = inputToString(elements[i]);
    if(!isNull(tmp.trim()))
      values.push(tmp);      
  }
  return values.join('&');
};


/**
 * toString is method to transform an Object to String. Example : <code>alert(Object);</code>
 * Here the method return datas for all type of INPUTs (text, hidden, password, radio, checkbox, ...) and TEXTAREA
 * Datas are writen like this : name of input + equal sign + value or only value if name is undefined.
 *
 * @return    a String, input,textarea datas
 * @need      isNull()
 * @need      String#trim()
 */
function inputToString(input)
{
  var name = input.name,
  value = input.value,
  tmp = new String();
	tn = input.tagName;
	switch(tn.toLowerCase())
  {
  	case 'input':
			switch(input.type)
  		{
    		case 'text' :
    		case 'hidden' :
    		case 'password' :
    		case 'textarea' :
      		tmp = !isNull(name) && !isNull(value) ? input.name + '=' + input.value : (!isNull(value) ? value : '');
      		break;
    		case 'radio' :
    		case 'checkbox' :
      		if(input.checked)        
        		tmp = !isNull(name) && !isNull(value) ? input.name + '=' + input.value : (!isNull(value) ? value : '');
      		break;
				default:
					//void
					break;
			}
			break;
		case 'select':
  		var name = input.name,
  		value, option,
  		values = new Array(),
  		optionsCollection = input.options;
  		if(!isNull(name))
  		{
    		for(var i = 0, length = optionsCollection.length; i < length; i++)
    		{
      		option = optionsCollection[i];
      		value = option.value;
      		if(option.selected && !isNull(value.trim()))
      		{
        		values.push(name + '=' + value);
      		}
    		}
  		}
  		else
  		{
    		for(var i = 0, length = optionsCollection.length; i < length; i++)
    		{
      		option = optionsCollection[i];
      		value = option.value;
      		if(option.selected && !isNull(value.trim()))
      		{
        		values.push(value);
      		}
    		}
  		}
  		tmp = values.join('&');
			break;
		case 'textarea' :
      tmp = !isNull(name) && !isNull(value) ? input.name + '=' + input.value : (!isNull(value) ? value : '');
			break;
	
  }
  return tmp;
};



/**
 * formMakeHTTPTransaction is a FORM's method to send FORM's values (url, datas, httpMode)
 *
 * @params params  An Object, with each properties are content value : 
 *                 url, datas, httpMode, headers, asyncMode, user, pass, events
 *                 (Optional)
 * @return         Void
 * @need           AJAX#makeHTTPTransaction()
 */
function formMakeHTTPTransaction(frm, params)
{
  params = params || new Object();
  params.url = params.url || frm.action;
  params.datas = params.datas || formToString(frm);
  params.httpMode = params.httpMode || frm.method;
  AJAX.makeHTTPTransaction(params);
};

/**
 * formMakeAndCall is a FORM's method to send FORM's values (url, datas, httpMode)
 * and call function when request is completed and status is in (200 to 299)(onsuccess)
 *
 * @params callback  An Function or FuncLink, a callback function 
 * @return           Void
 * @need             HTMLFormElement#makeHTTPTransaction()
 */
function formMakeAndCall(frm, callback)
{
  formMakeHTTPTransaction(frm, {onsuccess : callback});
};


/**
 * validateInput is a method to verify if the content is valid.
 *
 * @param        a RegExp or String, regular expression or name of predefined regular expression Example : <code>RegExp.EMAIL</code> or <code>'EMAIL'</code>
 * @param  boolean  empty_auth  Can the input value be empty ?
 *
 * @return       <code>true</code> if is validated else <code>false</code>.
 */
function validateInput(input)
{
  var state = false;
	tn = input.tagName;
	switch(tn.toLowerCase())
	{
		case 'input':
  		switch(input.type)
  		{
    		case 'text' :
    		case 'password' :
    		case 'textarea' :
      		if(!input.pattern)
      		{
        		var regexp = typeof arguments[1] === 'string' ? RegExp[arguments[1]] : arguments[1];
						input.pattern = regexp;
      		}
					else{
        		var regexp = input.pattern;
					}

					if(!input.empty_auth){
						var empty_auth = (arguments[2] && typeof arguments[2] === 'boolean') ? arguments[2] : false ;
						input.empty_auth = empty_auth;
					}
					else{
						var empty_auth = input.empty_auth;
					}
					
					if(empty_auth && input.value == ''){
						state = true;
					}
					else if(regexp && regexp.test){
	      		state = regexp.test(input.value.trim());
					}
					else{
						state = false;
					}
					
      		if(typeof input.onkeyup != 'function')
      		{
        		input.onblur = 
        		input.onkeydown = 
        		input.onkeyup = function()
        		{
          		validateInput(this);
        		};
        		input.onfocus = function()
        		{
          		return validateInput(this);
        		};
      		}
      		if(!state)
        		addClassName(input.parentNode,'error');
      		else    
        		removeClassName(input.parentNode,'error');
      		break;
    		case 'checkbox' :
      		state = arguments.length >= 2 ? input.checked : !input.checked;
      		//state = unchecked(false) -> click (callfunction and save state) -> state = checked(true)
      		if(!state)
        		addClassName(input.parentNode, 'error');
      		else    
        		removeClassName(input.parentNode, 'error');
      		break;    
    		case 'radio' :
      		var group = input.form[input.name];
      		for(var i = 0, l = group.length, element; i < l; i++)
      		{
        		element = group[i];
        		if(arguments.length >= 2 || element != input)
          		state = element.checked ? true : state;
        		else if(!element.checked)
          		state = !input.checked ? true : state;
        		else
          		state = element.checked ? true : state;
        		if(typeof element.onmouseup != 'function')
          		element.onmouseup = function(){return validateInput(input);};
      		}
      		//alert(state);
      		if(!state)
        		for(var i = 0, l = group.length; i < l; i++)
          		addClassName(group[i].parentNode,'error');
      		else
        		for(var i = 0, l = group.length; i < l; i++)
          		removeClassName(group[i].parentNode,'error');
      		break;
				default:
					//void
					break;
			}
			break;
		case 'select' :
  		var state = false;
  		if(typeof input.onkeyup != 'function')
  		{
    		input.onmouseup =
    		input.onkeyup = function()
    		{
      		validateInput(this);
    		};
  		}
  		//return state;
  		state = !isNull(inputToString(input));
  		if(!state)
    		addClassName(input.parentNode,'error');
  		else    
    		removeClassName(input.parentNode,'error');  
			break;
		case 'textarea' :
    	if(!input.pattern)
    	{    
     		var regexp = typeof arguments[1] === 'string' ? RegExp[arguments[1]] : arguments[1];
     		input.pattern = regexp;
    	}
    	else{
     		var regexp = input.pattern;
			}
			
			if(!input.empty_auth){
				var empty_auth = (arguments[2] && typeof arguments[2] === 'boolean') ? arguments[2] : false ;
				input.empty_auth = empty_auth;
			}
			else{
				var empty_auth = input.empty_auth;
			}
		
			if(empty_auth && input.value == ''){
				state = true;
			}
			else if(regexp && regexp.test){
	    	state = regexp.test(input.value.trim());
			}
			else{
				state = false;
			}
			
    	if(typeof input.onkeyup != 'function')
    	{
     		input.onblur = 
     		input.onkeydown = 
     		input.onkeyup = function()
     		{
       		validateInput(this);
     		};
     		input.onfocus = function()
     		{
       		return validateInput(this);
     		};
    	}
    	if(!state)
     		addClassName(input.parentNode,'error');
    	else    
     		removeClassName(input.parentNode,'error');
    	break;
		default:
			//void
			break;
  }
  return state;
};

/**
 * validateForm is a method to verify if all FORM's INPUT', SELECT' and TEXTAREA' content is valid
 *
 * @return    <code>true</code> if is valid else <code>false</code>.
 */
function validateForm(frm)
{
  var state = true, elementCollection = frm.elements;
  for(var i = 0, length = elementCollection.length, validation; i < length; i++)
  {
    var elmt = elementCollection[i];
    if(typeof elmt.onfocus == 'function')//Inputs(type text, password) and TextAreas and Selects
    {
      //fireevent
      //elmt.dispatchEvent(document.createEvent('HTMLEvents').initEvent('focus', true, true));
      //state = elmt.validate() ? state : false;//Can't do that because we dont know the pattern gived
      state = elmt.onfocus() ? state : false;
    }
    else if(typeof elmt.onmouseup == 'function')//Inputs(type checkbox and radio)
    {
      state = validateInput(elmt,'flag') ? state : false;
    }
  }
  return state;
};

var calendars = new Object();

/**
 * selectDate() dispalays a calendar to select a date
 *
 * @param  string    container_id  id of the div which will contain the calendar
 * @param  function  callback      function to execute after a date is selected
 * @param  boolean   mobile        does the calendar appear near the mouse ?
 * @param  string    locale        locale (FR|US)
 *
 * @return void
 */
function selectDate(container_id, callback, mobile, locale) 
{
	
	if(calendars['"'+container_id+'"']){
		calendars['"'+container_id+'"'].hide();
		calendars['"'+container_id+'"'] = undefined;
	}

		calendars['"'+container_id+'"'] = new YAHOO.widget.Calendar("cal", container_id, {close: true});
		var cal = calendars['"'+container_id+'"'];
		cal.locale = locale || 'FR';
		cal.container = gE(container_id);
	
		cal.cfg.setProperty("MONTHS_SHORT", ["Jan", "FÃ©v", "Mar", "Avr", "Mai", "Jui", "Jui", "AoÃ»", "Sep", "Oct", "Nov", "DÃ©c"]);
		cal.cfg.setProperty("MONTHS_LONG", ["Janvier", "FÃ©vrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "AoÃ»t", "Septembre", "Octobre", "Novembre", "DÃ©cembre"]);
		cal.cfg.setProperty("WEEKDAYS_1CHAR", ["S", "M", "D", "M", "D", "F", "S"]);
		cal.cfg.setProperty("WEEKDAYS_SHORT", ["Di", "Lu", "Ma", "Me", "Je", "Ve", "Sa"]);
		cal.cfg.setProperty("WEEKDAYS_MEDIUM", ["Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam"]);
		cal.cfg.setProperty("WEEKDAYS_LONG", ["Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"]);
		cal.cfg.setProperty("START_WEEKDAY", 1);

		cal.container.style.display = 'none';
	
	if(mobile){
		cal.container.style.position = 'absolute';
		cal.container.style.left = ((mouse.x >= 100) ? mouse.x - 100 : 0 ) + 'px';
		cal.container.style.top = mouse.y + 'px';
	}
	
	  cal.selectEvent.subscribe(function(){
			cal.hide();
//			removeEventListener(document, 'click', handleClickOutCal(container_id), false);

			callback( cal.toWopeDate(cal.getSelectedDates(), locale) );
 	 });

	

	  cal.render();
	
		calendars['"'+container_id+'"'].show();
		
//		addEventListener(document, 'click', handleClickOutCal(container_id), false);
	
}

function handleClickOutCal(id){
		if(gE(id).style.display != 'none' && (mouse.x < calendars['"'+id+'"'].container.offsetLeft || mouse.x > calendars['"'+id+'"'].container.offsetLeft + calendars['"'+id+'"'].container.offsetWidth || mouse.y < calendars['"'+id+'"'].container.offsetTop || mouse.y > calendars['"'+id+'"'].container.offsetTop + calendars['"'+id+'"'].container.offsetHeight )){
			calendars['"'+id+'"'].hide();
			calendars['"'+id+'"'] = undefined;
		}

}
  
	
	
/**
 * toWopeDate() is a method to create a calendar in your web page  
 *
 * @author : Sylvain@Wope ~ 11/2006
 *
 * @param : date you have selected in your calendar
 * @param : style you want to use to display the selected date
 *
 * @return : the date formatted as you wanted
 */
	 
	 YAHOO.widget.Calendar_Core.prototype.toWopeDate = function(date, locale) 
	 {
		 var returnDate = new Array();
		 var style = style;
     if (date instanceof Date) {
		   returnDate = [[date.getFullYear(), date.getMonth()+1, date.getDate()]];
		 } else if (typeof date == 'string') {
			   returnDate = this._parseDates(date);
			 } else if (date instanceof Array) {
			 	  for (var i=0;i<date.length;++i) {
					  var d = date[i];
					  var calYear, calMonth, calDate = new String();
					  calYear = d.getFullYear();
						calMonth = d.getMonth()+1;
            if (calMonth.toString().length == 1) {
	            calMonth = '0'+calMonth;
	          }
            calDate = d.getDate();
	          if (calDate.toString().length == 1) {
	            calDate = '0'+calDate;
	          }
					}
        }
				
				if (locale == 'US') {
			  	return calYear+'-'+calMonth+'-'+calDate;
				}
				if (locale == 'FR') {
					return calDate+'/'+calMonth+'/'+calYear;
				}
		};



/**
 * Regular expression of email
 * not case sensitive
 * Examples : 
 * mail@domain.ext
 * e-mail@subdomain.domain.net
 * e.mail@subdomain.subdomain.name
 * support but is not valid :
 * .....@test._...ffff
 */
RegExp.EMAIL = /^[a-z0-9._-]+@[a-z0-9._-]{2,}\.[a-z]{2,4}$/i;
/**
 * Gets the regular expression for valid email addresses as per RFC-822 except the part about "including linear-white-space" :
 * EMAIL /^([^\[\]()<>@,;:\\".]+|\"([^"\\\r]|\\.)*\")(\.([^\[\]()<>@,;:\\".]+|\"([^"\\\r]|\\.)*\"))*@(\[([^\\\[\]\r]|\\.)*\]|[^\[\]()<>@,;:\\".]+)(\.(\[([^\\\[\]\r]|\\.)*\]|[^\[\]()<>@,;:\\".]+))*$/;
 */

/**
 * Regular expression of integer
 * Not limited in length
 * Examples :
 * -2
 * + 0005
 * 900
 */
RegExp.INT = /^(?:[-+]\s?)?[0-9]+$/;

/**
 * Regular expression of float
 * Not limited in length
 * Examples :
 * - 2,4
 * +2
 * 589.54
 * .54
 */
//RegExp.FLOAT = /^(?:[-+]\s?)?[0-9]*[.,]?[0-9]*$/;
RegExp.FLOAT = /^(?:[-+]\s?)?(?:(?:[0-9]+)|(?:[.,][0-9]+)|(?:[0-9]+[.,][0-9]+))$/;

/**
 * Regular expression of (French) phone number
 * International number not supported
 * @see <a href="http://en.wikipedia.org/wiki/Country_calling_codes">Country calling codes</a>
 * Examples :
 * - 2,4
 * +2
 * 589.54
 * .54
 */
RegExp.PHONE_FR = /^0[1-9](?:[-. ]?[0-9]{2}){4}$/; // 06 36 67 49 53 | 0892 13 51 61

/**
 * Regular expression of (French) date
 * Format : DD MM YYYY (support 2 number years)
 * Examples :
 * 12 07 1990
 * 25-12-2000
 * 11/09/01
 */
RegExp.DATE_FR = /^((?:0[1-9])|(?:[1-2][0-9])|(?:3?[01]))[/ -]((?:0[1-9])|(?:1[0-2]))[/ -]([0-9]{2}(?:[0-9]{2})?)$/;
/**
 * Regular expression of (international) date
 * Format : YYYY MM DD (support 2 number years)
 * Examples :
 * 1990 07 12
 * 2000-12-25
 * 01/09/11
 */
RegExp.DATE=/^([0-9]{2}(?:[0-9]{2})?)[/ -]((?:0[1-9])|(?:1[0-2]))[/ -]((?:0[1-9])|(?:[1-2][0-9])|(?:3[01]))$/;

/**
 * Regular expression of (French) Zip Code
 * @see <a href="http://en.wikipedia.org/wiki/List_of_French_postal_codes>French postal codes</a>
 * Examples :
 * 10 000
 * 75000
 */
RegExp.ZIP_CODE_FR = /^[0-9]{2}\s?[0-9]{3}$/;

/**
 * Regular expression of something (printed characters)
 * Examples :
 * hello world !
 */
RegExp.SOMETHING = /[^ \t\n\r\f\v]+/;
 
/**
 * Regular expression of login
 */
RegExp.LOGIN = /^[a-z0-9\._]{4,50}$/i;

/**
 * Regular expression of price
 * Examples :
 * 678,31
 * 2,4
 * 2
 * 589.54
 * 1.54
 */
RegExp.MONEY = /^[0-9]+(?:[,\.][0-9]{1,2})$/;

/**
  * Regular expression of the passwords
	*	more than 6 chars
	*/

RegExp.PASSWORD = /^[a-zA-Z0-9_\*\$@&%\.,;:+!?=#-]{6}[a-zA-Z0-9_\*\$@&%\.,;:+!?=#-]*$/;


/**
 * Object dialBox contend methods to make a in popup
 */
var dialBox = {
  mask : null,
  box : null,
  /**
   * draw is a method to draw the in popup
   *
   * @param content   a String, content of the in popup
   * @return          a DOMObject, return the link of created object
   */
  draw : function(content, titleContent, closeOnClick)
  {
		if(closeOnClick !== false){closeOnClick = true;};
		
    //Ajout du masque
    var mask = document.createElement('DIV');
    mask.style.visibility = 'hidden';
    mask.className = 'dialbox-mask';
    this.mask = document.body.appendChild(mask);
    //addEventListener(mask, 'click', function(){dialBox.erase();}, false);//IE Bugs
		
		if(closeOnClick){
	    mask.onclick = function()
  	  {
    	  dialBox.erase();
   		};
		}
    
    //TODO: add an annimation    
    
    var box = document.createElement('DIV');//create parent
    box.style.visibility = 'hidden';
    box.className = 'dialbox';
    this.box = document.body.appendChild(box);
    this.box.appendChild(document.createElement('H2')).innerHTML = titleContent || 'Information :';
    this.box = this.box.appendChild(document.createElement('DIV'));
        
    if(typeof content === 'string')
      this.box.innerHTML = content;
    else
      this.box.appendChild(content);
      
    var boxStyle = box.style;
	
		if(isIE && !isIE7){
			boxStyle.left = ( getViewportWidth() - box.offsetWidth ) / 2 + document.documentElement.scrollLeft + 'px';
			boxStyle.top = ( getViewportHeight() - box.offsetHeight ) / 2 + document.documentElement.scrollTop + 'px';
		}else{
	    boxStyle.top = ( mask.offsetHeight - box.offsetHeight ) /2 + 'px';
  	  boxStyle.left = ( mask.offsetWidth - box.offsetWidth ) /2 + 'px';
		}

    mask.style.visibility = 'visible';
    boxStyle.visibility = 'visible';
		
		if(isIE){
			swapSelects(box);
		}
		
    return box;
  },
  
  /**
   * erase is a method to remove the in popup
   *
   * @return     Void
   */
  erase : function()
  {
    //TODO: ajouter une annimation de disparition
    //Supression du masque

		if(isIE){
			this.box.style.visibility = 'hidden';
			swapSelects(this.box);
		}
				
    remove(this.mask);
    this.mask = null;
    remove(this.box.parentNode);
    this.box = null;
  },
  
  /**
   * drawAJAXContent is a method to make a HTTPTransaction and draw a in popup
   *
   * @params params  An Object, with each properties are content value : 
   *                 url, datas, httpMode, headers, asyncMode, user, pass, events
   *                 (Optional)
   * @return           Void
   */
  drawAJAXContent : function(params)
  {
    AJAX.makeHttpRequest(params);
  }
};

/*
 * Object mouse contend x and y position of cursor
 */
var mouse = {
  x : null,
  y : null,
  toString : function()
  {
    return this.x + ' ' + this.y;
  }/*,
  getCursorStyle : function()
  {
    
  };*/
};

//initialize capture of x and y mouse pos
addEventListener(document, 'mousemove',function(e){
	mouse.x = e.pageX ? e.pageX : e.clientX + document.documentElement.scrollLeft;
	mouse.y = e.pageY ? e.pageY : e.clientY + document.documentElement.scrollTop;
}, false);

/**
 * Object dialBox contend methods to make a tooltips
 */
var toolTips;
//Initalize at document loaded
addEventListener(document, 'DOMContentLoaded', function()
  {
    toolTips = document.body.appendChild(document.createElement('DIV'));
    hide(toolTips);
    toolTips.style.display = 'none';
	toolTips.style.visibility = 'visible'; 
    toolTips.className = 'tooltips';
    toolTips.setPos = function(x, y)
    {
      this.style.left = x + 10 + 'px';
      this.style.top = y + 10 + 'px';
    };
    toolTips.activate = function(element, content)
    {
      if(content)
      element.toolTips.content = content;
      element.onmouseover = null;
      addEventListener(element, 'mouseover', function()
        {
          show(toolTips);
//too slow
/*  				if(isIE){
						swapSelects(toolTips);
					}*/
        }, false);
      addEventListener(element, 'mouseout', function()
        {
          hide(toolTips);
//too slow
/*					if(isIE){
						swapSelects(toolTips);
					}*/
        }, false);
    };
  }, false);


/**
 * activateToolTips is a Method of HTMLElement to activate tooltips on a element
 *
 * @param content      a String, content of tooltips or indicate if content is get by HTTPRequest
 * @return             Void
 */
function activateToolTips(el,content)
{
  if(content == 'AJAX_MODE')
  {
    var url = arguments[2];
    //TODO : finish this mode
  }
  else
  {
    el.toolTipsContent = content;
    el.onmouseover = function()
    {
      toolTips.innerHTML = el.toolTipsContent;
      show(toolTips);
//too slow
/*  		if(isIE){
				swapSelects(toolTips);
			}*/
  	}
    el.onmouseover();
    el.onmouseout = function()
    {
      hide(toolTips);

//too slow
/*			if(isIE){
				swapSelects(toolTips);
			}*/
    }
    el.onmousemove = function()
    {
      toolTips.setPos(mouse.x, mouse.y);

//too slow
/*			if(isIE){
				swapSelects(toolTips);
			}*/
    };
  }
};

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

/**
	* Object HUD contend methods to make a tooltips
	*/
var HUD = {
	//activate : function(plip,plop){}
  activate : function(message, className){
		className = className || '';
		HUD = document.body.appendChild(document.createElement('DIV'));
		if(!isNull(className))
			HUD.className = className;
		HUD.innerHTML = message;
	}
}

addEventListener(document,'DOMContentLoaded', function()
	{
	  if(document.getElementById('HUD'))//gE() bug avec IE
  	{
  	  HUD = gE('HUD');
   	  hide(HUD);
  	}
  	else
	  {
	    HUD = document.body.appendChild(document.createElement('DIV'));
   	  hide(HUD);
	    HUD.id = 'HUD';
	  }
	  HUD.activate = function(message, className)
	  {
	    this.innerHTML = message;
	    this.className = className || '';
	    show(this);
			
			if(HUD.attachEvent){
				HUD.style.top = document.documentElement.scrollTop + 'px';
			}
	  }
	});

/*//IE doesn't understand "position: fixed"
if(HUD.attachEvent){
	addEventListener(element, 'scroll', function(){
		HUD.style.top = document.documentElement.scrollTop + 'px';
	}, false);													
}*/

var module = {
  action : function(action, id, name, title, new_div, newSystem, instantiable)
  {
		title = title || '';
		new_div = new_div || false;
    var element = gE('M' + id);
    //var element = gE('M' + id);
    switch(action)
    {
      case 'del':
        HUD.activate('Fermeture du module, veuillez patienter', 'loading');
				if(new_div){
					element = element.parentNode;
				}
        remove(element);
				/*
        var liElement = document.createElement("li");
				var aElement = document.createElement("a");
        aElement.onclick = function(){
					if(gE('addable_modules_list').childNodes.length == 1){
						show(gE('empty_module_list_alert'));
					};
				  remove(this.parentNode);
				  module_pref('add',name);
				};
				aElement.appendChild(document.createTextNode(title));
				liElement.appendChild(aElement);
				
				var i=0;
        var good_pos = false;
				var titleListLength = gE('addable_modules_list').childNodes.length;
				var titleList = gE('addable_modules_list');
				while(good_pos == false && i < titleListLength){
					new_title = titleList.childNodes[i].firstChild.firstChild.data;
				  if(title.toLowerCase() > new_title.toLowerCase()){
						i++;
					}
					else{
						good_pos=true;
					}
				}
				if(i==titleListLength){
				  gE('addable_modules_list').appendChild(liElement);
				}
				else{
				  gE('addable_modules_list').insertBefore(liElement,gE('addable_modules_list').childNodes[i]);
				}
				hide(gE('empty_module_list_alert'));*/
				
				if( !instantiable){
	        var optElement = document.createElement("option");
					optElement.value = name;
					optElement.className = 'N';
					optElement.appendChild(document.createTextNode(title));
				
					var i=0;
 			    var good_pos = false;
					var titleList = gE('addable_modules_list');
					var titleListLength = titleList.options.length;
					
					while( !good_pos && i < titleListLength){
						new_title = titleList.options[i].innerHTML;
					  if( title.toLowerCase() > new_title.toLowerCase() ){
							i++;
						}
						else{
							good_pos = true;
						}
					}
					
					if( i == titleListLength ){
					 	titleList.appendChild(optElement);
					}else{
						titleList.insertBefore(optElement, titleList.childNodes[i]);
					}
					
				}
        break;
			case 'up':
			  HUD.activate('D&eacute;placement du module vers le haut, veuillez patienter', 'loading');
				//Gestion des modules venant d etre inseres
				if(new_div){
					element = element.parentNode;
				}
				
				var previousSibling = element.previousSibling;

				//Pour gerer les espaces inter-modules vus differemment selon les navigateurs
			  element.parentNode.appendChild(remove(element));
			  while(!isNull(previousSibling) && (previousSibling.nodeType == 3 || isNull(previousSibling.firstChild) || previousSibling.tagName.toLowerCase() != 'div') ){
				  previousSibling = previousSibling.previousSibling;
				}
				if(isNull(previousSibling)){
			    element.parentNode.appendChild(remove(element));
				}else{
					element.parentNode.insertBefore(remove(element), previousSibling);
				}
				break;
			case 'down':
				HUD.activate('D&acute;placement du module vers le bas, veuillez patienter', 'loading');
				if(new_div){
					element = element.parentNode;
				}
				
				var nextSibling = element.nextSibling;	
				var firstPos = element.parentNode.firstChild;
				//Pour gerer les espaces inter-modules vus differemment selon les navigateurs
				while(!isNull(nextSibling) &&  (nextSibling.nodeType == 3 || isNull(nextSibling.firstChild) || nextSibling.tagName.toLowerCase() != 'div') ){
				  nextSibling = nextSibling.nextSibling;
				}
				if(isNull(nextSibling)){
					if(firstPos != element){
					  element.parentNode.insertBefore(remove(element), firstPos);
					}
				}else{
				  element.parentNode.insertBefore(remove(nextSibling), element);
				}
				break;
      default:
				//void
        break;
    }
		
		if(newSystem){
			AJAX.getAndCall('include/engine/page/module/ajax/module.php?ga='+ action +'&mid='+ id, function() {hide(HUD);});
		}else{
			AJAX.getAndCall('include/ajax/module_pref.php?action='+ action +'&id='+ id, function() {hide(HUD);});
		}
		
  },

	minimize : function(id, newSystem)
	{
		var elem = gE("mod" + id);

		if(window.getComputedStyle(elem, '').display == 'none')
	  {
		  HUD.activate('D&eacute;roulement du module, veuillez patienter');
		  show(elem);
			
			if(newSystem){
				AJAX.getAndCall('include/engine/page/module/ajax/module.php?ga=minimaxi&mid='+ id, function() {hide(HUD);});
			}else{
				AJAX.getAndCall('include/ajax/module_pref.php?action=maximize&id='+ id, function(){hide(HUD);});
			}
		}
		else
		{
		  HUD.activate('R&eacute;duction du module, veuillez patienter');
		  hide(elem);
		
			if(newSystem){
				AJAX.getAndCall('include/engine/page/module/ajax/module.php?ga=minimaxi&mid='+ id, function() {hide(HUD);});
			}else{
				AJAX.getAndCall('include/ajax/module_pref.php?action=minimize&id='+ id, function(){hide(HUD);});
			}
			
		}
	}
																												
};

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


/**
 * TABS is an useful object to display a tabs menu
 *
*/
var TABS = {
	
	/**
	 * displayTab()
	 * @param   tabs_list_id    id of tabs menu
	 * @param   new_content_id  id of new tab content to display
	 * @return  void
	*/
	displayTab : function (tabs_list_id, new_content_id){
		tabs_list = gE(tabs_list_id);

		//search of current selected tab
		for(var i=0; i<tabs_list.childNodes.length; i++){
			if(tabs_list.childNodes[i].className != '' && tabs_list.childNodes[i].className != undefined){
				old_tab_id = tabs_list.childNodes[i].id;
				old_content_id = old_tab_id.replace(/tab_/,'');
			}
		}
		
		//change displayed tab if new tab different from current tab
		if(new_content_id != old_content_id){
			old_content = gE(old_content_id);
			new_content = gE(new_content_id);
			new_tab = gE('tab_'+new_content_id);
			old_tab = gE(old_tab_id);

			//tab is selected
			new_tab.className=old_tab.className;
			old_tab.className='';

			//tab content is displayed
			display(old_content);
			display(new_content);
		}
	}
	
}

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

/*
Copyright (c) 2005 JSON.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The Software shall be used for Good, not Evil.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
    The global object JSON contains two methods.

    JSON.stringify(value) takes a JavaScript value and produces a JSON text.
    The value must not be cyclical.

    JSON.parse(text) takes a JSON text and produces a JavaScript value. It will
    throw a 'JSONError' exception if there is an error.
*/
var JSON = {
    copyright: '(c)2005 JSON.org',
    license: 'http://www.crockford.com/JSON/license.html',
/*
    Stringify a JavaScript value, producing a JSON text.
*/
    stringify: function (v) {
        var a = [];

/*
    Emit a string.
*/
        function e(s) {
            a[a.length] = s;
        }

/*
    Convert a value.
*/
        function g(x) {
            var c, i, l, v;

            switch (typeof x) {
            case 'object':
                if (x) {
                    if (x instanceof Array) {
                        e('[');
                        l = a.length;
                        for (i = 0; i < x.length; i += 1) {
                            v = x[i];
                            if (typeof v != 'undefined' &&
                                    typeof v != 'function') {
                                if (l < a.length) {
                                    e(',');
                                }
                                g(v);
                            }
                        }
                        e(']');
                        return;
                    } else if (typeof x.valueOf == 'function') {
                        e('{');
                        l = a.length;
                        for (i in x) {
                            v = x[i];
                            if (typeof v != 'undefined' &&
                                    typeof v != 'function' &&
                                    (!v || typeof v != 'object' ||
                                        typeof v.valueOf == 'function')) {
                                if (l < a.length) {
                                    e(',');
                                }
                                g(i);
                                e(':');
                                g(v);
                            }
                        }
                        return e('}');
                    }
                }
                e('null');
                return;
            case 'number':
                e(isFinite(x) ? +x : 'null');
                return;
            case 'string':
                l = x.length;
                e('"');
                for (i = 0; i < l; i += 1) {
                    c = x.charAt(i);
                    if (c >= ' ') {
                        if (c == '\\' || c == '"') {
                            e('\\');
                        }
                        e(c);
                    } else {
                        switch (c) {
                        case '\b':
                            e('\\b');
                            break;
                        case '\f':
                            e('\\f');
                            break;
                        case '\n':
                            e('\\n');
                            break;
                        case '\r':
                            e('\\r');
                            break;
                        case '\t':
                            e('\\t');
                            break;
                        default:
                            c = c.charCodeAt();
                            e('\\u00' + Math.floor(c / 16).toString(16) +
                                (c % 16).toString(16));
                        }
                    }
                }
                e('"');
                return;
            case 'boolean':
                e(String(x));
                return;
            default:
                e('null');
                return;
            }
        }
        g(v);
        return a.join('');
    },
/*
    Parse a JSON text, producing a JavaScript value.
*/
    parse: function (text) {
        return (/^(\s+|[,:{}\[\]]|"(\\["\\\/bfnrtu]|[^\x00-\x1f"\\]+)*"|-?\d+(\.\d*)?([eE][+-]?\d+)?|true|false|null)+$/.test(text)) &&
            eval('(' + text + ')');
    }
};

