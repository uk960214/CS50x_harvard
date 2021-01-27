#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get Height Input From User
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Loop Each Line
    for (int i = 0; i < height; i++)
    {
        // Print Empty Space in Front of the Blocks
        for (int j = 0; j < height - i - 1; j++)
        {
            printf(" ");
        }

        // Print Blocks for First Half
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }

        // Add Space Between
        printf("  ");

        // Print Blocks for Second Half
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}