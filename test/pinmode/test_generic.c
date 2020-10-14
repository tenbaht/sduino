/*
 * Test the use of the _Generic preprocessor directive
 */

void func_uint8(const unsigned char);
void func_int8(const unsigned char);
void func_uint16(const unsigned char);
void func_int16(const unsigned char);
void func_var(const unsigned char);

unsigned char mode;

void test_generic(void)
{
_Generic(mode,
//    const unsigned char:	func_uint8(mode),
//    const char:			func_int8(mode),
    const unsigned int:		func_uint16(mode),
    const int:			func_int16(mode),
    default:			func_var(mode)
);
}
