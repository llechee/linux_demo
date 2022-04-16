/*
 * @Description: select demo server 
 * @Author: LeeChee
 * @Date: 2022-04-15 19:58:15
 * @LastEditTime: 2022-04-15 22:54:40
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
    int serv_sock,cint_sock=-1;
    struct sockaddr_in serv_adr,cint_adr;
    socklen_t cint_adr_sz = sizeof(cint_adr);
    serv_sock = socket(PF_INET , SOCK_STREAM,0);
    memset(&serv_adr , 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;//xieyi
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);//inet_addr("0.0.0.0")
    serv_adr.sin_port=htons(atoi(argv[1]));
    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
    {
        printf("bind error %d %s",errno,strerror(errno));
        return 0;
    }
    if(listen(serv_sock , 10) == -1)
    {
        printf("listen error %d %s",errno,strerror(errno));
        return 0;
    }
    fd_set reads,reads_cp;
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    timeval timeout = {0,500000}; //0.5s
    int max_sock = serv_sock;
    while(true)
    {
        reads_cp = reads;
        int fd_num = select(max_sock+1 , &reads_cp , NULL ,NULL,&timeout);
        if(fd_num == -1) 
        {
            printf("select error %d %s",errno,strerror(errno));
            close(serv_sock);
            return 0;
        }
        if(fd_num == 0) continue;
        for(int i=0;i< max_sock+1; i++)
        {
            if(FD_ISSET(i,&reads_cp))
            {
                if(i == serv_sock) // 说明有客户端连接进来了 服务器的套介子
                {
                    cint_sock = accept(serv_sock,(sockaddr*)&cint_adr,&cint_adr_sz);
                    FD_SET(cint_sock,&reads); // 添加
                    if(max_sock < cint_sock) max_sock = cint_sock;
                    printf("client connect : %d\n",cint_sock);
                }
                else
                {
                    char buffer[256] = "";
                    ssize_t str_len = read(i,buffer,sizeof(buffer));
                    if(str_len == 0) // 客户端中断
                    {
                        FD_CLR(i,&reads);
                        close(i);
                        printf("client disconnect : %d\n",i);
                    }
                    else
                    {
                        write(i,buffer,str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
    return  0;
}
