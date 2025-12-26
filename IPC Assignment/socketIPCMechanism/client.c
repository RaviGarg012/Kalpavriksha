#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    int option;
    float amount;
    char buffer[256] = {0};

    // Create Socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and Connect to Server
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    //
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1)
    {
        // Display Menu
        printf("\n--- ATM Menu ---\n");
        printf("1. Withdraw Amount\n");
        printf("2. Deposit Amount\n");
        printf("3. Display Balance\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &option);

        // Send option to server
        write(sock, &option, sizeof(option));

        if (option == 4)
        {
            printf("Exiting...\n");
            break;
        }

        // If Withdraw or Deposit, ask for amount and send it
        if (option == 1 || option == 2)
        {
            printf("Enter amount: ");
            scanf("%f", &amount);
            write(sock, &amount, sizeof(amount));
        }

        // Read server response
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, 256);
        printf("Server Response: %s\n", buffer);
    }

    close(sock);
    return 0;
}