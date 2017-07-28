//
// Created by 17723 on 2017/5/12.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define ERR_EXIT(m) \
		do \
		{	\
			perror(m); \
			exit(EXIT_FAILURE); \
		}while(0)

int main()
{
    int sockfd;
    if((sockfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
        //if((listenfd = socket(PF_INAT,SOCK_STREAM,0)) < 0)//0 系统自己选择协议
        ERR_EXIT("socket");

    struct sockaddr_in server;
    memset(&server,0,sizeof(server));
    server.sin_family = PF_INET;
    server.sin_port = htons(8893);
    inet_aton("113.54.243.33",&server.sin_addr);

    if(connect(sockfd,(struct sockaddr*)&server,sizeof(server)) < 0)
        ERR_EXIT("connect");

    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};
    while(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)
    {
        write(sockfd,sendbuf,strlen(sendbuf));
        read(sockfd,recvbuf,sizeof(recvbuf));
        fputs(recvbuf,stdout);

        memset(sendbuf,0,sizeof(sendbuf));
        memset(recvbuf,0,sizeof(recvbuf));
    }
    close(sockfd);
    return 0;
}
