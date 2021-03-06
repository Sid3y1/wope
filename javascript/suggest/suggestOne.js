
/**
 * Provides suggestions for state names (USA).
 * @class
 * @scope public
 */
function RemoteStateSuggestions() {

    if (typeof XMLHttpRequest != "undefined") {
        this.http = new XMLHttpRequest();
    } else if (typeof ActiveXObject != "undefined") {
        this.http = new ActiveXObject("MSXML2.XmlHttp");
    } else {
        alert("No XMLHttpRequest object available. This functionality will not work.");
    }

}

/**
 * Request suggestions for the given autosuggest control. 
 * @scope protected
 * @param oAutoSuggestControl The autosuggest control to provide suggestions for.
 */
RemoteStateSuggestions.prototype.requestSuggestions = function (oAutoSuggestControl /*:AutoSuggestControl*/,
                                                          bTypeAhead /*:boolean*/) {

    var oHttp = this.http;
                                                             
    //if there is already a live request, cancel it
    if (oHttp.readyState != 0) {
        oHttp.abort();
    }                 
    //type = document.getElementById('type').value;
    type='login';
    //build the URL
				var sURL = document.getElementById('URL').value + encodeURIComponent(oAutoSuggestControl.textbox.value);
    //var sURL = "http://www-etu.utt.fr/etu/poly/pages/xmlhttp.php?xmlhttp=liste&type=login&valeur=" + encodeURIComponent(oAutoSuggestControl.textbox.value);
   //var sURL = "http://www-etu.utt.fr/etu/poly/includes/js/sidtry.php"; 
//   alert(type);
    //open connection to states.txt file
    oHttp.open("get", sURL , true);
    oHttp.onreadystatechange = function () {
        if (oHttp.readyState == 4) {
            //evaluate the returned text JavaScript (an array)
            var aSuggestions = eval(oHttp.responseText);
        
            //provide suggestions to the control
            oAutoSuggestControl.autosuggest(aSuggestions, bTypeAhead);        
        }    
    };
    oHttp.send(null);
    

};
