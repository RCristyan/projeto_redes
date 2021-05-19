# Questão 1 - Código UDP

|Matrícula|Nome|
|--|--|
|15/0141220|Matheus de Cristo Estanislau|
|17/0044386|Renan Cristyan Araújo Pinheiro|

## Solução obtida
Para esta questão foram implementados dois códigos UDP, o lado do cliente e o lado do servidor.

O servidor se conecta no IP 127.0.0.1 na porta 8080.
O cliente faz a conexão na mesma porta.

Para executar o código, primeiramente compile o código do servidor:
``` gcc server.c -o server.o ```

Logo após, inicie o servidor com:
``` ./server.o ```

Com o servidor em execução, compile o código do cliente com:
``` gcc client.c -o client.o ```

E inicie o cliente com:
``` ./client.o ```

O cliente envia uma única mensagem, recebe a resposta e encerra a execução.
O servidor recebe a mensagem do cliente, responde mas continua em loop, aguardando mais mensgens.
