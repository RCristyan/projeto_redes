#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_MAX_SIZE 50

struct sockaddr_in serverAddress, clientAddress;

int setupServer(int port);
void waitForConnection(int socketFd);
void handleClientConnection(int clientFd);

/**
 * @brief Configurar servidor
 * @param port Porta do servidor
 * @return File descriptor do socket do servidor
 */
int setupServer(int port){

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

    return socketFd;
}

void childStuff(int clientFd){
    printf("em child stuff\n");
    printf("Servidor aceitou conexão do cliente\n");

    handleClientConnection(clientFd);  
}

/**
 * @brief Aguarda por conexão
 * @param socketFd File descriptor do socket do servidor
 */
void waitForConnection(int socketFd){
    while(1){
        printf("Aguardando conexão...\n");

        socklen_t clientAddressSize = sizeof(clientAddress);
        
        int clientFd = accept(socketFd, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if(clientFd < 0){
            perror("Conexão ao servidor não aceita");
            exit(1);
        }
        else{

            pid_t child;
            child = fork();

            if(child == 0){
                printf("Servidor aceitou conexão do cliente\n");

                handleClientConnection(clientFd);  
            }
            else continue;
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
        printf("IP: %s, Porta: %u : %s\n",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), client_msg);

        write(clientFd, "mensagem recebida\0", 19);

        if(strncmp("exit", client_msg, 4) == 0){
            printf("Encerrando conexão com cliente...\n");
            break;
        }
    }
}

int main(){

    int socketFd = setupServer(PORT);
    waitForConnection(socketFd);
    close(socketFd);

    return 0;
}