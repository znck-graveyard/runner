#ifndef RUNNER_HELPERS
#define RUNNER_HELPERS

#include <iostream>
#include <string>
#include <cstdlib>

#include <getopt.h>

#include <limits.h>

using std::string;

void die(int, string);

void help(bool);

long long num(char *);

int parse_args(int, char *argv[], Limits &, string &);

string pathFromFd(int);

#endif
