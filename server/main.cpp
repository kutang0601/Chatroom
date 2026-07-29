#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

using namespace std;

int main()
{
    int listenfd = socket(AF_INET,SOCK_STREAM,0);

    if (listenfd == -1)
    {
        cout << "socket error" << endl;
        return 1;
    }

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(8080);

    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(listenfd, (struct sockaddr*)& server_addr, sizeof(server_addr)) == -1)
    {
        cout << "bind error" << endl;
        return 1;
    }

    listen(listenfd, 5);

    cout << "server ready" << endl;

    int clientfd = accept( listenfd, nullptr, nullptr);

    if (clientfd == -1)
    {
        cout << "accept fail" << endl;
    }

    cout << "client connect" << endl;

    char buf[1024] = {0};

    memset(buf, 0, sizeof(buf));

    recv(clientfd, buf, sizeof(buf), 0);

    cout << "client say:" << buf << endl;

    char msg[] = "hello client";

    send(clientfd, msg, sizeof(msg), 0);

    close(listenfd);

    close(clientfd);

    return 0;
}