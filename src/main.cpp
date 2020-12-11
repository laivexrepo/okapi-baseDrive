#include "main.h"
#include "portdef.h"
#include "globals.h"
#include "autonomous.h"

#include <iostream>
#include <fstream>

// used for logging and tracking our verisons
#define RUN_VARIABLE 1
#define PROGRAM_VERSION "0.1 Aplpha"
#define VERSION_DATE "11-12-2020"

// For reading the okapi oodometer states
okapi::OdomState currentState;		// Odometer state return structure
okapi::QLength x, y;							// Length {x}_unit format
okapi::QAngle theta;							// Angle of turn in odometer frame

// We are writing okapi lib based code make sure that the following in main.h is uncommented:
// #include "okapi/api.hpp"
// We are not setting the name space so we need to preface okapi lib calls with okapi:: like
// we are doing for pros name space by prefacing calls with pros::
//
// To use literals i.e. 1_in add following to main.h: using namespace okapi::literals;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	terminalLogger();					// log OKAPI lib internal debug info to the console
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	bool encoderTest = false;				// Temporary to facilitate tracking wheel testing

	// Lets facilitate logging to the USD file system for a record of our movements
	// if we are opeining a FIEL logger we must also ensure that we coloe it in the end
	// usign the call to usdLoggerClose()
	if(usdLoggerOpen()) {
		usdLogEnable = true;					// Set a local flag indicating we cna log to the filesystem
	}

	if(usdLogEnable) {
		myUsdFile << "Program Run: " << RUN_VARIABLE << " Program Version: " << PROGRAM_VERSION;
		myUsdFile << " Version Date: " << VERSION_DATE << " \n";
	}

  std::cout << "Setting up odometer in Okapi Lib \n";

  // build a chassis controller with odometer support
	// Key to note: wheel base for driving wheels versus tracking wheel
	// Aodomoter ADIencoders ports (top/bottom) and if reversed or not
	std::shared_ptr<okapi::OdomChassisController> chassis =
	  okapi::ChassisControllerBuilder()
	    .withMotors({LEFT_MOTOR_FRONT, LEFT_MOTOR_BACK}, {RIGHT_MOTOR_FRONT, RIGHT_MOTOR_BACK}) // left motor is 1, right motor is 2 (reversed)
	    // green gearset, 4 inch wheel diameter, 15 inch wheelbase
	    .withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 15_in}, okapi::imev5GreenTPR})
	    // left encoder in ADI ports A & B, right encoder in ADI ports C & D (reversed)
	    .withSensors(okapi::ADIEncoder{'C', 'D'}, okapi::ADIEncoder{'A', 'B'})
	    // specify the tracking wheels diameter (2.75 in), track (9.75 in), and TPR (360)
	    .withOdometry({{2.75_in, 9.75_in}, okapi::quadEncoderTPR}, okapi::StateMode::FRAME_TRANSFORMATION)
	    .buildOdometry();

  // Now lets drive....
	if(encoderTest) {
    // temporary test mode, intended for pushing chassis nad seeing if trackign wheel encoders
		// when robot is pushed forward are giving both postivie number increments.
		while (true) {
			std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
			std::cout << "Encoder RIGHT value: " << encoderRight.get_value() << "\n";

      pros::delay(20);
		}

	} else {
		// Set the chassis maximum velocity to 100 RPM (range is 0 - 600RPM)
		std::cout << "Set the maximum velocity to: 100RPM \n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << " Set the maximum velocity to: 100RPM \n"; }
    chassis->setMaxVelocity(100);

		// set the state to zero
		std::cout << "Setting starting position of 0,0,0 \n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << " Setting starting position of 0,0,0 \n"; }

		chassis->setState({0_m, 0_m, 0_deg});
		std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
		std::cout << "Encoder RIGHT value: " << encoderRight.get_value() << "\n";

		currentState = chassis->getState();


		x = currentState.x;								// forward direction
		y = currentState.y;								// sideways direction
		theta = currentState.theta;				// angle

		std::cout << "Get state: " << std::to_string(x.convert(okapi::meter)) << " ";
		std::cout << std::to_string(y.convert(okapi::meter)) << " " << std::to_string(theta.convert(okapi::degree)) << "\n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << " Get state: " << std::to_string(x.convert(okapi::meter)) << " ";
			myUsdFile << std::to_string(y.convert(okapi::meter)) << " " << std::to_string(theta.convert(okapi::degree)) << "\n";
		}

    // drive to a point 1m in front of robot
		std:: cout << "Drive 4ft straight forward \n";
    chassis->driveToPoint({1_m,0_m});

		// for debugging purppose get encoder counts as well
		std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
		std::cout << "Encoder RIGHT value: " << encoderRight.get_value() << "\n";

		currentState = chassis->getState();
		x = currentState.x;
		y = currentState.y;
		theta = currentState.theta;

		std::cout << "Get state: " << std::to_string(x.convert(okapi::meter)) << " ";
		std::cout << std::to_string(y.convert(okapi::meter)) << " " << std::to_string(theta.convert(okapi::degree)) << "\n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << " Get state: " << std::to_string(x.convert(okapi::meter)) << " ";
		 	myUsdFile << std::to_string(y.convert(okapi::meter)) << " " << std::to_string(theta.convert(okapi::degree)) << "\n";
    }

		// For testing purpose we are resetting Odometer frame to 0,0,0 to see if we get a nive 45degree turn
		// at the end of the 1m move action
		std::cout << "Setting starting position of 0,0,0 \n";
		chassis->setState({0_m, 0_m, 0_deg});

		// for debugging purppose get encoder counts as well
		std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
		std::cout << "Encoder RIGHT value: " << encoderRight.get_value() << "\n";

		// turn 45 degrees and drive approximately 1.4m
		std::cout << "Setting 45degree pivot 1m, 1m \n";
		chassis->driveToPoint({1_m, 1_m});

		// for debugging purppose get encoder counts as well
		std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
		std::cout << "Encoder RIGHT value: " << encoderRight.get_value() << "\n";

    // Display the current state of robot within the odomoter frame
		currentState = chassis->getState();
		x = currentState.x;
		y = currentState.y;
		theta = currentState.theta;

		std::cout << "Get state: " << std::to_string(x.convert(okapi::meter)) << " ";
		std::cout << std::to_string(y.convert(okapi::meter)) << " " << std::to_string(theta.convert(okapi::degree)) << "\n";

    // Write to USD log file as well
		if(usdLogEnable) { myUsdFile << pros::c::millis() << " Get state: " << std::to_string(x.convert(okapi::meter)) << " ";
		 	myUsdFile << std::to_string(y.convert(okapi::meter)) << " " << std::to_string(theta.convert(okapi::degree)) << "\n";
    }


	}

	// Make sure that if we used USD file loggign, we close it before the program ends
  if (usdLogEnable) {
		usdLoggerClose();
		std::cout << "Closing USD file logger..........\n";
	}
  std::cout << "Finished........\n";
}
