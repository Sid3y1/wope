/**
 * Fonction permettant de connaitre si une variable est nulle
 *
 * @param mixed variable variable à tester
 * @return boulean
 */
  
/**
 * Function isNull to verify if the value is null or is a empty String
 * @params value    a Object (Number, String, ...), value to compare
 * @return          a Boolean, <code>true</code> if is null, <code>false</code>
 * @need            #getAndCall()
 */
function isNull(value)
{
  return value === null || value === '';
}

/**
 * Pointer to HTML DOMObject like document.body
 */
document.html = document.documentElement;
  