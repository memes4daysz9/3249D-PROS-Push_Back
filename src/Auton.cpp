#include "main.h"


double loadXY(){
    return X.load() + Y.load();
}

extern bool Rel_Move(double Distance){
    double pol = sgn(Distance);
    // temp fixing the odom / wheel problem
    Distance = (fabs(Distance) - ((0.18 * fabs(Distance)) - 0.9453)) * pol; // we DO NOT TOUCH THIS
    //theres more magic in these numbers than normal magic numbers
    

	pros::MotorGroup LeftMG({-1, -3, -5});
	pros::MotorGroup RightMG({2, 4, 6});

    
    const double BaseDistance = OdomDistance.load();
    double CurDistance = 0;

    const int MinP = 650; //temp
    const double kP = 250;
    const double kI = 0;
    const double KD = 0;
    double i = 0;
    double Output;

    const double Target = Distance;
    double error = Target - CurDistance;
    double LastError = 0;
    pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error:  %f              ",error);
    pros::screen::print(pros::E_TEXT_MEDIUM,4, "Distance: %f",pol);
    


while (abs(error) > StraightTolerance) {
    CurDistance = OdomDistance.load() - BaseDistance;
    error = Target - CurDistance;
    pros::screen::print(pros::E_TEXT_MEDIUM,5, "Eroor:  %f", error);

    i += error;
    double Output =
        (error * kP) +
        (i * kI) +
        ((error - LastError) * kD) +
        (MinP * sgn(error));

    Output = std::clamp(Output, -12000.0, 12000.0);

    LeftMG.move_voltage(Output);
    RightMG.move_voltage(Output);

    LastError = error;
    pros::delay(10);
}
    OdomDistance.store(0);// resets value for next loop
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

    const double BaseHeading = GetDegrees(Heading.load());
    double CurHeading = 0;// localized value, sets to 0 everytime a new function is called to cercumvent the drift issue

    const int MinP = 650; // temp
    const double kP = 25;
    const double kI = 0.01;
    const double KD = 0;
    double i;
    double Output;

    const double Target = RotationToDegrees(Degrees);
    double error = Target - RotationToDegrees(CurHeading);
    double LastError;

    while (abs(error) > RotationToDegrees(RotationTolerance)){
        CurHeading =  BaseHeading - GetDegrees(Heading.load());
        error = Target - RotationToDegrees(-CurHeading);
        pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error: %f" , error);
        i = i * kI;
        Output = (error * kP) + i + ((error - LastError) * kD) + int(MinP * sgn(error));

        LeftMG.move_voltage(Output);
        RightMG.move_voltage(-Output);
        LastError = error;

    }
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
    pros::delay(100);
    LeftMG.set_brake_mode(MOTOR_BRAKE_COAST);
    RightMG.set_brake_mode(MOTOR_BRAKE_COAST);

}