#include <fstream>
#include <iostream>

int main() {
    std::cout <<"Output File Streams" << std::endl;
  /// associating file on construction
  std::ofstream ofs("hello.txt");
  if (ofs.is_open()) {
    ofs << "Hello CS106L !" << '\n';
  }
  ofs.close();
  ofs << "this will not get written";

  /* try adding a 'mode' argument to the open method, like std::ios:app
   * What happens?
   */
  ofs.open("hello.txt");
  ofs << "this will though! It’s open again";
  return 0;
}