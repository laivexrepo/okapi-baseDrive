#include "main.h"
#include "portdef.h"
#include "globals.h"
#include "autonomous.h"

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

  std::cout << "Setting up odometer in Okapi Lib \n";

	std::shared_ptr<okapi::OdomChassisController> chassis =
	  okapi::ChassisControllerBuilder()
	    .withMotors({LEFT_MOTOR_FRONT, LEFT_MOTOR_BACK}, {-RIGHT_MOTOR_FRONT, -RIGHT_MOTOR_BACK}) // left motor is 1, right motor is 2 (reversed)
	    // green gearset, 4 inch wheel diameter, 11.5 inch wheelbase
	    .withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 11.5_in}, okapi::imev5GreenTPR})
	    // left encoder in ADI ports A & B, right encoder in ADI ports C & D (reversed)
	    .withSensors(okapi::ADIEncoder{'C', 'D', true}, okapi::ADIEncoder{'A', 'B'})
	    // specify the tracking wheels diameter (2.75 in), track (7 in), and TPR (360)
	    .withOdometry({{2.75_in, 7_in}, okapi::quadEncoderTPR}, okapi::StateMode::FRAME_TRANSFORMATION)
	    .buildOdometry();

  // Now lets drive....
	// set the state to zero
	std::cout << "Setting starting position of 0,0,0 \n";
	chassis->setState({0_in, 0_in, 0_deg});

	// turn 45 degrees and drive approximately 1.4 ft
	std::cout << "Setting 45degree pivot 1ft, 1ft \n";
	chassis->driveToPoint({1_ft, 1_ft});

	// turn approximately 45 degrees to end up at 90 degrees
	std::cout << "Turn to complete to a 90degree angle \n";
	chassis->turnToAngle(90_deg);

	// turn approximately -90 degrees to face {5_ft, 0_ft} which is to the north of the robot
	std::cout << "Turn to a point 5ft, 0ft \n";
	chassis->turnToPoint({5_ft, 0_ft});

	//while (true) {

		//pros::delay(20);
	//}

	std::cout << "Finished........\n";

}
