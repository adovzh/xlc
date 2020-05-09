//
// Created by Alexander Dovzhikov on 3/5/20.
//

#ifndef IMRCLIENT_SOCKET_TRANSPORT_H
#define IMRCLIENT_SOCKET_TRANSPORT_H

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <remote_transport.h>

namespace xlc {
  using namespace apache::thrift;
  using namespace apache::thrift::protocol;
  using namespace apache::thrift::transport;

  class socket_transport : public remote_transport {
      std::shared_ptr<TTransport> socket;
      std::shared_ptr<TTransport> transport;
  public:
      socket_transport(const std::string& host, int port);
      [[nodiscard]] bool isOpen() const override { return transport->isOpen(); }
      [[nodiscard]] std::shared_ptr<TTransport> get_transport() const { return transport; }
      void open() override { transport->open(); }
      void close() override { transport->close(); }
  };

  inline socket_transport::socket_transport(const std::string& host, int port):
    socket(new TSocket(host, port)), transport(new TBufferedTransport(socket))
  {
  }
}

#endif //IMRCLIENT_SOCKET_TRANSPORT_H
