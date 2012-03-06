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


class Ldap {
   var $dn = '';
   var $connexion;

   function Ldap () {

   }

   //Connexion au directory LDAP
   function Connect($server, $port = 389, $dn = '', $user = '', $pass = '') {
      $this->dn = $dn;
      if ($this->connexion = ldap_connect($server, $port)) {
         if ($pass == '') {$pass = crypt(microtime()); }

         if ($user == 'anonymous') {
            $pass = '';
            $user = '';
         }

         if (@ldap_bind($this->connexion, $user, $pass)) {
            return true;
         } else {
            return false;
         }
      } else {
         echo ''.label('error_cnx_ldap','include').'';
         return false;
      }
   }

   //Recherche
   function Search($dn, $filter) {
      $sr = ldap_search($this->connexion, $dn, $filter);
      if (!$sr) {
         $this->Error();
      }
      return $sr;
   }


   //Retourne le nb de lignes d'une recherche
   function NumRows($sr) {
      return ldap_count_entries($this->connexion, $sr);
   }

   //Retourne le resultat dans un tableau
   function FetchArray($sr) {
      return ldap_get_entries ($this->connexion, $sr);
   }

   //Ferme la connexion
   function Close() {
      ldap_close($this->connexion);
   }

   function  ModReplace ( $dn, $elementsToModify) {
      ldap_mod_replace ( $this->connexion, $dn, $elementsToModify);
   }

   function  ModDelete ( $dn, $elementsToDelete) {
      ldap_mod_del ( $this->connexion, $dn, $elementsToDelete);
   }

   //Affiche l'erreur ldap
   function Error() {
      echo '<table border="0" cellspacing="1" cellpadding="2" bgcolor="#DD0000" align="center">
             <tr>
              <td><font color="#FFFFFF"><b>Erreur LDAP (n°'.ldap_errno($this->connexion).')</b></font></td>
             </tr>
             <tr>
              <td bgcolor="#FFFFFF">' . ldap_error($this->connexion) . '</td>
             </tr>
           </table>';
   }
}

?>
