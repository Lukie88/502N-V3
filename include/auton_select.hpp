#pragma once

#include "main.h"       



int get_selected_auto();
void auton_menus();
void clear_screen();
void red_menu();
void blue_menu();
void skills_menu();
void driving_skills_menu();
void main_menu_grid();
void draw_hollow_rect(int x0, int y0, int x1, int y1, pros::Color color);
bool is_touch_in_rect(int x, int y, int x1, int y1, int x2, int y2);
void outline_section(short int section, pros::Color color);
short int touch_registration(short int x, short int y);