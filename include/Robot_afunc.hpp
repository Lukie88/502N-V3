#pragma once

#include "pros/rtos.hpp"

// Three intake at once-raw
void intakefunc(int speedmain, int speedscore, int speedmid);

// Utility helpers for common presets. Passing duration_ms <= 0 keeps the
// motors running until another command stops them.

void stopIntakes();
void runIntakeStore(int duration_ms = 0);
void runOuttake(int duration_ms = 0);
void scoreMiddleGoal(int duration_ms = 0);
void scoreHighGoal(int duration_ms = 0);