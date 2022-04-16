/*
 * @Description: client demo
 * @Author: LeeChee
 * @Date: 2022-04-14 10:10:57
 * @LastEditTime: 2022-04-14 21:31:11
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
#include <semaphore.h>

sem_t semid;
char name[64]="[DEFAULT]";

void* client_send_msg(void* arg)
{
    pthread_detach(pthread_self());
    int sock = *(int*)arg;
    char msg[256] = "";
    char buffer[1024];
    while(true)
    {
        memset(buffer,0,sizeof(buffer));
        fgets(msg,sizeof(msg),stdin);
        if((strcmp(msg,"q\n") == 0) || (strcmp(msg,"Q\n") == 0)) break;
        snprintf(buffer,sizeof(msg),"%s %s",name,msg);
        write(sock,buffer,strlen(buffer));
    }
    sem_post(&semid);
    pthread_exit(NULL);
}
void* client_recv_msg(void* arg)
{
    pthread_detach(pthread_self());
    int sock = *(int*)arg;
    char msg[256] = "";
    while(true)
    {
        ssize_t str_len = read(sock,msg,sizeof(msg));
        if(str_len <= 0) break;
        fputs(msg,stdout);
        memset(msg,0,str_len);
    }
    sem_post(&semid);
    pthread_exit(NULL);
}

int main(int argc , char* argv[])
{
    int sock = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));
    fputs("input:",stdout);
    scanf("%s",name);
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
    {
        printf("bind error %d %s",errno,strerror(errno));
        return 0;
    }

    pthread_t thsend,threcv;
    sem_init(&semid,0,-1);
    pthread_create(&thsend,NULL,client_send_msg,(void*)&sock);
    pthread_create(&threcv,NULL,client_recv_msg,(void*)&sock);
    sem_wait(&semid);
    close(sock);
    return 0;
}

