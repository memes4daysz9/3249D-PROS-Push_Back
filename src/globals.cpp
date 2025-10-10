#include "main.h"


extern constexpr double diameter = 3.25;
extern constexpr double radius = diameter/2;
extern constexpr double TrackLength = 16;
extern constexpr double Tolerance = 10;
extern double kP = 10;
extern double kI = 0.7;
extern double kD = 3;
extern bool AbortAuton = true;
extern const double Pi = 3.141592653;
extern std::atomic<double> X = 0;
extern std::atomic<double> Y = 0;       //atomics keep things thread safe, as these numbers go through 2 threads
extern std::atomic<double> Heading = 0;
extern uint64_t DeltaTime = 0;
extern uint64_t OdomCount = 0;
extern const int IntakeTrackLength = 0; // unkown YET. im too lazy to do calculations. The length from the beginning of bot to the end using the cable, not the tread length
extern float RotationsDoneIntake = 0; // for auto PTO
extern bool IntakeOverride = false;

extern double InchesToDegrees(float distance){return distance/((Pi * diameter)/360);}

extern double RotationToDegrees(double degrees){return (((degrees * (Pi / 180))*(TrackLength/2))/diameter*360)/2;}


extern double DegToRad(double x) {
    return (x * (Pi/180));
}
extern double RadToDeg(double x) {
    return (x* (180/Pi));
}

extern int sgn(double x){return (x>0) - (x<0);}