// src/auton_utils.cpp
#include "auton_select.hpp"
#include "lemlib/api.hpp"
#include "auton_routines.hpp"
#include "Portconfig.hpp"
#include "pros/screen.hpp"
#include "pros/rtos.hpp"


//Quadrant coordinate definitions
//Quadrant 0
constexpr short int Q0_X1 = 0;
constexpr short int Q0_Y1 = 0;
constexpr short int Q0_X2 = 126;
constexpr short int Q0_Y2 = 272;
//Quadrant 1
constexpr short int Q1_X1 = 354;
constexpr short int Q1_Y1 = 0;
constexpr short int Q1_X2 = 480;
constexpr short int Q1_Y2 = 272;
//Quadrant 2
constexpr short int Q2_X1 = 127;
constexpr short int Q2_Y1 = 0;
constexpr short int Q2_X2 = 353;
constexpr short int Q2_Y2 = 136;
//Quadrant 3
constexpr short int Q3_X1 = 127;
constexpr short int Q3_Y1 = 137;
constexpr short int Q3_X2 = 353;
constexpr short int Q3_Y2 = 272;

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
    bool auto_selected = false;
    selected_section = 10;
    short int double_tap_confirmation = 10;
    clear_screen();
    pros::screen_touch_status_s_t touch_status;
    // Run while we haven't locked selection
    while (auto_selected == false) {
        // --- INPUT HANDLING --- //

        touch_status = pros::screen::touch_status();
        // Move selection left (wrap 0 -> 3)
        if (touch_status.touch_status == pros::E_TOUCH_RELEASED) {
            double_tap_confirmation = touch_registration(touch_status.x, touch_status.y);
            
            if (double_tap_confirmation >= 0 && double_tap_confirmation <= 3) {
                // If the user tapped the SAME quadrant twice Lock it in
                if (double_tap_confirmation == selected_section) {
                    outline_section(selected_section, pros::Color::black);
                    auto_selected = true;}
                else{selected_section = double_tap_confirmation;
                outline_section(selected_section, pros::Color::light_cyan);}
            }
        }
        main_menu_grid();
        pros::delay(50);
    }
    

}

bool is_touch_in_rect(int x, int y, int x1, int y1, int x2, int y2) {
    // Check bounds: x must be between x1 and x2, y between y1 and y2
    return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}

void outline_section(short int section, pros::Color color) {
    switch (section) {
        case 0: // Red (left)
            draw_hollow_rect(0,0,126,    272, color);break;
        case 1: // Blue (right)
            draw_hollow_rect(354,0,480,  272, color);break;
        case 2: // Skills (top middle)
            draw_hollow_rect(127,0,353,  136, color);break;
        case 3: // Driving skills (bottom middle)
            draw_hollow_rect(127,137,353,272, color);break;
    }
}

short int touch_registration(short int x, short int y) {
    if (is_touch_in_rect(x, y, Q0_X1, Q0_Y1, Q0_X2, Q0_Y2)) {
                // Top Left: Red Front (Index 0)
                return 0;
            } else if (is_touch_in_rect(x, y, Q1_X1, Q1_Y1, Q1_X2, Q1_Y2)) {
                // Top Right: Blue Back (Index 1)
                return 1;
            } else if (is_touch_in_rect(x, y, Q2_X1, Q2_Y1, Q2_X2, Q2_Y2)) {
                // Bottom Left: Skills (Index 2)
                return 2;
            } else if (is_touch_in_rect(x, y, Q3_X1, Q3_Y1, Q3_X2, Q3_Y2)) {
                // Bottom Right: L1 Spin (Index 3)
                return 3;
            }
}