#include <winsock2.h>  // Biblioteca para estruturas e funções de sockets no domínio de rede
#include <stdio.h>       // Biblioteca padrão de entrada/saída
#include <stdlib.h>      // Biblioteca padrão para funções utilitárias como exit()
#include <string.h>      // Biblioteca para manipulação de strings
#include <sys/socket.h>  // Biblioteca para funções relacionadas a sockets
#include <unistd.h>      // Biblioteca para funções POSIX, como close e shutdown

int main(int argc, char const* argv[])
{
    // Variáveis para o socket do servidor, nova conexão, leitura de dados, e endereço
    int server_fd, new_socket, valread;
    struct sockaddr_in address; // Estrutura que armazenará o endereço do servidor
    int opt = 1; // Opção para reutilização de endereço e porta
    int addrlen = sizeof(address); // Tamanho da estrutura de endereço
    char buffer[1024] = { 0 }; // Buffer para armazenar a mensagem recebida do cliente
    char hello[1024]; // Buffer para armazenar a mensagem de resposta do servidor
    strcpy(hello, "Hello from server"); // Inicializa a mensagem de resposta com um texto fixo
  
    // Criando o descritor do socket do servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        // AF_INET: Protocolo IPv4
        // SOCK_STREAM: Protocolo TCP
        perror("socket failed"); // Mensagem de erro caso a criação do socket falhe
        exit(EXIT_FAILURE); // Encerra o programa com erro
    }
  
    // Configura o socket para reutilizar endereço e porta
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        // SOL_SOCKET: Nível de configuração do socket
        // SO_REUSEADDR | SO_REUSEPORT: Permite reutilizar o endereço/porta
        perror("setsockopt"); // Mensagem de erro caso a configuração falhe
        exit(EXIT_FAILURE); // Encerra o programa com erro
    }
    
    // Configuração do endereço do servidor
    address.sin_family = AF_INET; // Tipo de endereço (IPv4)
    address.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer endereço
    address.sin_port = htons(atoi(argv[1])); 
    // Porta do servidor (converte string para inteiro e, em seguida, para formato de rede)

    // Vincula o socket ao endereço e porta configurados
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed"); // Mensagem de erro caso o bind falhe
        exit(EXIT_FAILURE); // Encerra o programa com erro
    }

    // Coloca o socket em modo de escuta para aceitar conexões
    if (listen(server_fd, 3) < 0) {
        // 3: Número máximo de conexões pendentes na fila
        perror("listen"); // Mensagem de erro caso o listen falhe
        exit(EXIT_FAILURE); // Encerra o programa com erro
    }

    // Aceita uma conexão de um cliente
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        // Aceita a primeira conexão pendente na fila
        perror("accept"); // Mensagem de erro caso o accept falhe
        exit(EXIT_FAILURE); // Encerra o programa com erro
    }

    // Loop 'eterno' para receber mensagens até dizer chega
    while (true) {
        // Lê a mensagem enviada pelo cliente
        valread = read(new_socket, buffer, sizeof(buffer)); 
        // Lê os dados recebidos no socket conectado
        printf("%s\n", buffer); // Exibe a mensagem do cliente no terminal

        // Copiar o valor mandado pelo cliente
        strcpy(hello, valread);
        // Colocar um * no final para marcar a resposta
        strcat(hello, "*");
    
        // Envia a mensagem de resposta para o cliente
        send(new_socket, hello, strlen(hello), 0);
        printf("%s\n",hello); // Mensagem de confirmação no terminal

        // Quebra o ciclo caso a mensagem seja 'exit'
        if (strcmp(buffer, "exit") != 0) break;
    }   
    
    // Fecha o socket conectado (cliente)
    close(new_socket);
    
    // Fecha o socket do servidor e libera os recursos
    shutdown(server_fd, SHUT_RDWR); 
    // SHUT_RDWR: Fecha o socket para leitura e escrita
    return 0; // Finaliza o programa com sucesso
}