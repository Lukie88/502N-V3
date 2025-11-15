// src/auton_utils.cpp
#include "auton_select.hpp"
#include "lemlib/api.hpp"
#include "auton_routines.hpp"
#include "Portconfig.hpp"


short int selected_auto = 0;
short int selected_section = 0;
// 0 = red, 1 = blue, 2 = skills, 3 = driving skills

int get_selected_auto(){return selected_auto;}

void auton_menus(){
    bool autoselected = false;
    while (autoselected == false){
        main_menu();
        clear_screen();
        switch (selected_section){
            case 0:red_menu();break;
            case 1:blue_menu();break;
            case 2:skills_menu();break;
            case 3:driving_skills_menu();break;
        }

    }
}

void main_menu(){
    bool sectionselected = false;
    while (sectionselected == false){
        main_menu_grid();
        switch (selected_section){
            case 0: draw_hollow_rect(0,0,126,272,pros::Color::black); break;
            case 1: draw_hollow_rect(354,0,480,272,pros::Color::black); break;
            case 2: draw_hollow_rect(127,0,353,136,pros::Color::black); break;
            case 3: draw_hollow_rect(127,137,353,272,pros::Color::black); break;
        }
        if (controller.get_digital_new_press(DIGITAL_LEFT)){selected_section -= 1; if (selected_section < 0){selected_section = 3;}}
        if (controller.get_digital_new_press(DIGITAL_RIGHT)){selected_section += 1; if (selected_section > 3){selected_section = 0;}}
        if (controller.get_digital_new_press(DIGITAL_A)){sectionselected = true;}
    }

}

void clear_screen(){
    pros::screen::set_pen(pros::Color::white);
    pros::screen::fill_rect(0,0,480,272);
}

void red_menu(){}

void blue_menu(){}

void skills_menu(){}

void driving_skills_menu(){

}

void main_menu_grid(){
    pros::screen::set_pen(pros::Color::red);
    pros::screen::fill_rect(0,0,126,272);
    pros::screen::set_pen(pros::Color::blue);
    pros::screen::fill_rect(354,0,480,272);
    pros::screen::set_pen(pros::Color::gray);
    pros::screen::fill_rect(127,0,353,136);
    pros::screen::set_pen(pros::Color::white);
    pros::screen::fill_rect(127,137,353,272);
}

void draw_hollow_rect(int x0, int y0, int x1, int y1, pros::Color color){
    pros::screen::set_pen(color);
    pros::screen::fill_rect(x0,y0,x1,y0-5); //top
    pros::screen::fill_rect(x0,y1+5,x1,y1); //bottom
    pros::screen::fill_rect(x0,y0,x0-5,y1); //left
    pros::screen::fill_rect(x1+5,y0,x1,y1); //right
}