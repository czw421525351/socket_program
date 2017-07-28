//
// Created by 17723 on 2017/5/12.
//


#include <stdio.h>
#include <arpa/inet.h>

int main(void)
{
    unsigned int x = 0x12345678;
    unsigned char *p = (unsigned char*)&x;

    printf("0x%x 0x%x 0x%x 0x%x\n",p[0],p[1],p[2],p[3]);

    uint32_t y = 0;
    y = htonl(x);
    p = (unsigned char*)&y;

    printf("0x%x 0x%x 0x%x 0x%x\n",p[0],p[1],p[2],p[3]);
    return 0;
}