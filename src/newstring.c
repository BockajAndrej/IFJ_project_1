/**
 * @file newstring.c
 * @author Jakub Filo
 * @category String
 * @brief This file contains functions for working with string literal
 */
#include "newstring.h"
#include <stdlib.h>
#include <string.h>

// Initialize the dynamic string
bool dynamic_string_init(Dynamic_string *s)
{
    s->str = (char *)malloc(DYNAMIC_STRING_LEN_INC);
    if (s->str == NULL)
    {
        return false; // Memory allocation failed
    }
    s->str[0] = '\0'; // Initialize as an empty string
    s->length = 0;
    s->alloc_size = DYNAMIC_STRING_LEN_INC;
    return true;
}

// Free the memory allocated for the dynamic string
void dynamic_string_free(Dynamic_string *s)
{
    if (s->str != NULL)
    {
        free(s->str);
        s->str = NULL;
    }
    s->length = 0;
    s->alloc_size = 0;
}

// Add a character to the dynamic string, expanding its size if necessary
bool dynamic_string_add_char(Dynamic_string *s, char c)
{
    // Check if we need more space
    if (s->length + 1 >= s->alloc_size)
    {
        size_t new_size = s->alloc_size + DYNAMIC_STRING_LEN_INC;
        char *new_str = (char *)realloc(s->str, new_size);
        if (new_str == NULL)
        {
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

// Function to convert double to Dynamic_string
bool add_double_to_dynamic_string(Dynamic_string *s, double value)
{
    char buffer[32]; // Buffer to hold the string representation of the double

    // Convert the double to string using %.15g for maximum precision and shortest representation
    snprintf(buffer, sizeof(buffer), "%.15g", value);

    // Check if the value has a fractional part
    if (value == (int)value)
    {
        // If it's an integer, remove the decimal point
        snprintf(buffer, sizeof(buffer), "%.0f", value);
    }
    else
    {
        // Otherwise, use the standard formatting
        snprintf(buffer, sizeof(buffer), "%.6f", value); // Or %.15g for shorter representation
    }

    // Add each character of the string to the dynamic string
    for (size_t i = 0; buffer[i] != '\0'; i++)
    {
        if (!dynamic_string_add_char(s, buffer[i]))
        {
            return false; // Failed to add character
        }
    }

    return true; // Successfully added the double value
}

// Clear the dynamic string without freeing its memory
void dynamic_string_clear(Dynamic_string *s)
{
    s->length = 0;
    if (s->str != NULL)
    {
        s->str[0] = '\0'; // Reset to an empty string
    }
}

// Function to get the first character of a Dynamic_string
char dynamic_string_first_char(const Dynamic_string *s)
{
    if (s == NULL || s->length == 0)
    {
        return '\0'; // Return null character if the string is empty or NULL
    }
    return s->str[0]; // Return the first character
}
