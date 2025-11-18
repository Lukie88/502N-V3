#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/adi.hpp"
#include "portconfig.hpp"
#include "auton_select.hpp"
#include "auton_routines.hpp"
#include "Main_Drive.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "robot_afunc.hpp"


void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}
void poseDebugTask(void*) {
    while (true) {
        lemlib::Pose pose = chassis.getPose(); // (x, y, heading)

        pros::lcd::print(0, "X: %.2f in", pose.x);
        pros::lcd::print(1, "Y: %.2f in", pose.y);
        pros::lcd::print(2, "H: %.2f deg", pose.theta);

        pros::delay(50); // update ~20 times/sec
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
  chassis.setPose(0,0,0); // set starting position (x, y, heading)
  controller.rumble(".");
  pros::delay(20); // update every 20 ms
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // a`(controller.get_digital_new_press(DIGITAL_B)){auton_menus();}
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
}

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

    
chassis.setPose(-48,15,90);
runIntakeStore();
chassis.moveToPose(-27, 26, 70,3000,{.maxSpeed=90});
chassis.moveToPose(-4, 45, 45,3000,{.maxSpeed =127});
pros::delay(1500);
setMatchLoad(true);
chassis.moveToPose(-24, 25, 45, 1500,{.maxSpeed=-127 });


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


	while (true) {
    clear_screen();
 int forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
int turn    = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

  // --- Curvatherp drive ---
  DriveOutput driveOut = calc_curvatherp(forward, turn);
  left_mg.move(static_cast<int>(driveOut.left));
  right_mg.move(static_cast<int>(driveOut.right));

  // Intake mappings (priority order matches the requested controller layout)
  if (controller.get_digital(DIGITAL_R1)) {
      // R1 → Intake (Just Storing)
      runIntakeStore();
    } else if (controller.get_digital(DIGITAL_R2)) {
      // R2 → Intake + High Goal Scoring
      scoreHighGoal();
    } else if (controller.get_digital(DIGITAL_L1)) {
      // L1 → Intake + Middle Goal Scoring
      scoreMiddleGoal();
    } else if (controller.get_digital(DIGITAL_Y)) {
      // Y → Outake
      runOuttake();
    } else {
      // No intake buttons — stop all three
      stopIntakes();
    }

// A button — momentary pulse on Piston C (match load)
if (controller.get_digital_new_press(DIGITAL_RIGHT)) {
 pistonload.toggle();}

 if (controller.get_digital_new_press(DIGITAL_L2)) {
 pistonWing.toggle();}
 pros::delay(20); 
}
}

