#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "md5.h"

const int PASS_LEN=50;        // Maximum any password can be
const int HASH_LEN=33;        // Length of MD5 hash strings

int file_length(char *filename)
{
    struct stat info;
    int res = stat(filename, &info);
    if (res == -1) return -1;
    else return info.st_size;
}

// Stucture to hold both a plaintext password and a hash.
struct entry 
{
    char password[50];
    char hash[33];
};



// Read in the dictionary file and return an array of structs.
// Each entry should contain both the hash and the dictionary
// word.
struct entry *read_dictionary(char *filename, int *size)
{
    int fl = file_length(filename);
    FILE *d = fopen(filename, "r");
    if (!d)
    {
        printf("Can't open %s for reading\n", filename);
        exit(1);
    }
    char *fc = malloc(fl);
    fread(fc, 1, fl, d);
    fclose(d);
    int line = 0;
    for (int i = 0; i < fl; i++)
    {
        if (fc[i] == '\n') 
        {
            fc[i] = '\0';
            line++;
        }
    }
    struct entry *dd = malloc(line * sizeof(struct entry));
    char hashedpassword[50];
    char hash[33];
    dd[0].password = password;
    strcpy(dd[0].hash, hash);
    
    
    int count = 1;
    for (int i = 0; i < fl; i++)
    {
        if (fc[i] == '\0')
        {
            char *nextdd = &fc[i] + 1;

            //sscanf(nextdd, "%s %d", dd[count].password, &dd[count].hash);
            count++;
        }
    }
    
    *size = line;
    return dd;
    free(fc);
    free(dd);
}

int ddcomp(const void *a, const void *b)
{
    struct entry *da = (struct entry *)a;
    struct entry *db = (struct entry *)b;
    return strcmp(da->password, (*db).hash);
}

int dbcomp(const void *target, const void *elem)
{
    char *target_str = (char *)target;
    struct entry *delem = (struct entry *)elem;
    return strcmp(target_str, (*delem).hash);
}

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }
    int dlen;
    char hash[33];
    char password[50];
    // Read the dictionary file into an array of entry structures
    struct entry *dict = read_dictionary(argv[2], &dlen);
    // Sort the hashed dictionary using qsort.
    // You will need to provide a comparison function.
    qsort(dict, dlen, sizeof(struct entry), ddcomp);
    // Open the hash file for reading.
    FILE *h = fopen(argv[1], "r");
    if (!h)
    {
        printf("Can't open %s for reading\n", argv[1]);
        exit(1);
    }
    // TODO
    // For each hash, search for it in the dictionary using
    // binary search.
    // If you find it, get the corresponding plaintext dictionary word.
    // Print out both the hash and word.
    // Need only one loop. (Yay!)
    
    while (fgets(hash, 33, h) != NULL)
    {
        struct entry *found = bsearch(password, dict, dlen, sizeof(struct entry), dbcomp);
        if (found == NULL)
        {
            printf("Not found\n");
        }
        else
        { 
            printf("Found: %s %s\n", hash, found->password);
        }  
    }
}
