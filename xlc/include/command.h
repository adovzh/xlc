//
// Created by Alexander Dovzhikov on 28/4/20.
//

#ifndef XLC_COMMAND_H
#define XLC_COMMAND_H

#include <functional>
#include <tuple>
#include <future>

namespace xlc {
  class command {
  public:
      virtual ~command() = default;
      virtual void run() = 0;
  };

  template<typename Function, typename ... Args>
  class packed_command: public command {
  public:
      using ReturnType = std::invoke_result_t<Function, Args...>;
  private:
      using this_type = packed_command<Function, Args...>;
      // data members
      std::tuple<Function, Args...> args;
      std::promise<ReturnType> promise;
  public:
      explicit packed_command(Function&& f, Args&&... args);
      packed_command(const this_type&) = delete;

      this_type& operator=(const this_type&) = delete;

      void run() override;
      std::future<ReturnType> result();
  private:
      ReturnType execute(std::tuple<Function, Args...>&);
      template<typename... InvokeArgs, std::size_t... ArgsList>
      ReturnType execute(std::tuple<InvokeArgs...>&, std::index_sequence<ArgsList...>);
  };

  template<typename Function, typename ... Args>
  using packed_command_t = typename packed_command<Function, Args...>::ReturnType;

  template<typename Function, typename... Args>
  packed_command<Function, Args...>::packed_command(Function&& f, Args&& ... args):
          args(std::make_tuple(std::forward<Function>(f), std::forward<Args>(args)...)) {}

  template<typename Function, typename... Args>
  void packed_command<Function, Args...>::run()
  {
      try {
          promise.set_value(execute(args));
      }
      catch(...) {
          promise.set_exception(std::current_exception());
      }
  }

  template<typename Function, typename... Args>
  std::future<packed_command_t<Function, Args...>>
  packed_command<Function, Args...>::result()
  {
      return promise.get_future();
  }

  template<typename Function, typename... Args>
  packed_command_t<Function, Args...>
  packed_command<Function, Args...>::execute(std::tuple<Function, Args...>& args_tuple)
  {
    return execute(args_tuple, std::make_index_sequence<sizeof...(Args) + 1>{});
  }

  template<typename Function, typename... Args>
  template<typename... InvokeArgs, std::size_t... ArgsList>
  packed_command_t<Function, Args...>
  packed_command<Function, Args...>::execute(std::tuple<InvokeArgs...>& args_tuple, std::index_sequence<ArgsList...>)
  {
      return std::invoke(std::move(std::get<ArgsList>(args_tuple))...);
  }
}

#endif //XLC_COMMAND_H
