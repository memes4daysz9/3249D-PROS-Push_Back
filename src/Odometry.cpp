#include "main.h"

constexpr double TrackerRadius = 3.25 / 2;
const double SL = 17.5;
const double SR = 17.5;
const double GearRatio = 0.8;


void Odometry() {

    pros::MotorGroup LeftEnc({-1, -3, -5});
    pros::MotorGroup RightEnc({2, 4, 6});
    pros::Imu IMUa(12);
    pros::Imu IMUb(11);

    double LastL = 0;
    double LastR = 0;
    double LastHeading = 0;      // radians
    double IMUDeg;
    double IMURad;
    double LDeg;
    double RDeg;
    double DeltaLeft;
    double DeltaRight;
    double DeltaHeading;
    double LDis;
    double RDis;
    double RHeading;

    pros::screen::print(pros::E_TEXT_MEDIUM, 2, "Odometry started!");

    while (IMUa.is_calibrating() || IMUb.is_calibrating()) {
        pros::delay(10);
    }
    

    while (true) {

        // IMU Logic
        //note, using a switch case like this isnt the best pratice, i just kinda wanted to learn it
        /*switch (abs(IMUa.get_rotation() - IMUb.get_rotation()) > 0.1){ //if the difference is greater than a certain amount
            case true:
                if (IMUa.get_rotation() > IMUb.get_rotation()){ // effectivly disabling the IMU on port 12
                    Heading = IMUb.get_rotation();
                }else{
                    Heading = IMUa.get_rotation();
                }
            case false:
                Heading = (IMUa.get_rotation() + IMUb.get_rotation()) / 2;
        }*/
       Heading = IMUa.get_rotation();
       
        
        
        IMURad = DegToRad(Heading);

        LDeg = LeftEnc.get_position() * GearRatio;
        RDeg = RightEnc.get_position() * GearRatio;
        DeltaLeft  = LDeg - LastL;
        DeltaRight = RDeg - LastR;
        LDis = DegToRad(DeltaLeft)  * TrackerRadius;
        RDis = DegToRad(DeltaRight) * TrackerRadius;

        pros::screen::print(pros::E_TEXT_MEDIUM, 7,"RDeg: %f   LDeg: %f", RDeg, LDeg);


        //DeltaHeading = IMURad - LastHeading;

        //RHeading = IMURad;

        double ArcCenter = (RDis + LDis) / 2.0;
        OdomDistance.store(OdomDistance.load() + ArcCenter);
        /*double LocalOffsetX, LocalOffsetY;

        if (fabs(DeltaHeading) < 1e-6) {
            // Straight
            LocalOffsetX = 0;
            LocalOffsetY = ArcCenter;
        } else {
            LocalOffsetX = (ArcCenter / DeltaHeading) * (1 - cos(DeltaHeading));
            LocalOffsetY = (ArcCenter / DeltaHeading) * sin(DeltaHeading);
        }

        double AvgHeading = LastHeading + DeltaHeading * 0.5;

        X += LocalOffsetX * cos(AvgHeading) - LocalOffsetY * sin(AvgHeading);
        Y += LocalOffsetX * sin(AvgHeading) + LocalOffsetY * cos(AvgHeading);
        */
        LastL = LDeg;
        LastR = RDeg;
        //LastHeading
        /*LastHeading = RHeading;*/

        pros::delay(5);
    }
}
