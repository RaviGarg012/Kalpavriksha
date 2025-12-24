#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
// Path to the shared file
#define DB_FILE "../resource/accountDB.txt"

pthread_mutex_t file_mutex;

// Function to update or read the balance safely
void process_transaction(int client_sock, int option, float amount)
{
    float current_balance;
    char response[256];
    FILE *file;

    // Lock mutex before file access
    pthread_mutex_lock(&file_mutex);

    file = fopen(DB_FILE, "r+");
    if (file == NULL)
    {
        strcpy(response, "Error: Database file not found.\n");
        pthread_mutex_unlock(&file_mutex);
        send(client_sock, response, sizeof(response), 0);
        return;
    }
    // Read current balance
    fscanf(file, "%f", &current_balance);

    if (option == 1)
    {
        // Withdraw
        if (amount > current_balance)
        {
            sprintf(response, "Transaction Failed: Insufficient funds. Balance: %.2f\n", current_balance);
        }
        else
        {
            current_balance -= amount;
            rewind(file);
            fprintf(file, "%.2f", current_balance); // Update file
            sprintf(response, "Withdrawal Successful. New Balance: %.2f\n", current_balance);
        }
    }
    else if (option == 2)
    {
        // Deposit
        current_balance += amount;
        rewind(file);
        fprintf(file, "%.2f", current_balance); // Update file
        sprintf(response, "Deposit Successful. New Balance: %.2f\n", current_balance);
    }
    else if (option == 3)
    {
        // Display Balance
        sprintf(response, "Current Balance: %.2f\n", current_balance);
    }

    fclose(file);
    // Unlock mutex
    pthread_mutex_unlock(&file_mutex);
    // Send result to client
    write(client_sock, response, strlen(response) + 1);
}

// Thread function to handle client
void *handle_client(void *socket_desc)
{
    int sock = *(int *)socket_desc;
    free(socket_desc);
    int option;
    float amount;

    // Loop to keep serving the client until they choose Exit
    while (1)
    {
        // Receive option from client
        if (read(sock, &option, sizeof(option)) <= 0)
            break;

        if (option == 4)
            break;

        // If Withdraw or Deposit, receive amount
        if (option == 1 || option == 2)
        {
            read(sock, &amount, sizeof(amount));
        }
        else
        {
            amount = 0;
        }

        process_transaction(sock, option, amount);
    }

    close(sock);
    return NULL;
}

int main()
{
    int server_fd, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Initialize Mutex
    if (pthread_mutex_init(&file_mutex, NULL) != 0)
    {
        perror("Mutex init failed");
        return 1;
    }

    // Create Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("ATM Server listening on port %d...\n", PORT);

    while (1)
    {
        int client_fd;
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            continue;
        }

        printf("New Client Connected.\n");

        // Create a dedicated thread for each client
        pthread_t thread_id;
        new_sock = malloc(1);
        *new_sock = client_fd;

        if (pthread_create(&thread_id, NULL, handle_client, (void *)new_sock) < 0)
        {
            perror("Could not create thread");
            return 1;
        }
    }

    pthread_mutex_destroy(&file_mutex);
    return 0;
}