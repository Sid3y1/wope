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



/**
	* Programme issu du webmail roundCube completeent remodifie
	* Cette page permet de changer completement le type de session en utilisant 
	* le systemme Memcache
	*/
function sess_open($save_path, $session_name)
{
	return TRUE;
}



function sess_close()
{
	return TRUE;
}


// read session data
function sess_read($key)
{
	global $MEMCACHE, $SESS_CHANGED;
	$vars=$MEMCACHE->get("wope_sess_vars_$key");
	if ($vars!==FALSE) {
		$SESS_CHANGED=$MEMCACHE->get("wope_sess_changed_$key");
		if ($SESS_CHANGED!==FALSE) {
			return $vars;
		}
	}
	return FALSE;
}


// save session data
function sess_write($key, $vars)
{
	global $MEMCACHE;

	$max_lifetime=ini_get("session.cache_expire");
	$MEMCACHE->set("wope_sess_vars_$key",$vars,false,$max_lifetime);
	$MEMCACHE->set("wope_sess_changed_$key",time(),false,$max_lifetime);
	return TRUE;
}


// handler for session_destroy()
function sess_destroy($key)
{
	global $MEMCACHE;
	// empty roundcube_sess_vars et fait expirer la valeur dans 1s
	$MEMCACHE->set("wope_sess_vars_$key","",false,1);
	$MEMCACHE->set("wope_sess_changed_$key","",false,1);

	// also delete session entries in cache table
	$MEMCACHE->set("wope_cache_vars_$key","",false,1);
	return TRUE;
}


// garbage collecting function
function sess_gc($maxlifetime)
{
	//memcache has some mecanism for purging itself
	return TRUE;
}


// set custom functions for PHP session management
session_set_save_handler('sess_open', 'sess_close', 'sess_read', 'sess_write', 'sess_destroy', 'sess_gc');
?>
