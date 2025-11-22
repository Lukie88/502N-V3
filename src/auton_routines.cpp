#include "auton_routines.hpp"
#include "main_drive.hpp"
#include "portconfig.hpp"
#include "lemlib/api.hpp"
#include "robot_afunc.hpp"





namespace auton_routes {

// === RED AUTONS === //

void red_1() {
    // TEMP: simple placeholder – you can replace this later
//RED LEFT SIDE AUTON
chassis.setPose(-48,15,90);
setWingDescore(true);
runIntakeStore();
chassis.moveToPose( -20,22,70,1500, {.lead = 0.3, .maxSpeed = 90.0, .minSpeed = 25.0},true);
chassis.waitUntilDone();
setMatchLoad(true);//grab 3 balls
pros::Task::delay(1200);
setMatchLoad(false);

chassis.moveToPose(-8, 8, -45, 2000,{.forwards=false,.lead=0.1,  .maxSpeed=80});

pros::delay(1000);
stopIntakes();
pros::delay(500);
runOuttake();
pros::Task::delay(100);
scoreMiddleGoal();
pros::delay(1000);
stopIntakes();
pros::Task::delay(200);
chassis.moveToPose(-38, 46, -45, 1500,{.lead=0.1, .maxSpeed=100});
chassis.moveToPose(-80, 48, -100, 2000,{.lead=0.1, .maxSpeed=110});
setMatchLoad(true);
runIntakeStore();
pros::delay(2000);
chassis.moveToPose(-10, 49, -90, 3000,{.forwards=false, .maxSpeed=127, .minSpeed=40});
pros::delay(500);
setMatchLoad(false);
pros::delay(1500);
scoreHighGoal();
pros::Task::delay(1500);
stopIntakes();
}
void red_2() {
    chassis.setPose(-48, -15, 90);
setWingDescore(true);
runIntakeStore();

chassis.moveToPose(-20, -22, 110, 1500, {.lead = 0.3, .maxSpeed = 90.0, .minSpeed = 25.0}, true);
chassis.waitUntilDone();

setMatchLoad(true); // grab 3 balls
pros::Task::delay(1200);
setMatchLoad(false);

chassis.moveToPose(-8, -8, -135, 2000, {.forwards = false, .lead = 0.1, .maxSpeed = 80});

pros::delay(1000);
stopIntakes();
pros::delay(500);
runOuttake();
pros::Task::delay(100);
runOuttake();
pros::delay(1000);
stopIntakes();
pros::Task::delay(200);

chassis.moveToPose(-38, -46, -135, 1500, {.lead = 0.1, .maxSpeed = 100});

chassis.moveToPose(-80, -48, -80, 2000, {.lead = 0.1, .maxSpeed = 110});

setMatchLoad(true);
runIntakeStore();
pros::delay(2000);

chassis.moveToPose(-10, -49, -90, 3000, {.forwards = false, .maxSpeed = 127, .minSpeed = 40});

pros::delay(500);
setMatchLoad(false);
pros::delay(1500);
scoreHighGoal();
pros::Task::delay(1500);
stopIntakes();

}




void red_3() {
    chassis.setPose(0, 0, 0);
    // TODO
}

// === BLUE AUTONS === //

void blue_1() {
    //BLUE LEFT SIDE AUTON
chassis.setPose(-48,15,90);
setWingDescore(true);
runIntakeStore();
chassis.moveToPose( -20,22,70,1500, {.lead = 0.3, .maxSpeed = 90.0, .minSpeed = 25.0},true);
chassis.waitUntilDone();
setMatchLoad(true);//grab 3 balls
pros::Task::delay(1200);
setMatchLoad(false);

chassis.moveToPose(-8, 8, -45, 2000,{.forwards=false,.lead=0.1,  .maxSpeed=80});

pros::delay(1000);
stopIntakes();
pros::delay(500);
runOuttake();
pros::Task::delay(100);
scoreMiddleGoal();
pros::delay(1000);
stopIntakes();
pros::Task::delay(200);
chassis.moveToPose(-38, 46, -45, 1500,{.lead=0.1, .maxSpeed=100});
chassis.moveToPose(-80, 48, -100, 2000,{.lead=0.1, .maxSpeed=110});
setMatchLoad(true);
runIntakeStore();
pros::delay(2000);
chassis.moveToPose(-10, 49, -90, 3000,{.forwards=false, .maxSpeed=127, .minSpeed=40});
pros::delay(500);
setMatchLoad(false);
pros::delay(1500);
scoreHighGoal();
pros::Task::delay(1500);
stopIntakes();

}

void blue_2() {
    //BLUE RIGHT SIDE AUTON  
chassis.setPose(-48, -15, 90);
setWingDescore(true);
runIntakeStore();

chassis.moveToPose(-20, -22, 110, 1500, {.lead = 0.3, .maxSpeed = 90.0, .minSpeed = 25.0}, true);
chassis.waitUntilDone();

setMatchLoad(true); // grab 3 balls
pros::Task::delay(1200);
setMatchLoad(false);

chassis.moveToPose(-8, -8, -135, 2000, {.forwards = false, .lead = 0.1, .maxSpeed = 80});

pros::delay(1000);
stopIntakes();
pros::delay(500);
runOuttake();
pros::Task::delay(100);
runOuttake();
pros::delay(1000);
stopIntakes();
pros::Task::delay(200);

chassis.moveToPose(-38, -46, -135, 1500, {.lead = 0.1, .maxSpeed = 100});

chassis.moveToPose(-80, -48, -80, 2000, {.lead = 0.1, .maxSpeed = 110});

setMatchLoad(true);
runIntakeStore();
pros::delay(2000);

chassis.moveToPose(-10, -49, -90, 3000, {.forwards = false, .maxSpeed = 127, .minSpeed = 40});

pros::delay(500);
setMatchLoad(false);
pros::delay(1500);
scoreHighGoal();
pros::Task::delay(1500);
stopIntakes();
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
