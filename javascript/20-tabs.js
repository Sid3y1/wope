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
