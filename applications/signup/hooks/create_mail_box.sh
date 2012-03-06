
#CONFIG
MYSQL_USER="postfix"
MYSQL_PASS="Ahv5pee4os"
MYSQL_DB="postfix"

#WITH a trailing slash
POSTFIX_BASE_DIR="/var/spool/postfix/virtual"


###### Arguments #######

#login
username=$1
# password
password=$2
# Nom
realname=$3
# domaine du compte
domain=$4

#Get domain GroupID
vgid=`ls -l /var/spool/postfix/virtual/ | grep $domain | awk '{ print $4 }'`

#Get a free UserID
vuid=`mysql -u$MYSQL_USER -p$MYSQL_PASS -D$MYSQL_DB -e"SELECT MAX(uid) FROM users" | tail -n1 | awk '{ print $1 }'`

if [ ! `echo "INSERT INTO users (id, address, crypt, clear, name, uid, gid, home, domain, maildir, imapok, bool1, bool2) VALUES ('$username', '$username@$domain', ENCRYPT('$password'), '', '$realname', '$vuid', '$vgid', '$POSTFIX_BASE_DIR', '$domain', '$POSTFIX_BASE_DIR/$domain/$username/Maildir/', '1', '1', '1');" | mysql -u$MYSQL_USER -p$MYSQL_PASS -D$MYSQL_DB` ]
then
        mkdir $POSTFIX_BASE_DIR/$domain/$username
				#       chown grumly -R $POSTFIX_BASE_DIR/$domain/$username
				        maildirmake $POSTFIX_BASE_DIR/$domain/$username/Maildir
								#       chown grumly -R $POSTFIX_BASE_DIR/$domain/$username

								        chown $vuid:$vgid -R $POSTFIX_BASE_DIR/$domain/$username
												        chmod -R 777 $POSTFIX_BASE_DIR/$domain
																fi

