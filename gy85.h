#ifndef GY85_H
#define GY85_H
#include <stdio.h>


class gy85
{



  public:
    gy85();
    void initAll();
    void readGyro(double r[4]);
    void readMagneto(int r[3]);
    float magnetoGetHeading();
    void readAccelerometer(double r[3]);
};


#endif // GY85_H
