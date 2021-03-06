//
// Created by 17723 on 2017/5/13.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
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
    printf("server client close\n");
    exit(EXIT_SUCCESS);
}

int main()
{
    int listenfd;
    if((listenfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
        //if((listenfd = socket(PF_INAT,SOCK_STREAM,0)) < 0)//0 系统自己选择协议
        ERR_EXIT("socket");

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8881);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //serveraddr.sin_addr.s_addr = inet_addr("192.168.1.58");
    //inet_aton("192.168.1.58",&serveraddr.sin_addr);

    {
        int on = 1;
        if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
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
    pid_t pid;
    pid = fork();
    if(pid == -1)
        ERR_EXIT("fork");
    if(pid == 0)	//子进程发送数据
    {
        signal(SIGUSR1,handler);//注册一个信号

        char sendbuf[1024] = {0};
        while(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)
        {
            write(connfd,sendbuf,strlen(sendbuf));
            memset(sendbuf,0,sizeof(sendbuf));
        }
        printf("server client close\n");
        exit(EXIT_FAILURE);
    }
    else			//父进程接受数据
    {
        char recvbuf[1024];
        while(1)
        {
            memset(recvbuf,0,sizeof(recvbuf));
            int ret = read(connfd, recvbuf , sizeof(recvbuf));
            if(ret == 0)
            {
                printf("peer close\n");
                break;
            }
            else if(ret == -1)
                ERR_EXIT("read");
            fputs(recvbuf,stdout);
        }
        printf("server father close\n");
        kill(pid,SIGUSR1);//发送信号
        exit(EXIT_FAILURE);
    }


    return 0;
}
