<?php

/**
 * You can configure here everything related to mail systems.
 */

/**
 * Configure here hte host of your smtp server. 
 * If you do not configure this entry correctly, the site will not be able to send any emails.
 * By default, we consider that your mail server is on the same box than your web server.
 */
$config['mailHost']  = 'localhost';

/**
 * This entry is used only if you have a webmail linked to wope.
 * It allows the site to check if the user have new emails, and raise a popup to tell him so.
 * The time is in milliseconds.
 * Be aware that this feature can take a lot of CPU time if you have a lot of users.
 */
$config['timeToCheckMail']  = '1200000';



?>