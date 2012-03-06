-- MySQL dump 10.11
--
-- Host: localhost    Database: wope
-- ------------------------------------------------------
-- Server version	5.0.38-Ubuntu_0ubuntu1-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `bug_report_severity`
--

DROP TABLE IF EXISTS `bug_report_severity`;
CREATE TABLE `bug_report_severity` (
  `severity` varchar(50) NOT NULL,
  PRIMARY KEY  (`severity`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `bug_report_severity`
--

LOCK TABLES `bug_report_severity` WRITE;
/*!40000 ALTER TABLE `bug_report_severity` DISABLE KEYS */;
INSERT INTO `bug_report_severity` VALUES ('Basse'),('Haute'),('Normale');
/*!40000 ALTER TABLE `bug_report_severity` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `bug_report_type`
--

DROP TABLE IF EXISTS `bug_report_type`;
CREATE TABLE `bug_report_type` (
  `type` varchar(50) NOT NULL,
  PRIMARY KEY  (`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `bug_report_type`
--

LOCK TABLES `bug_report_type` WRITE;
/*!40000 ALTER TABLE `bug_report_type` DISABLE KEYS */;
INSERT INTO `bug_report_type` VALUES ('Erreur d\'affichage'),('Erreur Javascript'),('Erreur MySQL'),('Erreur PHP');
/*!40000 ALTER TABLE `bug_report_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `citation`
--

DROP TABLE IF EXISTS `citation`;
CREATE TABLE `citation` (
  `id` int(11) NOT NULL auto_increment,
  `date` date NOT NULL default '0000-00-00',
  `texte` text NOT NULL,
  `auteur` varchar(80) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `citation`
--

LOCK TABLES `citation` WRITE;
/*!40000 ALTER TABLE `citation` DISABLE KEYS */;
/*!40000 ALTER TABLE `citation` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `covoitCarte`
--

DROP TABLE IF EXISTS `covoitCarte`;
CREATE TABLE `covoitCarte` (
  `id` tinyint(3) unsigned NOT NULL auto_increment,
  `nom` varchar(30) NOT NULL default '',
  `article` varchar(5) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=203 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `covoitCarte`
--

LOCK TABLES `covoitCarte` WRITE;
/*!40000 ALTER TABLE `covoitCarte` DISABLE KEYS */;
INSERT INTO `covoitCarte` VALUES (1,'Ain','L\''),(2,'Aisne','L\''),(3,'Allier','L\''),(4,'Alpes de Haute Provence','Les'),(5,'Hautes Alpes','Les'),(6,'Alpes Maritimes','Les'),(7,'Ard','L\''),(8,'Ardennes','Les'),(9,'Ari','L\''),(10,'Aube','L\''),(11,'Aude','L\''),(12,'Aveyron','L\''),(13,'Bouches de Rh','Les'),(14,'Calvados','Le'),(15,'Cantal','Le'),(16,'Charente','La'),(17,'Charente Maritime','La'),(18,'Cher','Le'),(19,'Corr','La'),(23,'Creuse','La'),(24,'Dordogne','La'),(25,'Doubs','Le'),(26,'Drome','La'),(27,'Eure','L\''),(28,'Eure et Loir','L\''),(29,'Finist','Le'),(30,'Gard','Le'),(31,'Haute Garonne','La'),(32,'Gers','Le'),(33,'Gironde','La'),(34,'Herault','L\''),(35,'Ille et Vilaine','L\''),(36,'Indre','L\''),(37,'Indre et Loire','L\''),(38,'Is','L\''),(39,'Jura','Le'),(40,'Landes','Les'),(41,'Loir et Cher','Le'),(42,'Loire','La'),(43,'Haute Loire','La'),(44,'Loire Atlantique','La'),(45,'Loiret','Le'),(46,'Lot','Le'),(47,'Lot et Garonne','Le'),(48,'Loz','La'),(49,'Maine et Loire','Le'),(50,'Manche','La'),(51,'Marne','La'),(52,'Haute Marne','La'),(53,'Mayenne','La'),(54,'Meurthe et Moselle','La'),(55,'Meuse','La'),(56,'Morbihan','Le'),(57,'Moselle','La'),(58,'Ni','La'),(59,'Nord','Le'),(60,'Oise','L\''),(61,'Orme','L\''),(62,'Pas de Calais','Le'),(63,'Puy de D','Le'),(64,'Pyr','Les'),(65,'Hautes Pyr','Les'),(66,'Pyr','Les'),(67,'Bas Rhin','Le'),(68,'Haut Rhin','Le'),(69,'Rh','Le'),(70,'Haute Sa','La'),(71,'Sa','La'),(72,'Sarthe','La'),(73,'Savoie','La'),(74,'Haute Savoie','La'),(75,'Ile de France','L\''),(76,'Seine Maritime','La'),(77,'Seine et Marne','La'),(78,'Yvelines','Les'),(79,'Deux S','Les'),(80,'Somme','La'),(81,'Tarn','Le'),(82,'Tarn et Garonne','Le'),(83,'Var','Le'),(84,'Vaucluse','Le'),(85,'Vend','La'),(86,'Vienne','La'),(87,'Haute Vienne','La'),(88,'Vosges','Les'),(89,'Yonne','L\''),(90,'Territoire de Belfort','Le'),(91,'Essonne','L\''),(92,'Hauts de Seine','Les'),(93,'Seine St Denis','La'),(94,'Val de Marne','Le'),(95,'Val d\'Oise','Le'),(21,'C','La'),(22,'C','Les'),(201,'Basse Corse','La'),(202,'Haute Corse','La');
/*!40000 ALTER TABLE `covoitCarte` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `covoitListe`
--

DROP TABLE IF EXISTS `covoitListe`;
CREATE TABLE `covoitListe` (
  `id` int(5) unsigned NOT NULL auto_increment,
  `type` enum('offre','demande') NOT NULL default 'offre',
  `login` int(11) NOT NULL,
  `tel` varchar(20) default NULL,
  `date` date NOT NULL default '0000-00-00',
  `lieu_depart` varchar(30) default NULL,
  `dpt_depart` tinyint(2) unsigned NOT NULL default '0',
  `lieu_arrivee` varchar(30) default NULL,
  `dpt_arrivee` tinyint(2) unsigned NOT NULL default '0',
  `departements` varchar(50) default NULL,
  `description` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `covoitListe`
--

LOCK TABLES `covoitListe` WRITE;
/*!40000 ALTER TABLE `covoitListe` DISABLE KEYS */;
/*!40000 ALTER TABLE `covoitListe` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `eventsListe`
--

DROP TABLE IF EXISTS `eventsListe`;
CREATE TABLE `eventsListe` (
  `id` smallint(5) unsigned NOT NULL auto_increment,
  `login` int(11) NOT NULL,
  `id_events_type` tinyint(4) NOT NULL default '0' COMMENT 'id of the event type (for tagging)',
  `date` date NOT NULL default '0000-00-00',
  `heure` time NOT NULL,
  `titre` varchar(250) NOT NULL,
  `description` text NOT NULL,
  `auteur` varchar(250) NOT NULL,
  `actif` enum('Y','N') NOT NULL default 'N',
  `lieu` varchar(250) NOT NULL,
  `image` varchar(30) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Events list';

--
-- Dumping data for table `eventsListe`
--

LOCK TABLES `eventsListe` WRITE;
/*!40000 ALTER TABLE `eventsListe` DISABLE KEYS */;
/*!40000 ALTER TABLE `eventsListe` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `faq_content`
--

DROP TABLE IF EXISTS `faq_content`;
CREATE TABLE `faq_content` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `question` varchar(255) NOT NULL,
  `answer` text NOT NULL,
  `active` enum('Y','N') NOT NULL default 'Y',
  `date` date NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `question` (`question`),
  FULLTEXT KEY `answer` (`answer`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `faq_content`
--

LOCK TABLES `faq_content` WRITE;
/*!40000 ALTER TABLE `faq_content` DISABLE KEYS */;
/*!40000 ALTER TABLE `faq_content` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fetes`
--

DROP TABLE IF EXISTS `fetes`;
CREATE TABLE `fetes` (
  `mois` int(2) NOT NULL default '0',
  `jour` int(2) NOT NULL default '0',
  `fete` varchar(30) NOT NULL default '',
  PRIMARY KEY  (`mois`,`jour`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `fetes`
--

LOCK TABLES `fetes` WRITE;
/*!40000 ALTER TABLE `fetes` DISABLE KEYS */;
INSERT INTO `fetes` VALUES (1,1,'Jour de l\'an'),(1,2,'Basile'),(1,3,'Genevi'),(1,4,'Odilon'),(1,5,'Edouard'),(1,6,'M'),(1,7,'Raymond'),(1,8,'Lucien'),(1,9,'Alix'),(1,10,'Guillaume'),(1,11,'Pauline'),(1,12,'Tatiana'),(1,13,'Yvette'),(1,14,'Nina'),(1,15,'R'),(1,16,'Marcel'),(1,17,'Roseline'),(1,18,'Prisca'),(1,19,'Marius'),(1,20,'S'),(1,21,'Agn'),(1,22,'Vincent'),(1,23,'Banard'),(1,24,'Fran'),(1,25,'Conversion de Paul'),(1,26,'Paule'),(1,27,'Ang'),(1,28,'Thomas d\'Aquin'),(1,29,'Gildas'),(1,30,'Martine'),(1,31,'Marcelle'),(2,1,'Ella'),(2,2,'Pr'),(2,3,'Blaise'),(2,4,'V'),(2,5,'Agathe'),(2,6,'Gaston'),(2,7,'Eug'),(2,8,'Jacqueline'),(2,9,'Apolline'),(2,10,'Arnaud'),(2,11,'Notre Dame de Lourdes'),(2,12,'F'),(2,13,'B'),(2,14,'Valentin'),(2,15,'Claude'),(2,16,'Julienne'),(2,17,'Alexis'),(2,18,'Bernadette'),(2,19,'Gabin'),(2,20,'Aim'),(2,21,'Damien'),(2,22,'Isabelle'),(2,23,'Lazare'),(2,24,'Modeste'),(2,25,'Rom'),(2,26,'Nestor'),(2,27,'Honorine'),(2,28,'Romain'),(2,29,'Auguste'),(3,1,'Aubin'),(3,2,'Charles le Bon'),(3,3,'Gu'),(3,4,'Casimir'),(3,5,'Olive'),(3,6,'Colette'),(3,7,'F'),(3,8,'Jean de Dieu'),(3,9,'Fran'),(3,10,'Vivien'),(3,11,'Rosine'),(3,12,'Justine'),(3,13,'Rodrigue'),(3,14,'Mathilde'),(3,15,'Louise'),(3,16,'B'),(3,17,'Patrice'),(3,18,'Cyrille'),(3,19,'Joseph'),(3,20,'Printemps'),(3,21,'Cl'),(3,22,'L'),(3,23,'Victorien'),(3,24,'Catherine 1'),(3,25,'Annonciation'),(3,26,'Larissa'),(3,27,'Habib'),(3,28,'Gontran'),(3,29,'Gwladys'),(3,30,'Am'),(3,31,'Benjamin'),(4,1,'Hugues'),(4,2,'Sandrine'),(4,3,'Richard'),(4,4,'Isidore'),(4,5,'Ir'),(4,6,'Marcellin'),(4,7,'Jean-Baptiste de la Salle'),(4,8,'Julie'),(4,9,'Gautier'),(4,10,'Fulbert'),(4,11,'Stanislas'),(4,12,'Jules'),(4,13,'Ida'),(4,14,'Maxime'),(4,15,'Paterne'),(4,16,'Beno'),(4,17,'Anicet'),(4,18,'Parfait'),(4,19,'Emma'),(4,20,'Odette'),(4,21,'Anselme'),(4,22,'Alexandre'),(4,23,'Georges'),(4,24,'Fid'),(4,25,'Marc'),(4,26,'Alida'),(4,27,'Zita'),(4,28,'Val'),(4,29,'Catherine de Sienne'),(4,30,'Robert'),(5,1,'F'),(5,2,'Boris'),(5,3,'Philippe - Jacques'),(5,4,'Sylvain'),(5,5,'Judith'),(5,6,'Prudence'),(5,7,'Gis'),(5,8,'Armistice 1945'),(5,9,'Pac'),(5,10,'Solange'),(5,11,'Estelle'),(5,12,'Achille'),(5,13,'Rolande'),(5,14,'Matthias'),(5,15,'Denise'),(5,16,'Honor'),(5,17,'Pascal'),(5,18,'Eric'),(5,19,'Yves'),(5,20,'Bernardin'),(5,21,'Constantin'),(5,22,'Emile'),(5,23,'Didier'),(5,24,'Donatien'),(5,25,'Sophie'),(5,26,'B'),(5,27,'Augustin 1'),(5,28,'Germain'),(5,29,'Aymar'),(5,30,'Ferdinand'),(5,31,'Visitation de la Sainte Vierge'),(6,1,'Justin'),(6,2,'Blandine'),(6,3,'K'),(6,4,'Clotilde'),(6,5,'Igor'),(6,6,'Norbert'),(6,7,'Gilbert'),(6,8,'M'),(6,9,'Diane'),(6,10,'Landry'),(6,11,'Barnab'),(6,12,'Guy'),(6,13,'Antoine de Padoue'),(6,14,'Elis'),(6,15,'Germaine'),(6,16,'Jean Fran'),(6,17,'Herv'),(6,18,'L'),(6,19,'Romuald'),(6,20,'Silv'),(6,21,'Et'),(6,22,'Alban'),(6,23,'Audrey'),(6,24,'Jean-Baptiste'),(6,25,'Prosper'),(6,26,'Anthelme'),(6,27,'Fernand'),(6,28,'Ir'),(6,29,'Pierre - Paul'),(6,30,'Martial'),(7,1,'Thierry'),(7,2,'Martinien'),(7,3,'Thomas'),(7,4,'Florent'),(7,5,'Antoine'),(7,6,'Mariette'),(7,7,'Raoul'),(7,8,'Thibault'),(7,9,'Amandine'),(7,10,'Ulrich'),(7,11,'Beno'),(7,12,'Olivier'),(7,13,'Henri et Jo'),(7,14,'F'),(7,15,'Donald'),(7,16,'Nte Dame Mt Carmel'),(7,17,'Charlotte'),(7,18,'Fr'),(7,19,'Ars'),(7,20,'Marina'),(7,21,'Victor'),(7,22,'Marie Madeleine'),(7,23,'Brigitte'),(7,24,'Christine'),(7,25,'Jacques'),(7,26,'Anne et Joachin'),(7,27,'Nathalie'),(7,28,'Samson'),(7,29,'Marthe'),(7,30,'Juliette'),(7,31,'Ignace de Loyola'),(8,1,'Alphonse'),(8,2,'Julien Eymard'),(8,3,'Lydie'),(8,4,'Jean-Marie Vianney'),(8,5,'Abel'),(8,6,'Transfiguration'),(8,7,'Ga'),(8,8,'Dominique'),(8,9,'Amour'),(8,10,'Laurent'),(8,11,'Claire'),(8,12,'Clarisse'),(8,13,'Hippolyte'),(8,14,'Evrard'),(8,15,'Assomption'),(8,16,'Armel'),(8,17,'Hyacinthe'),(8,18,'H'),(8,19,'Jean Eudes'),(8,20,'Bernard'),(8,21,'Christophe'),(8,22,'Fabrice'),(8,23,'Rose de Lima'),(8,24,'Barth'),(8,25,'Louis'),(8,26,'Natacha'),(8,27,'Monique'),(8,28,'Augustin 2'),(8,29,'Sabine'),(8,30,'Fiacre'),(8,31,'Aristide'),(9,1,'Gilles'),(9,2,'Ingrid'),(9,3,'Gr'),(9,4,'Rosalie'),(9,5,'Ra'),(9,6,'Bertrand'),(9,7,'Reine'),(9,8,'Nativit'),(9,9,'Alain'),(9,10,'In'),(9,11,'Adelphe'),(9,12,'Apollinaire'),(9,13,'Aim'),(9,14,'Croix Glorieuse'),(9,15,'Roland'),(9,16,'Edith'),(9,17,'Renaud'),(9,18,'Nad'),(9,19,'Emilie'),(9,20,'Davy'),(9,21,'Matthieu'),(9,22,'Maurice'),(9,23,'Automne'),(9,24,'Th'),(9,25,'Hermann'),(9,26,'C'),(9,27,'Vincent de Paul'),(9,28,'Venceslas'),(9,29,'Michel - Gabriel - Rapha'),(9,30,'J'),(10,1,'Th'),(10,2,'L'),(10,3,'G'),(10,4,'Fran'),(10,5,'Fleur'),(10,6,'Bruno'),(10,7,'Serge'),(10,8,'P'),(10,9,'Denis'),(10,10,'Ghislain'),(10,11,'Firmin'),(10,12,'Wilfried'),(10,13,'G'),(10,14,'Juste'),(10,15,'Th'),(10,16,'Edwige'),(10,17,'Baudoin'),(10,18,'Luc'),(10,19,'Ren'),(10,20,'Adeline'),(10,21,'C'),(10,22,'Elodie'),(10,23,'Jean de Capistran'),(10,24,'Florentin'),(10,25,'Cr'),(10,26,'Dimitri'),(10,27,'Emeline'),(10,28,'Jude'),(10,29,'Narcisse'),(10,30,'Bienvenue'),(10,31,'Quentin'),(11,1,'Toussaint'),(11,2,'D'),(11,3,'Hubert'),(11,4,'Charles'),(11,5,'Sylvie'),(11,6,'Bertille'),(11,7,'Carine'),(11,8,'Geoffroy'),(11,9,'Th'),(11,10,'L'),(11,11,'Armistice 1918'),(11,12,'Christian'),(11,13,'Brice'),(11,14,'Sidoine'),(11,15,'Albert'),(11,16,'Marguerite'),(11,17,'Elisabeth'),(11,18,'Aude'),(11,19,'Tanguy'),(11,20,'Edmond'),(11,21,'Pr'),(11,22,'C'),(11,23,'Cl'),(11,24,'Flora'),(11,25,'Catherine 2'),(11,26,'Delphine'),(11,27,'S'),(11,28,'Jacques de la Marche'),(11,29,'Saturnin'),(11,30,'Andr'),(12,1,'Florence'),(12,2,'Viviane'),(12,3,'Fran'),(12,4,'Barbara'),(12,5,'G'),(12,6,'Nicolas'),(12,7,'Ambroise'),(12,8,'Immacul'),(12,9,'Pierre Fourier'),(12,10,'Romaric'),(12,11,'Daniel'),(12,12,'Jeanne-Fran'),(12,13,'Lucie'),(12,14,'Odile'),(12,15,'Ninon'),(12,16,'Alice'),(12,17,'Ga'),(12,18,'Gatien'),(12,19,'Urbain'),(12,20,'Th'),(12,21,'Hivers'),(12,22,'Fran'),(12,23,'Armand'),(12,24,'Ad'),(12,25,'No'),(12,26,'Etienne'),(12,27,'Jean'),(12,28,'Innocents'),(12,29,'David'),(12,30,'Roger'),(12,31,'Sylvestre');
/*!40000 ALTER TABLE `fetes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `forum_abus`
--

DROP TABLE IF EXISTS `forum_abus`;
CREATE TABLE `forum_abus` (
  `id_abus` int(22) NOT NULL auto_increment,
  `id_message` int(22) NOT NULL default '0',
  `login` int(11) NOT NULL,
  `date` datetime NOT NULL default '0000-00-00 00:00:00',
  `com` text,
  `is_read` varchar(20) NOT NULL default 'false',
  PRIMARY KEY  (`id_abus`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `forum_abus`
--

LOCK TABLES `forum_abus` WRITE;
/*!40000 ALTER TABLE `forum_abus` DISABLE KEYS */;
/*!40000 ALTER TABLE `forum_abus` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `forum_draft`
--

DROP TABLE IF EXISTS `forum_draft`;
CREATE TABLE `forum_draft` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `id_user` int(10) unsigned NOT NULL,
  `id_thread` int(10) unsigned NOT NULL,
  `subject` varchar(255) NOT NULL,
  `message` text NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `forum_draft`
--

LOCK TABLES `forum_draft` WRITE;
/*!40000 ALTER TABLE `forum_draft` DISABLE KEYS */;
/*!40000 ALTER TABLE `forum_draft` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `forum_login`
--

DROP TABLE IF EXISTS `forum_login`;
CREATE TABLE `forum_login` (
  `login` int(11) NOT NULL,
  `thread` int(11) NOT NULL default '0',
  `id_last_msg` int(11) NOT NULL default '0',
  `mail` enum('true','false') NOT NULL default 'false',
  `site` enum('true','false') NOT NULL default 'false',
  PRIMARY KEY  (`login`,`thread`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `forum_login`
--

LOCK TABLES `forum_login` WRITE;
/*!40000 ALTER TABLE `forum_login` DISABLE KEYS */;
/*!40000 ALTER TABLE `forum_login` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `forum_message`
--

DROP TABLE IF EXISTS `forum_message`;
CREATE TABLE `forum_message` (
  `id` int(22) NOT NULL auto_increment,
  `id_thread` int(11) NOT NULL default '0',
  `login` int(11) NOT NULL,
  `sujet` varchar(255) default NULL,
  `message` text,
  `surnom` varchar(255) default NULL,
  `date` datetime NOT NULL default '0000-00-00 00:00:00',
  `score` int(11) NOT NULL default '0',
  `host` varchar(255) NOT NULL default '',
  `uniqid` varchar(114) NOT NULL default '',
  `email` varchar(250) NOT NULL default '',
  `avatar` int(13) default NULL COMMENT 'adresse de l''avatar de l''auteur du message',
  `signature` text COMMENT 'Signature of the author',
  `myavatar` varchar(20) NOT NULL default 'false',
  `file` int(10) unsigned default NULL,
  PRIMARY KEY  (`id`),
  KEY `id_thread` (`id_thread`),
  KEY `login` (`login`),
  KEY `uniqid` (`uniqid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `forum_message`
--

LOCK TABLES `forum_message` WRITE;
/*!40000 ALTER TABLE `forum_message` DISABLE KEYS */;
/*!40000 ALTER TABLE `forum_message` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `forum_modo`
--

DROP TABLE IF EXISTS `forum_modo`;
CREATE TABLE `forum_modo` (
  `id` int(11) NOT NULL auto_increment,
  `id_message` int(11) NOT NULL default '0',
  `sujet` varchar(30) NOT NULL default '',
  `message` longtext NOT NULL,
  `surnom` varchar(30) NOT NULL default '',
  `email` varchar(40) NOT NULL default '',
  `com` longtext NOT NULL,
  `login_modo` int(11) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `forum_modo`
--

LOCK TABLES `forum_modo` WRITE;
/*!40000 ALTER TABLE `forum_modo` DISABLE KEYS */;
/*!40000 ALTER TABLE `forum_modo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `forum_salon`
--

DROP TABLE IF EXISTS `forum_salon`;
CREATE TABLE `forum_salon` (
  `id` int(11) NOT NULL auto_increment,
  `nom` varchar(255) NOT NULL default '',
  `descriptif` text NOT NULL,
  `droit_lecture` varchar(255) NOT NULL default '',
  `droit_ecriture` varchar(255) NOT NULL default '',
  `droit_moderation` varchar(255) NOT NULL default '',
  `droit_attachement` varchar(255) NOT NULL default '',
  `uniqid` varchar(255) NOT NULL default '',
  `nb_msg` int(10) unsigned NOT NULL,
  `nb_thread` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `forum_salon`
--

LOCK TABLES `forum_salon` WRITE;
/*!40000 ALTER TABLE `forum_salon` DISABLE KEYS */;
/*!40000 ALTER TABLE `forum_salon` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `forum_thread`
--

DROP TABLE IF EXISTS `forum_thread`;
CREATE TABLE `forum_thread` (
  `id` int(11) NOT NULL auto_increment,
  `nom_thread` varchar(255) NOT NULL default '',
  `login` int(10) unsigned NOT NULL,
  `createur` varchar(255) NOT NULL default '',
  `date_creation` datetime NOT NULL default '0000-00-00 00:00:00',
  `dernier_login` int(10) unsigned NOT NULL,
  `dernier_poste` varchar(255) NOT NULL default '',
  `date_dernier_poste` datetime NOT NULL default '0000-00-00 00:00:00',
  `sticky` set('true','false') NOT NULL default 'false',
  `locked` set('true','false') NOT NULL default 'false',
  `drop` set('true','false') NOT NULL default 'false',
  `id_salon` int(11) NOT NULL default '0',
  `test` int(10) unsigned NOT NULL,
  `nb_msg` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `forum_thread`
--

LOCK TABLES `forum_thread` WRITE;
/*!40000 ALTER TABLE `forum_thread` DISABLE KEYS */;
/*!40000 ALTER TABLE `forum_thread` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_file_manager_files`
--

DROP TABLE IF EXISTS `kernel_file_manager_files`;
CREATE TABLE `kernel_file_manager_files` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(250) collate utf8_bin NOT NULL,
  `size` int(10) unsigned NOT NULL,
  `filetype` varchar(100) collate utf8_bin NOT NULL,
  `right_r` varchar(50) collate utf8_bin NOT NULL,
  `right_w` varchar(50) collate utf8_bin NOT NULL,
  `searchable` tinyint(1) NOT NULL,
  `old_id` int(10) unsigned NOT NULL,
  `login` int(11) NOT NULL,
  `description` text collate utf8_bin NOT NULL,
  `date` datetime NOT NULL,
  `application` varchar(50) collate utf8_bin NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Dumping data for table `kernel_file_manager_files`
--

LOCK TABLES `kernel_file_manager_files` WRITE;
/*!40000 ALTER TABLE `kernel_file_manager_files` DISABLE KEYS */;
INSERT INTO `kernel_file_manager_files` VALUES (6,'grumly.jpg',15616,'image/jpeg','log','log',1,0,3,'','2007-04-17 19:05:47','unknow');
/*!40000 ALTER TABLE `kernel_file_manager_files` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_file_manager_quotas`
--

DROP TABLE IF EXISTS `kernel_file_manager_quotas`;
CREATE TABLE `kernel_file_manager_quotas` (
  `quota` int(11) NOT NULL,
  `login` int(11) NOT NULL,
  PRIMARY KEY  (`login`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_file_manager_quotas`
--

LOCK TABLES `kernel_file_manager_quotas` WRITE;
/*!40000 ALTER TABLE `kernel_file_manager_quotas` DISABLE KEYS */;
INSERT INTO `kernel_file_manager_quotas` VALUES (2147483647,3);
/*!40000 ALTER TABLE `kernel_file_manager_quotas` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_module_available`
--

DROP TABLE IF EXISTS `kernel_module_available`;
CREATE TABLE `kernel_module_available` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `id_module` int(10) unsigned NOT NULL,
  `right` smallint(5) unsigned NOT NULL,
  `strict` enum('Y','N') NOT NULL default 'N' COMMENT 'Checks the right strictly or takes account of the heritage',
  PRIMARY KEY  (`id`),
  KEY `id_module` (`id_module`),
  KEY `right` (`right`),
  CONSTRAINT `kernel_module_available_ibfk_1` FOREIGN KEY (`id_module`) REFERENCES `kernel_module_module` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_module_available_ibfk_2` FOREIGN KEY (`right`) REFERENCES `kernel_right` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_module_available`
--

LOCK TABLES `kernel_module_available` WRITE;
/*!40000 ALTER TABLE `kernel_module_available` DISABLE KEYS */;
INSERT INTO `kernel_module_available` VALUES (60,1,3,'N'),(63,4,1,'N'),(65,6,1,'N'),(66,7,3,'N'),(67,8,3,'N'),(68,9,3,'N'),(70,11,3,'N'),(71,12,3,'N'),(72,13,3,'N'),(73,14,1,'N'),(77,18,2,'N'),(79,20,2,'N'),(81,22,1,'N'),(84,25,1,'Y'),(86,2,2,'N'),(87,21,2,'N'),(89,15,2,'N'),(90,26,3,'N'),(91,5,1,'N'),(94,10,2,'N');
/*!40000 ALTER TABLE `kernel_module_available` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_module_column`
--

DROP TABLE IF EXISTS `kernel_module_column`;
CREATE TABLE `kernel_module_column` (
  `id` smallint(5) unsigned NOT NULL auto_increment,
  `title` varchar(50) NOT NULL,
  `default_column` enum('Y','N') NOT NULL default 'N',
  `id_tab` smallint(5) unsigned default NULL,
  `position` smallint(5) unsigned NOT NULL,
  `width` smallint(5) unsigned NOT NULL default '0',
  `available` enum('Y','N') NOT NULL default 'N',
  `mandatory` enum('Y','N') NOT NULL default 'N',
  `user_column` enum('Y','N') NOT NULL default 'N',
  `drag` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`id`),
  KEY `id_tab` (`id_tab`),
  CONSTRAINT `kernel_module_column_ibfk_1` FOREIGN KEY (`id_tab`) REFERENCES `kernel_module_tab` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_module_column`
--

LOCK TABLES `kernel_module_column` WRITE;
/*!40000 ALTER TABLE `kernel_module_column` DISABLE KEYS */;
INSERT INTO `kernel_module_column` VALUES (1,'Mandatory column','Y',1,1,246,'Y','Y','N','N'),(2,'User column','Y',1,2,522,'Y','Y','Y','Y');
/*!40000 ALTER TABLE `kernel_module_column` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_module_mandatory`
--

DROP TABLE IF EXISTS `kernel_module_mandatory`;
CREATE TABLE `kernel_module_mandatory` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `id_module` int(10) unsigned NOT NULL,
  `right` smallint(5) unsigned NOT NULL,
  `strict` enum('Y','N') NOT NULL default 'N' COMMENT 'Checks the right strictly or takes account of the heritage',
  PRIMARY KEY  (`id`),
  KEY `id_module` (`id_module`),
  KEY `right` (`right`),
  CONSTRAINT `kernel_module_mandatory_ibfk_1` FOREIGN KEY (`id_module`) REFERENCES `kernel_module_module` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_module_mandatory_ibfk_2` FOREIGN KEY (`right`) REFERENCES `kernel_right` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_module_mandatory`
--

LOCK TABLES `kernel_module_mandatory` WRITE;
/*!40000 ALTER TABLE `kernel_module_mandatory` DISABLE KEYS */;
INSERT INTO `kernel_module_mandatory` VALUES (8,14,1,'N'),(9,18,2,'N'),(13,25,1,'Y'),(15,15,2,'N'),(16,26,3,'N'),(17,5,2,'N');
/*!40000 ALTER TABLE `kernel_module_mandatory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_module_module`
--

DROP TABLE IF EXISTS `kernel_module_module`;
CREATE TABLE `kernel_module_module` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `class` varchar(50) NOT NULL,
  `title` varchar(50) NOT NULL,
  `description` varchar(255) default NULL,
  `instantiable` enum('Y','N') NOT NULL default 'N' COMMENT 'Can several modules be instantiated at the same time ?',
  `admin_available` enum('Y','N') NOT NULL default 'N',
  `default_module` enum('Y','N') NOT NULL default 'N' COMMENT 'Does the module appear when an user has no module ?',
  `default_theme` smallint(5) unsigned NOT NULL default '24' COMMENT 'Theme of the module when `default_module` is set to ''Y'' or module is mandatory',
  `default_column` smallint(5) unsigned default NULL COMMENT 'Column of the module when `default_module` is set to ''Y'' or module is mandatory',
  `default_position` smallint(5) unsigned default NULL COMMENT 'Position of the module when `default_module` is set to ''Y'' or module is mandatory',
  PRIMARY KEY  (`id`),
  KEY `default_column` (`default_column`),
  CONSTRAINT `kernel_module_module_ibfk_1` FOREIGN KEY (`default_column`) REFERENCES `kernel_module_column` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_module_module`
--

LOCK TABLES `kernel_module_module` WRITE;
/*!40000 ALTER TABLE `kernel_module_module` DISABLE KEYS */;
INSERT INTO `kernel_module_module` VALUES (1,'module_trombi','Trombinoscope',NULL,'Y','N','N',24,2,NULL),(2,'module_notes','Calepin','','N','N','Y',24,2,1),(4,'module_meteo','Meteo',NULL,'Y','N','N',15,2,NULL),(5,'module_mpub','Publicit&eacute;','','N','Y','N',24,1,6),(6,'module_tele','TÃ©lÃ©vision',NULL,'Y','N','N',24,2,NULL),(7,'module_agenda','Agenda',NULL,'N','Y','N',24,2,NULL),(8,'module_bottin','Bottin',NULL,'Y','N','N',24,2,NULL),(9,'module_news','ActualitÃ©s',NULL,'N','Y','N',24,2,NULL),(10,'module_forum','Forum','','N','N','N',24,2,NULL),(11,'module_recherche','Recherche externe',NULL,'Y','N','N',10,2,NULL),(12,'module_event','Informations',NULL,'Y','Y','N',24,2,NULL),(13,'module_traduction','Traduction',NULL,'Y','N','N',20,2,NULL),(14,'module_identification','Identification',NULL,'N','N','N',24,1,1),(15,'module_sommaire','Sommaire','','N','Y','N',24,1,4),(18,'module_add_module','InsÃ©rer un module :',NULL,'N','N','N',24,1,2),(20,'module_links','Liens utiles',NULL,'Y','Y','N',24,2,NULL),(21,'module_todolist','Todo List','','N','N','Y',24,2,2),(22,'module_cinema','CinÃ©ma',NULL,'Y','N','N',24,2,NULL),(25,'module_welcome','Bienvenue',NULL,'Y','Y','Y',24,2,1),(26,'module_survey','Sondages','','N','Y','N',24,1,5);
/*!40000 ALTER TABLE `kernel_module_module` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_module_tab`
--

DROP TABLE IF EXISTS `kernel_module_tab`;
CREATE TABLE `kernel_module_tab` (
  `id` smallint(5) unsigned NOT NULL auto_increment,
  `title` varchar(50) default NULL,
  `default_tab` enum('Y','N') NOT NULL default 'N',
  `default_position` smallint(6) NOT NULL,
  `available` enum('Y','N') NOT NULL default 'N',
  `mandatory` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_module_tab`
--

LOCK TABLES `kernel_module_tab` WRITE;
/*!40000 ALTER TABLE `kernel_module_tab` DISABLE KEYS */;
INSERT INTO `kernel_module_tab` VALUES (1,'Main tab','Y',1,'Y','Y');
/*!40000 ALTER TABLE `kernel_module_tab` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_module_user_column`
--

DROP TABLE IF EXISTS `kernel_module_user_column`;
CREATE TABLE `kernel_module_user_column` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `id_user` int(10) unsigned NOT NULL,
  `id_column` smallint(5) unsigned NOT NULL,
  `id_tab` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `id_tab` (`id_tab`),
  KEY `id_column` (`id_column`),
  KEY `id_user` (`id_user`),
  CONSTRAINT `kernel_module_user_column_ibfk_1` FOREIGN KEY (`id_tab`) REFERENCES `kernel_module_user_tab` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_module_user_column_ibfk_2` FOREIGN KEY (`id_column`) REFERENCES `kernel_module_column` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_module_user_column_ibfk_3` FOREIGN KEY (`id_user`) REFERENCES `usersInfo` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_module_user_column`
--

LOCK TABLES `kernel_module_user_column` WRITE;
/*!40000 ALTER TABLE `kernel_module_user_column` DISABLE KEYS */;
INSERT INTO `kernel_module_user_column` VALUES (13,6,1,7),(14,6,2,7),(15,1,1,8),(16,1,2,8),(17,9,1,9),(18,9,2,9),(19,3,1,10),(20,3,2,10),(21,65,1,11),(22,65,2,11),(23,68,1,12),(24,68,2,12);
/*!40000 ALTER TABLE `kernel_module_user_column` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_module_user_module`
--

DROP TABLE IF EXISTS `kernel_module_user_module`;
CREATE TABLE `kernel_module_user_module` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `id_user` int(10) unsigned NOT NULL,
  `id_module` int(10) unsigned NOT NULL,
  `name` varchar(50) default NULL,
  `theme` smallint(5) unsigned NOT NULL default '24',
  `id_column` int(10) unsigned NOT NULL,
  `position` smallint(5) unsigned NOT NULL,
  `minimized` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`id`),
  KEY `id_module` (`id_module`),
  KEY `id_column` (`id_column`),
  KEY `id_user` (`id_user`),
  CONSTRAINT `kernel_module_user_module_ibfk_1` FOREIGN KEY (`id_module`) REFERENCES `kernel_module_module` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_module_user_module_ibfk_2` FOREIGN KEY (`id_column`) REFERENCES `kernel_module_user_column` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_module_user_module_ibfk_3` FOREIGN KEY (`id_user`) REFERENCES `usersInfo` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_module_user_module`
--

LOCK TABLES `kernel_module_user_module` WRITE;
/*!40000 ALTER TABLE `kernel_module_user_module` DISABLE KEYS */;
INSERT INTO `kernel_module_user_module` VALUES (44,6,2,NULL,24,14,4,'N'),(46,6,14,NULL,24,13,1,'N'),(47,6,18,NULL,24,13,2,'N'),(48,6,15,NULL,24,13,4,'N'),(49,6,26,NULL,24,13,6,'N'),(50,6,5,NULL,24,13,7,'N'),(51,1,25,NULL,24,16,1,'N'),(52,1,14,NULL,24,15,1,'N'),(71,9,2,NULL,24,18,3,'N'),(72,9,21,NULL,24,18,4,'N'),(73,9,14,NULL,24,17,1,'N'),(74,9,18,NULL,24,17,2,'N'),(75,9,15,NULL,24,17,5,'N'),(76,9,26,NULL,24,17,6,'N'),(77,9,5,NULL,24,17,7,'N'),(78,9,4,NULL,15,18,5,'N'),(79,3,2,NULL,24,20,10,'N'),(80,3,21,NULL,24,20,11,'N'),(81,3,14,NULL,24,19,1,'N'),(82,3,18,NULL,24,19,2,'N'),(83,3,15,NULL,24,19,3,'N'),(84,3,26,NULL,24,19,4,'N'),(85,3,5,NULL,24,19,5,'N'),(90,9,22,NULL,24,18,2,'N'),(108,65,2,NULL,24,22,1,'N'),(109,65,21,NULL,24,22,2,'N'),(110,65,5,NULL,24,21,5,'N'),(111,65,14,NULL,24,21,1,'N'),(112,65,15,NULL,24,21,4,'N'),(113,65,18,NULL,24,21,2,'N'),(115,68,2,NULL,24,24,1,'N'),(116,68,21,NULL,24,24,2,'N'),(117,68,5,NULL,24,23,5,'N'),(118,68,14,NULL,24,23,1,'N'),(119,68,15,NULL,24,23,4,'N'),(120,68,18,NULL,24,23,2,'N'),(136,6,1,NULL,24,14,3,'N'),(139,9,10,NULL,24,18,1,'N'),(140,6,9,NULL,24,14,2,'N'),(141,6,7,NULL,24,14,1,'N'),(142,3,9,NULL,24,20,9,'N'),(143,3,7,NULL,24,20,8,'N'),(144,3,8,NULL,24,20,7,'N'),(148,3,22,NULL,24,20,6,'N'),(149,3,10,NULL,24,20,5,'N'),(150,3,12,NULL,24,20,4,'N'),(151,3,20,NULL,24,20,3,'N'),(154,3,13,NULL,20,20,2,'N'),(157,3,4,NULL,15,20,1,'N');
/*!40000 ALTER TABLE `kernel_module_user_module` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `kernel_module_user_module_available`
--

DROP TABLE IF EXISTS `kernel_module_user_module_available`;
/*!50001 DROP VIEW IF EXISTS `kernel_module_user_module_available`*/;
/*!50001 CREATE TABLE `kernel_module_user_module_available` (
  `idUser` int(10) unsigned,
  `id` int(10) unsigned,
  `title` varchar(50),
  `instantiable` enum('Y','N')
) */;

--
-- Temporary table structure for view `kernel_module_user_module_final`
--

DROP TABLE IF EXISTS `kernel_module_user_module_final`;
/*!50001 DROP VIEW IF EXISTS `kernel_module_user_module_final`*/;
/*!50001 CREATE TABLE `kernel_module_user_module_final` (
  `idUser` int(11) unsigned,
  `id` bigint(20) unsigned,
  `position` bigint(20) unsigned,
  `idModule` bigint(20) unsigned,
  `class` varchar(50),
  `title` varchar(50),
  `instantiable` varchar(1),
  `name` varchar(50),
  `theme` bigint(20) unsigned,
  `minimized` varchar(1),
  `mandatory` varchar(1),
  `column` int(11) unsigned,
  `columnMandatory` varchar(1),
  `columnWidth` smallint(6) unsigned,
  `tab` int(11) unsigned,
  `inUserColumn` varchar(1),
  `drag` varchar(1),
  `columnPosition` smallint(6) unsigned
) */;

--
-- Table structure for table `kernel_module_user_tab`
--

DROP TABLE IF EXISTS `kernel_module_user_tab`;
CREATE TABLE `kernel_module_user_tab` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `id_user` int(10) unsigned NOT NULL,
  `id_tab` smallint(5) unsigned NOT NULL,
  `name` varchar(50) default NULL,
  `position` smallint(6) NOT NULL,
  `columns` smallint(6) NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `id_tab` (`id_tab`),
  KEY `id_user` (`id_user`),
  CONSTRAINT `kernel_module_user_tab_ibfk_1` FOREIGN KEY (`id_tab`) REFERENCES `kernel_module_tab` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_module_user_tab_ibfk_2` FOREIGN KEY (`id_user`) REFERENCES `usersInfo` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_module_user_tab`
--

LOCK TABLES `kernel_module_user_tab` WRITE;
/*!40000 ALTER TABLE `kernel_module_user_tab` DISABLE KEYS */;
INSERT INTO `kernel_module_user_tab` VALUES (7,6,1,NULL,1,2),(8,1,1,NULL,1,2),(9,9,1,NULL,1,2),(10,3,1,NULL,1,2),(11,65,1,NULL,1,2),(12,68,1,NULL,1,2);
/*!40000 ALTER TABLE `kernel_module_user_tab` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_right`
--

DROP TABLE IF EXISTS `kernel_right`;
CREATE TABLE `kernel_right` (
  `id` smallint(5) unsigned NOT NULL auto_increment,
  `name` varchar(25) NOT NULL default '',
  `description` varchar(30) NOT NULL default '',
  `parent` smallint(5) unsigned default NULL,
  `lft` int(10) unsigned NOT NULL default '0',
  `rgt` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `parent` (`parent`),
  CONSTRAINT `kernel_right_ibfk_1` FOREIGN KEY (`parent`) REFERENCES `kernel_right` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Use the storred procedure to modify the data in this table';

--
-- Dumping data for table `kernel_right`
--

LOCK TABLES `kernel_right` WRITE;
/*!40000 ALTER TABLE `kernel_right` DISABLE KEYS */;
INSERT INTO `kernel_right` VALUES (1,'all','',NULL,1,46),(2,'log','',1,12,45),(3,'user','Droit de l\'utilisateur',2,27,44),(5,'ext','',2,15,16),(6,'ban_forum','Login bannis du forum',2,13,14),(7,'ban_site','Banni du sitea',2,17,18),(9,'admin','Administrateur (Client) du sit',10,35,40),(10,'webmaster','Droit qui permet la crÃ©ation ',3,34,41),(11,'moderateur','Droit de moderation forum',3,28,31),(12,'admin_droits','Gestion des droits',9,36,39),(13,'super_admin','Acces Ã   tout',12,42,43);
/*!40000 ALTER TABLE `kernel_right` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `kernel_right_user`
--

DROP TABLE IF EXISTS `kernel_right_user`;
/*!50001 DROP VIEW IF EXISTS `kernel_right_user`*/;
/*!50001 CREATE TABLE `kernel_right_user` (
  `login` int(10) unsigned,
  `idRight` smallint(5) unsigned
) */;

--
-- Table structure for table `kernel_right_user_get`
--

DROP TABLE IF EXISTS `kernel_right_user_get`;
CREATE TABLE `kernel_right_user_get` (
  `login` int(10) unsigned NOT NULL,
  `idRight` smallint(5) unsigned NOT NULL,
  PRIMARY KEY  (`login`,`idRight`),
  KEY `login` (`login`),
  KEY `idRight` (`idRight`),
  CONSTRAINT `kernel_right_user_get_ibfk_1` FOREIGN KEY (`login`) REFERENCES `usersInfo` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_right_user_get_ibfk_2` FOREIGN KEY (`idRight`) REFERENCES `kernel_right` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_right_user_get`
--

LOCK TABLES `kernel_right_user_get` WRITE;
/*!40000 ALTER TABLE `kernel_right_user_get` DISABLE KEYS */;
INSERT INTO `kernel_right_user_get` VALUES (1,1),(3,3),(4,3),(5,3),(6,3),(7,3),(8,3),(9,3),(10,3),(11,3),(13,3),(15,3),(21,3),(97,3),(3,5),(62,5),(63,5),(64,5),(65,5),(68,5),(69,5),(70,5),(71,5),(72,5),(73,5),(74,5),(3,9),(6,9),(9,9),(12,9),(3,11),(6,11),(9,11),(12,11),(3,12),(12,12),(3,13),(6,13),(9,13);
/*!40000 ALTER TABLE `kernel_right_user_get` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_right_user_set`
--

DROP TABLE IF EXISTS `kernel_right_user_set`;
CREATE TABLE `kernel_right_user_set` (
  `login` int(10) unsigned NOT NULL,
  `idRight` smallint(5) unsigned NOT NULL,
  `expire` date default NULL,
  PRIMARY KEY  (`login`,`idRight`),
  KEY `login` (`login`),
  KEY `idRight` (`idRight`),
  CONSTRAINT `kernel_right_user_set_ibfk_1` FOREIGN KEY (`login`) REFERENCES `usersInfo` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_right_user_set_ibfk_2` FOREIGN KEY (`idRight`) REFERENCES `kernel_right` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_right_user_set`
--

LOCK TABLES `kernel_right_user_set` WRITE;
/*!40000 ALTER TABLE `kernel_right_user_set` DISABLE KEYS */;
/*!40000 ALTER TABLE `kernel_right_user_set` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_tag_alias_notify`
--

DROP TABLE IF EXISTS `kernel_tag_alias_notify`;
CREATE TABLE `kernel_tag_alias_notify` (
  `idTag1` int(10) unsigned NOT NULL,
  `idTag2` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`idTag1`,`idTag2`),
  KEY `idTag1` (`idTag1`),
  KEY `idTag2` (`idTag2`),
  CONSTRAINT `kernel_tag_alias_notify_ibfk_1` FOREIGN KEY (`idTag1`) REFERENCES `kernel_tag_liste_tag` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_tag_alias_notify_ibfk_2` FOREIGN KEY (`idTag2`) REFERENCES `kernel_tag_liste_tag` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_tag_alias_notify`
--

LOCK TABLES `kernel_tag_alias_notify` WRITE;
/*!40000 ALTER TABLE `kernel_tag_alias_notify` DISABLE KEYS */;
/*!40000 ALTER TABLE `kernel_tag_alias_notify` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_tag_category`
--

DROP TABLE IF EXISTS `kernel_tag_category`;
CREATE TABLE `kernel_tag_category` (
  `id` smallint(5) unsigned NOT NULL auto_increment,
  `name` varchar(100) NOT NULL,
  `rightGet` smallint(5) unsigned NOT NULL default '1',
  `rightSet` smallint(5) unsigned NOT NULL default '1',
  `image` varchar(60) NOT NULL default '',
  `defaultShow` enum('true','false') NOT NULL default 'false',
  `description` varchar(100) NOT NULL default '',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `rightGet` (`rightGet`),
  KEY `rightSet` (`rightSet`),
  CONSTRAINT `kernel_tag_category_ibfk_1` FOREIGN KEY (`rightGet`) REFERENCES `kernel_right` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_tag_category_ibfk_2` FOREIGN KEY (`rightSet`) REFERENCES `kernel_right` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_tag_category`
--

LOCK TABLES `kernel_tag_category` WRITE;
/*!40000 ALTER TABLE `kernel_tag_category` DISABLE KEYS */;
/*!40000 ALTER TABLE `kernel_tag_category` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `kernel_tag_category_user`
--

DROP TABLE IF EXISTS `kernel_tag_category_user`;
/*!50001 DROP VIEW IF EXISTS `kernel_tag_category_user`*/;
/*!50001 CREATE TABLE `kernel_tag_category_user` (
  `IdCategory` smallint(5) unsigned,
  `login` int(10) unsigned,
  `name` varchar(100),
  `description` varchar(100),
  `image` varchar(60),
  `isShow` varchar(5)
) */;

--
-- Temporary table structure for view `kernel_tag_category_user_set`
--

DROP TABLE IF EXISTS `kernel_tag_category_user_set`;
/*!50001 DROP VIEW IF EXISTS `kernel_tag_category_user_set`*/;
/*!50001 CREATE TABLE `kernel_tag_category_user_set` (
  `idCategory` smallint(5) unsigned,
  `login` int(10) unsigned,
  `name` varchar(100),
  `image` varchar(60),
  `isShow` varchar(5)
) */;

--
-- Table structure for table `kernel_tag_comments`
--

DROP TABLE IF EXISTS `kernel_tag_comments`;
CREATE TABLE `kernel_tag_comments` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `texte` text NOT NULL,
  `date` timestamp NOT NULL default '0000-00-00 00:00:00',
  `idInformation` bigint(20) unsigned NOT NULL default '0',
  `author` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `idInformation` (`idInformation`),
  KEY `author` (`author`),
  CONSTRAINT `kernel_tag_comments_ibfk_1` FOREIGN KEY (`idInformation`) REFERENCES `kernel_tag_information` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_tag_comments_ibfk_2` FOREIGN KEY (`author`) REFERENCES `usersInfo` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_tag_comments`
--

LOCK TABLES `kernel_tag_comments` WRITE;
/*!40000 ALTER TABLE `kernel_tag_comments` DISABLE KEYS */;
/*!40000 ALTER TABLE `kernel_tag_comments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_tag_information`
--

DROP TABLE IF EXISTS `kernel_tag_information`;
CREATE TABLE `kernel_tag_information` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `tableOrig` varchar(60) NOT NULL,
  `idOrig` varchar(60) NOT NULL,
  `cacheNbDigg` int(11) NOT NULL default '0',
  `link` bigint(20) unsigned default NULL,
  PRIMARY KEY  (`id`),
  KEY `tableOrig` (`tableOrig`,`idOrig`),
  KEY `link` (`link`),
  CONSTRAINT `kernel_tag_information_ibfk_1` FOREIGN KEY (`link`) REFERENCES `kernel_tag_information` (`id`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_tag_information`
--

LOCK TABLES `kernel_tag_information` WRITE;
/*!40000 ALTER TABLE `kernel_tag_information` DISABLE KEYS */;
INSERT INTO `kernel_tag_information` VALUES (1,'news_liste','1',0,NULL);
/*!40000 ALTER TABLE `kernel_tag_information` ENABLE KEYS */;
UNLOCK TABLES;

/*!50003 SET @OLD_SQL_MODE=@@SQL_MODE*/;
DELIMITER ;;
/*!50003 SET SESSION SQL_MODE="" */;;
/*!50003 CREATE */ /*!50017 DEFINER=`root`@`localhost` */ /*!50003 TRIGGER `insertInformation` BEFORE INSERT ON `kernel_tag_information` FOR EACH ROW BEGIN
	
	IF (NEW.cacheNbDigg <> 0) THEN
		SET NEW.cacheNbDigg=0;
	END IF;
END */;;

DELIMITER ;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE */;

--
-- Table structure for table `kernel_tag_information_category`
--

DROP TABLE IF EXISTS `kernel_tag_information_category`;
CREATE TABLE `kernel_tag_information_category` (
  `idInformation` bigint(20) unsigned NOT NULL default '0',
  `idCategory` smallint(5) unsigned NOT NULL,
  PRIMARY KEY  (`idInformation`,`idCategory`),
  KEY `idInformation` (`idInformation`),
  KEY `idCategory` (`idCategory`),
  CONSTRAINT `kernel_tag_information_category_ibfk_1` FOREIGN KEY (`idInformation`) REFERENCES `kernel_tag_information` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_tag_information_category_ibfk_2` FOREIGN KEY (`idCategory`) REFERENCES `kernel_tag_category` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_tag_information_category`
--

LOCK TABLES `kernel_tag_information_category` WRITE;
/*!40000 ALTER TABLE `kernel_tag_information_category` DISABLE KEYS */;
/*!40000 ALTER TABLE `kernel_tag_information_category` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_tag_liste_tag`
--

DROP TABLE IF EXISTS `kernel_tag_liste_tag`;
CREATE TABLE `kernel_tag_liste_tag` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(30) NOT NULL default '',
  `alias` int(10) unsigned default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `alias` (`alias`),
  CONSTRAINT `kernel_tag_liste_tag_ibfk_1` FOREIGN KEY (`alias`) REFERENCES `kernel_tag_liste_tag` (`id`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_tag_liste_tag`
--

LOCK TABLES `kernel_tag_liste_tag` WRITE;
/*!40000 ALTER TABLE `kernel_tag_liste_tag` DISABLE KEYS */;
INSERT INTO `kernel_tag_liste_tag` VALUES (1,'@digg',NULL),(2,'@star',NULL),(3,'@shared',NULL);
/*!40000 ALTER TABLE `kernel_tag_liste_tag` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kernel_tag_tag`
--

DROP TABLE IF EXISTS `kernel_tag_tag`;
CREATE TABLE `kernel_tag_tag` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `idTagName` int(10) unsigned NOT NULL,
  `perso` enum('true','false') NOT NULL default 'false',
  `idInformation` bigint(20) unsigned NOT NULL default '0',
  `author` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `idInformation` (`idInformation`,`author`,`idTagName`),
  KEY `idTagName` (`idTagName`),
  KEY `idInformation_2` (`idInformation`),
  KEY `author` (`author`),
  CONSTRAINT `kernel_tag_tag_ibfk_1` FOREIGN KEY (`idTagName`) REFERENCES `kernel_tag_liste_tag` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_tag_tag_ibfk_2` FOREIGN KEY (`idInformation`) REFERENCES `kernel_tag_information` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_tag_tag_ibfk_3` FOREIGN KEY (`author`) REFERENCES `usersInfo` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_tag_tag`
--

LOCK TABLES `kernel_tag_tag` WRITE;
/*!40000 ALTER TABLE `kernel_tag_tag` DISABLE KEYS */;
/*!40000 ALTER TABLE `kernel_tag_tag` ENABLE KEYS */;
UNLOCK TABLES;

/*!50003 SET @OLD_SQL_MODE=@@SQL_MODE*/;
DELIMITER ;;
/*!50003 SET SESSION SQL_MODE="" */;;
/*!50003 CREATE */ /*!50017 DEFINER=`root`@`localhost` */ /*!50003 TRIGGER `insertTag` BEFORE INSERT ON `kernel_tag_tag` FOR EACH ROW BEGIN
	SELECT id INTO @digg FROM kernel_tag_liste_tag WHERE name="@digg";
	
	IF (NEW.idTagName=@digg) THEN
		UPDATE kernel_tag_information SET cacheNbDigg=cacheNbDigg+1 WHERE id=NEW.idInformation;
	END IF;
END */;;

/*!50003 SET SESSION SQL_MODE="" */;;
/*!50003 CREATE */ /*!50017 DEFINER=`root`@`localhost` */ /*!50003 TRIGGER `updateTag` BEFORE UPDATE ON `kernel_tag_tag` FOR EACH ROW BEGIN
	SELECT id INTO @digg FROM kernel_tag_liste_tag WHERE name="@digg";
	IF (OLD.idInformation<>NEW.idInformation) THEN
		IF (OLD.idTagName=@digg) THEN
			UPDATE kernel_tag_information SET cacheNbDigg=cacheNbDigg-1 WHERE id=OLD.idInformation;
		END IF;
		IF (NEW.idTagName=@digg) THEN
			UPDATE kernel_tag_information SET cacheNbDigg=cacheNbDigg+1 WHERE id=NEW.idInformation;
		END IF;
	ELSE
		IF (OLD.idTagName<>NEW.idTagName) THEN
			IF (OLD.idTagName=@digg) THEN
				UPDATE kernel_tag_information SET cacheNbDigg=cacheNbDigg-1 WHERE id=OLD.idInformation;
			END IF;
			
			IF (NEW.idTagName=@digg) THEN
				UPDATE kernel_tag_information SET cacheNbDigg=cacheNbDigg+1 WHERE id=NEW.idInformation;
			END IF;
		END IF;
	END IF;
END */;;

/*!50003 SET SESSION SQL_MODE="" */;;
/*!50003 CREATE */ /*!50017 DEFINER=`root`@`localhost` */ /*!50003 TRIGGER `deleteTag` AFTER DELETE ON `kernel_tag_tag` FOR EACH ROW BEGIN
	SELECT id INTO @digg FROM kernel_tag_liste_tag WHERE name="@digg";
	
	IF (OLD.idTagName=@digg) THEN
		UPDATE kernel_tag_information SET cacheNbDigg=cacheNbDigg-1 WHERE id=OLD.idInformation;
	END IF;
	
END */;;

DELIMITER ;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE */;

--
-- Table structure for table `kernel_tag_user_pref`
--

DROP TABLE IF EXISTS `kernel_tag_user_pref`;
CREATE TABLE `kernel_tag_user_pref` (
  `login` int(10) unsigned NOT NULL,
  `idCategory` smallint(5) unsigned NOT NULL,
  `isShow` enum('true','false') NOT NULL default 'false',
  PRIMARY KEY  (`login`,`idCategory`),
  KEY `login` (`login`),
  KEY `idCategory` (`idCategory`),
  CONSTRAINT `kernel_tag_user_pref_ibfk_1` FOREIGN KEY (`login`) REFERENCES `usersInfo` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `kernel_tag_user_pref_ibfk_2` FOREIGN KEY (`idCategory`) REFERENCES `kernel_tag_category` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `kernel_tag_user_pref`
--

LOCK TABLES `kernel_tag_user_pref` WRITE;
/*!40000 ALTER TABLE `kernel_tag_user_pref` DISABLE KEYS */;
/*!40000 ALTER TABLE `kernel_tag_user_pref` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `listes`
--

DROP TABLE IF EXISTS `listes`;
/*!50001 DROP VIEW IF EXISTS `listes`*/;
/*!50001 CREATE TABLE `listes` (
  `liste` varchar(25),
  `parent` varchar(25),
  `description` varchar(30)
) */;

--
-- Table structure for table `login`
--

DROP TABLE IF EXISTS `login`;
CREATE TABLE `login` (
  `login` varchar(20) NOT NULL default '',
  `type` varchar(20) NOT NULL default '',
  `last_log` datetime NOT NULL default '0000-00-00 00:00:00',
  `nb_log` smallint(6) NOT NULL default '0',
  PRIMARY KEY  (`login`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Table de r�f�rence des logins';

--
-- Dumping data for table `login`
--

LOCK TABLES `login` WRITE;
/*!40000 ALTER TABLE `login` DISABLE KEYS */;
INSERT INTO `login` VALUES ('julien','ext','0000-00-00 00:00:00',0),('grumly','ext','0000-00-00 00:00:00',0),('nicolas','ext','0000-00-00 00:00:00',0),('nicolas.demengel@gen','ext','0000-00-00 00:00:00',0),('plop.plap@plip.com','ext','0000-00-00 00:00:00',0),('papa.pingouin@banqui','ext','0000-00-00 00:00:00',0),('Gilles','ext','0000-00-00 00:00:00',0);
/*!40000 ALTER TABLE `login` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_event`
--

DROP TABLE IF EXISTS `module_event`;
CREATE TABLE `module_event` (
  `id` int(11) NOT NULL auto_increment,
  `nom` text NOT NULL,
  `jr_debut` date NOT NULL default '0000-00-00',
  `jr_fin` date NOT NULL default '0000-00-00',
  `titre` varchar(255) NOT NULL default '',
  `texte` longtext NOT NULL,
  `image` varchar(30) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `module_event`
--

LOCK TABLES `module_event` WRITE;
/*!40000 ALTER TABLE `module_event` DISABLE KEYS */;
/*!40000 ALTER TABLE `module_event` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_links`
--

DROP TABLE IF EXISTS `module_links`;
CREATE TABLE `module_links` (
  `id` int(10) NOT NULL auto_increment COMMENT 'primary key from the link',
  `name` varchar(200) NOT NULL COMMENT 'name of the link',
  `url` varchar(200) NOT NULL COMMENT 'address of the link',
  `description` text COMMENT 'little text about the link',
  `date` date NOT NULL COMMENT 'date of creation',
  `valid` enum('Y','N') NOT NULL default 'Y' COMMENT 'attribute showing if the link is visible by the users, or not.',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Table which include all the useful links';

--
-- Dumping data for table `module_links`
--

LOCK TABLES `module_links` WRITE;
/*!40000 ALTER TABLE `module_links` DISABLE KEYS */;
/*!40000 ALTER TABLE `module_links` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_mpub`
--

DROP TABLE IF EXISTS `module_mpub`;
CREATE TABLE `module_mpub` (
  `id` int(11) NOT NULL auto_increment COMMENT 'id de l''image',
  `nom` varchar(50) character set utf8 collate utf8_unicode_ci NOT NULL COMMENT 'nom de l''image',
  `link` text NOT NULL,
  `valide` enum('Y','N') NOT NULL default 'Y' COMMENT 'attribut indiquant si l''image est dans la rotation ou pas',
  `place` enum('top','bottom') NOT NULL COMMENT 'place of the ad : on the left/right, or just before the footer',
  `image` varchar(30) character set utf8 collate utf8_unicode_ci default NULL COMMENT 'id al?atoire de l''image de la publicit?',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `nom` (`nom`,`image`),
  FULLTEXT KEY `link` (`link`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `module_mpub`
--

LOCK TABLES `module_mpub` WRITE;
/*!40000 ALTER TABLE `module_mpub` DISABLE KEYS */;
/*!40000 ALTER TABLE `module_mpub` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_notes`
--

DROP TABLE IF EXISTS `module_notes`;
CREATE TABLE `module_notes` (
  `login` varchar(10) NOT NULL,
  `content` text NOT NULL,
  `date` datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`login`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `module_notes`
--

LOCK TABLES `module_notes` WRITE;
/*!40000 ALTER TABLE `module_notes` DISABLE KEYS */;
INSERT INTO `module_notes` VALUES ('3','Cliquez sur ce texte pour le modifier','2007-04-17 09:49:51'),('0','Cliquez sur ce texte pour le modifier','2007-04-17 09:50:44');
/*!40000 ALTER TABLE `module_notes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_survey`
--

DROP TABLE IF EXISTS `module_survey`;
CREATE TABLE `module_survey` (
  `id` int(8) NOT NULL auto_increment COMMENT 'id of the survey',
  `title` varchar(250) NOT NULL COMMENT 'title of the survey',
  `q1` varchar(200) NOT NULL COMMENT 'question n°1',
  `q2` varchar(200) NOT NULL COMMENT 'question n°2',
  `q3` varchar(200) NOT NULL COMMENT 'question n°3',
  `q4` varchar(200) NOT NULL COMMENT 'question n°4',
  `q5` varchar(200) NOT NULL COMMENT 'question n°5',
  `q6` varchar(200) NOT NULL COMMENT 'question n°6',
  `q7` varchar(200) NOT NULL COMMENT 'question n°7',
  `q8` varchar(200) NOT NULL COMMENT 'question n°8',
  `q9` varchar(200) NOT NULL COMMENT 'question n°9',
  `q10` varchar(200) NOT NULL COMMENT 'question n°10',
  `date` date NOT NULL COMMENT '"birthdate" of the survey',
  `active` enum('Y','N') NOT NULL default 'N' COMMENT 'shows if the survey is available or not for users',
  `ended` enum('Y','N') NOT NULL default 'N' COMMENT 'This attribute says if the survey has ended or not',
  `archive` enum('Y','N') NOT NULL default 'N' COMMENT 'shows if the survey is archived',
  `proposal` enum('Y','N') NOT NULL default 'N' COMMENT 'Attribute saying if the survey was/is a proposal or not',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='This table includes all the informations about the surveys o';

--
-- Dumping data for table `module_survey`
--

LOCK TABLES `module_survey` WRITE;
/*!40000 ALTER TABLE `module_survey` DISABLE KEYS */;
/*!40000 ALTER TABLE `module_survey` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_survey_answers`
--

DROP TABLE IF EXISTS `module_survey_answers`;
CREATE TABLE `module_survey_answers` (
  `id` int(8) NOT NULL auto_increment COMMENT 'id of the answer',
  `id_survey` int(8) NOT NULL COMMENT 'id of the survey related to the survey',
  `answer` varchar(3) NOT NULL COMMENT 'anwer of the survey',
  `login` int(8) NOT NULL COMMENT 'login of the user who has answered',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='This table includes all the answers of the surveys';

--
-- Dumping data for table `module_survey_answers`
--

LOCK TABLES `module_survey_answers` WRITE;
/*!40000 ALTER TABLE `module_survey_answers` DISABLE KEYS */;
/*!40000 ALTER TABLE `module_survey_answers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_survey_archives`
--

DROP TABLE IF EXISTS `module_survey_archives`;
CREATE TABLE `module_survey_archives` (
  `id` int(10) NOT NULL auto_increment COMMENT 'id of the archive',
  `id_survey` int(10) NOT NULL COMMENT 'id of the survey archived',
  `q1_results` int(30) NOT NULL COMMENT 'results of the question 1',
  `q2_results` int(30) NOT NULL COMMENT 'results of the question 2',
  `q3_results` int(30) NOT NULL COMMENT 'results of the question 3',
  `q4_results` int(30) NOT NULL COMMENT 'results of the question 4',
  `q5_results` int(30) NOT NULL COMMENT 'results of the question 5',
  `q6_results` int(30) NOT NULL COMMENT 'results of the question 6',
  `q7_results` int(30) NOT NULL COMMENT 'results of the question 7',
  `q8_results` int(30) NOT NULL COMMENT 'results of the question 8',
  `q9_results` int(30) NOT NULL COMMENT 'results of the question 9',
  `q10_results` int(30) NOT NULL COMMENT 'results of the question 10',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='This table includes the results of the survey related to the';

--
-- Dumping data for table `module_survey_archives`
--

LOCK TABLES `module_survey_archives` WRITE;
/*!40000 ALTER TABLE `module_survey_archives` DISABLE KEYS */;
/*!40000 ALTER TABLE `module_survey_archives` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_todolist`
--

DROP TABLE IF EXISTS `module_todolist`;
CREATE TABLE `module_todolist` (
  `id` int(11) NOT NULL auto_increment COMMENT 'Primary key',
  `login` int(11) NOT NULL COMMENT 'Login of the task''s bearer.',
  `title` varchar(250) NOT NULL COMMENT 'title of the task',
  `expiry` date NOT NULL COMMENT 'date of expiry of the task',
  `done` enum('Y','N') NOT NULL COMMENT 'This attribute show if the task is done or not',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='This table include all the informations about the users'' tas';

--
-- Dumping data for table `module_todolist`
--

LOCK TABLES `module_todolist` WRITE;
/*!40000 ALTER TABLE `module_todolist` DISABLE KEYS */;
/*!40000 ALTER TABLE `module_todolist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_welcome`
--

DROP TABLE IF EXISTS `module_welcome`;
CREATE TABLE `module_welcome` (
  `id` int(11) NOT NULL auto_increment,
  `title` varchar(255) NOT NULL,
  `text` longtext NOT NULL,
  `image` varchar(30) default NULL,
  `rights` varchar(250) NOT NULL,
  `valid` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `module_welcome`
--

LOCK TABLES `module_welcome` WRITE;
/*!40000 ALTER TABLE `module_welcome` DISABLE KEYS */;
/*!40000 ALTER TABLE `module_welcome` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `news_liste`
--

DROP TABLE IF EXISTS `news_liste`;
CREATE TABLE `news_liste` (
  `id_news` smallint(6) NOT NULL auto_increment,
  `titre` varchar(200) NOT NULL,
  `auteur` varchar(100) NOT NULL,
  `auteur_login` int(11) NOT NULL,
  `information` text NOT NULL,
  `date_creation` datetime NOT NULL,
  `id_news_type` tinyint(4) NOT NULL default '0',
  `origine` varchar(100) NOT NULL default '',
  `valide` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`id_news`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Liste des news';

--
-- Dumping data for table `news_liste`
--

LOCK TABLES `news_liste` WRITE;
/*!40000 ALTER TABLE `news_liste` DISABLE KEYS */;
/*!40000 ALTER TABLE `news_liste` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `news_read`
--

DROP TABLE IF EXISTS `news_read`;
CREATE TABLE `news_read` (
  `id_news` int(10) unsigned NOT NULL default '0',
  `login` int(11) NOT NULL,
  `date_lecture` datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`id_news`,`login`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Enregistrement de lecture des news';

--
-- Dumping data for table `news_read`
--

LOCK TABLES `news_read` WRITE;
/*!40000 ALTER TABLE `news_read` DISABLE KEYS */;
INSERT INTO `news_read` VALUES (1,3,'2007-04-17 18:34:55');
/*!40000 ALTER TABLE `news_read` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pageMenu`
--

DROP TABLE IF EXISTS `pageMenu`;
CREATE TABLE `pageMenu` (
  `id_menu` int(5) NOT NULL auto_increment,
  `href` varchar(50) NOT NULL default '',
  `name` varchar(50) NOT NULL default '',
  `class` varchar(20) NOT NULL default '',
  `valid` enum('Y','N') NOT NULL default 'Y',
  PRIMARY KEY  (`id_menu`)
) ENGINE=MyISAM AUTO_INCREMENT=32 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `pageMenu`
--

LOCK TABLES `pageMenu` WRITE;
/*!40000 ALTER TABLE `pageMenu` DISABLE KEYS */;
INSERT INTO `pageMenu` VALUES (16,'forum/','Forum','forum','Y'),(18,'covoiturage/','Covoiturage','covoiturage','Y'),(19,'trombi/','Trombinoscope','trombi','Y'),(20,'index.php','Accueil','home','Y'),(23,'file_browser/','Mes fichiers','file_browser','Y'),(26,'index.php?logout','DÃ©connexion','logout','Y');
/*!40000 ALTER TABLE `pageMenu` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prefs_liste`
--

DROP TABLE IF EXISTS `prefs_liste`;
CREATE TABLE `prefs_liste` (
  `pref_name` varchar(255) NOT NULL default '',
  `pref_value` longtext NOT NULL,
  `pref_title` varchar(50) NOT NULL default '',
  `description` text NOT NULL,
  `type` varchar(10) NOT NULL default '',
  `display` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`pref_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Liste des prÃ©fÃ©rences par dÃ©faut';

--
-- Dumping data for table `prefs_liste`
--

LOCK TABLES `prefs_liste` WRITE;
/*!40000 ALTER TABLE `prefs_liste` DISABLE KEYS */;
INSERT INTO `prefs_liste` VALUES ('forum_pseudo','','Pseudonyme','Votre pseudonyme sur le forum','text','Y'),('forum_email','','Email','L\'email que vous dÃ©sirez Ã©ventuellement afficher avec vos messages','text','Y'),('trombi_order','nom','[Trombi] Trier par','Tri des fiches dans une recherche du trombi','','N'),('trombi_limit','20','[Trombi] Taille des pages','Nombre de fiches par page','integer','N'),('site_citation','true','Afficher la citation du jour','Affiche chaque jour une citation en t&ecirc;te du site.','radio','N'),('last_log','','Dernier log','Date du dernier log','timestamp','N'),('nb_log','0','Nombre d\'identitications','','integer','N'),('email','','Email','Email de crÃ©ation du compte','text','N'),('notifications_annonce','','Nouvelle petite annonce','Notification pour une nouvelle petite annonce','checkbox','N'),('notifications_dem_covoiturage','','Nouvelle demande covoiturage','Nouvelle demande de covoiturage incluant son dÃ©partement','checkbox','N'),('modules_trade_followup','5','Annonces suivies','Nombre d\'annonces afichÃ©es dans le module de suivi...','integer','Y'),('modules_meteo_ville','Troyes','Changer la ville','Ville pour laquelle on va afficher la mÃ©tÃ©o.','select','Y'),('trombi_photo','detail','Afficher les photos','Afficher ou non les photos','radio','N'),('forum_myavatar','true','Avatar','Afficher ou non mon avatar','radio','Y'),('forum_mysignature','true','Afficher ou non ma signature','Afficher ou non ma signature','radio','N'),('forum_modoavatar','false','A Ã©tÃ© moderÃ© ou pas','','radio','N'),('forum_nbrsuivit','6','Module forum : suivi messages','Nombre de messages afichÃ©s sur le module de suivi','select','Y'),('forum_signature','','Signature des messages','Ce qui apparaÃ®tra en bas de chacun de vos messages','textarea','Y'),('covoiturage_dept_suivi','0','Suivi du covoiturage d\'un dÃ©partement','Permet d\'etre prÃ©venu des trajets (offres ou demandes) contenant le dÃ©partement dÃ©sirÃ©','text','N'),('covoiturage_suivi','false','Activer le suivi des trajets','PossibilitÃ© d\\\'etre alertÃ© d\'un covoiturage passant par le dÃ©partement de son choix','radio','N'),('site_drag_and_drop','false','Drag and Drop','Permet ou non le Drag en drop (Glisser dÃ©posÃ©) des modules de la page index !','radio','N'),('site_check_mail','false','Notification mail','Activer ou non l\'avertissement a la reception d\'un nouveau mail !','radio','Y'),('site_keynav','true','Raccourcis clavier','Naviguer sur le site grace a votre clavier !','radio','N'),('raccourcis_accueil','h','Raccourcis vers l\'accueil','Raccourcis vers l\'accueil','raccourcis','N'),('raccourcis_forum','f','Raccourcis  vers le forum vers le pod','Raccourcis vers le forum','raccourcis','N'),('raccourcis_mail','m','Raccourcis vers les mails','Raccourcis vers les mails','raccourcis','N'),('raccourcis_createmail','c','Raccourcis vers new mails','Raccourcis vers new mails','raccourcis','N'),('site_skin','default','Skin du site','Permet de choisir le skin de wope','select','Y'),('raccourcis_widget','w','Raccourcis pour afficher les widget !','Raccourcis pour afficher les widget !','raccourcis','N'),('site_webmail','roundcube','Webmail du site','Quel lecteur de mail voulez vous utiliser sur le site ?','select','N'),('site_pseudo','Pseudo','Pseudo du site','Le pseudo utilisÃ© par le site !','text','N'),('newsletter_subscribing','true','Recevoir la newsletter','La newsletter vous permet de recevoir par mail (ou dans le module appropriÃ©) les derniÃ¨res informations de votre communautÃ©, dans les domaines qui vous intÃ©ressent.','radio','Y'),('modules_columns','2','Nombre de colonnes','','select','N');
/*!40000 ALTER TABLE `prefs_liste` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prefs_values`
--

DROP TABLE IF EXISTS `prefs_values`;
CREATE TABLE `prefs_values` (
  `pvals_id` int(11) unsigned NOT NULL auto_increment,
  `pvals_name` varchar(255) NOT NULL default '',
  `pvals_value` varchar(30) NOT NULL default '',
  PRIMARY KEY  (`pvals_id`)
) ENGINE=MyISAM AUTO_INCREMENT=116 DEFAULT CHARSET=utf8 COMMENT='Tables pour les pr�f�rences qui ont plusieurs valeurs ';

--
-- Dumping data for table `prefs_values`
--

LOCK TABLES `prefs_values` WRITE;
/*!40000 ALTER TABLE `prefs_values` DISABLE KEYS */;
INSERT INTO `prefs_values` VALUES (56,'modules_meteo_ville','Troyes'),(57,'modules_meteo_ville','Paris'),(58,'modules_meteo_ville','Lyon'),(59,'modules_meteo_ville','Marseille'),(60,'modules_meteo_ville','Bordeaux'),(61,'modules_meteo_ville','Lille'),(62,'modules_meteo_ville','Nantes'),(63,'modules_meteo_ville','Toulouse'),(64,'modules_meteo_ville','Orl'),(65,'modules_meteo_ville','Brest'),(66,'modules_meteo_ville','Rouen'),(67,'modules_meteo_ville','Tours'),(68,'modules_meteo_ville','Strasbourg'),(69,'modules_meteo_ville','Dijon'),(70,'modules_meteo_ville','Amiens'),(71,'modules_meteo_ville','Ch'),(72,'modules_meteo_ville','Dax'),(73,'modules_meteo_ville','Lens'),(74,'modules_meteo_ville','Nice'),(75,'modules_meteo_ville','Niort'),(76,'modules_meteo_ville','Montpellier'),(77,'modules_meteo_ville','Rodez'),(78,'modules_meteo_ville','Reims'),(79,'modules_meteo_ville','Grenoble'),(80,'modules_meteo_ville','Auxerre'),(81,'modules_meteo_ville','La Rochelle'),(82,'modules_meteo_ville','Le Mans'),(83,'modules_meteo_ville','Clermont Ferrand'),(84,'modules_meteo_ville','Saint Etienne'),(85,'modules_meteo_ville','Metz'),(86,'modules_meteo_ville','Chamonix'),(87,'modules_meteo_ville','Rennes'),(88,'modules_meteo_ville','Bourges'),(89,'modules_meteo_ville','Le Havre'),(90,'modules_meteo_ville','Caen'),(91,'modules_meteo_ville','Saint Quentin'),(92,'modules_meteo_ville','Poitiers'),(93,'modules_meteo_ville','Biarritz'),(94,'modules_meteo_ville','Dunkerque'),(95,'forum_nbrsuivit','1'),(96,'forum_nbrsuivit','2'),(97,'forum_nbrsuivit','3'),(98,'forum_nbrsuivit','4'),(99,'forum_nbrsuivit','5'),(100,'forum_nbrsuivit','6'),(101,'forum_nbrsuivit','7'),(102,'forum_nbrsuivit','8'),(103,'forum_nbrsuivit','9'),(104,'forum_nbrsuivit','10'),(105,'forum_nbrsuivit','15'),(106,'forum_nbrsuivit','20'),(107,'forum_nbrsuivit','25'),(108,'site_skin','default'),(111,'site_webmail','IMP'),(112,'site_webmail','roundcube'),(115,'modules_columns','2');
/*!40000 ALTER TABLE `prefs_values` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sommaire_title`
--

DROP TABLE IF EXISTS `sommaire_title`;
CREATE TABLE `sommaire_title` (
  `id` tinyint(3) unsigned NOT NULL auto_increment,
  `name` varchar(100) NOT NULL,
  `url` text,
  `father_id` tinyint(3) unsigned default NULL,
  `is_hidden` enum('true','false') NOT NULL default 'false',
  `rights` varchar(100) default NULL,
  `category` enum('a','b','c') NOT NULL default 'a',
  PRIMARY KEY  (`id`),
  KEY `father_id` (`father_id`),
  CONSTRAINT `sommaire_title_ibfk_1` FOREIGN KEY (`father_id`) REFERENCES `sommaire_title` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `sommaire_title`
--

LOCK TABLES `sommaire_title` WRITE;
/*!40000 ALTER TABLE `sommaire_title` DISABLE KEYS */;
INSERT INTO `sommaire_title` VALUES (1,'FAQ','faq/',NULL,'false','all','a');
/*!40000 ALTER TABLE `sommaire_title` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `subscribeManager_event`
--

DROP TABLE IF EXISTS `subscribeManager_event`;
CREATE TABLE `subscribeManager_event` (
  `id_event` int(10) NOT NULL auto_increment,
  `nom` varchar(80) NOT NULL default '',
  `description` text NOT NULL,
  `login_admin` int(11) NOT NULL,
  `passwd_event` varchar(50) NOT NULL default '',
  `date_creation` datetime NOT NULL default '0000-00-00 00:00:00',
  `max_place` int(10) NOT NULL default '0',
  `date_start` date NOT NULL default '0000-00-00',
  `date_expire` date NOT NULL default '0000-00-00',
  `droit_event` varchar(50) NOT NULL default '',
  `reserve` enum('Y','N') NOT NULL default 'Y',
  `public` enum('Y','N') NOT NULL default 'Y',
  `inscription_public` enum('Y','N') NOT NULL default 'Y',
  PRIMARY KEY  (`id_event`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `subscribeManager_event`
--

LOCK TABLES `subscribeManager_event` WRITE;
/*!40000 ALTER TABLE `subscribeManager_event` DISABLE KEYS */;
/*!40000 ALTER TABLE `subscribeManager_event` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `subscribeManager_gens`
--

DROP TABLE IF EXISTS `subscribeManager_gens`;
CREATE TABLE `subscribeManager_gens` (
  `id` int(11) NOT NULL auto_increment,
  `login` int(11) NOT NULL,
  `id_event` int(10) NOT NULL default '0',
  `date` datetime NOT NULL default '0000-00-00 00:00:00',
  `com` text NOT NULL,
  `etat` enum('Y','N','P') NOT NULL default 'Y',
  `exterieur` varchar(200) NOT NULL default '',
  `isin` enum('N','Y') NOT NULL default 'N',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `subscribeManager_gens`
--

LOCK TABLES `subscribeManager_gens` WRITE;
/*!40000 ALTER TABLE `subscribeManager_gens` DISABLE KEYS */;
/*!40000 ALTER TABLE `subscribeManager_gens` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `surveyTools_ans`
--

DROP TABLE IF EXISTS `surveyTools_ans`;
CREATE TABLE `surveyTools_ans` (
  `login` int(11) NOT NULL,
  `id_sdg` int(11) NOT NULL default '0',
  `id_asq` int(11) NOT NULL default '0',
  `ans` text NOT NULL,
  `ip` varchar(100) NOT NULL default '',
  `marqueur` enum('N','H','I') NOT NULL default 'N',
  PRIMARY KEY  (`login`,`id_sdg`,`id_asq`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `surveyTools_ans`
--

LOCK TABLES `surveyTools_ans` WRITE;
/*!40000 ALTER TABLE `surveyTools_ans` DISABLE KEYS */;
/*!40000 ALTER TABLE `surveyTools_ans` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `surveyTools_asq`
--

DROP TABLE IF EXISTS `surveyTools_asq`;
CREATE TABLE `surveyTools_asq` (
  `id_sdg` int(11) NOT NULL default '0',
  `id_asq` int(11) NOT NULL auto_increment,
  `type` enum('radio','text','multiple','int') NOT NULL default 'radio',
  `label` varchar(200) NOT NULL default '',
  PRIMARY KEY  (`id_asq`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `surveyTools_asq`
--

LOCK TABLES `surveyTools_asq` WRITE;
/*!40000 ALTER TABLE `surveyTools_asq` DISABLE KEYS */;
/*!40000 ALTER TABLE `surveyTools_asq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `surveyTools_asq_ans`
--

DROP TABLE IF EXISTS `surveyTools_asq_ans`;
CREATE TABLE `surveyTools_asq_ans` (
  `id_asq` int(11) NOT NULL default '0',
  `id_ans` int(11) NOT NULL auto_increment,
  `label` varchar(200) NOT NULL default '',
  PRIMARY KEY  (`id_asq`,`id_ans`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `surveyTools_asq_ans`
--

LOCK TABLES `surveyTools_asq_ans` WRITE;
/*!40000 ALTER TABLE `surveyTools_asq_ans` DISABLE KEYS */;
/*!40000 ALTER TABLE `surveyTools_asq_ans` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `surveyTools_sdg`
--

DROP TABLE IF EXISTS `surveyTools_sdg`;
CREATE TABLE `surveyTools_sdg` (
  `id_sdg` int(11) NOT NULL auto_increment,
  `admin` varchar(10) character set latin1 NOT NULL default '',
  `droit` varchar(10) character set latin1 NOT NULL default '',
  `debut` date NOT NULL default '0000-00-00',
  `fin` date NOT NULL default '0000-00-00',
  `label` varchar(250) character set latin1 NOT NULL default '',
  `description` text character set latin1 NOT NULL,
  `valid` enum('N','Y') character set latin1 NOT NULL default 'Y',
  PRIMARY KEY  (`id_sdg`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `surveyTools_sdg`
--

LOCK TABLES `surveyTools_sdg` WRITE;
/*!40000 ALTER TABLE `surveyTools_sdg` DISABLE KEYS */;
/*!40000 ALTER TABLE `surveyTools_sdg` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `trombi_trombi`
--

DROP TABLE IF EXISTS `trombi_trombi`;
CREATE TABLE `trombi_trombi` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `id_user` int(10) unsigned NOT NULL,
  `nickname` varchar(30) NOT NULL default '',
  `lastname` varchar(30) NOT NULL default '',
  `firstname` varchar(30) NOT NULL default '',
  `position` varchar(60) NOT NULL default '',
  `birthdate` date NOT NULL default '0000-00-00',
  `signal_social` varchar(50) NOT NULL default '',
  `photo` varchar(255) NOT NULL default '',
  `hobbies` text NOT NULL,
  `address` varchar(60) NOT NULL default '',
  `address2` varchar(60) NOT NULL default '',
  `postal_code` varchar(6) NOT NULL default '',
  `city` varchar(60) NOT NULL default '',
  `phone` varchar(15) NOT NULL default '',
  `email` varchar(150) NOT NULL default '',
  `email2` varchar(150) NOT NULL default '',
  `site` varchar(30) NOT NULL default '',
  `msn` varchar(60) NOT NULL default '',
  `jabber` varchar(60) NOT NULL default '',
  `yahoo` varchar(60) NOT NULL default '',
  PRIMARY KEY  (`id`),
  KEY `id_user` (`id_user`),
  CONSTRAINT `trombi_trombi_ibfk_1` FOREIGN KEY (`id_user`) REFERENCES `usersInfo` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `trombi_trombi`
--

LOCK TABLES `trombi_trombi` WRITE;
/*!40000 ALTER TABLE `trombi_trombi` DISABLE KEYS */;
INSERT INTO `trombi_trombi` VALUES (1,3,'Admin Admin','Admin','Admin','','0000-00-00','','1','','','','','','','admin@wope.lo','','','','','');
/*!40000 ALTER TABLE `trombi_trombi` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `type_mime`
--

DROP TABLE IF EXISTS `type_mime`;
CREATE TABLE `type_mime` (
  `type_mime` varchar(50) NOT NULL default '',
  `extensions` varchar(50) NOT NULL default '',
  `description` varchar(100) NOT NULL default '',
  `image16` varchar(60) NOT NULL default '',
  `image32` varchar(60) NOT NULL default '',
  PRIMARY KEY  (`type_mime`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Types mimes';

--
-- Dumping data for table `type_mime`
--

LOCK TABLES `type_mime` WRITE;
/*!40000 ALTER TABLE `type_mime` DISABLE KEYS */;
INSERT INTO `type_mime` VALUES ('application/acad','*.dwg','Fichiers AutoCAD (d\'apr','',''),('application/applefile','&Acirc;','Fichiers AppleFile','',''),('application/astound','*.asd *.asn','Fichiers Astound','',''),('application/dsptype','*.tsp','Fichiers TSP','',''),('application/dxf','*.dxf','Fichiers AutoCAD (d\'apr','',''),('application/futuresplash','*.spl','Fichiers Flash Futuresplash','',''),('application/gzip','*.gz','Fichiers GNU Zip','',''),('application/listenup','*.ptlk','Fichiers Listenup','',''),('application/mac-binhex40','*.hqx','Fichiers binaires Macintosh','',''),('application/mbedlet','*.mbd','Fichiers Mbedlet','',''),('application/mif','*.mif','Fichiers FrameMaker Interchange Format','',''),('application/msexcel','*.xls *.xla','Fichiers Microsoft Excel','mime_type.application_x_excel.gif',''),('application/mshelp','*.hlp *.chm','Fichiers d\'aide Microsoft Windows','',''),('application/mspowerpoint','*.ppt *.ppz *.pps *.pot','Fichiers Microsoft Powerpoint','mime_type.application_mspowerpoint.gif',''),('application/msword','*.doc *.dot','Fichiers Microsoft Word','mime_type.application_msword.gif',''),('application/octet-stream','*.bin *.exe *.com *.dll *.class','Fichiers ex','',''),('application/oda','*.oda','Fichiers Oda','',''),('application/pdf','*.pdf','Fichiers Adobe PDF','mime_type.application_pdf.gif',''),('application/postscript','*.ai *.eps *.ps','Fichiers Adobe Postscript','',''),('application/rtc','*.rtc','Fichiers RTC','',''),('application/rtf','*.rtf','Fichiers Microsoft RTF','mime_type.application_rtf.gif',''),('application/studiom','*.smp','Fichiers Studiom','',''),('application/toolbook','*.tbk','Fichiers Toolbook','',''),('application/vocaltec-media-desc','*.vmd','Fichiers Vocaltec Mediadesc','',''),('application/vocaltec-media-file','*.vmf','Fichiers Vocaltec Media','',''),('application/x-bcpio','*.bcpio','Fichiers BCPIO','',''),('application/x-compress','*.z','Fichiers -','',''),('application/x-cpio','*.cpio','Fichiers CPIO','',''),('application/x-csh','*.csh','Fichiers C-Shellscript','',''),('application/x-director','*.dcr *.dir *.dxr','Fichiers -','',''),('application/x-dvi','*.dvi','Fichiers DVI','',''),('application/x-envoy','*.evy','Fichiers Envoy','',''),('application/x-gtar','*.gtar','Fichiers archives GNU tar','',''),('application/x-hdf','*.hdf','Fichiers HDF','',''),('application/x-httpd-php','*.php *.phtml','Fichiers PHP','',''),('application/x-javascript','*.js','Fichiers JavaScript c','',''),('application/x-latex','*.latex','Fichiers source Latex','',''),('application/x-macbinary','*.bin','Fichiers binaires Macintosh','',''),('application/x-mif','*.mif','Fichiers FrameMaker Interchange Format','',''),('application/x-netcdf','*.nc *.cdf','Fichiers Unidata CDF','',''),('application/x-nschat','*.nsc','Fichiers NS Chat','',''),('application/x-sh','*.sh','Fichiers Bourne Shellscript','',''),('application/x-shar','*.shar','Fichiers atchives Shell','',''),('application/x-shockwave-flash','*.swf *.cab','Fichiers Flash Shockwave','mime_type.application_shockwave_flash.gif',''),('application/x-sprite','*.spr *.sprite','Fichiers Sprite','',''),('application/x-stuffit','*.sit','Fichiers Stuffit','',''),('application/x-supercard','*.sca','Fichiers Supercard','',''),('application/x-sv4cpio','*.sv4cpio','Fichiers CPIO','',''),('application/x-sv4crc','*.sv4crc','Fichiers CPIO avec CRC','',''),('application/x-tar','*.tar','Fichiers archives tar','',''),('application/x-tcl','*.tcl','Fichiers script TCL','',''),('application/x-tex','*.tex','Fichiers TEX','',''),('application/x-texinfo','*.texinfo *.texi','Fichiers TEXinfo','',''),('application/x-troff','*.t *.tr *.roff','Fichiers TROFF (Unix)','',''),('application/x-troff-man','*.man *.troff','Fichiers TROFF avec macros MAN (Unix)','',''),('application/x-troff-me','*.me *.troff','Fichiers TROFF avec macros ME (Unix)','',''),('application/x-troff-ms','*.me *.troff','Fichiers TROFF avec macros MS (Unix)','',''),('application/x-ustar','*.ustar','Fichiers archives tar (Posix)','',''),('application/x-wais-source','*.src','Fichiers source WAIS','',''),('application/x-www-form-urlencoded','&Acirc;','Donn','',''),('application/zip','*.zip','Fichiers archives ZIP','mime_type.application_zip.gif',''),('audio/basic','*.au *.snd','Fichiers son','mime_type.audio_basic.gif',''),('audio/echospeech','*.es','Fichiers Echospeed','',''),('audio/tsplayer','*.tsi','Fichiers TS-Player','',''),('audio/voxware','*.vox','Fichiers Vox','',''),('audio/x-aiff','*.aif *.aiff *.aifc','Fichiers son AIFF','',''),('audio/x-dspeeh','*.dus *.cht','Fichiers parole','',''),('audio/x-midi','*.mid *.midi','Fichiers MIDI','mime_type.audio_midi.gif',''),('audio/x-mpeg','*.mp2','Fichiers MPEG','',''),('audio/x-pn-realaudio','*.ram *.ra','Fichiers RealAudio','',''),('audio/x-pn-realaudio-plugin','*.rpm','Fichiers plugin RealAudio','',''),('audio/x-qt-stream','*.stream','Fichiers -','',''),('audio/x-wav','*.wav','Fichiers Wav','',''),('drawing/x-dwf','*.dwf','Fichiers Drawing','',''),('image/cis-cod','*.cod','Fichiers CIS-Cod','',''),('image/cmu-raster','*.ras','Fichiers CMU-Raster','',''),('image/fif','*.fif','Fichiers FIF','',''),('image/gif','*.gif','Fichiers GIF','mime_type.image_gif.gif',''),('image/ief','*.ief','Fichiers IEF','',''),('image/jpeg','*.jpeg *.jpg *.jpe','Fichiers JPEG','mime_type.image_jpeg.gif',''),('image/tiff','*.tiff *.tif','Fichiers TIFF','mime_type.image_tiff.gif',''),('image/vasa','*.mcf','Fichiers Vasa','',''),('image/vnd.wap.wbmp','*.wbmp','Fichiers Bitmap (WAP)','',''),('image/x-freehand','*.fh4 *.fh5 *.fhc','Fichiers Freehand','',''),('image/x-portable-anymap','*.pnm','Fichiers PBM Anymap','',''),('image/x-portable-bitmap','*.pbm','Fichiers Bitmap PBM','',''),('image/x-portable-graymap','*.pgm','Fichiers PBM Graymap','',''),('image/x-portable-pixmap','*.ppm','Fichiers PBM Pixmap','',''),('image/x-rgb','*.rgb','Fichiers RGB','',''),('image/x-windowdump','*.xwd','X-Windows Dump','',''),('image/x-xbitmap','*.xbm','Fichiers XBM','',''),('image/x-xpixmap','*.xpm','Fichiers XPM','',''),('message/external-body','&Acirc;','Nouvelle avec contenu externe','',''),('message/http','&Acirc;','Nouvelle &Atilde;  ent','',''),('message/news','&Acirc;','Nouvelle de Newsgroup','',''),('message/partial','&Acirc;','Nouvelle avec contenu partiel','',''),('message/rfc822','&Acirc;','Nouvelle d\'apr','',''),('model/vrml','*.wrl','Visualisation de mondes virtuels','',''),('multipart/alternative','&Acirc;','Donn','',''),('multipart/byteranges','&Acirc;','Donn','',''),('multipart/digest','&Acirc;','Donn','',''),('multipart/encrypted','&Acirc;','Donn','',''),('multipart/form-data','&Acirc;','Donn','',''),('multipart/mixed','&Acirc;','Donn','',''),('multipart/parallel','&Acirc;','Donn','',''),('multipart/related','&Acirc;','Donn','',''),('multipart/report','&Acirc;','Donn','',''),('multipart/signed','&Acirc;','Donn','',''),('multipart/voice-message','&Acirc;','Donn','',''),('text/comma-separated-values','*.csv','Fichiers de donn','',''),('text/css','*.css','Fichiers de feuilles de style CSS','',''),('text/html','*.htm *.html *.shtml','Fichiers - HTML','mime_type.text_html.gif',''),('text/javascript','*.js','Fichiers JavaScript','',''),('text/plain','*.txt','Fichiers pur texte','mime_type.text_plain.gif',''),('text/richtext','*.rtx','Fichiers texte enrichi (Richtext)','',''),('text/rtf','*.rtf','Fichiers Microsoft RTF','mime_type.application_rtf.gif',''),('text/tab-separated-values','*.tsv','Fichiers de donn','',''),('text/vnd.wap.wml','*.wml','Fichiers WML (WAP)','',''),('application/vnd.wap.wmlc','*.wmlc','Fichiers WMLC (WAP)','',''),('text/vnd.wap.wmlscript','*.wmls','Fichiers script WML (WAP)','',''),('application/vnd.wap.wmlscriptc','*.wmlsc','Fichiers script C WML (WAP)','',''),('text/xml-external-parsed-entity','&Acirc;','Fichiers XML &Atilde;  l\'analyse syntaxique externe','',''),('text/x-setext','*.etx','Fichiers SeText','',''),('text/x-sgml','*.sgm *.sgml','Fichiers SGML','',''),('text/x-speech','*.talk *.spc','Fichiers Speech','',''),('video/mpeg','*.mpeg *.mpg *.mpe','Fichiers MPEG','',''),('video/quicktime','*.qt *.mov','Fichiers Quicktime','',''),('video/vnd.vivo','*viv *.vivo','Fichiers Vivo','',''),('video/x-msvideo','*.avi','Fichiers Microsoft AVI','',''),('video/x-sgi-movie','*.movie','Fichiers Movie','',''),('workbook/formulaone','*.vts *.vtts','Fichiers FormulaOne','',''),('x-world/x-3dmf','*.3dmf *.3dm *.qd3d *.qd3','3Fichiers DMF','',''),('x-world/x-vrml','*.wrl','Fichiers VRML','','');
/*!40000 ALTER TABLE `type_mime` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_lost_pass`
--

DROP TABLE IF EXISTS `user_lost_pass`;
CREATE TABLE `user_lost_pass` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `login` varchar(50) NOT NULL,
  `code` int(11) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `user_lost_pass`
--

LOCK TABLES `user_lost_pass` WRITE;
/*!40000 ALTER TABLE `user_lost_pass` DISABLE KEYS */;
/*!40000 ALTER TABLE `user_lost_pass` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `usersInfo`
--

DROP TABLE IF EXISTS `usersInfo`;
CREATE TABLE `usersInfo` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `login` varchar(50) NOT NULL,
  `pass` varchar(250) NOT NULL default '',
  `actif` enum('N','Y') NOT NULL default 'N',
  `dateCreation` timestamp NOT NULL default CURRENT_TIMESTAMP,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `login` (`login`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Coordonn�es des utilisateurs';

--
-- Dumping data for table `usersInfo`
--

LOCK TABLES `usersInfo` WRITE;
/*!40000 ALTER TABLE `usersInfo` DISABLE KEYS */;
INSERT INTO `usersInfo` VALUES (1,'default','','Y','2006-09-26 13:41:50'),(3,'admin','*4ACFE3202A5FF5CF467898FC58AAB1D615029441','Y','2007-04-17 07:48:38');
/*!40000 ALTER TABLE `usersInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users_auth_log`
--

DROP TABLE IF EXISTS `users_auth_log`;
CREATE TABLE `users_auth_log` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `date` datetime NOT NULL default '0000-00-00 00:00:00',
  `login` int(11) NOT NULL,
  `host` varchar(255) NOT NULL default '',
  `result` enum('Y','N') NOT NULL default 'Y',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Log des identifications';

--
-- Dumping data for table `users_auth_log`
--

LOCK TABLES `users_auth_log` WRITE;
/*!40000 ALTER TABLE `users_auth_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `users_auth_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `usersmenu`
--

DROP TABLE IF EXISTS `usersmenu`;
CREATE TABLE `usersmenu` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `login` int(11) NOT NULL,
  `rang` int(5) NOT NULL default '0',
  `id_menu` int(5) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `usersmenu`
--

LOCK TABLES `usersmenu` WRITE;
/*!40000 ALTER TABLE `usersmenu` DISABLE KEYS */;
INSERT INTO `usersmenu` VALUES (1,0,1,20),(3,0,3,19),(5,0,6,18),(7,0,8,34),(9,0,9,23),(12,0,10,26),(17,0,4,16);
/*!40000 ALTER TABLE `usersmenu` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `usersmodule`
--

DROP TABLE IF EXISTS `usersmodule`;
CREATE TABLE `usersmodule` (
  `login` varchar(8) NOT NULL default '',
  `module` varchar(25) NOT NULL default '',
  `position` int(2) default NULL,
  `id` int(5) NOT NULL auto_increment,
  `deroule` enum('Y','N') NOT NULL default 'Y',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=80 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `usersmodule`
--

LOCK TABLES `usersmodule` WRITE;
/*!40000 ALTER TABLE `usersmodule` DISABLE KEYS */;
INSERT INTO `usersmodule` VALUES ('default','module_identification',1,1,'Y'),('default','module_add_module',2,2,'Y'),('default','module_sommaire',3,3,'Y'),('default','module_survey',4,4,'Y'),('default','module_mpub',5,5,'Y'),('','module_welcome',1,6,'Y'),('9','9',1,79,'Y'),('9','module_agenda',6,63,'Y'),('9','module_forum',5,64,'N'),('6','module_todolist',3,11,'Y'),('9','9',2,78,'Y'),('9','module_news',3,73,'N'),('6','module_notes',2,18,'N'),('6','module_agenda',1,74,'Y'),('9','module_todolist',9,24,'Y'),('9','module_notes',8,25,'Y'),('9','module_traduction',7,26,'Y'),('9','module_trombi',4,65,'Y'),('1','module_news_tag',1,54,'Y');
/*!40000 ALTER TABLE `usersmodule` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `usersprefs2`
--

DROP TABLE IF EXISTS `usersprefs2`;
CREATE TABLE `usersprefs2` (
  `login` int(11) NOT NULL,
  `pref_name` varchar(255) NOT NULL default '',
  `pref_value` longtext,
  PRIMARY KEY  (`login`,`pref_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Table regrouppant toutes les pr�f�rences des utilisate';

--
-- Dumping data for table `usersprefs2`
--

LOCK TABLES `usersprefs2` WRITE;
/*!40000 ALTER TABLE `usersprefs2` DISABLE KEYS */;
INSERT INTO `usersprefs2` VALUES (9,'site_check_mail','false'),(9,'site_skin','cci_aube'),(6,'site_check_mail','false'),(6,'site_skin','default'),(6,'event_type','month'),(1,'forum_email',''),(1,'forum_myavatar','true'),(1,'forum_nbrsuivit','6'),(1,'forum_pseudo','Grumlin'),(1,'forum_signature','thjghjkg [b]ghjgjhkg[/b] hjkghjh gjghjghjk jhghjkghjk'),(1,'forum_mysignature','true'),(9,'event_type','month'),(9,'forum_pseudo','Nico'),(9,'forum_email',''),(9,'forum_myavatar','true'),(9,'forum_mysignature','true'),(9,'forum_modoavatar','false'),(9,'forum_nbrsuivit','6'),(9,'forum_signature','Nico'),(6,'forum_modoavatar','false'),(3,'event_type','month');
/*!40000 ALTER TABLE `usersprefs2` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'wope'
--
DELIMITER ;;
/*!50003 DROP FUNCTION IF EXISTS `addModuleToUser` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 FUNCTION `addModuleToUser`( _module INT UNSIGNED, _user INT UNSIGNED, _column INT UNSIGNED) RETURNS int(10) unsigned
    DETERMINISTIC
BEGIN
	DECLARE _result INT UNSIGNED;
	
	UPDATE kernel_module_user_module SET position = position + 1 WHERE id_user = _user AND id_column = _column;
	INSERT INTO kernel_module_user_module (id_user, id_module, theme, id_column, position)
		SELECT _user, _module, mm.default_theme, _column, '1' FROM kernel_module_module mm WHERE mm.id = _module ;
	
	SELECT LAST_INSERT_ID() INTO _result;
	RETURN _result;
	
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `addDefaultModulesToUser` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `addDefaultModulesToUser`(
	IN _user INT UNSIGNED
	)
BEGIN
	
	START TRANSACTION;
	
	DELETE FROM kernel_module_user_tab WHERE id_user = _user;
	INSERT INTO kernel_module_user_tab (id_user, id_tab, position, columns)
		SELECT _user, mt.id, mt.default_position, (SELECT COUNT(*) FROM kernel_module_column mc WHERE mc.id_tab = mt.id AND mc.available = 'Y' ) FROM kernel_module_tab mt WHERE mt.available = 'Y' AND ( mt.default_tab = 'Y' OR mt.mandatory = 'Y')
	;
	DELETE FROM kernel_module_user_column WHERE id_user = _user;
	INSERT INTO kernel_module_user_column (id_user, id_column, id_tab)
		SELECT 
			_user,
			mc.id,
			(SELECT mut.id FROM kernel_module_user_tab mut WHERE mut.id_user = _user AND mut.id_tab = mc.id_tab)
		FROM kernel_module_column mc
		WHERE mc.available = 'Y' AND ( mc.default_column = 'Y' OR mc.mandatory = 'Y')
	;
	
	INSERT INTO kernel_module_user_module (id_user, id_module, theme, id_column, position)
		SELECT
			muc.id_user,
			mm.id,
			mm.default_theme,
			muc.id,
			mm.default_position
		FROM ( kernel_module_module mm
			INNER JOIN kernel_module_user_column muc ON mm.default_column = muc.id_column )
				INNER JOIN kernel_module_available ma ON mm.id = ma.id_module
		WHERE mm.default_module = 'Y'
			AND muc.id_user = _user
			AND (
				( ma.strict= 'Y' AND _user IN ( SELECT login FROM kernel_right_user_get WHERE idRight = ma.right ) )
				OR
				( ma.strict= 'N' AND _user IN ( SELECT login	FROM kernel_right_user WHERE idRight = ma.right )	)
			)
	;
	COMMIT;
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `checkMandatoryColumnsForUser` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `checkMandatoryColumnsForUser`(
	IN _user INT UNSIGNED
	)
BEGIN
	DECLARE done SMALLINT UNSIGNED DEFAULT 0;
	DECLARE data INT UNSIGNED;
	DECLARE col SMALLINT UNSIGNED;
	
	DECLARE cur CURSOR FOR
	SELECT
		mc.id,
		IF( mc.id IN (SELECT muc.id_column FROM kernel_module_user_column muc WHERE muc.id_user = _user), 1, 0) AS isData
	FROM `kernel_module_column` mc
	WHERE mc.available = 'Y' AND mc.mandatory = 'Y'
	;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	
	OPEN cur;
	REPEAT
		FETCH cur INTO col, data;
		IF NOT done THEN
			IF NOT data THEN
			
				INSERT INTO `kernel_module_user_column` (id_user, id_column ) VALUES 
					( _user, col);
			END IF;
		END IF;
	UNTIL done END REPEAT;
	CLOSE cur;
	
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `checkMandatoryModulesForUser` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `checkMandatoryModulesForUser`(
	IN _user INT UNSIGNED
	)
BEGIN
	DECLARE done SMALLINT UNSIGNED DEFAULT 0;
	DECLARE module, data, col, cl INT UNSIGNED;
	DECLARE min ENUM('Y','N');
	DECLARE thm, ps SMALLINT UNSIGNED;
	DECLARE them, pos SMALLINT UNSIGNED;
	
	DECLARE cur CURSOR FOR
	SELECT
		mm.id,
		mm.default_theme,
		muc.id,
		mm.default_position,
		IF( mm.id IN (SELECT mum.id_module FROM kernel_module_user_module mum WHERE mum.id_user = _user), 1, 0) AS isData
	FROM ( ( `kernel_module_module` mm
		INNER JOIN kernel_module_user_column muc ON mm.default_column = muc.id_column )
			INNER JOIN `kernel_module_mandatory` mma ON mm.id = mma.id_module )
				INNER JOIN `kernel_module_available` ma ON mm.id = ma.id_module
	WHERE muc.id_user = _user
		AND (
				( mma.strict = 'Y' AND _user IN ( SELECT login FROM `kernel_right_user_get` WHERE idRight = mma.right ) )
				OR
				( mma.strict = 'N' AND _user IN ( SELECT login	FROM `kernel_right_user` WHERE idRight = mma.right ) )
			) AND (
				( ma.strict = 'Y' AND _user IN ( SELECT login FROM `kernel_right_user_get` WHERE idRight = ma.right ) )
					OR
				( ma.strict = 'N' AND _user IN ( SELECT login	FROM `kernel_right_user` WHERE idRight = ma.right )	)
			)
	;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	
	OPEN cur;
	REPEAT
		FETCH cur INTO module, them, col, pos, data;
		IF NOT done THEN
			IF data THEN
			
				SELECT 
					mum.theme,
					mum.id_column,
					mum.position,
					mum.minimized
				INTO thm, cl, ps, min
				FROM `kernel_module_user_module` mum 
				WHERE mum.id_user = _user AND mum.id_module = module;
				
				IF ( thm <> them OR cl <> col OR ps <> pos OR min <> 'N' ) THEN
					UPDATE `kernel_module_user_module` SET theme = them, id_column = col, position = pos, minimized = 'N' WHERE id_user = _user AND id_module = module;
				END IF;
			ELSE
				INSERT INTO `kernel_module_user_module` (id_user, id_module, theme, id_column, position, minimized ) VALUES 
					( _user, module, them, col, pos, 'N');
			END IF;
		END IF;
	UNTIL done END REPEAT;
	CLOSE cur;
	
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `checkModuleOrderFromUser` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `checkModuleOrderFromUser`(
	IN _user INT UNSIGNED
	)
BEGIN
	DECLARE done SMALLINT UNSIGNED DEFAULT 0;
	DECLARE module INT UNSIGNED;
	DECLARE col, curCol, pos, curPos SMALLINT UNSIGNED DEFAULT 0;
	
	DECLARE cur CURSOR FOR
		SELECT id, id_column, position FROM kernel_module_user_module WHERE id_user = _user ORDER BY id_column ASC, position ASC;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	
	OPEN cur;
	REPEAT
		FETCH cur INTO module, col, pos;
		IF NOT done THEN
		
			IF ( col <> curCol) THEN
				SET curCol = curCol + 1;
				SET curPos = 0;
			END IF;
			SET curPos = curPos + 1;
			
			IF ( pos <> curPos ) THEN
				UPDATE kernel_module_user_module SET position = curPos WHERE id = modules;
			END IF;
		END IF;
	UNTIL done END REPEAT;
	CLOSE cur;
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `checkModulesOrderForUser` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `checkModulesOrderForUser`(
	IN _user INT UNSIGNED
	)
BEGIN
	DECLARE done SMALLINT UNSIGNED DEFAULT 0;
	DECLARE module, col, curCol INT UNSIGNED;
	DECLARE pos, curPos SMALLINT UNSIGNED DEFAULT 1;
	
	DECLARE cur CURSOR FOR
		SELECT id, id_column, position FROM kernel_module_user_module WHERE id_user = _user ORDER BY id_column ASC, position ASC;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	
	SET curCol = 0;
	OPEN cur;
	REPEAT
		FETCH cur INTO module, col, pos;
		IF NOT done THEN
		
			IF ( col <> curCol) THEN
				SET curPos = 1;
				SET curCol = col;
			END IF;
			
			IF ( pos <> curPos ) THEN
				UPDATE kernel_module_user_module SET position = curPos WHERE id = module;
			END IF;
			
			SET curPos = curPos + 1;
		END IF;
	UNTIL done END REPEAT;
	CLOSE cur;
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `deleteRight` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `deleteRight`(IN _id INT)
BEGIN
	DECLARE _oldLft INT;
	DECLARE _oldRgt INT;
	DECLARE _size INT;
	
	SELECT `lft`,`rgt`
	INTO _oldLft, _oldRgt
	FROM kernel_right
	WHERE id=_id;
	SET _size = _oldLft - _oldRgt +1;
	
	START TRANSACTION;
	
	DELETE FROM kernel_right WHERE `lft`>=_oldLft AND `rgt`<=_oldRgt;
	
	UPDATE kernel_right SET `lft` = `lft` - _size WHERE `lft`>_oldLft;
	UPDATE kernel_right SET `rgt` = `rgt` - _size WHERE `rgt`>_oldLft;
	
	COMMIT;
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `insertRight` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `insertRight`(
		IN _name VARCHAR(25),
		IN _parent INT,
		IN _description VARCHAR(30))
BEGIN
	DECLARE _parentLft SMALLINT;
	IF (_parent IS NULL ) THEN
		
		INSERT INTO kernel_right ( name, description, parent, lft, rgt)
			VALUES ( _name, _description, NULL, 1, 2);
	ELSE
		
		SET _parentLft = NULL;
		SELECT lft INTO _parentLft FROM kernel_right WHERE id=_parent;
		IF (_parentLft IS NOT NULL) THEN
			
			UPDATE kernel_right SET lft = lft +2 WHERE lft > _parentLft;
			UPDATE kernel_right SET rgt = rgt +2 WHERE rgt > _parentLft;
			
			INSERT INTO kernel_right ( name, description, parent, lft, rgt)
				VALUES ( _name, _description, _parent, _parentLft+1, _parentLft+2);
		END IF;
	END IF;
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `moveModuleDown` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `moveModuleDown`(
	IN _module INT UNSIGNED
	)
BEGIN
	DECLARE us, col INT UNSIGNED;
	DECLARE pos, maxPos SMALLINT UNSIGNED;
	
	START TRANSACTION;
	
	SELECT id_user, id_column, position INTO us, col, pos FROM kernel_module_user_module WHERE id = _module;
	SELECT MAX(position) INTO maxPos FROM kernel_module_user_module WHERE id_user = us AND id_column = col;
	
	IF ( pos <> maxPos ) THEN
		UPDATE kernel_module_user_module SET position = position - 1 WHERE id_user = us AND id_column = col AND position = pos + 1;
		UPDATE kernel_module_user_module SET position = position + 1 WHERE id = _module;
	ELSE
		UPDATE kernel_module_user_module SET position = position + 1 WHERE id_user = us AND id_column = col;
		UPDATE kernel_module_user_module SET position = 1 WHERE id = _module;
	END IF;
		
	COMMIT;
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `moveModuleUp` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `moveModuleUp`(
	IN _module INT UNSIGNED
	)
BEGIN
	DECLARE col, us INT UNSIGNED;
	DECLARE pos, maxPos SMALLINT UNSIGNED;
	
	START TRANSACTION;
	
	SELECT id_user, id_column, position INTO us, col, pos FROM kernel_module_user_module WHERE id = _module;
	IF ( pos <> 1 ) THEN
		UPDATE kernel_module_user_module SET position = position + 1 WHERE id_user = us AND id_column = col AND position = pos - 1;
		UPDATE kernel_module_user_module SET position = position - 1 WHERE id = _module;
	ELSE
		SELECT MAX(position) INTO maxPos FROM kernel_module_user_module WHERE id_user = us AND id_column = col;
		
		UPDATE kernel_module_user_module SET position = position - 1 WHERE id_user = us AND id_column = col;
		UPDATE kernel_module_user_module SET position = maxPos WHERE id = _module;
	END IF;
		
	COMMIT;
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `removeModuleFromUser` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `removeModuleFromUser`(
	IN _module INT UNSIGNED,
	IN _user INT UNSIGNED
	)
BEGIN
	DECLARE pos SMALLINT UNSIGNED;
	DECLARE col INT UNSIGNED;
	
	START TRANSACTION;
	SELECT id_column, position INTO col, pos FROM kernel_module_user_module WHERE id = _module;
	
	UPDATE kernel_module_user_module SET position = position - 1
	WHERE id_user = _user	AND id_column = col	AND position > pos;
	DELETE FROM kernel_module_user_module WHERE id = _module;
		
	COMMIT;
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
/*!50003 DROP PROCEDURE IF EXISTS `updateRight` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `updateRight`(
		IN _id INT,
		IN _name VARCHAR(25),
		IN _description VARCHAR(30),
		IN _parent INT)
BEGIN
	DECLARE _oldParent SMALLINT;
	DECLARE _oldLft INT;
	DECLARE _oldRgt INT;
	DECLARE _newParentRgt INT;
	DECLARE _newParentLft INT;
	DECLARE _treeSize INT;
	DECLARE _offset INT;
	
	SELECT
		parent, lft, rgt
	INTO
		_oldParent,_oldLft, _oldRgt
	FROM kernel_right
	WHERE id=_id;
	IF (_oldParent = _parent) THEN
		
		UPDATE kernel_right SET name=_name, description=_description WHERE id=_id;
	ELSE
		
		
		SELECT kernel_right.lft, kernel_right.rgt INTO _newParentLft, _newParentRgt FROM kernel_right WHERE id=_parent;
		
		IF ( _oldLft > _newParentLft OR _oldRgt < _newParentRgt) THEN
			
			UPDATE kernel_right SET name=_name, description=_description, parent=_parent WHERE id=_id;
			
			CREATE TEMPORARY TABLE kernel_right_temp LIKE kernel_right;
			
			INSERT kernel_right_temp (id, name, description, parent, lft, rgt)
				SELECT kernel_right.id, kernel_right.name, kernel_right.description, kernel_right.parent, kernel_right.lft, kernel_right.rgt
				FROM kernel_right
				WHERE kernel_right.lft >= _oldLft AND kernel_right.rgt <= _oldRgt;
			SET _offset = _newParentLft - _oldLft + 1;
			SET _treeSize = _oldRgt - _oldLft + 1;
			
			UPDATE kernel_right_temp SET lft = lft + _offset, rgt = rgt + _offset;
			
			START TRANSACTION;
			SET FOREIGN_KEY_CHECKS = 0;
			
			DELETE FROM kernel_right WHERE lft >= _oldLft AND rgt <= _oldRgt;
			UPDATE kernel_right SET lft=lft - _treeSize WHERE lft >= _oldLft;
			UPDATE kernel_right SET rgt=rgt - _treeSize WHERE rgt >= _oldLft;
			
			UPDATE kernel_right SET lft=lft + _treeSize WHERE lft > _newParentLft;
			UPDATE kernel_right SET rgt=rgt + _treeSize WHERE rgt > _newParentLft;
			
			
			
			INSERT INTO kernel_right (id, name, description, parent, lft, rgt)
				SELECT id, name, description, parent, lft, rgt
				FROM kernel_right_temp;
			
			SET FOREIGN_KEY_CHECKS = 1;
			
			COMMIT;
			
			DROP TABLE kernel_right_temp;
		END IF;
	END IF;
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
DELIMITER ;

--
-- Final view structure for view `kernel_module_user_module_available`
--

/*!50001 DROP TABLE IF EXISTS `kernel_module_user_module_available`*/;
/*!50001 DROP VIEW IF EXISTS `kernel_module_user_module_available`*/;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `kernel_module_user_module_available` AS select `ui`.`id` AS `idUser`,`mm`.`id` AS `id`,`mm`.`title` AS `title`,`mm`.`instantiable` AS `instantiable` from ((`kernel_module_module` `mm` join `kernel_module_available` `ma` on((`mm`.`id` = `ma`.`id_module`))) join `usersInfo` `ui`) where ((`ui`.`actif` = _utf8'Y') and (((`ma`.`strict` = _utf8'Y') and `ui`.`id` in (select `kernel_right_user_get`.`login` AS `login` from `kernel_right_user_get` where (`kernel_right_user_get`.`idRight` = `ma`.`right`))) or ((`ma`.`strict` = _utf8'N') and `ui`.`id` in (select `kernel_right_user`.`login` AS `login` from `kernel_right_user` where (`kernel_right_user`.`idRight` = `ma`.`right`)))) and (not(`mm`.`id` in (select `mma`.`id_module` AS `id_module` from `kernel_module_mandatory` `mma` where (((`mma`.`strict` = _utf8'Y') and `ui`.`id` in (select `kernel_right_user_get`.`login` AS `login` from `kernel_right_user_get` where (`kernel_right_user_get`.`idRight` = `mma`.`right`))) or ((`mma`.`strict` = _utf8'N') and `ui`.`id` in (select `kernel_right_user`.`login` AS `login` from `kernel_right_user` where (`kernel_right_user`.`idRight` = `mma`.`right`))))))) and ((`mm`.`instantiable` = _utf8'Y') or ((`mm`.`instantiable` = _utf8'N') and (not(`mm`.`id` in (select `kernel_module_user_module`.`id_module` AS `id_module` from `kernel_module_user_module` where (`kernel_module_user_module`.`id_user` = `ui`.`id`))))))) order by `mm`.`title` */;

--
-- Final view structure for view `kernel_module_user_module_final`
--

/*!50001 DROP TABLE IF EXISTS `kernel_module_user_module_final`*/;
/*!50001 DROP VIEW IF EXISTS `kernel_module_user_module_final`*/;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `kernel_module_user_module_final` AS select `mum`.`id_user` AS `idUser`,`mum`.`id` AS `id`,`mum`.`position` AS `position`,`mm`.`id` AS `idModule`,`mm`.`class` AS `class`,`mm`.`title` AS `title`,`mm`.`instantiable` AS `instantiable`,`mum`.`name` AS `name`,`mum`.`theme` AS `theme`,`mum`.`minimized` AS `minimized`,if((((`mma`.`strict` = _utf8'Y') and `mum`.`id_user` in (select `kernel_right_user_get`.`login` AS `login` from `kernel_right_user_get` where (`kernel_right_user_get`.`idRight` = `mma`.`right`))) or ((`mma`.`strict` = _utf8'N') and `mum`.`id_user` in (select `kernel_right_user`.`login` AS `login` from `kernel_right_user` where (`kernel_right_user`.`idRight` = `mma`.`right`)))),_latin1'Y',_latin1'N') AS `mandatory`,`mum`.`id_column` AS `column`,`mc`.`mandatory` AS `columnMandatory`,`mc`.`width` AS `columnWidth`,`muc`.`id_tab` AS `tab`,`mc`.`user_column` AS `inUserColumn`,`mc`.`drag` AS `drag`,`mc`.`position` AS `columnPosition` from (((((`kernel_module_user_column` `muc` left join (`kernel_module_user_module` `mum` join `kernel_module_module` `mm` on((`mum`.`id_module` = `mm`.`id`))) on((`mum`.`id_column` = `muc`.`id`))) join `kernel_module_column` `mc` on((`muc`.`id_column` = `mc`.`id`))) join `kernel_module_user_tab` `mut` on((`muc`.`id_tab` = `mut`.`id`))) join `kernel_module_available` `ma` on((`mum`.`id_module` = `ma`.`id_module`))) left join `kernel_module_mandatory` `mma` on((`mum`.`id_module` = `mma`.`id_module`))) where (((`ma`.`strict` = _utf8'Y') and `mum`.`id_user` in (select `kernel_right_user_get`.`login` AS `login` from `kernel_right_user_get` where (`kernel_right_user_get`.`idRight` = `ma`.`right`))) or ((`ma`.`strict` = _utf8'N') and `mum`.`id_user` in (select `kernel_right_user`.`login` AS `login` from `kernel_right_user` where (`kernel_right_user`.`idRight` = `ma`.`right`)))) union select `muc2`.`id_user` AS `idUser`,0 AS `id`,0 AS `position`,0 AS `idModule`,_latin1'' AS `class`,_latin1'' AS `title`,_latin1'N' AS `instantiable`,_latin1'' AS `name`,24 AS `theme`,_latin1'N' AS `minimized`,_latin1'N' AS `mandatory`,`muc2`.`id` AS `column`,`mc2`.`mandatory` AS `columnMandatory`,`mc2`.`width` AS `columnWidth`,`muc2`.`id_tab` AS `tab`,`mc2`.`user_column` AS `inUserColumn`,`mc2`.`drag` AS `drag`,`mc2`.`position` AS `columnPosition` from (`kernel_module_user_column` `muc2` join `kernel_module_column` `mc2` on((`muc2`.`id_column` = `mc2`.`id`))) order by `tab`,`columnPosition`,`position` */;

--
-- Final view structure for view `kernel_right_user`
--

/*!50001 DROP TABLE IF EXISTS `kernel_right_user`*/;
/*!50001 DROP VIEW IF EXISTS `kernel_right_user`*/;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `kernel_right_user` AS (select `kernel_right_user_get`.`login` AS `login`,`d1`.`id` AS `idRight` from ((`kernel_right_user_get` join `kernel_right` `d1`) join `kernel_right` `d2`) where ((`kernel_right_user_get`.`idRight` = `d2`.`id`) and (`d1`.`lft` <= `d2`.`lft`) and (`d1`.`rgt` >= `d2`.`rgt`)) group by `kernel_right_user_get`.`login`,`d1`.`id`) */;

--
-- Final view structure for view `kernel_tag_category_user`
--

/*!50001 DROP TABLE IF EXISTS `kernel_tag_category_user`*/;
/*!50001 DROP VIEW IF EXISTS `kernel_tag_category_user`*/;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `kernel_tag_category_user` AS (select `kernel_tag_category`.`id` AS `IdCategory`,`kernel_right_user`.`login` AS `login`,`kernel_tag_category`.`name` AS `name`,`kernel_tag_category`.`description` AS `description`,`kernel_tag_category`.`image` AS `image`,ifnull(`kernel_tag_user_pref`.`isShow`,`kernel_tag_category`.`defaultShow`) AS `isShow` from ((`kernel_right_user` join `kernel_tag_category`) left join `kernel_tag_user_pref` on(((`kernel_right_user`.`login` = `kernel_tag_user_pref`.`login`) and (`kernel_tag_category`.`id` = `kernel_tag_user_pref`.`idCategory`)))) where (`kernel_tag_category`.`rightGet` = `kernel_right_user`.`idRight`)) */;

--
-- Final view structure for view `kernel_tag_category_user_set`
--

/*!50001 DROP TABLE IF EXISTS `kernel_tag_category_user_set`*/;
/*!50001 DROP VIEW IF EXISTS `kernel_tag_category_user_set`*/;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `kernel_tag_category_user_set` AS (select `kernel_tag_category`.`id` AS `idCategory`,`kernel_right_user`.`login` AS `login`,`kernel_tag_category`.`name` AS `name`,`kernel_tag_category`.`image` AS `image`,ifnull(`kernel_tag_user_pref`.`isShow`,`kernel_tag_category`.`defaultShow`) AS `isShow` from ((`kernel_right_user` join `kernel_tag_category`) left join `kernel_tag_user_pref` on(((`kernel_right_user`.`login` = `kernel_tag_user_pref`.`login`) and (`kernel_tag_category`.`id` = `kernel_tag_user_pref`.`idCategory`)))) where (`kernel_tag_category`.`rightSet` = `kernel_right_user`.`idRight`)) */;

--
-- Final view structure for view `listes`
--

/*!50001 DROP TABLE IF EXISTS `listes`*/;
/*!50001 DROP VIEW IF EXISTS `listes`*/;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `listes` AS select `d1`.`name` AS `liste`,if(isnull(`d1`.`parent`),NULL,`d2`.`name`) AS `parent`,`d1`.`description` AS `description` from (`kernel_right` `d1` join `kernel_right` `d2`) where (if(isnull(`d1`.`parent`),`d1`.`id`,`d1`.`parent`) = `d2`.`id`) */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2007-04-19 16:07:45
