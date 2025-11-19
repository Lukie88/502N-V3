// src/auton_utils.cpp
#include "auton_select.hpp"
#include "lemlib/api.hpp"
#include "auton_routines.hpp"
#include "Portconfig.hpp"
#include "pros/screen.hpp"
#include "pros/rtos.hpp"



//Quadrant 0
#define Q0_X1 0
#define Q0_Y1 0
#define Q0_X2 126
#define Q0_Y2 272
//Quadrant 1
#define Q1_X1 354
#define Q1_Y1 0
#define Q1_X2 480
#define Q1_Y2 272
//Quadrant 2
#define Q2_X1 127
#define Q2_Y1 0
#define Q2_X2 353
#define Q2_Y2 136
//Quadrant 3
#define Q3_X1 127
#define Q3_Y1 137
#define Q3_X2 353
#define Q3_Y2 272








short int selected_auto = 0;      // which specific auton inside a color menu (to be used later)
short int selected_section = 0;   // 0 = red, 1 = blue, 2 = skills, 3 = driving skills

int get_selected_auto() { return selected_auto; }

// // Clear entire Brain screen to white
void clear_screen() {
    pros::screen::set_pen(pros::Color::white);
    pros::screen::fill_rect(0, 0, 480, 272);
}

// Draw the 4 quadrants: Red, Blue, Skills, Driving Skills
void main_menu_grid() {
    // Red (left column)
    pros::screen::set_pen(pros::Color::red);
    pros::screen::fill_rect(Q0_X1, Q0_Y1, Q0_X2, Q0_Y2);

    // Blue (right column)
    pros::screen::set_pen(pros::Color::blue);
    pros::screen::fill_rect(Q1_X1, Q1_Y1, Q1_X2, Q1_Y2);

    // Skills (top middle)
    pros::screen::set_pen(pros::Color::gray);
    pros::screen::fill_rect(Q2_X1, Q2_Y1, Q2_X2, Q2_Y2);

    // Driving skills (bottom middle)
    pros::screen::set_pen(pros::Color::white);
    pros::screen::fill_rect(Q3_X1, Q3_Y1, Q3_X2, Q3_Y2);
}

// Draw a hollow rectangle with a fixed thickness around the given box
void draw_hollow_rect(int x0, int y0, int x1, int y1, pros::Color color) {
    pros::screen::set_pen(color);
    int t = 5; // border thickness in pixels

    // top
    pros::screen::fill_rect(x0, y0,    x1,    y0 + t);
    // bottom
    pros::screen::fill_rect(x0, y1 - t, x1,   y1);
    // left
    pros::screen::fill_rect(x0, y0,    x0 + t, y1);
    // right
    pros::screen::fill_rect(x1 - t, y0, x1,   y1);
}

// Stub menus for later expansion
void red_menu() {}
void blue_menu() {}
void skills_menu() {}
void driving_skills_menu() {}

/**
Top-level auton selection UI.
 * Runs during disabled/initialization to let the driver choose:
 *   - Red / Blue / Skills / Driving Skills
 *   - LEFT / RIGHT: cycle between the 4 sections
 *   - A: lock in the current section and exit
 *   - B: reset to default (Red)
 */
void auton_menus() {
    // This flag tracks whether the user has locked in a choice with A
    bool autoSelected = false;


    selected_section = 10;

    //clear_screen();
    pros::screen_touch_status_s_t touchstatus;
    // Run while robot is disabled and we haven't locked selection
    while (autoSelected == false) {
        // --- INPUT HANDLING --- //

        touchstatus = pros::screen::touch_status();
        // Move selection left (wrap 0 -> 3)
        if (touchstatus.touch_status == pros::E_TOUCH_RELEASED) {
            
            int x = touchstatus.x;
            int y = touchstatus.y;

            // Check touch against the four quadrant areas (buttons)
            if (is_touch_in_rect(x, y, Q0_X1, Q0_Y1, Q0_X2, Q0_Y2)) {
                // Top Left: Red Front (Index 0)
                selected_section = 0;
            } else if (is_touch_in_rect(x, y, Q1_X1, Q1_Y1, Q1_X2, Q1_Y2)) {
                // Top Right: Blue Back (Index 1)
                selected_section = 1;
            } else if (is_touch_in_rect(x, y, Q2_X1, Q2_Y1, Q2_X2, Q2_Y2)) {
                // Bottom Left: Skills (Index 2)
                selected_section = 2;
            } else if (is_touch_in_rect(x, y, Q3_X1, Q3_Y1, Q3_X2, Q3_Y2)) {
                // Bottom Right: L1 Spin (Index 3)
                selected_section = 3;
            }
            
            // If any quadrant was touched (re-selected or selected for the first time), lock it in
            if (selected_section >= 0 && selected_section <= 3) {
                 autoSelected = true;
            }
        }

        // A button: confirm current section and exit this menu
        if (controller.get_digital_new_press(DIGITAL_A)) {
            autoSelected = true; // this will break the while loop
        }
        
        // B button: reset to default (Red) and clear screen
        if (controller.get_digital_new_press(DIGITAL_B)) {
            selected_section = 0;
            clear_screen();
        }

        // UI

        // Draw all 4 quadrants
        main_menu_grid();

        // Draw a highlight box around the currently selected section
        switch (selected_section) {
            case 0: // Red (left)
                draw_hollow_rect(0,0,126,    272, pros::Color::black);break;
            case 1: // Blue (right)
                draw_hollow_rect(354,0,480,  272, pros::Color::black);break;
            case 2: // Skills (top middle)
                draw_hollow_rect(127,0,353,  136, pros::Color::black);break;
            case 3: // Driving skills (bottom middle)
                draw_hollow_rect(127,137,353,272, pros::Color::black);break;
        }

        pros::delay(50);
    }


    //I  want you to givem e a guide on how to create autonomous fucntions. so bacilly in the auton code when run autonomous, lets make one command for when i  run the auton commnad, itr spins the motor as it would in when i press L1
}

bool is_touch_in_rect(int x, int y, int x1, int y1, int x2, int y2) {
    // Check bounds: x must be between x1 and x2, y between y1 and y2
    return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}