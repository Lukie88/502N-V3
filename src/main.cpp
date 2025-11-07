#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/adi.hpp"

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
  pistonA.retract();
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

void intakefunc(int speedmain,int speedhalf1,int speedhalf2) {
  intakeMain.move_velocity(speedmain);
  intakeHalf1.move_velocity(speedhalf1);
  intakeHalf2.move_velocity(speedhalf2);
}
void opcontrol() {


	while (true) {

  int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
      // move the robot
  chassis.arcade(leftY, rightX);

  // Intake mappings
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      // R1 → Intake (No Scoring)
      // 11W: CCW | Body 5.5W: CW | Scoring 5.5W: Coast
      intakefunc(-600,-600,0);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      // R2 → Intake (Top Scoring)
      // 11W: CCW | Body 5.5W: CW | Scoring 5.5W: CCW
      intakefunc(600,-600,600);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      // L1 → Intake (Middle Scoring)
      // 11W: CCW | Body 5.5W: CW | Scoring 5.5W: CW
      intakefunc(-600,-600,-600);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      // L2 → Outtake
      // 11W: CW | Body 5.5W: CCW | Scoring 5.5W: CCW
      intakefunc(-600,600,-600);
    } else {
      // No intake buttons — stop all three
      intakefunc(0,0,0);
    }

// A button — momentary pulse on Piston C (match load)
if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
 pistonA.toggle();}
}
}