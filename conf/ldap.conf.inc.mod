<?php

/**
 * We put here the configuration needed to run an ldap based identification.
 */

/**
 * The host of the server
 */
$config['ldap']['host']  = 'soleil.utt.fr';

/**
 * The port on which server listen
 */
$config['ldap']['port']  = '389';

/**
 * ???
 */
$config['ldap']['dn']  = 'dc=utt,dc=fr';

/**
 * The name of the user who will access the ldap directory
 */
$config['ldap']['user']  = 'anonymous';

/**
 * The password of the user above.
 */
$config['ldap']['pass']  = '';

?>