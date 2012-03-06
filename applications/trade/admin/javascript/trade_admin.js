function trade_verifCreate(form){
	return  (	( form.category.options[form.category.selectedIndex].value != 'new' && form.category.options[form.category.selectedIndex].value != 'NULL' )	||  form.new_category.value != ''	)	&& form.title.value != ''	&& form.content.value != ''	&& form.contact_firstname.value != ''	&& form.contact_lastname.value != '' && ( form.contact_email.value != '' || form.contact_address.value != '' || form.contact_phone.value != '' || form.contact_mobile.value != '' );
}

function trade_searchSubCateg(select, sub_select_id, strCat){
	var tmp = strCat.split('#;#');
	var arCat = new Array();
	
	for(var i=0; i<tmp.length; i++){
		var atmp = tmp[i].split('#=>#');
		var btmp = atmp[1].split('#,#');
		for(var j=0; j<btmp.length; j++){
			btmp[j] = btmp[j].split('#-#');
		}
		arCat[atmp[0]] = btmp;
	}
	
  var cat = select.options[select.selectedIndex].value;
	var sub_select = gE(sub_select_id);
	var first_o_value = sub_select.options[0].value;
	var first_o_inner = sub_select.options[0].innerHTML;
	
	sub_select.options.length = 0;
	
	var first_o = document.createElement('option');
	//IE
	if(document.all){
		sub_select.add(first_o, 0);
	}else{
		sub_select.add(first_o, null);
	}

	first_o.value = first_o_value;
	first_o.appendChild(document.createTextNode(first_o_inner));

	if( arCat[cat] != undefined ){
		for(var i=0; i<arCat[cat].length; i++){
			var o = document.createElement('option');
			//IE
			if(document.all){
				sub_select.add(o, i+1);
			}else{
				sub_select.add(o, null);
			}
			o.value = arCat[cat][i][0];
			o.appendChild(document.createTextNode(arCat[cat][i][1]));
		}
	}

}

function trade_createSubCateg(select, sub_select_id, strCat){
 	var tmp = strCat.split('#;#');
	var arCat = new Array();
	
	for(var i=0; i<tmp.length; i++){
		var atmp = tmp[i].split('#=>#');
		var btmp = atmp[1].split('#,#');
		for(var j=0; j<btmp.length; j++){
			btmp[j] = btmp[j].split('#-#');
		}
		arCat[atmp[0]] = btmp;
	}
	
  var cat = select.options[select.selectedIndex].value;
	var sub_select = gE(sub_select_id);
	var ss_length = sub_select.options.length;
	
	//enables sub_select
  sub_select.disabled = false;
	
	//copy of first option
	var first_o_value = sub_select.options[0].value;
	var first_o_inner = sub_select.options[0].innerHTML;

	//copy of last option
	var last_o_value = sub_select.options[ss_length -1].value;
	var last_o_inner = sub_select.options[ss_length -1].innerHTML;
	var last_o_onclick = sub_select.options[ss_length -1].onclick;
	
	sub_select.options.length = 0;

	var first_o = document.createElement('option');
	//IE
	if(document.all){
		sub_select.add(first_o, 0);
	}else{
		sub_select.add(first_o, null);
	}

	first_o.value = first_o_value;
	first_o.appendChild(document.createTextNode(first_o_inner));

	if( arCat[cat] != undefined ){
		for(var i=0; i<arCat[cat].length; i++){
			var o = document.createElement('option');
			//IE
			if(document.all){
				sub_select.add(o, i+1);
			}else{
				sub_select.add(o, null);
			}
			o.value = arCat[cat][i][0];
			o.appendChild(document.createTextNode(arCat[cat][i][1]));
		}
	}
	
	var last_o = document.createElement('option');
	//IE
	if(document.all){
		sub_select.add(last_o, arCat[cat].length +1);
	}else{
		sub_select.add(last_o, null);
	}
	last_o.value = last_o_value;
	last_o.onclick = last_o_onclick;
	last_o.appendChild(document.createTextNode(last_o_inner));

}
