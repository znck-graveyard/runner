#include <iostream>
#include <analyzer.h>
#include <helpers.h>
#include <limits.h>
#include <process.h>

using namespace std;

int main(int argc, char *argv[]) {
  Limits limits;
  string input, output, error, analysis = "analysis.yml";
  pid_t pid, mid;

  int skip = parse_args(argc, argv, limits, analysis);

  pid = fork();
  if (0 == pid) {
    Process process(limits, input, output, error);

    return process.run(argc - skip, argv + skip);
  } else if (-1 == pid) {
    die(10, "Failed to launch program.");
  }

  mid = fork();
  if (0 == mid) {
    sleep(limits.totalTime * 4);
    kill(pid, 9);

    return 0;
  } else if (-1 == mid) {
    die(10, "Failed to launch program monitor.");
  }

  int status;
  struct rusage usage;

  wait4(pid, &status, 0, &usage);
  kill(mid, 9);
  waitpid(mid, NULL, 0);

  Analyzer analyzer;
  analyzer.dump(usage, analysis);

  if (WIFSIGNALED(status)) {
    int sig = WTERMSIG(status);
    switch(sig) {
      case SIGXCPU: die(1, "TLE Time Limit Exceeded.");
      case SIGFPE : die(1, "FPE Floating Point Exception.");
      case SIGILL : die(1, "ILL Illegal Instruction.");
      case SIGSEGV: die(1, "SEG Segmentation Fault.");
      case SIGABRT: die(1, "ABRT Aborted.");
      case SIGBUS : die(1, "BUS Bus error.");
      case SIGSYS : die(1, "SYS Invalid System Call.");
      case SIGXFSZ: die(1, "XFSZ Output File Too Large.");
      case SIGKILL: die(1, "KILL Program Killed.");
      default: die(1, "UKN Unknown Error.");
    }
  }

  if (analyzer.user_time + analyzer.sys_time > limits.runningTime) {
    die(1, "TLE Time Limit Exceeded.");
  }

  if (!WIFEXITED(status)) {
    die(1, "EXIT Program Exited Abnormally.");
  }

  if (0 != WEXITSTATUS(status)) {
    die(1, "EXIT Non-zero Exit Code.");
  }

  return 0;
}
