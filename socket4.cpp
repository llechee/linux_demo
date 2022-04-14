/*
 * @Description: 回声 demo server 
 * @Author: L z
 * @Date: 2022-04-13 05:30:32
 * @LastEditTime: 2022-04-13 07:40:21
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024 
#define OPSZ 4
void error_handling(char const *message);
int calculate(int opnum, int opnds[], char oprator);
int main(int argc, char *argv[])
{
    int serv_sock;
    int cInt_sock;
    char opinfo[BUF_SIZE]; 
    int result, opnd_cnt, i;
    int recv_cnt, recv_len;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cInt_addr;
    socklen_t cInt_addr_size;

    //char message[]="BYE";
    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);exit(1);
    }
    serv_sock=socket(PF_INET, SOCK_STREAM,0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;//xieyi
    serv_addr.sin_addr.s_addr=inet_addr("0.0.0.0"); //htonl(INADDR_ANY);//inet_addr("0.0.0.0")
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 ) 
        error_handling("bind() error");

    if(listen(serv_sock, 10)==-1)
        error_handling("listen() error");
    
    cInt_addr_size=sizeof(cInt_addr);
    
    for(i=0; i<5; i++) 
    { 
        opnd_cnt=0; 
        cInt_sock=accept(serv_sock, (struct sockaddr*)&cInt_addr, &cInt_addr_size); 
        read(cInt_sock, &opnd_cnt, 1); recv_len=0; 
        while((opnd_cnt*OPSZ+1)>recv_len) 
        { 
            recv_cnt=read(cInt_sock, &opinfo[recv_len], BUF_SIZE-1); 
            recv_len+=recv_cnt; 
        }
        result=calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len-1]); 
        write(cInt_sock, (char*)&result, sizeof(result)); 
        close(cInt_sock); 
    }
    close(serv_sock);
    return 0;
}

int calculate(int opnum, int opnds[], char op) 
{ 
    int result=opnds[0], i; 
    switch(op) 
    {
    case '+': 
        for(i=1; i<opnum; i++) result+=opnds[i]; 
        break; 
    case '-': 
        for(i=1; i<opnum; i++) result-=opnds[i]; 
        break; 
    case '*': 
        for(i=1; i<opnum; i++) result*=opnds[i]; 
        break; 
        }
    return result; 
}

void error_handling(char const *message)
{
    fputs(message, stderr);
    fputc('\n' , stderr);
    exit(1);
}
