#ifndef RUNNER_PROCESS
#define RUNNER_PROCESS

#include <string>
#include <cstdio>
#include <cstdlib>

#include <limits.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using std::string;

class Process {
protected:
  Limits limits;
  string input;
  string output;
  string error;
public:
  Process(Limits, string, string, string);
  int run(int argc, char *argv[]);
};

#endif
