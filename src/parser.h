#include <stdio.h>
#include <glib.h>
/*
   This tables keep parser methods which correspons to command name.
*/
static  GHashTable* command_parser;
static GHashTable* opcode;

int get_value(char *str);
//Parser methods
void find_name(char *current_command, char *line);
void parse(FILE *file);
int find_variables(); //Puts variables in memory.
void find_labels();


//Command Parsers
void set_data_segment(char *line); //Sets DATA_SEGMENT  and returns line number of code segment's start.
