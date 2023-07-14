#include "src/Interface/Interface.hpp"

using namespace std;

int main()
{
#ifdef ENABLE_LOGGING
  std::signal(SIGINT, HandleInterruptSignal);
#endif

  Interface().init();
  return 0;
}
