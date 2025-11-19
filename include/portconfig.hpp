#include "api.h"
#include "lemlib/api.hpp"
#include "pros/optical.hpp"



//controllers
inline pros::Controller controller(pros::E_CONTROLLER_MASTER);

//motors
inline pros::MotorGroup left_mg({1, -5, -3}, pros::MotorGearset::blue);
inline pros::MotorGroup right_mg({-19, 18, 17}, pros::MotorGearset::blue);

inline lemlib::Drivetrain drivetrain(&left_mg, // left motor group
                              &right_mg, // right motor group
                              11.4, // 10.5 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              2 // horizontal drift is 2 (for now)
);

// --- Intake motors (renamed by role in comments) ---
// 11W main intake
inline pros::Motor intakeMain(8, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
// Body 5.5W (formerly half intake 1)
inline pros::Motor intakescore(-7, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
// Scoring 5.5W (formerly half intake 2)
inline pros::Motor intakemid(-9, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);



// --- Keep ONLY the A-button mechanism (downward actuator) ---
inline pros::adi::Pneumatics pistonload('A', false); // match-load / downward mechanism
// Wing Mechanism- Button L2
inline pros::adi::Pneumatics pistonWing('B', false); // match-load / downward mechanism

// Optical Sensors
inline pros::Optical sorterOptical(12);

// Inertial Sensor
inline pros::Imu imu_sensor(15);
// horizontal tracking wheel rotational sensor
inline pros::Rotation horizontal_sensor(-13);
// vertical tracking wheel rotational sensor
inline pros::Rotation vertical_sensor(-16);

// horizontal tracking wheel
inline lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_sensor, lemlib::Omniwheel::NEW_275_HALF, 2.85);
// vertical tracking wheel
inline lemlib::TrackingWheel vertical_tracking_wheel(&vertical_sensor, lemlib::Omniwheel::NEW_275_HALF,0.591);

//Odometries
inline lemlib::OdomSensors sensors(&vertical_tracking_wheel, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu_sensor // inertial sensor
);

// lateral PID controller
inline lemlib::ControllerSettings lateral_controller(5.3, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              4.8, // derivative gain (kD)
                                              2.5, // anti windup
                                              0.25, // small error range, in inches
                                              250, // small error range timeout, in milliseconds
                                              0.75, // large error range, in inches
                                              1000, // large error range timeout, in milliseconds
                                              8 // maximum acceleration (slew)
);

// angular PID controller
inline lemlib::ControllerSettings angular_controller(1.357, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in degrees
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in degrees
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// create the chassis
inline lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);
