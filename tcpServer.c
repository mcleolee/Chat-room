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

// 可以写宏来替换下面的端口PORT和地址IP

int main()
{
    // char receiveBuf[SIZE] = {0};    // 接收的字符串 !moved to tcp_communication!
    int connectFd;                  // 用于保存 accept 处理后的返回值 作为通信套接字
    int listenFd;                   // 保存 socket 返回值 作为 监听套接字 

    char ip_test[50] = {"127.0.0.1"};
    short port_test = 6666;

    // tcp_server_init(ip_test, port_test, 8);
    listenFd = tcp_server_init(ip_test, port_test, 8);

    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //      处理客户端连接请求
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // 相当于前台
    // 当 accept 处理之后,监听套接字转接通信套接字
    // 优化 1
    // 客户端退出，服务器继续保持监听
    // 加了一个 while(1) 循环

    while (1)
    {
#if 0   // 不关心客户端的 IP 和端口
        // int     accept(int, struct sockaddr * __restrict, socklen_t * __restrict) __DARWIN_ALIAS_C(accept);
        connectFd = accept(listenFd, NULL, NULL);

#else   // Optimize: 关心客户端的 IP 和端口
        struct sockaddr_in client_address; // 定义一个结构体变量来保存 client 的ip等信息
        memset(&client_address, 0, sizeof(client_address));

        // typedef __darwin_socklen_t      socklen_t;
        // typedef __uint32_t              __darwin_socklen_t;     /* socklen_t (duh) */
        // typedef unsigned int            __uint32_t;
        socklen_t client_length = sizeof(client_address);
        connectFd = accept(listenFd, (struct sockaddr *)&client_address, &client_length);

        if(-1 == connectFd)
        {
            perror("accept");
            return -1;
        }
        // char		*inet_ntoa(struct in_addr);
        // #define ntohs(x)        __DARWIN_OSSwapInt16(x)
            // #define __DARWIN_OSSwapInt16(x) \
    ((__uint16_t)(__builtin_constant_p(x) ? __DARWIN_OSSwapConstInt16(x) : _OSSwapInt16(x)))

        printf("客户端(%s:%d)链接成功！\n",inet_ntoa(client_address.sin_addr/*这有问题？*/),ntohs(client_address.sin_port));
        
#endif
        printf("connectFd = %d\n", connectFd);  

        // 模拟发起客户端：nc 127.0.0.1 6666

        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        //           正常通信
        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        tcp_communication(connectFd);
    
        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        //          关闭套接字
        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        close(listenFd);
        return 0;

    }
}

int tcp_communication(int connectFd)
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

