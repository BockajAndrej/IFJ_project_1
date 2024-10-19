// newstring.c

#include "newstring.h"
#include <stdlib.h>
#include <string.h>

// Initialize the dynamic string
bool dynamic_string_init(Dynamic_string *s) {
    s->str = (char *)malloc(DYNAMIC_STRING_LEN_INC);
    if (s->str == NULL) {
        return false; // Memory allocation failed
    }
    s->str[0] = '\0'; // Initialize as an empty string
    s->length = 0;
    s->alloc_size = DYNAMIC_STRING_LEN_INC;
    return true;
}

// Free the memory allocated for the dynamic string
void dynamic_string_free(Dynamic_string *s) {
    if (s->str != NULL) {
        free(s->str);
        s->str = NULL;
    }
    s->length = 0;
    s->alloc_size = 0;
}

// Add a character to the dynamic string, expanding its size if necessary
bool dynamic_string_add_char(Dynamic_string *s, char c) {
    // Check if we need more space
    if (s->length + 1 >= s->alloc_size) {
        size_t new_size = s->alloc_size + DYNAMIC_STRING_LEN_INC;
        char *new_str = (char *)realloc(s->str, new_size);
        if (new_str == NULL) {
            return false; // Memory reallocation failed
        }
        s->str = new_str;
        s->alloc_size = new_size;
    }
    s->str[s->length] = c;
    s->length++;
    s->str[s->length] = '\0'; // Null-terminate the string
    return true;
}

// Clear the dynamic string without freeing its memory
void dynamic_string_clear(Dynamic_string *s) {
    s->length = 0;
    if (s->str != NULL) {
        s->str[0] = '\0'; // Reset to an empty string
    }
}

// Function to get the first character of a Dynamic_string
char dynamic_string_first_char(const Dynamic_string *s) {
    if (s == NULL || s->length == 0) {
        return '\0'; // Return null character if the string is empty or NULL
    }
    return s->str[0]; // Return the first character
}

