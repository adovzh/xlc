//
// Created by Alexander Dovzhikov on 3/5/20.
//

#ifndef XLC_IMR_CLIENT_H
#define XLC_IMR_CLIENT_H


#include <socket_transport.h>
#include <gen-cpp/InMemoryRepo.h>
#include <remote_client.h>
#include <future>

namespace xlc {
  using namespace dan::imr::thrift;

  class imr_client : public remote_client {
      InMemoryRepoClient client;
  public:
      imr_client(const std::string& host, int port) :
        remote_client(std::shared_ptr<remote_transport>(new socket_transport("localhost", 3333))),
        client(std::make_shared<TBinaryProtocol>(dynamic_cast<socket_transport&>(*get_transport()).get_transport())) {}
      std::future<bool> get_project(Project& project, std::string& name);
      std::future<bool> advance_major_version(std::string& name);
  };
}

#endif //XLC_IMR_CLIENT_H
