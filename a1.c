#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Print all the words in the word list to a single line with the appropriate number of spaces
 * char **wordList - The list of words for the current line (pointer to array of pointers)
 * int charsPerLine - The exact number of characters the line should have
 * int wordCount - The number of words for the current line
 * int charCount - The number of characters from the words for the current line
 */
void printLine(char **wordList, int charsPerLine, int wordCount, int charCount) {    
    int spacesToInsert = charsPerLine - charCount;

    if (wordCount == 1) { 
        // Print spaces to the left of the word
        // If the number of spaces is odd, add one extra space here
        printf("%*s", ((spacesToInsert/2) + (spacesToInsert%2)), "");
        // Print the word
        printf("%s", wordList[0]);
        // Print spaces to the right of the word
        printf("%*s\n", (spacesToInsert/2), "");
        free(wordList[0]); // free memory for current word
    } else {
        // Starting from after the frist word, print spaces between the words
        // If the total number of spaces does not evenly divide into (words-1),
        // then add one space character with each space print, 
        // until all extra spaces are distributed

        int extraSpaces = spacesToInsert%(wordCount-1);
        int numSpaces = spacesToInsert/(wordCount-1);

        for (int i = 0; i < wordCount; i++) {
            if (i > 0) {                    
                printf("%*s", (numSpaces + (extraSpaces-- > 0 ? 1 : 0)), "");
            }
            printf("%s", wordList[i]);
            free(wordList[i]); // free memory for current word
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <characters_per_line> <input_file>\n", argv[0]);
        return 1;
    }

    int charsPerLine = atoi(argv[1]);
    char *inputFileName = argv[2];
    
    FILE *file = fopen(inputFileName, "r");
    if (file == NULL) {
        perror("Error opening input file");
        return 1;
    }

    char inputChar;
    char *inputWord = malloc(1000); // Set buffer for a 1000 character word
    char **wordList;
    int words = 2000;
    wordList = malloc(words * sizeof(char *)); // Set buffer for 2000 words
    int wordCount = 0; 
    int charCount = 0;
    int currentCharCount = 0;
    int setForceNewLine = 0;
    int forceNewline = 0;

    while ((inputChar = fgetc(file)) != EOF) {
        if (inputChar == '-') {
            // A hyphen ends the current line
            // add the string terminator
            // print the current line

            inputWord[currentCharCount++] = inputChar;
            inputWord[currentCharCount] = '\0'; // null terminator

            // Indicate next word should be on a new line
            setForceNewLine = 1; // use it as boolean
        } else if (inputChar != ' ' && inputChar != '\n' && inputChar != '\r') {
            // Current character is not a space or newline
            // add to current word
            // do not trigger printing of the current line 

            inputWord[currentCharCount++] = inputChar;
            continue; // go back to while (next iteration)
        } else {
            // Current character is a space or newline
            // add string terminator to current word
            // print the current line

            inputWord[currentCharCount] = '\0'; // null terminator (end of the string (one word))
        }
        // The word is complete, reset the current character count (pointer)
        currentCharCount = 0;
        
        int inputWordLength = strlen(inputWord); // checking the lenth of the current word
        if (inputWordLength > charsPerLine) { // charsperline is provided by user in the beginninhg
            printf("Error. The word processor can't display the output");
            return 1;
        }

        // If the current word can't fit in the current line, or a new line is forced (forceNewline is 1 - we need to print the current line), 
        // print the current line.
        // Compare characters per line with the sum of:
        // - number of characters from words for the current line
        // - number of spaces for the current line (number of words - 1)
        // - the length of the new word + 1 to include its space

        // Relationship between number of words and minimum number of spaces per line:
        // 1 word = add 0 spaces in between
        // 2 words = add 1 space in between
        // n words = add n-1 spaces in between
        if (forceNewline || charCount + (inputWordLength+1) + (wordCount-1) > charsPerLine) {
            // Reset forceNewLine to false
            forceNewline = 0;

            // Print current line
            printLine(wordList, charsPerLine, wordCount, charCount);

            // Reset character and word counts for current line
            charCount = 0;
            wordCount = 0;
        }

        // Allocate memory to fit current input word
        wordList[wordCount] = malloc(inputWordLength * sizeof(char));
        // Add input word to wordList then increment the word count
        strcpy(wordList[wordCount++], inputWord);
        // Increment the character count by the length of the input word
        charCount += inputWordLength;
        
        if (setForceNewLine) {
            forceNewline = 1;
            setForceNewLine = 0;
        }
    }

    // Print out any remaining words in the word list
    printLine(wordList, charsPerLine, wordCount, charCount);
    free(wordList); // free memory for word list
    free(inputWord); // free memory for input word

    char *ANum = "A01346537"; // A numbers of everyone. AXXXX_AXXXX_AXXX format.

    FILE *outputFile = fopen(ANum, "w");

    if (outputFile == NULL) {
         printf("Failed to create the output file.\n");
         return 1;
    }

fclose(outputFile);

    return 0;
}

