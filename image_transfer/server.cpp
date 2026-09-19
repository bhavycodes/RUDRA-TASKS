#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

const int PORT = 8080;
const int BUFFER_SIZE = 4096;

bool receiveAll(int socket, char *buffer, size_t size)
{
    size_t totalReceived = 0;

    while (totalReceived < size)
    {
        ssize_t bytesReceived = recv(
            socket,
            buffer + totalReceived,
            size - totalReceived,
            0
        );

        if (bytesReceived <= 0)
        {
            return false;
        }

        totalReceived += bytesReceived;
    }

    return true;
}

int main()
{
    int serverSocket;
    int clientSocket;

    struct sockaddr_in serverAddress;

    // Create TCP socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
    {
        cout << "Socket creation failed." << endl;
        return 1;
    }

    // Configure server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(
            serverSocket,
            (struct sockaddr *)&serverAddress,
            sizeof(serverAddress)
        ) < 0)
    {
        cout << "Bind failed." << endl;
        close(serverSocket);
        return 1;
    }

    // Listen for client
    if (listen(serverSocket, 1) < 0)
    {
        cout << "Listen failed." << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Server waiting for client..." << endl;

    // Accept client
    clientSocket = accept(serverSocket, nullptr, nullptr);

    if (clientSocket < 0)
    {
        cout << "Accept failed." << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Client connected!" << endl;

    // Receive filename length
    uint32_t filenameLength;

    if (!receiveAll(
            clientSocket,
            (char *)&filenameLength,
            sizeof(filenameLength)
        ))
    {
        cout << "Failed to receive filename length." << endl;
        close(clientSocket);
        close(serverSocket);
        return 1;
    }

    filenameLength = ntohl(filenameLength);

    // Receive filename
    string filename(filenameLength, '\0');

    if (!receiveAll(
            clientSocket,
            filename.data(),
            filenameLength
        ))
    {
        cout << "Failed to receive filename." << endl;
        close(clientSocket);
        close(serverSocket);
        return 1;
    }

    cout << "Receiving: " << filename << endl;

    // Receive file size
    uint64_t fileSizeNetwork;

    if (!receiveAll(
            clientSocket,
            (char *)&fileSizeNetwork,
            sizeof(fileSizeNetwork)
        ))
    {
        cout << "Failed to receive file size." << endl;
        close(clientSocket);
        close(serverSocket);
        return 1;
    }

    uint64_t fileSize = be64toh(fileSizeNetwork);

    cout << "File size: " << fileSize << " bytes" << endl;

    // Create output file
    ofstream outputFile("received_" + filename, ios::binary);

    if (!outputFile)
    {
        cout << "Could not create output file." << endl;
        close(clientSocket);
        close(serverSocket);
        return 1;
    }

    // Receive image data
    char buffer[BUFFER_SIZE];

    uint64_t totalReceived = 0;

    while (totalReceived < fileSize)
    {
        uint64_t remaining = fileSize - totalReceived;

        int bytesToReceive =
            remaining < BUFFER_SIZE
            ? remaining
            : BUFFER_SIZE;

        int bytesReceived = recv(
            clientSocket,
            buffer,
            bytesToReceive,
            0
        );

        if (bytesReceived <= 0)
        {
            cout << "Connection lost while receiving file." << endl;
            outputFile.close();
            close(clientSocket);
            close(serverSocket);
            return 1;
        }

        outputFile.write(buffer, bytesReceived);

        totalReceived += bytesReceived;
    }

    outputFile.close();

    cout << "Image received successfully!" << endl;
    cout << "Saved as: received_" << filename << endl;

    close(clientSocket);
    close(serverSocket);

    return 0;
}
