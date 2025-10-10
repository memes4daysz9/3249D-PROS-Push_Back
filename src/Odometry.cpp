#include "main.h"


const double TrackerRadius  = 1.375;//since its a tracking wheel, its different, trust

const double SL = 7.5;// "the left-right distance from the tracking center to the left tracking wheel"
const double SR = 7.5;// "the left-right distance from the tracking center to the right tracking wheel"
const double SS = 2;// "the forward-backward distance from the tracking center to the back tracking wheel"

extern void Odometry(){

    double DeltaLeft;
    double DeltaRight;
    double RDeg;
    double LDeg; 
    double RDis;
    double LDis;
    double RHeading = 0;
    double LastL = 0;
    double LastR = 0;
    double LastHeading = 0;
    double DeltaHeading = 0;
    double LocalOffset[2]; // delta variants of x,y
    double DistanceOffset;
    double AvgHeading;
    constexpr double GearRatio = (4/5);
    

    double ArcCenter;
    uint64_t Time;
    uint64_t LastTime;

    pros::Motor LeftEnc(1,pros::v5::MotorGears::blue,pros::v5::MotorEncoderUnits::degrees);
    pros::Motor RightEnc(1,pros::v5::MotorGears::blue,pros::v5::MotorEncoderUnits::degrees);

    pros::screen::print(pros::E_TEXT_MEDIUM,2, "Odometry has started, GLHF");

    while (true)
    {
        
        RDeg = RightEnc.get_position() * GearRatio;
        LDeg = LeftEnc.get_position() * GearRatio;                 //local variables
 

        DeltaLeft = LDeg - LastL;
        DeltaRight = RDeg - LastR;                    //Deltas

        LDis = (DegToRad(DeltaLeft) * TrackerRadius); // Lr
        RDis = (DegToRad(DeltaRight) * TrackerRadius);//Rr               Converting to distance

        RHeading = LastHeading + (LDis - RDis) / (SL + SR);//            Getting theta

        DeltaHeading = (RDis - LDis) / TrackLength;
        Heading = RadToDeg(RHeading);//                         Using theta to get values for other things
        
        ArcCenter = (RDis + LDis) / 2;

        if (abs(DeltaHeading) < 1e-5) // straight
        {
            LocalOffset[0] = 0;
            LocalOffset[1] = ArcCenter;
        }
        else
        {
            LocalOffset[0] = ((ArcCenter / DeltaHeading) * (1 - cos(DeltaHeading)));
            LocalOffset[1] = ((ArcCenter / DeltaHeading) * sin(DeltaHeading));
        }

        AvgHeading = (LastHeading + DeltaHeading) / 2;


        X += ((LocalOffset[0] * cos(AvgHeading)) - (LocalOffset[1] * sin(AvgHeading)));
        Y += ((LocalOffset[0] * sin(AvgHeading)) + (LocalOffset[1] * cos(AvgHeading)));//                  Applying them
        
        pros::delay(5);//forces the driver task to run, otherwise Jackson won't be able to drive :(

        double LastL = LDeg;
        double LastR = RDeg;
        double LastHeading = DeltaHeading;


        
    }

}