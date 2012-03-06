<?php

// GENERAL STRUCTURE OF THE ARRAY :
/*
			$config['tables'][1] => table_A_name.
			$config['tables'][2] => table_A_translation.
			$config['tables'][3] => table_A_title_column.
			$config['tables'][4] => table_A_id_column.
			$config['tables'][5] => table_A_birthdate_column.
			
			$config['tables'][6] => table_B_name.
			...
*/

// NUMBER OF TABLES THAT CAN BE LINKED
	$config['tables_nb'] = 3; /*      /!\  THINK TO MODIFY THE displayInfosSelect() METHOD IF THIS NUMBER CHANGES /!\      */

// DEFINITION OF THE TABLES WHICH CAN BE LINKED TO THE PHOTOS
	$config['tables'] = Array();
	$config['tables'][1] = 'eventsListe';
	$config['tables'][6] = 'news_liste';
	$config['tables'][11] = 'forum_thread';

// DEFINITION OF THE TRANSLATION OF THE TABLES NAMES
	$config['tables'][2] = 'Evénements';
	$config['tables'][7] = 'Actualités';
	$config['tables'][12] = 'Sujets du forum';

// DEFINITION OF THE TITLE COLUMNS
	$config['tables'][3] = 'titre';
	$config['tables'][8] = 'titre';
	$config['tables'][13] = 'nom_thread';

// DEFINITION OF THE ID COLUMNS
	$config['tables'][4] = 'id';
	$config['tables'][9] = 'id_news';
	$config['tables'][14] = 'id';

// DEFINITION OF THE "BIRTHDATE" COLUMNS
	$config['tables'][5] = 'date';
	$config['tables'][10] = 'date_creation';
	$config['tables'][15] = 'date_creation';





// DEFINITIONS OF THE QUERIES USED IN THE APPLICATION

	$config['queries'] = Array();

	// USED IN "MY ALBUM"
		$config['queries'][1] = "SELECT photo.id AS photo_id, 
																		id_photo AS file_id, 
																		photo.id_album,
																		album.title AS title,
																		file.name, 
																		file.login, 
																		file.description, 
																		trombi.lastname, 
																		trombi.firstname
																		
														 FROM photo_album_photos photo, 
														 			photo_album_albums album,
														 			kernel_file_manager_files file, 
																	trombi_trombi trombi 
																	
														 WHERE (photo.id_album = album.id OR photo.id_album IS NULL)
															 AND photo.id_user='".$core->getUserId()."' 
														 	 AND file.id = id_photo 
															 AND file.login = trombi.id_user
														 
														 GROUP BY photo.id
														 ORDER BY photo.id DESC";

	// USED IN "PUBLIC PART"
		$config['queries'][2] = "SELECT photo.id AS photo_id, 
																		id_photo AS file_id, 
																		photo.id_album,
																		album.title AS title,
																		file.name, 
																		file.login, 
																		file.description, 
																		trombi.lastname, 
																		trombi.firstname 
																		
														 FROM photo_album_photos photo,
														 			photo_album_albums album,
														 			kernel_file_manager_files file, 
																	kernel_tag_information info,
																	kernel_tag_tag tag, 
																	kernel_tag_liste_tag liste_tag,
																	trombi_trombi trombi 
																	
														 WHERE (photo.id_album = album.id OR photo.id_album IS NULL)
														 	 AND file.id = id_photo 
														 	 AND info.tableOrig = 'photo_album_photos'
														 	 AND photo.id = info.idOrig
														 	 AND info.id = tag.idInformation 
															 AND tag.idTagName = liste_tag.id
															 AND liste_tag.name = '@shared' 
															 AND file.login = trombi.id_user
															 
														 GROUP BY photo.id
														 ORDER BY photo.id DESC";

	// USED IN "LAST ALBUMS"
		$config['queries'][3] = "SELECT MAX(photo.id_photo) AS cover, 
																		album.id, 
																		album.title, 
																		album.description, 
																		album.date 
																		
													 	 FROM photo_album_albums album, 
														 			photo_album_photos photo, 
																	kernel_file_manager_files file, 
																	kernel_tag_information info,
																	kernel_tag_tag tag,
																	kernel_tag_liste_tag liste_tag,
																	trombi_trombi trombi
																	
														 WHERE photo.id_album = album.id 
														 	 AND info.tableOrig = 'photo_album_photos'
														   AND file.id = photo.id_photo 
															 AND photo.id = info.idOrig
															 AND info.id  = tag.idInformation 
															 AND tag.idTagName = liste_tag.id
															 AND liste_tag.name = '@shared' 
															 
														 GROUP BY album.id 
														 ORDER BY album.id DESC";
		
	// USED TO DISPLAY TARGET ALBUM
		$config['queries'][4] = "SELECT photo.id AS photo_id, 
																		id_album, 
																		album.title AS title,
																		id_photo AS file_id, 
																		file.name, 
																		file.login, 
																		file.description, 
																		trombi.lastname, 
																		trombi.firstname 
																		
														 FROM photo_album_photos photo, 
														 			photo_album_albums album,
														 			kernel_file_manager_files file, 
																	trombi_trombi trombi 
																	
														 WHERE file.id = id_photo 
														 	 AND album.id = photo.id_album
														 	 AND file.login = trombi.id_user 
															 AND photo.id_album = ";


?>
