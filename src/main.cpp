#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/adi.hpp"

// controler
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
// Ensure positive power drives the robot forward on BOTH sides.
// Left side: ports 1 and 3 forward, port 2 reversed
pros::MotorGroup left_mg({1, -2, 3}, pros::MotorGearset::blue);
// Right side: mirror orientation â€” reverse the whole side unless a motor is physically flipped.
// Here we reverse ports 10 and 5, and keep 6 forward (adjust per your physical build if needed).
pros::MotorGroup right_mg({-10, -5, 6}, pros::MotorGearset::blue);

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_mg, // left motor group
                              &right_mg, // right motor group
                              10.5, // 10.5 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              2 // horizontal drift is 2 (for now)
);

// --- Intake motors (renamed by role in comments) ---
// 11W main intake
pros::Motor intakeMain(8, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
// Body 5.5W (formerly half intake 1)
pros::Motor intakeHalf1(-7, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
// Scoring 5.5W (formerly half intake 2)
pros::Motor intakeHalf2(-9, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);

// --- Keep ONLY the A-button mechanism (downward actuator) ---
pros::adi::Pneumatics pistonC('A', false); // match-load / downward mechanism



inline void pulseMatchLoad(int ms = 200) {
    pistonC.extend();
    pros::delay(ms);
    pistonC.retract();
}
// Inertials
pros::Imu imu_sensor(15);
// horizontal tracking wheel rotational sensor
pros::Rotation horizontal_sensor(-13);
// vertical tracking wheel rotational sensor
pros::Rotation vertical_sensor(-16);
// horizontal tracking wheel
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_sensor, lemlib::Omniwheel::NEW_275_HALF, 3);
// vertical tracking wheel
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_sensor, lemlib::Omniwheel::NEW_275_HALF,0);

//Odometries
lemlib::OdomSensors sensors(&vertical_tracking_wheel, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu_sensor // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(1, // proportional gain (kP)
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
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize(); // initialize brain screen
  chassis.calibrate(); // calibrate sensors

  pros::delay(20); // update every 20 ms
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
 // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);


}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

// Logical direction helpers (flip signs here if any motor spins opposite in real life)
	constexpr int DIR_11W_CCW     = +1;
	constexpr int DIR_11W_CW      = -1;
	constexpr int DIR_BODY_CCW    = +1;
	constexpr int DIR_BODY_CW     = -1;
	constexpr int DIR_SCORE_CW    = +1;
	constexpr int DIR_SCORE_CCW   = -1;

	while (true) {

  int leftY = -controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  int rightX = -controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
      // move the robot
  chassis.arcade(leftY, rightX);

  // Intake mappings
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      // R1 → Intake (No Scoring)
      // 11W: CCW | Body 5.5W: CW | Scoring 5.5W: Coast
      intakeMain.move(127 * DIR_11W_CCW);
      intakeHalf1.move(127 * DIR_BODY_CW);
      intakeHalf2.move(0);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      // R2 → Intake (Top Scoring)
      // 11W: CCW | Body 5.5W: CW | Scoring 5.5W: CCW
      intakeMain.move(127 * DIR_11W_CCW);
      intakeHalf1.move(127 * DIR_BODY_CW);
      intakeHalf2.move(127 * DIR_SCORE_CCW);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      // L1 → Intake (Middle Scoring)
      // 11W: CCW | Body 5.5W: CW | Scoring 5.5W: CW
      intakeMain.move(127 * DIR_11W_CCW);
      intakeHalf1.move(127 * DIR_BODY_CW);
      intakeHalf2.move(127 * DIR_SCORE_CW);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      // L2 → Outtake
      // 11W: CW | Body 5.5W: CCW | Scoring 5.5W: CCW
      intakeMain.move(127 * DIR_11W_CW);
      intakeHalf1.move(127 * DIR_BODY_CCW);
      intakeHalf2.move(127 * DIR_SCORE_CCW);
    } else {
      // No intake buttons — stop all three
      intakeMain.move(0);
      intakeHalf1.move(0);
      intakeHalf2.move(0);
    }

// A button — momentary pulse on Piston C (match load)
if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
  pulseMatchLoad(); // extends, waits ms, retracts
}

}


}