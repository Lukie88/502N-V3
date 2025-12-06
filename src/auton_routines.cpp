#include "auton_routines.hpp"
#include "main_drive.hpp"
#include "portconfig.hpp"
#include "lemlib/api.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robot_afunc.hpp"

namespace auton_routes {

// === RED AUTONS === //

void red_1() {

//RED LEFT SIDE AUTON
set_sorter_alliance(SorterAlliance::Red);
set_sorter_enabled(true);
 left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
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
run_color_sorter(SorterRequest::HighGoal);
pros::Task::delay(1500);
stopIntakes();
 left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void red_2() {

//RED RIGHT SIDE AUTON
set_sorter_alliance(SorterAlliance::Red);
set_sorter_enabled(true);
 left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
chassis.setPose(-48, -15, 90);
setWingDescore(true);
runIntakeStore();

chassis.moveToPose(-15, -25, 110, 2000, {.lead = 0.5, .maxSpeed = 90.0, .minSpeed = 25.0}, true);

pros::Task::delay(1200);

setMatchLoad(true); // grab 3 balls
pros::Task::delay(800);
setMatchLoad(false);

chassis.moveToPose(-10, -10, 45, 2000, { .lead = 0.1, .maxSpeed = 80});

pros::delay(500);
stopIntakes();
pros::delay(500);
runOuttake();
pros::Task::delay(100);
runlowscore();
pros::delay(1500);
stopIntakes();
pros::Task::delay(200);

chassis.moveToPose(-38, -44, 45, 2000, {.forwards=false,.lead = 0.1, .maxSpeed = 100});
chassis.turnToHeading(-80,1000);

chassis.moveToPose(-61, -43.5, -90, 3000, {.lead = 0.1, .maxSpeed = 110});//changed to -90 from -80

setMatchLoad(true);
runIntakeStore();
pros::delay(2500);

chassis.moveToPose(-10, -46, -90, 3000, {.forwards = false, .maxSpeed = 127, .minSpeed = 40});

pros::delay(500);
setMatchLoad(false);
pros::delay(1500);
run_color_sorter(SorterRequest::HighGoal);
 left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  pros::Task::delay(1000);
  stopIntakes();
}

void red_3() {
//RED AWP AUTON
drive_distance_inches(10);
}


// === BLUE AUTONS === //
void blue_1() {

//BLUE LEFT SIDE AUTON
set_sorter_alliance(SorterAlliance::Blue);
set_sorter_enabled(true);
 left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
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
run_color_sorter(SorterRequest::HighGoal);
pros::Task::delay(1500);
stopIntakes();
 left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void blue_2() {

//BLUE RIGHT SIDE AUTON  
set_sorter_alliance(SorterAlliance::Blue);
set_sorter_enabled(true);
 left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
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
runlowscore();
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
run_color_sorter(SorterRequest::HighGoal);
pros::Task::delay(2000);
 left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void blue_3() {
//BLUE AWP AUTON   
drive_distance_inches(10);

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
  chassis.setPose(-48,-14,180);

  setWingDescore(true);
  runIntakeStore();

  //Move to position to get ready for matchload
  chassis.moveToPose(-44, -48,180 , 1500, {.horizontalDrift=8, .lead= 0.6, .maxSpeed=90});
  pros::Task::delay(400);
  //Grab Matchload
  chassis.turnToHeading(-90, 800, { .maxSpeed = 60 });
  setMatchLoad(true);
  chassis.moveToPose(-61.5, -47,-90 , 3500, {.lead= 0.1,.maxSpeed=90, });
  pros::Task::delay(3500);
  chassis.waitUntilDone();
//   //Move away and turn to get ready to run
  chassis.moveToPoint(-44, -48, 1500, {.forwards = false,.maxSpeed = 80});
  chassis.turnToHeading(180, 800, { .maxSpeed = 60 });
  setMatchLoad(false);
  chassis.moveToPose(-35,-65,90,1000,{.maxSpeed=80});

//   //Move to other side of field
  chassis.turnToHeading(90, 1000, {.maxSpeed=70});
  chassis.moveToPoint(33, -65, 1700, {.maxSpeed=110});
  chassis.moveToPose(42, -52, 90, 2000, {.lead = 0.1,.maxSpeed = 80}); 
  chassis.moveToPose(18, -52, 87, 2000, {.forwards=false,.lead = 0.1,.maxSpeed = 80}); 
  pros::Task::delay(1000);
  scoreHighGoal();
 //   //Score on other side  scoreHighGoal();
  pros::Task::delay(3000);

//   //Intake Second Matchloaders
 chassis.setPose(26,-52,90);
  chassis.moveToPose(43, -51,90 ,1500, {.lead= 0.1,.maxSpeed=127});
    runIntakeStore();
  setMatchLoad(true);
  chassis.turnToHeading(90,1000);
  chassis.moveToPose(62, -51,90 ,3000, {.lead= 0.1,.maxSpeed=127});
  runIntakeStore();
  setMatchLoad(true);
  pros::Task::delay(3000);
  chassis.moveToPose(38, -52,90 , 1000, {.forwards=false,.lead= 0.1,.maxSpeed=127});
  chassis.turnToHeading(90,1000);
  chassis.moveToPose(15, -52,85 , 3500, {.forwards=false,.lead= 0.1,.maxSpeed=127});
  pros::Task::delay(1200);
  setMatchLoad(false);
  scoreHighGoal();
  pros::Task::delay(3000);
  chassis.setPose(26,-47,90);
  chassis.moveToPose(43, -42.5,90 , 1000, {.lead= 0.1,.maxSpeed=127});

//Go to 3rd intake
  runIntakeStore();
  chassis.moveToPose(43, 49,0 , 2500, {.lead= 0.1,.maxSpeed=127});
  chassis.turnToHeading(90, 1000);
  setMatchLoad(true);
  chassis.moveToPose(60, 49,90 , 2500, {.lead= 0.1,.maxSpeed=127});
  setMatchLoad(true);
  pros::Task::delay(3000);
  chassis.moveToPose(40, 49,90 , 1500, {.forwards=false,.lead= 0.1,.maxSpeed=127});
  pros::Task::delay(1150);
  setMatchLoad(false);
  

  chassis.moveToPose(28, 64.5,-90 , 1500, {.lead= 0.1,.maxSpeed=127});
  pros::Task::delay(1000);
  chassis.turnToHeading(-90, 1000);
  chassis.moveToPose(-33,62 ,-90 , 2000, {.lead= 0.1,.maxSpeed=127});
  chassis.moveToPose(-44, 52,-90 , 1500, {.lead= 0.1,.maxSpeed=127});
  chassis.moveToPose(-25, 52,-90 , 3500, {.forwards=false, .lead= 0.1,.maxSpeed=127});
  pros::Task::delay(1000);
  scoreHighGoal();
  pros::Task::delay(2500);
  chassis.moveToPose(-65, 52,-90 , 2500, {.lead= 0.3,.maxSpeed=127});
  runIntakeStore();
  setMatchLoad(true);
  pros::Task::delay(2500);
  chassis.moveToPose(-23, 52,-90 , 3500, {.forwards=false, .lead= 0.1,.maxSpeed=127});
  pros::Task::delay(2000);
  setMatchLoad(false);
  scoreHighGoal();
  pros::Task::delay(3000);
  stopIntakes();

 
  runIntakeStore();
  chassis.setPose(-23,52,-90);
  chassis.moveToPose(-40, 52,-90 , 2000, {.lead= 0.1,.maxSpeed=127});
  chassis.moveToPose(-40,10,180,2000,{.lead=0.1,.maxSpeed=127});
  chassis.turnToHeading(90, 1000);
  chassis.moveToPose(-70,10,90,3000,{.forwards=false,.lead=0.1,.maxSpeed=127});


  

}

void unselected_auton_routine() {
  chassis.setPose(0,0,0);
  chassis.moveToPose(0,20,0,1000,{});}
void skills_driving_routine() {opcontrol();}

} 
