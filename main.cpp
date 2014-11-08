#include <iostream>
#include <thread>
#include "gy85.h"
#include "udp_server.h"
#include "controlpackage.cpp"
#include <pigpio.h>
#include <wiringPi.h>

#define BR_PIN 23
#define BL_PIN 24
#define FR_PIN 25
#define FL_PIN 18
#define RELAY_PIN 4
using namespace std;
controlpackage ctrlpkg;

void listen_udp(udp_server serveri){
  cout << "starting UDP-listener thread" << endl;
  serveri.start_server();
}
//wiriping softpwm EI TOIMI
//KATSO PIGPIO
void initMotors(){

  gpioSetMode(RELAY_PIN, OUTPUT);
  gpioSetMode(BR_PIN,PI_OUTPUT);
  gpioSetMode(BL_PIN,PI_OUTPUT);
  gpioSetMode(FR_PIN,PI_OUTPUT);
  gpioSetMode(FL_PIN,PI_OUTPUT);

  gpioWrite(RELAY_PIN, 1);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  gpioServo(BR_PIN, 2000);
  gpioServo(BL_PIN, 2000);
  gpioServo(FR_PIN, 2000);
  gpioServo(FL_PIN, 2000);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  gpioServo(BR_PIN, 700);
  gpioServo(BL_PIN, 700);
  gpioServo(FR_PIN, 700);
  gpioServo(FL_PIN, 700);
}

void exitMotors(){
  gpioServo(BR_PIN, 700);
  gpioServo(BL_PIN, 700);
  gpioServo(FR_PIN, 700);
  gpioServo(FL_PIN, 700);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  gpioWrite(RELAY_PIN, 0);
}

int main(int argc, char **argv) {
    std::cout << "RPIQuadcopter version 1.0" << std::endl;

    udp_server servu(23456, &ctrlpkg);
    thread udp_listener_thread(listen_udp,servu);
    wiringPiSetup () ; //gy85 vaatii vielä, vaihda joskus pigpio?
    gy85 gy;
    gy.initAll();
     //reletta varten
    if (gpioInitialise() < 0){
        cout << "pigpio initialisointi ei onnistunut" << endl;
        gpioTerminate();
        return -1;
    }
    cout << "Press any key to init motors" << endl;
    string s; cin >> s;
    initMotors();
    cout << "e to exit, w to add more power, s to give less power" << endl;
    bool flag = true;
    int pwm = 700;
    while(flag){
        cin >> s;
        if(s[0] == 'e'){
            flag = false;
        }
        else if(s[0] == 'w'){
            pwm+=100;
            pwm = min(pwm, 2000);
        }
        else if(s[0] == 's'){
            pwm-=100;
            pwm = max(pwm, 700);
        }
        gpioServo(BR_PIN, pwm);
        gpioServo(BL_PIN, pwm);
        gpioServo(FR_PIN, pwm);
        gpioServo(FL_PIN, pwm);
    }





    exitMotors();
    gpioTerminate();
    //for(;;){
      //std::this_thread::sleep_for(std::chrono::seconds(3));
      //double r[4];
      //gy.readGyro(r);
      //cout << r[0] << ", " << r[1] << ", " << r[2] << ", " << r[3] << endl;
    //  cout << "slept three sec" << endl;
    //}


}
