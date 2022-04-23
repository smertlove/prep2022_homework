#include <malloc.h>
#include <stdbool.h>


#include "custom_string.h"



string_t* init_string(void) {
	string_t *string = calloc(1, sizeof(string_t));
	string->size = 0;
	string->memory = 0;
	return string;
}

void add_char (string_t *string, char ch) {
	if (ch == '\r') return;
	++string->size;
	if (string->data == NULL) {
		string->data = calloc(10, string->size);
		string->memory = 10;
	} else if (string->size > string->memory) {
		string->memory *= string->memory;
		char *tmp = realloc(string->data, string->memory);
		string->data = tmp;
	}
	string->data[string->size - 1] = ch;
}

void free_string(string_t *string){
    if (string != NULL && string->data != NULL)
    {
    free(string->data);
    }
    free(string);
}