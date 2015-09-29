#include <analyzer.h>

using namespace std;

Analyzer::Analyzer() {}

void Analyzer::dump(struct rusage &usage, string filename) {
  ofstream fout(filename.c_str());
  user_time = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1000000.0,
  sys_time = usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1000000.0;

  fout << "USER_TIME=" << user_time << endl;
  fout << "SYS_TIME=" << sys_time << endl;
  fout << "TOTAL_TIME=" << user_time + sys_time << endl;
  fout << "MEMORY=" << usage.ru_maxrss << endl;
  fout << "MINOR_FAULTS=" << usage.ru_minflt << endl;
  fout << "MAJOR_FAULTS=" << usage.ru_majflt << endl;
  fout << "INPUT_COUNT=" << usage.ru_inblock << endl;
  fout << "OUTPUT_COUNT=" << usage.ru_oublock << endl;
  fout << "VOLUNTRY_CONTEXT_SWITCH=" << usage.ru_nvcsw << endl;
  fout << "INVOLUNTRY_CONTEXT_SWITCH=" << usage.ru_nivcsw << endl;
}
