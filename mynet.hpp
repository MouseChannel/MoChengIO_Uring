
#include "myio.cpp"
#include <arpa/inet.h>
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

namespace MoChengNet {

class Net {

private:
public:
  Net() = default;

  void Start() {
    int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenfd == -1) {
      std::cout << "socket init failed" << std::endl;
    } else {
      std::cout << "socket: " << listenfd << " init succeed" << std::endl;
    }

    struct sockaddr_in serveraddr {
      AF_INET, htons(8888), inet_addr("0.0.0.0")
    };
    auto bindFd =
        bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    std::cout << bindFd << std::endl;
    std::cout << listen(listenfd, 20) << std::endl;

    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    printf("accepting:...\n");
    MoChengIO myio;
    thread t([&myio]() {
      myio.Work();
    });

    while (true) {

      int connfd =
          accept(listenfd, (struct sockaddr *)&clientAddr, &clientAddrSize);

      std::cout << "connect one: " << connfd << std::endl;
      char mes[] = "mocheng";
      // write(connfd, mes, sizeof(mes));
      // printf("write down");

      // read(connfd, mes, sizeof(mes));
      // printf("Message form server: %s\n", mes);
      myio.SubmitRead(connfd);
      close(connfd);
    }
    
    close(listenfd);
  }
};

} // namespace MoChengNet