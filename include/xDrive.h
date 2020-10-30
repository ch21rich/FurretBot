#include "main.h"

//This class controls the velocity of the drive with some calculations
class xDrive{
  public:
    //Angle zero is positive X Axis
		//Linear Velocity is in Inches/Sec
		//Rotational Velocity is in Deg/Sec, clockwise

    xDrive(double wheelDiameter, double wheelToWheel):
      wheelCircumfrence(wheelDiameter * M_PI), wheelBaseCircumfrence(wheelToWheel * M_PI)
    {}

    xDrive(double wheelDiameter, double wheelToWheel, double linScale, double rScale):
      wheelCircumfrence(wheelDiameter * M_PI), wheelBaseCircumfrence(wheelToWheel * M_PI), xScaleJoy(linScale), yScaleJoy(linScale), rScaleJoy(rScale)
    {}

    xDrive(double wheelDiameter, double wheelToWheel, double xScale, double yScale, double rScale):
      wheelCircumfrence(wheelDiameter * M_PI), wheelBaseCircumfrence(wheelToWheel * M_PI), xScaleJoy(xScale), yScaleJoy(yScale), rScaleJoy(rScale)
    {}

    double root2 = sqrt(2);
    double DPS2RPM = 6;
    double degToRad = M_PI / 180;
    double wheelCircumfrence;
    double wheelBaseCircumfrence;
    double maxRPM = 200;

    double xScaleJoy;
    double yScaleJoy;
    double rScaleJoy;

    double xVel;
    double yVel;
    double rVel;
    double linearVel;
    double joyAngle;
    double robotAngle;
    double modAngle;

    double wheelTL;
    double wheelTR;
    double wheelBL;
    double wheelBR;

    void calculateWheelSpeeds(double x, double y, double r){
      xVel = x * xScaleJoy;
      yVel = y * yScaleJoy;
      rVel = r * rScaleJoy;

      linearVel = sqrt(x * x + y * y);
      double linearMax = maxRPM;

      double xRPM = xVel/(wheelCircumfrence * root2) * 360 / DPS2RPM; //Convert velocity to motor speeds
  		double yRPM = yVel/(wheelCircumfrence * root2) * 360 / DPS2RPM; //Convert velocity to motor speeds
  		double rRPM = rVel;//wheelBaseCircumfrence/wheelCircumfrence * rVel / DPS2RPM;

      //Budget the velocity, since you cannot go above 200 RPM
      pros::lcd::print(3, "%f", getSign(rRPM));
      if(rRPM > maxRPM) rRPM = getSign(rRPM) * maxRPM;
      pros::lcd::print(4, "%f", getSign(rRPM));
      double speedRatio = 0;
      if(abs(xVel) + abs(yVel) != 0) speedRatio = abs(xVel) / (abs(xVel) + abs(yVel));
      if(abs(xVel) + abs(yVel) + abs(rVel) > maxRPM){
        linearMax -= abs(rVel);
        xRPM = linearMax * speedRatio * getSign(xVel);
        yRPM = linearMax * (1 - speedRatio) * getSign(yVel);
      }

      //Calculate the speed for each wheel
      wheelTL = yRPM + xRPM + rRPM;
  		wheelTR = yRPM - xRPM - rRPM;
  		wheelBL = yRPM - xRPM + rRPM;
  		wheelBR = yRPM + xRPM - rRPM;
    }
    void calculateWheelSpeeds(double x, double y, double r, double robotAngle){
      xVel = x;
      yVel = y;
      rVel = r;
      linearVel = sqrt(x * x + y * y);

      joyAngle = atan2(x, y) * 180 / M_PI;
      modAngle = robotAngle + joyAngle;
      xVel = linearVel * cos(modAngle);
		  yVel = linearVel * sin(modAngle);

      double linearMax = maxRPM;

      double xRPM = xVel/(wheelCircumfrence * root2) * 360 / DPS2RPM;//Convert velocity to motor speeds
  		double yRPM = yVel/(wheelCircumfrence * root2) * 360 / DPS2RPM;//Convert velocity to motor speeds
  		double rRPM = wheelBaseCircumfrence/wheelCircumfrence * rVel / DPS2RPM;//Convert velocity to motor speeds

      //Budget the velocity, since you cannot go above 200 RPM

      if(rRPM > maxRPM) rRPM = getSign(rRPM) * maxRPM;

      double speedRatio = 0;
      if(abs(xVel) + abs(yVel) != 0) speedRatio = abs(xVel) / (abs(xVel) + abs(yVel));
      if(abs(xVel) + abs(yVel) + abs(rVel) > maxRPM){
        linearMax -= abs(rVel);
        xRPM = linearMax * speedRatio * getSign(xVel);
        yRPM = linearMax * (1 - speedRatio) * getSign(yVel);
      }
      //Calculate the speed for each wheel
      wheelTL = yRPM + xRPM + rRPM;
  		wheelTR = yRPM - xRPM - rRPM;
  		wheelBL = yRPM - xRPM + rRPM;
  		wheelBR = yRPM + xRPM - rRPM;
    }
  private:
    //General Useful Functions Used in the class
    double getSign(double input){
      if(input == 0) return 0;
      return abs(input)/input;
    }
    double abs(double input){
      if(input > 0) return input;
      return -input;
    }
};


//Odometry Currently Not Used, In Beta
class odometryX{
  public:
    odometryX(double xDistanceToWheel, double yDistanceToWheel):
      wheelXDist(xDistanceToWheel), wheelYDist(yDistanceToWheel)
    {}
    const double encToRev = 360;
    const double wheelCircumfrence = 2.75 * M_PI;
    double wheelXDist;
    double wheelYDist;

    double cX = 0;
    double pX = 0;
    double pXEnc = 0;

    double cY = 0;
    double pY = 0;
    double pYEnc = 0;

    double cR = 0;
    double pR;

    double cT = 0;
    double pT = 0;

    void updateOdom(double nextTime, double xIn, double yIn, double rIn){
      pT = cT;
      cT = nextTime;

      pR = cR;
      cR = rIn;

      //If we rotate at speed cR - pR, in deg per sec, then the distance traveled by the wheel will be (speed / 360) * (2 * wheelDistance) * (change in time)
      double xRDist = (cR - pR) * 2 * wheelXDist * (cT - pT);
      double yRDist = (cR - pR) * 2 * wheelYDist * (cT - pT);

      pX = cX;
      cX += ((xIn - pXEnc)/encToRev * wheelCircumfrence - xRDist) * (cT - pT);

      pY = cY;
      cY += ((yIn - pYEnc)/encToRev * wheelCircumfrence - yRDist) * (cT - pT);

      pXEnc = xIn;
      pYEnc = yIn;
    }

  private:
    double getSign(double input){
      return abs(input)/input;
    }
    double abs(double input){
      if(input > 0) return input;
      return -input;
    }
};
