#include "main.h"
#include "pid.h"
#include "ramp.h"
#include "xDrive.h"
#include "okapi/api.hpp"

using namespace pros;

const double ticksPerDeg = 900/360;
//200*4.125*pi/60 * sqrt(2), = 60
const double robotSpeed = 43.196 * sqrt(2);
const double rotationSpeed = 200;

extern Controller control;
extern Motor FrontLeft;
extern Motor FrontRight;
extern Motor BackLeft;
extern Motor BackRight;
extern Motor LeftIntake;
extern Motor RightIntake;

//Ports
const int FLPort = 1;
const int FRPort = 3;
const int BLPort = 8;
const int BRPort = 9;
const int LIPort = 17;
const int RIPort = 7;
const int LRPort = 16;
const int RRPort = 10;
const int IMUPort = 6;

Controller control (E_CONTROLLER_MASTER);
Motor FrontLeft(FLPort, false);
Motor FrontRight(FRPort, true);
Motor BackLeft(BLPort, false);
Motor BackRight(BRPort, true);
Motor LeftIntake(LIPort, false);
Motor RightIntake(RIPort, true);
Motor LeftRoller(LRPort, false);
Motor RightRoller(RRPort, true);
Imu inertial(IMUPort);

ADIEncoder xWheel(1, 2, true);
ADIEncoder yWheel(5, 6, true);


//Initialize custom classes
ramp FLRamp = ramp(1000);
ramp FRRamp = ramp(1000);
ramp BLRamp = ramp(1000);
ramp BRRamp = ramp(1000);

ramp LIRamp = ramp(2000);
ramp RIRamp = ramp(2000);

pidController autoLnPID = pidController(0, 2, 0.2, 0);
pidController autoLnrtPID = pidController(0, 2, 0, 0);
pidController autoPerpPID = pidController(0, 0, 0, 0);
pidController autoRtPID = pidController(0, 2, 0.005, 3);

xDrive drive = xDrive(4.125, 18.7, robotSpeed/127, rotationSpeed/127);

//Calibrate the sensors
void calibrateSensors(){
  xWheel.reset();
  yWheel.reset();
  inertial.reset();

  int timeInit = millis();

  inertial.reset();
  while(inertial.is_calibrating()){
    lcd::print(1, "Calibrating");
    delay(10);
  }
  delay(2000);
  lcd::print(1, "Calibration took %f", millis() - timeInit);

  //Additional PID Tuning
  autoLnPID.minLim = 50;
  autoLnPID.iLim = 100;
  autoLnrtPID.minLim = 0;
  autoRtPID.iLim = 100;
}
