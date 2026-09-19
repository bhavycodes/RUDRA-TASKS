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
            cout << "\nServer disconnected." << endl;
            break;
        }

        cout << "\nServer: " << buffer << endl;
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
    int clientSocket;
    struct sockaddr_in serverAddress;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket < 0)
    {
        cout << "Socket creation failed" << endl;
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(clientSocket,
                (struct sockaddr*)&serverAddress,
                sizeof(serverAddress)) < 0)
    {
        cout << "Connection failed" << endl;
        return 1;
    }

    cout << "Connected to server!" << endl;

    thread receiver(receiveMessages, clientSocket);
    thread sender(sendMessages, clientSocket);

    receiver.join();
    sender.join();

    close(clientSocket);

    return 0;
}
