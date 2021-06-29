// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// Index by first 2 letters => 26 Letters * 26 Letters
const unsigned int N = 676;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash checking word
    int index = hash(word);

    // Move cursor to index at hash table
    node *cursor = table[index];

    // While cursor does not point to NULL, compare words in the linked list
    while (cursor)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }

    // If no match, return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // If word is longer than 1 char, hash 2 letters, else hash 1
    if (word[1])
    {
        int zero = tolower(word[0]) - 97;
        int one = tolower(word[1]) - 97;
        return zero * 26 + one;
    }
    else
    {
        return tolower(word[0]) - 97;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Prepare to add to hash table
    char word[LENGTH + 1];

    // Until dict file ends
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Allocate memory for new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        else
        {
            // Copy word into node, set next to NULL
            strcpy(n->word, word);
            n->next = NULL;
        }

        // Hash word
        int index = hash(n->word);

        // Add the new node to first item in linked list at the index
        n->next = table[index];
        table[index] = n;

        // Increment wordcount
        size();
    }

    // Close dict after reading all
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Initialize static int at first function call
    static int count = 0;

    // After returning current count, increment count
    return count++;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor, *tmp;
    int i;
    int length = N;

    // Loop hash table
    for (i = 0; i < length; i++)
    {
        // Set cursor to point to first item in each linked list
        cursor = table[i];
        while (cursor)
        {
            // Set tmp to point at what cursor is pointing
            tmp = cursor;
            // Move cursor to next item in linked list
            cursor = cursor->next;
            // Free temp
            free(tmp);
        }
    }
    return true;
}
