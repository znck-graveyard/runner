#include <process.h>
#include <iostream>
#include <fstream>

using namespace std;

static void exit(int errno, string errmsg) {
  ofstream ferr("errors", ofstream::out | ofstream::app);
  ferr << "E"<< errno << " " << errmsg << endl;

  // throw errno;
}

static void set_environment_limits(Limits limits) {
  struct rlimit limit;

  limit.rlim_cur = limits.stack;
  limit.rlim_max = limits.stack + 1024; // + 1KB
  if (0 != setrlimit(RLIMIT_STACK, &limit)) {
    exit(131, "Failed to set stack limit.");
  }

  limit.rlim_cur = limit.rlim_max = limits.memory;
  if (0 != setrlimit(RLIMIT_DATA, &limit)) {
    exit(132, "Failed to set memory limit.");
  }

  limit.rlim_cur = limits.runningTime;
  limit.rlim_max = limits.totalTime;
  if (0 != setrlimit(RLIMIT_CPU, &limit)) {
    exit(133, "Failed to set time limit.");
  }

  limit.rlim_cur = limit.rlim_max = limits.outputSize;
  if (0 != setrlimit(RLIMIT_FSIZE, &limit)) {
    exit(134, "Failed to set maximum open files.");
  }

  limit.rlim_cur = limit.rlim_max = limits.files;
  if (0 != setrlimit(RLIMIT_NOFILE, &limit)) {
    exit(135, "Failed to set maximum open files.");
  }

  limit.rlim_cur = limit.rlim_max = limits.processes;
  if (0 != setrlimit(RLIMIT_NPROC, &limit)) {
    exit(136, "Failed to set maximum subprocesses/threads.");
  }
}

static void set_redirected_files(string input, string output, string error) {
  if(input.length() && NULL == freopen(input.c_str(), "r", stdin)) {
    exit(138, "Failed to redirect stdin.");
  }

  if(output.length() && NULL == freopen(output.c_str(), "w", stdout)) {
    exit(139, "Failed to redirect stdout.");
  }

  if(error.length() && NULL == freopen(error.c_str(), "w", stderr)) {
    exit(140, "Failed to redirect stderr.");
  }
}

static void check_user() {
  if (0 == geteuid() || 0 == getegid()) {
    exit(151, "Cannot run as root.");
  }

  uid_t UID = getuid();
  gid_t GID = getgid();

  if (0 != setresgid(GID, GID, GID) || 0 != setresuid(UID, UID, UID)) {
    exit(152, "Cannot change user or/and group of subprocess.");
  }
}

Process::Process(Limits limits, string input, string output, string error) {
  this->limits = limits;
  this->input = input;
  this->output = output;
  this->error = error;
}

int Process::run(int argc, char *argv[]) {
  try {
    set_environment_limits(limits);
    // set_redirected_files(input, output, error);
    check_user();
  } catch(int errno) {
    return errno;
  }

  for(int i = 3; i < limits.files; ++i) {
    close(i);
  }

  char **commands = (char **) malloc(sizeof(char *) * (argc + 1));
  for (int i = 0; i < argc; ++i) {
    commands[i] = argv[i];
  }
  commands[argc] = NULL;

  execve(argv[0], commands, NULL);

  exit(0);
}
