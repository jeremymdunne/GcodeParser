#ifndef _GCODE_PARSER_HPP_
#define _GCODE_PARSER_HPP_

#include <cstdint>
#include <cstring>
#include <iostream>

#define GCODE_FLOAT_NO_VALUE 123456789
#define GCODE_LONG_NO_VALUE 123456789
#define GCODE_INT_NO_VALUE 255

typedef struct{
  float feedrate;
  float tool_offset;

  // G grpi[ ]
  uint8_t g_group_1;      /**< Motion: G0, G1, G2, G3, and others. */
  uint8_t g_group_2;      /**< Plane Select: G17, G18, G19. */
  uint8_t g_group_3;      /**< Distance Mode: G90, G91. */
  uint8_t g_group_5;      /**< Feedrate Mode: G93, G94. */
  uint8_t g_group_6;      /**< Unit: G20, G21. */
  uint8_t g_group_7;      /**< Cutter Compensation: G40, G41, G42. */
  uint8_t g_group_8;      /**< Tool Length Offset: G43, G49. */
  uint8_t g_group_10;     /**< Return Mode: G98, G99. */
  uint8_t g_group_12;     /**< Coordinate System Selection: G53, G54, etc. */
  uint8_t g_group_13;     /**< Path Control Mode: G61. */

  // M group modal commands. Note: a value of 255 signifies not selected
  uint8_t m_group_4;      /**< Stopping, M0, M1. */
  uint8_t m_group_6;      /**< Tool Change: M6. */
  uint8_t m_group_7;      /**< Spindle Turning: M3, M4, M5. */
  uint8_t m_group_8;      /**< Coolant: M7, M8, M9. */
  uint8_t m_group_9;      /**< Feed, Speed Override Switches: M48, M49. */

} _gcode_modal_t;


typedef struct{
  float a;
  float b;
  float c;
  float x;
  float y;
  float z;
  float i;
  float j;
  float k;

  float f;
  float s;
  unsigned long p;
  float r;
  float t;
  uint8_t m;
  uint8_t g;
  unsigned long n;
} _command_t;

_command_t newCommand(){
  _command_t command;
  command.a = GCODE_FLOAT_NO_VALUE;
  command.b = GCODE_FLOAT_NO_VALUE;
  command.c = GCODE_FLOAT_NO_VALUE;
  command.x = GCODE_FLOAT_NO_VALUE;
  command.y = GCODE_FLOAT_NO_VALUE;
  command.z = GCODE_FLOAT_NO_VALUE;
  command.i = GCODE_FLOAT_NO_VALUE;
  command.j = GCODE_FLOAT_NO_VALUE;
  command.k = GCODE_FLOAT_NO_VALUE;

  command.f = GCODE_FLOAT_NO_VALUE;
  command.s = GCODE_FLOAT_NO_VALUE;
  command.p = GCODE_LONG_NO_VALUE;
  command.r = GCODE_FLOAT_NO_VALUE;
  command.t = GCODE_FLOAT_NO_VALUE;
  command.m = GCODE_INT_NO_VALUE;
  command.g = GCODE_INT_NO_VALUE;
  command.n = GCODE_LONG_NO_VALUE;

  return command; 
}


typedef enum{
  GCODE_PARSER_OK = 0,
  GCODE_PARSER_BAD_PARSE
} _gcode_paraser_state_t;

class GcodeParser {
  public:
    /**
     * @breif Gcode Parser Constructor.
     * @details Initialize Gcode Parser object to parse lines.
     */
    GcodeParser();

    /**
     * @breif parse a single line of Gcode.
     * @details parses a line of Gcode by constructing the command and modifying the modal state.
     * @param line char* to the line of gcode.
     * @param length int length of the line of gcode (excluding null termination).
     * @param modal _gcode_modal_t* to a modal state to modify.
     * @param command _command_t* to construct.
     */
    void parseLine(char* line, uint8_t length, _gcode_modal_t* modal, _command_t* command);

    void resetModal();

    void setModal(_gcode_modal_t modal);

    _gcode_modal_t getModal();

    _gcode_paraser_state_t getState();

  private:

    _gcode_modal_t _modal_state;      /**< Running modal state of the Gcode. */

    void parseCommand(char* line, uint8_t length, _command_t* command);
    void parseModal(char* line, uint8_t length, _gcode_modal_t* modal);

    /**
     * @breif parse a single field from a gcode line.
     * @details parses a field value as a float, terminates on a new field, space, or eol.
     * @param line char* to parse, pointing to starting letter of the field.
     * @param length maximum length to parse for.
     * @param letter char* to store the parsed letter field in.
     * @param value float* to store the parsed field value in.
     * @return int char array index at end of parsing.
     */
    int parseField(char* line, uint8_t length, char* letter, float* value);
};


#endif
