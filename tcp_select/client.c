#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_MAX_SIZE 50

int socketFd;
struct sockaddr_in serverAddress, clientAddress;

/**
 * @brief Conectar ao servidor
 * @param address Endereço do servidor
 * @param port Porta do servidor
 */ 
void connectToServer(char* address, int port){
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd < 0){
        perror("Criação de socket falhou");
        exit(1);
    }
    printf("Socket criado com sucesso\n");
    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(address);
    serverAddress.sin_port = htons(port);

    if(connect(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0){
        perror("Conexão com o servidor falhou");
        exit(1);
    }

    printf("Conectado ao servidor\n");
}

void reconnect(int signal){
    signal = 0;
    interactWithServer(socketFd);
}

/**
 * @brief Envia string para o servidor e recebe uma resposta
 * @param clientFd File descriptor do cliente
 */
void interactWithServer(int clientFd){
    signal(SIGQUIT, reconnect);

    char msg_to_send[BUFFER_MAX_SIZE];
    char server_response[BUFFER_MAX_SIZE];

    int msg_sent = 0;

    fd_set readfds;
    int    num_readable;
    struct timeval tv;
    int    num_bytes;
    char   buf[BUFFER_MAX_SIZE];
    int    fd_stdin;

    fd_stdin = fileno(stdin);

    while(1){

        if(msg_sent != 0){
            bzero(server_response, BUFFER_MAX_SIZE);
            int readBytes = read(clientFd, server_response, sizeof(server_response));
            printf("Recebidos %d caracteres\n", readBytes);
            printf("Recebido do servidor: %s\n", server_response);

            bzero(server_response, BUFFER_MAX_SIZE);
            readBytes = read(clientFd, server_response, sizeof(server_response));
            printf("Recebidos %d caracteres\n", readBytes);
            printf("Alguém diz: %s\n", server_response);
        }

        FD_ZERO(&readfds);
        FD_SET(fileno(stdin), &readfds);
        FD_SET(clientFd, &readfds);

        tv.tv_sec = 10;
        tv.tv_usec = 0;

        fseek(stdin, 0, SEEK_END);
        bzero(msg_to_send, sizeof(msg_to_send));
        printf("> ");
        
        fflush(stdout);

        num_readable = select(fd_stdin + 2, &readfds, NULL, NULL, NULL);

        if (num_readable == -1) {
                fprintf(stderr, "Error in select : %s\n", strerror(errno));
                exit(1);
        }
        if (num_readable == 0) {
                printf("Listening mode (pressione CTRL + \\ para voltar a escrever)\n");
        } else {
                num_bytes = read(fd_stdin, msg_to_send, BUFFER_MAX_SIZE);
                if (num_bytes < 0) {
                        fprintf(stderr, "Error on read : %s\n", strerror(errno));
                        exit(1);
                }

                printf("Enviando: %s", msg_to_send);
                write(clientFd, msg_to_send, sizeof(msg_to_send));
                msg_sent = 1;
        }

        if(strncmp("exit", msg_to_send, 4) == 0){
            printf("Encerrando conexão com servidor...\n");
            break;
        }
    }
}

int main(){
    connectToServer("127.0.0.1", 8080);
    interactWithServer(socketFd);

    close(socketFd);

    return 0;
}