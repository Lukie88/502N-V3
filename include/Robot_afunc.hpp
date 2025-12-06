#pragma once

#include "pros/rtos.hpp"

// === Intake helpers ===
// Raw helper to command the three intake motors at once.
void intakefunc(int speedmain, int speedscore, int speedmid);
int get_intake_target_speed();

// Utility helpers for common presets. Passing duration_ms <= 0 keeps the
// motors running until another command stops them.
void stopIntakes();
void runIntakeStore(int duration_ms = 0);
void runOuttake(int duration_ms = 0);
void scoreMiddleGoal(int duration_ms = 0);
void scoreHighGoal(int duration_ms = 0);
void runlowscore(int duration_ms = 0);
// === Pneumatic helpers ===
// "Wing" refers to the L2 descore piston, and "match load" to the right-arrow
// downward actuator.
void setWingDescore(bool extend);
void toggleWingDescore();
void pulseWingDescore(int duration_ms = 0);

void setMatchLoad(bool extend);
void toggleMatchLoad();
void pulseMatchLoad(int duration_ms = 0);

// === Color sorter helpers ===
enum class SorterAlliance { None, Red, Blue };
enum class SorterRequest { HighGoal, MiddleGoal };

extern bool sorterEnabled;

SorterAlliance get_sorter_alliance();
void set_sorter_alliance(SorterAlliance alliance);
void cycle_sorter_alliance();
const char* sorter_alliance_name(SorterAlliance alliance);
bool team_assigned();

void set_sorter_enabled(bool enabled);
void toggle_sorter_enabled();

void run_color_sorter(SorterRequest request);
void init_sorter_sensor();