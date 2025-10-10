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
	pros::Task OdomTask(Odometry,"Odom"); //adds the odometry task to the stack of tasks
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
void autonomous() {} // to be moved to another file

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

float CalcChargeAmt(float Input)
{
	//assuming that the power of the gun is exponential, we can use the sqrt to get how much to charge the gun using an input of how many balls we wanna put in
	int Base = 200;//in degrees how much it rotates to preform 1 ball
	return sqrt(Input) * Base;
}



void Shoot()
{
	pros::adi::DigitalOut ReleasePiston(8);
	ReleasePiston.set_value(0); // fire
	pros::delay(1000);
	ReleasePiston.set_value(1); // reset
}

void Railgun()//seperate always running thread
{
	pros::Motor UtilityA(UtilityMotorA,pros::v5::MotorGearset::blue,pros::v5::MotorEncoderUnits::degrees);
	pros::Motor UtilityB(UtilityMotorB,pros::v5::MotorGearset::blue,pros::v5::MotorEncoderUnits::degrees);
	pros::adi::Encoder RailgunEnc (3, 4, false);
	pros::Controller MainCont(pros::E_CONTROLLER_MASTER);
	int LocalBalls = 0; // local variable for ChargeAmount
	int LastChange = 0; //delta for this, to detect when a new value is there
	int Target = CalcChargeAmt(ChargeAmount);
	int Init = 0;
	int error = RailgunEnc.get_value() - Target;
	const int Tolerance = 15;

	while (true)
	{
		if (!UtilityMode) //not intaking, start charging railgun
		{
			LocalBalls = ChargeAmount.load();

			error = RailgunEnc.get_value() - Target;

			if (error > Tolerance)
			{
				UtilityA.move(-127);
				UtilityB.move(-127);//voltage
				RGReady = false;
			}else
			{
				UtilityA.move(0);
				UtilityB.move(0); // stop
				RGReady = true;
			}

			if (MainCont.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && RGReady)// to be updated with real number
			{
				RailgunEnc.reset();
				Shoot();
			}
			if (LocalBalls != LastChange) //update the target
			{
				Target = CalcChargeAmt(ChargeAmount);
			}
		}
		LastChange = LocalBalls;
		pros::delay(500);
	}	
}



void Intake() // intended to be a seperate thread
{
	
	pros::Controller MainCont(pros::E_CONTROLLER_MASTER);	
	//for now, assume that CW is intake, CCW is railgun + outtake
	pros::Motor UtilityA(UtilityMotorA,pros::v5::MotorGearset::blue,pros::v5::MotorEncoderUnits::degrees);
	pros::Motor UtilityB(UtilityMotorB,pros::v5::MotorGearset::blue,pros::v5::MotorEncoderUnits::degrees);

	pros::adi::Encoder IntakeRotation({ 11, 5, 6}, false); // no 6 7 jokes :(
					//expander port, top port, bottom port

	bool Button = MainCont.get_digital(pros::E_CONTROLLER_DIGITAL_A);
	const int InitialDegrees = IntakeRotation.get_value(); // the starting position at the start of the funciton. used for relative movements
	const int DegreesPer = 180;//temp magic number, the motor should spin this amount to intake 1 ball
	int error = IntakeRotation.get_value() - InitialDegrees; // how far the intake has rotated from the initial position
	const int Tolerance = 5; // Degrees of error allowed


	//imma only allow 1 thread to access Utility motors at a time
	UtilityMode = true; // force railgun to stop charging
	UtilityA.move(127);
	UtilityB.move(127);//voltage

	while (abs(error) > Tolerance)
	{
		error = IntakeRotation.get_value() - InitialDegrees;
		pros::delay(20);
	}
	Button = MainCont.get_digital(pros::E_CONTROLLER_DIGITAL_A);
	if (Button) Intake(); // if the button is still held, restart the intake function
	UtilityA.move(0);
	UtilityB.move(0);
	UtilityMode = false; // allow railgun to charge again

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
	const int mod = 7;// the exponent for the curve, higher = more curve
	float heading;
	float x;		//local variables
	float y;
	int LocalBalls; // local variable for ChargeAmount
	bool BallincHeld = false;


	while (true) 
	{

		/*			Drivetrain Functions			*/

		F = MainCont.get_analog(ANALOG_LEFT_Y);
		T = MainCont.get_analog(ANALOG_RIGHT_X);

		left = F + T;
		right = F - T;

		LeftMG.move((100*(((1-curve)*left)/100+(curve*pow(left/100,mod)))));
		RightMG.move((100*(((1-curve)*right)/100+(curve*pow(right/100,mod)))));

		/*			Utility Motors Functions			*/

		if (MainCont.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !BallincHeld)
		{
			LocalBalls++;
			BallincHeld = true;
			
		}else if( MainCont.get_digital(pros::E_CONTROLLER_DIGITAL_UP)  && !BallincHeld) //sets the ball count for the railgun to be set to
		{
			LocalBalls--;
			BallincHeld = true;
		}else 
		{
			BallincHeld = false;
		}

		if (MainCont.get_digital(pros::E_CONTROLLER_DIGITAL_A) && !UtilityMode) // if the button is pressed and the intake isnt already running
		{
			pros::Task IntakeTask(Intake,"Intake");
		}


		/*			Screen Functions			*/
		heading = (float)Heading.load();
		x = (float)X.load();			//loading the atomic variable, whenever its safe, then put it to a local variable
		y = (float)Y.load();

		pros::screen::print(pros::E_TEXT_MEDIUM,3, "X: %f, Y: %f, Heading: %f" , x, y, heading);

		pros::delay(20);
	}
}