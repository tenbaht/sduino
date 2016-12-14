// Define config for Serial.begin(baud, config);
/* coding of the bit field: 0www pp ss
 * wordlength www: 0:8 1:9 (5,6,7 not supported)
 * parity     pp:  0:n 2:e 3:o
 * stopbits   ss:  0:1 1:0.5 2:2 3:1.5
 * => 8:0x3x 9:0x4x
 * => n1:1 n2:2 e1:8 e2:A o1:C o2:E
*/
#define SERIAL_8N1 0x01
#define SERIAL_9N1 0x11
#define SERIAL_8N2 0x02
#define SERIAL_9N2 0x12
#define SERIAL_8E1 0x08
#define SERIAL_9E1 0x18
#define SERIAL_8E2 0x0A
#define SERIAL_9E2 0x1A
#define SERIAL_8O1 0x0C
#define SERIAL_9O1 0x1C
#define SERIAL_8O2 0x0E
#define SERIAL_9O2 0x1E
