#include "main.h"

//Class ramp to have a steady acceleration, so that there is not too much wheel slip or strain on the motorSetup
//Currently not used
class ramp {
  public:
  	double currentVel = 0;
  	double targetVel = 0;

  	//degrees/second/second
  	double maxAcc = 1;

  	double time = 0;
  	double pTime = time;

    ramp(double acceleration):
    maxAcc(acceleration)
    {}

  	void setTarget(double newVel) {
  		targetVel = newVel;
  	}
  	void updateVel(double newTime) {
  		pTime = time;
  		time = newTime;

  		if(currentVel == targetVel || getSign(targetVel - currentVel) != getSign(targetVel - currentVel - getSign(targetVel - currentVel) * maxAcc * (time - pTime))) {
  			currentVel = targetVel;
  		}else {
  			currentVel += getSign(targetVel - currentVel) * maxAcc * (time - pTime);
  		}
  	}
  	double getSign(double input) {
  		if(input != 0) {
  			return abs(input)/input;
  		}
  		return 0;
  	}
    double abs(double input){
      if(input > 0) return input;
      return -input;
    }
};
