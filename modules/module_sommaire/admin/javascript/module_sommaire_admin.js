					function verifCreate(url){
						var error="";
						if(document.getElementById("title_name").value==""){
							error = error+"Veuillez donner un nom à votre titre. ";
						}
						if(document.getElementById("rights").selectedIndex==-1){
							error = error+"Veuillez définir qui a le droit de voir le titre.";
						}
						if((gE("title_category").selectedIndex!=0) && (gE("title_father_id").selectedIndex==0)){
							error = error+"Veuillez donner un père à votre titre.";
						}
						if(error!=""){
							alert(error);
						}
						else{
							for (var i=0; i<gE('form_title_create').title_is_hidden.length;i++) {
         				if (gE('form_title_create').title_is_hidden[i].checked) {
				          var is_hidden = gE('form_title_create').title_is_hidden[i].value;
								}
							}
							var rights ="";
							for (var i=0; i<gE('rights').length;i++){
								if(gE('rights')[i].selected==true){
									var rights = rights + gE('rights')[i].value + ' ';
								}
							}
							AJAX.getAndUpdate(url+'&title_name='+gE('title_name').value+'&title_url='+gE('title_url').value+'&title_category='+gE('title_category').value+'&title_father_id='+gE('title_father_id').value+'&title_is_hidden='+is_hidden+'&rights='+rights,'admin_module_sommaire_content');
						}
					}
					
					function enableFatherId(url){
						var cat = gE("title_category").options[gE("title_category").selectedIndex].value;
						AJAX.getAndUpdate(url+'&cat='+cat, "selectContent");
					}
						
					function verifUpdate(url){
						var error="";
						if(gE("title_name").value==""){
							error = error+"Veuillez donner un nom à votre titre.";
						}
						if(document.getElementById("rights").selectedIndex==-1){
							error = error+"Veuillez définir qui a le droit de voir le titre.";
						}
						if((gE("title_category").selectedIndex!=0) && (gE("title_father_id").selectedIndex==0)){
							error = error+"Veuillez donner un père à votre titre.";
						}
						if(error!=""){
							alert(error);
						}
						else{
							for (var i=0; i<gE('form_title_update').title_is_hidden.length;i++) {
         				if (gE('form_title_update').title_is_hidden[i].checked) {
				          var is_hidden = gE('form_title_update').title_is_hidden[i].value;
								}
							}
							var rights ="";
							for (var i=0; i<gE('rights').length;i++){
								if(gE('rights')[i].selected==true){
									var rights = rights + gE('rights')[i].value + ' ';
								}
							}
							AJAX.getAndUpdate(url+'&title_name='+gE('title_name').value+'&title_url='+gE('title_url').value+'&title_category='+gE('title_category').value+'&title_father_id='+gE('title_father_id').value+'&title_is_hidden='+is_hidden+'&rights='+rights+'&title_to_alt_id='+gE('title_to_alt_id').value,'admin_module_sommaire_content');
							var new_option = document.createElement('option');
							new_option.value = gE('title_to_alt_id').value;
							new_option.appendChild(document.createTextNode(gE('title_name').value));
							var i=0;
							while(gE('title_to_alt').options[i].value!=gE('title_to_alt_id').value){
								i++;
							}
							gE('title_to_alt').replaceChild(new_option,gE('title_to_alt').options[i]);
						}
					}
