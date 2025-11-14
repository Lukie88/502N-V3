#pragma once

#include <cmath>
#include <algorithm>


struct DriveOutput {
    float left;
    float right;
};

// Compute left/right motor power using the "curvatherp" blend:
//  - Blends curvature drive and tank drive based on forward magnitude
//  - Inputs are joystick values in [-127, 127]
DriveOutput calc_curvatherp(int throttle, int turn);

//arcade pure
DriveOutput calc_ez_arcade(int throttle, int turn);
