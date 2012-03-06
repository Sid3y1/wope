<?php

/**
 * You must define here the configuration of the filemanager system.
 * Filemanager is the system which is used to include files in the different application trought ou the site.
 * For more informations : http://www.wope-project.org/wiki
 */

/**
 * This is the quota configuration. It defines how much space each user will have to store is own files.
 * -1 mean no limits
 * The quota is given in bytes
 */
$config['fileManager']['quota'] = -1;

/**
 * You can define here where you want the file to be stored.
 */
$config['fileManager']['path'] = $config['baseServer'].'data/files';


?>