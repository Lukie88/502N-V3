#include "auton_routines.hpp"
#include "Main_Drive.hpp"
#include "Portconfig.hpp"
#include "lemlib/api.hpp"
#include "robot_afunc.hpp"



namespace auton_routes {

// === RED AUTONS === //

void red_1() {
    // TEMP: simple placeholder – you can replace this later
    chassis.setPose(0, 0, 0);
}

void red_2() {
    chassis.setPose(0, 0, 0);
    // TODO: implement later
}

void red_3() {
    chassis.setPose(0, 0, 0);
    // TODO
}

// === BLUE AUTONS === //

void blue_1() {
    chassis.setPose(0, 0, 0);
    // TODO
}

void blue_2() {
    chassis.setPose(0, 0, 0);
    // TODO
}

void blue_3() {
    chassis.setPose(0, 0, 0);
    // TODO
}

// === SKILLS === //

void skills_auton_routine() {
    chassis.setPose(0, 0, 0);
    // TODO: full skills path
}

void skills_driving_routine() {
    chassis.setPose(0, 0, 0);
    // Maybe just park in a safe place, or leave empty
}

} // namespace auton
