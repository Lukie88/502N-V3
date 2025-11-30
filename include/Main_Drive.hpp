#pragma once
#include "main.h"

struct DriveOutput {
    float left;
    float right;
};

// Compute left/right motor power using the "curvatherp" blend:
//  - Blends curvature drive and tank drive based on forward magnitude
//  - Inputs are joystick values in [-127, 127]
DriveOutput calc_curvatherp(int throttle, int turn);
//arcade pure

DriveOutput calc_arcade(int throttle, int turn);
// Drive a straight-line distance (in inches) using only the drivetrain motor encoders.
void drive_distance_inches(double inches, int speed = 100);
