#include "main.h"


double loadXY(){
    return X.load() + Y.load();
}

extern bool Rel_Move(double Distance){
    double pol = sgn(Distance);
    // temp fixing the odom / wheel problem
    Distance = (fabs(Distance) - ((0.175 * fabs(Distance)) - 0.8453)) * pol; // we DO NOT TOUCH THIS
    //theres more magic in these numbers than normal magic numbers
    

	pros::MotorGroup LeftMG({1, -3, -5});
	pros::MotorGroup RightMG({-2, 4, 6});

    
    const double BaseDistance = OdomDistance.load();
    double CurDistance = 0;

    const int MinP = 1700; //temp
    const double kP = 450;//450
    const double kI = 0;//0.1
    const double kD = 300; //300
    double i = 0;
    double Output;

    const double Target = Distance;
    double error = Target - CurDistance;
    const double Dscaler = 0.025; // 25 ms
    double LastError = 0;
    pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error:  %f              ",error);
    pros::screen::print(pros::E_TEXT_MEDIUM,6, "Distance: %f",pol);
    


    while (abs(error) > StraightTolerance) {
        CurDistance = OdomDistance.load() - BaseDistance;
        error = Target - CurDistance;
        pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error:  %f", error);

        i += error;
        double Output = (error * kP) +(i * kI) +((error - LastError) * kD) +(MinP * sgn(error));


        LeftMG.move_voltage(Output);
        RightMG.move_voltage(Output);

        LastError = error;
        pros::delay(25);
    }
    OdomDistance.store(0);// resets value for next loop
    AFinished(sgn(Distance), sgn(Distance));
    return true;
}



extern bool Rel_Rotate(double Degrees){
	pros::MotorGroup LeftMG({1, -3, -5});
	pros::MotorGroup RightMG({-2, 4, 6});
    pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error:              ");

    const double BaseHeading = (Heading.load());
    double CurHeading = 0;// localized value, sets to 0 everytime a new function is called to cercumvent the drift issue

    /*const int MinP = 650; // temp
    const double kP = 190;
    const double kI = 0.35;         Old
    const double kD = 340;*/

    const int MinP = 1600; //temp
    const double kP = 50;//450
    const double kI = 0;//0.1
    const double kD = 400; //300


    double i;
    double Output;

    const double Target = Degrees;
    double error = Target - CurHeading;
    double LastError;

    while (abs(error) > RotationTolerance){
        CurHeading =  BaseHeading - (Heading.load());
        error = Target - CurHeading;
        pros::screen::print(pros::E_TEXT_MEDIUM,4, "Heading: %f ", CurHeading);
        pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error: %f" , error);
        i = i * kI;
        Output = (error * kP) + i + ((error - LastError) * kD) + int(MinP * sgn(error));

        LeftMG.move_voltage(-Output);
        RightMG.move_voltage(Output);
        pros::delay(5);
        LastError = error;
        

    }
    OdomDistance.store(0);
    AFinished(sgn(Degrees) * -1, sgn(Degrees) * 1);
    return true;

}


extern bool Abs_Rotate(double Degrees){
    //from where its starts, not where its on the field, otherwise id need 4 diff autons (also easier)
	pros::MotorGroup LeftMG({1, -3, -5});
	pros::MotorGroup RightMG({-2, 4, 6});
    pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error:              ");

    double CurHeading = Heading.load();

    /*const int MinP = 650; // temp
    const double kP = 190;
    const double kI = 0.35;         Old
    const double kD = 340;*/

    const int MinP = 1600; //temp
    const double kP = 50;//450
    const double kI = 0;//0.1
    const double kD = 400; //300


    double i;
    double Output;

    const double Target = Degrees;
    double error = Target - CurHeading;
    double LastError;

    while (abs(error) > RotationTolerance){
        CurHeading =  Heading.load();
        error = Target - CurHeading;
        pros::screen::print(pros::E_TEXT_MEDIUM,4, "Heading: %f ", CurHeading);
        pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error: %f" , error);
        i = i * kI;
        Output = (error * kP) + i + ((error - LastError) * kD) + int(MinP * sgn(error));

        LeftMG.move_voltage(Output);
        RightMG.move_voltage(-Output);
        pros::delay(5);
        LastError = error;
        

    }
    OdomDistance.store(0);
    AFinished(sgn(Degrees) * -1, sgn(Degrees) * 1);
    return true;


}
extern bool MatchLoad(int time) // time you want to matchload for
{
    pros::MotorGroup LeftMG({1, -3, -5});
	pros::MotorGroup RightMG({-2, 4, 6});
    pros::Motor IntakeMotor(8,pros::v5::MotorGear::blue, pros::v5::MotorUnits::degrees);
    const int power = 3500; // voltage sent to both motors to move
    const int PreDelay = 750;
    LeftMG.move_voltage(power*4);
    RightMG.move_voltage(power*4);

    pros::delay(PreDelay);//get up onto the loader

    LeftMG.move_voltage(power);
    RightMG.move_voltage(power);
    IntakeMotor.move_voltage(12000);
    pros::delay((time * 1000) - PreDelay); //converts into milliseconds from seconds
    LeftMG.move_voltage(0);    
    RightMG.move_voltage(0);
    IntakeMotor.move_voltage(0); 
    return true;   
}

extern bool AFinished(int Left, int Right)
{
    pros::MotorGroup LeftMG({1, -3, -5});
	pros::MotorGroup RightMG({-2, 4, 6});
    LeftMG.set_brake_mode(MOTOR_BRAKE_HOLD);
    RightMG.set_brake_mode(MOTOR_BRAKE_HOLD);
    LeftMG.move_voltage(Left * -1500);
    RightMG.move_voltage(Right * -1500);
    pros::delay(67.5);
    LeftMG.brake();
    RightMG.brake();
    pros::delay(55);
    LeftMG.set_brake_mode(MOTOR_BRAKE_COAST);
    RightMG.set_brake_mode(MOTOR_BRAKE_COAST);
    return true;

}