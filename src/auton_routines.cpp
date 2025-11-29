#include "auton_routines.hpp"
#include "main_drive.hpp"
#include "portconfig.hpp"
#include "lemlib/api.hpp"
#include "robot_afunc.hpp"

namespace auton_routes {

// === RED AUTONS === //

void red_1() {

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
chassis.moveToPose(-20.2, 49, -90, 3000,{.forwards=false, .maxSpeed=127, .minSpeed=40});//changed this
pros::delay(500);
setMatchLoad(false);
pros::delay(1500);
scoreHighGoal();
pros::Task::delay(1500);
stopIntakes();

}

void red_2() {

//RED RIGHT SIDE AUTON
chassis.setPose(-48, -15, 90);
setWingDescore(true);
runIntakeStore();

chassis.moveToPose(-20, -25, 110, 2000, {.lead = 0.3, .maxSpeed = 90.0, .minSpeed = 25.0}, true);
chassis.waitUntilDone();

setMatchLoad(true); // grab 3 balls
pros::Task::delay(1200);
setMatchLoad(false);

chassis.moveToPose(-10, -10, 45, 2000, { .lead = 0.1, .maxSpeed = 80});

pros::delay(1000);
stopIntakes();
pros::delay(500);
runOuttake();
pros::Task::delay(100);
runOuttake();
pros::delay(2000);
stopIntakes();
pros::Task::delay(200);

chassis.moveToPose(-38, -46, 45, 2200, {.forwards=false,.lead = 0.1, .maxSpeed = 100});
chassis.turnToHeading(-80,1000);

chassis.moveToPose(-61, -46, 270, 2000, {.lead = 0.1, .maxSpeed = 110});//changed to -90 from -80

setMatchLoad(true);
runIntakeStore();
pros::delay(1500);

chassis.moveToPose(-10, -48, -90, 3000, {.forwards = false, .maxSpeed = 127, .minSpeed = 40});

pros::delay(500);
setMatchLoad(false);
pros::delay(1500);
scoreHighGoal();
pros::Task::delay(750);
stopIntakes();

}

void red_3() {
//RED AWP AUTON
drive_distance_inches(10);
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
}

void blue_3() {
//BLUE AWP AUTON   
}

// === SKILLS === //

void skills_auton_routine() {
   
   chassis.setPose(-50,-15,180);
  setWingDescore(true);
  runIntakeStore();

  //Move to position to get ready for matchload
  chassis.moveToPose(-42, -45,180 , 1500, {.lead= 0.1,.maxSpeed=127});
  pros::Task::delay(1000);
  setMatchLoad(true);
  
  //Grab Matchload
  chassis.moveToPose(-61.5, -48,-95 , 3500, {.lead= 0.1,.maxSpeed=100, });
  pros::Task::delay(3500);


  //Move away and turn to get ready to run
  chassis.moveToPose(-52, -46,-90 , 1000, {.forwards=false,.lead= 0.1,.maxSpeed=127, .minSpeed=60});
  chassis.moveToPose(-37, -65,-90 , 1000, {.forwards=false,.lead= 0.5,.maxSpeed=90});
  pros::Task::delay(1000);
  setMatchLoad(false);

//Move to other side of field
  chassis.turnToHeading(90, 1000);
  chassis.moveToPose(33, -58,90, 2500, {.lead= 0.1,.maxSpeed=127});
  chassis.moveToPose(46, -45,90 , 1100, {.lead= 0.3,.maxSpeed=127});
   
//Score on other side
  chassis.moveToPose(16, -45,90 , 3500, {.forwards=false,.lead= 0.3,.maxSpeed=127});
  pros::Task::delay(1000);
  scoreHighGoal();
  pros::Task::delay(3000);
  
//Intake Second Matchloaders
  chassis.moveToPose(60, -46,90 ,2500, {.lead= 0.1,.maxSpeed=127});
    runIntakeStore();
  setMatchLoad(true);
  pros::Task::delay(3000);
    chassis.moveToPose(18, -46,80 , 3500, {.forwards=false,.lead= 0.1,.maxSpeed=127});
    pros::Task::delay(1200);
    setMatchLoad(false);
    scoreHighGoal();
    pros::Task::delay(3000);
 chassis.moveToPose(43, -42.5,90 , 1000, {.lead= 0.1,.maxSpeed=127});

//Go to 3rd intake
runIntakeStore();
 chassis.moveToPose(43, 49,-10 , 2500, {.lead= 0.1,.maxSpeed=127});
 chassis.turnToHeading(80, 1000);
    chassis.moveToPose(58, 47,80 , 2500, {.lead= 0.1,.maxSpeed=127});
    setMatchLoad(true);
    pros::Task::delay(3000);

 chassis.moveToPose(38, 66.5,90 , 1500, {.forwards=false,.lead= 0.1,.maxSpeed=127});
 pros::Task::delay(1000);
  setMatchLoad(false);
 chassis.turnToHeading(-90, 1000);
 chassis.moveToPose(-33,56 ,-90 , 2000, {.lead= 0.1,.maxSpeed=127});
 chassis.moveToPose(-44, 42,-90 , 1500, {.lead= 0.1,.maxSpeed=127});
chassis.moveToPose(-25, 47,-90 , 3500, {.forwards=false, .lead= 0.1,.maxSpeed=127});
 pros::Task::delay(1000);
 scoreHighGoal();
 pros::Task::delay(2500);
  chassis.moveToPose(-65, 42,-90 , 2500, {.lead= 0.3,.maxSpeed=127});
 setMatchLoad(true);
 pros::Task::delay(2500);
  chassis.moveToPose(-23, 46,-90 , 3500, {.forwards=false, .lead= 0.1,.maxSpeed=127});
 pros::Task::delay(2000);
 setMatchLoad(false);
 scoreHighGoal();
 pros::Task::delay(3000);
 stopIntakes();

 
 runIntakeStore();
   chassis.moveToPose(-43, 46,-90 , 1500, {.lead= 0.1,.maxSpeed=127});
  chassis.moveToPose(-64, 18,180 , 2000, {.lead= 0.5, .maxSpeed=127, });
  chassis.moveToPose(-64,-6,180,50000, {.lead= 0.1,.maxSpeed=127});

  setMatchLoad(true);
  drive_distance_inches(24);
    pistonodom.set_value(true);

}

void skills_driving_routine() {
void opcontrol();  
}

} 
