#include "main.h"

//point not currently used, to be used in tandem with odometry
 class point{
  public:
    point():
      x(0), y(0)
    {}
    point(double xCoord, double yCoord):
      x(xCoord), y(yCoord)
    {}
    double x;
    double y;
  private:
    void normalize(){
      double magnitude = sqrt(x * x + y * y);
      x /= magnitude;
      y /= magnitude;
    }
 };
