#include "api.hpp"
#include <iostream>
#include <fstream>
#include <string>

extern std::string dfa2re(DFA &d);

int main() {
  std::cout<<"start"<<std::endl;
  std::ifstream infile("dfa2re.in");
  std::cout<<"start"<<std::endl;
  std::ofstream outfile("dfa2re.out");
  std::cout<<"start"<<std::endl;
  auto d = DFA::from_string(std::string((std::istreambuf_iterator<char>(infile)), 
                                   std::istreambuf_iterator<char>()));
  std::cout<<"start"<<std::endl;
  outfile << dfa2re(d);
  return 0;
}
