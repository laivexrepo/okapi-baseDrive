#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "main.h"

// ------------ make sure all motors are available to all code -------------
extern pros::Motor left_wheel_front;
extern pros::Motor left_wheel_back;

extern pros::Motor right_wheel_front;
extern pros::Motor right_wheel_back;


// ---------- make sure we set objects for the remote controls ------------
extern pros::Controller master;
extern pros::Controller partner;

// --------- make sure we got IMU available ------------------------------
extern pros::Imu imu_sensor;

// --------- shaft encoders drive train -----------------------------------
extern pros::ADIEncoder encoderLeft;
extern pros::ADIEncoder encoderRight;

// ------------ Global accessible Logging functions -------------------------------
extern void robotDataLogger();

//extern void terminalLogger();     // terminal logger of OKAPI events
extern bool usdLoggerOpen();      // open a USD file logger
extern void usdLoggerClose();     // if USD file logger used close it before exiting program

extern std::ofstream myUsdFile;   // File stream variable
extern bool usdLogEnable;         // If USD based file logging is active and allowed

// ----------- Global variable to control drive base mode -----------------
#define ARCADE_MODE false     // run in tankmode - if true arcade mode

// ----------- Global variables to assist in code development ------------
#define DEBUG true            // turn on DEBUG statements, should be off in final build

#define MANUAL_AUTON false     // allow for autonomous testing use Remote intead of
                               // competition switch -- set to false for production
                               // and competition!!!

#define RUN_AUTON true         // Run autonomous by default at startup
// ---------- Global Task Variables ----------------------------------------


#endif
