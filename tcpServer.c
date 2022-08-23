#include <stdio.h>
#include <string.h>
#include <sys/socket.h>     //man 3 socket 
#include <sys/socket.h>     //man 7 ip 
#include <netinet/in.h> 
#include <netinet/ip.h>     /* superset of previous */
#include <sys/socket.h>     // man  3 inet_addr
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

    // socket(int domain, int type, int protocol);
    // PF_INET          Internet version 4 protocols,
    // SOCK_STREAM      TCP协议 流式套接字
    // 0                0
    listenFd = socket(PF_INET,SOCK_STREAM,0);

    if(-1 == listenFd)
    {
        perror("socket");
        return -1;
    }
    printf("socket %d is ready\n", listenFd);
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    填充ip,端口,协议等信息
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#if 0 // 写法 1, cannot be compiled in c++
    struct sockaddr_in server_address; // 用于保存服务器的 info （?）
    /*
    所以 sockaddr_in 是在头文件里定义好了的
    #include <netinet/in.h> 
    
    如下
    //* Socket address, internet style.

    struct sockaddr_in {
        __uint8_t       sin_len;
        sa_family_t     sin_family;
        in_port_t       sin_port;
        struct  in_addr sin_addr;
        char            sin_zero[8];
    };

    */

    memset(&server_address, 0, sizeof(server_address));
    // IPV4 
    // socket.h
    // #define PF_INET         AF_INET
    server_address.sin_family       = PF_INET;
    // _endian.h
    // #define htons(x)        __DARWIN_OSSwapInt16(x)
    // 端口号 1024-49151
    server_address.sin_port         = htons(6666);
    // #include <netinet/in.h> 
    /*
    * Internet address (a structure for historical reasons)

    struct in_addr {
        in_addr_t s_addr;
    };
    */
    // 转化 ip
    server_address.sin_addr.s_addr  = inet_addr("127.0.0.1");


#else // 写法 2
struct sockaddr_in ser_addr = 
    {
        .sin_family     = PF_INET,
        .sin_port       = htons(6666), // SERV_PORT
        // .cpp 文件会报错，建议创建.c文件
        .sin_addr.s_addr= inet_addr("127.0.0.1") // SERV_IP
        //.sin_addr.s_addr = inet_addr(argv[1])   //用命令行参数  运行时 输入ip ： ./a.out  127.0.0.1 
    };




#endif

    // 填充 ip 等信息
    // bind
    // listen


    // return listenfd;
}