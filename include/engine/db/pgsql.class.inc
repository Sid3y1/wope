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


class Pgsql {
   var $connexion;


   function Pgsql () {

   }


   function connect ($db) {

      if (!$this->connexion = pg_connect('host='.$db->host.' port=5432 dbname='.$db->base.' user='.$db->user.' password='.$db->pass)) {
         $this->error();
         exit;
      }
      return (1);

   }

   /**
     Execute une requete
     debug == 1 => on affiche la requete

   */
   function query($query, $debug = 0) {
      if ($debug) {
         echo $query;
      }
      if (!$result = pg_query($this->connexion,$query)) {
         $this->error();
      }
      return ($result);
   }

   //Retourne un champ de résultat
   //non implÃ©entÃ© ss postgre, implementation artificielle grace
   //a fetch array!
   function result($result, $column) {
      $result = pg_fetch_array($result,$column);
      return $result;
   }

   //Retourne l'id du dernier INSERT
   function insertId() {
      echo ''.label('no_function_postgre','include').' !';
      //$result = mysql_insert_id();
      //return $result;
   }

   //Retourne  le nombre de lignes d'un SELECT
   function numRows($result) {
      if ($result) {
         $result = pg_num_rows($result);
         return $result;
      } else {
         return 0;
      }
   }

   //décompose un resultSet dans un tableau
   function fetchArray($result) {
      $result = pg_fetch_array($result);
      return $result;
   }

   //retourne le nombre de lignes affectées par un UPDATE ou un DELETE
   function affectedRows() {
      $result = pg_affected_rows($this->connexion);
      return $result;
   }
	
	 //retourne une chaîne protégée pour passage à pgsql
   function escapeString($str){
	 		$result = pg_escape_string($str);
			return $result;
	 }

   //Ferme une connexion
   function close() {
      $result = pg_close($this->connexion);
      return $result;
   }


   //Affiche l'erreur sql
   function error() {
   global $_SERVER;
      echo '<table border="0" cellspacing="1" cellpadding="2" bgcolor="#00DD00" align="center">
             <tr>
              <td><font color="#FFFFFF"><b>Erreur PG  SQL (n°'.mysql_errno().')</b></font></td>
             </tr>
             <tr>
              <td bgcolor="#FFFFFF">'.pg_last_error().'</td>
             </tr>
           </table>';
   //var_dump($_SERVER);
   //$file = fopen('../erreurSql.log', 'a', 1);
   $file = fopen("sql.log","a",1);
   $text="\n Erreur Pg n ".mysql_errno()." - ".date('r')." - ".$_SERVER["PHP_SELF"]."\n".pg_last_error()."\n";
   echo 'coucou';
   echo $text;
   fwrite($file, $text);
   fclose($file);




   }
}

?>
