#include "auton_routines.hpp"
#include "Portconfig.hpp" // or "devices.hpp" or whatever header defines `chassis`
#include "Main_Drive.hpp"
#include "lemlib/api.hpp"


namespace auton {

// === RED AUTONS === //

void red_1(lemlib::Chassis& chassis) {
    // TEMP: simple placeholder – you can replace this later
    chassis.setPose(0, 0, 0);
}

void red_2(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 0);
    // TODO: implement later
}

void red_3(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 0);
    // TODO
}

// === BLUE AUTONS === //

void blue_1(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 0);
    // TODO
}

void blue_2(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 0);
    // TODO
}

void blue_3(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 0);
    // TODO
}

// === SKILLS === //

void skills_auton_routine(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 0);
    // TODO: full skills path
}

void skills_driving_routine(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 0);
    // Maybe just park in a safe place, or leave empty
}

} // namespace auton
