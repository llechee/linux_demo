/*
 * @Description: 回声服务器client 
 * @Author: L z
 * @Date: 2022-04-13 05:25:40
 * @LastEditTime: 2022-04-13 05:30:15
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024 
#define RLT_SIZE 4 
#define OPSZ 4

void error_handling(char const *message);

int main(int argc, char* argv[])
{
    int sock;
    char opmsg[BUF_SIZE]; 
    int result, opnd_cnt, i;
    struct sockaddr_in serv_addr;
    char message[30];
    //int str_len;

    if(argc!=3){
        printf("Usage : %s <lP><port>\n" , argv[0]);
        exit(1);
    }
    sock=socket(PF_INET, SOCK_STREAM,0);
    if(sock == -1)
        error_handling( "socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error!");
    else printf("connecting...\n");
    fputs("Operand count: ", stdout); 
    scanf("%d", &opnd_cnt); 
    opmsg[0]=(char)opnd_cnt; 
    for(i=0; i<opnd_cnt; i++) 
    { 
        printf("Operand %d: ", i+1); 
        scanf("%d", (int*)&opmsg[i*OPSZ+1]); 
    }
        fgetc(stdin); 
        fputs("Operator: ", stdout); 
        scanf("%c", &opmsg[opnd_cnt*OPSZ+1]); 
        write(sock, opmsg, opnd_cnt*OPSZ+2); 
        read(sock, &result, RLT_SIZE); 
        printf("Operation result: %d \n", result); 
        close(sock);
    return 0;
}
void error_handling(char const *message)
{
    fputs(message, stderr);
    fputc('\n' , stderr);
    exit(1);
}