/*
 * Stepper.h - Stepper library for Wiring/Arduino - Version c/1.1.0
 *
 * Original library        (0.1)   by Tom Igoe.
 * Two-wire modifications  (0.2)   by Sebastian Gassner
 * Combination version     (0.3)   by Tom Igoe and David Mellis
 * Bug fix for four-wire   (0.4)   by Tom Igoe, bug fix from Noah Shibley
 * High-speed stepping mod         by Eugene Kozlenko
 * Timer rollover fix              by Eugene Kozlenko
 * Five phase five wire    (1.1.0) by Ryan Orendorff
 * Ported to C syntax      (c/1.1.0) by Michael Mayer
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Drives a unipolar, bipolar, or five phase stepper motor.
 *
 * When wiring multiple stepper motors to a microcontroller, you quickly run
 * out of output pins, with each motor requiring 4 connections.
 *
 * By making use of the fact that at any time two of the four motor coils are
 * the inverse of the other two, the number of control connections can be
 * reduced from 4 to 2 for the unipolar and bipolar motors.
 *
 * A slightly modified circuit around a Darlington transistor array or an
 * L293 H-bridge connects to only 2 microcontroler pins, inverts the signals
 * received, and delivers the 4 (2 plus 2 inverted ones) output signals
 * required for driving a stepper motor. Similarly the Arduino motor shields
 * 2 direction pins may be used.
 *
 * The sequence of control signals for 5 phase, 5 control wires is as follows:
 *
 * Step C0 C1 C2 C3 C4
 *    1  0  1  1  0  1
 *    2  0  1  0  0  1
 *    3  0  1  0  1  1
 *    4  0  1  0  1  0
 *    5  1  1  0  1  0
 *    6  1  0  0  1  0
 *    7  1  0  1  1  0
 *    8  1  0  1  0  0
 *    9  1  0  1  0  1
 *   10  0  0  1  0  1
 *
 * The sequence of control signals for 4 control wires is as follows:
 *
 * Step C0 C1 C2 C3
 *    1  1  0  1  0
 *    2  0  1  1  0
 *    3  0  1  0  1
 *    4  1  0  0  1
 *
 * The sequence of controls signals for 2 control wires is as follows
 * (columns C1 and C2 from above):
 *
 * Step C0 C1
 *    1  0  1
 *    2  1  1
 *    3  1  0
 *    4  0  0
 *
 * The circuits can be found at
 *
 * http://www.arduino.cc/en/Reference/Stepper
 */

// ensure this library description is only included once
#ifndef Stepper_h
#define Stepper_h

// library interface description

#ifdef __cplusplus

// C++ interface for use with regular gcc compiler
class Stepper {
  public:
    // constructors:
    Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2);
    Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                 int motor_pin_3, int motor_pin_4);
    Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                 int motor_pin_3, int motor_pin_4,
                                 int motor_pin_5);

    // speed setter method:
    void setSpeed(long whatSpeed);

    // mover method:
    void step(int number_of_steps);

    int version(void);

  private:
    void stepMotor(int this_step);

    int direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int pin_count;            // how many pins are in use.
    int step_number;          // which step the motor is on

    // motor pin numbers:
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;
    int motor_pin_5;          // Only 5 phase motor

    unsigned long last_step_time; // time stamp in us of when the last step was taken
};

#else

// plain C interface for use with SDCC

// the descriptor structure for one motor
struct Stepper_s {
    unsigned char	direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    unsigned char	pin_count;            // how many pins are in use.
    unsigned char	step_number;          // which step the motor is on

    // motor pin numbers:
    unsigned char	motor_pin_1;
    unsigned char	motor_pin_2;
    unsigned char	motor_pin_3;
    unsigned char	motor_pin_4;
    unsigned char	motor_pin_5;	// Only 5 phase motor

    unsigned long last_step_time; // time stamp in us of when the last step was taken
};

// re-Initialize the motor connections at run time (optional)
#ifdef NO_5PHASE
void Stepper_4phase(
        struct Stepper_s *this,
        unsigned int number_of_steps,
        unsigned char motor_pin_1,
        unsigned char motor_pin_2,
        unsigned char motor_pin_3,
        unsigned char motor_pin_4
);
#else
void Stepper_5phase(
        struct Stepper_s *this,
        unsigned int number_of_steps,
        unsigned char motor_pin_1,
        unsigned char motor_pin_2,
        unsigned char motor_pin_3,
        unsigned char motor_pin_4,
        unsigned char motor_pin_5
);
#endif

// set the pin mode for the motor pins (optional)
void Stepper_activateOutputs(struct Stepper_s *this);

// speed setter method:
void Stepper_setSpeed(struct Stepper_s *this, long whatSpeed);

// mover method:
void Stepper_step(struct Stepper_s *this, int number_of_steps);

// replace the version() method with a simple define
inline int Stepper_version(struct Stepper_s *this){(void)this; return 5;}

// (not so) private:
void Stepper_stepMotor(struct Stepper_s *this, int this_step);


// Pseudo-OO interface: Plain C disguised as almost-C++, thanks to X-Macros

#include <xmacro.h>


/* usage:
 *
 * Instantiation
 * -------------
 *
 * Typically as a global declaration, but a local declaration would be
 * possible as well. Has to be placed in the source file before any of the
 * "methods" can be used. Supports polymorphism.
 *
 * Variants are
 *	2 phase (bipolar) motor
 *	4 phase (unipolar) motor
 *	5 phase motor
 *
 * Syntax:
 *	Stepper (instancename,NumSteps,Pin1,Pin2);
 *	Stepper (instancename,NumSteps,Pin1,Pin2,Pin3,Pin4);
 *	Stepper (instancename,NumSteps,Pin1,Pin2,Pin3,Pin4,Pin5);
 *
 *
 * Constructors (typically in the setup() function)
 * ------------------------------------------------
 *
 * There is no explicit constructor method. The output mode is set
 * automatically right before the very first motor step.
 *
 * This means that the driver pins stay in input mode and are left floating
 * until the first step is requested. This won't be a problem in most cases
 * as most motor drivers are using pull-up or pull-down resistors to ensure
 * definite signal levels.
 *
 * If needed, the application could call the stepMotor(0) method in the
 * setup() function to force an immediate port pin initialization:
 *
 * setup() {
 *   myStepper_stepMotor(0);	// sets the motor pins to output mode
 * }
 *
 *
 * Methods
 * -------
 *
 *      instancename_setSpeed(speed);     // set speed in rpm
 *      instancename_step(numberOfSteps); // mover method
 *      instancename_version();           // return the library version
 *      instancename_stepMotor(stepid);   // internal, use for output init
 *
 */



// special macros specific to this "class"

// The first macro is the instantiation macro. It allocates and initializes
// a 'struct stepper_s' data stucture directly. This breaks the module
// encapsulation of the data structures, but it is the only way to get away
// without the need to call a begin() method in the setup() function.
// A pointer to this struct is used as the instance reference later.
#define XStepperInst(instance,N,P1,P2,P3,P4,P5,...) \
        struct Stepper_s instance = {0,0,N,0,0,P1,P2,P3,P4,P5,0};


// These macros define an equivalent to the begin() method. Not part of the
// original library design, these functions offer a possiblility to change
// the motor configuration at run time.

#ifdef NO_5PHASE
// without 5 phase support: use Stepper_4phase for re-Init
#define XStepperBegin2(instance) inline \
        void instance##_2phase( \
            unsigned arg1, \
            uint8_t arg2, uint8_t arg3 \
        ){ \
            Stepper_4phase(&instance,arg1,arg2,arg3,0,0); \
        }
#define XStepperBegin4(instance) inline \
        void instance##_4phase( \
            unsigned arg1, \
            uint8_t arg2, uint8_t arg3, uint8_t arg4, uint8_t arg5 \
        ){ \
            Stepper_4phase(&instance,arg1,arg2,arg3,arg4,arg5); \
        }
#define XStepperBegin5(instance)
        // empty define, no 5 phase support

#else
// with 5 phase support: use Stepper_5phase for re-Init
#define XStepperBegin2(instance) inline \
        void instance##_2phase( \
            unsigned arg1, \
            uint8_t arg2, uint8_t arg3 \
        ){ \
            Stepper_5phase(&instance,arg1,arg2,arg3,0,0,0); \
        }
#define XStepperBegin4(instance) inline \
        void instance##_4phase( \
            unsigned arg1,\
            uint8_t arg2, uint8_t arg3, uint8_t arg4, uint8_t arg5 \
        ){ \
            Stepper_5phase(&instance,arg1,arg2,arg3,arg4,arg5,0); \
        }
#define XStepperBegin5(instance) inline \
        void instance##_5phase( \
            unsigned arg1,\
            uint8_t arg2, uint8_t arg3, uint8_t arg4, uint8_t arg5, uint8_t arg6 \
        ){ \
            Stepper_5phase(&instance,arg1,arg2,arg3,arg4,arg5,arg6); \
        }

#endif


// The instantiation macro *must* be first in the list to allow for a
// extern declaration if the global "object" is used in different source code
// modules.
//
// The (unneeded) extern declaration at the end of the list consumes the
// semicolon following at the end of line after the macro call.
#define Stepper(instance,...) \
	XStepperInst	(instance,__VA_ARGS__,0,0,0) \
	\
	XStepperBegin2	(instance) \
	XStepperBegin4	(instance) \
	XStepperBegin5	(instance) \
        \
	X4Method1	(Stepper,instance,setSpeed,long) \
	X4Method1	(Stepper,instance,step,int) \
	X4Method0	(Stepper,instance,activateOutputs) \
	X4Method0return	(Stepper,instance,int,version) \
	extern struct stepper_s instance##_dummy


#endif // ifdef __cplusplus

#endif

