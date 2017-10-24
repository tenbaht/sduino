# Preprocessor macros to disguise plain C as C++

Porting some Arduino-like functions to C is not too difficult. The challenge
is to keep the syntax differences to the real C++-based Arduino system as
small a possible while still using plain C.

There are subtle differences in the API concepts of the Arduino libraries.
Unfortunatly, they all require different strategies to mimic a C++ user API.
The basic criteria are:

* multi-instance or single-instance

* is a constructor call mandatory or not (typically the `begin()`-method)


Additionally, there are some more detailed destinctions:

* polymorph instantiation declaration or simple declaration

* general single instance library or pre-instantiated single instance
  library

* inheritance via virtual methods (most output-type libraries inherit from
  the Print class)

As a result, there are several different cases to consider:


## Single-instance, pre-instantiated, with constructor

  The data is kept locally within the class module. There is no
  instantiation declaration. No instance references need to be passed to any
  any function calls. All initialization is done by the constructor
  function.

  This is the easiest and most straight-forward case. No help from the
  preprocessor is required. It is sufficient to choose the function names to
  match the original class::method names. Polymorph methods are represented
  by a set of name-mangled functions.

  The behaviour of this construction is very similar to a real C++ class.

  **Examples:**
[SPI](api/SPI.md), [I2C](api/I2C.md),
[HardwareSerial](api/HardwareSerial.md) 

```c
#include <I2C.h>

setup() {
    I2c_begin();
    I2c_write(0x1e, 0x02,0x00);
}

```

### Required preprocessor help

* none



## Single-instance, with constructor

  The data is kept locally within the class module. The instantiation
  declaration defines the one instance name to be used. It might pass some
  configuration data which is remembered for later use. As there is always
  exactly one instance, no instance references need to be remembered and
  passed to any function calls. The instance name is more a cosmetic issue.

  The constructor does the actual configuration and initialization. It might
  use the configuration data given earlier with the instantiation
  declaration.

  The behaviour of this construction is very similar to a real C++ class.

  Polymorph instantiation declarations are possible.
  Non-constant values for the initialization are supported.

  **Examples:** [LiquidCrystal](api/LiquidCrystal.md)

```c
int rs_pin = 2;

LiquidCrystal (lcd, rs_pin,3,4, 5,6,7,8);

setup() {
    lcd_begin(16,2);
    lcd_setCursor(0,1);
    lcd_print_s("Hello, world!");
}
```


### Required preprocessor help

* polymorph instantiation
* remember configuration data for later use with the constructor
* setting up alias definitions to include the instance name in the function
  names.



## Multi-instance class, with constructor

  The data can be kept locally within the class module. The constructor
  allocates and initializes the required data structure and does required
  I/O-initializations. It returns a reference item to identify the instance.
  This reference item is typically a pointer to the instance structure or
  and index number to an internal table. It is passed to any following
  method call.

  The behaviour of this construction is very similar to a real C++ class.
  The main pitfall is the missing automatic destructor call. Luckily, this
  C++ feature is rarely used, anyway.

  Polymorph instantiation declarations are possible.

  Examples: [Servo](api/Servo.md)

```c
#include <Servo.h>

Servo(servo1);
Servo(servo2);

setup() {
    servo1_attach(5);
    servo2_attach(6);
}

loop() {
    servo1_write(90);
    servo2_write(150);
}
```


### Required preprocessor help

* polymorph instantiation
* remember configuration data for later use with the constructor
* save the instance reference item on initialization
* setting up alias definitions to 
  * include the instance name in the function names and
  * include the instance reference item with the parameter list of every function call.






## Multi-instance class, no constructor

  As there is no strictly defined first function call which could allocate
  and initialize internal data structures, the data has to be kept in a data
  structure in the user area. A pointer to this data structure is passed to
  any following method call.

  This case can be difficult. As there is no dedicated constructor call, the
  library has to keep track if the current instance has been initialized
  already and has to do so if not. That means, that the needed I/O resources
  can't be initialized before the first function call. That might be a
  problem for some applications as I/O pins might stay in a floating input
  state for quite some time.

  Second drawback of this concept is the fact, that the instance data
  initialization values have to be known at compile time and can't be
  variables or the result of function calls at run time, e.g. from a
  configuration space in EEPROM.

  To solve these two problems it might be necessary to introduce an
  additional constructor-type function, that can be called from the setup()
  function or for any needed reconfiguration. Since the instance data is in
  the user space anyway, it could be modified there directly. But this would
  be an even worse violation of all modularization concepts and should be
  avoided.

  Polymorph instantiation declarations are possible.

  Examples: [Stepper](api/Stepper.md)

  By requiring the user to add a call to the added constructor as the
  mandatory first call to the instance it could be treated as any other
  multi-instance class with constructor, see above. The resulting API is not
  identical to Arduino anymore, but still very similar.


```c
#include <Stepper.h>

Stepper (StepperA, 100, 6, 7);
Stepper (StepperB, 200, 8, 9, 10, 11);

void setup() {
  StepperA_setSpeed(60);
  StepperB_setSpeed(60);
}

void loop() {
  StepperA_step(20);
  StepperB_step(40);
}
```


### Required preprocessor help

* polymorph instantiation.
* convert the instantiation declaration into an instance data item declaration
  and initialization.
* setting up alias definitions to 
  * include the instance name in the function names and
  * include a pointer to the instance data item with the parameter list of
    every function call.




## Inheritance

Most output-type libraries inherit the methods from class Print by providing
a matching write() method. This works by providing all print functions a
function pointer to the write function, that should be used.

### Required preprocessor help

* setting up alias definitions to 
  * include the instance name of the output module in the function names of
    the print functions and
  * include a pointer to the output write function with the parameter list of
    every (aliased) print function call.

