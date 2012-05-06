#include <glib.h>
#define MEMORY_SIZE 32768

typedef struct{
    int type;
    int address;
}variable;


extern int MEMORY[];
extern GHashTable* names;

extern int DATA_SEGMENT;
extern int CODE_SEGMENT;
