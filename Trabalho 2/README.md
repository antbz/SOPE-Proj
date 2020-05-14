# SOPE-proj-2

Para correr o programa, execute o comando `make` na pasta de raíz do projeto.
Serão gerados dois executáveis, Q2 (servidor) e U2 (cliente) que aceitam os comandos
tal qual especificado no enunciado do projeto.

**Q2:** ` ./Q2 <-t nsecs> [-l nplaces] [-n nthreads] fifoname `

**U2:** ` ./U2 <-t nsecs> fifoname `

Os programas podem ser executados por qualquer ordem.

## Detalhes de implementação

Inicializados ambos os programas, o programa cliente começará a lançar pedidos ao
servidor com um intervalo de **50ms** entre cada pedido. Cada pedido gerado tem uma
duração aleatória de entre **200 a 999ms**. Estas condições permitem simular um
cenário de grande competição para acesso aos recursos do servidor, tal como pedido.

O servidor aceita pedidos de qualquer duração até ao instante em que termina o seu
tempo de execução. Posteriormente, verifica se existem pedidos em fila nesse instante.
Se existirem, envia aos respetivos clientes a informação de que o 
quarto de banho está encerrado.

Por defeito (ou seja, não especficando a opção `-n nthreads`) o programa do servidor
limita o número máximo de threads ativas a atender pedidos a **300 threads**. Este
limite tem como objetivo evitar que se possa atingir o limite de threads do sistema.
Há, no entanto, a opção de remover este limite executando o programa com a opção
`-n 0`.

Quando é especificada a opção `-l nplaces`, a alocação de um número de quarto de 
banho a um pedido é feita recorrendo a uma fila, na lógica de que o primeiro quarto
de banho a ser libertado será o primeiro a ser novamente ocupado.

## Registo de operações
Apresentam-se de seguida as situações que determinam um determinado
registo por parte dos programas na *stdout*.

### Cliente - U2
- **IWANT** - Cliente faz pedido inicial, escrevendo com sucesso a mensagem no fifo público
- **IAMIN** - Cliente acusa que recebeu resposta positiva do servidor
- **CLOSD** - Cliente acusa que recebeu resposta do servidor indicando que o quarto de banho está encerrado.
- **FAILD** - Cliente reporta que não foi possível obter resposta do servidor.

### Servidor - Q2
- **RECVD** - Servidor acusa receção do pedido.
- **ENTER** - Servidor envia resposta ao cliente aceitando o pedido.
- **TIMUP** - Servidor reporta que tempo de utilização do cliente terminou.
- **2LATE** - Servidor envia resposta ao cliente dizendo que o quarto de banho está encerrado.
- **GAVUP** - Servidor acusa que não conseguiu enviar resposta ao cliente.
