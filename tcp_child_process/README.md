# Questão 3 - Código TCP (Suporte para 2 ou mais clientes)

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
A diferença deste código para o anterior é que este é capaz de tratar mais de 1 cliente simultaneamente. Isso foi possível graças ao uso da chamada de sistema fork().
Após aceitar uma conexão, o processo principal do servidor não a trata diretamente, mas instancia um processo filho que trata a conexão, enquanto ele volta a escutar novas conexões. Ou seja, para cada nova conexão, um novo processo é criado pra tratá-la.

## Limitações
O servidor irá imprimir mensagens de maneira inexperada caso o cliente encerre a conexão de forma anormal (por exemplo, pressionando CTRL+C). Entretanto, ele ainda irá esperar por novas conexões, o bug é apenas visual, sem impactar no funcionamento do código do servidor.
