#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

/**
 * @brief Envia string para o servidor e recebe uma resposta
 * @param clientFd File descriptor do cliente
 */
void interactWithServer(int clientFd){
    char msg_to_send[BUFFER_MAX_SIZE];
    char server_response[BUFFER_MAX_SIZE];

    fd_set readfds;
    int    num_readable;
    struct timeval tv;
    int    num_bytes;
    char   buf[BUFFER_MAX_SIZE];
    int    fd_stdin;

    fd_stdin = fileno(stdin);

    while(1){
        FD_ZERO(&readfds);
        FD_SET(fileno(stdin), &readfds);

        tv.tv_sec = 10;
        tv.tv_usec = 0;

        fseek(stdin, 0, SEEK_END);
        bzero(msg_to_send, sizeof(msg_to_send));
        printf("Digite a mensagem a ser enviada para o servidor:\n> ");
        
        fflush(stdout);
        num_readable = select(fd_stdin + 1, &readfds, NULL, NULL, &tv);
        // scanf("%[^\n]s", msg_to_send);

        if (num_readable == -1) {
                fprintf(stderr, "\nError in select : %s\n", strerror(errno));
                exit(1);
        }
        if (num_readable == 0) {
                printf("\nPerforming default action\n");
                bzero(server_response, BUFFER_MAX_SIZE);
                int readBytes = read(clientFd, server_response, sizeof(server_response));
                printf("Recebidos %d caracteres\n", readBytes);
                printf("Recebido do servidor: %s\n", server_response);
                continue;  /* since I don't want to test forever */
        } else {
                num_bytes = read(fd_stdin, msg_to_send, BUFFER_MAX_SIZE);
                if (num_bytes < 0) {
                        fprintf(stderr, "\nError on read : %s\n", strerror(errno));
                        exit(1);
                }
                /* process command, maybe by sscanf */
                // printf("\nRead %d bytes\n", num_bytes);
                // break; /* to terminate loop, since I don't process anything */

                printf("Enviando: %s\n", msg_to_send);
                write(clientFd, msg_to_send, sizeof(msg_to_send));

                // bzero(server_response, BUFFER_MAX_SIZE);
                // int readBytes = read(clientFd, server_response, sizeof(server_response));
                // printf("Recebidos %d caracteres\n", readBytes);
                // printf("Recebido do servidor: %s\n", server_response);
        }

        // printf("Enviando: %s\n", msg_to_send);
        // write(clientFd, msg_to_send, sizeof(msg_to_send));

        bzero(server_response, BUFFER_MAX_SIZE);
        int readBytes = read(clientFd, server_response, sizeof(server_response));
        printf("Recebidos %d caracteres\n", readBytes);
        printf("Recebido do servidor: %s\n", server_response);

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