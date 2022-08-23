#ifndef _TCP_H_
#define _TCP_H_

#define SERVER_IP "127.0.0.1" // INADDR_ANY
#define SERVER_PORT 6666 

#define SIZE 1024

int tcp_server_init(char *ip,short port, int backlog, int listenFd);
int tcp_communication(int connectFd);
int tcp_print(char *s);

#endif