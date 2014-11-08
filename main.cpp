#include <iostream>
#include <thread>
#include "gy85.h"
#include "udp_server.h"
#include "controlpackage.cpp"
#include <pigpio.h>
#include <wiringPi.h>

using namespace std;
controlpackage ctrlpkg;

void listen_udp(udp_server serveri){
  cout << "starting UDP-listener thread" << endl;
  serveri.start_server();
}
//wiriping softpwm EI TOIMI
//KATSO PIGPIO
int main(int argc, char **argv) {
    std::cout << "RPIQuadcopter version 1.0" << std::endl;

    udp_server servu(23456, &ctrlpkg);
    thread udp_listener_thread(listen_udp,servu);
    gy85 gy;
    gy.initAll();
    pinMode(7, OUTPUT); //reletta varten
    if (gpioInitialise() < 0){
        cout << "pigpio initialisointi ei onnistunut" << endl;
        gpioTerminate():
        return -1:
    }

   // Set out ESC pins to output. TODO: Vaiha noi numerot, niin että ne definataan ylhäällä!
   gpioSetMode(23,PI_OUTPUT);
   gpioSetMode(24,PI_OUTPUT);
   gpioSetMode(27,PI_OUTPUT);
   gpioSetMode(22,PI_OUTPUT);



    cout << "Laita jotain niin rele paalle" << endl;
    string s; cin >> s;

    //std::this_thread::sleep_for(std::chrono::seconds(2));
    digitalWrite(7, HIGH);//rele paalle
    std::this_thread::sleep_for(std::chrono::seconds(2));
    gpioServo(27, 2000);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    gpioServo(27, 700);
    cout << "MIN POWER, any key to set  to 800" << endl;
    cin >> s;
    gpioServo(27, 800);
    cout << "any input -> off" << endl;
    cin >> s;
    digitalWrite(7, LOW);//rele pois
    gpioTerminate():
    //for(;;){
      //std::this_thread::sleep_for(std::chrono::seconds(3));
      //double r[4];
      //gy.readGyro(r);
      //cout << r[0] << ", " << r[1] << ", " << r[2] << ", " << r[3] << endl;
    //  cout << "slept three sec" << endl;
    //}


}
