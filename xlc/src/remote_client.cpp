//
// Created by Alexander Dovzhikov on 4/5/20.
//

#include <remote_client.h>

namespace xlc {
  remote_client::remote_client(std::shared_ptr<remote_transport> rt):
          transport(std::move(rt)),
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
      return exec_function([this] {
          std::cout << "Connecting remote (lambda function) client..." << std::endl;
          transport->open();
          return true;
      });
  }

  std::future<bool> remote_client::disconnect()
  {
      return exec_function([this] {
          std::cout << "Disconnecting remote (lambda function) client..." << std::endl;
          transport->close();
          return true;
      });
  }
}