-- Modifications of the menu tables --
--RENAME TABLE `pageMenu` TO `kernel_skin_menu`;
--ALTER TABLE `kernel_skin_menu`  COMMENT = 'This table contain the menu entries.' ENGINE = innodb;
--ALTER TABLE `kernel_skin_menu` ADD `rank` TINYINT UNSIGNED NOT NULL COMMENT 'Used to order the menu entries' AFTER `class` ;
--DROP TABLE `usersmenu`;

-- New modification of menu table in order to add permission management and rename some fields --
-- 05/28/07 --
ALTER TABLE `kernel_skin_menu` CHANGE `class` `image` VARCHAR( 200 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL ;
ALTER TABLE `kernel_skin_menu` ADD `permission` VARCHAR( 200 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `rank` ;

-- Ajout des preférences pour le module horoscope
INSERT INTO `prefs_liste` ( `pref_name` , `pref_value` , `pref_title` , `description` , `type` , `display` )
VALUES (
'module_horoscope_sign', 'none', 'Module horoscope', 'Choisissez votre signe pour le module horoscope', 'select', 'Y'
);

INSERT INTO `prefs_values` (`pvals_id`, `pvals_name`, `pvals_value`) VALUES 
(116, 'module_horoscope_sign', 'lion'),
(117, 'module_horoscope_sign', 'belier'),
(118, 'module_horoscope_sign', 'taureau'),
(119, 'module_horoscope_sign', 'gemeaux'),
(120, 'module_horoscope_sign', 'cancer'),
(121, 'module_horoscope_sign', 'vierge'),
(122, 'module_horoscope_sign', 'balance'),
(123, 'module_horoscope_sign', 'scorpion'),
(124, 'module_horoscope_sign', 'sagittaire'),
(125, 'module_horoscope_sign', 'capricorne'),
(126, 'module_horoscope_sign', 'verseau'),
(127, 'module_horoscope_sign', 'poissons'),
(127, 'module_horoscope_sign', 'none');