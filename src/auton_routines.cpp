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
chassis.moveToPose(-22, 22, 70,1500,{.maxSpeed=90,.minSpeed=60});
pros::Task::delay(1000);
setMatchLoad(true);
chassis.moveToPose(-6,44, 30,2000,{.maxSpeed =127});
pros::delay(500);
setMatchLoad(false);

chassis.moveToPose(-24, 23, 45, 1500, {.forwards=false, .maxSpeed=127});
pros::delay(500);

chassis.moveToPose(-2, 8, -45, 2000,{.forwards=false, .maxSpeed=127});
pros::delay(1000);
stopIntakes();
pros::delay(500);
pros::Task::delay(100);
scoreMiddleGoal(1500);
pros::delay(1000);
stopIntakes();
pros::Task::delay(200);
chassis.moveToPose(-38, 46, -45, 2000,{.maxSpeed=127});
chassis.moveToPose(-80, 48, -90, 2000,{.maxSpeed=127, .minSpeed=80});
setMatchLoad(true);
runIntakeStore();
pros::delay(1500);
chassis.moveToPose(-10, 48, -90, 3000,{.forwards=false, .maxSpeed=127, .minSpeed=50});
pros::delay(500);
setMatchLoad(false);
pros::delay(1500);
scoreHighGoal();
pros::Task::delay(1500);
stopIntakes();}

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
