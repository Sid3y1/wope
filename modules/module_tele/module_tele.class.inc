<?
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


class Module_tele extends Module {

	public final function __construct($informations = Array()) {
		parent::__construct($informations);
	}
	
	public function display() 
	{
	  global $core;
		//Source du site d'où l'on tient toutes les informations.
		$fichier = 'http://www.programme-tv.net/programme-tv-de-ce-soir.html';
		//Le fichier physique local va permettre de rapatrier les infos 1 fois toutes les 6h ou quand le jour change
		//Ainsi, on évite de bouffer de la bande passante à chaque fois qu'un blaireau se connecte et regarde le progTV :)
		$fichier_physique = dirname(__FILE__)."/prog_tv.html";
		//Si le fichier local n'existe pas ou a plus de 6 heures ou si la date à changé on le génère...
		if(!file_exists($fichier_physique) || date("YmdH", filemtime($fichier_physique))+6 < date("YmdH") || date("Ymd",filemtime($fichier_physique)) != date("Ymd"))
		{
			//Création du fichier physique local à partir du fichier distant.
			$affichage ="";
			if (!$handle = fopen($fichier,'r')) 
			{
				echo '<p>Problème d\'ouverture du fichier distant<p>';
				return false;
			}
			if (!$handle_local = fopen($fichier_physique,'w'))
			{
				echo '<p>Problème d\'ouverture du fichier local<p>';
				return false;
			}
			//Flag permettant d'enregistrer que la partie du code html qui nous intéresse
			$enregistre = false;
			$i = 1;//Variable contenant les Chaines (1 = TF1, 2 = France 2, ...)
			fwrite($handle_local, '<div>');
			//On lit le fichier distant jusqu'à trouver la partie qui nous intéresse, vive les regexp :p
			while ($ligne = fgets($handle,4096))
			{
				if(preg_match('/<div id="ptv"/',$ligne))
				  $enregistre = true;
				if($enregistre == false)
				  continue;
				else
				{
					//On charcute le fichier pour récupérer ce qui nous intéresse.
					if ((preg_match_all('/<a href="(.*?)"/',$ligne, $url)) && (preg_match_all('/;">.*?:.*? : (.*?) <span class="normal">/',$ligne,$titre)) && (preg_match_all('/;">(.*?):(.*?) : .*? <span class="normal">/',$ligne,$heure)) && (preg_match_all('/<span class="normal"> \((.*?)\)/', $ligne, $duree)) && (preg_match_all('/<span class="normal"> \(.*?\) (.*?)<\/span>/', $ligne, $type)))
					{
						//On affiche ici les logos des 6 chaines hertziennes.
						fwrite($handle_local, '<div class="one_channel"><div class="channel'.$i.' channels">&nbsp;</div><div class="prog_tele prog'.$i++.'"><ul>');
						while(list($key, $val) = each($url[1]))
						{
							//On récupère éventuellement une image et le résumé du film...
							$img = "";
							$desc = "Non communiqué";
							$image_recuperee = FALSE;
							$resume_recupere = FALSE;
							//On ouvre la page html qui contient les détails de chaque film.
							if(!$handle_distant = fopen($val,'r'))
							  echo '<li>Erreur lors de l\'ouverture du fichier distant</li>';
							//On lit cette page pour y trouver une image de l'émission et un résumé.
							while ($l = fgets($handle_distant, 4096))
							{
								if (preg_match('/^<img src="(.*?)"/', $l, $image) && !$image_recuperee)
								{
									$img = $image[1];
									$image_recuperee = TRUE;
								}
								if (preg_match('/R&eacute;sum&eacute; : <\/strong>(.*?)<\/p>/', $l, $resume) && !$resume_recupere)
								{
									$desc = $resume[1];
									$resume_recupere = TRUE;
								}
							}
							//On convertit les chaines de caractères contenant l'heure et les minutes au format numérique pour pouvoir les comparer avec des nombres. On filtre ainsi l'affichage des émissions passant après ou à 20h40.
							$h = intval($heure[1][$key]);
							$m = intval($heure[2][$key]);
							if ((($h == 20) && ($m >= 40)) || ($h > 20))
							{
							//On écrit les liens avec le nom du film et l'heure dans le fichier local.Ici on fait un test sur le fait qu'on a récupéré ou non une image, si oui on l'affiche si non on affiche que le résumé.
              if (!$image_recuperee)
                $affichage = '<li><a href="'. $val .'" onmouseover="//activateToolTips(this,\'<h3>'.$heure[1][$key].'h'.$heure[2][$key].' - '.addslashes($titre[1][$key]).' - '.$duree[1][$key].' - '.addslashes($type[1][$key]).'</h3><p>'.addslashes($desc).'</p>\')">'.$heure[1][$key].'h'.$heure[2][$key].' - '.$titre[1][$key].'</a></li>';
							else
                $affichage = '<li><a href="'. $val .'" onmouseover="//activateToolTips(this,\'<h3>'.$heure[1][$key].'h'.$heure[2][$key].' - '.addslashes($titre[1][$key]).' - '.$duree[1][$key].' - '.addslashes($type[1][$key]).'</h3><p>'.addslashes($desc).'</p><img src='.$img.' />\')">'.$heure[1][$key].'h'.$heure[2][$key]." - ".$titre[1][$key]."</a></li>";
							fwrite($handle_local,$affichage);
							}
							fclose($handle_distant);
						}
						fwrite($handle_local, '</ul></div><div class="clear"></div></div>');
						$affichage = '';
					}
				}
				if (preg_match('/<div class="pied">/', $ligne))
				  $enregistre = false;
			}
			fwrite($handle_local, '</div>');
			fclose($handle);
			fclose($handle_local);
			chmod($fichier_physique, 0664);
		}
		//On affiche le fichier local
		echo utf8_encode(file_get_contents($fichier_physique)); // Je rajoute un encodage en utf8 du resultat pour ne pas qu'il y ait de problème d'affichage [Rémi]
	}//accolade fonction resume 
}//accolade classe

?>
