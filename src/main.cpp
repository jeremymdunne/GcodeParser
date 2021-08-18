#include <iostream>

#include <cstring>

#include "GcodeParser.hpp"

int main(){
  GcodeParser parser;

  char test[] = {"X235.456"};
  _gcode_modal_t modal;
  _command_t command;
  parser.parseLine(test, strlen(test), &modal, &command );
  std::cout << "X: " << command.x << std::endl;
  std::cout << "Y: " << (float)command.y << std::endl; 
  return 0;
}
