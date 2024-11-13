/**
 * @file ADT.h
 * @author Pavel Glvač <xglvacp00>
 * @category Abstract data types
 * @brief Header file for the Abstract data type
 */

#ifndef ADT_H
#define ADT_H

#define TABLE_SIZE 100  ///< Size of the hash table

/**
 * @struct Symbol
 * @brief Represents a symbol entry in the hash table.
 */
typedef struct Symbol {
    char *name;                     ///< Key for the symbol (symbol's name)
    char *type;                     ///< Type of the symbol
    int intValue;                   ///< Integer value of the symbol
    float floatValue;               ///< Floating-point value of the symbol
    unsigned int address;           ///< Address of the symbol
    unsigned int hasIntValue : 1;   ///< Flag indicating if intValue is valid
    unsigned int hasFloatValue : 1; ///< Flag indicating if floatValue is valid
    struct Symbol *next;            ///< Pointer to the next symbol in case of a collision
} Symbol;

/**
 * @struct HashTable
 * @brief Represents the hash table for storing symbols.
 */
typedef struct HashTable {
    Symbol *table[TABLE_SIZE];
} HashTable;

/**
 * @brief Dynamically copies a string.
 * @param str Pointer to the string to copy.
 * @return A dynamically allocated copy of the string, or NULL if memory allocation fails.
 */
char *copy_string(const char *str);

/**
 * @brief Frees a dynamically allocated string.
 * @param str Pointer to the string to free.
 */
void free_string(char *str);

/**
 * @brief Hashes a string to an index in the table.
 * @param key Pointer to the string to hash.
 * @return Hash value as an index within the table.
 */
unsigned int hash(const char *key)   ;                 

/**
 * @brief Creates a new hash table.
 * @return Pointer to a newly allocated HashTable, or NULL if allocation fails.
 */
HashTable *create_table();                       

/**
 * @brief Inserts a symbol into the hash table.
 * 
 * @param table Pointer to the hash table.
 * @param name Symbol's name.
 * @param type Symbol's type.
 * @param intValue Integer value of the symbol (if applicable).
 * @param hasInt Indicates if intValue is valid.
 * @param floatValue Floating-point value of the symbol (if applicable).
 * @param hasFloat Indicates if floatValue is valid.
 * @param address Memory address of the symbol.
 * 
 * TODO: Handle cases where symbols with the same name already exist in the table.
 */
void insert_symbol(HashTable *table, const char *name, const char *type, int intValue, int hasInt, float floatValue, int hasFloat, unsigned int address);  // Vloženie symbolu do tabuľky

/**
 * @brief Deletes a symbol from the hash table by name.
 * 
 * @param table Pointer to the hash table.
 * @param name Name of the symbol to delete.
 */
void delete_symbol(HashTable *table, const char *name);

/**
 * @brief Searches for a symbol in the hash table by name.
 * 
 * @param table Pointer to the hash table.
 * @param name Name of the symbol to search for.
 * @return Pointer to the Symbol if found, or NULL if not found.
 */
Symbol *search_symbol(HashTable *table, const char *name);  

/**
 * @brief Frees all memory associated with the hash table.
 * 
 * @param table Pointer to the hash table to free.
 */
void free_table(HashTable *table);
#endif