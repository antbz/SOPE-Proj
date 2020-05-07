# SOPE-proj-2

Para correr o programa, execute o comando make na pasta de raíz do projeto.
Serão gerados dois executáveis, Q1 (servidor) e U1 (cliente) que aceitam os comandos
tal qual especificado no enunciado do projeto.

Deve ser executado o programa servidor em primeiro lugar, caso contrário, o programa
cliente reportará um erro e não executará.

## Detalhes de implementação

Inicializados ambos os programas, o programa cliente começará a lançar pedidos ao
servidor com um intervalo de **50ms** entre cada pedido. Cada pedido gerado tem uma
duração aleatória de entre **200 a 999ms**. Estas condições permitem simular um
cenário de grande competição para acesso aos recursos do servidor, tal como pedido.

O servidor aceita pedidos de qualquer duração até ao instante em que termina o seu
tempo de execução. Posteriormente, verifica se existe algum pedido que estivesse em
fila nesse instante. Se existir, envia ao respetivo cliente a informação de que o 
quarto de banho está encerrado.

## Registo de operações
Apresentam-se de seguida as situações que determinam um determinado
registo por parte dos programas na *stdout*.

### Cliente - U1
- **IWANT** - Cliente faz pedido inicial, escrevendo com sucesso a mensagem no fifo público
- **IAMIN** - Cliente acusa que recebeu resposta positiva do servidor
- **CLOSD** - Cliente acusa que recebeu resposta do servidor indicando que o quarto de banho está encerrado.
- **FAILD** - Cliente reporta que não foi possível establecer comunicação com o fifo público do servidor
ou que este demorou demasiado tempo a dar resposta no fifo privado.

### Servidor - Q1
- **RECVD** - Servidor acusa receção do pedido.
- **ENTER** - Servidor envia resposta ao cliente aceitando o pedido.
- **TIMUP** - Servidor reporta que tempo de utilização do cliente terminou.
- **2LATE** - Servidor envia resposta ao cliente dizendo que o quarto de banho está encerrado.
- **GAVUP** - Servidor acusa que não conseguiu enviar resposta ao cliente.
