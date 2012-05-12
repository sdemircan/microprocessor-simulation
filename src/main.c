#include <stdio.h>
#include "parser.h"
#include "memory.h"
int main(){
   FILE *file;
   char line[100];

   file = fopen("program.s", "r");
   parse(file);
   fclose(file);
   return 0;
}
