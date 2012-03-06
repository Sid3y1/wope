/**
 * trim is a Method of String to remove all before and after whitespace
 * @return     a String, result of fitering before and after whitespace
 */
String.prototype.trim = function()
{
	return this.replace(/^\s*|\s*$/g, '');
};