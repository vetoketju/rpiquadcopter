#include <iostream>
#include <thread>
#include "gy85.h"
#include "udp_server.h"
#include "controlpackage.cpp"
#include <softPwm.h>
using namespace std;
controlpackage ctrlpkg;

void listen_udp(udp_server serveri){
  cout << "starting UDP-listener thread" << endl;
  serveri.start_server();
}

int main(int argc, char **argv) {
    std::cout << "RPIQuadcopter version 1.0" << std::endl;

    udp_server servu(23456, &ctrlpkg);
    thread udp_listener_thread(listen_udp,servu);
    gy85 gy;
    gy.initAll();
    // just a test
    for(;;){
      std::this_thread::sleep_for(std::chrono::seconds(3));
      double r[4];
      gy85.readGyro(r);
      cout << r[0] << ", " << r[1] << ", " << r[2] << ", " << r[3] << endl;
      cout << "slept three sec" << endl;
    }


}
