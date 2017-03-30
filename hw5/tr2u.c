//
//  tr2u.c
//  cs35_hw5
//
//  Created by Douglas Yao on 2/11/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/** Checks whether a character is in a character array and return the index of the character, otherwise -1
 */
int find_match_in_array(char* array, char to_match) {
    int i;
    for (i=0; i < strlen(array); ++i) {
        if (array[i] == to_match) {
            return i;
        }
    }
    return -1;
}

/** Checks for duplicate characters in a character array. Returns 1 if any duplicates, otherwise 0.
 */
int find_duplicates(char* array) {
    int i,j;
    for (i = 0; i < strlen(array); ++i) {
        for (j = i + 1; j < strlen(array); ++j) {
            if (array[i] == array[j]) {
                return 1;
            }
        }
    }
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Error: Must have 2 arguments\n");
        exit(1);
    }
    
    char* from = argv[1];
    char* to = argv[2];
    
    if (find_duplicates(from)) {
        fprintf(stderr, "Error: First argument must not contain duplicates\n");
        exit(1);
    }
    
    if (strlen(from) != strlen(to)) {
        fprintf(stderr, "Error: Arguments must be the same length\n");
        exit(1);
    }
    
    char current;
    ssize_t state = read(0, &current, 1);
    
    while (state > 0) {
        int position = find_match_in_array(from, current);
        if (position != -1) {
            current = to[position];
        }
        write(1, &current, 1);
        state = read(0, &current, 1);
    }
    
    return 0;
}
