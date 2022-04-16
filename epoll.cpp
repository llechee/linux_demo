/*
 * @Description: epoll demo server
 * @Author: LeeChee
 * @Date: 2022-04-15 23:28:42
 * @LastEditTime: 2022-04-16 07:24:03
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
#include <sys/epoll.h>
#include <fcntl.h>

int main(int argc,char* argv[])
{
    int serv_sock,cint_sock=-1;
    struct sockaddr_in serv_adr,cint_adr;
    
    char buffer[1024]="";
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
        close(serv_sock);
        return 0;
    }
    epoll_event event;
    int epfd,event_cnt;
    if((epfd = epoll_create(2048)) == -1 )
    {
        close(serv_sock);
        return 0;
    }
    //epoll shijian
    epoll_event* all_events = new epoll_event[128]; //create 128 ge 事件
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd,EPOLL_CTL_ADD,serv_sock,&event);
    while(true)
    {
        event_cnt = epoll_wait(epfd,all_events,128,1000); // 等1s
        if(event_cnt == -1)
        {
            printf("bind error %d %s",errno,strerror(errno));
            return 0;
        }
        if(event_cnt==0) continue;
        for(int i=0;i<event_cnt;i++)
        {
            if(all_events[i].data.fd == serv_sock)
            {
                socklen_t cint_adr_sz = sizeof(cint_adr);
                cint_sock = accept(serv_sock,(sockaddr*)&cint_adr,&cint_adr_sz);
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = cint_sock;
                // int flag = fcntl(cint_sock,F_GETFL,0);
                // fcntl(cint_sock,F_SETFL,flag | O_NONBLOCK);
                epoll_ctl(epfd,EPOLL_CTL_ADD,cint_sock,&event);
                printf("clients connect! %d \n",cint_sock);
            }
            else 
            {
                ssize_t len = read(all_events[i].data.fd ,buffer, sizeof(buffer));
                if(len <= 0)
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL,all_events[i].data.fd,NULL);
                    close(all_events[i].data.fd);
                    printf("client is close %d/n" , all_events[i].data.fd);
                }
                else
                {
                    write(all_events[i].data.fd,buffer,len);
                }           
            }
        }
    }

    delete[]all_events;
    close(serv_sock);
    close(epfd);
    return 0;
}