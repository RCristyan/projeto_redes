#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

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
    // serverAddress.sin_addr.s_addr = inet_addr("127.0.1.1");
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

    while(1){
        fseek(stdin, 0, SEEK_END);
        bzero(msg_to_send, sizeof(msg_to_send));
        printf("Digite a mensagem a ser enviada para o servidor:\n> ");
        scanf("%[^\n]s", msg_to_send);

        // descartar '/n' que sobrou
        getchar();

        printf("Enviando: %s\n", msg_to_send);
        write(clientFd, msg_to_send, sizeof(msg_to_send));

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