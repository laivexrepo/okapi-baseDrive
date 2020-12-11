// ------- globals.cpp ---------------------------------------------------------
//
// Use globals.cpp together with globals.h to define all motor and other objects
// which should be available/accessible in autonomous and opcontrol code.
//
// forexample setup a motor definition and then add a extern pros::motor to
// globals.h to ensure all other modules have access -- of course by including
// #include "globals.h" in the relevant source (.cpp) files

#include "main.h"
#include "portdef.h"

#include <iostream>       // For file system support in C++ ability to write to file stream
#include <fstream>
#include <chrono>
#include <ctime>

// --------------------- Global Motor Definitions ------------------------------

pros::Motor left_wheel_front (LEFT_MOTOR_FRONT, MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_wheel_back (LEFT_MOTOR_BACK, MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_wheel_front (RIGHT_MOTOR_FRONT, MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_wheel_back (RIGHT_MOTOR_BACK, MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

// -------------------- Remote Controls -----------------------------------------

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);

// ------------------ IMU ------------------------------------------------------

pros::Imu imu_sensor(IMU_PORT);

// ---------------- SHAFT encoder in drive train -------------------------------
pros::ADIEncoder encoderLeft (QUAD_LEFT_TOP_PORT, QUAD_LEFT_BOTTOM_PORT);
pros::ADIEncoder encoderRight (QUAD_RIGHT_TOP_PORT, QUAD_RIGHT_BOTTOM_PORT);

// --------------------- Global Function definitions ---------------------------

void robotDataLogger() {
  // This function can be called and will write an entry to the dataLogger file
  // which is stored on the SD card and can record actions as the program runs
  // for later evaluation of performance

}

void terminalLogger() {
  // functions logs to console terminal
  okapi::Logger::setDefaultLogger(
	    std::make_shared<okapi::Logger>(
	        okapi::TimeUtilFactory::createDefault().getTimer(), // It needs a Timer
	        "/ser/sout", // Output to the PROS terminal
	        okapi::Logger::LogLevel::info // Show info, errors and warnings
	    )
	);
}

std::ofstream myUsdFile;
bool usdLogEnable = false;

bool usdLoggerOpen() {
  // We are going to open a log file, but first we need to know if we have USD card installed
	if(pros::usd::is_installed()) {

		myUsdFile.open("/usd/loggerFile.txt", std::ios::trunc);	// truncate the file and then write to it
  	// check if the file was opened
		std::cout << "USD file system is ready for writing \n";
    // The V5 has no date/time functions or clock, so we can only use msec since start of execution
    // to track our timing.
  	myUsdFile << "Opened USD file for write at: " << pros::c::millis() << "ms since system uptime. \n";

    return(true);
  	//myUsdFile.close();
	} else {
		std::cout << "USD File system failed to open - either no card, wrong format or other error \n";
    return(false);
	}
}

void usdLoggerClose() {
  myUsdFile.close();
}
