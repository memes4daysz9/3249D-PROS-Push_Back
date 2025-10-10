#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "latest Time working on the code: 1:05AM");
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


void Intake() // intended to be a seperate thread
{
	pros::Controller MainCont(pros::E_CONTROLLER_MASTER);
	int Timeout = 300;//milliseconds till it switches from Singular to Hold to intake
	bool Button = MainCont.get_digital(pros::E_CONTROLLER_DIGITAL_A);
	const int DegreesPer = 180;//temp magic number, the motor should spin this amount to intake 1 ball
	int TimeSinceCall = 0;// in ms

	while (Button)
	{
		Button = MainCont.get_digital(pros::E_CONTROLLER_DIGITAL_A);

		pros::Task::delay_until(pros::millis, 50);// every 50ms, loop this again till called again

	}

}


void opcontrol()
{

	pros::Controller MainCont(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup LeftMG({1, 3, 5});    
	pros::MotorGroup RightMG({2, 4, 6});  

	float F; // Forward input from controller
	float T;// Turning input from controller
	float left; // Left side output
	float right;// Right side output
	const float curve = 0.75; // the input control curve
	bool IntakeSingular;//

	while (true) 
	{
		F = MainCont.get_analog(ANALOG_LEFT_Y);
		T = MainCont.get_analog(ANALOG_RIGHT_X);

		left = F + T;
		right = F - T;

		LeftMG.move((100*(((1-curve)*left)/100+(curve*pow(left/100,7)))));
		RightMG.move((100*(((1-curve)*right)/100+(curve*pow(right/100,7)))));
		pros::delay(20);
	}
}