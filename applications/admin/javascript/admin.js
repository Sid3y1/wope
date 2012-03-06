/**
*   Copyright : (C) 2007 Wope
*   License : GNU GPL
*   Contact : http://www.wope-project.org
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
*   MA  02111-1307  USA
*
**/

//TODO: eradication of this function. Use AJAX.submitAnd(Call|Update) instead.
/**
 * submitForm() send form datas with ajax
 * @param form   id of form object
 * @url url      url where to send datas
 * @backurl      url to call with ajax if success (optional)
 * @backdiv      div to refresh with ajax if success (optional)
 * @return void  
**/
function submitForm(form, url, backurl, backdiv){
	backurl = backurl || '';
	backdiv = backdiv || '';
	var handleSuccess =  function(o){ 
  	if(o.responseText !== undefined){
			if(backurl!='' || backdiv!='')
				AJAX.getAndUpdate(backurl,backdiv);
		}
	}

	var handleFailure = function(o){
  	if(o.responseText !== undefined){
			alert(o.responseText);
		}
	}

	var handleUpload = function(o){
  	if(o.responseText !== undefined){
			if(backurl!='' || backdiv!='')
				AJAX.getAndUpdate(backurl,backdiv);
		}
	}

	var callback ={ 
		success: handleSuccess,
  	failure: handleFailure, 
		upload: handleUpload
	};
	
	var frm = document.getElementById(form);
	YAHOO.util.Connect.setForm(frm, true);
	YAHOO.util.Connect.asyncRequest('POST', url, callback);
}
