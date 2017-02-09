#include <stdarg.h>
//#include <stdio.h>

int a_function ( int x, ... )
{
	va_list a_list;
	int sum=0;

	va_start( a_list, x );

	while (x--) sum += va_arg(a_list, int);

	return sum;
}

void main()
{
	a_function(2, 11,22);
	a_function(3, 11,22,33);
}
