#include "main.h"
#include "portdef.h"
#include "globals.h"
#include "autonomous.h"

#include <iostream>
#include <fstream>

// used for logging and tracking our verisons
#define RUN_VARIABLE 11
#define PROGRAM_VERSION "0.3 Alpha"
#define VERSION_DATE "15-12-2020"


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
	std::cout << "Setting OKAPI log level \n";
	okapi::Logger::setDefaultLogger(
			std::make_shared<okapi::Logger>(
					okapi::TimeUtilFactory::createDefault().getTimer(), // It needs a Timer
					"/ser/sout", // Output to the PROS terminal
					okapi::Logger::LogLevel::info // Show info, errors and warnings -- warn, debug, info
			)
	);
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

	// For reading the okapi oodometer states
	okapi::OdomState currentState;		// Odometer state return structure
	okapi::QLength x, y;							// Length {x}_unit format
	okapi::QAngle theta;							// Angle of turn in odometer frame

  std::cout << "Attempting initialization of USD logger \n";
	// Lets facilitate logging to the USD file system for a record of our movements
	// if we are opeining a FILE logger we must also ensure that we coloe it in the end
	// using the call to usdLoggerClose()
	if(usdLoggerOpen()) {
		usdLogEnable = true;					// Set a local flag indicating we cna log to the filesystem
		std::cout << "USD logger setup succeeded and enabled\n";
	}
	if(usdLogEnable) {
		myUsdFile << "Program Run: " << RUN_VARIABLE << " Program Version: " << PROGRAM_VERSION;
		myUsdFile << " Version Date: " << VERSION_DATE << " \n";
	}


  // Now lets drive....
	if(encoderTest) {
    // temporary test mode, intended for pushing chassis and seeing if tracking wheel encoders
		// when robot is pushed forward are giving both postivie number increments.
		while (true) {
			// for debugging purppose get encoder counts as well and show on console
			std::cout << pros::c::millis() << "\t Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
		  std::cout << "RIGHT value: " << encoderRight.get_value() << "\n";

      pros::delay(20);
		}

	} else {

		std::cout << "Setting up odometer in Okapi Lib \n";

		std::shared_ptr<okapi::OdomChassisController> chassis =
			okapi::ChassisControllerBuilder()
				.withMotors({LEFT_MOTOR_FRONT, LEFT_MOTOR_BACK}, {RIGHT_MOTOR_FRONT, RIGHT_MOTOR_BACK}) // left motor is 1, right motor is 2 (reversed)
				// green gearset, 4 inch wheel diameter, 15 inch wheelbase
				// METRIC: 0.1016m diameter -- 0.3750m wheel base
				//.withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 15_in}, okapi::imev5GreenTPR})
				.withDimensions(okapi::AbstractMotor::gearset::green, {{0.1016_m, 0.3750_m}, okapi::imev5GreenTPR})
				// left encoder in ADI ports A & B, right encoder in ADI ports C & D (reversed)
				.withSensors(okapi::ADIEncoder{'C', 'D'}, okapi::ADIEncoder{'A', 'B'})
				// specify the tracking wheels diameter (2.75 in), track (9.75 in), and TPR (360)
				// METRIC: 0.06985m diameter -- 0.2450m wheel base
				//.withOdometry({{2.75_in, 9.75_in}, okapi::quadEncoderTPR}, okapi::StateMode::FRAME_TRANSFORMATION)
				.withOdometry({{0.06985_m, 0.2450_m}, okapi::quadEncoderTPR}, okapi::StateMode::FRAME_TRANSFORMATION)
				.buildOdometry();

		// Set the chassis maximum velocity to 100 RPM (range is 0 - 600RPM)
		std::cout << "Set the maximum velocity to: 100RPM \n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << "\t Set the maximum velocity to: 100RPM \n"; }
    chassis->setMaxVelocity(100);

		// set the state to zero
		std::cout << "Setting starting position of 0,0,0 \n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << "\t Setting starting position of 0,0,0 \n"; }

		chassis->setState({0_m, 0_m, 0_deg});
		// for debugging purppose get encoder counts as well and show on console
		std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
	  std::cout << "RIGHT value: " << encoderRight.get_value() << "\n";

		currentState = chassis->getState();
		std::cout << "Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << " ";
		std::cout << std::to_string(currentState.y.convert(okapi::meter)) << " " << std::to_string(currentState.theta.convert(okapi::degree)) << "\n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << "\t Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << "m ";
			myUsdFile << std::to_string(currentState.y.convert(okapi::meter)) << "m " << std::to_string(currentState.theta.convert(okapi::degree)) << "Deg. \n";
			myUsdFile << pros::c::millis() << "\t Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
			myUsdFile << "RIGHT value: " << encoderRight.get_value() << "\n";
		}

    // drive to a point 1m in front of robot
		std:: cout << "Drive 1m straight forward \n";
    chassis->driveToPoint({1_m,0_m});

		// for debugging purppose get encoder counts as well and show on console
		std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
		std::cout << "RIGHT value: " << encoderRight.get_value() << "\n";

		currentState = chassis->getState();
		std::cout << "Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << " ";
		std::cout << std::to_string(currentState.y.convert(okapi::meter)) << " " << std::to_string(currentState.theta.convert(okapi::degree)) << "\n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << "\t Drive 1m straight forward \n";
			myUsdFile << pros::c::millis() << "\t Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << "m ";myUsdFile << std::to_string(currentState.y.convert(okapi::meter)) << "m " << std::to_string(currentState.theta.convert(okapi::degree)) << "Deg. \n";
			myUsdFile << pros::c::millis() << "\t Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
			myUsdFile << "RIGHT value: " << encoderRight.get_value() << "\n";
		}

		// For testing purpose we are resetting Odometer frame to 0,0,0 to see if we get a nice 45degree turn
		// at the end of the 1m forward move action

		std::cout << "Setting starting position of 0,0,0 \n";
		chassis->setState({0_m, 0_m, 0_deg});

		// for debugging purppose get encoder counts as well and show on console
		std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
		std::cout << "RIGHT value: " << encoderRight.get_value() << "\n";

		currentState = chassis->getState();
		std::cout << "Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << " ";
		std::cout << std::to_string(currentState.y.convert(okapi::meter)) << " " << std::to_string(currentState.theta.convert(okapi::degree)) << "\n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << "\t Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << "m ";
			myUsdFile << std::to_string(currentState.y.convert(okapi::meter)) << "m " << std::to_string(currentState.theta.convert(okapi::degree)) << "Deg. \n";
			myUsdFile << pros::c::millis() << "\t Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
			myUsdFile << "RIGHT value: " << encoderRight.get_value() << "\n";
		}

		// turn 45 degrees and drive approximately 1.4m
		std::cout << "Setting 45degree pivot 1m, 1m \n";
		chassis->driveToPoint({1_m, 1_m});

		currentState = chassis->getState();
		// for debugging purppose get encoder counts as well and show on console
		std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
		std::cout << "RIGHT value: " << encoderRight.get_value() << "\n";

		std::cout << "Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << " ";
		std::cout << std::to_string(currentState.y.convert(okapi::meter)) << " " << std::to_string(currentState.theta.convert(okapi::degree)) << "\n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << "\t 45degree turn 1m,1m,0 \n";
			myUsdFile << pros::c::millis() << "\t Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << "m ";myUsdFile << std::to_string(currentState.y.convert(okapi::meter)) << "m " << std::to_string(currentState.theta.convert(okapi::degree)) << "Deg. \n";
			myUsdFile << pros::c::millis() << "\t Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
			myUsdFile << "RIGHT value: " << encoderRight.get_value() << "\n";
		}

		// Face a 90 degree angle -- turning in place.
		std::cout << "Turning to 90degree heading \n";
		chassis->turnToAngle(90_deg);

		// for debugging purppose get encoder counts as well and show on console
		std::cout << "Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
		std::cout << "RIGHT value: " << encoderRight.get_value() << "\n";

		currentState = chassis->getState();
		std::cout << "Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << " ";
		std::cout << std::to_string(currentState.y.convert(okapi::meter)) << " " << std::to_string(currentState.theta.convert(okapi::degree)) << "\n";
		if(usdLogEnable) { myUsdFile << pros::c::millis() << "\t Turning to 90degree heading \n";
			myUsdFile << pros::c::millis() << "\t Get state: " << std::to_string(currentState.x.convert(okapi::meter)) << "m ";myUsdFile << std::to_string(currentState.y.convert(okapi::meter)) << "m " << std::to_string(currentState.theta.convert(okapi::degree)) << "Deg. \n";
			myUsdFile << pros::c::millis() << "\t Encoder LEFT value: " << encoderLeft.get_value() << " -- ";
			myUsdFile << "RIGHT value: " << encoderRight.get_value() << "\n";
		}	}

	// Make sure that if we used USD file logging, we close it before the program ends
  if (usdLogEnable) {
		usdLoggerClose();
		std::cout << "Closing USD file logger..........\n";
	}

  std::cout << "Finished........\n";
}
