#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


int main(void)
{
    // Get input text from user
    string t = get_string("Text: ");

    // Set counters
    int l = 0;
    int sp = 0;
    int s = 0;

    // Loop through text to count
    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (isalpha(t[i]))
        {
            l++;
        }
        if ((int) t[i] == 32)
        {
            sp++;
        }
        if ((int) t[i] == 33 || (int) t[i] == 46 || (int) t[i] == 63)
        {
            s++;
        }
    }

    int w = sp + 1;

    // Calculate Coleman-Liau index
    float L = (float) l / w * 100;
    float S = (float) s / w * 100;
    float i = 0.0588 * L - 0.296 * S - 15.8;
    int x = (int) round(i);

    // Print result
    if (x < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (x >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", x);
    }
}
