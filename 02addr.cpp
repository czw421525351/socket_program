//
// Created by 17723 on 2017/5/12.
//


#include <stdio.h>
#include <arpa/inet.h>

int main(void)
{
    unsigned int addr = inet_addr("113.54.237.86");//同上
    printf("addr = %u \n",ntohl(addr));

    struct in_addr ipaddr;
    ipaddr.s_addr = addr;
    printf("%s\n",inet_ntoa(ipaddr));//相反转化

    return 0;
}