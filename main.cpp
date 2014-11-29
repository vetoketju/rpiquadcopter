#include <iostream>
#include <thread>
#include "gy85.h"
#include "udp_server.h"
#include "controlpackage.cpp"
#include <pigpio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <chrono>


#define BR_PIN 23
#define BL_PIN 24
#define FR_PIN 25
#define FL_PIN 18
#define RELAY_PIN 4
using namespace std;

controlpackage pkg;
bool is_on = false;
int pwm_br = 700;
int pwm_bl = 700;
int pwm_fr = 700;
int pwm_fl = 700;
unsigned long long last_received;
unsigned long long last_time;
void listen_udp(udp_server serveri){
  serveri.setup(&pkg);
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
void applyChanges(){
    if(pkg.axis_y < 0){
        pwm_br = max(700, pwm_br-10);
        pwm_bl = max(700, pwm_bl-10);
        pwm_fr = max(700, pwm_fr-10);
        pwm_fl = max(700, pwm_fl-10);
    }
    else if(pkg.axis_y > 0){
        pwm_br = min(2000, pwm_br+10);
        pwm_bl = min(2000, pwm_bl+10);
        pwm_fr = min(2000, pwm_fr+10);
        pwm_fl = min(2000, pwm_fl+10);
    }

    //we should balance here or something
}
void balance(){


    //we should balance here or something
}
void mainloop(){
    cout << "e to exit, w to add more power, s to give less power" << endl;

    while(true){
        unsigned long long time_now = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        if(time_now - last_time < 50){
            std::this_thread::sleep_for(std::chrono::milliseconds(50-(time_now-last_time)));
        }
        last_time =chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        if(pkg.command == 1){
            cout << "copter is on" << endl;
            is_on  = true;
        }
        else if(pkg.command == 2){
            is_on = false;
            break;
        }
        else if(pkg.command == 3){
            //TODO: add calibrating
        }
        else if(pkg.command == 4){
            //TODO: camera on
        }
        else if(pkg.command == 5){
            //TODO: camera off
        }
        if(!is_on){
            //Waiting till the first packet from controller is recieved
            continue;
        }


        if(last_received == pkg.timestamp && time_now - pkg.timestamp > 3000){
            cout << "No communication in last 3 seconds\nshutting down" << endl;
            break;
        }
        else if(last_received == pkg.timestamp){
            balance();
            continue;
        }

        //All checks are done and the copter must be stabilized and control applied
        last_received = pkg.timestamp;
        //Let's apply the changes from controller, and stabilize
        applyChanges();
        balance();
        gpioServo(BR_PIN, pwm_br);
        gpioServo(BL_PIN, pwm_bl);
        gpioServo(FR_PIN, pwm_fr);
        gpioServo(FL_PIN, pwm_fl);
    }

    cout << "exiting mainloop" << endl;
}
int main(int argc, char **argv) {
    std::cout << "RPIQuadcopter version 1.0" << std::endl;

    udp_server servu(23456);
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
    cout << "Now entering mainloop" << endl;
    mainloop();


    exitMotors();
    gpioTerminate();
    //for(;;){
      //std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 500 MS
        // prints out timestamp,
      //cout << "timestamp: " << pkg.timestamp << endl;
      //double r[4];
      //gy.readGyro(r);
      //cout << r[0] << ", " << r[1] << ", " << r[2] << ", " << r[3] << endl;
    //}

    return 0;
}
