#include "udp_server.h"

#define BUFSIZE 2048

// echo -n "asd" >/dev/udp/127.0.0.1/23456

using namespace std;

	void udp_server::setup(controlpackage* ptr_pkg){
	  this->pkg = ptr_pkg;
	}

	void udp_server::start_server(){
	  cout << "starting UDP Server..." << endl;
	  if(port < 1000 || port > 65535){
	    cout << "UDP Server terminated: invalid port" << endl;
	    error = true;
	  }
	  
	  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                cout << "UDP Server terminated: cannot create socket" << endl;
                error = true;
	  }
	  
	  memset((char *)&myaddr, 0, sizeof(myaddr));
	  myaddr.sin_family = AF_INET;
	  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	  myaddr.sin_port = htons(port);

	  if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
                cout << "UDP Server terminated: port bind failed" << endl;
                error = true;
	  }

	  if(error == true){cout << "UDP Server terminated: some error occured" << endl;}
	  else { cout << "UDP Server running" << endl; }
	  
	  if(error == false){
	    
	    for (;;) {
		  recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		  if(recvlen != 8){
		    cout << "UDP Server info: Invalid package length" <<endl;
		  }else{
			if(udp_server::parsePackage(buf)){
			  //cout << "parse ok" << endl;
			}else{
			  //cout << "parse failed" << endl;
			}
		  }
	    }
	    cout << "UDP Server terminated" << endl;
	  }
	
	  
	}
	
	bool udp_server::parsePackage(unsigned char buf[]){
		if(sizeof(buf) != 8) return 0;
	  
		int chk = 0;
		for(int i = 0; i < 7; i++){
		  chk += buf[i] * (i+1);
		}
		
		if(buf[7] != chk % 255){
		  cout << "Invalid checkdigit" << endl;
		  return 0;
		}
		
		pkg->command = buf[0];
		pkg->axis_x = buf[1];
		pkg->axis_y = buf[2];
		pkg->axis_z = buf[3];
		pkg->axis_r = buf[4];
		pkg->cam_x = buf[5];
		pkg->cam_y = buf[6];
		pkg->timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
		
		return 1;
	}
	
	udp_server::udp_server(int port)
	{
	  this->port = port;
	}