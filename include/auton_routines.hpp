#pragma once

#include "lemlib/api.hpp"

// All auton routines live in the auton namespace
namespace auton {

    // Game autons
    void red_1(lemlib::Chassis& chassis);
    void red_2(lemlib::Chassis& chassis);
    void red_3(lemlib::Chassis& chassis);
    void blue_1(lemlib::Chassis& chassis);
    void blue_2(lemlib::Chassis& chassis);
    void blue_3(lemlib::Chassis& chassis);

    // Skills
    void skills_auton_routine(lemlib::Chassis& chassis);
    void skills_driving_routine(lemlib::Chassis& chassis);
}
