// Usage: ./mycat < path/to/file

#include <iostream>

int main() {
  using namespace std;
  char ch;
  cin.get(ch);
  while (cin.fail() == false) {
    cout << ch;
    cin.get(ch);
  }

  return 0;
}
