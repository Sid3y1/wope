/**
 * FuncLink is a Class to return Function for dont loose referencial of Object|Class's method.
 * Work too for anonymous and named Function.
 *
 * @param method  a (anonymous|named)Function or a String, a function or name of function or method
 * @param obj     a Object, if method is a method (Optional)
 * @return        the Function;
 */
var FuncLink = function(method, obj)
{
  obj = obj || null;
  if(typeof obj === 'object')//if using "obj.method"
  {
    //Use closure for using indirect call of obj's method,
    //to keep the referencial 'this'
    return function()
    {
      for(var i = 0, l = arguments.length, args = new Array(); i < l; i++)
      {
        args.push('arguments[' + i + ']');
      }
      eval('obj[method](' + args.join(',') + ')');
    };
  }
  else if(typeof method === 'function')//if is only a function (not a method)
  {
    return method;
  }
  else if(typeof method === 'string' && obj === null)//if the function is passed in String
  {
    return eval(method);
  }
  return function(){};
};