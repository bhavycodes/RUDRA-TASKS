#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

const int PORT = 8080;
const int BUFFER_SIZE = 4096;

bool sendAll(int socket, const char *buffer, size_t size)
{
    size_t totalSent = 0;

    while (totalSent < size)
    {
        ssize_t bytesSent = send(
            socket,
            buffer + totalSent,
            size - totalSent,
            0
        );

        if (bytesSent <= 0)
        {
            return false;
        }

        totalSent += bytesSent;
    }

    return true;
}

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddress;

    // Ask for image path
    string filePath;

    cout << "Enter image path: ";
    getline(cin, filePath);

    // Open image
    ifstream inputFile(filePath, ios::binary | ios::ate);

    if (!inputFile)
    {
        cout << "Could not open image." << endl;
        return 1;
    }

    // Get file size
    streamsize fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    // Extract filename
    size_t slashPosition = filePath.find_last_of("/\\");

    string filename;

    if (slashPosition == string::npos)
    {
        filename = filePath;
    }
    else
    {
        filename = filePath.substr(slashPosition + 1);
    }

    cout << "Image: " << filename << endl;
    cout << "Size: " << fileSize << " bytes" << endl;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket < 0)
    {
        cout << "Socket creation failed." << endl;
        return 1;
    }

    // Configure server
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // Server is running on same computer
    inet_pton(
        AF_INET,
        "127.0.0.1",
        &serverAddress.sin_addr
    );

    // Connect to server
    if (connect(
            clientSocket,
            (struct sockaddr *)&serverAddress,
            sizeof(serverAddress)
        ) < 0)
    {
        cout << "Connection failed." << endl;
        close(clientSocket);
        return 1;
    }

    cout << "Connected to server!" << endl;

    // Send filename length
    uint32_t filenameLength = htonl(filename.length());

    if (!sendAll(
            clientSocket,
            (char *)&filenameLength,
            sizeof(filenameLength)
        ))
    {
        cout << "Failed to send filename length." << endl;
        close(clientSocket);
        return 1;
    }

    // Send filename
    if (!sendAll(
            clientSocket,
            filename.c_str(),
            filename.length()
        ))
    {
        cout << "Failed to send filename." << endl;
        close(clientSocket);
        return 1;
    }

    // Send file size
    uint64_t fileSizeNetwork = htobe64(fileSize);

    if (!sendAll(
            clientSocket,
            (char *)&fileSizeNetwork,
            sizeof(fileSizeNetwork)
        ))
    {
        cout << "Failed to send file size." << endl;
        close(clientSocket);
        return 1;
    }

    // Send image data
    char buffer[BUFFER_SIZE];

    uint64_t totalSent = 0;

    while (inputFile.read(buffer, BUFFER_SIZE) ||
           inputFile.gcount() > 0)
    {
        streamsize bytesRead = inputFile.gcount();

        if (!sendAll(
                clientSocket,
                buffer,
                bytesRead
            ))
        {
            cout << "Failed while sending image." << endl;
            inputFile.close();
            close(clientSocket);
            return 1;
        }

        totalSent += bytesRead;

        cout << "\rSent: "
             << totalSent
             << " / "
             << fileSize
             << " bytes"
             << flush;
    }

    cout << endl;
    cout << "Image sent successfully!" << endl;

    inputFile.close();
    close(clientSocket);

    return 0;
}
