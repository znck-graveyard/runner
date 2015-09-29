#include <helpers.h>

using namespace std;

void die(int status_code, string message) {
  // TODO: print message to a stream.
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
  {NULL, 0, NULL, 0}            // 7+
};

void help(bool err = false) {
  std::cerr << ""
  "usage: runner [options] program <argument ...> \n"
  "\n"
  "options:\n"
  "  -a [filename], --analysis=[filename]  Set analysis filename. (Default: analysis.yml)\n"
  "  -s [num], --stack=[num]               Set stack size limit. (In bytes, Default: 8MB)\n"
  "  -m [num], --memory=[num]              Set memory size limit. (In bytes, Default: 64MB)\n"
  "  -o [num], --output-size=[num]         Set output file size limit. (In bytes, Default: 50MB)\n"
  "  -t [num], --time=[num]                Set running time limit. (In seconds, Default: 2s)\n"
  "  -f [num], --open-files=[num]          Set maximum open files limt. (Default: 16)\n"
  "  -p [num], --processes=[num]           Set maximum subprocesses/threads count. (Default: 1)\n"
  "  -h, -?, --help                        Print help text.\n";

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
  while((c = getopt_long(argc, argv, "s:m:o:t:f:p:h?a:", options, &id)) != -1) {
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
        }
        break;
      case 's':
        limits.stack = num(optarg);
        break;
      case 'm':
        limits.memory = num(optarg);
        break;
      case 'o':
        limits.outputSize = num(optarg);
        break;
      case 't':
        limits.runningTime = num(optarg);
        break;
      case 'f':
        limits.files = num(optarg);
        break;
      case 'p':
        limits.processes = num(optarg);
        break;
      case 'h':
      case '?':
        help();
        break;
      case 'a':
        filename = string(optarg);
        break;
    }
  }
  ++id;
  if(id == argc) {
    help(true);
  }
  return id;
}

string pathFromFd(int fd) {
  char path[1024];
  // if (-1 != fcntl(fd, F_GETPATH, path)) {
  //
  // }
}
