/*
 * @Description: selcet client dmeo
 * @Author: LeeChee
 * @Date: 2022-04-15 20:01:05
 * @LastEditTime: 2022-04-15 22:47:37
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/times.h>
#include <sys/select.h>


int main(int argc,char* argv[])
{
    int sock = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
    {
        printf("connect error %d %s",errno,strerror(errno));
        return 0;
    }
    char msg[128] = "";
    while(true)
    {
        printf("input: ");
        fgets(msg,sizeof(msg),stdin);
        if(!strcmp(msg,"q\n") || !strcmp(msg,"Q\n")) break;
        
            write(sock,msg,strlen(msg));
            memset(msg,0,sizeof(msg));
            ssize_t str_len = read(sock,msg,sizeof(msg));
            printf("server: %s\n",msg);
        
    }
    close(sock);
    return  0;
}