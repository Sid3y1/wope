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
