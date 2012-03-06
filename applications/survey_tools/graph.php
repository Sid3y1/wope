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


/*
	** Graphique sectoriel au format GIF
	*/


/*
	** Convertir les degrés en radians
	*/
function radians($degrees)
{
	return($degrees * (pi()/180.0));
}

/*
	** prendre x,y dans le cercle,
	** centre = 0,0
	*/
function circle_point($degrees, $diameter)
{
	$x = cos(radians($degrees)) * ($diameter/2);
	$y = sin(radians($degrees)) * ($diameter/2);

	return (array($x, $y));
}


//remplir les paramètres
$ChartDiameter = 100;
$ChartFont = 2;
$ChartFontHeight = imagefontheight($ChartFont);
$ChartData = array("15", "45",		"70","40", "25");
$ChartLabel = array("Html", "Dhtml","Javascript", "Java", "Php");

				$ChartLabel = unserialize(str_replace('THISISTHAT','"',$_GET['label']));
				$ChartData = unserialize(str_replace('THISISTHAT','"',$_GET['data']));

//déterminer la taille du graphique
$ChartWidth = $ChartDiameter + 300;
$ChartHeight = $ChartDiameter + 20 +
(($ChartFontHeight + 2) * count($ChartData));

$ChartHeight = $ChartDiameter + 20;
//détermine le total de toutes les valeurs
for($index = 0; $index < count($ChartData); $index++)
{
	$ChartTotal += $ChartData[$index];
}

$ChartCenterX = $ChartDiameter/2 + 10;
$ChartCenterY = $ChartDiameter/2 + 10;


//image
$image = imagecreate($ChartWidth, $ChartHeight);

//couleurs
$colorBody = imagecolorallocate($image, 0xFF, 0xFF, 0xFF);
$colorBorder = imagecolorallocate($image, 0x00, 0x00, 0x00);
$colorText = imagecolorallocate($image, 0x00, 0x00, 0x00);

$colorSlice[] = imagecolorallocate($image, 0xFF, 0x00, 0x00);
$colorSlice[] = imagecolorallocate($image, 0x00, 0xFF, 0x00);
$colorSlice[] = imagecolorallocate($image, 0x00, 0x00, 0xFF);
$colorSlice[] = imagecolorallocate($image, 0xFF, 0xFF, 0x00);
$colorSlice[] = imagecolorallocate($image, 0xFF, 0x00, 0xFF);
$colorSlice[] = imagecolorallocate($image, 0x00, 0xFF, 0xFF);
$colorSlice[] = imagecolorallocate($image, 0x99, 0x00, 0x00);
$colorSlice[] = imagecolorallocate($image, 0x00, 0x99, 0x00);
$colorSlice[] = imagecolorallocate($image, 0x00, 0x00, 0x99);
$colorSlice[] = imagecolorallocate($image, 0x99, 0x99, 0x00);
$colorSlice[] = imagecolorallocate($image, 0x99, 0x00, 0x99);
$colorSlice[] = imagecolorallocate($image, 0x00, 0x99, 0x99);

//arrière-plan
imagefill($image, 0, 0, $colorBody);


/*
	** Dessiner chaque portion
	*/
$Degrees = 0;
for($index = 0; $index < count($ChartData); $index++)
{
	$StartDegrees = round($Degrees);
	$Degrees += (($ChartData[$index]/$ChartTotal)*360);
	$EndDegrees = round($Degrees);

	$CurrentColor = $colorSlice[$index%(count($colorSlice))];

	//dessiner un arc
	imagearc($image,
			$ChartCenterX,
			$ChartCenterY,
			$ChartDiameter,
			$ChartDiameter,
			$StartDegrees,
			$EndDegrees,
			$CurrentColor);

	//Tracer le début de la ligne à partir du centre
	list($ArcX, $ArcY) = circle_point($StartDegrees, $ChartDiameter);
	imageline($image,
			$ChartCenterX,
			$ChartCenterY,
			floor($ChartCenterX + $ArcX),
			floor($ChartCenterY + $ArcY),
			$CurrentColor);


	//dessiner la fin de la ligne
	list($ArcX, $ArcY) = circle_point($EndDegrees, $ChartDiameter);
	imageline($image,
			$ChartCenterX,
			$ChartCenterY,
			ceil($ChartCenterX + $ArcX),
			ceil($ChartCenterY + $ArcY),
			$CurrentColor);

	//remplir les portions
	$MidPoint = round((($EndDegrees - $StartDegrees)/2) +  $StartDegrees);
	list($ArcX, $ArcY) = circle_point($MidPoint, $ChartDiameter/2);
	imagefilltoborder($image,
			floor($ChartCenterX + $ArcX),
			floor($ChartCenterY + $ArcY),
			$CurrentColor,
			$CurrentColor);

}


//la bordure

imagearc($image,
		$ChartCenterX,
		$ChartCenterY,
		$ChartDiameter,
		$ChartDiameter,
		0,
		180,
		$colorBorder);

imagearc($image,
		$ChartCenterX,
		$ChartCenterY,
		$ChartDiameter,
		$ChartDiameter,
		180,
		360,
		$colorBorder);

/*
imagearc($image,
		$ChartCenterX,
		$ChartCenterY,
		$ChartDiameter+7,
		$ChartDiameter+7,
		0,
		180,
		$colorBorder);

imagearc($image,
		$ChartCenterX,
		$ChartCenterY,
		$ChartDiameter+7,
		$ChartDiameter+7,
		180,
		360,
		$colorBorder);
*/

imagefilltoborder($image,
		floor($ChartCenterX +  ($ChartDiameter/2) +  0),
		$ChartCenterY,
		$colorBorder,
		$colorBorder);
/**/

//la légende
for($index = 0; $index < count($ChartData); $index++)
{
	$CurrentColor = $colorSlice[$index%(count($colorSlice))];
	$LineY = 10 +  ($index*($ChartFontHeight+2));
	$LineX = $ChartDiameter + 20;

	//la couleur des boîtes
	imagerectangle($image,
			$LineX,
			$LineY,
			$LineX + $ChartFontHeight,
			$LineY+$ChartFontHeight,
			$colorBorder);

	imagefilltoborder($image,
			$LineX + 2,
			$LineY + 2,
			$colorBorder,
			$CurrentColor);

	//Les titres
	imagestring($image,
			$ChartFont,
			$LineX + 10 + $ChartFontHeight,
			$LineY,
			"$ChartLabel[$index]: $ChartData[$index]",
			$colorText);


}


//afficher l'image
header("Content-type: image/gif");
imagegif($image);
?>

