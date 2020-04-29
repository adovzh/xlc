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

  namespace impl {
    template<std::size_t... Args>
    struct args {};

    template<std::size_t N, std::size_t... Args>
    struct args_list: args_list<N-1, N-1, Args...> {};

    template<std::size_t... Args>
    struct args_list<0, Args...>: args<Args...> {};
  }

  template<typename Function, typename ... Args>
  class packed_command: public command {
      std::function<Function> fun;
      std::tuple<Args...> args;
      std::promise<std::invoke_result_t<Function, Args...>> promise;
  public:
      explicit packed_command(Function&& f, Args&&... args);
      void run() override;
      std::future<std::invoke_result_t<Function, Args...>> result();
  private:
      std::invoke_result_t<Function, Args...> execute(std::tuple<Args...>&);
      template<std::size_t... ArgsList>
      std::invoke_result_t<Function, Args...> execute(std::tuple<Args...>&, impl::args<ArgsList...>);
  };

  template<typename Function, typename... Args>
  packed_command<Function, Args...>::packed_command(Function&& f, Args&& ... args):
          fun(std::forward<Function>(f)), args(std::make_tuple(std::forward<Args>(args)...)) {}

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
  std::future<std::invoke_result_t<Function, Args...>>
  packed_command<Function, Args...>::result()
  {
      return promise.get_future();
  }

  template<typename Function, typename... Args>
  std::invoke_result_t<Function, Args...>
  packed_command<Function, Args...>::execute(std::tuple<Args...>& args_tuple)
  {
      return execute(args_tuple, impl::args_list<sizeof...(Args)>());
  }

  template<typename Function, typename... Args>
  template<std::size_t... ArgsList>
  std::invoke_result_t<Function, Args...>
  packed_command<Function, Args...>::execute(std::tuple<Args...>& args_tuple, impl::args<ArgsList...>)
  {
      return fun(std::get<ArgsList>(args_tuple)...);
  }
}

#endif //XLC_COMMAND_H