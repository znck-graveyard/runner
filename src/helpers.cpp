#include <helpers.h>
#include <fstream>

using namespace std;

void die(int status_code, string message) {
  ofstream ferr("runner.log", ofstream::out | ofstream::app);
  ferr << "W"<< status_code << " " << message << endl;

  cerr << message << endl;

  exit(status_code);
}

static struct option options[] = {
  {"stack", 1, NULL, 0},        // 0
  {"memory", 1, NULL, 0},       // 1
  {"output-size", 1, NULL, 0},  // 2
  {"time", 1, NULL, 0},         // 3
  {"open-files", 1, NULL, 0},   // 4
  {"processes", 1, NULL, 0},    // 5
  {"help", 1, NULL, 0},         // 6
  {"analysis", 1, NULL, 0},     // 7
  {"user", 1, NULL, 0},         // 8
  {"group", 1, NULL, 0},        // 9
  {NULL, 0, NULL, 0}            // 9+
};

void help(bool err = false) {
  std::cerr << ""
  "usage: runner [options] program <argument ...> \n"
  "\n"
  "options:\n"
  "  --analysis=[filename]       Set analysis filename. (Default: analysis.yml)\n"
  "  --stack=[num]               Set stack size limit. (In bytes, Default: 8MB)\n"
  "  --memory=[num]              Set memory size limit. (In bytes, Default: 64MB)\n"
  "  --output-size=[num]         Set output file size limit. (In bytes, Default: 50MB)\n"
  "  --time=[num]                Set running time limit. (In seconds, Default: 2s)\n"
  "  --open-files=[num]          Set maximum open files limt. (Default: 16)\n"
  "  --processes=[num]           Set maximum subprocesses/threads count. (Default: 1)\n"
  "  --user=[num]                Set user id. (Default: current user)\n"
  "  --group=[num]               Set group id. (Default: current user's group)\n"
  "  --help                      Print help text.\n";

  if(err) {
    exit(0);
  }
}

long long num(char *s) {
  long long n = 0;
  int i = 0;
  while (s[i] != '\0') {
    n = (n << 3) + (n << 1) + s[i++] - '0';
  }

  return n;
}

int parse_args(int argc, char *argv[], Limits &limits, string &filename) {
  int id = 0, c;
  while((c = getopt_long(argc, argv, "", options, &id)) != -1) {
    switch (c) {
      case 0:
        switch (id) {
          case 0:
            limits.stack = num(optarg);
            break;
          case 1:
            limits.memory = num(optarg);
            break;
          case 2:
            limits.outputSize = num(optarg);
            break;
          case 3:
            limits.runningTime = num(optarg);
            break;
          case 4:
            limits.files = num(optarg);
            break;
          case 5:
            limits.processes = num(optarg);
            break;
          case 6:
            help();
            break;
          case 7:
            filename = string(optarg);
            break;
          case 8:
            limits.uid = num(optarg);
            break;
          case 9:
            limits.gid = num(optarg);
            break;
        }
        break;
    }
  }
  if(optind == argc) {
    help(true);
  }
  return optind;
}

string pathFromFd(int fd) {
  char path[1024];
  // if (-1 != fcntl(fd, F_GETPATH, path)) {
  //
  // }
}
