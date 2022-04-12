/*
 * @Description:  socket server demo1
 * @Author: L z
 * @Date: 2022-04-12 03:33:13
 * @LastEditTime: 2022-04-12 06:13:43
 */
//server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char const *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int cInt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cInt_addr;
    socklen_t cInt_addr_size;

    char message[]="BYE";
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

    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
    
    cInt_addr_size=sizeof(cInt_addr);
    cInt_sock=accept(serv_sock, (struct sockaddr*)&cInt_addr,&cInt_addr_size);
    if(cInt_sock==-1)
        error_handling("accept() error");
    
    write(cInt_sock, message, sizeof(message));
    close(cInt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char const *message)
{
    fputs(message, stderr);
    fputc('\n' , stderr);
    exit(1);
}



