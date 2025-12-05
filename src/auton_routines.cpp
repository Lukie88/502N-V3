#include "auton_routines.hpp"
#include "main_drive.hpp"
#include "portconfig.hpp"
#include "lemlib/api.hpp"
#include "robot_afunc.hpp"

namespace auton_routes {

// === RED AUTONS === //

void red_1() {

//RED LEFT SIDE AUTON
imu_sensor.set_heading(90);
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

void tunepids(){
  left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0,0,0);
chassis.turnToHeading(90, 5000);
chassis.turnToHeading(180, 5000);
chassis.turnToHeading(270, 5000);
chassis.turnToHeading(0, 5000);
}



// === SKILLS === //

void skills_auton_routine() {
  left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(-48,-14.8,180);
  
  setWingDescore(true);
  runIntakeStore();

  //Move to position to get ready for matchload
  chassis.moveToPose(-45, -50,180 , 1500, {.horizontalDrift=8, .lead= 0.6, .maxSpeed=90});
  pros::Task::delay(400);
  //Grab Matchload
  chassis.turnToHeading(270, 800, { .maxSpeed = 60 });
    setMatchLoad(true);
  chassis.moveToPose(-61.5, -47,270 , 1000, {.lead= 0.3,.maxSpeed=90, });
  pros::Task::delay(3500);
chassis.waitUntilDone();
//   //Move away and turn to get ready to run
chassis.moveToPoint(-44, -48, 1500, {.forwards = false,.maxSpeed = 80});  
  chassis.turnToHeading(180, 800, { .maxSpeed = 60 });
setMatchLoad(false);
chassis.moveToPoint(-45,-60,1000,{.maxSpeed=80});

//   //Move to other side of field
  chassis.turnToHeading(90, 1000, {.maxSpeed=70});
  chassis.moveToPoint(33, -63, 1700, {.maxSpeed=110});
 chassis.moveToPose(42, -47, 95, 3000, {.lead = 0.3,.maxSpeed = 80});
//  chassis.moveToPose(26, -47, 90, 1000, {.lead = 0.3,.maxSpeed = 80});
// pros::Task::delay(500);

//  //   //Score on other side
//   scoreHighGoal();
//   pros::Task::delay(3000);
  
// //   //Intake Second Matchloaders
//   chassis.moveToPose(60, -46,90 ,2500, {.lead= 0.1,.maxSpeed=127});
//   runIntakeStore();
//   setMatchLoad(true);
//   pros::Task::delay(3000);
//   chassis.moveToPose(18, -46,80 , 3500, {.forwards=false,.lead= 0.1,.maxSpeed=127});
//   pros::Task::delay(1200);
//   setMatchLoad(false);
//   scoreHighGoal();
//   pros::Task::delay(3000);
//   chassis.moveToPose(43, -42.5,90 , 1000, {.lead= 0.1,.maxSpeed=127});

// //Go to 3rd intake
//   runIntakeStore();
//   chassis.moveToPose(43, 49,-10 , 2500, {.lead= 0.1,.maxSpeed=127});
//   chassis.turnToHeading(80, 1000);
//   chassis.moveToPose(58, 47,80 , 2500, {.lead= 0.1,.maxSpeed=127});
//   setMatchLoad(true);
//   pros::Task::delay(3000);

//   chassis.moveToPose(38, 66.5,90 , 1500, {.forwards=false,.lead= 0.1,.maxSpeed=127});
//   pros::Task::delay(1000);
//   setMatchLoad(false);
//   chassis.turnToHeading(-90, 1000);
//   chassis.moveToPose(-33,56 ,-90 , 2000, {.lead= 0.1,.maxSpeed=127});
//   chassis.moveToPose(-44, 42,-90 , 1500, {.lead= 0.1,.maxSpeed=127});
//   chassis.moveToPose(-25, 47,-90 , 3500, {.forwards=false, .lead= 0.1,.maxSpeed=127});
//   pros::Task::delay(1000);
//   scoreHighGoal();
//   pros::Task::delay(2500);
//   chassis.moveToPose(-65, 42,-90 , 2500, {.lead= 0.3,.maxSpeed=127});
//   setMatchLoad(true);
//   pros::Task::delay(2500);
//   chassis.moveToPose(-23, 46,-90 , 3500, {.forwards=false, .lead= 0.1,.maxSpeed=127});
//   pros::Task::delay(2000);
//   setMatchLoad(false);
//   scoreHighGoal();
//   pros::Task::delay(3000);
//   stopIntakes();

 
//   runIntakeStore();
//   chassis.moveToPose(-43, 46,-90 , 1500, {.lead= 0.1,.maxSpeed=127});
//   chassis.moveToPose(-64, 18,180 , 2000, {.lead= 0.5, .maxSpeed=127, });
//   chassis.moveToPose(-64,-6,180,50000, {.lead= 0.1,.maxSpeed=127});

//   setMatchLoad(true);
//   drive_distance_inches(24);
//   pistonodom.set_value(true);

}


void unselected_auton_routine() {
  chassis.setPose(0,0,0);
  chassis.moveToPose(0,20,0,1000,{});}
void skills_driving_routine() {opcontrol();}

} 
