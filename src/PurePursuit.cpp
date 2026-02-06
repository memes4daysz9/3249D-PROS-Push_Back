#include "main.h"


//Here lies Ben and Josh, as both of our souls could not take the pain that this single file upholds

//arrays with 2 elements (1), will be treated as X, then Y becuase we are normal human beings with a (semi) functional brain

double Sol1[1];
double Sol2[1];

const double LookAheadDistance = 10;

const int8_t PathLimit = 255;

double Paths[255][1];

double BestPoint[1]; 



bool CheckForIntersection(double p1[1] , double p2[1])
{
    double op1[1];
    double op2[1];// With Offset (idk what that means yet)

    double BotX = X.load();
    double BotY = Y.load();
    
    op1[0] = p1[0] - BotX;
    op1[1] = p1[1] - BotY;

    op2[0] = p2[0] - BotX;
    op2[1] = p2[1] - BotY;
    
    bool Colliding;
    double ColXm, ColXp, ColYm, ColYp;
    double Discriminent;
    double MinPoint[1], MaxPoint[1];

    MinPoint[0] = std::min(op1[0],op2[0]);
    MinPoint[1] = std::min(op1[1],op2[1]);

    MaxPoint[0] = std::max(op1[0],op2[0]);
    MaxPoint[0] = std::max(op1[1],op2[1]);

    double Dx = op2[0] - op1[0], Dy = op2[1] - op1[1], Dr = sqrt(pow(Dx,2) + pow(Dy,2));
    double D = (op1[0] * op2[1]) - (op2[0] * op1[1]);

    Sol1[0] = (D * Dy - sgn(Dy) * Dx * sqrt(pow(LookAheadDistance,2) * pow(Dr,2) - pow(D,2))) / pow(Dr,2);    
    Sol2[0] = (D * Dy + sgn(Dy) * Dx * sqrt(pow(LookAheadDistance,2) * pow(Dr,2) - pow(D,2))) / pow(Dr,2);

    Sol1[1] = (-D * Dx - abs(Dy) * sqrt(pow(LookAheadDistance,2) * pow(Dr,2) - pow(D,2))) / pow(Dr,2);
    Sol1[1] = (-D * Dx + abs(Dy) * sqrt(pow(LookAheadDistance,2) * pow(Dr,2) - pow(D,2))) / pow(Dr,2);
    
    Discriminent = pow(LookAheadDistance,2) * pow(Dr,2) - pow(D,2);
    
    if (Discriminent > 0 || Discriminent == 0)
    {
        Colliding = true;
    }else
    {
        Colliding = false;
        return false;
    }
    if (/*x*/ ((MinPoint[0] <= Sol1[0] <= MaxPoint[0]) || (MinPoint[0] <= Sol2[0] <= MaxPoint[0])) && /*y*/ ((MinPoint[1] <= Sol1[1] <= MaxPoint[1]) || (MinPoint[1] <= Sol2[1] <= MaxPoint[1])))
    {
        return true;
    }else
    {
        return false;
    }
    
}


bool GetBestPoint()
{
    double Distance[1], x[1], y[1];
    double p1[1], p2[1];

    for (int i = PathLimit; i > 0; i--)
    {
        x[0] = Paths[i+1][0];
        x[1] = Paths[i][0];
        y[0] = Paths[i+1][1];
        y[1] = Paths[i][1];

        p1[0] = x[0];
        p1[1] = y[0];
        p2[0] = x[1];
        p2[1] = y[1];

        if (CheckForIntersection(p1,p2))
        { // has intersected, checking best point (out of two)
            Distance[0] = x[1] - Sol1[0];
            if (abs(Sol1[0] - x[1]) < abs(Sol2[0] - x[1]) || abs(Sol1[1] - x[1]) < abs(Sol2[1] - x[1]))
            {
                Distance[1] = Sol1[1];
                Distance[0] = Sol1[0];
                BestPoint[0] = Distance[0];
                BestPoint[1] = Distance[1];
            }else
            {
                Distance[1] = Sol2[1];
                Distance[0] = Sol2[0];
                BestPoint[0] = Distance[0];
                BestPoint[1] = Distance[1];
            }
        }
    }
}