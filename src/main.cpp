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
	pros::screen::print(pros::E_TEXT_MEDIUM,1, "latest Time working on the code: 2:33AM");
	pros::Imu IMUa(12);
    pros::Imu IMUb(13);

	IMUa.reset(true);
	IMUb.reset(true);
	IMUa.set_data_rate(5);
	IMUb.set_data_rate(5);
	pros::Task OdomTask(Odometry,"Odom"); //adds the odometry task to the stack of tasks	
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

/*void autonomous() {
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
}*/
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

											/*			Left Side			*/

/*void autonomous()
{


}*/


											/*			Right Side			*/
											
void autonomous(){
	pros::Motor HoodMotor(7,pros::v5::MotorGears::blue,pros::v5::MotorEncoderUnits::degrees);
	pros::Motor IntakeMotor(8,pros::v5::MotorGear::blue, pros::v5::MotorUnits::degrees);
	pros::adi::Pneumatics WingsPiston('c',false); // starts retracted
	pros::adi::Pneumatics MiddleScorePiston('b',true); // starts extended
	pros::adi::Pneumatics MatchLoaderPiston('a',false);
	pros::MotorGroup LeftMG({-1, -3, -5});
	pros::MotorGroup RightMG({2, 4, 6});
	Rel_Move(24);
	Rel_Rotate(-5);
	IntakeMotor.move_voltage(12000);
	Rel_Move(3.25);
	Rel_Rotate(-19);
	Rel_Move(5);
	Rel_Move(7);
	Rel_Rotate(-114);
	Rel_Move(36);
	Rel_Rotate(-40);
	Rel_Move(-20);
	HoodMotor.move_voltage(12000);
	pros::delay(1500);
	HoodMotor.move_voltage(0);
	Rel_Move(19);
	MatchLoaderPiston.extend();
	pros::delay(1500);
	LeftMG.move_voltage(12000);
	RightMG.move_voltage(12000);
	pros::delay(250);
	LeftMG.move_voltage(0);
	RightMG.move_voltage(0);
	MatchLoad(3);
	//untested
	Rel_Move(24);
	HoodMotor.move_voltage(12000);
	
}

/*void autonomous()
{
	pros::Motor HoodMotor(7,pros::v5::MotorGears::blue,pros::v5::MotorEncoderUnits::degrees);
	pros::Motor IntakeMotor(8,pros::v5::MotorGear::blue, pros::v5::MotorUnits::degrees);
	pros::adi::Pneumatics WingsPiston('c',false); // starts retracted
	pros::adi::Pneumatics MiddleScorePiston('b',true); // starts extended
	pros::adi::Pneumatics MatchLoaderPiston('a',false);

	Rel_Move(24);
	Rel_Rotate(19);
	IntakeMotor.move_voltage(12000);
	Rel_Move(5);
	Rel_Rotate(10);
	Rel_Move(5);
	Rel_Rotate(90);
	Rel_Move(34);

	Rel_Rotate(40);
	MatchLoaderPiston.extend();
	pros::delay(750);
	Rel_Move(5.5);
	MatchLoad(2);// 2 seconds
	Rel_Move(-24);

}*/


											/*			Right Side AWP			*/
/*void autonomous()
{
	pros::Motor HoodMotor(7,pros::v5::MotorGears::blue,pros::v5::MotorEncoderUnits::degrees);
	pros::Motor IntakeMotor(8,pros::v5::MotorGear::blue, pros::v5::MotorUnits::degrees);
	pros::adi::Pneumatics WingsPiston('c',false); // starts retracted
	pros::adi::Pneumatics MiddleScorePiston('b',true); // starts extended
	pros::adi::Pneumatics MatchLoaderPiston('a',false);
	
	Rel_Move(60);
	Rel_Rotate(-90);
	MatchLoaderPiston.extend();
	Rel_Move(21.5);
	MatchLoad(2);
	Rel_Move(-48);
	MatchLoaderPiston.retract();
	IntakeMotor.move_voltage(12000);
	HoodMotor.move_voltage(12000);
	pros::delay(3000);
	HoodMotor.move_voltage(0);
	Rel_Move(24);
	
	Rel_Rotate(145);
	Rel_Move(24);
	Rel_Rotate(5);
	Rel_Move(5);
	Rel_Move(-5);
	Rel_Rotate(-10);
	Rel_Move(5);
	Rel_Move(-5);
	IntakeMotor.move_voltage(0);
	Rel_Rotate(5);
	Rel_Move(18);
	IntakeMotor.move_voltage(-12000);
}
*/

				/*			Testing Matchloading			*/

/*void autonomous()
{
	pros::adi::Pneumatics MatchLoaderPiston('a',false);
	MatchLoaderPiston.extend();
	pros::delay(1000);
	MatchLoad(1.5);

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

bool Player2 = 0;

bool Toggle2ndPlayer(){
	pros::Controller MainCont(pros::E_CONTROLLER_MASTER);
	pros::Controller SideCont(pros::E_CONTROLLER_PARTNER);
	pros::MotorGroup LeftMG({-1, -3, -5}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
	pros::MotorGroup RightMG({2, 4, 6}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
	LeftMG.set_brake_mode(MOTOR_BRAKE_HOLD);
    RightMG.set_brake_mode(MOTOR_BRAKE_HOLD);
	LeftMG.brake();// so 
	LeftMG.brake();
	Player2 = ((Player2 + 1)*(Player2 < 1)); // 0 and 1
	MainCont.rumble("..");
	pros::delay(250);
	SideCont.rumble(".."); 
	LeftMG.set_brake_mode(MOTOR_BRAKE_COAST);
    RightMG.set_brake_mode(MOTOR_BRAKE_COAST);
	return true;
}


void opcontrol()
{
	pros::Controller MainCont(pros::E_CONTROLLER_MASTER);
	pros::Controller SideCont(pros::E_CONTROLLER_PARTNER);
	pros::MotorGroup LeftMG({-1, -3, -5}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
	pros::MotorGroup RightMG({2, 4, 6}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
	pros::Motor HoodMotor(7,pros::v5::MotorGears::blue,pros::v5::MotorEncoderUnits::degrees);
	pros::Motor IntakeMotor(8,pros::v5::MotorGear::blue, pros::v5::MotorUnits::degrees);
	pros::adi::Pneumatics WingsPiston('c',false); // starts retracted
	pros::adi::Pneumatics MiddleScorePiston('b',true); // starts extended
	pros::adi::Pneumatics MatchLoaderPiston('a',false);

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
		if (Player2){
			F = SideCont.get_analog(ANALOG_LEFT_Y);
			T = SideCont.get_analog(ANALOG_RIGHT_X);
		}else{
			F = MainCont.get_analog(ANALOG_LEFT_Y);
			T = MainCont.get_analog(ANALOG_RIGHT_X);
		}

		if ((MainCont.get_digital(PlayerSwapA)) && (SideCont.get_digital(PlayerSwapA)))
		{
			Toggle2ndPlayer();
		}
		left = F + T;
		right = F - T;

		LeftMG.move((100*(((1-curve)*left)/100+(curve*pow(left/100,mod)))));
		RightMG.move((100*(((1-curve)*right)/100+(curve*pow(right/100,mod)))));

		HoodMotor.move_velocity((MainCont.get_digital(HoodButton) - MainCont.get_digital(InverseHoodButton) + MainCont.get_digital(ScoreButton) - MainCont.get_digital(InverseScoreButton) - Player2) + (SideCont.get_digital(HoodButton) - SideCont.get_digital(InverseHoodButton) + SideCont.get_digital(ScoreButton) - SideCont.get_digital(InverseScoreButton) + Player2) * 600);


		/*			Utility Motor Functions			*/
		HoodMotor.move_velocity(((MainCont.get_digital(HoodButton) + MainCont.get_digital(ScoreButton) + SideCont.get_digital(HoodButton) + SideCont.get_digital(ScoreButton)) - (MainCont.get_digital(InverseHoodButton) + MainCont.get_digital(InverseScoreButton) + SideCont.get_digital(InverseHoodButton) + SideCont.get_digital(InverseScoreButton))) * 600);
		//slightly long
		//handles both controllers Movements involving these two motors
		IntakeMotor.move_velocity(((MainCont.get_digital(IntakeButton) + MainCont.get_digital(ScoreButton) + SideCont.get_digital(IntakeButton) + SideCont.get_digital(ScoreButton)) - (MainCont.get_digital(InverseIntakeButton) + MainCont.get_digital(InverseScoreButton) + SideCont.get_digital(InverseIntakeButton) + SideCont.get_digital(InverseScoreButton))) * 600);;
		
		/*			Piston Functions		*/
		if (MainCont.get_digital(WingsButton) || SideCont.get_digital(WingsButton)){WingsPiston.toggle(); pros::delay(150);}
		if (MainCont.get_digital(MiddleScoreButton) || SideCont.get_digital(MiddleScoreButton)){MiddleScorePiston.toggle(); pros::delay(150);}
		if (MainCont.get_digital(MatchLoaderButton) || SideCont.get_digital(MatchLoaderButton)){MatchLoaderPiston.toggle(); pros::delay(150);}


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

		pros::delay(20);
	}
}