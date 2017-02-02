#ifndef _XMACRO_H_
#define _XMACRO_H_

/////// X-Macro like magic
/*
  A general introduction into the idea behind X-Macros:
  https://en.wikipedia.org/wiki/X_Macro

// The instantiation macro *must* be the last in the list as it is the only
// macro that does not result in a syntax error with the following ';'
#define CLASSNAME(instance) \
	XConstructor1	(CLASSNAME,instance,attach,byte,pin) \
	XConstructor3	(CLASSNAME,instance,attach_minmax,byte,pin,int,min,int,max) \
	XMethod0	(CLASSNAME,instance,detach) \
	XMethod1	(CLASSNAME,instance,write,unsigned int, val) \
	XMethod0return	(CLASSNAME,instance,int,read) \
	XInstanciation	(CLASSNAME,instance)

*/

/// generic X-Macros
#define XInstanciation(type,instance) \
	type instance
// constructor with one argument
#define XConstructor1(class,instance,name,atype,arg) inline \
	void instance##_##name(atype arg)\
	{instance=class##_##name(arg);}
#define XConstructor2(class,instance,name,atype1,arg1,atype2,arg2) inline \
	void instance##_##name(atype1 arg1,atype2 arg2)\
	{instance=class##_##name(arg1,arg2);}
#define XConstructor3(class,instance,name,atype1,arg1,atype2,arg2,atype3,arg3) inline \
	void instance##_##name(atype1 arg1,atype2 arg2,atype3 arg3)\
	{instance=class##_##name(arg1,arg2,arg3);}
// simple method without arguments
#define XMethod0(class,instance,name) inline \
	void instance##_##name()\
	{class##_##name(instance);}
#define XMethod0return(class,instance,type,name) inline \
	type instance##_##name()\
	{return class##_##name(instance);}
// method with one argument
#define XMethod1(class,instance,name,atype1,arg1) inline \
	void instance##_##name(atype1 arg1)\
	{class##_##name(instance,arg1);}
#define XMethod1return(class,instance,type,name,atype1,arg1) inline \
	type instance##_##name(atype1 arg1)\
	{return class##_##name(instance,arg1);}


#endif
