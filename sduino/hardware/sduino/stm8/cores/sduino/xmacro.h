#ifndef _XMACRO_H_
#define _XMACRO_H_

//////// A macro to count the number of macro argements
// found here:
// http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5?pli=1

#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0



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



// generic X-Macros for use with preinstantiated "classes"
//
// preinstantiated "classes" don't need a instance identifier as first
// parameter.

// simple method without arguments
#define XPreMethod0(class,instance,name) inline \
	void instance##_##name()\
	{class##_##name();}
#define XPreMethod0return(class,instance,type,name) inline \
	type instance##_##name()\
	{return class##_##name();}
// method with one argument
#define XPreMethod1(class,instance,name,atype1) inline \
	void instance##_##name(atype1 arg1)\
	{class##_##name(arg1);}
#define XPreMethod1return(class,instance,type,name,atype1) inline \
	type instance##_##name(atype1 arg1)\
	{return class##_##name(arg1);}
// method with two arguments
#define XPreMethod2(class,instance,name,atype1,atype2) inline \
	void instance##_##name(atype1 arg1, atype2 arg2)\
	{class##_##name(arg1,arg2);}
#define XPreMethod2return(class,instance,type,name,atype1,atype2) inline \
	type instance##_##name(atype1 arg1, atype2 arg2)\
	{return class##_##name(arg1,arg2);}
// method with three arguments
#define XPreMethod3(class,instance,name,atype1,atype2,atype3) inline \
	void instance##_##name(atype1 arg1, atype2 arg2, atype3 arg3)\
	{class##_##name(arg1,arg2,arg3);}
#define XPreMethod3return(class,instance,type,name,atype1,atype2,atype3) inline \
	type instance##_##name(atype1 arg1, atype2 arg2, atype3 arg3)\
	{return class##_##name(arg1,arg2,arg3);}
// method with four arguments
#define XPreMethod4(class,instance,name,atype1,atype2,atype3,atype4) inline \
	void instance##_##name(atype1 arg1, atype2 arg2, atype3 arg3, atype4 arg4)\
	{class##_##name(arg1,arg2,arg3,arg4);}
#define XPreMethod4return(class,instance,type,name,atype1,atype2,atype3,atype4) inline \
	type instance##_##name(atype1 arg1, atype2 arg2, atype3 arg3, atype4 arg4)\
	{return class##_##name(arg1,arg2,arg3,arg4);}



// specialized set of X-Macros to add the Print methods to an output "class".
//
// Used to mimic the inhertance from the Print class. Calling the single
// macro 'XPrintMethods' brings in all the Print-Methods listed here.
//
// name scheme: INSTANCE_NAME(...) -> Print_NAME(CLASS_write,...)
// example:     lcd_print_s(str)   -> Print_print_s(LiquidCristal_write, str)

// just an alias for the class_write() function
#define Xprintchar(class,instance) inline \
        size_t instance##_print_c(uint8_t arg1)\
        {return class##_write(arg1);}

#define Xprinthelper0(class,instance,name) inline \
        size_t instance##_##name(void)\
        {return Print_##name(class##_write);}
#define Xprinthelper1(class,instance,name,atype1) inline \
        size_t instance##_##name(atype1 arg1)\
        {return Print_##name(class##_write,arg1);}
#define Xprinthelper2(class,instance,name,atype1,atype2) inline \
        size_t instance##_##name(atype1 arg1,atype2 arg2)\
        {return Print_##name(class##_write,arg1,arg2);}

#define XPrintMethods(class,instance) \
        Xprintchar	(class,instance) \
        Xprinthelper1	(class,instance,print_s,const char*) \
        Xprinthelper2	(class,instance,print_sn,const uint8_t*,size_t) \
        Xprinthelper1	(class,instance,print_i,long) \
        Xprinthelper1	(class,instance,print_u,unsigned long) \
        Xprinthelper2	(class,instance,print_ib,long,uint8_t) \
        Xprinthelper2	(class,instance,print_ub,unsigned long,uint8_t) \
        Xprinthelper0	(class,instance,println) \
        Xprinthelper1	(class,instance,println_s,const char*) \
        Xprinthelper1	(class,instance,println_i,long) \
        Xprinthelper1	(class,instance,println_u,unsigned long)



// generic X-Macros for use with multiple instance "classes"
//
// Multiple instance "classes" need a instance identifier as first parameter.
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
