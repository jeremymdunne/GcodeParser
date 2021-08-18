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
  parseCommand(line, length, &parsed_command);
  // quick copy
  memcpy(command, &parsed_command, sizeof(_command_t));
  // std::cout << "Field: " << field << " Value: " << value << std::endl;
}

void GcodeParser::parseCommand(char* line, uint8_t length, _command_t* command){
    // go through the line and parse all fields
    char field;
    float value;
    int end;
    uint8_t i = 0;
    while(i < length - 1){
      end = parseField(&line[i], length - i, &field, &value);
      if(end != 0){
        switch(field){
          case('A'):
            command->a = value;
            break;
          case('B'):
            command->b = value;
            break;
          case('C'):
            command->c = value;
            break;
          case('X'):
            command->x = value;
            break;
          case('Y'):
            command->y = value;
            break;
          case('Z'):
            command->z = value;
            break;
          case('I'):
            command->i = value;
            break;
          case('J'):
            command->j = value;
            break;
          case('K'):
            command->k = value;
            break;
          case('F'):
            command->f = value;
            break;
          case('S'):
            command->s = value;
            break;
          case('P'):
            command->p = (unsigned int)value;
            break;
          case('R'):
            command->r = value;
            break;
          case('T'):
            command->t = value;
            break;
          case('N'):
            command->n = value;
            break;
          case('M'):
            command->m = (uint8_t)value;
            break;
          case('G'):
            command->g = (uint8_t)value;
            break;
        }
      }
      i += end;
    }
}


void GcodeParser::updateModalState(_command_t* command){
  // check for M and G states
  if(command->m != GCODE_INT_NO_VALUE){
    switch(command->m){
      // M Group 4
      case(0):
        _modal_state.m_group_4 = 0;
        break;
      case(1):
        _modal_state.m_group_4 = 1;
        break;
      // M Group 6

      // M Group 7
      case(3):
        _modal_state.m_group_7 = 3;
        break;
      case(4):
        _modal_state.m_group_7 = 4;
        break;
      case(5):
        _modal_state.m_group_7 = 5;
        break;

      // M Group 8

      // M Group 9
    }
  }
  else if(command->g != GCODE_INT_NO_VALUE){
    switch(command->g){
      // G Group 1
      case(0):
        _modal_state.g_group_1 = 0;
        break;
      case(1):
        _modal_state.g_group_1 = 1;
        break;
      case(2):
        _modal_state.g_group_1 = 2;
        break;
      case(3):
        _modal_state.g_group_1 = 3;
        break;

      // G Group 2
      case(17):
        _modal_state.g_group_2 = 17;
        break;
      case(18):
        _modal_state.g_group_2 = 18;
        break;
      case(19):
        _modal_state.g_group_2 = 19;
        break;

      // G Group 3
      case(90):
        _modal_state.g_group_3 = 90;
        break;
      case(91):
        _modal_state.g_group_3 = 91;
        break;

      // G Group 5
      case(20):
        _modal_state.g_group_5 = 20;
        break;
      case(21):
        _modal_state.g_group_5 = 21;
        break;

      // G Group 7

      // G Group 8

      // G Group 10

      // G Group 12

      // G Group 13
    }
  }
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
