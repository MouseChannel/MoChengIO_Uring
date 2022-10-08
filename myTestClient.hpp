
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace MoChengNet {

class MyClient {
private:
public:
  MyClient() = default;

  void Start() {

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Note: 指定 对端(Server) 协议族 / port / ipaddr
    struct sockaddr_in serv_addr;
    // memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    // [2] request to server, 直到 server 传回数据后, connect() 才 return
    int currFD =
        connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("currFD: %d\n", currFD);
    // [3] 读 对端的 Response
    std::string mes;
    while (true) {

      std::cin >> mes;

      char buffer[mes.length()];

      // read(sockfd, buffer, sizeof(buffer));

      // printf("Message form server: %s\n", buffer);
      strcpy(buffer, mes.c_str());
      write(sockfd, buffer, sizeof(buffer));  

      memset(buffer, 0, mes.size());
      printf("before: %s\n", buffer);
      read(sockfd, buffer, sizeof(buffer));
      printf("after: %s\n", buffer);
    }
    // [4] 关 套接字
    close(sockfd);
  }
};

} // namespace MoChengNet