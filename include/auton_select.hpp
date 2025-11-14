#pragma once

#include "main.h"       
#include "lemlib/api.hpp"
#include <map>

namespace auton {

// --------- High-level menu modes ---------

// Main menu: Skills or Game?
enum class MainMode {
    Skills,
    Game
};

// Skills submenu: Autonomous or Driving?
enum class SkillsMode {
    SkillsAuton,
    SkillsDriving
};

// Game submenu: alliance colour
enum class AllianceColor {
    Red,
    Blue
};

// Internal menu stage / screen
enum class MenuStage {
    MainMenu,        // "Skills or Game?"
    SkillsSubMenu,   // "Autonomous or Driving?"
    GameAllianceMenu,// "Red or Blue?"
    GameAutonMenu,   // "Which auton for this alliance?"
    Done             // Selection locked in
};

// Definition of a single game auton routine
struct GameAuton {
    const char* name;                    // Display name ("Red Close 3", etc.)
    AllianceColor alliance;              // Red or Blue
    void (*routine)(lemlib::Chassis&);   // Function pointer to run this auton
};

// --------- Public API (what main.cpp will use) ---------

// Call this once in initialize() or a setup file to register your auton routines.
// (Implementation in auton_utils.cpp will contain a static list you edit.)
void setup_default_game_autons();

// Call this from competition_initialize().
// While the robot is DISABLED, this runs the controller-based menu:
//  - Main Menu: Skills or Game
//  - If Skills:    choose Autonomous or Driving
//  - If Game:      choose Alliance, then specific auton
// It returns when either:
//  - Selection is confirmed (menu stage = Done), OR
//  - Robot is enabled (field control turns on auton/driver)
void run_menu_selector();

// Call this from autonomous().
// Dispatches to the correct routine based on the last selection:
//  - Skills + SkillsAuton  -> run skills auton routine
//  - Skills + SkillsDriving-> (optional) run a driver-like skills mode
//  - Game                  -> run the selected game auton for that alliance
void run_selected_auton();

// --------- Optional accessors (useful for debugging / printing) ---------

MainMode      get_main_mode();
SkillsMode    get_skills_mode();
AllianceColor get_alliance_color();

// If main mode is Game, this returns the selected game auton.
// (Behaviour is undefined if you call it in Skills mode.)
const GameAuton& get_selected_game_auton();

} 
