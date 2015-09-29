#ifndef RUNNER_ANALYZER
#define RUNNER_ANALYZER

#include <string>
#include <fstream>

#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>

using std::string;

class Analyzer {
public:
  double user_time, sys_time;
  Analyzer();
  void dump(struct rusage &, string);
};

#endif
