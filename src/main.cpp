#include "main.h"
#include "autonomous.h"

void leftBtn(){

}
void centerBtn(){

}
void rightBtn(){

}
void initialize() {
	pros::lcd::initialize();

	pros::lcd::register_btn0_cb(leftBtn);
	pros::lcd::register_btn1_cb(centerBtn);
	pros::lcd::register_btn2_cb(rightBtn);

	calibrateSensors();

	//autonSelector();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	switch(selected){
     case 0:
		 	disabledAuton();
     case 1:
		 	pop();
     case 2:

     case 3:

     case 4:

     case 5:

     case 6:

     case 7:

		 case 8:
     	skills();
   }
}

void opcontrol() {

	while (true) {
		driverControl(
		 control.get_analog(E_CONTROLLER_ANALOG_LEFT_X),
		 control.get_analog(E_CONTROLLER_ANALOG_LEFT_Y),
		 control.get_analog(E_CONTROLLER_ANALOG_RIGHT_X));

		intake(0);
		rollers(0);

		if(control.get_digital(E_CONTROLLER_DIGITAL_L1)) intake(200);
		if(control.get_digital(E_CONTROLLER_DIGITAL_L2)) intake(-200);

		if(control.get_digital(E_CONTROLLER_DIGITAL_R1)) rollers(200);
		if(control.get_digital(E_CONTROLLER_DIGITAL_R2)) rollers(-200);

		if(control.get_digital(E_CONTROLLER_DIGITAL_A)) skills();
		// if(control.get_digital(E_CONTROLLER_DIGITAL_A)) driveForward(30, autoLnPID, 0, autoLnrtPID);
		// if(control.get_digital(E_CONTROLLER_DIGITAL_B)) rotate(0, autoRtPID);
		// if(control.get_digital(E_CONTROLLER_DIGITAL_Y)) driveStrafe(10, autoLnPID, 0, autoLnrtPID);
		// if(control.get_digital(E_CONTROLLER_DIGITAL_X)) calibrateSensors();

		printOnScreen();

		delay(10);
	}
}
