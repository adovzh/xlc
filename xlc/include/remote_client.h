//
// Created by Alexander Dovzhikov on 18/4/20.
//

#ifndef XLC_REMOTE_CLIENT_H
#define XLC_REMOTE_CLIENT_H

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <blocking_queue.h>
#include <command.h>
#include <remote_transport.h>
#include <future>
#include <utility>
#include <iostream>

namespace xlc {

  class remote_client {
      using command_ptr = std::unique_ptr<command>;
  private:
      std::shared_ptr<remote_transport> transport;
      blocking_queue<command_ptr> queue;
      std::thread consumer_thread;
  public:
      explicit remote_client(std::shared_ptr<remote_transport> rt);
      ~remote_client();
      [[nodiscard]] bool isOpen() const { return transport->isOpen(); }
      std::future<bool> connect();
      std::future<bool> disconnect();
  protected:
      [[nodiscard]] const std::shared_ptr<remote_transport>& get_transport() const { return transport; }

      template<typename Function, typename... Args>
      auto exec_function(Function&& func, Args&&... args)
      {
          using Command = packed_command<std::decay_t<Function>, std::decay_t<Args>...>;
          using ReturnType = typename Command::ReturnType;

          auto cmd = std::make_unique<Command>(std::forward<Function>(func), std::forward<Args>(args)...);
          auto fut = cmd->result();
          queue.offer(std::move(cmd), 1000L);
          std::cout << "[exec_function]: " << cmd.get() << std::endl;
          return fut;
      }
  private:
      void consumer();
  };

}


#endif //XLC_REMOTE_CLIENT_H
