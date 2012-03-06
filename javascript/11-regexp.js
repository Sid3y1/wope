/**
 * Regular expression of email
 * not case sensitive
 * Examples : 
 * mail@domain.ext
 * e-mail@subdomain.domain.net
 * e.mail@subdomain.subdomain.name
 * support but is not valid :
 * .....@test._...ffff
 */
RegExp.EMAIL = /^[a-z0-9._-]+@[a-z0-9._-]{2,}\.[a-z]{2,4}$/i;
/**
 * Gets the regular expression for valid email addresses as per RFC-822 except the part about "including linear-white-space" :
 * EMAIL /^([^\[\]()<>@,;:\\".]+|\"([^"\\\r]|\\.)*\")(\.([^\[\]()<>@,;:\\".]+|\"([^"\\\r]|\\.)*\"))*@(\[([^\\\[\]\r]|\\.)*\]|[^\[\]()<>@,;:\\".]+)(\.(\[([^\\\[\]\r]|\\.)*\]|[^\[\]()<>@,;:\\".]+))*$/;
 */

/**
 * Regular expression of integer
 * Not limited in length
 * Examples :
 * -2
 * + 0005
 * 900
 */
RegExp.INT = /^(?:[-+]\s?)?[0-9]+$/;

/**
 * Regular expression of float
 * Not limited in length
 * Examples :
 * - 2,4
 * +2
 * 589.54
 * .54
 */
//RegExp.FLOAT = /^(?:[-+]\s?)?[0-9]*[.,]?[0-9]*$/;
RegExp.FLOAT = /^(?:[-+]\s?)?(?:(?:[0-9]+)|(?:[.,][0-9]+)|(?:[0-9]+[.,][0-9]+))$/;

/**
 * Regular expression of (French) phone number
 * International number not supported
 * @see <a href="http://en.wikipedia.org/wiki/Country_calling_codes">Country calling codes</a>
 * Examples :
 * - 2,4
 * +2
 * 589.54
 * .54
 */
RegExp.PHONE_FR = /^0[1-9](?:[-. ]?[0-9]{2}){4}$/; // 06 36 67 49 53 | 0892 13 51 61

/**
 * Regular expression of (French) date
 * Format : DD MM YYYY (support 2 number years)
 * Examples :
 * 12 07 1990
 * 25-12-2000
 * 11/09/01
 */
RegExp.DATE_FR = /^((?:0[1-9])|(?:[1-2][0-9])|(?:3?[01]))[/ -]((?:0[1-9])|(?:1[0-2]))[/ -]([0-9]{2}(?:[0-9]{2})?)$/;
/**
 * Regular expression of (international) date
 * Format : YYYY MM DD (support 2 number years)
 * Examples :
 * 1990 07 12
 * 2000-12-25
 * 01/09/11
 */
RegExp.DATE=/^([0-9]{2}(?:[0-9]{2})?)[/ -]((?:0[1-9])|(?:1[0-2]))[/ -]((?:0[1-9])|(?:[1-2][0-9])|(?:3[01]))$/;

/**
 * Regular expression of (French) Zip Code
 * @see <a href="http://en.wikipedia.org/wiki/List_of_French_postal_codes>French postal codes</a>
 * Examples :
 * 10 000
 * 75000
 */
RegExp.ZIP_CODE_FR = /^[0-9]{2}\s?[0-9]{3}$/;

/**
 * Regular expression of something (printed characters)
 * Examples :
 * hello world !
 */
RegExp.SOMETHING = /[^ \t\n\r\f\v]+/;
 
/**
 * Regular expression of login
 */
RegExp.LOGIN = /^[a-z0-9\._]{4,50}$/i;

/**
 * Regular expression of price
 * Examples :
 * 678,31
 * 2,4
 * 2
 * 589.54
 * 1.54
 */
RegExp.MONEY = /^[0-9]+(?:[,\.][0-9]{1,2})$/;

/**
  * Regular expression of the passwords
	*	more than 6 chars
	*/

RegExp.PASSWORD = /^[a-zA-Z0-9_\*\$@&%\.,;:+!?=#-]{6}[a-zA-Z0-9_\*\$@&%\.,;:+!?=#-]*$/;

