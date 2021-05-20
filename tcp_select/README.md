# Questão 4 - Código TCP (Conversa entre clientes)

|Matrícula|Nome|
|--|--|
|15/0141220|Matheus de Cristo Estanislau|
|17/0044386|Renan Cristyan Araújo Pinheiro|

## Solução obtida
Para esta questão foram implementados dois códigos TCP, o lado do cliente e o lado do servidor.

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

Inicie o segundo cliente com:
``` ./client.o ```

O cliente 1 envia uma mensagem, recebe a resposta do servidor, e sua mensagem é exibida no terminal do cliente 2.
O cliente 2 recebe a mensagem do cliente 1 e responde, também recebe uma resposta do servidor.
O servidor fica observando os descritores de arquivo dos dois clientes utilizando a chamada de sistema select(), recebe a mensagem do primeiro que enviar e redireciona para o outro cliente. Ele fica em loop aguardando mensagens e conexões.
Caso os clientes enviem a mensagem exit, eles terão suas conexões desfeitas.

## Limitações
No estado atual do código, só é possível manter uma conversa de maneira organizada com apenas 2 clientes. Caso haja mais de 2 clientes, as mensagens chegam sem nenhuma ordem e pode ser difícil entender o fluxo da conversa.
Além disso, caso um dos clientes encerre de maneira inesperada (por exemplo, pressionando CTRL+C), o servidor entra em loop infinito e não consegue se recuperar, também não consegue receber mais nenhuma conexão. Este bug ainda segue sem correção.
