#include "main.h"


extern constexpr double diameter = 3.25;
extern constexpr double radius = diameter/2;
extern constexpr double TrackLength = 16;
extern constexpr double StraightTolerance = 0.05;
extern constexpr double RotationTolerance = 0.5;
extern const double Pi = 3.141592653;
//extern const double Pi = 3.14;
extern std::atomic<double> X = 0;
extern std::atomic<double> Y = 0;       //atomics keep things thread safe, as these numbers go through 2 threads
extern std::atomic<double> Heading = 0;
extern std::atomic<double> OdomDistance = 0; // for Straight auton movement
extern uint64_t DeltaTime = 0;
extern uint64_t OdomCount = 0;

extern std::atomic<bool> UtilityMode = false;
//true = intake, false = railgun
extern std::atomic<int> ChargeAmount = 0; // how many balls will the railgun shoot

extern std::atomic<bool> RGReady = false;// true if the RailGun is Ready to fire at the set amount

extern double InchesToDegrees(float distance){return (distance / ((Pi * diameter) * 0.8)) * 360;}

extern double RotationToDegrees(double degrees){return (((degrees * (Pi / 180))*(TrackLength/2))/diameter*360)/2;}

extern double DegToRad(double x) {
    return (x * (Pi/180));
}
extern double RadToDeg(double x) {
    return (x* (180/Pi));
}

extern int sgn(double x){return (x>0) - (x<0);}



extern pros::controller_digital_e_t IntakeButton = pros::E_CONTROLLER_DIGITAL_R1;// right paddle
extern pros::controller_digital_e_t InverseIntakeButton = pros::E_CONTROLLER_DIGITAL_R2; //left paddle
extern pros::controller_digital_e_t HoodButton = pros::E_CONTROLLER_DIGITAL_L1;
extern pros::controller_digital_e_t InverseHoodButton = pros::E_CONTROLLER_DIGITAL_L2;
extern pros::controller_digital_e_t ScoreButton = pros::E_CONTROLLER_DIGITAL_Y; // right paddle
extern pros::controller_digital_e_t InverseScoreButton = pros::E_CONTROLLER_DIGITAL_RIGHT;//left paddle
//toggleables
extern pros::controller_digital_e_t WingsButton = pros::E_CONTROLLER_DIGITAL_DOWN;//left bottom thingy
extern pros::controller_digital_e_t MiddleScoreButton = pros::E_CONTROLLER_DIGITAL_A;
extern pros::controller_digital_e_t MatchLoaderButton = pros::E_CONTROLLER_DIGITAL_B; // right bottom thingy


