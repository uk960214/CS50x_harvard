#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Process argv. If undesired value, return 1
    string key = "";
    if (argc == 2)
    {
        if (strlen(argv[1]) == 26)
        {
            key = argv[1];
        }
        else
        {
            printf("Key must contain 26 characters\n");
            return 1;
        }
    }
    else
    {
        printf("Usage ./substitution key\n");
        return 1;
    }

    // Create key array with lowercase and uppercase
    int keylower[26];
    int keyupper[26];
    for (int i = 0; i < 26; i++)
    {
        if (isalpha(key[i]))
        {
            // If key value is not unique, return 1
            for (int j = 0; j < i; j++)
            {
                if (keylower[j] == key[i] || keyupper[j] == key[i])
                {
                    return 1;
                }
            }

            // If key value is unique, put into array
            keylower[i] = tolower(key[i]);
            keyupper[i] = toupper(key[i]);
        }
        // If key is not alphabet, return 1
        else
        {
            return 1;
        }
    }

    // Get input string
    string text = get_string("plaintext: ");
    int n = strlen(text);
    int output[n];

    for (int k = 0; k < n; k++)
    {
        if (isalpha(text[k]))
        {
            // For uppercase character, find cipher in keyupper array
            if (isupper(text[k]))
            {
                output[k] = keyupper[text[k] - 65];
            }
            // For lowercase, find cipher in keylower array
            else
            {
                output[k] = keylower[text[k] - 97];
            }
        }
        // If value not alphabet, return the original value
        else
        {
            output[k] = text[k];
        }
    }

    // Print ciphertext
    printf("ciphertext: ");
    for (int i = 0; i < n; i++)
    {
        printf("%c", output[i]);
    }
    printf("\n");
    return 0;
}