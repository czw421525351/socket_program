//
// Created by 17723 on 2017/5/12.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define ERR_EXIT(m) \
		do \
		{	\
			perror(m); \
			exit(EXIT_FAILURE); \
		}while(0)

int main()
{
    int listenfd;
    if((listenfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
        //if((listenfd = socket(PF_INAT,SOCK_STREAM,0)) < 0)//0 系统自己选择协议
        ERR_EXIT("socket");

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8893);

    //三种方式
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//本机的任意地址
    //serveraddr.sin_addr.s_addr = inet_addr("192.168.18.16");
    //inet_aton("192.168.1.58",&serveraddr.sin_addr);

    {
        int on = 1;
        if(0 > setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
            ERR_EXIT("setsockopt");
    }

    if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
        ERR_EXIT("bind");

    if(listen(listenfd,SOMAXCONN) < 0)
        ERR_EXIT("listen");

    struct sockaddr_in clientaddr;
    memset(&clientaddr,0,sizeof(clientaddr));
    socklen_t clientlen = sizeof(clientaddr);
    int connfd;
    if((connfd = accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen)) < 0)
        ERR_EXIT("accept");

    printf("client ip = %s , port = %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

    char recvbuf[1024];
    while(1)
    {
        memset(recvbuf,0,sizeof(recvbuf));

        int ret = read(connfd, recvbuf , sizeof(recvbuf));
        fputs(recvbuf,stdout);

        write(connfd,recvbuf,ret);
    }
    close(connfd);
    close(listenfd);
    return 0;
}



