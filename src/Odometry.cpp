#include "main.h"

//keeping old one bc i dont trust 2AM me

constexpr double TrackerRadius = 3.25 / 2;
const double LRDis = 17.5;
constexpr double GearRatio = 480.0 / 600.0;

void Odometry() {

    pros::Motor LeftEnc(1);
    pros::Motor RightEnc(-2);
    pros::Imu IMUa(12);
    pros::Imu IMUb(11);

    double LastL = 0;
    double LastR = 0;
    double LastHeading = 0;
    double IMUDeg;
    double IMURad;
    double LDeg;
    double RDeg;
    double DeltaLeft;
    double DeltaRight;
    double DeltaHeading;
    double LDis;
    double RDis;
    double ArcCenter;
    double LocalOffsetX;
    double LocalOffsetY;
    double AvgHeading;
    X.store(0);
    Y.store(0);

    pros::screen::print(pros::E_TEXT_MEDIUM, 2, "Odometry started!");

    while (IMUa.is_calibrating() || IMUb.is_calibrating()) {
        pros::delay(10);
    }

    while (true) {

        IMUDeg = IMUa.get_rotation();
        Heading.store(IMUDeg);
        IMURad = DegToRad(IMUDeg);

        LDeg = LeftEnc.get_position() * GearRatio;
        RDeg = RightEnc.get_position() * GearRatio;
        DeltaLeft = LDeg - LastL;
        DeltaRight = RDeg - LastR;
        LDis = DegToRad(DeltaLeft) * TrackerRadius;
        RDis = DegToRad(DeltaRight) * TrackerRadius;

        DeltaHeading = IMURad - LastHeading;

        ArcCenter = (RDis + LDis) / 2.0;
        OdomDistance.store(OdomDistance.load() + ArcCenter);

        if (fabs(DeltaHeading) < 1e-6) {
            LocalOffsetX = 0;
            LocalOffsetY = ArcCenter;
        } else {
            LocalOffsetX = (ArcCenter / DeltaHeading) * (1.0 - cos(DeltaHeading));
            LocalOffsetY = (ArcCenter / DeltaHeading) * sin(DeltaHeading);
        }

        AvgHeading = LastHeading + DeltaHeading * 0.5;

        X.store(X.load() + LocalOffsetX * cos(AvgHeading) - LocalOffsetY * sin(AvgHeading));
        Y.store(Y.load() + LocalOffsetX * sin(AvgHeading) + LocalOffsetY * cos(AvgHeading));

        LastL = LDeg;
        LastR = RDeg;
        LastHeading = IMURad;

        pros::screen::print(pros::E_TEXT_MEDIUM,3, "X: %f, Y: %f, Heading: %f" , X.load(), Y.load(), Heading.load());
        
        pros::delay(8);
    }
}