#include "main.h"
#include "lemlib/api.hpp"
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

namespace {
enum class BMacroState { Idle, OuttakeBurst, MiddleScore };

bool handleBMacro() {
    static bool bWasPressedLast = false;
    static BMacroState macroState = BMacroState::Idle;
    static uint32_t macroStartTime = 0;

    // --- timing constants ---
    constexpr uint32_t kOuttakeDurationMs      = 200;   // 0.2 s outtake
    constexpr uint32_t kMiddleScoreDurationMs  = 2500;  // total 2.5 s scoring phase
    constexpr uint32_t kMiddleScoreBoostMs     = 500;   // first 0.5 s at full speed
    constexpr uint32_t kMiddleScoreDecayEndMs  = 1500;  // at 1.5 s we reach "final" speed
    // last 0.5 s (3.0–3.5 s) will run at half of final speed

    // --- speed constants (match your presets convention) ---
    constexpr int kScoreStartSpeed = 500;   // like scoreMiddleGoal
    constexpr int kScoreEndSpeed   = 200;   // "final" speed before halving
    constexpr int kScoreHalfSpeed  = kScoreEndSpeed / 2; // last 0.5 s

    bool     bPressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B);
    uint32_t now      = pros::millis();

    // Start macro on rising edge if idle
    if (bPressed && !bWasPressedLast && macroState == BMacroState::Idle) {
        macroState     = BMacroState::OuttakeBurst;
        macroStartTime = now;

        // Match load DOWN at macro start
        setMatchLoad(true);   // assumes "extend = down"
    }

    switch (macroState) {
        case BMacroState::OuttakeBurst: {
            uint32_t elapsed = now - macroStartTime;
            if (elapsed < kOuttakeDurationMs) {
                // OUTTAKE: same pattern as runOuttake ( +, +, + )
                intakefunc(+600, +600, +600);
            } else {
                // move to middle-goal scoring phase
                macroState     = BMacroState::MiddleScore;
                macroStartTime = now;
            }
            break;
        }

        case BMacroState::MiddleScore: {
            uint32_t elapsed = now - macroStartTime;

            if (elapsed < kMiddleScoreDurationMs) {
                int mainSpeed  = 0;
                int scoreSpeed = 0;
                int midSpeed   = 0;

                if (elapsed < kMiddleScoreBoostMs) {
                    // 0.0–0.5 s: full power middle-goal scoring
                    // pattern: (-, +, -) like scoreMiddleGoal
                    mainSpeed  = -kScoreStartSpeed;
                    scoreSpeed =  kScoreStartSpeed;
                    midSpeed   = -kScoreStartSpeed;
                }
                else if (elapsed < kMiddleScoreDecayEndMs) {
                    // 0.5–3.0 s: decay from start → end
                    const uint32_t decayWindow  = kMiddleScoreDecayEndMs - kMiddleScoreBoostMs; // 2500 ms
                    uint32_t       decayElapsed = elapsed - kMiddleScoreBoostMs;

                    double progress = static_cast<double>(decayElapsed) / decayWindow; // 0..1
                    if (progress > 1.0) progress = 1.0;

                    double currentMag = kScoreStartSpeed +
                                        (kScoreEndSpeed - kScoreStartSpeed) * progress;
                    int current = static_cast<int>(currentMag);

                    // keep middle-goal pattern (-, +, -)
                    mainSpeed  = -current;
                    scoreSpeed =  current;
                    midSpeed   = -current;
                }
                else {
                    // 3.0–3.5 s: half of the final speed, still middle-goal pattern
                    mainSpeed  = -kScoreHalfSpeed;
                    scoreSpeed =  kScoreHalfSpeed;
                    midSpeed   = -kScoreHalfSpeed;
                }

                intakefunc(mainSpeed, scoreSpeed, midSpeed);
            } else {
                // macro finished: stop and bring match load back UP
                stopIntakes();
                setMatchLoad(false);   // assumes "retract = up"
                macroState = BMacroState::Idle;
            }
            break;
        }

        case BMacroState::Idle:
        default:
            break;
    }

    bWasPressedLast = bPressed;
    return macroState != BMacroState::Idle;
}
}



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

void autonomous() {

  run_selected_auton();

}

void opcontrol() {
  controller.print(0, 0, "Clr:%-4s", sorter_alliance_name(get_sorter_alliance()));
  pistonodom.set_value(true); 
pistonWing.set_value(true);

	while (true) {
  
  int forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  int turn    = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

  // --- Curvatherp drive ---
  DriveOutput driveOut = calc_curvatherp(forward, turn);
  left_mg.move(static_cast<int>(driveOut.left));
  right_mg.move(static_cast<int>(driveOut.right));

// --- Intake Controls ---

  bool macroRunning = handleBMacro();

  if (color_selected) {
  if (controller.get_digital_new_press(DIGITAL_X)) {
      cycle_sorter_alliance();
      controller.print(0, 0, "Clr:%-4s", sorter_alliance_name(get_sorter_alliance()));
  }}

  if (controller.get_digital_new_press(DIGITAL_B)) {
      toggle_sorter_enabled();
      controller.print(1, 0, "Sort:%s ", sorterEnabled ? "ON" : "OFF");
  }
  if (!macroRunning) {

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
