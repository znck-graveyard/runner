#include <limits.h>

Limits::Limits() {
  stack = 8 * 1024 * 1024; // 8MB
  memory = 64 * 1024 * 1024; // 64MB
  runningTime = 2000; // 2 seconds
  outputSize = 50 * 1024 * 1024; // 50MB
  files = 16; // Number of open files
  processes = 1; // Number of threads/subprocesses
  uid = getuid();
  gid = getgid();
}
