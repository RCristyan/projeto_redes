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

struct sockaddr_in serverAddress, clientAddress;

/**
 * @brief Conectar ao servidor
 * @param address Endereço do servidor
 * @param port Porta do servidor
 */
int connectToServer(char *address, int port)
{
  int socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socketFd < 0)
  {
    perror("Criação de socket falhou");
    exit(1);
  }
  printf("Socket criado com sucesso\n");
  bzero(&serverAddress, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(address);
  serverAddress.sin_port = htons(port);

  return socketFd;
}

/**
 * @brief Envia string para o servidor e recebe uma resposta
 * @param clientFd File descriptor do cliente
 */
void interactWithServer(int clientFd)
{
  char msg_to_send[BUFFER_MAX_SIZE];
  char server_response[BUFFER_MAX_SIZE];

  fseek(stdin, 0, SEEK_END);
  bzero(msg_to_send, sizeof(msg_to_send));
  printf("Digite a mensagem a ser enviada para o servidor:\n> ");
  scanf("%[^\n]s", msg_to_send);

  getchar();

  printf("Enviando: %s\n", msg_to_send);
  sendto(clientFd, (char *)msg_to_send, BUFFER_MAX_SIZE, MSG_CONFIRM, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  printf("Aguardando mensagem...\n");
  // int readBytes = recvfrom(clientFd, (char *)server_response, BUFFER_MAX_SIZE, MSG_WAITALL, (struct sockaddr *)&serverAddress, (socklen_t *)sizeof(serverAddress));
  if(recvfrom(clientFd, (char *)server_response, BUFFER_MAX_SIZE, MSG_WAITALL, (struct sockaddr *)&serverAddress, (socklen_t *)sizeof(serverAddress)) < 0){
    perror("erro ao receber");
    exit(1);
  }
  printf("\nMensagem do Servidor: %s\n", server_response);
}

int main()
{
  int socket = connectToServer("127.0.0.1", 8080);
  interactWithServer(socket);
  close(socket);

  return 0;
}