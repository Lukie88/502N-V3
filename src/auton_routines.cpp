#include "auton_routines.hpp"
#include "Main_Drive.hpp"
#include "Portconfig.hpp"
#include "lemlib/api.hpp"
#include "robot_afunc.hpp"



namespace auton_routes {

// === RED AUTONS === //

void red_1() {
    // TEMP: simple placeholder – you can replace this later
chassis.setPose(-48,15,90);
setWingDescore(true);
runIntakeStore();
chassis.moveToPose(-18, 26, 70,1500,{.minSpeed=30});

chassis.moveToPose(-5,48, 30,1500,{.maxSpeed =127});
setMatchLoad(true);
pros::delay(500);
setMatchLoad(false);
pros::delay(1000);

chassis.moveToPose(-24, 23, 45, 1500, {.forwards=false, .minSpeed=60});
pros::delay(500);

chassis.moveToPose(-6, 9, -45, 2000,{.forwards=false});
pros::delay(1000);
stopIntakes();
pros::delay(500);
scoreMiddleGoal();
pros::delay(2000);
stopIntakes();
chassis.moveToPose(-42, 47, -45, 1500,{.minSpeed=60});
chassis.moveToPose(-70, 47, -90, 1000,{.minSpeed=50});
setMatchLoad(true);
runIntakeStore();
pros::delay(2000);
chassis.moveToPose(-10, 43, -90, 2000,{.forwards=false});
pros::delay(500);
setMatchLoad(false);
pros::delay(1500);
scoreHighGoal();
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
