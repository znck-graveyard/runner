#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
  int a;
  cout << "UID:: " << getuid() << endl;
  cin >> a;
  cout << a + 1 << endl;

  return 0;
}
