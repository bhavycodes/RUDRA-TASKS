#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>

using namespace std;

void receiveMessages(int socket)
{
    char buffer[1024];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(socket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived <= 0)
        {
            cout << "\nClient disconnected." << endl;
            break;
        }

        cout << "\nClient: " << buffer << endl;
        cout << "You: ";
        cout.flush();
    }
}

void sendMessages(int socket)
{
    string message;

    while (true)
    {
        cout << "You: ";
        getline(cin, message);

        if (message == "exit")
            break;

        send(socket, message.c_str(), message.length(), 0);
    }
}

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
    {
        cout << "Socket creation failed" << endl;
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket,
             (struct sockaddr*)&serverAddress,
             sizeof(serverAddress)) < 0)
    {
        cout << "Bind failed" << endl;
        return 1;
    }

    listen(serverSocket, 1);

    cout << "Server waiting for connection..." << endl;

    clientSocket = accept(serverSocket, nullptr, nullptr);

    if (clientSocket < 0)
    {
        cout << "Accept failed" << endl;
        return 1;
    }

    cout << "Client connected!" << endl;

    thread receiver(receiveMessages, clientSocket);
    thread sender(sendMessages, clientSocket);

    receiver.join();
    sender.join();

    close(clientSocket);
    close(serverSocket);

    return 0;
}
