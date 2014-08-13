#ifndef GY_85_H
#define GY_85_H
#include <stdio.h>


class gy85
{
  private:
    
    
  public:
    gy85();
    void init();
    void readGyro(double r[4]);
    void readMagneto(int r[3]);
    void readAccelerometer(double r[3]);
};
