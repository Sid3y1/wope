<?php

/**
 * We put here the configuration needed to use the memcache cache system.
 */

/**
 * We can configure here the host of the servers we want to use.
 * You can define how many server you want.
 */
$config['memcache']['server']['0']  = '';
$config['memcache']['server']['1']  = '';

/**
 * The ports on which those server will listen on.
 */
$config['memcache']['port']['0']  = '11211';
$config['memcache']['port']['1']  = '11211';

/**
 * ???
 */
$config['memcache']['menu']  = '1';

/**
 * ???
 */
$config['memcache']['modules']  = '1';

?>
