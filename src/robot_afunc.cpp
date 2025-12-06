#include "robot_afunc.hpp"
#include "portconfig.hpp"
#include <cstdint>


namespace {

struct IntakeTargets {
    int main = 0;
    int score = 0;
    int mid = 0;
};

IntakeTargets lastTargets;

void rememberTargets(int speedmain, int speedscore, int speedmid) {
    lastTargets.main = speedmain;
    lastTargets.score = speedscore;
    lastTargets.mid = speedmid;
}


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
    rememberTargets(speedmain, speedscore, speedmid);
    intakeMain.move_velocity(speedmain);
    intakescore.move_velocity(speedscore);
    intakemid.move_velocity(speedmid);
}

int get_intake_target_speed() {
    // Positive return values mean the intakes are trying to bring rings inward,
    // aligning with the sorter spec that checks for "> 0" before acting.
    return -lastTargets.main;
}
void stopIntakes() {intakefunc(0, 0, 0);}
void runIntakeStore(int duration_ms) {runPreset(-300, 0, -300, duration_ms);}
void runOuttake(int duration_ms) {runPreset(600, 600, 600, duration_ms);}
void runlowscore(int duration_ms) {runPreset(600, 0, 600, duration_ms);}
void scoreMiddleGoal(int duration_ms) {
    //runPreset(600,600,600,400);
    runPreset(-600, 600, -600, duration_ms);}
void scoreHighGoal(int duration_ms) {runPreset(-600, -600, -600, duration_ms);}

// === Pneumatic helpers ===
void setWingDescore(bool extend) {extendAndMaybeRetract(pistonWing, extend, 0);}
void toggleWingDescore() {pistonWing.toggle();}
void pulseWingDescore(int duration_ms) {extendAndMaybeRetract(pistonWing, true, duration_ms);}
void setMatchLoad(bool extend) {extendAndMaybeRetract(pistonload, extend, 0);}
void toggleMatchLoad() {pistonload.toggle();}
void pulseMatchLoad(int duration_ms) {extendAndMaybeRetract(pistonload, true, duration_ms);}

// === Color sorter helpers ===
bool sorterEnabled = true;

namespace {
SorterAlliance currentAlliance = SorterAlliance::None;
bool sorterSensorReady = false;

enum class RingColor { None, Red, Blue };

static uint32_t highDumpUntilMs = 0;

RingColor alliance_to_ring(SorterAlliance alliance) {
    switch (alliance) {
        case SorterAlliance::Red:
            return RingColor::Red;
        case SorterAlliance::Blue:
            return RingColor::Blue;
        default:
            return RingColor::None;
    }
}

RingColor detect_ring_color() {
    // pros::c::optical_rgb_s_t rgb = sorterOptical.get_rgb();
    // int total = rgb.red + rgb.green + rgb.blue;

    
    // if (total < 90) {
        if (sorterOptical.get_proximity() < 10) {

        return RingColor::None;
    }

    // double redRatio = static_cast<double>(rgb.red) / total;
    // double blueRatio = static_cast<double>(rgb.blue) / total;
    // double ratioDelta = redRatio - blueRatio;

    // if (ratioDelta >= 0.12) {
     // Hue gives a more reliable indication of ring color than raw RGB ratios
    double hue = sorterOptical.get_hue();

    // Treat wrap-around near 0/360 as "red"
    bool looksRed = (hue < 30.0) || (hue > 330.0);
    bool looksBlue = (hue > 170.0 && hue < 250.0);

    if (looksRed && !looksBlue) {
        return RingColor::Red;
    }

    if (looksBlue && !looksRed) {
        return RingColor::Blue;
    }

    return RingColor::None;
}

void apply_preset(SorterRequest request) {
    if (request == SorterRequest::HighGoal) {
        scoreHighGoal();
    } else {
        scoreMiddleGoal();
    }
}
} // namespace

SorterAlliance get_sorter_alliance() { return currentAlliance; }

void set_sorter_alliance(SorterAlliance alliance) { currentAlliance = alliance; }

void cycle_sorter_alliance() {
    switch (currentAlliance) {
        case SorterAlliance::None:
            currentAlliance = SorterAlliance::Red;
            break;
        case SorterAlliance::Red:
            currentAlliance = SorterAlliance::Blue;
            break;
        case SorterAlliance::Blue:
            currentAlliance = SorterAlliance::None;
            break;
    }
}

const char* sorter_alliance_name(SorterAlliance alliance) {
    switch (alliance) {
        case SorterAlliance::Red:
            return "Red";
        case SorterAlliance::Blue:
            return "Blue";
        default:
            return "None";
    }
}

bool team_assigned() { return currentAlliance != SorterAlliance::None; }

void set_sorter_enabled(bool enabled) {
    sorterEnabled = enabled;
    if (!enabled && sorterSensorReady) {
        sorterOptical.set_led_pwm(0);
    } else if (enabled && sorterSensorReady) {
        sorterOptical.set_led_pwm(90);
    }
}

void toggle_sorter_enabled() { set_sorter_enabled(!sorterEnabled); }

void run_color_sorter(SorterRequest request) {
    bool sorterActive = sorterEnabled && team_assigned();

    SorterRequest desiredRequest = request;

    if (sorterActive) {
        RingColor observed = detect_ring_color();
        RingColor targetColor = alliance_to_ring(currentAlliance);
        bool matchesAlliance = observed != RingColor::None && observed == targetColor;
        bool wrongColor = observed != RingColor::None && observed != targetColor;

        if (wrongColor) {
            desiredRequest = (request == SorterRequest::HighGoal) ? SorterRequest::MiddleGoal
                                                                 : SorterRequest::HighGoal;
        } else if (!matchesAlliance) {
            desiredRequest = request;
        }
    }

    apply_preset(desiredRequest);
}

void init_sorter_sensor() {
    sorterOptical.set_led_pwm(100);
    sorterOptical.set_integration_time(50);
    pros::delay(75);
    sorterSensorReady = true;
}
