#include "Main_Drive.hpp"

// ----- CONSTANTS (same as your RobotDrive) ----- //
static const float CONTROLLER_THROTTLE_CURVE = 1.2f;
static const float CONTROLLER_THROTTLE_DEADBAND = 3.0f;

static const float CONTROLLER_TURN_CURVE = 1.8f;
static const float CONTROLLER_TURN_DEADBAND = 3.0f;

static const float CURVATHERP_INTERP_START = 2.0f;
static const float CURVATHERP_INTERP_END   = 50.0f;
static const float CURVATHERP_TURN_COEFF   = 1.0f;

// ----- SMALL HELPER FUNCTIONS ----- //

static inline float clip(float x, float lo, float hi) {
    return std::max(lo, std::min(hi, x));
}

static inline int deadband(int value, float band) {
    return (std::abs(value) < band) ? 0 : value;
}

// Nonlinear joystick curve: keeps small inputs gentle, large inputs strong
static float pilonCurve(int input, float maxInput, float exponent) {
    if (input == 0) return 0.0f;
    float normalized = static_cast<float>(input) / maxInput;  // -1..1
    float sign = (normalized >= 0.0f) ? 1.0f : -1.0f;
    float mag  = std::pow(std::fabs(normalized), exponent);
    return sign * mag * maxInput; // back to -max..max
}

// ----- ARCADE MATH (optional) ----- //

DriveOutput calc_ez_arcade(int throttle, int turn) {
    // Deadband then curve inputs
    float curved_throttle = pilonCurve(deadband(throttle, CONTROLLER_THROTTLE_DEADBAND),
                                       127.0f, CONTROLLER_THROTTLE_CURVE);
    float curved_turn     = pilonCurve(deadband(turn, CONTROLLER_TURN_DEADBAND),
                                       127.0f, CONTROLLER_TURN_CURVE);

    // Basic arcade
    float left  = curved_throttle + curved_turn;
    float right = curved_throttle - curved_turn;

    // Normalize to [-127, 127]
    float ratio = std::max(std::fabs(left), std::fabs(right)) / 127.0f;
    if (ratio > 1.0f) {
        left  /= ratio;
        right /= ratio;
    }

    return {left, right};
}

// ----- CURVATHERP MATH ----- //

DriveOutput calc_curvatherp(int throttle, int turn) {
    // Curve inputs
    double forward_stick = pilonCurve(throttle, 127.0f, CONTROLLER_THROTTLE_CURVE);
    double turn_stick    = pilonCurve(turn,     127.0f, CONTROLLER_TURN_CURVE);

    // Compute blend factor based on forward magnitude
    double interpolator =
        (std::fabs(forward_stick) - CURVATHERP_INTERP_START) /
        (CURVATHERP_INTERP_END - CURVATHERP_INTERP_START);
    interpolator = clip(interpolator, 0.0f, 1.0f);

    turn_stick *= CURVATHERP_TURN_COEFF;

    double forward_clamped = forward_stick / 127.0;
    double turn_clamped    = turn_stick    / 127.0;

    // Curvature-style output
    double left_curvature =
        forward_clamped + std::fabs(forward_clamped) * turn_clamped;
    double right_curvature =
        forward_clamped - std::fabs(forward_clamped) * turn_clamped;
    double max_curv = std::max(std::fabs(left_curvature), std::fabs(right_curvature));

    if (max_curv > 1.0) {
        left_curvature  /= max_curv;
        right_curvature /= max_curv;
    }

    // Tank-style output
    double left_tank  = clip(forward_clamped + turn_clamped,  -1.0, 1.0);
    double right_tank = clip(forward_clamped - turn_clamped,  -1.0, 1.0);

    // Blend between curvature and tank
    double leftPower =
        left_curvature  * interpolator + left_tank  * (1.0 - interpolator);
    double rightPower =
        right_curvature * interpolator + right_tank * (1.0 - interpolator);

    return { static_cast<float>(leftPower  * 127.0),
             static_cast<float>(rightPower * 127.0) };
}
