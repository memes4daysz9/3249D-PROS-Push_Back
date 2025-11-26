#include "main.h"

#pragma once



extern void Odometry();

extern double InchesToDegrees(float distance);
extern double RotationToDegrees(double degrees);
extern double DegToRad(double x);
extern double RadToDeg(double x);
extern int sgn(double x);
extern const double diameter;
extern const double TrackLength;
extern const double StraightTolerance;
extern const double RotationTolerance;
extern const double radius;
extern double kP;
extern double kI;
extern double kD;

extern std::atomic<double> X;
extern std::atomic<double> Y;       //atomics keep things thread safe, as these numbers go through 2 threads
extern std::atomic<double> Heading;


template<typename... Args>
extern void WriteToFile(Args&&... args);
extern void CreateFile();
extern void Odometry();
extern uint64_t DeltaTime;
extern uint64_t OdomCount;
extern float RotationsDoneIntake;
extern std::atomic<bool> UtilityMode;//true = intake, false = railgun
extern std::atomic<bool> RGReady;// true if the RailGun is Ready to fire at the set amount

extern std::atomic<int> ChargeAmount; // how many balls will the railgun shoot

#define UtilityMotorA 7
#define UtilityMotorB 8


extern bool Rel_Rotate(double Degrees);
extern bool Rel_Move(double Distance);