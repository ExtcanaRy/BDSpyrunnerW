#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uthash/uthash.h"

#define SYM_MAX_LEN 4096

#ifdef __cplusplus
extern "C" {
#endif

// Add a new sym-rva pair to the hash table
void add_entry(const char *sym, unsigned int rva);

// Find the rva of the given sym from the hash table
unsigned int get_rva_from_hashmap(const char *sym);

// Load the hash table from a file
void load_hashmap_from_file(const char *filename);

// Save the hash table to a file
void save_hashmap_to_file(const char *filename);

void free_hashmap(void);

#ifdef __cplusplus
}
#endif
