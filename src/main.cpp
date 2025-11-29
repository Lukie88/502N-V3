#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/adi.hpp"
#include "portconfig.hpp"
#include "auton_select.hpp"
#include "auton_routines.hpp"
#include "main_drive.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "robot_afunc.hpp"


void initialize() {
  chassis.calibrate(); // calibrate sensors
  chassis.setPose(0,0,0); // set starting position (x, y, heading)
  init_sorter_sensor();
  controller.rumble("."); // main systems calibrated
  init_sorter_sensor();

  //calibrate gui
  pros::delay(500);
  brain_menu();
  pros::Task lvgl_handler(lvgl_task, NULL, "LVGL Handler");
  controller.rumble(".-."); // gui operational
  pros::delay(20); // update every 20 ms
}

void disabled() {}

void competition_initialize() {}

void autonomous() {run_selected_auton();}

void opcontrol() {
  controller.print(0, 0, "Clr:%-4s", sorter_alliance_name(get_sorter_alliance()));
	while (true) {
  
  int forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  int turn    = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

  // --- Curvatherp drive ---
  DriveOutput driveOut = calc_curvatherp(forward, turn);
  left_mg.move(static_cast<int>(driveOut.left));
  right_mg.move(static_cast<int>(driveOut.right));

// --- Intake Controls ---
  if (color_selected) {
  if (controller.get_digital_new_press(DIGITAL_X)) {
      cycle_sorter_alliance();
      controller.print(0, 0, "Clr:%-4s", sorter_alliance_name(get_sorter_alliance()));
  }}

  if (controller.get_digital_new_press(DIGITAL_B)) {
      toggle_sorter_enabled();
      controller.print(1, 0, "Sort:%s ", sorterEnabled ? "ON" : "OFF");
  }

  if (controller.get_digital(DIGITAL_R1)) {
      // R1 → Intake (Just Storing)
      runIntakeStore();
    } else if (controller.get_digital(DIGITAL_R2)) {
      // R2 → Intake + High Goal Scoring
      run_color_sorter(SorterRequest::HighGoal);
    } else if (controller.get_digital(DIGITAL_L1)) {
      // L1 → Intake + Middle Goal Scoring
      run_color_sorter(SorterRequest::MiddleGoal);
    } else if (controller.get_digital(DIGITAL_Y)) {
      // Y → Outake
      runOuttake();
    } else {
      // No intake buttons — stop all three
      stopIntakes();
    }

// A button — momentary pulse on Piston C (match load)
if (controller.get_digital_new_press(DIGITAL_RIGHT)) {pistonload.toggle();}

if (controller.get_digital_new_press(DIGITAL_L2)) {pistonWing.toggle();}
 pros::delay(20); 
if (controller.get_digital_new_press(DIGITAL_A)) {
 pistonodom.toggle();}
 pros::delay(20); 
}
}
