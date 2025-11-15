// src/auton_utils.cpp
#include "auton_select.hpp"
#include "lemlib/api.hpp"
#include "auton_routines.hpp"



short int selected_auto = 0;


int get_selected_auto(){return selected_auto;}

void auton_menus(){
    bool autoselected = false;
    while (autoselected == false){
        main_menu();
    }
}

void main_menu(){
    bool sectionselected = false;
    while (sectionselected == false){
        pros::screen::set_pen(pros::Color::red);
        pros::screen::fill_rect(0,0,96,272);
        pros::screen::set_pen(pros::Color::blue);
        pros::screen::fill_rect(384,0,480,272);
    }

}

void clear_screen(){
    pros::screen::set_pen(pros::Color::white);
    pros::screen::fill_rect(0,0,480,272);
}