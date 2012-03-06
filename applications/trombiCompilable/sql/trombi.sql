DROP TABLE IF EXISTS `trombi_trombi`;
CREATE TABLE `trombi_trombi` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT, 
  `id_user` INT UNSIGNED NOT NULL, 
  `nickname` VARCHAR(30)  NOT NULL default '', 
  `lastname` VARCHAR(30)  NOT NULL default '', 
  `firstname` VARCHAR(30)  NOT NULL default '', 
  `birthdate` DATE  NOT NULL default '0000-00-00', 
  `signal_social` VARCHAR(50)  NOT NULL default '', 
  `photo` VARCHAR(255) NOT NULL default '', 
  `freespace` TEXT  NOT NULL default '', 
  `address` VARCHAR(60)  NOT NULL default '', 
  `address2` VARCHAR(60)  NOT NULL default '', 
  `postal_code` VARCHAR(6)  NOT NULL default '', 
  `city` VARCHAR(60)  NOT NULL default '', 
  `phone` VARCHAR(15)  NOT NULL default '', 
  `fax` VARCHAR(15)  NOT NULL default '', 
  `email` VARCHAR(150)  NOT NULL default '', 
  `email2` VARCHAR(150)  NOT NULL default '', 
  `site` VARCHAR(30)  NOT NULL default '', 
  `company_name` VARCHAR(30)  NOT NULL default '', 
  `logo` VARCHAR(255) NOT NULL default '', 
  `sector` VARCHAR(30)  NOT NULL default '', 
  `company_products` TEXT  NOT NULL default '', 
  `position` VARCHAR(60)  NOT NULL default '', 
  `company_address` VARCHAR(60)  NOT NULL default '', 
  `company_address2` VARCHAR(60)  NOT NULL default '', 
  `company_postal_code` VARCHAR(6)  NOT NULL default '', 
  `company_city` VARCHAR(60)  NOT NULL default '', 
  `company_country` VARCHAR(30)  NOT NULL default '', 
  `company_phone` VARCHAR(60)  NOT NULL default '', 
  `company_email` VARCHAR(60)  NOT NULL default '', 
  `company_website` VARCHAR(60)  NOT NULL default '', 
  `company_fax` VARCHAR(60)  NOT NULL default '', 
  `company_freespace` TEXT  NOT NULL default '', 
  `studies` TEXT  NOT NULL default '', 
  `professional_history` TEXT  NOT NULL default '', 
  `languages` TEXT  NOT NULL default '', 
  `hobbies` TEXT  NOT NULL default '', 
  `history_freespace` TEXT  NOT NULL default '', 
  PRIMARY KEY  (`id`)
,  INDEX  (`id_user`)
,	 FOREIGN KEY (`id_user`) REFERENCES `usersInfo` (`id`) 
	 ON DELETE CASCADE 
	 ON UPDATE CASCADE 
) ENGINE = InnoDB CHARACTER SET utf8 COLLATE utf8_general_ci;

ALTER TABLE `trombi_trombi` ADD INDEX (`nickname`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`lastname`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`firstname`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`birthdate`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`phone`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`email`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`email2`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`site`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`company_name`); 

