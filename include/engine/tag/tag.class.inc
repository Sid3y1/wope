<?php
/**
*   Copyright : (C) 2007 Wope
*   License : GNU GPL
*   Contact : http://www.wope-project.org
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
*   MA  02111-1307  USA
*
**/


error_reporting(E_ALL);

/**
 * tagging - class.Tag.php
 *
 * $Id$
 *
 * This file is part of tagging.
 *
 * Automatic generated with ArgoUML 0.22 on 16.10.2006, 17:39:13
 *
 * @author Vincent Keravec
 */


/**
 * Short description of class Tag
 *
 * @access public
 * @author Vincent Keravec
 */
class Tag
{
    // --- ATTRIBUTES ---

    /**
     * Array containing the tableNames=>ids key value pairs
     *
     * @access private
     * @var Array
     */
    private $tableNames = null;

    /**
     * Cache all the categories that this user can see
     *
     * @access private
     * @var Array
     */
    private $cacheValidCategory = null;

    /**
     * Cache the last accessed information
     *
     * @access private
     * @var Array
     */
    private $cacheLastInformation = null;

		protected $core;
		protected $db;
		

    /**
     * Create a Tag object
     *
     * @access public
     * @author Vincent Keravec
     * @param array
     * @return void
     */
    public function __construct($tableNames, $core, $db)
    {
        // section -64--88-0-76--3d256e4f:10e3bbc2a13:-8000:0000000000000B50 begin
        $this->tableNames=$tableNames;
        $this->core=$core;
        $this->db=$db;
        // section -64--88-0-76--3d256e4f:10e3bbc2a13:-8000:0000000000000B50 end
    }


    // --- OPERATIONS ---

    /**
     * Mark an information as starred
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @return boolean
     */
    public function starInformation($idInformation)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000093F begin
        $star=$this->getIdTagName("@star");
        $returnValue=$this->addTagInformation($idInformation,$star,true);
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000093F end

        return (bool) $returnValue;
    }

    /**
     * Mark an information as public / private
     *
     * @access public
     * @author Sylvain Ramboz
     * @param int
     * @return boolean
     */
    public function shareInformation($idInformation)
    {
        $returnValue = (bool) false;

        $share=$this->getIdTagName("@shared");
        $returnValue=$this->addTagInformation($idInformation,$share,true);

        return (bool) $returnValue;
    }

    /**
     * Add a tag to an information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param int
     * @param boolean
     * @return boolean
     */
    public function addTagInformation($idInformation, $idTagName, $perso)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000941 begin
        
        $idInformation = (int) $idInformation;
        $idTagName= (int) $idTagName;
        
        if (isset($perso) && $perso) {
        	$persoValue="true";
        } else {
        	$persoValue="false";	
        }
        
        $user = (int)$this->core->getUserId();
                
	    $query="INSERT INTO kernel_tag_tag SET "
	    		."idTagName=$idTagName, "
	    		."idInformation=$idInformation, "
	    		."author=$user, "
	    		."perso='$persoValue' "
	    		."ON DUPLICATE KEY UPDATE perso='$persoValue'";
        if ($this->db->query($query)) {
        	$returnValue=true;
        }
        
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000941 end

        return (bool) $returnValue;
    }

    /**
     * Remove a tag from an information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param int
     * @return boolean
     */
    public function removeTagInformation($idInformation, $idTagName)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000943 begin
        
        $idInformation = (int) $idInformation;
        $user = (int)$this->core->getUserId();
        $idTagName= (int) $idTagName;
       	
       	$query="DELETE FROM kernel_tag_tag WHERE idInformation=$idInformation AND author=$user AND idTagName=$idTagName";
		if ($this->db->query($query)) {
			// 2 idTagnames reserved : 1=@digg, 2=@star 
			if ($idTagName>2) {
				// Check if the tag is not used anymore
				$query="SELECT COUNT(*) FROM kernel_tag_tag WHERE idTagName=$idTagName";
				if ($result=$this->db->query($query)) {
					if($this->db->result($result,0)==0) {
						// The tag is not used anymore Autocleaning...
						$query = "DELETE FROM kernel_tag_liste_tag WHERE id=$idTagName";
						$this->db->query($query);
					}
				}
			}
			$returnValue=true;
		}
       	       	
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000943 end

        return (bool) $returnValue;
    }

    /**
     * Add a comment to an information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param string
     * @return boolean
     */
    public function addComment($idInformation, $texte)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000945 begin
        $idInformation = (int) $idInformation;
        $user = (int)$this->core->getUserId();
        $texte = addslashes($texte);
        $query="INSERT INTO kernel_tag_comments ( idInformation, author, texte, date) "
        	." VALUES ($idInformation, $user, '$texte', NOW())";
        if ($this->db->query($query)) {
        	$returnValue=true;
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000945 end

        return (bool) $returnValue;
    }

    /**
     * Mark an information as digg
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @return boolean
     */
    public function diggInformation($idInformation)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000947 begin
        $digg=$this->getIdTagName("@digg");
        $returnValue=$this->addTagInformation($idInformation,$digg,false);
        $query="UPDATE kernel_tag_information set cacheNbDigg=cacheNbDigg+1";
        
        if ($this->cacheLastInformation['id']==$idInformation) {
        	$this->cacheLastInformation['cacheNbDigg']++;
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000947 end

        return (bool) $returnValue;
    }
	
	public function isDigg($idInformation)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000095F begin
        $idInformation = (int) $idInformation;
        $user = (int)$this->core->getUserId();
        $digg=$this->getIdTagName("@digg");
        $query = "SELECT count(*) "
    			."FROM `kernel_tag_tag` "
    			."WHERE idInformation=$idInformation and author=$user and idTagName=$digg";
        if ($result=$this->db->query($query)) {
        	if ($this->db->result($result,0)==1) {
        		$returnValue=true;
        	}
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000095F end

        return (bool) $returnValue;
    }
	
    /**
     * Remove the star on an Information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @return boolean
     */
    public function unstarInformation($idInformation)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000949 begin
        $star=$this->getIdTagName("@star");
        $returnValue=$this->removeTagInformation($idInformation,$star);
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000949 end

        return (bool) $returnValue;
    }
		
    /**
     * Remove the sharing on an Information
     *
     * @access public
     * @author Sylvain Ramboz
     * @param int
     * @return boolean
     */
    public function unshareInformation($idInformation)
    {
        $returnValue = (bool) false;

        $share=$this->getIdTagName("@shared");
        $returnValue=$this->removeTagInformation($idInformation,$share);

        return (bool) $returnValue;
    }

    /**
     * Specify a possible alias to the administrator
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param int
     * @return boolean
     */
    public function notifyAlias($idTagName1, $idTagName2)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000094B begin
        $idTagName1= (int) $idTagName1;
        $idTagName2= (int) $idTagName2;
        $query = "INSERT INTO `kernel_tag_alias_notify` VALUES ($idTagName1,$idTagName2)";
        if ($this->db->query($query)) {
        	$returnValue=true;
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000094B end

        return (bool) $returnValue;
    }

    /**
     * Search all the informations containing the specified tag
     *
     * @access public
     * @author Vincent Keravec
     * @param string
     * @return array
     */
    public function searchTag($idTagName)
    {
        $returnValue = array();

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000094D begin
        $tableListQuery="";
        foreach (array_keys($this->tableNames) as $tableName) {
       		if ($tableListQuery=="") {
       			$tableListQuery="kernel_tag_information.tableOrig='$tableName'";
       		} else {
       			$tableListQuery.=" OR kernel_tag_information.tableOrig='$tableName'";
       		}
       	}
        $idTagName= (int) $idTagName;
		$query="SELECT kernel_tag_information.id, kernel_tag_information.tableOrig, kernel_tag_information.idOrig "
			."FROM kernel_tag_information, kernel_tag_tag "
			."WHERE kernel_tag_information.id=kernel_tag_tag.idInformation AND kernel_tag_tag.idTagName=$idTagName AND ($tableListQuery)";
		if ($result=$this->db->query($query)) {
       		while ($row=$this->db->fetchArray($result)) {
       			$returnValue[]=$row;
       		}
       	}
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000094D end

        return (array) $returnValue;
    }

    /**
     * Remove a comment on an information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @return boolean
     */
    public function removeComment($idComment)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000094F begin
        $idComment = (int) $idComment;
        $user = (int)$this->core->getUserId();
        
		if ($this->checkModerateurUser()) {
       		$query = "DELETE FROM kernel_tag_comments WHERE id=$idComment";
       	} else {
       		$query = "DELETE FROM kernel_tag_comments WHERE id=$idComment and author=$user";
       	}
       	if ($this->db->query($query)) {
       		$returnValue=true;
       	}
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000094F end

        return (bool) $returnValue;
    }

    /**
     * Accept a proposed alias
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param int
     * @return boolean
     */
    public function acceptAlias($idTagName1, $idTagName2)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000951 begin
        if ($this->checkModerateurUser()) {
	        $idTagName1=(int) $idTagName1;
	        $idTagName2=(int) $idTagName2;
	        $query = "SELECT IFNULL(alias, id) FROM `kernel_tag_liste_tag` WHERE id=$idTagName1";
	        $result = $this->db->query($query);
	        if ($result) {
	        	$tagNameParent=$this->db->result($result,0);
	        	$query = "UPDATE `kernel_tag_liste_tag` SET alias=$tagNameParent WHERE id=$idTagName2";
	        	if ($this->db->query($query)) {
	        		$query = "DELETE FROM `kernel_tag_alias_notify` "
	        				."WHERE "
	        					."(idTag1=$idTagName1 and idTag2=$idTagName2) "
	        				."OR "
	        					."(idTag2=$idTagName1 and idTag1=$idTagName2)";
	        		$this->db->query($query);
	        		$returnValue=true;
	        	}
	        }
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000951 end

        return (bool) $returnValue;
    }

    /**
     * Deny a proposed alias
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param int
     * @return boolean
     */
    public function denyAlias($idTagName1, $idTagName2)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000953 begin
        if ($this->checkModerateurUser()) {
	        $idTagName1=(int) $idTagName1;
	        $idTagName2=(int) $idTagName2;
	        
	        $query = "DELETE FROM `kernel_tag_alias_notify` "
					."WHERE "
						."(idTag1=$idTagName1 and idTag2=$idTagName2) "
					."OR "
						."(idTag2=$idTagName1 and idTag1=$idTagName2)";
	        
	        if ($this->db->query($query)) {
	        	$returnValue=true;	
	        }
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000953 end

        return (bool) $returnValue;
    }

    /**
     * Add a category
     *
     * @access public
     * @author Vincent Keravec
     * @param string
     * @param string
     * @param string
     * @param boolean
     * @return boolean
     */
    public function addCategory($name, $rightGet, $rightSet, $image, $defaultShow, $description)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000955 begin
        if ($this->checkAdminUser()) {
        	$defaultShow=($defaultShow?"true":"false");
	        $name= addslashes($name);
	        $image= addslashes($image);
	        $description= addslashes($description);
	        $rightGet= (int)$rightGet;
	        $rightSet= (int)$rightSet;
	        if (is_null($image) || $image=='') {
	        	$updateImage=false;	
	        } else {
	        	$updateImage=true;	
	        }
	        $query="INSERT INTO `kernel_tag_category` (`name`,`rightGet`,`rightSet`".($updateImage?",`image`":"").",`defaultShow`,`description`) "
	        		."VALUES ('$name',$rightGet,$rightSet".($updateImage?",'$image'":"").",'$defaultShow','$description')";
	        
	        if ($this->db->query($query)) {
	        	$returnValue=true;	
	        }
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000955 end

        return (bool) $returnValue;
    }

    /**
     * Remove a category
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @return boolean
     */
    public function removeCategory($id)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000957 begin
        if ($this->checkAdminUser()) {
	        $id= (int) $id;
	        
	        $query="DELETE FROM `kernel_tag_category` WHERE `id`=$id";
	        
	        if ($this->db->query($query)) {
	        	$returnValue=true;	
	        }
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000957 end

        return (bool) $returnValue;
    }

    /**
     * Modify a category
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param string
     * @param string
     * @param string
     * @param boolean
     * @return boolean
     */
    public function modifyCategory($id, $name, $rightGet, $rightSet, $image, $defaultShow, $description = '')
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000959 begin
        if ($this->checkAdminUser()) {
        	$id= (int) $id;
        	$defaultShow=( $defaultShow?"true":"false");
	        $name = addslashes($name);
	        $image = addslashes($image);
	        $rightGet = (int)$rightGet;
	        $rightSet = (int)$rightSet;
	        $description = addslashes($description);
	        
	        if (is_null($image) || $image == '') {
	        	$updateImage = false;
	        } else {
	        	$updateImage = true;	
	        }
	        $query="UPDATE `kernel_tag_category` "
	        		."SET `name`='$name',`rightGet`=$rightGet,`rightSet`=$rightSet,`defaultShow`='$defaultShow',`description`='$description' "
	        		.($updateImage?",`image`='$image'":"")
	        		."WHERE `id`=$id";
	        if ($this->db->query($query)) {
	        	$returnValue=true;
	        }
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000959 end

        return (bool) $returnValue;
    }

    /**
     * Get the number of digg of this information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @return int
     */
    public function getNbDigg($idInformation)
    {
        $returnValue = (int) 0;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000095B begin
        $idInformation=(int)$idInformation;
        if ($this->cacheLastInformation!=null 
        		&& isset($this->cacheLastInformation['id'])
        		&& $this->cacheLastInformation['id']==$idInformation){
        	$returnValue=$this->cacheLastInformation['cacheNbDigg'];
        } else {
        	$query="SELECT `id`,`tableOrig`,`idOrig`,`cacheNbDigg`,IF(`link` IS NULL,'',`link`) "
        			."FROM `kernel_tag_information` "
        			."WHERE id=$idInformation";
        	if ($result=$this->db->query($query)) {
        		if ($row=$this->db->fetchArray($result)) {
        			$this->cacheLastInformation=$row;
        			$returnValue=$row['cacheNbDigg'];
        		}
        	}
        }
        
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000095B end

        return (int) $returnValue;
    }

    /**
     * Get an array containing all the comments of this information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param int
     * @param int
     * @return array
     */
    public function getComments($idInformation, $start, $limit)
    {
        $returnValue = array();

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000095D begin
        $idInformation=(int)$idInformation;
        $start=(int) $start;
        $limit=(int) $limit;
        
        $query = "SELECT comm.id, tt.lastname, tt.firstname, comm.idInformation, comm.texte, comm.date "
    			."FROM `kernel_tag_comments` comm, trombi_trombi tt "
    			."WHERE comm.idInformation=$idInformation "
    			."		AND tt.id_user=comm.author "
    			."ORDER BY id ASC ";/*
    			."LIMIT $start,$limit";*/ 
        if ($result=$this->db->query($query)) {
					$temp['comms_nb'] = $this->db->numRows($result);
        	while ($row=$this->db->fetchArray($result)) {
        		// Revert the userName to the original
        		$temp['author']=stripslashes($row['lastname']." ".$row['firstname']);
        		$temp['texte']=stripslashes($row['texte']);
        		$temp['id']=$row['id'];
        		$temp['idInformation']=$row['idInformation'];
        		$temp['date']=$row['date'];
        		$returnValue[]=$temp;
        	}
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000095D end

        return (array) $returnValue;
    }

    /**
     * Check if an information is marked as starred
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @return boolean
     */
    public function isStarred($idInformation)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000095F begin
        $idInformation = (int) $idInformation;
        $user = (int)$this->core->getUserId();
        $star=$this->getIdTagName("@star");
        $query = "SELECT count(*) "
    			."FROM `kernel_tag_tag` "
    			."WHERE idInformation=$idInformation and author=$user and idTagName=$star";
        if ($result=$this->db->query($query)) {
        	if ($this->db->result($result,0)==1) {
        		$returnValue=true;
        	}
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000095F end

        return (bool) $returnValue;
    }

    /**
     * Check if an information is shared or private
     *
     * @access public
     * @author Sylvain Ramboz
     * @param int
     * @return boolean
     */
    public function isShared($idInformation)
    {
        $returnValue = (bool) false;

        $idInformation = (int) $idInformation;
        $user = (int)$this->core->getUserId();
        $shared=$this->getIdTagName("@shared");
        $query = "SELECT count(*) "
    			."FROM `kernel_tag_tag` "
    			."WHERE idInformation=$idInformation and author=$user and idTagName=$shared";
        if ($result=$this->db->query($query)) {
        	if ($this->db->result($result,0)==1) {
        		$returnValue=true;
        	}
        }

        return (bool) $returnValue;
    }

    /**
     * Get the cloud tag corresponding to an information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @return array
     */
    public function getCloudTag($idInformation)
    {
        $returnValue = array();

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000961 begin
        $idInformation = (int) $idInformation;
        $query = "SELECT "
        			."kernel_tag_liste_tag2.id as idTagName, "
        			."kernel_tag_liste_tag2.name as tagName, "
        			."count(idTagName) as tagCount "
        		."FROM `kernel_tag_tag`,`kernel_tag_liste_tag` kernel_tag_liste_tag,`kernel_tag_liste_tag` kernel_tag_liste_tag2 "
        		."WHERE kernel_tag_tag.idInformation=$idInformation "
        			."AND kernel_tag_tag.perso='false' "
        			."AND kernel_tag_tag.idTagName=kernel_tag_liste_tag.id "
        			."AND kernel_tag_liste_tag.name NOT LIKE '@%' "
        			."AND IF(kernel_tag_liste_tag.alias IS NULL,kernel_tag_liste_tag.id,kernel_tag_liste_tag.alias) = kernel_tag_liste_tag2.id "
        		."GROUP BY kernel_tag_liste_tag2.id "
        		."LIMIT 0, 10";
        if ($result=$this->db->query($query)) {
        	while ($row=$this->db->fetchArray($result)) {
        		$temp["idTagName"]=$row['idTagName'];
        		$temp["tagName"]=stripslashes($row['tagName']);
        		$temp["tagCount"]=$row['tagCount'];
        		$returnValue[]=$temp;
        	}	
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000961 end

        return (array) $returnValue;
    }

    /**
     * Get the list of available tagNames
     *
     * @access public
     * @author Vincent Keravec
     * @return array
     */
    public function getTagNames() {
        $returnValue = array();

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000963 begin
        //$start=(int) $start;
        //$limit=(int) $limit;
        
        $query = "SELECT id,name "
        		."FROM `kernel_tag_liste_tag` "
        		."WHERE kernel_tag_liste_tag.name NOT LIKE '@%'";
        		
        if ($result=$this->db->query($query)) {
        	while ($row=$this->db->fetchArray($result)) {
        		$temp['id']=$row['id'];
        		$temp['name']=stripslashes($row['name']);
        		$returnValue[]=$temp;
        	}
        }
        
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000963 end

        return (array) $returnValue;
    }

    /**
     * Get the id of an information or create it if it doesn't exist
     *
     * @access public
     * @author Vincent Keravec
     * @param string
     * @param string
     * @return int
     */
    public function getIdInformation($table, $idTableValue)
    {
        $returnValue = (int) 0;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000965 begin
        $table=addslashes($table);
      	$idTableValue=addslashes($idTableValue);
      	
        if ($this->cacheLastInformation!=null 
        		&& isset($this->cacheLastInformation['tableOrig'])
        		&& $this->cacheLastInformation['tableOrig']==$table
        		&& $this->cacheLastInformation['idOrig']==$idTableValue){
        	$returnValue=$this->cacheLastInformation['id'];
        } else {
        	$query="SELECT count(`id`) "
        			."FROM `kernel_tag_information` "
        			."WHERE tableOrig='$table' AND idOrig='$idTableValue'";
        	if ($result=$this->db->query($query)) {
        		if ($this->db->result($result,0)==1) { 	
	    			$query="SELECT `id`,`tableOrig`,`idOrig`,`cacheNbDigg`,IF(`link` IS NULL,'',`link`) "
			    			."FROM `kernel_tag_information` "
			    			."WHERE tableOrig='$table' AND idOrig='$idTableValue'";
			    	if ($result=$this->db->query($query)) {
			    		if ($row=$this->db->fetchArray($result)) {
			    			$this->cacheLastInformation=$row;
			    			$returnValue=$row['id'];
			    		}
			    	}
        		} else {
        			$query="INSERT INTO `kernel_tag_information` "
        				."(`tableOrig`,`idOrig`,`cacheNbDigg`,`link`) "
        				."VALUES "
        				."('$table','$idTableValue','0',NULL)";
        			$this->db->query($query);
        			
        			$this->cacheLastInformation['id']=$this->db->insertId();
        			$this->cacheLastInformation['tableOrig']=$table;
        			$this->cacheLastInformation['idOrig']=$idTableValue;
        			$this->cacheLastInformation['cacheNbDigg']=0;
        			$this->cacheLastInformation['link']='';
        			$returnValue=$this->cacheLastInformation['id'];
        		}
        	}
        }
        
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000965 end

        return (int) $returnValue;
    }

    /**
     * Link to information together
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param int
     * @return boolean
     */
    public function linkInformation($idInformation1, $idInformation2)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000967 begin
        if ($this->checkModerateurUser()) {
	        $idInformation1= (int)$idInformation1;
	        $idInformation2= (int)$idInformation2;
	        $query = "SELECT IFNULL(link, id) FROM `kernel_tag_information` WHERE id=$idInformation1";
	        $result = $this->db->query($query);
	        if ($result) {
	        	$tagNameParent=$this->db->result($result,0);
	        	$query = "UPDATE `kernel_tag_information` SET link=$tagNameParent WHERE id=$idInformation2";
	        	if ($this->db->query($query)) {
	        		$returnValue=true;
	        	}
	        }
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000967 end

        return (bool) $returnValue;
    }

    /**
     * Check if a tagName exist in the database
     *
     * @access public
     * @author Vincent Keravec
     * @param string
     * @return boolean
     */
    public function tagNameExist($tagName)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000096C begin
        $tagName=addslashes($tagName);
        $query = "SELECT count(name) "
        		."FROM `kernel_tag_liste_tag` "
        		."WHERE name='$tagName'";
        if ($result=$this->db->query($query)) {
        	if ($this->db->result($result,0)==1) {
        		$returnValue=true;
        	}
        }
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000096C end

        return (bool) $returnValue;
    }

    /**
     * Add a tagName to the list of tagNames
     *
     * @access public
     * @author Vincent Keravec
     * @param string
     * @return int
     */
    public function addTagName($tagName)
    {
        $returnValue = (int) 0;

        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000096E begin
        $tagName=addslashes($tagName);
        $query = "INSERT INTO `kernel_tag_liste_tag` (`name`,`alias`) VALUES ( '$tagName',NULL)";
        if ($result=$this->db->query($query)) {
        	$returnValue=$this->db->insertId();
        }
        
        // section -64--88-0-76--657101a2:10e36934f38:-8000:000000000000096E end

        return (int) $returnValue;
    }

    /**
     * Get the list of unvalidated alias
     *
     * @access public
     * @author Vincent Keravec
     * @return array
     */
    public function getPendingAlias($start,$limit)
    {
        $returnValue = array();

        // section -64--88-0-76--3d256e4f:10e3bbc2a13:-8000:0000000000000B5F begin
        $start=(int)$start;
        $limit=(int) $limit;
        $check =array();
        
        $query = "SELECT idTag1, t1.name as tagName1, idTag2, t2.name as tagName2 "
        		."FROM `kernel_tag_alias_notify`,`kernel_tag_liste_tag` t1, `kernel_tag_liste_tag` t2 "
        		."WHERE t1.id=idTag1 AND t2.id=idTag2 "
        		."LIMIT $start,$limit";
        if ($result=$this->db->query($query)) {
        	while ($row=$this->db->fetchArray($result)) {
        		if (!in_array($row['idTag1']."-".$row['idTag2'],$check)) {
		    		$temp['tagName1']=stripslashes($row['tagName1']);
		    		$temp['tagName2']=stripslashes($row['tagName2']);
		    		$temp['idTag1']=$row['idTag1'];
		    		$temp['idTag2']=$row['idTag2'];
		    		
		    		$check[]=$row['idTag1']."-".$row['idTag2'];
		    		$check[]=$row['idTag2']."-".$row['idTag1'];
		    		
		    		$returnValue[]=$temp;
        		}
        	}
        }
        // section -64--88-0-76--3d256e4f:10e3bbc2a13:-8000:0000000000000B5F end

        return (array) $returnValue;
    }

    /**
     * Get the cloud tag for the whole site
     *
     * @access public
     * @author Vincent Keravec
     * @return string
     */
    public function getCloudTagSite()
    {
        $returnValue = array();

        // section -64--88-0-76--3d256e4f:10e3bbc2a13:-8000:0000000000000B63 begin
        $query = "SELECT "
        			."kernel_tag_liste_tag2.id as idTagName, "
        			."kernel_tag_liste_tag2.name as tagName, "
        			."count(idTagName) as tagCount "
        		."FROM `kernel_tag_tag`,`kernel_tag_liste_tag` kernel_tag_liste_tag,`kernel_tag_liste_tag` kernel_tag_liste_tag2 "
        		."WHERE kernel_tag_tag.perso='false' "
        			."AND kernel_tag_tag.idTagName=kernel_tag_liste_tag.id "
        			."AND kernel_tag_liste_tag.name NOT LIKE '@%' "
        			."AND IF(kernel_tag_liste_tag.alias IS NULL,kernel_tag_liste_tag.id,kernel_tag_liste_tag.alias) = kernel_tag_liste_tag2.id "
        		."GROUP BY kernel_tag_liste_tag2.id "
        		."LIMIT 0,10";
        if ($result=$this->db->query($query)) {
        	while ($row=$this->db->fetchArray($result)) {
        		$temp["tagName"]=stripslashes($row['tagName']);
        		$temp["idTagName"]=$row['idTagName'];
        		$temp["tagCount"]=$row['tagCount'];
        		$returnValue[]=$temp;
        	}	
        }
        // section -64--88-0-76--3d256e4f:10e3bbc2a13:-8000:0000000000000B63 end

        return (array) $returnValue;
    }

    /**
     * Get the cloud tag of a module
     *
     * @access public
     * @author Vincent Keravec
     * @return string
     */
    public function getCloudTagModule()
    {
        $returnValue = array();

        // section -64--88-0-76--3d256e4f:10e3bbc2a13:-8000:0000000000000B65 begin
        $tableListQuery="";
       	foreach (array_keys($this->tableNames) as $tableName) {
       		if ($tableListQuery=="") {
       			$tableListQuery="kernel_tag_information.tableOrig='$tableName'";
       		} else {
       			$tableListQuery.=" OR kernel_tag_information.tableOrig='$tableName'";
       		}
       	}
        
        $query = "SELECT "
        			."kernel_tag_liste_tag2.id as idTagName, "
        			."kernel_tag_liste_tag2.name as tagName, "
        			."count(idTagName) as tagCount "
        		."FROM `kernel_tag_tag`,`kernel_tag_liste_tag` kernel_tag_liste_tag,`kernel_tag_liste_tag` kernel_tag_liste_tag2, kernel_tag_information "
        		."WHERE kernel_tag_tag.perso='false' "
        			."AND kernel_tag_tag.idTagName=kernel_tag_liste_tag.id "
        			."AND kernel_tag_liste_tag.name NOT LIKE '@%' "
        			."AND kernel_tag_tag.idInformation=kernel_tag_information.id "
        			."AND IF(kernel_tag_liste_tag.alias IS NULL,kernel_tag_liste_tag.id,kernel_tag_liste_tag.alias) = kernel_tag_liste_tag2.id "
        			."AND ($tableListQuery) "
        		."GROUP BY kernel_tag_liste_tag2.id "
        		."LIMIT 0,10";
        if ($result=$this->db->query($query)) {
        	while ($row=$this->db->fetchArray($result)) {
        		$temp["tagName"]=stripslashes($row['tagName']);
        		$temp["tagCount"]=$row['tagCount'];
        		$temp["idTagName"]=$row['idTagName'];
        		$returnValue[]=$temp;
        	}	
        }
        // section -64--88-0-76--3d256e4f:10e3bbc2a13:-8000:0000000000000B65 end

        return (array) $returnValue;
    }

    /**
     * Verify that the user can define this category
     *
     * @access private
     * @author Vincent Keravec
     * @param string
     * @return boolean
     */
    private function checkSetCategory($idCategoryName)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:0000000000000999 begin
        $idCategoryName=(int)$idCategoryName;
        $query="SELECT Droit.name FROM kernel_tag_category, Droit "
        	."WHERE id=$idCategoryName AND Droit.id=kernel_tag_category.rightSet";
        if ($result=$this->db->query($query)) {
        	if ($droit=$this->db->result($result,0)) {
        		$returnValue=$this->core->verifDroit($droit);	
        	}	
        }
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:0000000000000999 end

        return (bool) $returnValue;
    }

    /**
     * Verify if a user is an admin or a moderateur
     *
     * @access private
     * @author Vincent Keravec
     * @return boolean
     */
    private function checkModerateurUser()
    {
        $returnValue = (bool) false;

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:000000000000099B begin
        if ($this->core->verifDroits("moderateur") || $this->core->verifDroits("admin")) {
    		// The user must be admin or moderateur to set a user different than him
    		$returnValue=true;	
    	}
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:000000000000099B end

        return (bool) $returnValue;
    }
    
    /**
     * Check if a user is an admin
     *
     * @access private
     * @author Vincent Keravec
     * @return boolean
     */
    private function checkAdminUser()
    {
        $returnValue = (bool) false;

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009A3 begin
       if ($this->core->verifDroits("admin")) {
    		// The user must be admin
    		$returnValue=true;	
    	}
        
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009A3 end

        return (bool) $returnValue;
    }

    /**
     * Remove all Tags with the specified tagName from this information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param int
     * @return boolean
     */
    public function removeTagsInformation($idInformation, $idTagName)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009AF begin
        $idInformation=(int)$idInformation;
        $idTagName=(int)$idTagName;
        if ($this->checkModerateurUser()) {
       		$query="DELETE FROM kernel_tag_tag WHERE idInformation=$idInformation AND idTagName=$idTagName";
			if ($this->db->query($query)) {
				$returnValue=true;
			}
       	}
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009AF end

        return (bool) $returnValue;
    }

    /**
     * Remove the link between two informations
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param int
     * @return boolean
     */
    public function unlinkInformation($idInformation1, $idInformation2)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009B8 begin
        if ($this->checkModerateurUser()) {
	        $idInformation1= (int)$idInformation1;
	        $idInformation2= (int)$idInformation2;
	        $query = "SELECT IFNULL(link, '') FROM `kernel_tag_information` WHERE id=$idInformation2";
	        $result = $this->db->query($query);
	        if ($result) {
	        	$link=$this->db->result($result,0);
	        	if ($link==$idInformation1) {
			    	$query = "UPDATE `kernel_tag_information` SET link=NULL WHERE id=$idInformation2";
			    	if ($this->db->query($query)) {
			    		$returnValue=true;
			    	}
	        	}
	        }
	        if (!$returnValue) {
	        	$query = "SELECT IFNULL(link, '') FROM `kernel_tag_information` WHERE id=$idInformation1";
		        $result = $this->db->query($query);
		        if ($result) {
		        	$link=$this->db->result($result,0);
		        	if ($link==$idInformation2) {
				    	$query = "UPDATE `kernel_tag_information` SET link=NULL WHERE id=$idInformation1";
				    	if ($this->db->query($query)) {
				    		$returnValue=true;
				    	}
		        	}
		        }	
	        }
        }
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009B8 end

        return (bool) $returnValue;
    }

    /**
     * Show a category
     *
     * @access public
     * @author Vincent Keravec
     * @param string
     * @return boolean
     */
    public function showCategory($idCategory)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009D2 begin
        $user = (int)$this->core->getUserId();
        $idCategory = (int)$idCategory;
        
        $query = "INSERT INTO `kernel_tag_user_pref` (`login`,`idCategory`,`isShow`) "
        		."VALUES ($user,$idCategory,'true') "
        		."ON DUPLICATE KEY UPDATE `isShow`='true'";
        if ($this->db->query($query)) {
        	$returnValue=true;
        }
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009D2 end

        return (bool) $returnValue;
    }

    /**
     * Hide a category
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @return boolean
     */
    public function hideCategory($idCategory)
    {
        $returnValue = (bool) false;

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009D5 begin
        $user = (int)$this->core->getUserId();
        $idCategory = (int)$idCategory;
        
        $query = "INSERT INTO `kernel_tag_user_pref` (`login`,`idCategory`,`isShow`) "
        		."VALUES ($user,$idCategory,'false') "
        		."ON DUPLICATE KEY UPDATE `isShow`='false'";
        if ($this->db->query($query)) {
        	$returnValue=true;
        }
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009D5 end

        return (bool) $returnValue;
    }

    /**
     * Short description of method getIdTagName
     *
     * @access public
     * @author Vincent Keravec
     * @param string
     * @return int
     */
    public function getIdTagName($tagName)
    {
        $returnValue = (int) 0;

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009EF begin
        $tagName=addslashes($tagName);
        $query = "SELECT id "
        		."FROM `kernel_tag_liste_tag` "
        		."WHERE name='$tagName'";
        if ($result=$this->db->query($query)) {
        	$returnValue=$this->db->result($result,0);
        }
        
        if ($returnValue==0) {
        	$returnValue=$this->addTagName($tagName);
        }
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009EF end

        return (int) $returnValue;
    }
/**
     * Short description of method getTagName
     *
     * @access public
     * @author Vincent Keravec
     * @param string
     * @return int
     */
    public function getTagName($idTagName)
    {
        $returnValue = "";

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009EF begin
        $idTagName=(int)$idTagName;
        
        $query = "SELECT name "
        		."FROM `kernel_tag_liste_tag` "
        		."WHERE id=$idTagName";
        if ($result=$this->db->query($query)) {
        	$returnValue=$this->db->result($result,0);
        }
              
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009EF end

        return $returnValue;
    }
    /**
     * Get the list of the different available category for this user and their
     *
     * @access public
     * @author Vincent Keravec
     * @return array
     */
    public function getListCategory()
    {
        $returnArray = array();

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009F4 begin
        $user = (int)$this->core->getUserId();
        //$start=(int)$start;
        //$limit=(int)$limit;
        
        $query = "SELECT idCategory as id, name, image, isShow, description "
        		."FROM `kernel_tag_category_user` "
        		."WHERE `kernel_tag_category_user`.login=$user ";
        		//."LIMIT $start, $limit";
        if ($result=$this->db->query($query)) {
        	while($row=$this->db->fetchArray($result)){
        		$temp['id']=$row['id'];
        		$temp['name']=stripslashes($row['name']);
        		$temp['image']=stripslashes($row['image']);
        		$temp['description']=stripslashes($row['description']);
        		$temp['isShow']=$row['isShow'];
        		$returnArray[]=$temp;
        	}
        }
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009F4 end

        return (array) $returnArray;
    }
    
	/**
     * Get the set different category for an information
     *
     * @access public
     * @author Vincent Keravec
     * @param int
     * @param array
     * @return boolean
     */
    public function setInformationCategory($idInformation, $idCategories) {
        $returnValue = (bool) false;

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009F4 begin
        
        $user = (int)$this->core->getUserId();
        $idInformation=(int)$idInformation;
        $listCategories="";
        for ($i = 0; $i < sizeof($idCategories); $i++) {
			$idCategories[$i]=(int)$idCategories[$i];
			if ($listCategories=="") {
        		$listCategories.="".$idCategories[$i]."";	
        	} else {
        		$listCategories.=",".$idCategories[$i];	
        	}
		}
        
        $query = "DELETE FROM kernel_tag_information_category WHERE idCategory IN "
        		."(SELECT idCategory FROM kernel_tag_category_user_set "
        			."WHERE login=".$user." "
        				."AND idInformation='".$idInformation."')";
        if ($this->db->query($query)) {
        	// check if we have a category to set
        	if (sizeof($idCategories)>0) {
	        	$query="INSERT INTO kernel_tag_information_category (idInformation,idCategory)"
	        			." SELECT $idInformation, idCategory "
	        			." FROM kernel_tag_category_user_set "
	        			." WHERE idCategory IN ($listCategories)"
	        			."		AND login=$user";
	        	if ($this->db->query($query)) {
	        		$returnValue=true;
	        	}
        	} else {
        		//echo "\$idCategories vide !";
        		$returnValue=true;
        	}
        }
        
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009F4 end

        return (bool) $returnValue;
    }
    
    /**
     * Get the list of the different available category for this user and their
     *
     * @access public
     * @author Vincent Keravec
     * @return array
     */
    public function getListCategoryAdmin()
    {
        $returnArray = array();

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009F4 begin
        
        if ($this->checkAdminUser()) {
	        $user = (int)$this->core->getUserId();
	        //$start=(int)$start;
	        //$limit=(int)$limit;
	        
	        $query = "SELECT categ.id, categ.name, categ.image, categ.defaultShow, categ.description, d1.name as rightGet, d2.name as rightSet "
	        		."FROM `kernel_tag_category` categ, "
	        		."		kernel_right d1, kernel_right d2 "
	        		."WHERE categ.rightGet = d1.id AND categ.rightSet=d2.id";
	        		
	        if ($result=$this->db->query($query)) {
	        	while($row=$this->db->fetchArray($result)){
	        		$temp['id']=$row['id'];
	        		$temp['name']=stripslashes($row['name']);
	        		$temp['image']=stripslashes($row['image']);
	        		$temp['rightSet']=stripslashes($row['rightSet']);
	        		$temp['rightGet']=stripslashes($row['rightGet']);
	        		$temp['defaultShow']=$row['defaultShow'];
	        		$temp['description']=stripslashes($row['description']);
	        		$returnArray[]=$temp;
	        	}
	        }
        }
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009F4 end

        return (array) $returnArray;
    }
    
    /**
     * Get the list of the different available category for this user and their
     *
     * @access public
     * @author Vincent Keravec
     * @param int $id
     * @return array
     */
    public function getInfoCategoryAdmin($id)
    {
        $returnArray = array();

        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009F4 begin
        
        if ($this->checkAdminUser()) {
	        $user = (int)$this->core->getUserId();
	        $id=(int)$id;
	        //$start=(int)$start;
	        //$limit=(int)$limit;
	        
	        $query = "SELECT id, name, rightGet, rightSet, image, defaultShow, description "
	        		."FROM `kernel_tag_category` "
	        		."WHERE id=$id";
	        		
	        if ($result=$this->db->query($query)) {
	        	while($row=$this->db->fetchArray($result)){
	        		$temp['id']=$row['id'];
	        		$temp['name']=stripslashes($row['name']);
	        		$temp['image']=stripslashes($row['image']);
	        		$temp['description']=stripslashes($row['description']);
	        		$temp['rightSet']=($row['rightSet']);
	        		$temp['rightGet']=($row['rightGet']);
	        		$temp['defaultShow']=$row['defaultShow'];
	        		$returnArray=$temp;
	        	}
	        }
        }
        // section -64--88-0-76-741d77e0:10e40fe6e2d:-8000:00000000000009F4 end

        return (array) $returnArray;
    }
	
	public function getTagNamesUser($idInformation) {
        $returnValue = array();

        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000963 begin
        //$start=(int) $start;
        //$limit=(int) $limit;
        $login=(int)$this->core->getUserId();
        $query = "SELECT list_tag.id,list_tag.name "
        		."FROM `kernel_tag_liste_tag` as list_tag, kernel_tag_tag as tag "
        		."WHERE list_tag.name NOT LIKE '@%' "
        		."		AND tag.idTagName=list_tag.id "
        		."		AND tag.author=".$login
        		."		AND tag.idInformation=".$idInformation;
        		
        		
        if ($result=$this->db->query($query)) {
        	while ($row=$this->db->fetchArray($result)) {
        		$temp['id']=$row['id'];
        		$temp['name']=stripslashes($row['name']);
        		$returnValue[]=$temp;
        	}
        }
        
        // section -64--88-0-76--657101a2:10e36934f38:-8000:0000000000000963 end

        return (array) $returnValue;
    }

} /* end of class Tag */



?>
