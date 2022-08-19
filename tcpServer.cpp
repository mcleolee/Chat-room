#include <stdio.h>
#include <string.h>
#include <sys/socket.h>     //man 3 socket 
#include <sys/socket.h>     //man 7 ip 
#include <netinet/in.h> 
#include <netinet/ip.h>     /* superset of previous */
#include <sys/socket.h>     // man  3 inet_addr
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

// 可以写宏来替换下面的端口PORT和地址IP
#define SERVER_IP “0.0.0.0” // INADDR_ANY
#define SERVER_PORT ”6666“ 

#define SIZE 1024

int server_init(char *ip,short port, int backlog);
// 把 server 的初始化封装进这个函数
int server_init(char *ip,short port, int backlog)
{
    char receiveBuf[SIZE] = {0};    // 接收的字符串
    int listenFd;                   // 保存 socket 返回值 作为 监听套接字
    int connectFd;                  // 用于保存 accept 处理后的返回值 作为通信套接字
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //         创建套接字
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // PF_INET:IPV4 协议
    // SOCK_STREAM: TCP协议 流式套接字



    // 填充 ip 等信息
    // bind
    // listen


    // return listenfd;
}