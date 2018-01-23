// #include <iostream>
#include "GcdLog.h"

namespace carl {

  GcdLog::ratfun_ptr GcdLog::hook = nullptr;

  void GcdLog::register_hook(GcdLog::ratfun_ptr hook_) {
    hook = hook_;
  }

  void GcdLog::reset() {
    hook = nullptr;
  }

  void GcdLog::log_before(void *rat_fun) {
    if (hook)
      (*hook)(rat_fun, true, std::chrono::nanoseconds::zero());
  }
  
  void GcdLog::log_after(void *rat_fun, std::chrono::nanoseconds duration) {
    if (hook)
      (*hook)(rat_fun, false, duration);
    //    std::cout << "GCD took :" <<  duration.count() << "ns" << std::endl;
  }

}
