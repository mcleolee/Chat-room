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

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//                        CLIENT
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#if 1

int tcp_client_init(char *ip,short port)
{

    //创建通信套结字 ：注意：客户端 没有监听，创建的套结字直接就是通信套结字

    
    // int     socket(int, int, int);
    // 
    int connectFd = socket(PF_INET, SOCK_STREAM, 0);
    if(-1 == connectFd)
    {
        perror("socket error\n");
        return -1;
    }

    //以下填充的是 要连接的服务器ip 和端口等信息
#if 1
        // struct sockaddr_in {
        // 	__uint8_t       sin_len;
        // 	sa_family_t     sin_family;
        // 	in_port_t       sin_port;
        // 	struct  in_addr sin_addr;
        // 	char            sin_zero[8];
        // };
    struct sockaddr_in server_address =
    {
        .sin_family         = PF_INET,
        .sin_addr.s_addr    = inet_addr(ip),
        .sin_port           = htons(port)
    };
#else // argv 没传进来，不用这个方法

    struct sockaddr_in serveraddr = {0};
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons( atoi(argv[2]) );
#endif
    // 连接服务器
    int len = sizeof(server_address);
    //主动连接服务器
    if(-1 == connect(connectFd, (struct sockaddr *)&server_address, len))
    {
        perror("connect error\n");
        return -1;
    }
    return connectFd;
}

int tcp_client_communication(int connectFd, char buf[])
{
//通信
    while(1)
    {
        int sizeof_buf = sizeof(buf);
        fgets(buf,sizeof_buf,stdin);
    
        write(connectFd, buf, strlen(buf));
        //sleep(1);
        read(connectFd, buf,sizeof_buf);
        
        printf("%s\n",buf);
        // write(STDOUT_FILENO, buf,strlen(buf)); //等同于 printf 输出到屏幕
    }
}





#endif






// client 用不到 server 的函数，很合理吧
#if 0
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//                        SERVER
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#if 1
// 把 server 的初始化封装进这个函数
int tcp_server_init(char *ip, short port, int backlog)
{
    int listenFd_init;
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //         创建套接字
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    // socket(int domain, int type, int protocol);
    // PF_INET          Internet version 4 protocols,
    // SOCK_STREAM      TCP协议 流式套接字
    // 0                0
    listenFd_init = socket(PF_INET,SOCK_STREAM,0);

    if(-1 == listenFd_init)
    {
        perror("socket");
        return -1;
    }
    printf("socket %d is ready\n", listenFd_init);
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    填充ip,端口,协议等信息
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#if 1 // 写法 1
    struct sockaddr_in server_address; // 用于保存服务器的 info （?）
    /*
    所以 sockaddr_in 是在头文件里定义好了的
    #include <netinet/in.h> 
    
    如下
    // * Socket address, internet style.

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
    server_address.sin_port         = htons(port);
    // #include <netinet/in.h> 
    /*
    * Internet address (a structure for historical reasons)

    struct in_addr {
        in_addr_t s_addr;
    };
    */
    // 转化 ip
    server_address.sin_addr.s_addr  = inet_addr(ip);
    /*
    tcpServer.c:198:49: 
    warning: 
    incompatible integer to pointer conversion passing 'char' to 
    parameter of type 'const char *'; remove * [-Wint-conversion]
    server_address.sin_addr.s_addr  = inet_addr(*ip);
                                                ^~~
    */


#else // 写法 2 ,cannot be compiled in c++, use C plz.
struct sockaddr_in ser_addr = 
    {
        .sin_family     = PF_INET,
        .sin_port       = htons(port), // SERV_PORT
        .sin_addr.s_addr= inet_addr(*ip) // SERV_IP
        //.sin_addr.s_addr = inet_addr(argv[1])   //用命令行参数  运行时 输入ip ： ./a.out  127.0.0.1 
    };

#endif

    // Optimize: 设置套接字重用
    int opt = 1; 
    int m = setsockopt(listenFd_init, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(-1 == m)
    {
        perror("setsockopt");
        return -1;
    }

    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //     绑定 IP 和端口等信息
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    // int     bind(int, const struct sockaddr *, socklen_t) __DARWIN_ALIAS(bind);
    int ret = bind(listenFd_init, (struct sockaddr*)&server_address, sizeof(server_address));
    if(-1 == ret)
    {
        perror("bind");
        return -1;
    }
    printf("bind is OK\n");

    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //           建立监听
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    // int     listen(int, int) __DARWIN_ALIAS(listen);
    ret = listen(listenFd_init, 8);
    if(-1 == ret)
    {
        perror("listen");
        return -1;
    }
    printf("Waiting for the client to connect...\n"); 

    return listenFd_init;
}

        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        //           正常通信
        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
int tcp_server_communication(int connectFd)
{
    while(1)
    {
        int count = -1;
        char receiveBuf[SIZE] = {0};
        memset(receiveBuf, 0, sizeof(receiveBuf));
        // read(int fd, void *buf, size_t count);
        /*
        | fd    |   要操作的文件对应的文件描述符   |
        | buf   |   指向保存内容空间的指针        |
        | count |   要读取的文件内容字节数        |
        */
        count = read(connectFd, receiveBuf, sizeof(receiveBuf));
        if (-1 == count)
        {
            perror("read");
            return -1;
        }
        else if (0 == count)
        {
            printf("client had quit!\n");
            break;
        }
        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        //            正常通信
        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        printf("receive:%s\n", receiveBuf);

        if(strncmp(receiveBuf, "sl", 2) == 0)
        {
            system("sl");
        }
        else if(strncmp(receiveBuf, "wtf", 3) == 0)
        {
            tcp_print("wtf???\n");
        }
        
        // 将接收到的数据转换成大写
            for(int i=0;i<count; i++)
            {
                // 将 recvbuf 里面的字母逐个变为大写再送回 recvbuf
                receiveBuf[i] = toupper(receiveBuf[i]);
            }
            write(connectFd, receiveBuf, count);

    }
    close(connectFd);
    return 0;
}


int tcp_print(char *s)
{
    printf("%s",s);
    return 0;
}

#endif

#endif