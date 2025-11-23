#include "main.h"


extern constexpr double diameter = 3.25;
extern constexpr double radius = diameter/2;
extern constexpr double TrackLength = 16;
extern constexpr double StraightTolerance = 10;
extern constexpr double RotationTolerance = 5;
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

extern std::atomic<bool> UtilityMode = false;
//true = intake, false = railgun
extern std::atomic<int> ChargeAmount = 0; // how many balls will the railgun shoot

extern std::atomic<bool> RGReady = false;// true if the RailGun is Ready to fire at the set amount

extern double InchesToDegrees(float distance){return distance/((Pi * diameter)/360);}

extern double RotationToDegrees(double degrees){return (((degrees * (Pi / 180))*(TrackLength/2))/diameter*360)/2;}


extern double DegToRad(double x) {
    return (x * (Pi/180));
}
extern double RadToDeg(double x) {
    return (x* (180/Pi));
}

extern int sgn(double x){return (x>0) - (x<0);}

