#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_MAX_SIZE 50

struct sockaddr_in serverAddress;

int setupServer(int port);
void waitForConnection(int socketFd);
void handleClientConnection(int clientFd);

/**
 * @brief Configurar servidor
 * @param port Porta do servidor
 * @return File descriptor do socket do servidor
 */
int setupServer(int port){

    int socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socketFd == -1){
        printf("Criação do socket falhou\n");
        exit(1);
    }
    printf("Socket criado\n");
    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);

    if(bind(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        perror("Bind falhou");
        exit(1);
    }
    printf("Bind realizado com sucesso\n");

    return socketFd;
}

int main(){
    int socketFd = setupServer(PORT);

    char messageReceived[BUFFER_MAX_SIZE];
    struct sockaddr_in clientAdress;
    
    socklen_t cAddressLen = sizeof(clientAdress);

    while(1){
        printf("Esperando mensagem\n");
        int readBytes = recvfrom(socketFd, messageReceived, BUFFER_MAX_SIZE, 0, ( struct sockaddr *) &clientAdress, &cAddressLen);

        printf("Received message from IP: %s and port: %i\n",
            inet_ntoa(clientAdress.sin_addr), ntohs(clientAdress.sin_port));
        printf("Mensagem recebida: %s\n", messageReceived);
        
        printf("Enviando mensagem: %s\n", messageReceived);

        int sentBytes = sendto(socketFd, messageReceived, BUFFER_MAX_SIZE, 0, (struct sockaddr *)&clientAdress, cAddressLen);
        printf("Mensagem enviada\n", sentBytes);

        bzero(messageReceived, BUFFER_MAX_SIZE);
    }

    close(socketFd);

    return 0;
}