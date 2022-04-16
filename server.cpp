/*
 * @Description: 多线程并发服务器demo
 * @Author: LeeChee
 * @Date: 2022-04-14 09:45:14
 * @LastEditTime: 2022-04-14 21:29:06
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
#include <pthread.h>

int cint_socks[100]{0};
int cint_cnt{0};
pthread_mutex_t mutex1;

void send_msg(char* const msg,ssize_t str_len)
{
    pthread_mutex_lock(&mutex1);
    for(int i=0;i<cint_cnt;i++)
    {
        
        if(cint_socks[i]>=0)
            write(cint_socks[i],msg,str_len);
        
    }
    pthread_mutex_unlock(&mutex1);
}
void* handle_cint(void* arg)
{
    pthread_detach(pthread_self());
    int cint_sock = *(int*)arg;
    char msg[1024] = "";
    ssize_t str_len = 0;
    while((str_len = read(cint_sock,msg,sizeof(msg))) > 0)
        send_msg(msg,str_len);
    pthread_mutex_lock(&mutex1);
    // for(int i=0;i<cint_sock;i++)
    // {
    //     if(cint_sock == cint_socks[i])
    //     {
            
    //         cint_socks[i] = -1;
    //         close(cint_sock);
    //         break;
    //     }
    // }
    *(int*)arg = -1;
    pthread_mutex_unlock(&mutex1);
    close(cint_sock);
    pthread_exit(NULL);
}

int main(int argc , char* argv[])
{
    int serv_sock,cint_sock;
    struct sockaddr_in serv_adr,cint_adr;
    socklen_t cint_adr_sz = sizeof(cint_adr);
    serv_sock = socket(PF_INET , SOCK_STREAM,0);
    memset(&serv_adr , 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;//xieyi
    serv_adr.sin_addr.s_addr=inet_addr("0.0.0.0"); //htonl(INADDR_ANY);//inet_addr("0.0.0.0")
    serv_adr.sin_port=htons(atoi(argv[1]));
    pthread_mutex_init(&mutex1 , NULL);
    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
    {
        printf("bind error %d %s",errno,strerror(errno));
        return 0;
    }
    if(listen(serv_sock , 10) == -1)
    {
        printf("bind error %d %s",errno,strerror(errno));
        return 0;
    }
    while(1)
    {
        cint_sock = accept(serv_sock , (struct sockaddr*)&cint_adr , &cint_adr_sz);
        if(cint_sock == -1)
        {
            printf("bind error %d %s",errno,strerror(errno));
            break;
        }
        pthread_mutex_lock(&mutex1);
        cint_socks[cint_cnt++] = cint_sock;
        pthread_mutex_unlock(&mutex1);
        pthread_t thread;
        pthread_create(&thread , NULL , handle_cint , &cint_socks[cint_cnt - 1]);
        //pthread_mutex_unlock(&mutex1);
    }
    close(serv_sock);
    pthread_mutex_destroy(&mutex1);
    return 0;
}

