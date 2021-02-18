#include "main.h"
#include "functions.h"

//This file has all of the autonomous
void disabledAuton(){

}

void pop(){

}

void skills(){
  flipout();
  intake(0);
  rollers(-200);
  //intake(0);
  rollers(0);
  driveForward(21.5, autoLnPID, 0, autoLnrtPID);
  rotate(-135, autoRtPID);
  driveForward(24, autoLnPID, -135, autoLnrtPID);
  rollers(200);
  intake(-200);
  delay(1500);
  intake(0);
  rollers(0);
  driveForward(-11, autoLnPID, -135, autoLnrtPID);
  rotate(0, autoRtPID);

  intake(-200);
  rollers(50);
  driveForward(43, autoLnPID, 0, autoLnrtPID);
  delay(100);
  intake(0);
  rollers(0);
  driveStrafe(5, autoLnPID, 0, autoLnrtPID);
  rotate(-90, autoRtPID);
  driveForward(10, autoLnPID, -90, autoLnrtPID);
  rollers(200);
  delay(1500);
  rollers(0);
  driveForward(-10, autoLnPID, -90, autoLnrtPID);
  rotate(0, autoRtPID);
  driveStrafe(2.5, autoLnPID, 0, autoLnrtPID);

  intake(-200);
  rollers(50);
  driveForward(44, autoLnPID, 0, autoLnrtPID);
  delay(100);
  intake(0);
  rollers(0);
  driveStrafe(-9, autoLnPID, 0, autoLnrtPID);
  rotate(-45, autoRtPID);
  driveForward(10, autoLnPID, -45, autoLnrtPID);
  rollers(200);
  delay(1500);
  rollers(0);
  driveForward(-10, autoLnPID, -45, autoLnrtPID);
}
