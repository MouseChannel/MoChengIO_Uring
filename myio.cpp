#include <bits/types/struct_iovec.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <liburing.h>
#include <thread>
using namespace std;

int SIZE = 1024;
enum io_type {
  M_ACCEPT,
  M_READ,
  M_WRITE,
};
struct io_data {
  io_type type;
  iovec iovecs;
  int fd;
  
};

class MoChengIO {

private:
  io_uring ring;
  io_uring_sqe *sqe;
  io_uring_cqe *cqe;

public:
  MoChengIO() { auto ret = io_uring_queue_init(64, &ring, 0); }

  void Work() {
    while (true) {
      auto err = io_uring_wait_cqe(&ring, &cqe);
      if (err < 0) {
        printf("wait cqe failed: %d\n ", err);
        return;
      }
      auto data = (io_data *)io_uring_cqe_get_data(cqe);
      switch (data->type) {

      case M_ACCEPT:
        cout << "accept" << endl;

        break;
      case M_READ:
        cout << "read\n" << (char *)data->iovecs.iov_base << endl;
        // this_thread::sleep_for(chrono::seconds(3));
        SubmitRead(data->fd);
        SubmitWrite(data);

        break;
      case M_WRITE:
        cout << "write" << endl;
        break;
      }
      io_uring_cqe_seen(&ring, cqe);
    }
  }

  void SubmitRead(int fd) {
    sqe = io_uring_get_sqe(&ring);
    struct io_data *data;

    data = (io_data *)malloc(SIZE + sizeof(io_data));
    data->type = io_type::M_READ;
    data->iovecs.iov_base = data + 1;
    data->iovecs.iov_len = SIZE;
    data->fd = fd;

    io_uring_prep_readv(sqe, fd, &data->iovecs, 1, 0);
    io_uring_sqe_set_data(sqe, data);
    io_uring_submit(&ring);
  }
  void SubmitWrite( io_data *data) {
    sqe = io_uring_get_sqe(&ring);
    // struct io_data *data;

    // data = (io_data *)malloc(SIZE + sizeof(io_data));
    data->type = io_type::M_WRITE;
    // data->iovecs.iov_base = data + 1;
    // data->iovecs.iov_len = SIZE;

    io_uring_prep_writev(sqe, data->fd, &data->iovecs, 1, 0);
    io_uring_sqe_set_data(sqe, data);
    io_uring_submit(&ring);
  }
};
