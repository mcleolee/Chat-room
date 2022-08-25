#include <stdio.h>
#include <string.h>
#include <sys/socket.h>     //man 3 socket / man 7 ip 
#include <netinet/in.h> 
#include <netinet/ip.h>     /* superset of previous */
#include <sys/socket.h>     // man  3 inet_addr
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "tcp.h"

int main(int argc, char **argv)
{
#if 0
    if(3 != argc)
    {
        printf("Usage: <%s> <IP> <PORT>\n", argv[0]);
        return -1;
    }
#endif

char buf[SIZE] = {0};

char ip_test_client[50] = "127.0.0.1";
short port_test_client = 6666;

int connectFd = tcp_client_init(ip_test_client, port_test_client); // 好像没用到backlog，如果后期不用就删掉吧

printf("the connectFd is %d\n",connectFd);

tcp_client_communication(connectFd, buf);
 
    //关闭套结字
    close(connectFd);










}