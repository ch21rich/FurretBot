#include "motorSetup.h"
#include "main.h"

//Useful Constants
const double wheelCircumfrence = 2.75 * M_PI;

//For debugging things
void printOnScreen(){
	//lcd::print(1, "Velocity FL: %f", FrontLeft.get_actual_velocity());
	//lcd::print(2, "Target Velocity FL: %f", drive.wheelTL);
  lcd::print(0, "Inertial Reading: %f", inertial.get_rotation());
  lcd::print(1, "Y Wheel Reading: %f", ((double) yWheel.get_value()));
  lcd::print(2, "X Wheel Reading: %f", ((double) xWheel.get_value()));
}

void driverControl(double x, double y, double r){
  //Calculates speed of wheels for driver control
	drive.calculateWheelSpeeds(x, y, r);

  // FLRamp.setTarget(drive.wheelTL);
  // FRRamp.setTarget(drive.wheelTR);
  // BLRamp.setTarget(drive.wheelBL);
  // BRRamp.setTarget(drive.wheelBR);
  //
  // FLRamp.updateVel(((double) millis()) / 1000);
  // FRRamp.updateVel(((double) millis()) / 1000);
  // BLRamp.updateVel(((double) millis()) / 1000);
  // BRRamp.updateVel(((double) millis()) / 1000);

	FrontLeft.move_velocity(drive.wheelTL);
	FrontRight.move_velocity(drive.wheelTR);
	BackLeft.move_velocity(drive.wheelBL);
	BackRight.move_velocity(drive.wheelBR);
}

void runDriveValues(){
  //Shortcut to run the values from the xDrive class
  FrontLeft.move_velocity(drive.wheelTL);
	FrontRight.move_velocity(drive.wheelTR);
	BackLeft.move_velocity(drive.wheelBL);
	BackRight.move_velocity(drive.wheelBR);
}

void stopDrive(bool hold = false){
  //Shortcut to stop the drive quickly
  if(hold){
    FrontLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    FrontRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    BackLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    BackRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  }
  FrontLeft.move_velocity(0);
	FrontRight.move_velocity(0);
	BackLeft.move_velocity(0);
	BackRight.move_velocity(0);
  delay(100);
  FrontLeft.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  FrontRight.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  BackLeft.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  BackRight.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void intake(double velocity){
  //Shortcut to power the intakes
  LeftIntake.move_velocity(velocity);
  RightIntake.move_velocity(velocity);
}

void rollers(double velocity){
  //Shortcut to power the rollers.
  LeftRoller.move_velocity(velocity);
  RightRoller.move_velocity(velocity);
}

void flipout(){
  LeftIntake.move_velocity(200);
  delay(1000);
  intake(200);
  rollers(200);
  delay(250);
  intake(0);
  rollers(0);
}

void driveForward(double inches, pidController controller, double angle, pidController rtController, int timeMax = 5000){
  //Reset The Controller's integral values
  controller.resetID();
  rtController.resetID();
  //Get the current tracking wheel value, and calculate a target X inches away.
  double initialY = ((double) yWheel.get_value()) * wheelCircumfrence/360;
  double targetY = ((double) yWheel.get_value()) * wheelCircumfrence/360 + inches;
  int initialT = millis();

  //Set the PID's target, and initialize its error.
  controller.tVal = targetY;
  controller.error = controller.tVal - ((double) yWheel.get_value()) * wheelCircumfrence/360;
  rtController.tVal = angle;
  rtController.error = angle - inertial.get_rotation();
  while(!controller.withinTarget() && millis() - initialT < timeMax){
    printOnScreen();
    //Calculate speed based on how far away we are from target, and the time taken.
    rtController.update(inertial.get_rotation());
    controller.update(((double) yWheel.get_value()) * wheelCircumfrence/360);
    //Calculate speed needed for each wheel
    drive.calculateWheelSpeeds(0, controller.calculateOut(), rtController.calculateOut());
    //Move the drive
    runDriveValues();
    //Delay to have a steady loop time, and not calculate too rapidly
    delay(10);
  }
  //Stop drive
  stopDrive(true);
  //Wait for the drive to settle for a bit
}

void driveStrafe(double inches, pidController controller, double angle, pidController rtController, int timeMax = 5000){
  //Reset The Controller's integral values
  controller.resetID();
  rtController.resetID();
  //Get the current tracking wheel value, and calculate a target X inches away.
  double initialX = ((double) xWheel.get_value()) * wheelCircumfrence/360;
  double targetX = ((double) xWheel.get_value()) * wheelCircumfrence/360 + inches;
  int initialT = millis();

  //Set the PID's target, and initialize its error.
  controller.tVal = targetX;
  controller.error = controller.tVal - ((double) xWheel.get_value()) * wheelCircumfrence/360;
  rtController.tVal = angle;
  rtController.error = angle - inertial.get_rotation();
  while(!controller.withinTarget() && millis() - initialT < timeMax){
    printOnScreen();
    //Calculate speed based on how far away we are from target, and the time taken.
    rtController.update(inertial.get_rotation());
    controller.update(((double) xWheel.get_value()) * wheelCircumfrence/360);
    //Calculate speed needed for each wheel
    drive.calculateWheelSpeeds(controller.calculateOut(), 0, rtController.calculateOut());
    //Move the drive
    runDriveValues();
    //Delay to have a steady loop time, and not calculate too rapidly
    delay(10);
  }
  //Stop drive
  stopDrive(true);
  //Wait for the drive to settle for a bit
}

void rotate(double angle, pidController controller){
  //Reset The Controller's integral values
  controller.resetID();
  double initialR = inertial.get_rotation();

  //Set the PID's target, and initialize its error
  controller.tVal = angle;
  controller.error = angle - initialR;

  while(!controller.withinTarget()){
    //Calculate speed based on how far away we are from nominal angle.
    controller.update(inertial.get_rotation());
    //Calculate speed needed for each wheel
    drive.calculateWheelSpeeds(0, 0, controller.calculateOut());
    //Move the drive
    runDriveValues();
    //Delay to have a steady loop time, and not calculate too rapidly
    delay(10);
  }
  //Stop drive
  stopDrive(true);
  //Wait for the drive to settle for a bit
}



//Picks the autonomous
//Auton Data
int selected = 0;
std::string autons[9] = {"Disabled", "Pop", "Red X", "Red Y", "Red Z", "Blue X", "Blue Y", "Blue Z", "Skills"};
int size = 9;//*(&autons + 1) - autons;

void autonSelector(){
  control.clear();
  delay(200);
  while(true){
    control.clear();
    delay(100);
    control.print(2, 1, autons[selected].c_str());
    delay(100);
     if(control.get_digital(DIGITAL_RIGHT)){
       selected = (selected + 1 + size) % size;
     }else if(control.get_digital(DIGITAL_LEFT)){
       selected = (selected - 1 + size) % size;
     }else if(control.get_digital(DIGITAL_A)){
       pros::delay(200);
       if(control.get_digital(DIGITAL_A)){
         goto slctEnd;
       }
     }
   }
   slctEnd:
   control.rumble("..");
}
