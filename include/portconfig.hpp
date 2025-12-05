#include "api.h"
#include "lemlib/api.hpp"
#include "pros/optical.hpp"
#pragma once
#include "distance_relocalization.hpp"



//controllers
inline pros::Controller controller(pros::E_CONTROLLER_MASTER);

//motors
inline pros::MotorGroup left_mg({8, -6, -7}, pros::MotorGearset::blue); 
inline pros::MotorGroup right_mg({-19, 17, 18}, pros::MotorGearset::blue); // 17,18,19 rev

inline lemlib::Drivetrain drivetrain(&left_mg, // left motor group
                              &right_mg, // right motor group
                              11.375, // 11.375 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              2 // horizontal drift is 2 (for now)
);

// 11W main intake
inline pros::Motor intakeMain(9, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
// Body 5.5W (formerly half intake 1)
inline pros::Motor intakescore(-4, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
// Scoring 5.5W (formerly half intake 2)
inline pros::Motor intakemid(-10, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);



// --- Keep ONLY the A-button mechanism (downward actuator) ---
inline pros::adi::Pneumatics pistonload('C', false); // match-load / downward mechanism
// Wing Mechanism- Button L2
inline pros::adi::Pneumatics pistonWing('D', false); // wing / downward mechanism
// Piston odom
inline pros::adi::Pneumatics pistonodom('B', false); // match-load / downward mechanism

// Optical Sensors
inline pros::Optical sorterOptical(12);

// Inertial Sensor
inline pros::Imu imu_sensor(15);
// horizontal tracking wheel rotational sensor
inline pros::Rotation horizontal_sensor(-16);
// vertical tracking wheel rotational sensor
inline pros::Rotation vertical_sensor(-1);

// horizontal tracking wheel
inline lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_sensor, lemlib::Omniwheel::NEW_275_HALF, 2.85);
// vertical tracking wheel
inline lemlib::TrackingWheel vertical_tracking_wheel(&vertical_sensor, lemlib::Omniwheel::NEW_275_HALF,0.6125);

//Odometries
inline lemlib::OdomSensors sensors(&vertical_tracking_wheel, // Primary vertical tracking wheel 
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are not using it
                            &horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu_sensor // inertial sensor
);

// lateral PID controller
inline lemlib::ControllerSettings lateral_controller(5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              4.8, // derivative gain (kD)
                                              2.5, // anti windup
                                              0.25, // small error range, in inches
                                              250, // small error range timeout, in milliseconds
                                              0.5, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              10 // maximum acceleration (slew)
);

inline lemlib::ControllerSettings angular_controller(
    1.74,    // kP  (down from 2.0 to tame the first lunge).
    0.0015,  // kI  (keep – it’s doing the steady-state cleanup)
    10.0,    // kD  (up from 6.0 to add more braking near the target)
    4,      // anti-windup
    1.0,    // small error range (deg)
    200,    // small error timeout (ms)
    3,    // large error range (deg)
    500,    // large error timeout (ms)
    0      // slew (limits how fast output can change; was 0)
);


// create the chassis
inline lemlib::Chassis chassis(drivetrain,                           // drivetrain settings
                        lateral_controller,     // lateral PID settings
                        angular_controller,             // angular PID settings
                        sensors                                      // odometry sensors
);


// Offsets are measured in robot frame: +X forward, +Y to the left.
inline constexpr relocalize::SensorOffset DIST_BACK_OFFSET{-6.0, 0.0};  // behind center
inline constexpr relocalize::SensorOffset DIST_LEFT_OFFSET{0.0, 6.0};   // left of center
inline constexpr relocalize::SensorOffset DIST_RIGHT_OFFSET{0.0, -6.0}; // right of center

inline pros::Distance distBack(1);
inline pros::Distance distLeft(2);
inline pros::Distance distRight(3);

// Customize which walls each sensor is expected to see for relocalization. someting is a bit off here
// Example: back sensor points toward bottom wall (Y_NEG), left toward left wall (X_NEG), right toward right wall (X_POS).
inline relocalize::DistanceSensorConfig distanceSensorConfigs[] = {
    {&distBack, DIST_BACK_OFFSET, -1.0, 0.0, relocalize::WallAxis::Y_NEG},
    {&distLeft, DIST_LEFT_OFFSET, 0.0, -1.0, relocalize::WallAxis::X_NEG},
    {&distRight, DIST_RIGHT_OFFSET, 0.0, 1.0, relocalize::WallAxis::X_POS},
};

inline constexpr int DIST_SENSOR_COUNT = sizeof(distanceSensorConfigs) / sizeof(distanceSensorConfigs[0]);