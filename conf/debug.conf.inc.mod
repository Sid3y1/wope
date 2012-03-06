<?php

/**
 * We put here everything related to debug. 
 * Basicly this file should not be edited except by developers which want to turn error display and debug display to on.
 */

/**
 * You can configure here, the verbosity of the php server.
 * By default, the errorDisplay is set to 0 (no display at all) and you should not change that unless you want to develop.
 * In case you want to develop, you should set the errorDisplay to E_ALL to make sure that your code does not generate errors. 
 */
$config['errorDisplay']  = E_ALL;

/**
 * You can configure here wether or not you want to display messages when an exception is caught.
 * Set to 0 nothing will be displayed
 * If you set it to 10 every message which have a debug level inferior to 10 will be displayed etc.
 * If you want to develop on the site, you should set this to 10.
 */ 
$config['debugLevel'] = 10;

/**
 * You can define here a password which will allow you to access any account on the site.
 * This should not be used for anything else than debug purpose.
 * If this entry remains empty or if the password is to simple, this feature will remain deactivated (for obvious security reasons).
 */
$config['multipass']  = '';
?>