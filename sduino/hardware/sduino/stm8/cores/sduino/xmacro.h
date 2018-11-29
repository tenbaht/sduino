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

// a macro to generate a function call depending on the number of arguments.
// Format of the function call: PREFIXn(...)
// PREFIX is the first parameter to VARFUNC()
// n is the number of arguments after PREFIX
// example: 'VARFUNC(init,a,b,c)' expands to 'init3(a,b,c)'
//
// used for polymorph instantiation calls.
#define VARFUNC(PREFIX,...) \
         PRE_NARG_(PREFIX,__VA_ARGS__,PP_RSEQ_N())(__VA_ARGS__)
#define PRE_NARG_(PREFIX,...) \
         PRE_ARG_N(PREFIX,__VA_ARGS__)
#define PRE_ARG_N(PREFIX, \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) PREFIX##N




/////// X-Macro like magic
/*
  A general introduction into the idea behind X-Macros:
  https://en.wikipedia.org/wiki/X_Macro

// The instantiation macro *must* be the last in the list as it is the only
// macro that does not result in a syntax error with the following ';'
#define CLASSNAME(instance) \
	XConstructor1	(CLASSNAME,instance,attach,byte) \
	XConstructor3	(CLASSNAME,instance,attach_minmax,byte,int,int) \
	XMethod0	(CLASSNAME,instance,detach) \
	XMethod1	(CLASSNAME,instance,write,unsigned int) \
	XMethod0return	(CLASSNAME,instance,int,read) \
	XInstanciation	(CLASSNAME,instance)

*/



// generic X-Macros for use with single instance "classes"
//
// case 2: single instance, using a constructor
//
// No instance identifier required, only function name alias are needed.
//
// name scheme: INSTANCE_NAME(...) -> CLASS_NAME(...)
// example:     Serial_begin(baud) -> HardwareSerial_begin(baud)

// simple method without arguments
#define X2Method0(class,instance,name) inline \
	void instance##_##name()\
	{class##_##name();}
#define X2Method0return(class,instance,type,name) inline \
	type instance##_##name()\
	{return class##_##name();}
// method with one argument
#define X2Method1(class,instance,name,atype1) inline \
	void instance##_##name(atype1 arg1)\
	{class##_##name(arg1);}
#define X2Method1return(class,instance,type,name,atype1) inline \
	type instance##_##name(atype1 arg1)\
	{return class##_##name(arg1);}
// method with two arguments
#define X2Method2(class,instance,name,atype1,atype2) inline \
	void instance##_##name(atype1 arg1, atype2 arg2)\
	{class##_##name(arg1,arg2);}
#define X2Method2return(class,instance,type,name,atype1,atype2) inline \
	type instance##_##name(atype1 arg1, atype2 arg2)\
	{return class##_##name(arg1,arg2);}
// method with three arguments
#define X2Method3(class,instance,name,atype1,atype2,atype3) inline \
	void instance##_##name(atype1 arg1, atype2 arg2, atype3 arg3)\
	{class##_##name(arg1,arg2,arg3);}
#define X2Method3return(class,instance,type,name,atype1,atype2,atype3) inline \
	type instance##_##name(atype1 arg1, atype2 arg2, atype3 arg3)\
	{return class##_##name(arg1,arg2,arg3);}
// method with four arguments
#define X2Method4(class,instance,name,atype1,atype2,atype3,atype4) inline \
	void instance##_##name(atype1 arg1, atype2 arg2, atype3 arg3, atype4 arg4)\
	{class##_##name(arg1,arg2,arg3,arg4);}
#define X2Method4return(class,instance,type,name,atype1,atype2,atype3,atype4) inline \
	type instance##_##name(atype1 arg1, atype2 arg2, atype3 arg3, atype4 arg4)\
	{return class##_##name(arg1,arg2,arg3,arg4);}




// generic X-Macros for use with multiple instance "classes"

// case 3: multiple instance, using a constructor returning an instance ID
//
// "Methods" receive an instance identifier as first parameter.

// instanciation
#define XInstanciation(type,instance) \
	type instance

// constructor with one argument
#define XConstructor1(class,instance,name,atype1) inline \
	void instance##_##name(atype1 arg1)\
	{instance=class##_##name(arg1);}
#define XConstructor2(class,instance,name,atype1,atype2) inline \
	void instance##_##name(atype1 arg1,atype2 arg2)\
	{instance=class##_##name(arg1,arg2);}
#define XConstructor3(class,instance,name,atype1,atype2,atype3) inline \
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
#define XMethod1(class,instance,name,atype1) inline \
	void instance##_##name(atype1 arg1)\
	{class##_##name(instance,arg1);}
#define XMethod1return(class,instance,type,name,atype1) inline \
	type instance##_##name(atype1 arg1)\
	{return class##_##name(instance,arg1);}




// case 4: multiple instance, no constructor, instance data in user space
//
// "Methods" receive a pointer to the instance data as first parameter.

// simple method without arguments
#define X4Method0(class,instance,name) inline \
	void instance##_##name()\
	{class##_##name(&instance);}
#define X4Method0return(class,instance,type,name) inline \
	type instance##_##name()\
	{return class##_##name(&instance);}
// method with one argument
#define X4Method1(class,instance,name,atype1) inline \
	void instance##_##name(atype1 arg1)\
	{class##_##name(&instance,arg1);}
#define X4Method1return(class,instance,type,name,atype1) inline \
	type instance##_##name(atype1 arg1)\
	{return class##_##name(&instance,arg1);}



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
        Xprinthelper1	(class,instance,print_f,double) \
        Xprinthelper2	(class,instance,print_fd,double,uint8_t) \
        Xprinthelper0	(class,instance,println) \
        Xprinthelper1	(class,instance,println_s,const char*) \
        Xprinthelper2	(class,instance,println_sn,const uint8_t*,size_t) \
        Xprinthelper1	(class,instance,println_i,long) \
        Xprinthelper1	(class,instance,println_u,unsigned long) \
        Xprinthelper2	(class,instance,println_ib,long,uint8_t) \
        Xprinthelper2	(class,instance,println_ub,unsigned long,uint8_t) \
        Xprinthelper1	(class,instance,println_f,double) \
        Xprinthelper2	(class,instance,println_fd,double,uint8_t)


#endif
