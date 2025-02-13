// Client side C/C++ program to demonstrate Socket programming
#include <winsock2.h>  // Biblioteca para funções de conversão e manipulação de endereços IP
#include <stdio.h>      // Biblioteca padrão de entrada/saída
#include <stdlib.h>     // Biblioteca padrão para funções utilitárias, como atoi
#include <string.h>     // Biblioteca para manipulação de strings
#include <sys/socket.h> // Biblioteca para funções relacionadas a sockets
#include <unistd.h>     // Biblioteca para funções POSIX, como close

int main(int argc, char const* argv[])
{
    // Variáveis para o socket, leitura de dados, e descrição do endereço do servidor
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char hello[1024]; // Mensagem que será enviada ao servidor
    strcpy(hello, "Hello from Client"); // Inicializa a mensagem com um texto fixo
    char buffer[1024] = { 0 }; // Buffer para armazenar a resposta do servidor

    // Criação do socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // AF_INET: IPv4, SOCK_STREAM: TCP
        printf("\n Socket creation error \n"); // Mensagem de erro caso o socket não seja criado
        return -1; // Finaliza o programa com erro
    }

    // Configuração do endereço do servidor
    serv_addr.sin_family = AF_INET; // Família de endereços IPv4
    serv_addr.sin_port = htons(atoi(argv[2])); // Porta do servidor (conversão de string para inteiro e depois para formato de rede)

    // Converte o endereço IP de texto para o formato binário
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n"); // Mensagem de erro caso o endereço seja inválido
        return -1; // Finaliza o programa com erro
    }

    // Estabelece a conexão com o servidor
    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n"); // Mensagem de erro caso a conexão falhe
        return -1; // Finaliza o programa com erro
    }

    // Loop para enviar mensagens até receber um 'exit'
    while (true)
    {
        // Ler o que o cliente digitar e salvar na mensagem 'hello'
        fgets(hello, 1024, stdin);

        // Envia a mensagem "Hello from Client" para o servidor
        send(sock, hello, strlen(hello), 0);
        printf("Hello message sent\n"); // Confirmação de envio no terminal

        // Lê a resposta do servidor e armazena no buffer
        valread = read(sock, buffer, sizeof(buffer));
        printf("%s\n", buffer); // Exibe a resposta do servidor no terminal

        // Quebrar o loop caso a mensagem seja 'exit'
        if (strcmp(hello, "exit") != 0) break;
    }

    // Fecha o socket conectado
    close(client_fd);
    return 0; // Finaliza o programa com sucesso
}