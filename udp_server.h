#ifndef UDP_SERVER_H
#define UDP_SERVER_H
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "controlpackage.cpp"
#include <iostream>
#include <chrono>

class udp_server
{
  private:
    struct sockaddr_in myaddr;             /* our address */
    struct sockaddr_in remaddr;            /* remote address */
    socklen_t addrlen;                   /* length of addresses */
    int recvlen;                           /* # bytes received */
    int fd;                                /* our socket */
    unsigned char buf[2048];           /* receive buffer */
    bool error;
    int port;
    controlpackage *ctrl;

  public:
    udp_server(int port, controlpackage *ctrl);
    void start_server();
};

#endif // UDP_SERVER_H
