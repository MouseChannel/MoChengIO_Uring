#include "myTestClient.hpp"
#include "mynet.hpp"
#include <bits/types/struct_iovec.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>

#include "iotest.cpp"
#include "lib/echo_uring.cpp"
#include "lib/uring_server.cpp"
#include "liburing.h"
#include <linux/io_uring.h>
#include <sys/ioctl.h>

// #include"ppp.cpp"

using namespace MoChengNet;
using namespace std;
struct A {
  int t;
  int q;
  int d;
};
int main() {

  // MyClient c;
  // c.Start();
  Net n;
  n.Start();
}
