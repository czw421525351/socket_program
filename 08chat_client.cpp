//
// Created by 17723 on 2017/5/13.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define ERR_EXIT(m) \
		do \
		{	\
			perror(m); \
			exit(EXIT_FAILURE); \
		}while(0)

void handler(int sig)
{
    printf("recv a sig = %d\n",sig);
    printf("client father close\n");
    exit(EXIT_SUCCESS);
}
int main()
{
    int sockfd;
    if((sockfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
        //if((listenfd = socket(PF_INAT,SOCK_STREAM,0)) < 0)//0 系统自己选择协议
        ERR_EXIT("socket");

    struct sockaddr_in server;
    memset(&server,0,sizeof(server));
    server.sin_family = PF_INET;
    server.sin_port = htons(8881);
    inet_aton("113.54.243.33",&server.sin_addr);

    if(connect(sockfd,(struct sockaddr*)&server,sizeof(server)) < 0)
        ERR_EXIT("connect");

    pid_t pid;
    pid = fork();
    if(pid == -1)
        ERR_EXIT("fork");
    if(pid == 0)    //子进程接受数据
    {

        char recvbuf[1024];
        while(1)
        {
            memset(recvbuf,0,sizeof(recvbuf));
            int ret = read(sockfd,recvbuf,sizeof(recvbuf));
            if(ret == -1)
                ERR_EXIT("read");
            else if(ret == 0)
            {
                //ERR_EXIT("peer close");
                printf("peer close\n");
                break;
            }
            fputs(recvbuf,stdout);
        }
        kill(getppid(),SIGUSR1);
        printf("client child close\n");
        exit(EXIT_FAILURE);
    }
    else           //父进程发送数据
    {
        signal(SIGUSR1,handler);
        char sendbuf[1024] = {0};
        while(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)
        {
            write(sockfd,sendbuf,strlen(sendbuf));
            memset(sendbuf,0,sizeof(sendbuf));
        }
        printf("client father close\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}