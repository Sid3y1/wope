/**
 * forEach is a method of Array  to execute for each element of an Array
 *
 * @param func       a Function, accepte one argument, the array element
 * @return           Void;
 */
Array.prototype.forEach = function(func)
{
  for(var i = 0, l = this.length; i < l; i ++)
  {
    func(this[i]);
  }
};

/**
 * indexOf is a method of Array  to get the index of first element in the Array
 *
 * @param value       a Object (String, Number, ...), element to search index
 * @param begin       a Number, index where begin of search
 * @param strict      a Boolean, if search is strict (Optional)
 * @return            a Number, index of the first element that matches value if exist else -1
 */
Array.prototype.indexOf = function(value, begin, strict)
{
  for(var i = +begin || 0; i < this.length; i++)
  {
    if(this[i] === value || strict && this[i] == value)
    {
      return i;
    }
  }
  return -1;
};

/**
 * insert is a method of Array to insert value at index, without overwriting existing keys
 *
 * @param index       a Number, index where insert element
 * @param value       a Object (String, Number, ...), element to insert
 * @return            a Array, the array with the element inserted;
 */
Array.prototype.insert = function(index, value)
{
  if(index >= 0)
  {
    var a = this.slice(), b = a.splice(index);
    a[index] = value;
    return a.concat(b);
  }
};

/**
 * insert is a method of Array to get index of the last element that matches value
 *
 * @param value       a Object (String, Number, ...), element to search index
 * @param begin       a Number, index where begin of search
 * @param strict      a Boolean, if search is strict (Optional)
 * @return            a Array, the array with the element inserted;
 */
Array.prototype.lastIndexOf = function(value, begin, strict)
{
  begin = +begin || 0;
  var i = this.length;
  while(i-- > begin)
  {
    if(this[i] === value || strict && this[i] == value )
    {
      return i;
    }
  }
  return -1;
};

/**
 * remove is a method of Array to remove each elements index
 *
 * @param index     a Number, index of element to remove
 * @return          Void
 */
//TODO: support a Array argument
Array.prototype.remove = function()
{
  for(var i = 0; i < arguments.length; i++)
    this.splice(arguments[i] ,1);
};

/**
 * clear is a method of Array to remove all element of the Array
 *
 * @return   Void
 */
Array.prototype.clear = function()
{
  this.splice(0, this.length);
};
