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
} // namespace

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