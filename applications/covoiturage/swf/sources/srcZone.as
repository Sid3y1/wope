/******************* srcZone.as ***************************************
Script ActionScript indispensable � la bonne ex�cution du fichier map.swf en 'mode B'
Il est appel� par chaque zone cliquable de d�partement au sein de la carte. Il g�re les comportements de ces boutons
(chaque bouton se comporte de la mm facon, mais avec des donnees diff�rentes).

J'ai externalis� les commandes ici car d�sormais une modif sur ces comportements se r�alise une fois ici, et pas 95 pour chaque bouton dans Flash !!

r�alisation : JB Blanc (creation301@yahoo.fr)
date de cr�ation : 04/08/2003
date de derni�re MAJ : 07/08/2003
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
