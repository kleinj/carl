#pragma once

#include <chrono>

namespace carl {

  class GcdLog {
  public:
    typedef void (*ratfun_ptr)(void*,bool,std::chrono::nanoseconds);

    static void register_hook(ratfun_ptr hook);
    static void reset();
    static void log_before(void *rat_fun);
    static void log_after(void *rat_fun, std::chrono::nanoseconds duration);
  private:
    static ratfun_ptr hook;
  };

}
