/**
 * @file newstring.h
 * @author Jakub Filo
 * @category String
 * @brief This file contains functions for working with string literal
 */
#ifndef NEWSTRING_H
#define NEWSTRING_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// The amount of memory to allocate for the dynamic string at a time
#define DYNAMIC_STRING_LEN_INC 16

// Dynamic string structure
typedef struct {
    char *str;        // Pointer to the character array (string)
    size_t length;    // Current length of the string
    size_t alloc_size; // Total allocated size for the string
} Dynamic_string;

// Function declarations
bool dynamic_string_init(Dynamic_string *s);
void dynamic_string_free(Dynamic_string *s);
bool dynamic_string_add_char(Dynamic_string *s, char c);
bool add_double_to_dynamic_string(Dynamic_string *s, double value);
void dynamic_string_clear(Dynamic_string *s);
char dynamic_string_first_char(const Dynamic_string *s);

#endif // NEWSTRING_H
