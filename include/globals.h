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

// --------- makre sure we got IMU available ------------------------------
extern pros::Imu imu_sensor;

// ------------ Global accessible functions -------------------------------
extern void robotDataLogger();

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
