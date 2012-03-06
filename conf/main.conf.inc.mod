<?php

/**
 * This file is the main configuration file of Wope. It replaces the old conf_secure.inc file.
 * Some basic informations are directly defined here.
 * All the other configuration informations are defined in their respective files and then include here.
 * If you want to add any configuration informations, please don't write it directly in this file. Create a file in the conf directory and include it here.
 */

/**
 * Basic configurations
 */

/**
 * You can define here the name of the site.
 * This name will be used in all the emails the site will send, il will also be used as the name of the site cookie.
 */
$config['site_name']  = 'The name of your site';

/**
 * Write here all the subdirectories behind the domain of your site.
 */
$config['baseDir']  = '';

/**
 * Write here the full Url of the site
 */
$config['baseUrl']  = 'http://wope.example.com';

/**
 * If you want to use https, write here the full https url of the site.
 */
$config['baseUrls']  = $config['baseUrl'];

/**
 * Write here the full path to the site directory (at the filesystem level).
 */
$config['baseServer']  = '/path/to/wope/root/';

/**
 * Write here the time we should wait between to launch of automatic actions.
 * The site is able to launch script himself at regular interval of time.
 * Its used to check if the user as new emails, or to refresh some part of the page.
 * The time is given in milliseconds.
 * Be aware that launching script like that can take a lot of CPU time if you have a lot of users. 
 */
$config['timeUnit']  = '60000';

$config['database']['type']  = 'mysql';

/**
 * The host of database server
 */
$config['database']['host']  = 'localhost';

/**
 * The name of the base in which the data of the site are stored
 */
$config['database']['base']  = 'The name of the base';

/**
 * The name of the user who will access the base.
 * Avoid using the root user for obvious security reasons
 */
$config['database']['user']  = 'The name of the user';

/**
 * The password of the user defined above.
 */
$config['database']['pass']  = 'The paassword';

/**
 * Here just for backward compatibility reasons.
 */
$config['local']  = '';

/**
 * Includes
 */

/**
 * Configure here the email address which will be used in the site to receive or to send emails.
 */
include(dirname(__FILE__).'/contacts.conf.inc');

/**
 * This configuration file allow you to configure the filemanager system which
 * is used to include files in the different application of the site
 */
include(dirname(__FILE__).'/filemanager.conf.inc');

/**
 * In this file we configure some the directories of the site (eg the directory where the avatars are stored).
 */
include(dirname(__FILE__).'/dir.conf.inc');

/**
 * In this file we configure the cache system of the site.
 */
include(dirname(__FILE__).'/cache.conf.inc');

/**
 * In this file you can configure everything related to emails,
 * including the host of the smtp server which will be used by the site to send emails.
 */
include(dirname(__FILE__).'/mail.conf.inc');

/**
 * If you want to use the phpmyvistes software to collect audience date on your site, please do it in this configuration file
 */
include(dirname(__FILE__).'/phpmyvisites.conf.inc');

/**
 * If you want to configure how the site handles error ans debug messages you can do it here
 * These configuration are mostly for developers since the default configuration is made for normal use (no error or debug messages are displayed).
 */
include(dirname(__FILE__).'/debug.conf.inc');

/**
 *  If you want to use the memcache system include this configuration file.
 */
//include(dirname(__FILE__).'/memcache.conf.inc');

/**
 *  If we need an ldap server for the identification, we include the configuration here
 */
//include(dirname(__FILE__).'/ldap.conf.inc');



?>
