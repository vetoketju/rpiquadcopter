#include <iostream>
#include <thread>
#include "udp_server.h"

using namespace std;


void listen_udp(udp_server serveri){
  cout << "startging listener thread" << endl;
  serveri.start_server();
}

int main(int argc, char **argv) {
    std::cout << "Hello, asd asd world!" << std::endl;
    udp_server servu(23456);
    thread udp_listener_thread(listen_udp,servu);
    
    for(;;){
      
      std::this_thread::sleep_for(std::chrono::seconds(3));
      cout << "slept three sec" << endl;
    }
    
}
