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

    int socketFd = socket(AF_INET, SOCK_DGRAM, 0);
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
    printf("Servidor escutando na porta %d\n", PORT);

    return socketFd;
}

int main(){
    char messageReceived[BUFFER_MAX_SIZE];
    char message[BUFFER_MAX_SIZE] = "Server Received Message\0";
    int socketFd = setupServer(PORT);
    struct sockaddr_in clentAddres;
    printf("Esperando mensagem\n");
    int readBytes = recvfrom(socketFd, (char *)messageReceived, BUFFER_MAX_SIZE, MSG_WAITALL, ( struct sockaddr *) &clentAddres, sizeof(clentAddres));
    printf("Mensagem recebida: %s\n", messageReceived);
    printf("\nEnviando mensagem: %s\n", message);
    sendto(socketFd, (char *)message, BUFFER_MAX_SIZE, 0, (const struct sockaddr *) &clentAddres, sizeof(clentAddres));
    printf("\nMensagem enviada\n");
    close(socketFd);

    return 0;
}