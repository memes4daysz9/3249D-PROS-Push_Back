#include "main.h"


double loadXY(){
    return X.load() + Y.load();
}

extern bool Rel_Move(double Distance){
    double pol = sgn(Distance);
    // temp fixing the odom / wheel problem
    Distance = (fabs(Distance) - ((0.175 * fabs(Distance)) - 0.8453)) * pol; // we DO NOT TOUCH THIS
    //theres more magic in these numbers than normal magic numbers
    

	pros::MotorGroup LeftMG({-1, -3, -5});
	pros::MotorGroup RightMG({2, 4, 6});

    
    const double BaseDistance = OdomDistance.load();
    double CurDistance = 0;

    const int MinP = 1100; //temp
    const double kP = 400;//450
    const double kI = 0.2;//0.1
    const double kD = 300;
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
    Finished();
    return true;
}

extern double GetDegrees(double H)
{
    if (H > 180){
        return H - 360;
    }else{
        return H;
    }
}

extern bool Rel_Rotate(double Degrees){
	pros::MotorGroup LeftMG({-1, -3, -5});
	pros::MotorGroup RightMG({2, 4, 6});
    pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error:              ");

    const double BaseHeading = (Heading.load());
    double CurHeading = 0;// localized value, sets to 0 everytime a new function is called to cercumvent the drift issue

    const int MinP = 650; // temp
    const double kP = 160;
    const double kI = 0.25;
    const double kD = 350;
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
    Finished();
    return true;

}
extern void Finished()
{
    pros::MotorGroup LeftMG({-1, -3, -5});
	pros::MotorGroup RightMG({2, 4, 6});
    LeftMG.set_brake_mode(MOTOR_BRAKE_HOLD);
    RightMG.set_brake_mode(MOTOR_BRAKE_HOLD);
    LeftMG.move_velocity(0);
    RightMG.move_velocity(0);
    pros::delay(2500);
    LeftMG.set_brake_mode(MOTOR_BRAKE_COAST);
    RightMG.set_brake_mode(MOTOR_BRAKE_COAST);

}