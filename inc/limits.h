#ifndef RUNNER_LIMITS
#define RUNNER_LIMITS

#include <sys/types.h>
#include <unistd.h>

class Limits {
public:
  long long stack;
  long long memory;
  long long runningTime;
  long long totalTime;
  long long outputSize;
  long long files;
  long long processes;
  uid_t uid;
  gid_t gid;

  Limits();
};

#endif
