// src/auton_utils.cpp
#include "auton_select.hpp"
#include "auton_routines.hpp"  


namespace auton {

namespace {

// GAME AUTON TABLE

GameAuton gameAutons[] = {
    // name           alliance             routine
    {"Red Nig1",  AllianceColor::Red,  red_1},
    {"Red Nig2", AllianceColor::Red,  red_2},
    {"Red Nig3", AllianceColor::Red,  red_3},
    {"Blue Nig1", AllianceColor::Blue, blue_1},
    {"Blue Nig2",AllianceColor::Blue, blue_2},
    {"Blue Nig3",AllianceColor::Blue, blue_3}

    
};

constexpr int GAME_AUTON_COUNT =
    sizeof(gameAutons) / sizeof(gameAutons[0]);

// NOTE ANDREW: ONE Red and ONE Blue routine or filter logic fucked

// GLOBAL SELECTION STATE (MENU + CHOICES) 

MenuStage     menuStage        = MenuStage::MainMenu;
MainMode      mainMode         = MainMode::Game;
SkillsMode    skillsMode       = SkillsMode::SkillsAuton;
AllianceColor allianceColor    = AllianceColor::Red;
int           selectedGameAutonIndex = 0;


// FIND FIRST AUTON FOR AN ALLIANCE 

int findFirstAutonIndexForAlliance(AllianceColor alliance) {
    for (int i = 0; i < GAME_AUTON_COUNT; ++i) {
        if (gameAutons[i].alliance == alliance) return i;
    }
    // Fallback: if none exist, just return 0
    return 0;
}


// ======== 5. MENU HANDLER FUNCTIONS (ONE PER SCREEN) ========

void handleMainMenu() {
    // internal UI state for this screen
    static bool skillsHighlighted = false; // false = Game, true = Skills

    // Toggle between "Game" and "Skills"
    if (controller.get_digital_new_press(DIGITAL_UP) ||
        controller.get_digital_new_press(DIGITAL_DOWN)) {
        skillsHighlighted = !skillsHighlighted;
    }

    // Confirm selection
    if (controller.get_digital_new_press(DIGITAL_A)) {
        if (skillsHighlighted) {
            mainMode   = MainMode::Skills;
            menuStage  = MenuStage::SkillsSubMenu;
        } else {
            mainMode   = MainMode::Game;
            menuStage  = MenuStage::GameAllianceMenu;
        }
    }
}

void handleSkillsSubMenu() {
    static bool autonHighlighted = true; // true = Auton, false = Driving

    // Toggle between "Autonomous" and "Driving"
    if (controller.get_digital_new_press(DIGITAL_UP) ||
        controller.get_digital_new_press(DIGITAL_DOWN)) {
        autonHighlighted = !autonHighlighted;
    }

    // Confirm selection
    if (controller.get_digital_new_press(DIGITAL_A)) {
        skillsMode = autonHighlighted ? SkillsMode::SkillsAuton
                                      : SkillsMode::SkillsDriving;
        menuStage  = MenuStage::Done;
    }

    // Go back to main menu
    if (controller.get_digital_new_press(DIGITAL_B)) {
        menuStage = MenuStage::MainMenu;
    }
}

void handleGameAllianceMenu() {
    static bool redHighlighted = true; // true = Red, false = Blue

    // Toggle between Red and Blue using any of these buttons
    if (controller.get_digital_new_press(DIGITAL_LEFT) ||
        controller.get_digital_new_press(DIGITAL_RIGHT) ||
        controller.get_digital_new_press(DIGITAL_UP) ||
        controller.get_digital_new_press(DIGITAL_DOWN)) {
        redHighlighted = !redHighlighted;
    }

    if (controller.get_digital_new_press(DIGITAL_A)) {
        allianceColor = redHighlighted ? AllianceColor::Red
                                       : AllianceColor::Blue;

        // Start from the first auton of that alliance
        selectedGameAutonIndex = findFirstAutonIndexForAlliance(allianceColor);
        menuStage = MenuStage::GameAutonMenu;
    }

    if (controller.get_digital_new_press(DIGITAL_B)) {
        menuStage = MenuStage::MainMenu;
    }
}

void handleGameAutonMenu() {
    // Move selection LEFT: go backward through the list, but only land on
    // autons that match the chosen alliance
    if (controller.get_digital_new_press(DIGITAL_LEFT)) {
        int attempts = 0;
        do {
            selectedGameAutonIndex =
                (selectedGameAutonIndex - 1 + GAME_AUTON_COUNT) % GAME_AUTON_COUNT;
            attempts++;
        } while (gameAutons[selectedGameAutonIndex].alliance != allianceColor &&
                 attempts <= GAME_AUTON_COUNT);
    }

    // Move selection RIGHT: go forward with same filtering
    if (controller.get_digital_new_press(DIGITAL_RIGHT)) {
        int attempts = 0;
        do {
            selectedGameAutonIndex =
                (selectedGameAutonIndex + 1) % GAME_AUTON_COUNT;
            attempts++;
        } while (gameAutons[selectedGameAutonIndex].alliance != allianceColor &&
                 attempts <= GAME_AUTON_COUNT);
    }

    // Confirm current auton
    if (controller.get_digital_new_press(DIGITAL_A)) {
        menuStage = MenuStage::Done;
    }

    // Go back to alliance choice
    if (controller.get_digital_new_press(DIGITAL_B)) {
        menuStage = MenuStage::GameAllianceMenu;
    }
}


// DISPLAY FUNCTION (BRAIN + CONTROLLER TEXT)

void renderMenu() {
    pros::lcd::clear_line(0);
    pros::lcd::clear_line(1);
    pros::lcd::clear_line(2);
    pros::lcd::clear_line(3);

    controller.clear_line(0);

    switch (menuStage) {
    case MenuStage::MainMenu: {
        pros::lcd::print(0, "Main Menu");
        pros::lcd::print(1, "UP/DOWN: Skills / Game");
        pros::lcd::print(2, "A: select");

        const char* current =
            (mainMode == MainMode::Skills) ? "Skills" : "Game";
        controller.print(0, 0, "Main: %s", current);
        break;
    }

    case MenuStage::SkillsSubMenu: {
        pros::lcd::print(0, "Skills Mode");
        pros::lcd::print(1, "UP/DOWN: Auton / Driving");
        pros::lcd::print(2, "A: confirm, B: back");

        const char* current =
            (skillsMode == SkillsMode::SkillsAuton) ? "Auton" : "Driving";
        controller.print(0, 0, "Skills: %s", current);
        break;
    }

    case MenuStage::GameAllianceMenu: {
        pros::lcd::print(0, "Game Mode");
        pros::lcd::print(1, "LEFT/RIGHT: Red / Blue");
        pros::lcd::print(2, "A: confirm, B: back");

        const char* current =
            (allianceColor == AllianceColor::Red) ? "Red" : "Blue";
        controller.print(0, 0, "Alliance: %s", current);
        break;
    }

    case MenuStage::GameAutonMenu: {
        const GameAuton& auton = gameAutons[selectedGameAutonIndex];
        pros::lcd::print(0, "Game Auton Select");
        pros::lcd::print(1, "LEFT/RIGHT: change");
        pros::lcd::print(2, "A: confirm, B: back");
        pros::lcd::print(3, "Auton: %s", auton.name);

        controller.print(0, 0, "%s", auton.name);
        break;
    }

    case MenuStage::Done: {
        pros::lcd::print(0, "Selection locked");
        break;
    }
    }
}

} 


// PUBLIC API IMPLEMENTATION (MATCHES HEADER) 

void setup_default_game_autons() {
    // Use the static gameAutons[] table above
    // Andrew can be smart and build dynamic setup, put array here
}

void run_menu_selector() {
    // Reset to a clean state each time
    menuStage        = MenuStage::MainMenu;
    mainMode         = MainMode::Game;
    skillsMode       = SkillsMode::SkillsAuton;
    allianceColor    = AllianceColor::Red;
    selectedGameAutonIndex = findFirstAutonIndexForAlliance(allianceColor);

    // Loop while robot is disabled AND we haven't confirmed selection
    while (pros::competition::is_disabled() &&
           menuStage != MenuStage::Done) {

        switch (menuStage) {
        case MenuStage::MainMenu:
            handleMainMenu();
            break;
        case MenuStage::SkillsSubMenu:
            handleSkillsSubMenu();
            break;
        case MenuStage::GameAllianceMenu:
            handleGameAllianceMenu();
            break;
        case MenuStage::GameAutonMenu:
            handleGameAutonMenu();
            break;
        default:
            break;
        }

        renderMenu();
        pros::delay(100);
    }
}

void run_selected_auton() {
    if (mainMode == MainMode::Skills) {
        if (skillsMode == SkillsMode::SkillsAuton) {
            skills_auton_routine(chassis);
        } else {
            skills_driving_routine(chassis);
        }
    } else {
        const GameAuton& selected = gameAutons[selectedGameAutonIndex];
        selected.routine(chassis);
    }
}

//Debugging / logging the blacks 

MainMode get_main_mode() {
    return mainMode;
}

SkillsMode get_skills_mode() {
    return skillsMode;
}

AllianceColor get_alliance_color() {
    return allianceColor;
}

const GameAuton& get_selected_game_auton() {
    return gameAutons[selectedGameAutonIndex];
}

} 
