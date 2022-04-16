/*
 * @Description: 多任务并发服务器
 * @Author: LeeChee
 * @Date: 2022-04-14 09:38:10
 * @LastEditTime: 2022-04-14 09:43:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char const *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
    int serv_sock, cint_sock;
    struct sockaddr_in serv_adr, cint_adr;
    pid_t pid;

    
}