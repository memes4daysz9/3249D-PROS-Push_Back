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
	pros::delay(150); // this makes sure system devices initialize before this, ironically itll just skip things in here bc RTOS and the screen arent initialized yet
	pros::screen::set_pen(0x00ffffff);
	pros::screen::print(pros::E_TEXT_MEDIUM,1, "latest Time working on the code: 1:05AM");
	pros::Imu IMUa(12);
    pros::Imu IMUb(13);

	IMUa.reset(true);
	IMUb.reset(true);
	IMUa.set_data_rate(5);
	IMUb.set_data_rate(5);
	pros::Task OdomTask(Odometry,"Odom"); //adds the odometry task to the stack of tasks
	pros::Motor BarMotor(7,pros::v5::MotorGears::red,pros::v5::MotorEncoderUnits::degrees);
	BarMotor.set_brake_mode(MOTOR_BRAKE_HOLD);
	
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() 
{

}

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


/*			Rotation PID testing			*/

void autonomous() {
	Rel_Rotate(15);
	Rel_Rotate(-15);
	Rel_Rotate(45);
	Rel_Rotate(-45);
	Rel_Rotate(90);
	Rel_Rotate(-90);
	Rel_Rotate(145);
	Rel_Rotate(-145);
	Rel_Rotate(180);
	Rel_Rotate(-180);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
} 
// to be moved to another file

/*			Straight			*/
/*void autonomous()
{
	pros::Motor IntakeMotor(7,pros::v5::MotorGears::blue,pros::v5::MotorEncoderUnits::degrees);
	pros::Motor HighGoalMotor(8,pros::v5::MotorGears::blue,pros::v5::MotorEncoderUnits::degrees);

	Rel_Move(5);
	Rel_Move(-5);
	Rel_Move(10);
	Rel_Move(-10);
	Rel_Move(20);
	Rel_Move(-20);
	Rel_Move(40);
	Rel_Move(-40);
	Rel_Move(65);
	Rel_Move(-65);
}*/

/*void autonomous()
{
	Rel_Move(20);
	Rel_Rotate(180);
	Rel_Move(20);
}*/

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





void opcontrol()
{

	pros::Controller MainCont(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup LeftMG({-1, -3, -5}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
	pros::MotorGroup RightMG({2, 4, 6}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
	pros::Motor BarMotor(7,pros::v5::MotorGears::red,pros::v5::MotorEncoderUnits::degrees);
	pros::Motor IntakeMotor(8,pros::v5::MotorGear::blue, pros::v5::MotorUnits::degrees);

	float F; // Forward input from controller
	float T;// Turning input from controller
	float left; // Left side output
	float right;// Right side output
	const float curve = 0.75; // the input control curve
	const int mod = 7;// the exponent for the curve, higher = more curve
	float heading;
	double x;		//local variables
	double y;
	double LeftWattage = 0;
	double RightWattage = 0;
	double WattageDiff = 0;
	

	while (true) 
	{

		/*			Drivetrain Functions			*/

		F = MainCont.get_analog(ANALOG_LEFT_Y);
		T = MainCont.get_analog(ANALOG_RIGHT_X);

		left = F + T;
		right = F - T;

		LeftMG.move((100*(((1-curve)*left)/100+(curve*pow(left/100,mod)))));
		RightMG.move((100*(((1-curve)*right)/100+(curve*pow(right/100,mod)))));

		/*			4Bar + Intake Functions			*/
		BarMotor.move_velocity((MainCont.get_digital(HoodButton) - MainCont.get_digital(DIGITAL_R2)) * 100); // temp

		IntakeMotor.move_velocity((MainCont.get_digital(DIGITAL_L2) - MainCont.get_digital(DIGITAL_L1)) * 600);
		

		/*			Screen Functions			*/
		heading = (float)Heading.load();
		x = X.load();			//loading the atomic variable, whenever its safe, then put it to a local variable
		y = Y.load();
		LeftWattage = LeftMG.get_power();
		RightWattage = RightMG.get_power();

		pros::screen::print(pros::E_TEXT_MEDIUM,3, "X: %f, Y: %f, Heading: %f" , x, y, heading);
		pros::screen::print(pros::E_TEXT_MEDIUM,4,"Total Power Left-Side: %f", LeftWattage);
		pros::screen::print(pros::E_TEXT_MEDIUM,5,"Total Power Right-Side: %f", RightWattage);
		pros::screen::print(pros::E_TEXT_MEDIUM,6,"Wattage Diff: %f", abs(RightWattage - LeftWattage) / 3);

		MainCont.print(0, 0, "% Power: %f", ((100*(((1-curve)*left)/100+(curve*pow(left/100,mod))))/4.31));

		pros::delay(20);
	}
}