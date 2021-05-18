#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_MAX_SIZE 50
#define MAX_CLIENTS 4

struct sockaddr_in serverAddress, clientAddress;
int globalSocket = 0;

fd_set readfds;
int client_socket[MAX_CLIENTS];
int client_socket_size = 0;
int max = 0; // max socket
int sd = 0; // qualquer socket
int select_return = 0;

int setupServer(int port);
void waitForConnection(int socketFd);
void handleClientConnection(int clientFd);
void close_server();

/**
 * @brief Configurar servidor
 * @param port Porta do servidor
 * @return File descriptor do socket do servidor
 */
int setupServer(int port){

    signal(SIGINT, close_server);

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd == -1){
        printf("Criação do socket falhou\n");
        exit(1);
    }
    printf("Socket criado\n");
    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);

    if(bind(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0){
        perror("Bind falhou");
        exit(1);
    }
    printf("Bind realizado com sucesso\n");

    if((listen(socketFd, 5)) != 0){
        perror("Listen falhou");
        exit(1);
    }
    printf("Servidor escutando na porta %d\n", PORT);

    for(int i = 0; i < MAX_CLIENTS; i++) client_socket[i] = 0;

    return socketFd;
}

void close_server(){
    printf("\nFechando socket do servidor, fd = %d...\n", globalSocket);
    close(globalSocket);
    exit(0);
}

/**
 * @brief Aguarda por conexão
 * @param socketFd File descriptor do socket do servidor
 */
void waitForConnection(int socketFd){
    while(1){
        printf("Aguardando conexão...\n");

        FD_ZERO(&readfds);
        FD_SET(socketFd, &readfds);
        max = socketFd;

        for(int i = 0; i < MAX_CLIENTS; i++){
            sd = client_socket[i];
            
            if(sd > 0) FD_SET(sd, &readfds);
            if(sd > max) max = sd;
        }

        select_return = select(max+1, &readfds, NULL, NULL, NULL);
        if(select_return < 0) perror("Erro em select");

        socklen_t clientAddressSize = sizeof(clientAddress);

        if(FD_ISSET(socketFd, &readfds)){
            int clientFd = accept(socketFd, (struct sockaddr*)&clientAddress, &clientAddressSize);
            if(clientFd < 0){
                perror("Conexão ao servidor não aceita");
                exit(1);
            }
            printf("Nova conexão\n");
            printf("fd = %d, IP: %s, Porta: %u", clientFd, inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

            // adicionar ao array de sockets
            for(int i = 0; i < MAX_CLIENTS; i++){
                if(client_socket[i] == 0){
                    client_socket[i] = clientFd;
                    client_socket_size++;
                    printf("Adicionando no array de sockets com index %d\n", i);
                    printf("Tamanho do array de sockets: %d\n", client_socket_size);
                    break;
                }
            }
        }
        
        for(int i = 0; i < MAX_CLIENTS; i++){
            sd = client_socket[i];
            char client_msg[BUFFER_MAX_SIZE];
            bzero(client_msg, BUFFER_MAX_SIZE);

            if(FD_ISSET(sd, &readfds)){
                // enviar mensagem de volta
                int readBytes = read(sd, client_msg, sizeof(client_msg));
                printf("IP: %s, Porta: %u: %s", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), client_msg);
                
                // enviar para o cliente
                for(int i = 0; i < MAX_CLIENTS; i++){
                    if(client_socket[i] != 0){
                        send(client_socket[i], client_msg, strlen(client_msg), 0);
                    }
                }

                // send(sd, client_msg, strlen(client_msg), 0);

                if(strncmp("exit", client_msg, 4) == 0){
                    printf("Encerrando conexão com cliente...\n");
                    close(sd);
                    client_socket[i] = 0;
                    client_socket_size--;
                    break;
                }
            }
        }
    }
}

/**
 * @brief Trata conexão do cliente
 * @param clientFd File descriptor do socket do cliente
 */
void handleClientConnection(int clientFd){
    char client_msg[BUFFER_MAX_SIZE];

    while(1){
        bzero(client_msg, BUFFER_MAX_SIZE);
        int readBytes = read(clientFd, client_msg, sizeof(client_msg));
        printf("IP: %s, Porta: %u : %s\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), client_msg);

        write(clientFd, "mensagem recebida\0", 19);

        if(strncmp("exit", client_msg, 4) == 0){
            printf("Encerrando conexão com cliente...\n");
            break;
        }
    }
}

int main(){
    
    int socketFd = setupServer(PORT);
    globalSocket = socketFd;
    waitForConnection(socketFd);
    close(socketFd);

    return 0;
}