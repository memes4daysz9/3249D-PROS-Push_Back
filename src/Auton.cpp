#include "main.h"


double loadXY(){
    return X.load() + Y.load();
}

extern bool Rel_Move(double Distance){
	pros::MotorGroup LeftMG({-1, -3, -5});
	pros::MotorGroup RightMG({2, 4, 6});

    pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error:              ");
    
    const double BaseDistance = loadXY();
    double CurDistance = 0;

    const int MinP = 650; //temp
    const double kP = 10;
    const double kI = 0;
    const double KD = 0;
    double i;
    double Output;

    const double Target = InchesToDegrees(Distance);
    double error = Target - InchesToDegrees(CurDistance);
    double LastError = 0;

    while (abs(error) > InchesToDegrees(StraightTolerance)){
        CurDistance = loadXY() - BaseDistance;
        error = Target - InchesToDegrees(CurDistance);
        pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error: %f" , error);
        i = (i + error) * kI;

        Output = (error * kP) + i + ((error - LastError) * kD) + int(MinP * sgn(error));

        LeftMG.move_voltage(Output);
        RightMG.move_voltage(Output);
        LastError = error;
    }
    return true;
}

double GetDegrees(double H)
{
    
}

extern bool Rel_Rotate(double Degrees){
	pros::MotorGroup LeftMG({-1, -3, -5});
	pros::MotorGroup RightMG({2, 4, 6});
    pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error:              ");

    const double BaseHeading = Heading.load();
    double CurHeading = 0;// localized value, sets to 0 everytime a new function is called to cercumvent the drift issue

    const int MinP = 650; // temp
    const double kP = 10;
    const double kI = 0;
    const double KD = 0;
    double i;
    double Output;

    const double Target = RotationToDegrees(Degrees);
    double error = Target - RotationToDegrees(CurHeading);
    double LastError;

    while (abs(error) > RotationToDegrees(RotationTolerance)){
        CurHeading =  BaseHeading - Heading.load();
        error = Target - RotationToDegrees(CurHeading);
        pros::screen::print(pros::E_TEXT_MEDIUM,5, "Error: %f" , error);
        i = i * kI;
        Output = (error * kP) + i + ((error - LastError) * kD) + int(MinP * sgn(error));

        LeftMG.move_voltage(Output);
        RightMG.move_voltage(-Output);
        LastError = error;

    }
    return true;

}