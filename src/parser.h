#include <stdio.h>
#include <glib.h>
/*
   This tables keep parser methods which correspons to command name.
*/
static  GHashTable* command_parser;
static GHashTable* opcode;


//Parser methods
void parse(FILE *file);


//Command Parsers
void set_data_segment(char *line); //Sets DATA_SEGMENT  and returns line number of code segment's start.
void set_code_segment(char *line); //Sets DATA_SEGMENT  and returns line number of code segment's start.
