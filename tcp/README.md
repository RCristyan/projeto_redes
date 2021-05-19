# Questão 2 - Código TCP

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

O cliente envia uma mensagem, aguarda a resposta e pode enviar outra, e o processo continua em loop. Caso queira finalizar a conexão, ele deve enviar a mensagem exit.
O servidor recebe a mensagem do cliente, responde e continua em loop, aguardando mais mensgens. Caso receba a mensagem exit, ele fecha a conexão com o cliente e aguarda por uma nova.

## Limitações
Com este código fornecido, não é possível o tratamento simultâneo de mais de 2 clientes. Ou seja, assim que um cliente se conecta ao servidor, quaisquer novos cliente ficarão em espera até que a conexão seja encerrada.
