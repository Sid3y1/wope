var URL = function(href)
{
  var url = href.split('?');
  this.href = url[0];
  this.datas = arguments[1] || url[1] || '';
};
URL.prototype.toString = function()
{
  var tmp = this.href;
  if(!isNull(this.datas))
    tmp += '?' + this.datas;
  return tmp;
};
URL.getHash = function()
{
  var hash = document.location.hash
  return hash.substring(1, hash.length);
};