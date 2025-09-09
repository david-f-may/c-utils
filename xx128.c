#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define XXH_INLINE_ALL // Important for including inline definitions
#include "xxhash.h"

int main(int argc, char *argv[])
{
    char* data;
    size_t data_len;
    FILE *fp;
    struct stat statbuf;
    XXH128_hash_t hash_value; // Structure to hold the 128-bit hash

    if (argc > 1)
        fp = fopen (argv[1], "rb");
    else
        fp = fopen ("sqlite3.c", "rb");
    if (fp == NULL) {
        printf ("***Error: Could not open %s\n", argc > 1 ?  argv[1] : "sqlite3.c");
        return -1;
    }
    if (argc > 1)
        stat (argv[1], &statbuf);
    else
        stat ("sqlite3.c", &statbuf);
    data = malloc ((size_t) (statbuf.st_size+1));
    if (data == NULL) {
        printf ("***Error: Could not allocate %ld bytes for sqlite3.c\n", statbuf.st_size+1);
        return -1;
    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    (void) fread(data, (size_t) statbuf.st_size, 1, fp);
#pragma GCC diagnostic pop
    data[(size_t)statbuf.st_size] = '\0';
    // Calculate the 128-bit hash
    // XXH3_128bits(input_data, data_length, seed)
    // hash_value = XXH3_128bits(data, data_len, 0); // Using a seed of 0
    hash_value = XXH128(data, statbuf.st_size, 0); // Using a seed of 0

    // Print the 128-bit hash (represented as two 64-bit unsigned integers)
    printf("%s 128-bit xxHash: %016lx%016lx\n", argc > 1 ? argv[1] : "sqlite3.c", hash_value.high64, hash_value.low64);
    fclose(fp);
    free(data);

    return 0;
}
