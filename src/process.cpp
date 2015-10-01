#include <process.h>
#include <iostream>
#include <fstream>
#include <errno.h>

using namespace std;

static void rotate_log() {
  ofstream ferr("runner.log");
  ferr << "# Runner Log" << endl;
}

static void exit(int error_number, string errmsg) {
  ofstream ferr("runner.log", ofstream::out | ofstream::app);
  ferr << "E"<< error_number << " " << errmsg << endl;

  throw error_number;
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

static void check_user(Limits limits) {
  uid_t UID = limits.uid;
  gid_t GID = limits.gid;

  // TODO: Change current user to less privileged user.
  int t = 0;
  while(setuid(UID)) {
    if (EAGAIN == errno) {
      if (++t > 4) {
        exit(154, "Failed to set user id.");
      }
    } else if (EINVAL == errno) exit(152, "User ID specified is not valid in this user namespace");
    else if (EPERM == errno) exit(153, "User is not privileged.");
    exit(154, "Failed to set user id.");
  }

  if (0 == geteuid() || 0 == getegid()) {
    exit(151, "Cannot run as root.");
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
    rotate_log();
    set_environment_limits(limits);
    // set_redirected_files(input, output, error);
    check_user(limits);
  } catch(int error) {
    return error;
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
  switch (errno) {
    case E2BIG  :
      exit(200 + E2BIG , "The total number of bytes in the environment (envp) and argument list (argv) is too large."); break;
    case EACCES :
      exit(200 + EACCES, "Search permission is denied on a component of the path prefix of filename or the name of a script interpreter. or The file or a script interpreter is not a regular file. or Execute permission is denied for the file or a script or ELF interpreter. or The filesystem is mounted noexec."); break;
    case EAGAIN :
      exit(200 + EAGAIN, "Having changed its real UID using one of the set*uid() calls, the caller was—and is now still—above its RLIMIT_NPROC resource limit."); break;
    case EFAULT :
      exit(200 + EFAULT, "filename or one of the pointers in the vectors argv or envp points outside your accessible address space."); break;
    case EINVAL :
      exit(200 + EINVAL, "An ELF executable had more than one PT_INTERP segment (i.e., tried to name more than one interpreter)."); break;
    case EIO    :
      exit(200 + EIO   , "An I/O error occurred."); break;
    case EISDIR :
      exit(200 + EISDIR, "An ELF interpreter was a directory."); break;
    case ELIBBAD :
      exit(200 + ELIBBAD, "An ELF interpreter was not in a recognized format."); break;
    case ELOOP  :
      exit(200 + ELOOP , "Too many symbolic links were encountered in resolving filename or the name of a script or ELF interpreter."); break;
    case EMFILE :
      exit(200 + EMFILE, "The process has the maximum number of files open."); break;
    case ENAMETOOLONG :
      exit(200 + ENAMETOOLONG, "filename is too long."); break;
    case ENFILE :
      exit(200 + ENFILE, "The system limit on the total number of open files has been reached."); break;
    case ENOENT :
      exit(200 + ENOENT, "The file filename or a script or ELF interpreter does not exist, or a shared library needed for file or interpretercannot be found."); break;
    case ENOEXEC :
      exit(200 + ENOEXEC, "An executable is not in a recognized format, is for the wrong architecture, or has some other format error that means it cannot be executed."); break;
    case ENOMEM :
      exit(200 + ENOMEM, "Insufficient kernel memory was available."); break;
    case ENOTDIR :
      exit(200 + ENOTDIR, "A component of the path prefix of filename or a script or ELF interpreter is not a directory."); break;
    case EPERM :
      exit(200 + EPERM, "The filesystem is mounted nosuid, the user is not the superuser, and the file has the set-user-ID or set-group-IDbit set.or The process is being traced, the user is not the superuser and the file has the set-user-ID or set-group-ID bit set."); break;
    case ETXTBSY :
      exit(200 + ETXTBSY, "Executable was open for writing by one or more processes."); break;
    default:
      exit(250, "execve exited with unknown error code."); break;
  }

  exit(EXIT_FAILURE);
}
