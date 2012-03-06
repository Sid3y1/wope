/******************* srcZone.as ***************************************
Script ActionScript indispensable à la bonne exécution du fichier map.swf en 'mode B'
Il est appelé par chaque zone cliquable de département au sein de la carte. Il gère les comportements de ces boutons
(chaque bouton se comporte de la mm facon, mais avec des donnees différentes).

J'ai externalisé les commandes ici car désormais une modif sur ces comportements se réalise une fois ici, et pas 95 pour chaque bouton dans Flash !!

réalisation : JB Blanc (creation301@yahoo.fr)
date de création : 04/08/2003
date de dernière MAJ : 07/08/2003
**********************************************************************/
on (rollOver) {
	_root.currentID = this.id;
	_root.currentZone = this.nom;
	_root.setValues(this.id);
}
on (rollOut) {
	_root.currentID = "";
	_root.currentZone = "";
	_root.resetValues();
}
on (release) {
	if(_root.displayMode == "modeB" && _root.read_only != true){
		_root.addCurrentDptToList(this.id);
	}
}
