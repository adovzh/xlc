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
#include <future>

#include <gen-cpp/InMemoryRepo.h>

namespace xlc {
  using namespace apache::thrift;
  using namespace apache::thrift::protocol;
  using namespace apache::thrift::transport;
  using namespace dan::imr::thrift;

  class remote_client {
      using command_ptr = std::shared_ptr<command>;
  private:
      std::shared_ptr<TTransport> socket;
      std::shared_ptr<TTransport> transport;
      InMemoryRepoClient client;
      blocking_queue<command_ptr> queue;
      std::thread consumer_thread;
  public:
      remote_client();
      ~remote_client();
      [[nodiscard]] bool isOpen() const { return transport->isOpen(); }
      std::future<bool> connect();
      std::future<bool> disconnect();
      std::future<bool> get_project(Project& project, std::string& name);
      std::future<bool> advance_major_version(std::string& name);
  private:
      void consumer();

      template<typename Function, typename... Args>
      auto exec_function(Function&& func, Args&&... args)
      {
          using PC_RR = packed_command<std::decay_t<Function>, std::decay_t<Args>...>;
          using ReturnType = typename PC_RR::ReturnType;

          auto* c_ptr = new PC_RR(std::forward<Function>(func), std::forward<Args>(args)...);
          auto fut = c_ptr->result();
          command_ptr c(c_ptr);
          queue.offer(c, 1000L);
          return fut;
      }

      bool connect_impl();
      bool disconnect_impl();
      bool get_project_impl(Project& project, std::string& name);
      bool advance_major_version_impl(std::string& name);
  };

  remote_client::remote_client():
    socket(new TSocket("localhost", 3333)), transport(new TBufferedTransport(socket)),
    client(std::make_shared<TBinaryProtocol>(transport)),
    consumer_thread(&remote_client::consumer, this)
  {
  }

  remote_client::~remote_client()
  {
      consumer_thread.join();
  }

  void remote_client::consumer()
  {
      std::cout << "Starting a queue consumer" << std::endl;
      command_ptr c;
      bool result;
      while ((result = queue.poll(c, 2000L))) {
          std::cout << "Received a command <no name>" << std::endl;
          c->run();
      }
      std::cout << "Queue consumer timed out: "<< std::boolalpha << !result << std::endl;
  }

  std::future<bool> remote_client::connect()
  {
      return exec_function(&remote_client::connect_impl, this);
  }

  std::future<bool> remote_client::disconnect()
  {
      return exec_function(&remote_client::disconnect_impl, this);
  }

  std::future<bool> remote_client::get_project(Project &project, std::string &name)
  {
      return exec_function(&remote_client::get_project_impl, this, std::ref(project), std::ref(name));
  }

  std::future<bool> remote_client::advance_major_version(std::string& name)
  {
      return exec_function(&remote_client::advance_major_version_impl, this, std::ref(name));
  }

  bool remote_client::connect_impl()
  {
      std::cout << "Connecting remote (member-function) client..." << std::endl;
      transport->open();
      client.ping();
      return true;
  }

  bool remote_client::disconnect_impl()
  {
      std::cout << "Disconnecting remote (member-function) client..." << std::endl;
      transport->close();
      return true;
  }

  bool remote_client::get_project_impl(Project& project, std::string& name)
  {
      client.getProject(project, name);
      return true;
  }

  bool remote_client::advance_major_version_impl(std::string& name)
  {
      client.advanceMajorVersion(name);
      return true;
  }
}


#endif //XLC_REMOTE_CLIENT_H
