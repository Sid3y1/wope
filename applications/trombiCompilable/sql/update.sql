-- Ce fichier doit être modifié pour correspondre aux demandes client.
-- Changer les modify en add si le champ n'existe pas déjà chez le client
-- Effacer aussi ces lignes avant d'envoyer le fichier ;)
SELECT 'A modifier';
quit;
-- START 
ALTER TABLE `trombi_trombi` 
 MODIFY COLUMN   `nickname` VARCHAR(30)  NOT NULL default '', 
 MODIFY COLUMN   `lastname` VARCHAR(30)  NOT NULL default '', 
 MODIFY COLUMN   `firstname` VARCHAR(30)  NOT NULL default '', 
 MODIFY COLUMN   `birthdate` DATE  NOT NULL default '0000-00-00', 
 MODIFY COLUMN   `signal_social` VARCHAR(50)  NOT NULL default '', 
 MODIFY COLUMN   `photo` VARCHAR(255) NOT NULL default '', 
 MODIFY COLUMN   `freespace` TEXT  NOT NULL default '', 
 MODIFY COLUMN   `address` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `address2` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `postal_code` VARCHAR(6)  NOT NULL default '', 
 MODIFY COLUMN   `city` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `phone` VARCHAR(15)  NOT NULL default '', 
 MODIFY COLUMN   `fax` VARCHAR(15)  NOT NULL default '', 
 MODIFY COLUMN   `email` VARCHAR(150)  NOT NULL default '', 
 MODIFY COLUMN   `email2` VARCHAR(150)  NOT NULL default '', 
 MODIFY COLUMN   `site` VARCHAR(30)  NOT NULL default '', 
 MODIFY COLUMN   `company_name` VARCHAR(30)  NOT NULL default '', 
 MODIFY COLUMN   `logo` VARCHAR(255) NOT NULL default '', 
 MODIFY COLUMN   `sector` VARCHAR(30)  NOT NULL default '', 
 MODIFY COLUMN   `company_products` TEXT  NOT NULL default '', 
 MODIFY COLUMN   `position` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `company_address` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `company_address2` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `company_postal_code` VARCHAR(6)  NOT NULL default '', 
 MODIFY COLUMN   `company_city` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `company_country` VARCHAR(30)  NOT NULL default '', 
 MODIFY COLUMN   `company_phone` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `company_email` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `company_website` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `company_fax` VARCHAR(60)  NOT NULL default '', 
 MODIFY COLUMN   `company_freespace` TEXT  NOT NULL default '', 
 MODIFY COLUMN   `studies` TEXT  NOT NULL default '', 
 MODIFY COLUMN   `professional_history` TEXT  NOT NULL default '', 
 MODIFY COLUMN   `languages` TEXT  NOT NULL default '', 
 MODIFY COLUMN   `hobbies` TEXT  NOT NULL default '', 
 MODIFY COLUMN   `history_freespace` TEXT  NOT NULL default '' ; 
ALTER TABLE `trombi_trombi` ADD INDEX (`nickname`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`lastname`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`firstname`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`birthdate`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`phone`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`email`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`email2`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`site`); 

ALTER TABLE `trombi_trombi` ADD INDEX (`company_name`); 

