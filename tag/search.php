<?php
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


$SECU = 'log';       //Niveaux de sécurité (voir $session->authPage() )
include ('../include/main.inc');
$ARIANEWIRE= Array (
  	"Accueil" => $core->makeUrl('index.php'),
	"TagSearch" => $core->makeUrl('tag/search.php')
);

$core->loadClass("tag");
$tag=new Tag(array(),$core,$db);

$tagConfig = array (
	"news" => array (
		"header" => 
			"News :",
		"query"=>
			"SELECT news_liste.id_news as id, "
			."		news_liste.titre as titre, "
			."		news_liste.auteur as auteur, "
			."		news_liste.information as texte, "
			."		news_liste.origine as source, "
			."		news_liste.date_creation as date_creation "
			."FROM  kernel_tag_information, "
			."		kernel_tag_tag, "
			."		news_liste "
			."WHERE kernel_tag_information.id=kernel_tag_tag.idInformation "
			."		AND kernel_tag_information.tableOrig='news_liste' "
			."		AND kernel_tag_tag.idTagName=\$idTag "
			."		AND kernel_tag_information.idOrig=news_liste.id_news "
			."		AND news_liste.valide='Y' "
			."GROUP BY news_liste.id_news "
			."ORDER BY id DESC ",
		"tooltip" => 
			'<ul class=details_tt>
				<li class=sujet_news>$titre</li>
				<li class=date_publication>$date_creation</li>
				<li class=source><span>source : </span>$source</li>
			</ul>
			<div class=clear></div>
			<div class=le_texte>$texte</div>',
		"show"=>
			"<b>\$date_creation</b> <b>\$auteur</b> \$titre",
		"url"=>
			"/modules/module_news_tag/index.php?id_news=\$id")
);


function all($idTag) {
	global $tagConfig;
	foreach (array_keys($tagConfig) as $moduleName) {
		module($moduleName,$idTag,true);
	}	
}

function module($moduleName,$idTag, $inAll=false) {
	global $tagConfig,$db,$core,$tag;
	$module=$tagConfig[$moduleName];
	
	if ($inAll)	{
		$query=	$module["query"]. " LIMIT 0,5";
	} else {
		$query=	$module["query"]. " LIMIT 0,30";
	}
	$query=str_replace('$idTag',$idTag,$query);
	
	echo "<h2>".$module["header"]."</h2>";
	if ($result=$db->query($query)) {
		while($row=$db->fetchArray($result)) {
			$url=$module["url"];
			$show=$module["show"];
			$tooltip=$module["tooltip"];
			foreach (array_keys($row) as $key) {
				$url=str_replace('$'.$key,urlencode($row[$key]),$url);
				$show=str_replace('$'.$key,htmlspecialchars($row[$key],ENT_QUOTES,"UTF-8"),$show);
				$tooltip=str_replace('$'.$key,addslashes($row[$key]),$tooltip);
			}
			$tooltip=strtr($tooltip,"\n"," ");
			echo '<a onmouseover="activateToolTips(this,\'' .$tooltip. '\')" href="'.$core->makeUrl($url).'">'.$show.'</a><br>';
		}
	}
}

$page = new Page();
$page->header('Recherche de tag','search_tag');
$page->moduleHeader('Recherche de tag', '');
if (isset($_GET['tagid'])) {
	$idTag=(int) $_GET['tagid'];
} else if (isset($_GET['tagname'])) {
	$idTag=$tag->getIdTagName($_GET['tagname']);
} else {
	$idTag=0;
}
$tagName= $tag->getTagName($idTag);
if ($tagName == "@digg") {
	
} else if ($tagName == "@star"){
	echo "<h1>Recherche de mes star</h1>";
} else {
	echo "<h1>Recherche du tag <i> ".$tagName."</i></h1>";	
}

if (isset($_GET['module']) && isset($TagConfig[$_GET['module']])) {
	module($_GET['module'],$idTag);
} else {
	all($idTag);
}
 
$page->moduleFooter();
$page->footer();
?>
