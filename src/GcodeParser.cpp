#include "GcodeParser.hpp"



GcodeParser::GcodeParser(){
  // initialize modal state
  _modal_state.feedrate = 0;
  _modal_state.tool_offset = 0;
  _modal_state.g_group_1 = 0;     // rapids
  _modal_state.g_group_2 = 17;    // XY plane
  _modal_state.g_group_3 = 90;    // absolute position
  _modal_state.g_group_5 = 94;    // unit per minute mode
  _modal_state.g_group_6 = 21;    // this is debatable
  _modal_state.g_group_7 = 40;    // cutter compenstation off
  _modal_state.g_group_8 = 43;    // tool compensation plus
  _modal_state.g_group_10 = 98;   // this currently should not be used
  _modal_state.g_group_12 = 53;   // machine coordinate system
  _modal_state.g_group_13 = 60;   // todo: look into this

  _modal_state.m_group_4 = 255;   // prevent stop
  _modal_state.m_group_6 = 255;   // prevent tool change
  _modal_state.m_group_7 = 5;     // spindle stop
  _modal_state.m_group_8 = 9;     // no coolant
  _modal_state.m_group_9 = 255;   // arguably this should be removed

}


void GcodeParser::parseLine(char* line, uint8_t length, _gcode_modal_t* modal, _command_t* command){
  // go through the line and construct the command from the provided information
  // from the command, update the modal state if necessary
  _command_t parsed_command = newCommand();
  // iterate through constructing each leter as found


  char field;
  float value;
  int end;
  uint8_t i = 0;
  while(i < length - 1){
    end = parseField(&line[i], length - i, &field, &value);
    if(end != 0){
      switch(field){
        case('A'):
          parsed_command.a = value;
          break;
        case('B'):
          parsed_command.b = value;
          break;
        case('C'):
          parsed_command.c = value;
          break;
        case('X'):
          parsed_command.x = value;
          break;
        case('Y'):
          parsed_command.y = value;
          break;
        case('Z'):
          parsed_command.z = value;
          break;
        case('I'):
          parsed_command.i = value;
          break;
        case('J'):
          parsed_command.j = value;
          break;
        case('K'):
          parsed_command.k = value;
          break;

        case('F'):
          parsed_command.f = value;
          break;
        case('S'):
          parsed_command.s = value;
          break;
        case('P'):
          parsed_command.p = (unsigned int)value;
          break;
        case('R'):
          parsed_command.r = value;
          break;
        case('T'):
          parsed_command.t = value;
          break;
        case('N'):
          parsed_command.n = value;
          break;
        case('M'):
          parsed_command.m = (uint8_t)value;
          break;
        case('G'):
          parsed_command.g = (uint8_t)value;
          break;
      }
    }
    i += end;
  }

  // quick copy
  parsed_command.y = GCODE_FLOAT_NO_VALUE;
  memcpy(command, &parsed_command, sizeof(_command_t));
  // std::cout << "Field: " << field << " Value: " << value << std::endl;


}

int GcodeParser::parseField(char* line, uint8_t length, char* letter, float* value){
  bool done = false;
  uint8_t start = 0;
  while(((int)line[start] < 65) & ((int)line[start] > 122) & (start < length)) start ++;
  *letter = line[start];
  uint8_t i = start + 1;
  // iterate until a start char is found
  while((!done) & (i < length)){
    // check for char
    if(((int)line[i] > 65) & ((int)line[i] < 122)) done = true;
    // check for comment
    if(line[i] == ';') done = true;
    // check for space
    if((int)line[i] == 32) done = true;
    if(!done) i ++;
  }
  // extract the val
  char value_str[i];
  for(uint8_t p = start; p < i - 1; p ++){
    value_str[p] = line[p + 1];
  }
  value_str[i-1] = '\0';
  *value = (float)atof(value_str);
  return i;
}


void parseModal(char* line, uint8_t length, _gcode_modal_t* modal);
