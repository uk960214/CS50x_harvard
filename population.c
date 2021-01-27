#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int start_size;
    do
    {
        start_size = get_int("Please insert start size: ");
    }
    while (start_size < 9);

    int population = start_size;
    // TODO: Prompt for end size
    int end_size;
    do
    {
        end_size = get_int("Please insert end size: ");
    }
    while (end_size < start_size);
    // TODO: Calculate number of years until we reach threshold
    int year = 0;
    if (start_size == end_size)
    {
        // TODO: Print number of years if start size equals end size
        printf("Years: %i \n", year);
    }
    else
    {
        do
        {
            year ++;
            population = population + (population / 3) - (population / 4);
        }
        while (population < end_size);
        // TODO: Print number of years
        printf("Years: %i \n", year);
    }

}