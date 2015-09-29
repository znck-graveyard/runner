#ifndef RUNNER_LIMITS
#define RUNNER_LIMITS

class Limits {
public:
  long long stack;
  long long memory;
  long long runningTime;
  long long totalTime;
  long long outputSize;
  long long files;
  long long processes;

  Limits();
};

#endif
