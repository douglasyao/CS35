//
//  sfrobu.c
//  UCLA CS35L Winter 2017
//
//  Created by Douglas Yao on 2/12/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


/** Compares two frobnicated character arrays
 */
int frobcmp(const char *a, const char *b) {
    for ( ;; a++, b++) {
        if(*a == ' ' && *b == ' ') {
            return 0;
        }
        else if (*a == ' ' || ((*a^42) < (*b^42))) {
            return -1;
        }
        else if (*b == ' ' || ((*a^42) > (*b^42))) {
            return 1;
        }
    }
}

/** Compares two frobnicated character arrays ignoring case
 */
int frobcmp_toupper(const char *a, const char *b) {
    if ((*a^42) > UCHAR_MAX || (*a^42) < 0 || (*b^42) > UCHAR_MAX || (*b^42) < 0 ) {
        fprintf(stderr, "Input to toupper out of range");
        exit(1);
    }
    for ( ;; a++, b++) {
        if(*a == ' ' && *b == ' ') {
            return 0;
        }
        else if (*a == ' ' || (toupper(*a^42) < toupper(*b^42))) {
            return -1;
        }
        else if (*b == ' ' || (toupper(*a^42) > toupper(*b^42))) {
            return 1;
        }
    }
}


/** qsort requires function to take arguments of const void * type.
 */
int qsort_frobcmp(const void *a, const void *b) {
    const char *ia = *(const char **)a;
    const char *ib = *(const char **)b;
    return frobcmp(ia, ib);
}


/** toupper version of the previous function
 */
int qsort_frobcmp_toupper(const void *a, const void *b) {
    const char *ia = *(const char **)a;
    const char *ib = *(const char **)b;
    return frobcmp_toupper(ia, ib);
}



/** Check if state returned by read() is invalid
 */
void check_read_error(ssize_t state) {
    if (state < 0) {
        fprintf(stderr, "Error reading file\n");
        exit(1);
    }
}


/** Check if pointer returned by malloc or realloc is NULL
 */
void check_memory_error(void* pointer) {
    if (pointer == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
}


int main(int argc, char* argv[]) {
    
    // check arguments
    if (argc > 2) {
        fprintf(stderr, "Maximum of one argument\n");
        exit(1);
    }
    
    int toupper = 0;
    if (argc == 2) {
        if (strcmp(argv[1], "-f") == 0) {
            toupper = 1;
        }
        else {
            fprintf(stderr, "Argument can only be '-f'\n");
            exit(1);
        }
    }
    
    
    // get info about stdin
    struct stat filestat;
    if (fstat(0, &filestat) < 0) {
        fprintf(stderr, "Error calling fstat\n");
        exit(1);
    }
    
    // initialize variables
    char* all_words_together;
    char** all_words;
    
    // allocate initial memory for entire file contents and read in file
    if (filestat.st_size > 0) {
        all_words_together = (char*)malloc(filestat.st_size + 1); // allocate additional byte in case we need to append a space
        check_memory_error(all_words_together);
        ssize_t state = read(0, all_words_together, filestat.st_size);
        check_read_error(state);
    }
    else {
        all_words_together = (char*)malloc(1);
    }
    
    // if file is growing, continue to read in file and allocate memory
    char current;
    ssize_t gstate = read(0, &current, 1);
    int additional_chars = 0;
    while (gstate > 0) {
        char* temp = realloc(all_words_together, filestat.st_size + additional_chars + 2);
        check_memory_error(temp);
        all_words_together = temp;
        all_words_together[filestat.st_size + additional_chars] = current;
        ++additional_chars;
        gstate = read(0, &current, 1);
    }
    check_read_error(gstate);
    
    
    // allocate memory for array of pointers to character arrays
    int wordcount = 0;
    int i;
    if (filestat.st_size + additional_chars == 1) {
        if (all_words_together[0] != ' ') {
            ++wordcount;
        }
    }
    else if (filestat.st_size + additional_chars == 2) {
        if (all_words_together[0] != ' ' || all_words_together[1] != ' ') {
            ++wordcount;
        }
    }
    else {
        for (i = 0; i < filestat.st_size + additional_chars - 1; ++i) {
            if (all_words_together[i] != ' ' && (all_words_together[i+1] == ' ' || i == filestat.st_size + additional_chars - 2)) {
                ++wordcount;
            }
        }
    }
    all_words = (char**)malloc(wordcount * sizeof(char*));
    check_memory_error(all_words);
    
    
    // if file is empty
    if (wordcount == 0) {
        free(all_words_together);
        return 0;
    }
    
    int word_index = 0;
    
    // if file contains only one character
    if (filestat.st_size + additional_chars == 1) {
        all_words[0] = all_words_together;
        all_words_together[1] = ' ';
        ++word_index;
    }
    else { // populate all_words with pointers to beginning of words
        char* curr = &all_words_together[0];
        char* next = &all_words_together[1];
        if (*curr != ' ') {
            all_words[word_index] = curr;
            ++word_index;
        }
        
        while (next != &all_words_together[filestat.st_size + additional_chars]) {
            if (*curr == ' ' && *next != ' ') {
                all_words[word_index] = next;
                ++word_index;
            }
            ++curr; ++next;
        }
        
        if (*next != ' ') {
            all_words_together[filestat.st_size + additional_chars] = ' ';
        }
    }
    
    // sort words
    if (toupper) {
        qsort(all_words, word_index, sizeof(char*), qsort_frobcmp_toupper);
    }
    else {
        qsort(all_words, word_index, sizeof(char*), qsort_frobcmp);
    }
    
    // output words to stdout
    int j;
    for (j = 0; j < word_index; ++j) {
        char* curr_char = all_words[j];
        while (*curr_char != ' ') {
            write(1, curr_char, 1);
            ++curr_char;
        }
        char space = ' ';
        write(1, &space, 1);
    }
    
    // deallocate memory
    free(all_words);
    free(all_words_together);
    return 0;
    
}
