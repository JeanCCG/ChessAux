#include "src/Interface/Interface.hpp"
#if ENABLE_LOGGING
#include <csignal>
#endif

using namespace std;
int main()
{
#if ENABLE_LOGGING
  std::signal(SIGINT, HandleInterruptSignal);
#endif

  Interface().init();
  return 0;
}
