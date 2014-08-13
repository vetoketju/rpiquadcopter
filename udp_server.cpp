#include "udp_server.h"
#include <iostream>
#include "controlpackage.cpp"
#define BUFSIZE 2048

// HOW TO SEND ME TEST MESSAGE?
// echo -n "asd" >/dev/udp/127.0.0.1/23456
// IMPORTANT NOTE!!!: For some reason using "localhost" does not work, must send to 127.0.0.1 IP!

using namespace std;

	void udp_server::start_server(){
	  cout << "start" << endl;
	  if(port < 1000 || port > 65535){
	    cout << "invalid port" << endl;
	    error = true;
	  }

	  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                cout << "cannot create socket" << endl;
                error = true;
	  }
	  /* bind the socket to any valid IP address and a specific port */

	  memset((char *)&myaddr, 0, sizeof(myaddr));
	  myaddr.sin_family = AF_INET;
	  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	  myaddr.sin_port = htons(port);

	  if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
                cout << "port bind failed" << endl;
                error = true;
	  }

	 // std::thread
	  if(error == true){cout << "some error occured" << endl;}
	  else { cout << "no errors" << endl; }
	  /* now loop, receiving data and printing what we received */
	  if(error == false){
	    for (;;) {
		  cout << "waiting on port: " << port << endl;
		  recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		  cout << "something received" << endl;
		  cout << "received message with sizeof "<< recvlen << endl;
		  if (recvlen > 0) {
			  buf[recvlen] = 0;
			  cout << "message: " << buf << endl;
			  //printf("received message: \"%s\"\n", buf);
		  }
	    }
	    cout << "end of loop" << endl;
	  }
	}

	udp_server::udp_server(int port, controlpackage ctrl)
	{
	  this->port = port;
    this->ctrl = ctrl;
	}
