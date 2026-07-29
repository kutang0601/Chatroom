#include<iostream>
#include <ostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<cstring>
#include<unistd.h>


using namespace std;

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        cout << "socket error" << endl;
        return 1;
    }

    sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(8080);

    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        cout << "connect error" << endl;
        return 1;
    }

    cout << "connect success" << endl;

    char msg[] = "hello serve";
    
    send(sockfd, msg, sizeof(msg), 0);

    char buf[1024] = {0};
    
    memset(buf, 0, sizeof(buf));

    recv(sockfd, buf, sizeof(buf), 0);

    cout << "serve say:" << buf << endl;

    close(sockfd);

    return 0;
}