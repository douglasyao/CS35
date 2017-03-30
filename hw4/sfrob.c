//
//  sfrob.c
//  UCLA CS35L Winter 2017
//
//  Created by Douglas Yao on 2/3/17.
//

#include <stdio.h>
#include <stdlib.h>


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

/** qsort requires function to take arguments of const void * type.
 */
int qsort_frobcmp(const void *a, const void *b) {
    const char *ia = *(const char **)a;
    const char *ib = *(const char **)b;
    return frobcmp(ia, ib);
}

/** Check for stdin read errors
 */
void check_read_error() {
    if (ferror(stdin)) {
        fprintf(stderr, "Read error");
        exit(1);
    }
}


/**  Appends character to existing character array and allocates the required memory. Checks for memory allocation errors.
 */
char* append_char_to_word(char* word, char character, int char_index) {
    char* temp = realloc(word, (char_index+1)*(sizeof(char)));
    if (temp == NULL) {
        free(word);
        fprintf(stderr, "Memory allocation error");
        exit(1);
    }
    word = temp;
    word[char_index] = character;
    return word;
}


/**  Appends character array to existing array of character arrays and allocates the required memory. 
 Checks for memory allocation errors.
 */
char** append_word_to_words(char** words, char* word, int word_index) {
    char** temp = realloc(words, (word_index+1)*(sizeof(char*)));
    if (temp == NULL) {
        size_t i;
        for (i = 0; i < word_index; ++i) {
            free(words[i]);
        }
        free(words);
        fprintf(stderr, "Memory allocation error");
        exit(1);
    }
    words = temp;
    words[word_index] = word;
    return words;
}


int main() {
    
    // initialize variables
    char curr_char;
    char next_char;
    char* curr_word;
    curr_word = (char*)malloc(sizeof(char));
    char** all_words;
    all_words = (char**)malloc(sizeof(char*));
    
    int char_index = 0;
    int word_index = 0;
    
    // read in characters from stdin. The reason why we store both the current character and the
    // next character is so that we can detect and remove multiple spaces in a row.
    curr_char = getchar();
    check_read_error();
    
    // if file is empty
    if (curr_char == EOF) {
        free(curr_word);
        free(all_words);
        return 0;
    }
    
    next_char = getchar();
    check_read_error();
    char space = ' ';
    
    while (curr_char != EOF) {
        
        // if both current character and next character are space, then continue to read in
        // characters until a non-space character is reached
        if (curr_char == ' ' && next_char == ' ') {
            while (next_char == ' ') {
                curr_char = next_char;
                next_char = getchar();
                check_read_error();
            }
        }
        
        // if current character is not a space, add the character to the word
        if (curr_char != ' ') {
            curr_word = append_char_to_word(curr_word, curr_char, char_index);
            ++char_index;
            
            // if current character is the last character in the file, append a space
            if (next_char == EOF) {
                curr_word = append_char_to_word(curr_word, space, char_index);
                all_words = append_word_to_words(all_words, curr_word, word_index);
                break;
            }
            
            curr_char = next_char;
            next_char = getchar();
            check_read_error();
        }
        
        // if the current character is a space and not the last character in the file
        else if (curr_char == ' ' && next_char != EOF) {
            
            // if file starts with spaces, then continue reading
            if (word_index == 0 && char_index == 0) {
                curr_char = next_char;
                next_char = getchar();
                check_read_error();
            }
            
            // add space to current word and and add current word to all words
            else {
                curr_word = append_char_to_word(curr_word, space, char_index);
                char_index = 0;
                all_words = append_word_to_words(all_words, curr_word, word_index);
                ++word_index;
                char* new_word;
                new_word = (char*)malloc(sizeof(char));
                curr_word = new_word;
                curr_char = next_char;
                next_char = getchar();
                check_read_error();
            }
        }
        
        // if the current character is a space and the last character in the file
        else if (curr_char == ' ' && next_char == EOF) {
            
            // if file consists of only spaces
            if (word_index == 0 && char_index == 0) {
                free(curr_word);
                free(all_words);
                return 0;
            }
            else {
                curr_word = append_char_to_word(curr_word, curr_char, char_index);
                all_words = append_word_to_words(all_words, curr_word, word_index);
                break;
            }
        }
    }
    
    // sort words
    qsort(all_words, (word_index + 1), sizeof(char*), qsort_frobcmp);
    
    // output words to stdout
    size_t i;
    for (i = 0; i <= word_index; ++i) {
        size_t j;
        for (j = 0;; ++j) {
            putchar(all_words[i][j]);
            if (ferror(stdout)) {
                size_t k;
                for (k = 0; k < word_index; ++k) {
                    free(all_words[k]);
                }
                free(all_words);
                fprintf(stderr, "Output error");
                exit(1);
            }
            if (all_words[i][j] == ' ') {
                break;
            }
        }
    }
    
    // deallocate memory
    size_t k;
    for (k = 0; k <= word_index; ++k) {
        free(all_words[k]);
    }
    free(all_words);
    return 0;

}
