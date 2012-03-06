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

/**
 *  Fonction permettant de convertir l'Interclassement de toutes les tables
 *  d'une base. Cette fonction ne converti pas les données contenu dans la
 *  base. Il faut passer un script iconv sur un dump de la base pour convertir
 *  les données
 *      
 *  Ici on converti en utf-8   
 *
 */
 
mysql_connect('localhost','user','mdp'); // on s'identifie

mysql_select_db('base');  // on se connecte à la base

$tablesres = mysql_query('SHOW TABLES');

while($table = mysql_fetch_array($tablesres))
{
    mysql_query("ALTER TABLE ".$table['Tables_in_base']." CONVERT TO CHARACTER SET utf8");  // on effectue la conversion sur toute les tables
}

mysql_close();  // on quitte proprement

?>
