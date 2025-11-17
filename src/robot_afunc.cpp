#include "robot_afunc.hpp"
#include "portconfig.hpp"

namespace {
void runPreset(int speedmain, int speedscore, int speedmid, int duration_ms) {
    intakefunc(speedmain, speedscore, speedmid);
    if (duration_ms > 0) {
        pros::delay(duration_ms);
        stopIntakes();
    }
}

void extendAndMaybeRetract(pros::adi::Pneumatics& piston, bool extend, int duration_ms) {
    if (extend) {
        piston.extend();
    } else {
        piston.retract();
    }

    if (duration_ms > 0 && extend) {
        pros::delay(duration_ms);
        piston.retract();
    }
}
} // namespace

// === Intake helpers ===
void intakefunc(int speedmain, int speedscore, int speedmid) {
    intakeMain.move_velocity(speedmain);
    intakescore.move_velocity(speedscore);
    intakemid.move_velocity(speedmid);
}

void stopIntakes() {
    intakefunc(0, 0, 0);
}

void runIntakeStore(int duration_ms) {
    runPreset(-600, 0, -600, duration_ms);
}

void runOuttake(int duration_ms) {
    runPreset(600, 600, 600, duration_ms);
}

void scoreMiddleGoal(int duration_ms) {
    runPreset(-600, 600, -600, duration_ms);
}

void scoreHighGoal(int duration_ms) {
    runPreset(-600, -600, -600, duration_ms);
}

// === Pneumatic helpers ===
void setWingDescore(bool extend) {
    extendAndMaybeRetract(pistonWing, extend, 0);
}

void toggleWingDescore() {
    pistonWing.toggle();
}

void pulseWingDescore(int duration_ms) {
    extendAndMaybeRetract(pistonWing, true, duration_ms);
}

void setMatchLoad(bool extend) {
    extendAndMaybeRetract(pistonload, extend, 0);
}

void toggleMatchLoad() {
    pistonload.toggle();
}

void pulseMatchLoad(int duration_ms) {
    extendAndMaybeRetract(pistonload, true, duration_ms);
}