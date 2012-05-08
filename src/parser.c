#include <stdio.h>
#include <string.h>
#include "parser.h"
#include <stdlib.h>
#include "memory.h"

//Declaration of extern memory variables
GHashTable* names;
int MEMORY[MEMORY_SIZE];
int DATA_SEGMENT;
int CODE_SEGMENT;

int data_segment_counter = -1;
int code_segment_counter = 0;

void find_name(char *current_command, char *line){
    for(int i = 0; i < strlen(line); i++){
        if (line[i] != ' ' && line[i] != '\n'){
             current_command[i] = line[i];
	}else{
            current_command[i] = '\0';
	    break;
	}
    }
}

void init_opcodes(){
    opcode = g_hash_table_new(g_str_hash, g_int_hash);
    g_hash_table_insert(opcode, "MOVE", 0x00000000);
    g_hash_table_insert(opcode, "MOVE", 0x01000000);
    g_hash_table_insert(opcode, "IN",   0x02000000);
    g_hash_table_insert(opcode, "OUT",  0x03000000);
    g_hash_table_insert(opcode, "ADD",  0x04000000);
    g_hash_table_insert(opcode, "SUB",  0x05000000);
    g_hash_table_insert(opcode, "AND",  0x06000000);
    g_hash_table_insert(opcode, "OR",   0x07000000);
    g_hash_table_insert(opcode, "NEG",  0x08000000);
    g_hash_table_insert(opcode, "ASL",  0x09000000);
    g_hash_table_insert(opcode, "ASR",  0x0A000000);
    g_hash_table_insert(opcode, "CMP",  0x0B000000);
    g_hash_table_insert(opcode, "MOVE", 0x0C000000);
    g_hash_table_insert(opcode, "BEQ",  0x0D000000);
    g_hash_table_insert(opcode, "BNE",  0x0E000000);
    g_hash_table_insert(opcode, "BCC",  0x0F000000);
    g_hash_table_insert(opcode, "BCS",  0x10000000);
    g_hash_table_insert(opcode, "BRA",  0x11000000);
    g_hash_table_insert(opcode, "STOP", 0x12000000);
    printf("Opcode table initialized...\n");
}


void init_command_parser(){
    command_parser = g_hash_table_new(g_str_hash, g_int_hash);
}

void next(int start_index, char *line, char *current_token){
    for(int i = start_index; i < strlen(line); i++){
        if (line[i] != ' ' && line[i] != '\n'){
             current_token[i-start_index] = line[i];
	}else{
            current_token[i-start_index] = '\0';
	    return;
	}
    }
}

void set_data_segment(char *line){
    char address[10];
    next(4, line, address);
    DATA_SEGMENT = get_value(address); 
}

void set_code_segment(char *line){
    char address[10];
    next(4, line, address);
    CODE_SEGMENT = get_value(address); 
}

int get_value(char *str){
    return atoi(str);
}

void initialize_names(){
    names = g_hash_table_new(g_str_hash, g_int_hash);
}

int  find_variables(FILE *file){//TODO Now only supported variable type is integer and code should be better.
    printf("Finding variables... \n");
   
    initialize_names(); 
    char line[100];
    char current_token[20];
    char variable_name[20];
    int position;
    
    while (fgets(line, 100, file) != NULL){
        position = 0;
  	find_name(current_token, line);
        position += strlen(current_token) + 1;
        if (strcmp(current_token, "ORG") == 0){
	    switch(data_segment_counter){
		case -1:
	    	    set_data_segment(line);
	    	    printf("Data segment starts at: 0x%x\n", DATA_SEGMENT);
		    break;
		default:
		    set_code_segment(line);
	    	    printf("Code segment starts at: 0x%x\n", CODE_SEGMENT);
		    return 1;
	    }
            data_segment_counter++;
	}else{
            strcpy(variable_name, current_token);
	    next(position, line, current_token);
            position += strlen(current_token) + 1;
	    next(position, line, current_token);
 	    int value = get_value(current_token);
	    
            MEMORY[CODE_SEGMENT + data_segment_counter] = value;
            g_hash_table_insert(names, variable_name, CODE_SEGMENT + data_segment_counter);
            printf("Variable %s at %x\n", variable_name, CODE_SEGMENT + data_segment_counter);
            data_segment_counter++;
	} 	
    }
}

void find_labels(FILE *file){
    printf("Finding labels...\n");
    char line[100];
    char current_token[20];
    int address;
   
    while (fgets(line, 100, file) != NULL){
  	find_name(current_token, line);
	int found_in_names = g_hash_table_lookup(names, current_token);
        if (found_in_names == 0){
	    address = DATA_SEGMENT + data_segment_counter;
            g_hash_table_insert(names, current_token, address);
	}
        code_segment_counter++;
    }
    
 	
}

void parse(FILE *file){
   init_opcodes();

   if (file == NULL){
        printf ("Cannot open file! \n");
   }else{
            find_variables(file);
            find_labels(file);
   }
}


